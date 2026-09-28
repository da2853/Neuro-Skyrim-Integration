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
| `src/main.cpp` | patch 2: `gamefoundry::OnDataLoaded()` on `kDataLoaded`, `gamefoundry::OnFrame()` in the update hook |
| `xmake.lua` | skip the copy-into-game step when `SKYRIM_PATH` is unset (CI) |

## Settings

Environment variables win over `_neuroSkyrim.ini` (the upstream file in the game
directory), which wins over the default. Invalid values fall through.

| Setting | Env | Ini key (`[Settings]`) | Default |
| --- | --- | --- | --- |
| Bridge WebSocket URL (`ws://` or `wss://`) | `NEURO_SDK_WS_URL` | `wsUrl` | `ws://localhost:8000` |
| Input echo on/off | `GF_INPUT_ECHO` (`0`/`1`) | `inputEcho` | `1` |
| Echo flush interval, ms (10-1000) | `GF_INPUT_ECHO_MS` | `inputEchoMs` | `50` |

The resolved values are logged once to the SKSE plugin log
(`GameFoundry: wsUrl=... inputEcho=... inputEchoMs=...`).

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

Run the core tests locally:

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
