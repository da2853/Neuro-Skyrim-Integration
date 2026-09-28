#pragma once

// GameFoundry fork additions: pure logic with no RE/SKSE dependency, so it can be
// unit-tested with a host compiler (gamefoundry/tests/test_core.cpp).
// See GAMEFOUNDRY.md for the wire format and settings.

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdint>
#include <istream>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace gamefoundry::core
{
    inline constexpr std::string_view kDefaultWsUrl = "ws://localhost:8000";
    inline constexpr std::string_view kInputEchoPrefix = "[gf:input/v1]";
    inline constexpr std::string_view kIniFileName = "_neuroSkyrim.ini";

    inline std::string_view trim(std::string_view s)
    {
        constexpr std::string_view ws = " \t\r\n";
        const auto b = s.find_first_not_of(ws);
        if (b == std::string_view::npos)
            return {};
        const auto e = s.find_last_not_of(ws);
        return s.substr(b, e - b + 1);
    }

    inline bool iequals(std::string_view a, std::string_view b)
    {
        return a.size() == b.size() &&
               std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) {
                   return std::tolower(static_cast<unsigned char>(x)) ==
                          std::tolower(static_cast<unsigned char>(y));
               });
    }

    // Value of `key = value` anywhere in an ini stream (section-agnostic, key is
    // case-insensitive, `;` and `#` start a comment line). Last occurrence wins.
    inline std::optional<std::string> ini_value(std::istream& in, std::string_view key)
    {
        std::optional<std::string> out;
        std::string line;
        while (std::getline(in, line)) {
            const auto t = trim(line);
            if (t.empty() || t.front() == ';' || t.front() == '#' || t.front() == '[')
                continue;
            const auto eq = t.find('=');
            if (eq == std::string_view::npos)
                continue;
            if (iequals(trim(t.substr(0, eq)), key))
                out = std::string(trim(t.substr(eq + 1)));
        }
        return out;
    }

    inline bool is_ws_url(std::string_view s)
    {
        return (s.starts_with("ws://") && s.size() > 5) || (s.starts_with("wss://") && s.size() > 6);
    }

    // Patch 0 precedence: environment (NEURO_SDK_WS_URL) > ini (wsUrl) > default.
    // Invalid values fall through to the next source.
    inline std::string resolve_ws_url(const char* env, const std::optional<std::string>& ini)
    {
        if (env) {
            const auto e = trim(env);
            if (is_ws_url(e))
                return std::string(e);
        }
        if (ini && is_ws_url(trim(*ini)))
            return std::string(trim(*ini));
        return std::string(kDefaultWsUrl);
    }

    inline std::optional<long> parse_long(std::string_view s)
    {
        s = trim(s);
        long v = 0;
        const auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), v);
        if (ec != std::errc{} || p != s.data() + s.size())
            return std::nullopt;
        return v;
    }

    // Value from env if set and valid, else ini, else fallback; clamped to [lo, hi].
    inline long resolve_long(const char* env, const std::optional<std::string>& ini, long fallback, long lo, long hi)
    {
        std::optional<long> v;
        if (env)
            v = parse_long(env);
        if (!v && ini)
            v = parse_long(*ini);
        return std::clamp(v.value_or(fallback), lo, hi);
    }

    // One input event as the engine dispatched it.
    //   kind 'b' button:     device, id = idCode (DIK scancode / mouse button / pad), a = value, b = heldDownSecs
    //   kind 'm' mouse move: a = dx, b = dy (raw mickeys)
    //   kind 't' thumbstick: id = stick id, a = x, b = y
    //   kind 'c' char:       id = UTF-32 code point
    struct EchoEvent
    {
        std::int64_t  t_ns{};
        char          kind{};
        std::int32_t  device{};
        std::uint32_t id{};
        double        a{};
        double        b{};
        std::string   user;  // engine user event ("Forward", "Jump", ...), may be empty
    };

    inline void append_json_string(std::string& out, std::string_view s)
    {
        static constexpr char hex[] = "0123456789abcdef";
        out.push_back('"');
        for (const char ch : s) {
            const auto c = static_cast<unsigned char>(ch);
            switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            default:
                if (c < 0x20) {
                    out += "\\u00";
                    out.push_back(hex[c >> 4]);
                    out.push_back(hex[c & 0xF]);
                } else {
                    out.push_back(ch);
                }
            }
        }
        out.push_back('"');
    }

    inline void append_number(std::string& out, double v)
    {
        if (!std::isfinite(v))
            v = 0.0;
        char buf[64];
        const auto [p, ec] = std::to_chars(buf, buf + sizeof(buf), v);
        out.append(buf, ec == std::errc{} ? p : buf);
    }

    inline void append_int(std::string& out, std::int64_t v)
    {
        char buf[32];
        const auto [p, ec] = std::to_chars(buf, buf + sizeof(buf), v);
        out.append(buf, ec == std::errc{} ? p : buf);
    }

    // `[gf:input/v1]{"seq":N,"flush_ns":T,"dropped":D,"ev":[[t_ns,"k",device,id,a,b,"user"],...]}`
    inline std::string encode_batch(std::uint64_t seq, std::int64_t flush_ns, std::uint64_t dropped,
        const std::vector<EchoEvent>& events)
    {
        std::string out;
        out.reserve(kInputEchoPrefix.size() + 64 + events.size() * 48);
        out += kInputEchoPrefix;
        out += "{\"seq\":";
        append_int(out, static_cast<std::int64_t>(seq));
        out += ",\"flush_ns\":";
        append_int(out, flush_ns);
        out += ",\"dropped\":";
        append_int(out, static_cast<std::int64_t>(dropped));
        out += ",\"ev\":[";
        for (std::size_t i = 0; i < events.size(); ++i) {
            const auto& e = events[i];
            if (i)
                out.push_back(',');
            out.push_back('[');
            append_int(out, e.t_ns);
            out += ",\"";
            out.push_back(e.kind);
            out += "\",";
            append_int(out, e.device);
            out.push_back(',');
            append_int(out, e.id);
            out.push_back(',');
            append_number(out, e.a);
            out.push_back(',');
            append_number(out, e.b);
            out.push_back(',');
            append_json_string(out, e.user);
            out.push_back(']');
        }
        out += "]}";
        return out;
    }

    // Batches events and decides when to flush. Not thread-safe: the engine
    // dispatches input and ticks the socket on the main thread.
    class EchoBatcher
    {
    public:
        EchoBatcher(std::int64_t flush_interval_ns, std::size_t max_events) :
            m_interval(flush_interval_ns), m_max(max_events) {}

        void push(EchoEvent e)
        {
            if (m_events.empty())
                m_first_ns = e.t_ns;
            if (m_events.size() >= m_max * 4) {  // sender stalled; bound memory
                ++m_dropped;
                return;
            }
            m_events.push_back(std::move(e));
        }

        [[nodiscard]] bool due(std::int64_t now_ns) const
        {
            return !m_events.empty() && (m_events.size() >= m_max || now_ns - m_first_ns >= m_interval);
        }

        // Encodes and clears the pending batch. Call `sent(false)` if delivery failed.
        std::string take(std::int64_t now_ns)
        {
            auto msg = encode_batch(m_seq, now_ns, m_dropped, m_events);
            m_last_count = m_events.size();
            m_events.clear();
            return msg;
        }

        void sent(bool ok)
        {
            ++m_seq;
            if (!ok)
                m_dropped += m_last_count;
        }

        [[nodiscard]] std::size_t   pending() const { return m_events.size(); }
        [[nodiscard]] std::uint64_t seq() const { return m_seq; }
        [[nodiscard]] std::uint64_t dropped() const { return m_dropped; }

    private:
        std::int64_t           m_interval;
        std::size_t            m_max;
        std::int64_t           m_first_ns{};
        std::uint64_t          m_seq{};
        std::uint64_t          m_dropped{};
        std::size_t            m_last_count{};
        std::vector<EchoEvent> m_events;
    };
}
