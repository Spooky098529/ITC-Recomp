// What the mode manager's tick (Modes::Tick, sub_82107910) runs besides the
// current mode's slots, as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/mode-tick.md, the globals in
// docs/known-data.md): the per-mode settings, the sign-in watch, the
// billboards' ring, the screen fades and the menu music. Their globals are
// shared with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order. Adapters: src/hooks/mode_tick.cpp.

#pragma once

#include "c_runtime/exit.h"

#include <cstdint>
#include <functional>
#include <optional>

namespace mode_tick {

// --- The billboards' ring ---

// The ring half (0 or 1) and the vertex count Billboards::BuildVertices
// writes (docs/functions/sprites.md).
constexpr uint32_t kRingHalf = 0x826A5098;
constexpr uint32_t kRingCount = 0x826A509C;

// sub_8209BD18 Billboards::FlipRing: the other half, its count 0.
void BillboardsFlipRing(uint8_t* base);

// --- The screen fades (two layers) ---

constexpr uint32_t kFadeOutDurations = 0x826A50AC;
constexpr uint32_t kFadeInDurations = 0x826A50B4;
constexpr uint32_t kFadeColors = 0x826A50BC;  // three words a layer
constexpr uint32_t kFadeAlphas = 0x826A50D4;
constexpr uint32_t kFadeInCounts = 0x826A50DC;
constexpr uint32_t kFadeOutCounts = 0x826A50E4;

// sub_8209CDE8 ScreenFades::Init: both layers' words 0.
void ScreenFadesInit(uint8_t* base);
// sub_8209D038 ScreenFades::FadeIn: `layer` fades in from the color over
// `passes`; the other layer's (layer 0's for any layer but 0) counts and
// alpha 0.
void ScreenFadesFadeIn(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                       uint32_t blue, uint32_t layer);
// sub_8209D0C8 ScreenFades::FadeOut: `layer` fades out to the color over
// `passes`.
void ScreenFadesFadeOut(uint8_t* base, uint32_t passes, uint32_t red, uint32_t green,
                        uint32_t blue, uint32_t layer);

// The two layers' sort keys (668 and 676, words in the image) and the camera
// pass the color rectangles are recorded for (camera pass 0).
constexpr uint32_t kFadeKeys = 0x82001BF4;
constexpr uint32_t kFadeCameraPass = 0x826BAC00;
// The color rectangle ScreenFades::Update builds in its frame (at the
// original's r1 less kScreenFadesUpdateFrame, + 112): an identity matrix at
// +0, the kind +64 (0: flat), +72 = 2, x, y, width, height at +80 to +92,
// the color's red, green, blue and alpha words at +96 to +108 (+76 not
// written).
constexpr uint32_t kScreenFadesUpdateFrame = 416;
constexpr uint32_t kFadeRect = 112;

// sub_8209D130 ScreenFades::Update (with ColorRect::Record, sub_8217C348,
// merged: the rectangle's kind is always 0, RecordFlat): layer 0's counts
// stepped (only when layer 1's in and out counts are 0) and layer 1's, each
// layer's rectangle built in the original's frame at `frame` + kFadeRect
// and, when its alpha is above 0, `draw(key, rect)` (the adapter's
// ScreenFades::RecordScissor with the key, then ColorRect::RecordFlat with
// key + 1: guest calls). Returns 1 while any count isn't 0, else 0.
uint32_t ScreenFadesUpdate(uint8_t* base, uint32_t frame,
                           const std::function<void(uint32_t key, uint32_t rect)>& draw);

// sub_8209CE68 ScreenFades::RecordScissor's two chunks on the scene's
// command list `list` (the adapter's getters: [0x826BABA0]'s slot 8, then 1),
// each handed to the render queue (slot 8, then 2; its slot 4) by the
// adapter: the scissor rectangle (command 31: 0, 0, 1280, 720) with the key
// and -10,000.0, then commands 1 and 200 with the key and -9,000.0.
// ScissorChunksFit says whether both can be recorded without the messages
// the original's command list calls would print (a chunk open, the list
// full): else the original runs.
bool ScissorChunksFit(uint8_t* base, uint32_t list);
uint32_t RecordScissorChunk(uint8_t* base, uint32_t list);
uint32_t RecordScissorEndChunk(uint8_t* base, uint32_t list);

// sub_8217A370 ColorRect::RecordFlat's chunk on the command list at
// [0x826D83D0 + 4]: the blend states by the alpha, the helper object's
// draw and four 24-byte vertices of the rectangle `rect`. Returns its first
// slot, which the adapter hands to the render queue at [0x826D83D0] (its
// slot 4, with the key and 0.0); nullopt, with nothing done, where the
// original's command list calls would print a message.
constexpr uint32_t kColorRectLists = 0x826D83D0;
std::optional<uint32_t> ColorRectRecordFlat(uint8_t* base, uint32_t rect);

// --- The music ---

// sub_821063B0 Music::Play: the music `name`'s index among the 18 names
// (lstrcmpA, `pcr` the thread's r13), its two text ids written in the
// original's frame at `frame` (its r1 less kMusicPlayFrame: the names'
// table at +96, the ids at +80 and +84, as it leaves them). Returns the
// addresses the call through [0x826C5C80]'s slot 4 takes in r7 and r8 (the
// adapter's guest call).
constexpr uint32_t kMusicPlayFrame = 288;
constexpr uint32_t kMusicPlayer = 0x826C5C80;
struct MusicIds {
  uint32_t r7, r8;
};
MusicIds MusicPlay(uint8_t* base, uint32_t pcr, uint32_t frame, uint32_t name);

// --- The sign-in watch and the players ---

// Player n's controller's user (four words), the signed-in users' mask
// (written by Network::Update, docs/functions/system-services.md).
constexpr uint32_t kPlayerUsers = 0x826BABA4;
constexpr uint32_t kSignedInMask = 0x826C7954;

// sub_8209F658 Players::SignedInSlot: the player's user's index among the
// signed-in users, or -1.
int32_t PlayersSignedInSlot(uint8_t* base, uint32_t player);

// The action mappers' entries (8 bytes each, four; action-mappers.md).
constexpr uint32_t kActionMapperEntries = 0x826BAB80;

// sub_8209F318 ActionMappers::SetBlocked: each entry's +4 = value.
void ActionMappersSetBlocked(uint8_t* base, uint32_t value);

// --- The per-mode settings, set when a mode is made ---

// The sign-in watch's seven checks a mode turns off (1 = off), its state
// (0 to 9) and the reason its prompt was started for.
constexpr uint32_t kChecks = 0x826BAB40;
constexpr uint32_t kWatchState = 0x826BAB3C;
constexpr uint32_t kWatchReason = 0x826BAB5C;
constexpr uint32_t kOtherUserSignedIn = 0x826BAB68;
constexpr uint32_t kUserSignedOut = 0x826BAB6C;

// sub_82106868 Modes::SetChecks: the checks by the new mode's type. Type 1
// (the race) copies the current stage (Stages::Copy) into the original's
// frame at `frame` + 80 (its r1 less kSetChecksFrame), as it leaves it.
// False, with nothing written, where Stages::Copy's plain C++ declines (the
// original then runs).
constexpr uint32_t kSetChecksFrame = 176;
bool ModesSetChecks(uint8_t* base, uint32_t type, uint32_t frame);

// sub_82106740 Modes::SetSceneTargetMode (with its switch's piece
// sub_82106778): the 1024 x 576 target object's (`[0x8266D08C]`) +28 by the
// new mode's type.
void ModesSetSceneTargetMode(uint8_t* base, uint32_t type);

// sub_822934B8 Presence::SetForMode (with its switch's piece sub_82293504):
// presence is an Xbox feature (mapped, not ported), and its calls'
// XUserSetContextEx wrapper (sub_82294F70) is Dropped (B5 step 2): what's
// left of the calls is the wrapper's, the last error cleared (`pcr` the
// thread's r13). False for type 1, which makes no call (r3 left); true
// otherwise (r3 = 0, the wrapper's answer).
bool PresenceSetForMode(uint8_t* base, uint32_t pcr, uint32_t type);

// sub_8209ED58 SignInWatch::CheckIdle: state 0's check, the state set to 1
// with a reason when one comes up. False, with nothing written, where the
// original would trap (the network's vector at 0x826C7308 with its end
// before its begin, or not a whole number of 384-byte entries).
bool SignInWatchCheckIdle(uint8_t* base);
// Whether SignInWatch::CheckIdle's plain C++ would hand the original the
// call (its traps).
bool SignInWatchCheckIdleTraps(uint8_t* base);

// The user table's names (32-byte UTF-16, from +14, 32 bytes a slot;
// Network::Update's), the last pass's signed-in count, mask and name.
constexpr uint32_t kUserNames = 0x826C7670 + 14;
constexpr uint32_t kSignedInCount = 0x826C7958;
constexpr uint32_t kLastCount = 0x826BAB60;
constexpr uint32_t kLastMask = 0x826BAB64;
constexpr uint32_t kLastName = 0x826BAB1C;

// sub_8209EEC8 SignInWatch::Update (with its switch's piece sub_8209F040):
// the player's user compared with the last pass's, the mode's checks
// applied, state 0's check (SignInWatch::CheckIdle), and the users kept for
// the next pass. The player's name is copied into the original's frame at
// `frame` + 80 (its r1 less kSignInWatchUpdateFrame), as it leaves it.
// False, with nothing written, where the original must run: the state 1 to
// 9 (the prompts' handlers, never seen in play) or CheckIdle's traps.
constexpr uint32_t kSignInWatchUpdateFrame = 224;
bool SignInWatchUpdate(uint8_t* base, uint32_t frame);

// sub_825D9518 SignInWatch::StaticInit (a static set-up, no direct callers):
// the state 0x826BAB3C = 0, the seven checks at 0x826BAB40 = 0, 0x826BAB70 =
// 0, the 32-byte name at 0x826BAB1C cleared, and its clean-up 0x825DAF08 (an
// empty function) registered with atexit (through `call` when the native one
// can't); returns atexit's r3.
int32_t SignInWatchStaticInit(uint8_t* base, const c_runtime::AtexitCall& call);

}  // namespace mode_tick
