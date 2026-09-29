#pragma once

// GameFoundry fork hooks. Upstream files call into these with one line each:
//   Socket.cpp  NeuroSocket::Initialize   -> gamefoundry::WebSocketUrl()   (patch 0)
//   main.cpp    kDataLoaded               -> gamefoundry::OnDataLoaded()   (patch 2)
//   main.cpp    OnUpdateHook::OnUpdateMod -> gamefoundry::OnFrame()        (patches 1, 2 and 3)
//   Socket.cpp  action lists, registration, dispatch -> gamefoundry/Actions.hpp (patch 3)
// See GAMEFOUNDRY.md.

namespace gamefoundry
{
    // Bridge URL: NEURO_SDK_WS_URL env > `wsUrl` in _neuroSkyrim.ini > ws://localhost:8000.
    // Resolved once and cached; the pointer stays valid for the process lifetime.
    const char* WebSocketUrl();

    // Registers the input-echo sink on BSInputDeviceManager (unless disabled) and
    // arms the telemetry push.
    void OnDataLoaded();

    // Flushes a due input-echo batch and sends a due telemetry sample (patch 1).
    // Called once per frame from the update hook, which also runs while paused.
    void OnFrame();
}
