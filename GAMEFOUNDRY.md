# GameFoundry fork

Fork of [vituha230/Neuro-Skyrim-Integration](https://github.com/vituha230/Neuro-Skyrim-Integration)
used by [GameFoundry](https://github.com/da2853/GameFoundry) (submodule
`third_party/Neuro-Skyrim-Integration`, pinned in `games/skyrim/deps.toml`).

`gamefoundry` is the default branch. Upstream `main` is merged into it by the
weekly `upstream-sync` workflow through a PR; `main` in this fork is left as
forked and is not used.

## What the fork changes

All new code lives in `src/gamefoundry/`. Upstream files get one-line hooks so
upstream merges stay clean:

| File | Change |
| --- | --- |
| `src/Socket.cpp` | patch 0: `desc.url = gamefoundry::WebSocketUrl()` instead of the hard-coded `ws://localhost:8000` |
| `src/main.cpp` | patch 2: `gamefoundry::OnDataLoaded()` on `kDataLoaded`, `gamefoundry::OnFrame()` in the update hook (patch 1 runs from the same two hooks) |
| `xmake.lua` | skip the copy-into-game step when `SKYRIM_PATH` is unset (CI) |
| `src/Socket.cpp` | patch 3: `GF_EXTRA_ACTIONS` appended to `ActionsList` and `ActionsListNoForces`; `gamefoundry::RegisterActions()` where `walk_to_object` is registered; `gamefoundry::HandleAction()` in the action dispatch (player alive, no force) |

## Settings

Environment variables win over `_neuroSkyrim.ini` (the upstream file in the game
directory), which wins over the default. Invalid values fall through.

| Setting | Env | Ini key (`[Settings]`) | Default |
| --- | --- | --- | --- |
| Bridge WebSocket URL (`ws://` or `wss://`) | `NEURO_SDK_WS_URL` | `wsUrl` | `ws://localhost:8000` |
| Input echo on/off | `GF_INPUT_ECHO` (`0`/`1`) | `inputEcho` | `1` |
| Echo flush interval, ms (10-1000) | `GF_INPUT_ECHO_MS` | `inputEchoMs` | `50` |
| Telemetry push on/off | `GF_TELEMETRY` (`0`/`1`) | `telemetry` | `1` |
| Telemetry interval, ms (50-1000) | `GF_TELEMETRY_MS` | `telemetryMs` | `100` |

The resolved values are logged once to the SKSE plugin log
(`GameFoundry: wsUrl=... inputEcho=... inputEchoMs=... telemetry=... telemetryMs=...`).
Once a minute the telemetry push logs its rate, cost and size
(`GameFoundry: telemetry N msgs in 60.0 s (0 failed), read+encode avg U us max U us, send avg U us max U us, size avg B max B bytes`).

## Patch 1: telemetry push wire format

Every `telemetryMs` (default 100 ms, 10 Hz) while the socket is connected, the
update hook reads the game state on the main thread and sends one **silent**
`context` message (at most 1000 bytes):

```
[gf:telemetry/v1]{"seq":N,"t_ns":T,"us":U,"send_us":V,"pos":[x,y,z],"heading":deg,"cam":[x,y,z],
 "rot":[pitch,roll,yaw],"fov":deg,"cell":{"id":"0001A26F","name":"...","interior":false},
 "location":"...","menu_stack":["Dialogue Menu"],"paused":false,"loading":false,
 "in_combat":false,"in_dialogue":true,"dead":false,
 "vitals":{"health":[cur,max],"stamina":[cur,max],"magicka":[cur,max]},
 "nearby":[[id,"name","kind",dist,bearing,hostile],...]}
```

- `seq` increments per sample (a gap means a failed send); `t_ns` is the
  plugin's `steady_clock` at the sample; `us` is how long the previous sample
  took to read the game and encode, `send_us` how long its `SendContext`
  (libneurosdk queue + mongoose wakeup) took, both in microseconds.
- World units; angles in degrees. `pos` and `heading` are the player's
  (`data.location`, `data.angle.z`); `cam` and `rot` are the camera root's
  world transform. `rot` is `[pitch, roll, yaw]` in the convention of
  `Actor::data.angle` (the inverse of `NiMatrix3::EulerAnglesToAxesZXY`):
  pitch positive looking down, yaw 0 = +Y (north) turning clockwise, 0-360.
  `fov` is `PlayerCamera::worldFOV`.
- `menu_stack` is the UI menu stack bottom to top without the always-open
  `HUD Menu`, `Cursor Menu` and `Fader Menu`. `paused` is `UI::GameIsPaused()`,
  `loading` is the `Loading Menu`, `in_dialogue` the `Dialogue Menu`.
- `vitals` are current and maximum actor values.
- `nearby`: up to 8 entries of the plugin's own object list (the `[id N]` ids
  its context messages and actions use) within 4096 units, nearest first:
  id, display name (at most 40 bytes), kind (`actor`, `corpse`, `door`,
  `container`, `activator`, `furniture`, `flora`, `item`, `other`), distance,
  bearing relative to the player's heading (-180..180, positive right), and
  whether an actor is hostile. The furthest entries are dropped first if the
  message would exceed 1000 bytes.
- Keys whose value the game cannot give are omitted, not guessed: at the main
  menu or during loading only `menu_stack` and the flags are sent.

