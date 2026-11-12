// The type 12 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/network-mode.md, the globals in
// docs/known-data.md): the game's Xbox Live mode (the network battle and
// time attack). Its screens are made native as any front-end mode's, and
// its Xbox Live calls (sign-in, gamer card, sessions, privileges,
// leaderboards, the network manager's functions) are answered by plugging
// in data: the values the SDK's XAM imports return today (the user's call,
// 2026-11-12). Plain C++ over guest memory (the machines and the mode's
// block are shared with the recompiled code), with the originals' loads and
// stores in their order and their frames' words.
// Adapters: src/hooks/network_mode.cpp (behind --native_network_mode),
// register steps shared with the harness in src/hooks/network_mode.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides (the screen start, Strings::Load,
// Archive::WaitIdle, StandScene::Create, VirtualAlloc, the fades and the
// loading screen).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace network_mode {

using Calls = race_hud_object::Calls;

// The mode's fields: the delegate's holder (+4) and the delegate (+8), the
// dispatcher (+36: the handlers, the count +200), the type 12 block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kBlock = 848;
// The block's words: the first screen's index (+0x2100C the shown one,
// +0x21010 the first), +0x21024 (set when the session was made here).
constexpr uint32_t kShown = 0x2100C;
constexpr uint32_t kFirstShown = 0x21010;
constexpr uint32_t kSessionMade = 0x21024;

// The globals the Init reads: the mode before, the session and the
// network's state (docs/known-data.md).
constexpr uint32_t kModeBefore = 0x826032E0;
constexpr uint32_t kSession = 0x82816E3C;
constexpr uint32_t kNetworkState = 0x8281611C;

// The guest functions the Init calls.
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kArchiveWaitIdle = 0x8209D730;
constexpr uint32_t kStandSceneCreate = 0x82180B00;
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kLoadingShow = 0x822478D0;
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kStringFile = 59;
constexpr uint32_t kFadesPasses = 0x82001BEC;
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kSlot8Thunk = 0x8223D4B0;

// The type 12 block's machines (ModeType12::Init's VirtualAlloc, 135,208
// bytes at the mode's +848), by the index ModeType12::Machine numbers them.
constexpr uint32_t kBlockBytes = 135208;
constexpr uint32_t kModeSelectAt = 0;      // 0, NetModeSelect
constexpr uint32_t kScreen1At = 9640;      // 1
constexpr uint32_t kScreen3At = 19280;     // 3
constexpr uint32_t kRankingAt = 28920;     // 4, NetRanking
constexpr uint32_t kTaCourseAt = 38560;    // 5, NetTaCourse
constexpr uint32_t kPowerGraphAt = 48200;  // 14, PowerGraphScreen12
constexpr uint32_t kScreen6At = 57840;     // 6 (9,660 bytes)
constexpr uint32_t kScreen7At = 67500;     // 7 (9,732 bytes)
constexpr uint32_t kDialog10At = 77232;    // 10 (9,648 bytes)
constexpr uint32_t kDialog11At = 86880;    // 11 (9,644 bytes)
constexpr uint32_t kScreen8At = 96524;     // 8 (9,668 bytes)
constexpr uint32_t kScreen9At = 106192;    // 9 (9,696 bytes)
constexpr uint32_t kDialog12At = 115888;   // 12 (9,648 bytes)
constexpr uint32_t kDialog13At = 125536;   // 13 (9,644 bytes)

// --- The machines' set-ups (init.cpp) ---

// The fourteen InitStates (r3 the machine): their transitions added to
// `machine` (StateMachine::Add's plain C++) from functors built in their
// frames (`frame`, the function's own: the caller's r1 less the frame's
// bytes; its back chain written, the originals' stores in order; the
// functors' words the originals never write copied as the frame holds
// them). Machines 6 to 13 first call NetScreen::InitBase(machine, index,
// machine), its base Init's frame below theirs. The last Add's r3 (an
// index when `replaced`, else the new transition).
constexpr uint32_t kModeSelectInitFrame = 1232;  // sub_8227D680, 17 transitions
constexpr uint32_t kScreen1InitFrame = 1360;     // sub_82276510, 19
constexpr uint32_t kScreen3InitFrame = 1232;     // sub_8225F330, 17
constexpr uint32_t kRankingInitFrame = 1360;     // sub_822873A0, 19
constexpr uint32_t kTaCourseInitFrame = 1232;    // sub_82281858, 17
constexpr uint32_t kScreen6InitFrame = 528;      // sub_82268410, 6 (with the base's 10)
constexpr uint32_t kScreen7InitFrame = 592;      // sub_8226EB40, 7
constexpr uint32_t kScreen8InitFrame = 592;      // sub_82262A88, 7
constexpr uint32_t kScreen9InitFrame = 592;      // sub_822646B0, 7
constexpr uint32_t kDialog10InitFrame = 464;     // sub_82288600, 5
constexpr uint32_t kDialog11InitFrame = 464;     // sub_82277B18, 5
constexpr uint32_t kDialog12InitFrame = 528;     // sub_8227F6C8, 6
constexpr uint32_t kDialog13InitFrame = 464;     // sub_8227EA50, 5
constexpr uint32_t kPowerGraphInitFrame = 848;   // sub_821B96B8, 11
uint32_t ModeSelectInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen1InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen3InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t RankingInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t TaCourseInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen6InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen7InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen8InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Screen9InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Dialog10InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Dialog11InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Dialog12InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t Dialog13InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t PowerGraphInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// The eight base Inits (r3 the machine, r4 the functors' object: the
// machine itself in play): the ten common transitions, as the InitStates
// above. 784-byte frames, r27 to r31 saved.
constexpr uint32_t kInitBaseFrame = 784;
uint32_t Screen6InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced);  // sub_82278C20
uint32_t Screen7InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced);  // sub_82279118
uint32_t Screen8InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced);  // sub_8227A000
uint32_t Screen9InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                         bool* replaced);  // sub_8227A4F8
uint32_t Dialog10InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced);  // sub_82279610
uint32_t Dialog11InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced);  // sub_82279B08
uint32_t Dialog12InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced);  // sub_8227A9F0
uint32_t Dialog13InitBase(uint8_t* base, uint32_t machine, uint32_t object, uint32_t frame,
                          bool* replaced);  // sub_8227AEE8

