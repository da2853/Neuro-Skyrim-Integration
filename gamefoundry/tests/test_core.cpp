// Host-compiler tests for src/gamefoundry/core.hpp (no Skyrim, no SKSE).
//   c++ -std=c++20 -Wall -Wextra -Werror -I src gamefoundry/tests/test_core.cpp -o test_core && ./test_core
// Prints one encoded input-echo batch and one telemetry sample (one line each) so the
// Python decoder fixtures can be checked against them.

#include "gamefoundry/core.hpp"

#include <cmath>
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

// NiMatrix3::EulerAnglesToAxesZXY from CommonLibSSE, the forward transform.
static void zxy(float (&m)[3][3], double x, double y, double z)
{
    const double cx = std::cos(x), sx = std::sin(x), cy = std::cos(y), sy = std::sin(y), cz = std::cos(z), sz = std::sin(z);
    m[0][0] = float(cz * cy + sz * sx * sy);
    m[0][1] = float(sz * cx);
    m[0][2] = float(-cz * sy + sz * sx * cy);
    m[1][0] = float(-sz * cy + cz * sx * sy);
    m[1][1] = float(cz * cx);
    m[1][2] = float(sz * sy + cz * sx * cy);
    m[2][0] = float(cx * sy);
    m[2][1] = float(-sx);
    m[2][2] = float(cx * cy);
}

static bool near(double a, double b, double eps = 1e-3) { return std::fabs(a - b) < eps; }

static void test_euler()
{
    const double d = kPi / 180.0;
    const double cases[][3] = { { 0, 0, 0 }, { 10, 0, 90 }, { -30, 0, 200 }, { 45, 5, 359 }, { -80, -20, 1 }, { 20, 0, 180 } };
    for (const auto& c : cases) {
        float m[3][3];
        zxy(m, c[0] * d, c[1] * d, c[2] * d);
        const auto e = euler_from_zxy(m);
        CHECK(near(e.pitch, c[0]) && near(e.roll, c[1]) && near(e.yaw, c[2]));
        if (!(near(e.pitch, c[0]) && near(e.roll, c[1]) && near(e.yaw, c[2])))
            std::fprintf(stderr, "euler %g %g %g -> %g %g %g\n", c[0], c[1], c[2], e.pitch, e.roll, e.yaw);
    }
    // Forward = column 1: yaw 90 faces +X, pitch positive faces down.
    float m[3][3];
    zxy(m, 30 * d, 0, 90 * d);
    CHECK(near(m[0][1], std::cos(30 * d)) && near(m[1][1], 0) && near(m[2][1], -std::sin(30 * d)));
    // Straight down: yaw survives, roll is reported as 0.
    zxy(m, 90 * d, 0, 120 * d);
    const auto down = euler_from_zxy(m);
    CHECK(near(down.pitch, 90, 1e-2) && near(down.roll, 0) && near(down.yaw, 120, 1e-2));
}

static void test_angles()
{
    CHECK(wrap360(-90) == 270 && wrap360(720) == 0 && wrap360(359.5) == 359.5);
    CHECK(wrap180(270) == -90 && wrap180(180) == 180 && wrap180(-180) == 180);
    const Vec3 o{ 0, 0, 0 };
    CHECK(near(relative_bearing_deg(o, 0, { 0, 100, 0 }), 0));
    CHECK(near(relative_bearing_deg(o, 0, { 100, 0, 0 }), 90));
    CHECK(near(relative_bearing_deg(o, 90, { 100, 0, 0 }), 0));
    CHECK(near(relative_bearing_deg(o, 90, { 0, 100, 0 }), -90));
    CHECK(near(relative_bearing_deg(o, 0, { 0, -100, 0 }), 180));
    CHECK(relative_bearing_deg(o, 45, o) == 0);
}

static void test_utf8()
{
    CHECK(utf8_prefix("abc", 5) == "abc");
    CHECK(utf8_prefix("abcdef", 3) == "abc");
    CHECK(utf8_prefix("ab\xC3\xA9z", 3) == "ab");        // never split a 2-byte character
    CHECK(utf8_prefix("ab\xC3\xA9z", 4) == "ab\xC3\xA9");
    CHECK(utf8_prefix("\xE2\x82\xAC\xE2\x82\xAC", 5) == "\xE2\x82\xAC");
}

static void test_overlays()
{
    CHECK(is_overlay_menu("HUD Menu") && is_overlay_menu("Cursor Menu") && is_overlay_menu("Fader Menu"));
    CHECK(!is_overlay_menu("Dialogue Menu") && !is_overlay_menu("Loading Menu"));
}

