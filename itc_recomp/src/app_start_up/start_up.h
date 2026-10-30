// The app's start-up natively (docs/functions/app-start-up.md): plain C++
// over the guest layout. The adapters are in src/hooks/app_start_up.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <mutex>
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

// --- The light billboards' shaders ---

// The guest calls Billboards::Init makes (the adapter's), each with the
// original's return address: the HLSL compile (D3DXCompileShader through its
// override, the stored shaders: `source`, `length`, the entry and profile
// strings, the shader buffer out; with r1 = `frame`, whose +84 and +92 are
// its errors and constant table outs), a buffer's slot 3 (its data) and slot
// 2 (Release), the video output (`at` 0 to 5: the three locks and unlocks),
// and VertexShader::Create and PixelShader::Create through their overrides.
struct BillboardCalls {
  std::function<uint32_t(int which, uint32_t source, uint32_t length, uint32_t entry,
                         uint32_t profile, uint32_t out)>
      compile;
  std::function<uint32_t(int which, uint32_t buffer)> buffer_data;
  std::function<void(int which, uint32_t buffer)> release;
  std::function<uint32_t(int at)> video_output;
  std::function<uint32_t(uint32_t data)> vertex_shader;
  std::function<uint32_t(uint32_t data)> pixel_shader;
  MemcpyCall memcpy_call;
};

// sub_8209BE88 Billboards::Init: the ring flipped (0x826A5098 = 1 when it
// was 0, else 0; 0x826A509C = 0); the vertex shader's source
// ([0x82600A54], "main", "vs_3_0") compiled into `frame` + 96 (the errors
// to `frame` + 104, released when the compile fails and they're set) and
// made under the device lock (`frame` + 108) into 0x826A50A0, its buffer
// released; the pixel shader's ([0x82600A60], "ps_3_0", `frame` + 100,
// errors + 112, lock + 116) into 0x826A50A4; the vertex declaration
// (CreateVertexDeclaration(0x82600A64), lock + 116) into 0x826A50A8;
// 0x8266D090 = the word at 0x82000A34 (0.0). `frame` is the original's
// 160-byte frame. Needs --native_heap (CreateVertexDeclaration) and the
// device lock native (the adapter checks).
void BillboardsInit(uint8_t* base, uint32_t frame, const BillboardCalls& calls);

// --- The text: the font's set-up (strings-and-font.md) ---

constexpr uint32_t kFontBytes = 16592;

// The guest calls the font's set-up makes (Text::Init's adapter's), each
// with the original's return address: the video output ([0x8266D088]'s
// slot 7, then that object's slot 3), the render object ([0x826BABA0]'s
// slot 8, then that object's slot 1 for `which` 0, the command list, or
// slot 2 for 1, the render queue), the HLSL compile (D3DXCompileShader
// through its override, r9 = 0; `which` 0 the vertex shader, 1 to 4 the
// pixel shaders; r1 is the
// frame, whose +84 and +92 are the errors and constant table outs), a
// buffer's slot 3 (its data) and slot 2 (Release), VertexShader::Create and
// PixelShader::Create through their overrides, and Strings::Init (its plain
// C++ where its adapter's condition holds, else the adapter; its r3).
struct FontCalls {
  std::function<uint32_t()> video_output;
  std::function<uint32_t(int which)> render_object;
  std::function<uint32_t(int which, uint32_t source, uint32_t length, uint32_t profile,
                         uint32_t out)>
      compile;
  std::function<uint32_t(int which, uint32_t buffer)> buffer_data;
  std::function<uint32_t(int which, uint32_t buffer)> release;
  std::function<uint32_t(uint32_t data)> vertex_shader;
  std::function<uint32_t(int which, uint32_t data)> pixel_shader;
  MemcpyCall memcpy_call;
  std::function<uint32_t()> strings_init;
};

// sub_821088B8 Font::CreateShaders: the vertex declaration
// (CreateVertexDeclaration(0x826032F8)) into +16560; the vertex shader's
// source ([0x82603338], "main", "vs_3_0") compiled into `frame` + 96 (the
// errors to `frame` + 100, never released) and made into +16564; the four
// pixel shaders from the 12-byte entries at 0x826C5C04 (+0 the source, +4
// its length; "ps_2_0") into +16568 to +16580; each buffer released. No
// error checks. `frame` is the original's 160-byte frame. Returns r3 as the
// last release leaves it.
uint32_t FontCreateShaders(uint8_t* base, uint32_t font, uint32_t frame, const FontCalls& calls);
// sub_82109228 Font::Font: +0 the video output, +4 and +8 the command list
// and render queue; the shaders made (`shaders_frame`) with the device
// locked (the word at `frame` + 88, the original's 128-byte frame); the
// pages (+20 to +32) and their files (+40 to +52) cleared; the ten colors
// at +16528 (five of their bytes through the frame's +80 to +84, as the
// original); +12 = 0. Returns 14 (r3 as the original leaves it). Needs the
// device lock native (the adapter checks).
uint32_t FontConstruct(uint8_t* base, uint32_t font, uint32_t frame, uint32_t shaders_frame,
                       const FontCalls& calls);
// sub_8210ADD0 Text::Init: Font::Init (sub_82109A00: 16,592 bytes of pages,
// the allocation layer's AllocPages as VirtualAlloc's adapter gives them,
// into 0x826C5C00, then Font::Font on them), then Strings::Init. The frames
// are Font::Font's and Font::CreateShaders' (below Text::Init's and
// Font::Init's 96 bytes each). Only with --native_heap (the adapter checks).
// Returns r3 as Strings::Init leaves it.
uint32_t TextInit(uint8_t* base, uint32_t font_frame, uint32_t shaders_frame,
                  const FontCalls& calls);

