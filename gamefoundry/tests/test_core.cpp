// Host-compiler tests for src/gamefoundry/core.hpp (no Skyrim, no SKSE).
//   c++ -std=c++20 -Wall -Wextra -Werror -I src gamefoundry/tests/test_core.cpp -o test_core && ./test_core
// Prints one encoded batch so the Python decoder fixture can be checked against it.

#include "gamefoundry/core.hpp"

#include <cstdio>
#include <cstdlib>
#include <sstream>

using namespace gamefoundry::core;

static int failures = 0;
#define CHECK(cond)                                                            \
    do {                                                                       \
        if (!(cond)) {                                                         \
            std::fprintf(stderr, "%s:%d: CHECK(%s)\n", __FILE__, __LINE__, #cond); \
            ++failures;                                                        \
        }                                                                      \
    } while (0)

static std::optional<std::string> ini(const char* text, const char* key)
{
    std::istringstream in(text);
    return ini_value(in, key);
}

static void test_ini()
{
    // Upstream's default file plus our keys; upstream keys must not be confused with ours.
    const char* f =
        "[Settings]\n"
        "autoLoadLastSave = 0\n"
        "forceNewGame = 0\n"
        "character = Gary\n"
        "; wsUrl = ws://commented:1\n"
        "WSURL= ws://127.0.0.1:8123 \r\n"
        "inputEchoMs=20\n";
    CHECK(ini(f, "wsUrl") == std::optional<std::string>("ws://127.0.0.1:8123"));
    CHECK(ini(f, "character") == std::optional<std::string>("Gary"));
    CHECK(ini(f, "inputEchoMs") == std::optional<std::string>("20"));
    CHECK(!ini(f, "inputEcho").has_value());  // prefix of inputEchoMs, must not match
    CHECK(!ini("", "wsUrl").has_value());
    CHECK(ini("wsUrl=a\nwsUrl=b\n", "wsUrl") == std::optional<std::string>("b"));
}

static void test_url()
{
    const std::optional<std::string> none;
    CHECK(resolve_ws_url(nullptr, none) == "ws://localhost:8000");
    CHECK(resolve_ws_url(nullptr, std::string("ws://h:1")) == "ws://h:1");
    CHECK(resolve_ws_url("ws://env:2", std::string("ws://h:1")) == "ws://env:2");
    CHECK(resolve_ws_url("  wss://env:3 ", none) == "wss://env:3");
    CHECK(resolve_ws_url("", std::string("ws://h:1")) == "ws://h:1");         // empty env ignored
    CHECK(resolve_ws_url("http://x", std::string("ws://h:1")) == "ws://h:1"); // invalid env ignored
    CHECK(resolve_ws_url(nullptr, std::string("garbage")) == "ws://localhost:8000");
    CHECK(resolve_ws_url("ws://", none) == "ws://localhost:8000");
}

static void test_long()
{
    const std::optional<std::string> none;
    CHECK(resolve_long(nullptr, none, 50, 10, 1000) == 50);
    CHECK(resolve_long(nullptr, std::string(" 20 "), 50, 10, 1000) == 20);
    CHECK(resolve_long("30", std::string("20"), 50, 10, 1000) == 30);
    CHECK(resolve_long("x", std::string("20"), 50, 10, 1000) == 20);
    CHECK(resolve_long(nullptr, std::string("1"), 50, 10, 1000) == 10);   // clamped
    CHECK(resolve_long(nullptr, std::string("0"), 1, 0, 1) == 0);
    CHECK(!parse_long("12abc").has_value());
}

static std::vector<EchoEvent> sample()
{
    return {
        { 1'000'000, 'b', 0, 0x11, 1.0, 0.0, "Forward" },
        { 1'500'000, 'm', 1, 10, -5, 3, "" },
        { 2'000'000, 'b', 1, 0, 0.0, 0.25, "Left Attack/Block" },
        { 2'000'000, 'b', 0, 0x11, 0.0, 1e8, "Say \"hi\"\\\n" },
    };
}

static void test_encode()
{
    const auto s = encode_batch(7, 3'000'000, 2, sample());
    const std::string expected =
        "[gf:input/v1]{\"seq\":7,\"flush_ns\":3000000,\"dropped\":2,\"ev\":["
        "[1000000,\"b\",0,17,1,0,\"Forward\"],"
        "[1500000,\"m\",1,10,-5,3,\"\"],"
        "[2000000,\"b\",1,0,0,0.25,\"Left Attack/Block\"],"
        "[2000000,\"b\",0,17,0,1e+08,\"Say \\\"hi\\\"\\\\\\u000a\"]]}";
    CHECK(s == expected);
    if (s != expected)
        std::fprintf(stderr, "got:      %s\nexpected: %s\n", s.c_str(), expected.c_str());
    CHECK(encode_batch(0, 0, 0, {}) == "[gf:input/v1]{\"seq\":0,\"flush_ns\":0,\"dropped\":0,\"ev\":[]}");
}

static void test_batcher()
{
    EchoBatcher b(50'000'000, 3);
    CHECK(!b.due(0));
    b.push({ 100, 'm', 1, 10, 1, 1, "" });
    CHECK(!b.due(100 + 49'999'999));
    CHECK(b.due(100 + 50'000'000));
    b.push({ 200, 'm', 1, 10, 1, 1, "" });
    b.push({ 300, 'm', 1, 10, 1, 1, "" });
    CHECK(b.due(300));  // max events reached
    const auto m = b.take(400);
    CHECK(m.find("\"seq\":0") != std::string::npos);
    b.sent(false);  // lost: 3 events dropped, seq advances so the harness sees a gap
    CHECK(b.pending() == 0 && b.seq() == 1 && b.dropped() == 3);
    b.push({ 500, 'm', 1, 10, 1, 1, "" });
    const auto m2 = b.take(600);
    CHECK(m2.find("\"seq\":1,\"flush_ns\":600,\"dropped\":3") != std::string::npos);
    b.sent(true);
    CHECK(b.seq() == 2 && b.dropped() == 3);

    EchoBatcher bounded(1, 2);  // memory bound = 4 * max events
    for (int i = 0; i < 10; ++i)
        bounded.push({ i, 'm', 1, 10, 1, 1, "" });
    CHECK(bounded.pending() == 8 && bounded.dropped() == 2);
}

int main()
{
    test_ini();
    test_url();
    test_long();
    test_encode();
    test_batcher();
    if (failures) {
        std::fprintf(stderr, "%d check(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::printf("%s\n", encode_batch(7, 3'000'000, 2, sample()).c_str());
    std::fprintf(stderr, "ok\n");
    return EXIT_SUCCESS;
}
