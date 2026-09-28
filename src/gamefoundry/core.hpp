#pragma once

// GameFoundry fork additions: pure logic with no RE/SKSE dependency, so it can be
// unit-tested with a host compiler (gamefoundry/tests/test_core.cpp).
// See GAMEFOUNDRY.md for the wire format and settings.

#include <algorithm>
#include <array>
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
    inline constexpr std::string_view kTelemetryPrefix = "[gf:telemetry/v1]";
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

    // ------------------------------------------------------------ patch 1: telemetry push

    inline constexpr double kPi = 3.14159265358979323846;
    inline constexpr double kRadToDeg = 180.0 / kPi;

    // Angle in degrees wrapped to [0, 360).
    inline double wrap360(double deg)
    {
        if (!std::isfinite(deg))
            return 0.0;
        deg = std::fmod(deg, 360.0);
        if (deg < 0.0)
            deg += 360.0;
        return deg >= 360.0 ? 0.0 : deg;
    }

    // Angle in degrees wrapped to (-180, 180].
    inline double wrap180(double deg)
    {
        deg = wrap360(deg);
        return deg > 180.0 ? deg - 360.0 : deg;
    }

    struct Vec3
    {
        double x{}, y{}, z{};
    };

    // Pitch, roll, yaw in degrees, the convention of Actor::data.angle (x, y, z):
    // pitch positive looking down, yaw 0 = +Y (north), clockwise, in [0, 360).
    struct Euler
    {
        double pitch{}, roll{}, yaw{};
    };

    // Inverse of NiMatrix3::EulerAnglesToAxesZXY (CommonLibSSE), which is how the
    // engine builds a node rotation from actor angles. `m` is NiMatrix3::entry.
    // Column 1 (m[0][1], m[1][1], m[2][1]) is the forward vector.
    inline Euler euler_from_zxy(const float (&m)[3][3])
    {
        const double sx = std::clamp(-static_cast<double>(m[2][1]), -1.0, 1.0);
        const double pitch = std::asin(sx);
        double roll = 0.0, yaw = 0.0;
        if (std::sqrt(1.0 - sx * sx) > 1e-6) {
            yaw = std::atan2(m[0][1], m[1][1]);
            roll = std::atan2(m[2][0], m[2][2]);
        } else {  // looking straight up or down: roll and yaw share an axis, report roll 0
            yaw = std::atan2(-static_cast<double>(m[1][0]), m[0][0]);
        }
        return { pitch * kRadToDeg, wrap180(roll * kRadToDeg), wrap360(yaw * kRadToDeg) };
    }

    // Bearing of `to` seen from `from` facing `heading_deg` (yaw convention above),
    // in (-180, 180], positive to the right.
    inline double relative_bearing_deg(const Vec3& from, double heading_deg, const Vec3& to)
    {
        const double dx = to.x - from.x, dy = to.y - from.y;
        if (dx == 0.0 && dy == 0.0)
            return 0.0;
        return wrap180(std::atan2(dx, dy) * kRadToDeg - heading_deg);
    }

    // Menus that are open for the whole session and say nothing about game state.
    inline bool is_overlay_menu(std::string_view name)
    {
        return name == "HUD Menu" || name == "Cursor Menu" || name == "Fader Menu";
    }

    // At most `max_bytes` of `s`, cut on a UTF-8 character boundary.
    inline std::string_view utf8_prefix(std::string_view s, std::size_t max_bytes)
    {
        if (s.size() <= max_bytes)
            return s;
        std::size_t n = max_bytes;
        while (n > 0 && (static_cast<unsigned char>(s[n]) & 0xC0) == 0x80)
            --n;
        return s.substr(0, n);
    }

    struct CellInfo
    {
        std::uint32_t form_id{};
        std::string   name;
        bool          interior{};
    };

    struct Vital
    {
        double cur{}, max{};
    };

    struct NearbyEntry
    {
        std::int64_t id{};       // the plugin's object-list id ([id N] in its context text)
        std::string  name;
        std::string  kind;       // actor, corpse, door, container, item, activator, furniture, flora, other
        double       dist{};     // world units from the player
        double       bearing{};  // degrees, relative to the player's heading
        bool         hostile{};
    };

    struct TelemetrySample
    {
        std::uint64_t             seq{};
        std::int64_t              t_ns{};
        std::int64_t              cost_us{};  // previous sample: read + encode, microseconds
        std::int64_t              send_us{};  // previous sample: SendContext, microseconds
        std::optional<Vec3>       pos;
        std::optional<double>     heading;
        std::optional<Vec3>       cam;
        std::optional<Euler>      rot;
        std::optional<double>     fov;
        std::optional<CellInfo>   cell;
        std::optional<std::string> location;
        std::vector<std::string>  menu_stack;  // bottom to top, overlays removed
        bool                      paused{};
        bool                      loading{};
        bool                      in_combat{};
        bool                      in_dialogue{};
        bool                      dead{};
        std::optional<std::array<Vital, 3>> vitals;  // health, stamina, magicka
        std::vector<NearbyEntry>  nearby;            // nearest first
    };

    inline constexpr std::size_t kTelemetryMaxBytes = 1000;
    inline constexpr std::size_t kTelemetryMaxName = 40;

    // `v` rounded to 1/scale (scale 10 = one decimal). Dividing by the integer scale
    // gives the double nearest the decimal, so to_chars prints it short ("1234.5").
    inline double round_to(double v, double scale)
    {
        return std::isfinite(v) ? std::round(v * scale) / scale : 0.0;
    }

    inline void append_vec(std::string& out, const Vec3& v, double scale)
    {
        out.push_back('[');
        append_number(out, round_to(v.x, scale));
        out.push_back(',');
        append_number(out, round_to(v.y, scale));
        out.push_back(',');
        append_number(out, round_to(v.z, scale));
        out.push_back(']');
    }

    inline void append_bool(std::string& out, std::string_view key, bool v)
    {
        out += ",\"";
        out += key;
        out += "\":";
        out += v ? "true" : "false";
    }

    inline void append_hex8(std::string& out, std::uint32_t v)
    {
        static constexpr char hex[] = "0123456789ABCDEF";
        out.push_back('"');
        for (int shift = 28; shift >= 0; shift -= 4)
            out.push_back(hex[(v >> shift) & 0xF]);
        out.push_back('"');
    }

    inline std::string encode_telemetry_n(const TelemetrySample& s, std::size_t n_nearby)
    {
        std::string out;
        out.reserve(512);
        out += kTelemetryPrefix;
        out += "{\"seq\":";
        append_int(out, static_cast<std::int64_t>(s.seq));
        out += ",\"t_ns\":";
        append_int(out, s.t_ns);
        out += ",\"us\":";
        append_int(out, s.cost_us);
        out += ",\"send_us\":";
        append_int(out, s.send_us);
        if (s.pos) {
            out += ",\"pos\":";
            append_vec(out, *s.pos, 10);
        }
        if (s.heading) {
            out += ",\"heading\":";
            append_number(out, wrap360(round_to(*s.heading, 100)));
        }
        if (s.cam) {
            out += ",\"cam\":";
            append_vec(out, *s.cam, 10);
        }
        if (s.rot) {
            out += ",\"rot\":";
            append_vec(out, { s.rot->pitch, s.rot->roll, wrap360(round_to(s.rot->yaw, 100)) }, 100);
        }
        if (s.fov) {
            out += ",\"fov\":";
            append_number(out, round_to(*s.fov, 100));
        }
        if (s.cell) {
            out += ",\"cell\":{\"id\":";
            append_hex8(out, s.cell->form_id);
            out += ",\"name\":";
            append_json_string(out, utf8_prefix(s.cell->name, kTelemetryMaxName));
            append_bool(out, "interior", s.cell->interior);
            out.push_back('}');
        }
        if (s.location) {
            out += ",\"location\":";
            append_json_string(out, utf8_prefix(*s.location, kTelemetryMaxName));
        }
        out += ",\"menu_stack\":[";
        for (std::size_t i = 0; i < s.menu_stack.size(); ++i) {
            if (i)
                out.push_back(',');
            append_json_string(out, utf8_prefix(s.menu_stack[i], kTelemetryMaxName));
        }
        out.push_back(']');
        append_bool(out, "paused", s.paused);
        append_bool(out, "loading", s.loading);
        append_bool(out, "in_combat", s.in_combat);
        append_bool(out, "in_dialogue", s.in_dialogue);
        append_bool(out, "dead", s.dead);
        if (s.vitals) {
            static constexpr std::string_view names[3] = { "health", "stamina", "magicka" };
            out += ",\"vitals\":{";
            for (std::size_t i = 0; i < 3; ++i) {
                if (i)
                    out.push_back(',');
                out.push_back('"');
                out += names[i];
                out += "\":[";
                append_number(out, round_to((*s.vitals)[i].cur, 10));
                out.push_back(',');
                append_number(out, round_to((*s.vitals)[i].max, 10));
                out.push_back(']');
            }
            out.push_back('}');
        }
        out += ",\"nearby\":[";
        for (std::size_t i = 0; i < n_nearby && i < s.nearby.size(); ++i) {
            const auto& e = s.nearby[i];
            if (i)
                out.push_back(',');
            out.push_back('[');
            append_int(out, e.id);
            out.push_back(',');
            append_json_string(out, utf8_prefix(e.name, kTelemetryMaxName));
            out.push_back(',');
            append_json_string(out, e.kind);
            out.push_back(',');
            append_number(out, round_to(e.dist, 1.0));
            out.push_back(',');
            append_number(out, round_to(wrap180(e.bearing), 1.0));
            out.push_back(',');
            out += e.hostile ? "true" : "false";
            out.push_back(']');
        }
        out += "]}";
        return out;
    }

    // `[gf:telemetry/v1]{...}`; drops the furthest `nearby` entries until the
    // message fits in `max_bytes` (everything else is bounded by construction).
    inline std::string encode_telemetry(const TelemetrySample& s, std::size_t max_bytes = kTelemetryMaxBytes)
    {
        std::size_t n = s.nearby.size();
        auto out = encode_telemetry_n(s, n);
        while (out.size() > max_bytes && n > 0)
            out = encode_telemetry_n(s, --n);
        return out;
    }

    // Decides when the next sample is due. Not thread-safe (main thread only).
    class Ticker
    {
    public:
        explicit Ticker(std::int64_t interval_ns) :
            m_interval(interval_ns) {}

        [[nodiscard]] bool due(std::int64_t now_ns)
        {
            if (m_next != 0 && now_ns < m_next)
                return false;
            // Schedule from the ideal time so the rate holds at any frame rate; after a
            // long stall (loading, alt-tab) restart from now instead of bursting.
            m_next = (m_next != 0 && now_ns - m_next < m_interval) ? m_next + m_interval : now_ns + m_interval;
            return true;
        }

    private:
        std::int64_t m_interval;
        std::int64_t m_next{};
    };
}
