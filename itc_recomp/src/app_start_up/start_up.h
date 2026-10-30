// The app's start-up natively (docs/functions/app-start-up.md): plain C++
// over the guest layout. The adapters are in src/hooks/app_start_up.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace app_start_up {

// The globals the start-up keeps the app's interface (app + 16, vtable
// 0x82000E38) in.
constexpr uint32_t kReadErrorApp = 0x82600344;    // ReadError::SetApp (+4 of 0x82600340)
constexpr uint32_t kAppInterface84 = 0x8266D084;  // sub_8209AC30
constexpr uint32_t kRenderApp = 0x826BABA0;       // sub_8209F580
// Movies::Set's: the movie class (the app + 128).
constexpr uint32_t kMovieClass = 0x826C1CDC;
// sub_8225FB70's two words.
constexpr uint32_t kWords8110 = 0x82816110;
constexpr uint32_t kWords8114 = 0x82816114;

// The interface's slot +44 (sub_82379350: its +112, the app + 128).
constexpr uint32_t kInterfaceMovie = 0x82379350;

// sub_8209AC50 ReadError::SetApp: `interface` stored at 0x82600344.
void ReadErrorSetApp(uint8_t* base, uint32_t interface);
// sub_8209AC30: `interface` stored at 0x8266D084.
void SetAppInterface84(uint8_t* base, uint32_t interface);
// sub_8209F580: `interface` stored at 0x826BABA0.
void SetRenderApp(uint8_t* base, uint32_t interface);
// sub_8225FB70: 0x82816110 and 0x82816114 = 0.
void ClearWords8110(uint8_t* base);
// sub_820A4360 Movies::Set: the interface's slot +44 answer (the movie
// class) stored at 0x826C1CDC; returns it. nullopt, with nothing done, where
// the slot isn't sub_82379350.
std::optional<uint32_t> MoviesSet(uint8_t* base, uint32_t interface);

// memcpy (sub_825A7410) through the guest, where c_runtime::Memcpy refuses.
using MemcpyCall = std::function<void(uint32_t destination, uint32_t source, uint32_t size)>;

// sub_822A8980 CreateVertexDeclaration: the 12-byte elements at `elements`
// up to the one whose stream (halfword +0) is 255; XMemAlloc(56 + 12 x
// count, 0x24800000), its first 56 bytes cleared, +0 = 0x00100005, +4 = 1,
// +20 = 0xFFFF0000, +24 the count, +28 the highest stream, +32 to +47 a
// stream map (255 for each stream used, else 0), +48 = 0, the elements
// copied to +52. Returns the declaration, or 0 when the allocation fails.
// Only with --native_heap (memory::crt::XMemAlloc). nullopt, with nothing
// done, for a stream above 15 (the original's map store, a byte on its
// stack at +80 + the stream, reaches past the map into its frame or its
// caller's: never in play).
std::optional<uint32_t> CreateVertexDeclaration(uint8_t* base, uint32_t elements,
                                                const MemcpyCall& memcpy_call);

// --- The scene target ---

constexpr uint32_t kSceneTargetApp = 0x8266D088;  // the video output's way in
constexpr uint32_t kSceneTarget = 0x8266D08C;
constexpr uint32_t kDisplayTiling = 0x8260034C;  // the display's tiling block (a pointer)

// The guest calls the device's users make: the video output ([0x8266D088]'s
// vtable slot 7, then that object's slot 3; `at` 0 for the lock, 1 for the
// unlock picks the original's return addresses).
using VideoOutputCall = std::function<uint32_t(int at)>;

// sub_8209ADD8 Tiling::LargestTile: the largest width (right - left) and
// height (bottom - top) of the block's tiles (+12 the count, +16 on 16
// bytes each) into the two words at `out` (0, 0 with none).
void TilingLargestTile(uint8_t* base, uint32_t tiling, uint32_t out);
// sub_8209B070 SceneTarget::SceneTarget: +16 = 1024, +20 = 576, +28 = 0;
// with the device locked (VideoOutput::LockDevice's plain C++, the device at
// `frame` + 80, the original's 192-byte frame, with its parameter blocks at
// +96 and +112): +4, +0 (over +4's EDRAM tiles), +24 (a 1280 x 720
// texture), and the display's largest tile aligned by its multisample type
// for +12 and +8 (over +12's tiles). Returns the object. Needs --native_heap,
// --native_graphics_resources and --native_video_output (the adapter checks).
uint32_t SceneTargetConstruct(uint8_t* base, uint32_t target, uint32_t frame,
                              const VideoOutputCall& video_output);
