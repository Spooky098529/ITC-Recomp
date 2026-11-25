// The boot mode (the game mode of type 3, which plays the boot movies) and
// the modes' other methods in 82106040-82108160 as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/boot-mode.md,
// the layouts in docs/known-data.md: "boot mode"). The mode, its state
// machine, the message rings and the movie globals are guest data shared with
// the recompiled code, so these work on guest memory, with the originals'
// loads and stores in their order. Where the originals build values in their
// stack frames (the transitions' functors, the next mode's answer), the
// stores are replayed in the original's frame at `frame` (the original's r1
// after its stwu), so stack words the original never writes reach the same
// places. Adapters: src/hooks/boot_mode.cpp (and the wraps in
// src/hooks/title_screen.cpp).

#pragma once

#include <cstdint>
#include <functional>

namespace boot_mode {

// The boot movies' globals (0x826C5BA4 to 0x826C5BC8), the pad blocked
// words and the start fade's passes word: schema::BootModeGlobals
// (itc_recomp/schema/boot-mode.toml). The mode asked for's none, and the
// movie id ending the list.
constexpr uint32_t kNoModeAsked = 13;
constexpr uint32_t kEndOfList = 11;
// The movies' path table (no schema describes it). The movie class is
// schema::MoviesGlobals::movie_class (movies.toml); the action mappers'
// entries and the player's entry index schema::RacerControllersGlobals
// (PlayerMapperEntry, racer-controllers.toml).
constexpr uint32_t kMoviePaths = 0x82600BEC;
constexpr uint32_t kSkipAction = 152;

// The boot mode's layout: schema::BootMode, BootMachine, ModeDispatcher (the
// mode's +36) and ModeAnswer. The boot machine's offset, for the adapters'
// register arithmetic, and the message rings' shape (50 messages of 12
// bytes).
constexpr uint32_t kMachine = 848;
constexpr uint32_t kRingSlots = 50;
constexpr uint32_t kRingMessageBytes = 12;

// The vtables.
constexpr uint32_t kBootModeVtable = 0x82006B70;
constexpr uint32_t kBootModeVtable32 = 0x82006B64;
constexpr uint32_t kModeVtable = 0x820069EC;
constexpr uint32_t kModeVtable32 = 0x82006A0C;
constexpr uint32_t kDelegateVtable = 0x82010FA0;
constexpr uint32_t kMemberFunctorVtable = 0x82011918;

// The functions the plain C++ recognizes where pointers hold them.
constexpr uint32_t kStartMovie = 0x82107B90;
constexpr uint32_t kWaitForMovie = 0x82107C58;
constexpr uint32_t kEndMovie = 0x82107BF8;
constexpr uint32_t kNextMode = 0x82108018;
constexpr uint32_t kNextModeThunk = 0x82235C08;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
// The movie class's slots on our FFmpeg player (src/hooks/movies.cpp).
constexpr uint32_t kMoviePlay = 0x820AEC10;
constexpr uint32_t kMovieStart = 0x820AE4E0;
constexpr uint32_t kMovieStop = 0x820AE828;
constexpr uint32_t kMoviePause = 0x820AE898;
constexpr uint32_t kMovieResume = 0x820AE8F8;
constexpr uint32_t kMovieGetState = 0x820AEA80;
constexpr uint32_t kMovieRelease = 0x820AE528;

// The originals' frames (their stwu sizes).
constexpr uint32_t kInitFrame = 112;         // BootMode::Init
constexpr uint32_t kMachineInitFrame = 336;  // BootMachine::Init
constexpr uint32_t kStartMovieFrame = 96;
constexpr uint32_t kWaitForMovieFrame = 112;
constexpr uint32_t kStateFrame = 96;  // Movies::State
constexpr uint32_t kEndMovieFrame = 96;
constexpr uint32_t kNextModeFrame = 144;
constexpr uint32_t kDeletingDestructorFrame = 112;  // BootMode's
constexpr uint32_t kModeDeletingDestructorFrame = 96;
constexpr uint32_t kInvokeFrame = 112;  // ModeDelegate::Invoke

// What the plain C++ needs from the adapter: guest calls (`target` with r3
// and r4, from a frame at `sp`, returning to `lr`; r3 after), r13 (the PCR,
// for the last error XMP's dropped calls clear) and, for Movies::State,
// whether --skip_intro reports the movie finished (src/hooks/title_screen.cpp;
// may be empty).
struct Calls {
  std::function<uint64_t(uint32_t target, uint32_t sp, uint32_t lr, uint64_t r3, uint64_t r4)>
      call;
  uint32_t pcr = 0;
  // --play_movies_with_ffmpeg: the movie class's slots are our player's.
  bool ffmpeg_movies = false;
  std::function<bool()> finished_early;
};

// --- Movies:: (docs/functions/movies.md): the movie class's slots on the
// object at 0x826C1CDC; the FFmpeg player's plain C++ (movies/movie.h) where
// the slot holds its function and --play_movies_with_ffmpeg is on, else a
// guest call from `sp` (the caller's frame: they're tail calls) returning to
// `lr`. Each returns r3 as the slot leaves it (the object, for ours). ---

// sub_820A43A8 Movies::Play: slot 1 with the path of movie `id`.
uint64_t MoviesPlay(uint32_t id, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_820A43D8 Start (slot 2), sub_820A43F0 Stop (3), sub_820A4408 Pause
// (4), sub_820A4420 Resume (5), sub_820A4438 Release (7).
uint64_t MoviesStart(uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t MoviesStop(uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t MoviesPause(uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t MoviesResume(uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t MoviesRelease(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_820A4450 Movies::State: slot 6's answer, 0 to 3 kept, anything above
// the word at its frame's +80 (`frame`: its r1 after its stwu, the slot
// called from there); 1 at once when calls.finished_early says so.
uint64_t MoviesState(uint32_t frame, const Calls& calls);

// --- The boot machine's handlers (r3 the machine, r4 the dispatcher) ---

// sub_82107B90 Boot::StartMovie: XMP's playback taken (Dropped: only the
// last error it clears), Movies::Play(the table's next id), the index plus
// one, ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0). Returns r3 (FadeIn's,
// the passes word).
uint64_t StartMovie(uint32_t frame, const Calls& calls);
// sub_82107C58 Boot::WaitForMovie: see the row. Returns r3.
uint64_t WaitForMovie(uint32_t machine, uint32_t dispatcher, uint32_t frame,
                      const Calls& calls);
// sub_82107BF8 Boot::EndMovie: Movies::Stop and Release, then XMP's playback
// handed back (Dropped: the last error cleared, its answer 0). Returns r3 (0).
uint64_t EndMovie(uint32_t frame, const Calls& calls);

// sub_82107DD0 BootMachine::Init: the boot machine's three transitions, built
// in the original's frame at `frame`; returns the last StateMachine::Add's
// answer (`replaced`: the index, else the new transition).
uint32_t MachineInit(uint32_t machine, uint32_t frame, bool* replaced);
// sub_82107F68 BootMode::Init: BootMachine::Init on +848 (its frame below
// `frame`), +848 added to the dispatcher, the delegate at +8 set, the mode
// asked for 13 and the movie not paused. Returns BootMachine::Init's answer.
uint32_t Init(uint32_t mode, uint32_t frame, bool* replaced);
// sub_82108018 BootMode::NextMode: see the row; `answer` the 8-byte answer.
void NextMode(uint32_t answer, uint32_t mode, uint32_t frame, const Calls& calls);

// sub_82106D08 BootMode::~BootMode: the vtables put back (the empty
// sub_82106540 on +848 left out), then the base's.
void Destruct(uint32_t mode);
// sub_82106D68 BootMode::DeletingDestructor: ~BootMode, then operator delete
// (a guest call from `frame`) when bit 0 of `flags` is set.
void DeletingDestructor(uint32_t mode, uint32_t flags, uint32_t frame,
                        const Calls& calls);
// sub_82106E38 Mode::DeletingDestructor: the base's vtables, then operator
// delete when bit 0 of `flags` is set.
void ModeDeletingDestructor(uint32_t mode, uint32_t flags, uint32_t frame,
                            const Calls& calls);

// sub_82235C08 Mode::NextModeThunk: the mode's slot 6 (BootMode::NextMode's
// plain C++ for the boot mode's, from `sp` less its frame; else a guest call
// from `sp`). Returns r3.
uint64_t NextModeThunk(uint64_t answer, uint64_t mode, uint32_t sp, uint32_t lr,
                       const Calls& calls);
// sub_8223D440 ModeDelegate::Invoke: the delegate's function on (an answer at
// `frame` + 80, the object plus its adjustment), Mode::NextModeThunk's plain
// C++ where it holds it, then the 8 bytes copied to `answer`.
void ModeDelegateInvoke(uint32_t answer, uint32_t delegate, uint32_t frame,
                        const Calls& calls);

// sub_82107358 ScreenMachine::ScreenMachine: StateMachine::StateMachine, then
// +9600 to +9636 = 0.
void ScreenMachineConstruct(uint32_t machine);

}  // namespace boot_mode