// sub_8227B3E0 NetScreen::InitBase (r3 the machine, r4 the index, r5 the
// functors' object, moved into r4): a leaf with no frame tail-calling the
// index's base Init from its jump table (6 through the jump
// sub_8227B41C), its frame below `sp` (the caller's r1); other indexes
// (above 13 or below 6, unsigned) return at once. Whether one ran; `r3`
// its last Add's (untouched when none ran).
bool InitBase(uint8_t* base, uint32_t machine, uint32_t index, uint32_t object, uint32_t sp,
              bool* replaced, uint32_t* r3);

// --- The mode's Init (mode.cpp) ---

// sub_82278058 ModeType12::Init (r3 the mode): see the row; the fourteen
// InitStates as plain C++, the other callees through their overrides. The
// second FadeIn's r3. 224-byte frame (and the caller's +20 = the mode).
constexpr uint32_t kInitFrame = 224;
uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);

// --- The mode's delegate functions and block helpers (mode.cpp) ---

// The machines' words: the state, the movie loaded, back (to the title),
// the mode again, held.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineBack = 9624;
constexpr uint32_t kMachineAgain = 9632;
// The mode's message ring (+240; the +32 object's +208) and the block's
// next-mode answer (8 bytes, NextMode's, handed over by Teardown).
constexpr uint32_t kRing = 240;
constexpr uint32_t kNextAnswer = 0x21014;
// The globals: the sound manager, the sign-in watch's "not on Live" check
// (0x826BAB40 + 16), the title's flag NextMode sets, the session's buffer
// Session::Release frees, the race data and the time attack's set-up (the
// course route, whether +0x82816684 picks the race data's +0x59618, that
// word, the car's 1,884 bytes and its +3892, the word for the session's
// +216).
constexpr uint32_t kSoundManager = 0x826C5C80;
constexpr uint32_t kSignInWatch = 0x826BAB40;
constexpr uint32_t kTitleFlag = 0x826364EC;
constexpr uint32_t kSessionBuffer = 0x826C6AD4;
constexpr uint32_t kSessionRows = 0x13CD34;  // the session's +1,297,716
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kTaRoute = 0x82816660;
constexpr uint32_t kTaHasKind = 0x82816680;
constexpr uint32_t kTaKind = 0x82816684;
constexpr uint32_t kTaCar = 0x82816688;
constexpr uint32_t kTaCarBytes = 1884;
constexpr uint32_t kTaCarWord = 0x82816E38;
constexpr uint32_t kTaSessionWord = 0x82816694;

// sub_82277E10 ModeType12::Machine (with its case 0's entry sub_82277E44):
// the machine of index `index` in the block (2 and above 14: 0).
uint32_t Machine(uint32_t block, uint32_t index);
// sub_82277FF0 ModeType12::FindShown: the first machine (0 to 14) in state
// 5 or 4 becomes the shown one (+0x2100C); none: kept. The last index it
// looked at (Machine's r3 the original leaves: the one found, else 14).
uint32_t FindShown(uint8_t* base, uint32_t block);
// sub_82277EE0 ModeType12::AllLoaded: 1 when all fourteen machines' movies
// are loaded (+9608), else 0.
uint32_t AllLoaded(uint8_t* base, uint32_t block);

// The delegate functions (r3 the 8-byte answer, r4 the mode; `sp` the
// caller's r1): sub_8227B440 WaitForLoads (176-byte frame), sub_8227B930
// NextMode (4,112, the time attack's car in it), sub_8227B868 LeaveScreen
// (128), sub_8227B770 WaitForShown (112), sub_8227BE10 Teardown (144).
void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void Teardown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);

// The +32 vtable's slots (r3 the mode + 32): sub_822788D8 ForceExit
// (240-byte frame) and sub_82278B98 Resume (96). r3 as the originals leave
// it (the shown machine).
uint64_t ForceExit(uint8_t* base, uint32_t object, uint32_t sp, const Calls& calls);
uint32_t Resume(uint8_t* base, uint32_t object, uint32_t sp);

// sub_82292710 Session::Free: with a session ([0x82816E3C]), its buffers
// released (sub_82291670 Session::Release: the session's +0x13CD34 through
// operator delete and [0x826C6AD4] through operator delete[], each cleared)
// and the session's pages freed (VirtualFree), the global cleared. 96-byte
// frames. r3 as the original leaves it.
uint64_t SessionFree(uint8_t* base, uint32_t sp, const Calls& calls);

}  // namespace network_mode
