// The app's state stack and its layers natively (docs/functions/app-states.md,
// "AppStates::" and "StateLayer::"): their set-up at boot (AppStates::Create,
// the layers' constructors, AddLayer, AddChild) and App::Start, which pushes
// the game modes' holder. Plain C++ over the guest layout, store for store as
// the recompiled functions; every virtual call a guest call through AppCalls
// (app.h), and the callees ours where their plain C++ exists (the lists'
// constructors and insert, the operation list, Push, Modes::Modes), the
// originals where those hand back. Adapters: src/hooks/app_states.cpp.
//
// Each takes `frame`, its own guest frame as the original's (made by the
// adapter or the native caller with AppCalls::in_frame), and makes its
// callees' frames the same way. They allocate through operator new's plain
// C++, so they run only while --native_heap routes the game's heap to the
// allocation layer (the adapters' condition); where operator new hands back
// (its new handler), the function returns false with nothing done but stores
// the original would make again, and the original runs.

#pragma once

#include <cstdint>
#include <optional>

#include "app_states/app.h"

namespace app_states {

// The vtables a layer's constructors store: its own at +0, its state's at
// +4 (the state interface's base first), and the state stack's.
constexpr uint32_t kLayerVtable = 0x820028E0;
constexpr uint32_t kLayerStateVtable = 0x820028B0;
constexpr uint32_t kStateBaseVtable = 0x82002880;
constexpr uint32_t kStatesVtable = 0x820028F8;
constexpr uint32_t kLayerBytes = 48;
constexpr uint32_t kStatesBytes = 12;
// A layer: +8 the app's interface (the app + 16), +12 the children, +24 the
// states (lists of pointers), +36 the operations.
constexpr uint32_t kLayerOwner = 8;
constexpr uint32_t kLayerChildren = 12;
constexpr uint32_t kLayerStates = 24;

// The targets ours in play.
constexpr uint32_t kAddLayer = 0x820AE130;
// The base delegate's vtable (a temporary's, as its destructor leaves it).
constexpr uint32_t kDelegateBaseVtable = 0x82000ABC;
// The state stack's three delegates' targets.
constexpr uint32_t kStatesOnUpdate = 0x820AE1B8;
constexpr uint32_t kStatesOnUpdateToggled = 0x820AE1C0;
constexpr uint32_t kStatesOnMessage = 0x820AE1C8;

// sub_820AD118 StateLayer::StateLayer (`state` none) and sub_820ADBD8
// StateLayer::StateLayer(state) (the state queued as a push): the vtables,
// +8 = `owner`, the children's and states' lists and the operation list made.
// `frame` is the original's (96 bytes, or 112 with a state).
void LayerConstruct(uint8_t* base, uint32_t layer, uint32_t owner, std::optional<uint32_t> state,
                    uint32_t frame, const AppCalls& calls);
// sub_820ADA08 StateLayer::AddChild: the child's state (+4) slot 1 (r4 = the
// layer), then the child appended to the children (+12). `frame` the
// original's 112 bytes (`child` stored at its caller's r1 + 28).
void LayerAddChild(uint8_t* base, uint32_t layer, uint32_t child, uint32_t frame,
                   const AppCalls& calls);
// sub_820AE130 AppStates::AddLayer (the stack's slot 1): a new layer holding
// `state` (owner the app's slot 2) added to the root as a child (0 when
// operator new answers 0). False, with nothing done, where operator new
// hands back. `frame` the original's 112 bytes.
bool StatesAddLayer(uint8_t* base, uint32_t states, uint32_t state, uint32_t frame,
                    const AppCalls& calls);
// sub_820AE348 AppStates::AppStates: +0 the vtable, +4 the app (`setup`'s
// +0), +8 a root layer (owner the app's slot 2), then the three delegates
// handed to the app's slot 1's interface (keys 8 and 2, listener 9). False,
// with nothing done, where operator new hands back. `frame` the original's
// 176 bytes.
bool StatesConstruct(uint8_t* base, uint32_t states, uint32_t setup, uint32_t frame,
                     const AppCalls& calls);
// sub_820AE498 AppStates::Create: 12 bytes from operator new built by
// AppStates::AppStates; the block, 0, or nullopt where operator new hands
// back (nothing done). `frame` the original's 96 bytes.
std::optional<uint32_t> StatesCreate(uint8_t* base, uint32_t setup, uint32_t frame,
                                     const AppCalls& calls);

// sub_82092878 App::Start (the +20 interface's slot +36, from
// Framework::Start once at boot): the boot movies' table (0x826C5BAC) set,
// the modes' holder made (Modes::Modes with 3) and pushed on the state stack
// (its slot 1, AppStates::AddLayer), then 0x826364EC = 2. nullopt, with
// nothing done, where operator new hands back. `frame` the original's 96
// bytes. Its r3 (the slot's), or nullopt.
std::optional<uint32_t> AppStart(uint8_t* base, uint32_t interface, uint32_t frame,
                                 const AppCalls& calls);
constexpr uint32_t kBootMovies = 0x826C5BAC;
constexpr uint32_t kStartFlag = 0x826364EC;

}  // namespace app_states