// --- The app's start-up ---

// The guest calls App::StartUp makes: `call` a guest call of the callee at
// `target` (its adapter) with r3 and r4 when given (else left as they
// are) and the original's return address; its r3. `virtual_call` the
// interface's slot at `offset` (r3 the interface); its r3.
struct StartUpCalls {
  std::function<uint32_t(uint32_t target, std::optional<uint32_t> r3,
                         std::optional<uint32_t> r4, uint32_t return_address)>
      call;
  std::function<uint32_t(uint32_t offset, uint32_t return_address)> virtual_call;
};

// sub_82106548 App::StartUp: `interface` (app + 16) to the four globals
// (ReadError::SetApp, sub_8209AC30, sub_8209F580's plain C++;
// SceneTarget::Create through its adapter), Archive::Init(slot +16's
// answer, 0xC80000), ActionMappers::Init(interface, 1), CameraPasses::Init,
// ScreenFades::Init, Movies::Set (its plain C++, the adapter where it hands
// back), Billboards::Init, SoundGlobals::Init(slot +48's answer),
// SaveInterface::Init(interface), Network::SetUp(interface), Text::Init,
// sub_8225FB70's plain C++, Players::ResetControllers and
// Archive::WaitIdle, each callee with r3 as the original leaves it. Both
// GlobalMemoryStatus calls (sub_82294DA0, Dropped: their buffer is never
// read) left out. `frame` is the original's 128-byte frame. Returns r3 as
// the original leaves it (`frame` + 80, the last GlobalMemoryStatus's
// buffer).
uint32_t AppStartUp(uint8_t* base, uint32_t interface, uint32_t frame, const StartUpCalls& calls);

// --- The movie class's construction (movies.md) ---

constexpr uint32_t kMovieVtable = 0x82002910;
constexpr uint32_t kMovieWrapperVtable = 0x8206CC90;
constexpr uint32_t kMovieWrapperBlockBytes = 32;  // the critical section, +28 the player
constexpr uint32_t kDelegateVtable = 0x82002850;  // Delegate (12 bytes)

// The wrapper's critical section (the 32-byte block at its +4): in its
// place one host mutex (the user's call for a subsystem's critical section,
// 2026-10-24 and 2026-10-25: Edram::Alloc, the memory blocks' lock),
// recursive as a critical section is (no user takes it twice; nothing reads
// its depth). Its users are the wrapper's slot 0 (sub_82593E78, open) and
// slots 2 to 4 (sub_82593D88, never seen in play), both through ScopedLock
// (sub_8258E580, sub_8258E5B8), whose adapters (src/hooks/models.cpp) take
// the mutex for the block the native constructor made (MovieWrapperLock);
// nothing else enters it (the movie library is given only the wrapper, and
// with --play_movies_with_ffmpeg none of the wrapper's slots runs).
std::recursive_mutex& MovieWrapperMutex();
// The block the native constructor made (0 before, or when the recompiled
// one made it); the harness saves and puts it back.
uint32_t MovieWrapperLock();
void SetMovieWrapperLock(uint32_t critical_section);
bool IsMovieWrapperLock(uint32_t critical_section);
// sub_8258E580 ScopedLock::ScopedLock for the wrapper's section: the lock
// word (+0) = the section, then the mutex taken.
void MovieWrapperLockEnter(uint8_t* base, uint32_t lock, uint32_t critical_section);
// sub_8258E5B8 ScopedLock::~ScopedLock for a lock word holding it: the mutex
// given back; returns the word (the original's r3).
uint32_t MovieWrapperLockLeave(uint8_t* base, uint32_t lock);

// sub_82593DC0 MovieWrapper::MovieWrapper: +0 the vtable 0x8206CC90, +4 a
// 32-byte block (operator new; InitializeCriticalSection left out, the host
// mutex above in its place, the block's first 28 bytes as operator new
// leaves them) whose +28 (the player) = 0. Returns the wrapper, or nullopt
// where operator new's plain C++ can't answer (the original runs; the store
// it made again is the same).
std::optional<uint32_t> MovieWrapperConstruct(uint8_t* base, uint32_t wrapper);

// The guest calls Movie::Movie makes: `virtual_call` the object's vtable slot
// at `offset` with r4, r5 and the original's return address; `wrapper`
// MovieWrapper::MovieWrapper on the 8 bytes (its plain C++, the original
// where it hands back); each returns r3.
struct MovieCalls {
  std::function<uint32_t(uint32_t object, uint32_t offset, uint32_t r4, uint32_t r5,
                         uint32_t return_address)>
      virtual_call;
  std::function<uint32_t(uint32_t wrapper)> wrapper;
};

// sub_820AEA88 Movie::Movie: r3 the movie (312 bytes), `block` the set-up
// block (+0 the run loop, +4 a word kept at +4). The vtable, the command at
// +12 (+12 0x820066DC, +16 0x82002908, +20 to +30, +36 a delegate's vtable,
// +40, +44), +8 the wrapper (8 bytes, operator new, or 0), the run loop's
// AddTickHandler (slot 2) and AddListener (slot 1), each key 1 with a
// 12-byte delegate on `frame` (the original's 160-byte frame: +80 the key,
// +88 and +104 the delegates; +88 left 0x82000ABC, as the original), +308
// (the state) = 0. Returns the movie, or nullopt where operator new's plain
// C++ can't answer (the original runs from the start: the stores it made
// again are the same).
std::optional<uint32_t> MovieConstruct(uint8_t* base, uint32_t movie, uint32_t block,
                                       uint32_t frame, const MovieCalls& calls);

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