// sub_8209B378 SceneTarget::Create: `interface` at 0x8266D088, then a
// 32-byte SceneTarget (operator new) into 0x8266D08C, or 0 there when the
// allocation fails. `frame` is SceneTarget::SceneTarget's. Returns the
// target (r3 as the original leaves it: 0 without one), or nullopt where
// operator new's plain C++ can't answer (the original runs; the store it
// made again is the same).
std::optional<uint32_t> SceneTargetCreate(uint8_t* base, uint32_t interface, uint32_t frame,
                                          const VideoOutputCall& video_output);

// --- The network manager's set-up (system-services.md) ---

constexpr uint32_t kNetworkManager = 0x826C74A0;
constexpr uint32_t kUserTable = 0x826C7670;      // the machine id's address too
constexpr uint32_t kXNetConfig = 0x82957680;     // sub_82577560's block (76 bytes)
constexpr uint32_t kXNetConfigSource = 0x8265D408;  // its source's address (the word there)
constexpr uint32_t kSessionState = 0x826C6AA8;

// sub_8214F1C0 SignIn::SetUp: the fewest and most users (0x826C7944,
// 0x82618B7C), 0x826C7948, 0x82618B80, the app (0x826C799C, when not 0);
// with no listener yet (0x826C794C), 0x826C7994 and 0x826C7964 = 0 and a
// listener (mask 3, user::kNoListener). Returns r3 as the original leaves
// it (the listener when one was made, else `fewest`).
uint32_t SignInSetUp(uint8_t* base, uint32_t fewest, uint32_t most, uint32_t word7948,
                     uint32_t word8B80, uint32_t app);

// sub_82577560 XNet::Configure (probably): the 44-byte header at `source`
// copied to `config` (memcpy), then, unless +8's bit 0 is set (then
// 0x80004005), the pointers into the data after the header worked out from
// its sizes (+60 when +38 isn't 0, +44 to +72). Returns 0 or 0x80004005.
uint32_t XNetConfigure(uint8_t* base, uint32_t config, uint32_t source,
                       const MemcpyCall& memcpy_call);
// sub_82575B08 XNetStartup (probably): WSAStartup (0, ours), then
// XNetConfigure(0x82957680, [0x8265D408]); 0, or 1627 when that fails.
uint32_t XNetStartup(uint8_t* base, const MemcpyCall& memcpy_call);
// sub_82148868 Network::StartUp: XNetStartup, two listeners (masks 2 and 1)
// at 0x826C6AB4 and 0x826C6AB8, 0x826C6AB0 = 0, 0x826C6ABC = -1, 60 bytes
// at 0x826C6650 and 28 at 0x826C6080 cleared, 0x826C72FC = 0, 0x826C7300 =
// `app`, the user table (384 bytes at 0x826C7670) cleared. Returns the
// user table's address (the last memset's r3).
uint32_t NetworkStartUp(uint8_t* base, uint32_t app, const MemcpyCall& memcpy_call);
// sub_8214E190 Network::SetUp: SignInSetUp(0, 4, 0, 1, app), NetworkStartUp,
// the system link socket not made, the title's XNADDR into `frame` + 80 (the
// original's 144-byte frame; its machine id not answered), the voice set-up
// without the engine (hooks/voice.h), then 0x826C6AA8, 0x826C6AD4,
// 0x826C7938, 0x826C793C and 0x826C7940 = 0. Returns the tick count (the
// voice set-up's r3, left as the original leaves it).
uint32_t NetworkSetUp(uint8_t* base, uint32_t app, uint32_t frame, const MemcpyCall& memcpy_call);

}  // namespace app_start_up