The GameFoundry decoder is `games/skyrim/telemetry_push.py`.

## Patch 2: input echo wire format

An `InputEvent*` sink on `BSInputDeviceManager` records every input event the
engine dispatches (what the plugin injected through `BSInputEventQueue`, plus
any real hardware input). Events are batched and sent as one **silent**
`context` message per flush:

```
[gf:input/v1]{"seq":N,"flush_ns":T,"dropped":D,"ev":[[t_ns,kind,device,id,a,b,user],...]}
```

- `seq` increments per batch (a gap means a lost batch); `dropped` is the
  cumulative count of events that could not be sent (socket down, backlog).
- `t_ns` and `flush_ns` are the plugin's `steady_clock` in nanoseconds. The
  harness maps an event to its own clock with
  `t_ns_harness = recv_t_ns - (flush_ns - t_ns)`.
- `device`: 0 keyboard, 1 mouse, 2 gamepad, 3 virtual keyboard (`RE::INPUT_DEVICE`).
- `kind` and fields:
  - `"b"` button: `id` = idCode (keyboard: DirectInput scan code; mouse: 0 left,
    1 right, 2 middle, 3-7 extra, 8 wheel up, 9 wheel down), `a` = value
    (1 pressed, 0 released), `b` = heldDownSecs. Held keys repeat every frame
    with `a=1, b>0`.
  - `"m"` mouse move: `a` = dx, `b` = dy (raw counts).
  - `"t"` thumbstick: `id` = stick, `a` = x, `b` = y.
  - `"c"` char: `id` = code point.
- `user` is the engine user event bound to the input (`"Forward"`, `"Jump"`,
  `"Left Attack/Block"`), empty when none.

The GameFoundry decoder is `games/skyrim/input_echo.py`.

## CI and releases

`.github/workflows/build.yml` runs the host-compiler tests
(`gamefoundry/tests/test_core.cpp`) and builds on `windows-latest` with xmake
3.1.1 and MSVC in `releasedbg`. Each push to `gamefoundry` publishes release
`gf-<sha7>` with the DLL, PDB, `mysc.esp`, a zip in game-directory layout
(`Data/SKSE/Plugins/*.dll`, `Data/mysc.esp`, `Data/Scripts/*.pex`) and
`SHA256SUMS`.

Run the core tests locally (they also print one sample of each wire format,
which the GameFoundry decoder tests use as fixtures):

```
c++ -std=c++20 -Wall -Wextra -Werror -I src gamefoundry/tests/test_core.cpp -o test_core && ./test_core
```

## Live smoke test (needs Skyrim SE 1.6.1170 + SKSE 2.2.6)

1. Unzip the release zip into the game directory; add Address Library.
2. Start a Neuro API server on another port, for example `ws://127.0.0.1:8123`,
   and set `wsUrl = ws://127.0.0.1:8123` in `_neuroSkyrim.ini` (or export
   `NEURO_SDK_WS_URL`).
3. Launch through `skse64_loader.exe`. The server should see `startup` and
   `actions/register` from game `Skyrim` on that port; the SKSE log shows the
   `GameFoundry:` lines.
4. Once in game, move the mouse and press keys: the server receives silent
   `context` messages starting with `[gf:input/v1]`.
5. Every 100 ms the server receives a silent `context` message starting with
   `[gf:telemetry/v1]`; the SKSE log shows the per-minute telemetry line.

## Patch 3: agent actions (`move`, `turn`, `enter_door`)

Upstream's actions all walk to a target chosen by id, so an agent that the
walker cannot get past something has no way to step around it, and in the
Helgen intro (MQ101 stage < 200) upstream turns every interaction into a plain
walk ("Your hands are bound"), which leaves the keep doors impassable. Patch 3
adds three actions in `src/gamefoundry/Actions.cpp`, registered only where
upstream registers `walk_to_object` (so never in the cutscenes before the
player can walk):

| Action | Arguments | Registered when | Behaviour | End message (non-silent context) |
| --- | --- | --- | --- | --- |
| `move` | `direction`: `forward`/`back`/`left`/`right`, `seconds` 0.3-5 | movement controls enabled | resets the walker, then holds the mapped key for the time given (paused time does not count) | `[You walked forward 2.4 m]`, with `: something blocks the way` under 0.3 m |
| `turn` | `degrees` -180..180, positive = right | looking controls enabled | resets the walker, then turns the player at 150 deg/s with `Actor::SetHeading` | `[You turned right 90 degrees]` |
| `enter_door` | `id` (a door in the object list) | `is_intro2()` (outside the intro `walk_to_object_and_interact` opens doors) | walks to the door with the upstream walker (interaction 0) until within 200 units (40 s timeout). A load door (`ExtraTeleport`) is then activated with `TESObjectREFR::ActivateRef`. An ordinary door is opened first if closed (`ActivateRef`, 0.8 s), then the player turns to face it (150 deg/s, like `turn`) and walks forward 2.5 s through the doorway | `[You opened [id N] Name]` (load door), `[You went through [id N] Name]`, `[[id N] Name does not open. ...]` or `[Couldnt reach the door. ...]` |

The immediate `action/result` is `[You start walking forward...]`,
`[You start turning...]`, `[You go through the door...]` or `[You walk to the
door...]`; failures (`You cannot walk right now`, `This object is not a door`,
...) come back as `success: false`.