static TelemetrySample telemetry_sample()
{
    TelemetrySample s;
    s.seq = 42;
    s.t_ns = 123'456'789;
    s.cost_us = 57;
    s.pos = Vec3{ 12345.678, -2345.04, 7890.0 };
    s.heading = 359.999;
    s.cam = Vec3{ 12345.6, -2345.0, 8010.25 };
    s.rot = Euler{ 12.345, 0.0, 90.0 };
    s.fov = 80.0;
    s.cell = CellInfo{ 0x0001A26F, "Whiterun \"Plains\"", false };
    s.location = "Whiterun Hold";
    s.menu_stack = { "Dialogue Menu" };
    s.in_dialogue = true;
    s.vitals = std::array<Vital, 3>{ Vital{ 95.5, 100 }, Vital{ 80, 120 }, Vital{ 0, 50 } };
    s.nearby = {
        { 12, "Iron Sword", "item", 140.4, -30.2, false },
        { 9, "Imperial Soldier", "actor", 512.6, 179.9, true },
    };
    return s;
}

static void test_encode_telemetry()
{
    const auto s = encode_telemetry(telemetry_sample());
    const std::string expected =
        "[gf:telemetry/v1]{\"seq\":42,\"t_ns\":123456789,\"us\":57,"
        "\"pos\":[12345.7,-2345,7890],\"heading\":0,"
        "\"cam\":[12345.6,-2345,8010.3],\"rot\":[12.35,0,90],\"fov\":80,"
        "\"cell\":{\"id\":\"0001A26F\",\"name\":\"Whiterun \\\"Plains\\\"\",\"interior\":false},"
        "\"location\":\"Whiterun Hold\",\"menu_stack\":[\"Dialogue Menu\"],"
        "\"paused\":false,\"loading\":false,\"in_combat\":false,\"in_dialogue\":true,\"dead\":false,"
        "\"vitals\":{\"health\":[95.5,100],\"stamina\":[80,120],\"magicka\":[0,50]},"
        "\"nearby\":[[12,\"Iron Sword\",\"item\",140,-30,false],[9,\"Imperial Soldier\",\"actor\",513,180,true]]}";
    CHECK(s == expected);
    if (s != expected)
        std::fprintf(stderr, "got:      %s\nexpected: %s\n", s.c_str(), expected.c_str());

    // Main menu: nothing about the world is known, only menus.
    TelemetrySample menu;
    menu.menu_stack = { "Main Menu" };
    menu.paused = true;
    CHECK(encode_telemetry(menu) ==
          "[gf:telemetry/v1]{\"seq\":0,\"t_ns\":0,\"us\":0,\"menu_stack\":[\"Main Menu\"],"
          "\"paused\":true,\"loading\":false,\"in_combat\":false,\"in_dialogue\":false,\"dead\":false,\"nearby\":[]}");

    // Size cap: the furthest nearby entries go first; names are cut to 40 bytes.
    auto big = telemetry_sample();
    big.nearby.clear();
    for (int i = 0; i < 20; ++i)
        big.nearby.push_back({ 1000 + i, std::string(60, 'x'), "container", 100.0 * i, 0, false });
    const auto capped = encode_telemetry(big);
    CHECK(capped.size() <= kTelemetryMaxBytes);
    CHECK(capped.find("[1000,") != std::string::npos);
    CHECK(capped.find("[1019,") == std::string::npos);
    CHECK(capped.find(std::string(41, 'x')) == std::string::npos);
    CHECK(encode_telemetry(big, 10).find("\"nearby\":[]") != std::string::npos);  // cannot fit: nearby empty

    // Non-finite numbers never reach the wire (JSON has no NaN).
    auto nan = telemetry_sample();
    nan.fov = std::nan("");
    CHECK(encode_telemetry(nan).find("\"fov\":0,") != std::string::npos);
}

static void test_ticker()
{
    Ticker t(100);
    CHECK(t.due(1000));
    CHECK(!t.due(1099));
    CHECK(t.due(1100));   // on schedule
    CHECK(t.due(1250));   // late by 50: next stays on the 100 grid (1300)
    CHECK(!t.due(1299));
    CHECK(t.due(1300));
    CHECK(t.due(5000));   // long stall: restart from now, no burst
    CHECK(!t.due(5050));
    CHECK(t.due(5100));
}

int main()
{
    test_ini();
    test_url();
    test_long();
    test_encode();
    test_batcher();
    test_euler();
    test_angles();
    test_utf8();
    test_overlays();
    test_encode_telemetry();
    test_ticker();
    if (failures) {
        std::fprintf(stderr, "%d check(s) failed\n", failures);
        return EXIT_FAILURE;
    }
    std::printf("%s\n", encode_batch(7, 3'000'000, 2, sample()).c_str());
    std::printf("%s\n", encode_telemetry(telemetry_sample()).c_str());
    std::fprintf(stderr, "ok\n");
    return EXIT_SUCCESS;
}
