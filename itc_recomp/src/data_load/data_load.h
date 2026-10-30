// The data load at boot natively (docs/functions/data-load.md): plain C++
// over the guest layout. The adapters are in src/hooks/data_load.cpp.

#pragma once

#include "archive/archive.h"
#include "c_runtime/exit.h"

#include <cstdint>
#include <functional>
#include <optional>

namespace data_load {

// .rdata's floats the constructors load with lfs and store with stfs.
constexpr uint32_t kOneFloat = 0x82000A30;   // 1.0
constexpr uint32_t kZeroFloat = 0x82000A34;  // 0.0

// --- The save defaults' bit sets ---

constexpr uint32_t kDefaultsBase = 0x826C4064;  // the last set, and Defaults::Init's r3
constexpr uint32_t kTable3044 = 0x826C3044;     // 15 words
constexpr uint32_t kSets3080 = 0x826C3080;      // 48 sets of 15 words, 60 bytes apart
constexpr uint32_t kTable3C24 = 0x826C3C24;     // 7 words
constexpr uint32_t kTable50A8 = 0x826C50A8;     // 15 words
constexpr uint32_t kTable50E4 = 0x826C50E4;     // 7 words
// Everything Defaults::Init writes: 0x826C3044 to 0x826C5100.
constexpr uint32_t kDefaultsFirst = kTable3044;
constexpr uint32_t kDefaultsBytes = kTable50E4 + 28 - kTable3044;

// sub_820CCD10 Bits224::Fill: the 7 words at `set` cleared, then bits 0-198
// set.
void Bits224Fill(uint8_t* base, uint32_t set);
// sub_820CCD68 Bits480::Fill: the 15 words at `set` cleared, then bits 0-453
// set.
void Bits480Fill(uint8_t* base, uint32_t set);
// sub_820CCDC0 Defaults::Init: the save defaults' bit sets filled (each
// cleared first, so what they hold after doesn't depend on before): the
// table at 0x826C3044, 0x826C3C24 (bits 0-60), 0x826C50E4 (Bits224::Fill),
// 0x826C50A8, the 48 sets at 0x826C3080 (a range each by their index, none
// for 29), and 0x826C4064 (cleared, ORed with sets 17, 35, 38, 41 and 47,
// Bits480::Fill, cleared again: it ends 0). `frame` is the original's
// 128-byte frame: its +80 to +96 get the five set indices (17, 35, 38, 41,
// 47), as the original leaves them. Returns 0x826C4064 (the original's r3).
uint32_t DefaultsInit(uint8_t* base, uint32_t frame);

// --- The 5,232-byte records ---

constexpr uint32_t kRecordBytes = 5232;

// sub_821042A0 Part464::Part464: 0.0 into its float fields. Returns `part`.
uint32_t Part464Construct(uint8_t* base, uint32_t part);
// sub_82104690 Member4080::Member4080: Part464 on +16 and +480, then 0.0
// into +956, +972, +1068, +1084, +1116 and +1148. Returns `member`.
uint32_t Member4080Construct(uint8_t* base, uint32_t member);
// sub_82102888 Member1136::Member1136: 0.0 into its float fields. Returns
// `member` (r3 as the original leaves it).
uint32_t Member1136Construct(uint8_t* base, uint32_t member);
// sub_82104C20 Record5232::Record5232: 1.0 into +156 to +204 and four rows'
// four words (+252 to +588), 0.0 into fourteen words from +636 to +1116,
// then Member1136 (+1136) and Member4080 (+4080). Returns `record`.
uint32_t Record5232Construct(uint8_t* base, uint32_t record);

// --- The session's objects ---

constexpr uint32_t kRaceDataBytes = 0xA3B00;
constexpr uint32_t kRaceDataSlot = 0x826C5B2C;

// sub_82102208 RaceData::Create: 0xA3B00 bytes (operator new's plain C++)
// and, when not null, RaceData::RaceData (race_setup's plain C++); the block,
// or 0, into 0x826C5B2C. Returns it (r3 as the original leaves it), or
// nullopt where operator new's plain C++ can't answer (the original runs;
// nothing written yet).
std::optional<uint32_t> RaceDataCreate(uint8_t* base);

constexpr uint32_t kInputRecordingsBytes = 0x2225C0;
constexpr uint32_t kInputRecordingsSlot = 0x826C5B4C;
constexpr uint32_t kReplayBytes = 0x13C8F0;
constexpr uint32_t kReplaySlot = 0x826C5B50;

// sub_821054A8 InputRecordings::InputRecordings: Record5232 on seven records
// from +0x20FE90 and seven from +0x2196A0. Returns `block`.
uint32_t InputRecordingsConstruct(uint8_t* base, uint32_t block);
// sub_82105508 InputRecordings::Create: 0x2225C0 bytes (operator new's plain
// C++), InputRecordings::InputRecordings when not null; the block, or 0, into
// 0x826C5B4C, then the word at block + 0x2225B0 = 0 (at 0x2225B0 when the
// allocation failed, as the original). Returns the block (r3), or nullopt
// where operator new's plain C++ can't answer (the original runs; nothing
// written yet).
std::optional<uint32_t> InputRecordingsCreate(uint8_t* base);
// sub_82105560 Replay::Create: 0x13C8F0 bytes, Record5232 on seven records
// from +0x2BF10 when not null; the block, or 0, into 0x826C5B50, then the
// word at block + 0x2B600 = 1 (at 0x2B600 when the allocation failed).
// Returns r3 as the original leaves it (the last record, or 0), or nullopt
// where operator new's plain C++ can't answer.
std::optional<uint32_t> ReplayCreate(uint8_t* base);

// The two static objects with Replay::Create's layout (C runtime static
// set-ups, no direct callers: _cinit's initializer tables, once at boot).
constexpr uint32_t kStaticReplayA = 0x826D8520;  // its records from +0x2BF10
constexpr uint32_t kStaticReplayB = 0x82816E50;
constexpr uint32_t kStaticReplayAExit = 0x825DB170;  // their clean-ups
constexpr uint32_t kStaticReplayBExit = 0x825DB1B0;
// sub_825D9E60 Replay826D8520::StaticInit (0x826D8520, clean-up 0x825DB170)
// and sub_825D9F68 Replay82816E50::StaticInit (0x82816E50, 0x825DB1B0):
// Record5232 on the seven records from the object + 0x2BF10, then the
// clean-up registered with atexit (natively, else through `call`). Return
// atexit's r3. The frame's saved words (the link register, r30 and r31 below
// r1) aren't written.
int32_t StaticReplayAInit(uint8_t* base, const c_runtime::AtexitCall& call);
int32_t StaticReplayBInit(uint8_t* base, const c_runtime::AtexitCall& call);

// --- The two UI movie sets (the loading screen's and another) ---

constexpr uint32_t kMovieBytes = 40976;
constexpr uint32_t kLoadingMovies = 0x82815E68;  // the list; its count at +64
constexpr uint32_t kLoadingShown = 0x82815E60;
constexpr uint32_t kOtherMovies = 0x82816610;    // the list; its count at +64
constexpr uint32_t kOtherMovieIds = 0x82638134;  // three asset ids
constexpr uint32_t kOtherWords = 0x828165D0;     // nine words
constexpr uint32_t kOtherWord8600 = 0x82816600;
constexpr uint32_t kOtherWord8130 = 0x82638130;
constexpr uint32_t kRenderApp = 0x826BABA0;

// What the movie sets call: UiMovie::UiMovie (its plain C++, the original
// where it hands back; r3 the movie), Archive::Load, and the renderer's
// objects (`render_object`: [0x826BABA0]'s slot 8's answer's slot 1 for
// `which` 0, the command list, slot 2 for 1, the renderer: guest calls).
struct MovieCalls {
  std::function<uint32_t(uint32_t movie)> construct;
  archive::LoadCall load;
  std::function<uint32_t(int which)> render_object;
};

// sub_82247AA0 LoadingScreen::AddMovie (the list at 0x82815E68) and
// sub_822912A0 (the other set's, 0x82816610): a 40,976-byte movie (operator
// new's plain C++; UiMovie::UiMovie when not null, else 0), sub_8211A9E8 on
// it, UiMovie::Load(movie, id), the movie appended to the list (its count at
// +64 up by one). Returns UiMovie::Load's serial (r3), or nullopt where
// operator new's plain C++ can't answer (nothing written yet: the original
// runs).
std::optional<int64_t> LoadingScreenAddMovie(uint8_t* base, uint32_t id, const MovieCalls& calls);
std::optional<int64_t> OtherMoviesAdd(uint8_t* base, uint32_t id, const MovieCalls& calls);
// `add` for a set's caller: MovieListAdd, or the original where it hands back.
using MovieAdd = std::function<int64_t(uint32_t id)>;
// sub_82247B98 LoadingScreen::Load: the loading screen's count (+64) and
// shown flag (0x82815E60) = 0, then `add` with assets 22575, 22574 and
// 22576. Returns the last serial (r3).
int64_t LoadingScreenLoad(uint8_t* base, const MovieAdd& add);
// sub_82291328: `add` with the three ids at 0x82638134, nine words at
// 0x828165D0, 0x82816600 and 0x82638130 = 0, then UiMovie::SetRenderer(the
// renderer, the command list) (ui_movies' plain C++). Returns r3 as the
// original leaves it (the renderer: SetRenderer leaves it).
uint32_t OtherMoviesLoad(uint8_t* base, const MovieAdd& add,
                         const std::function<uint32_t(int which)>& render_object);

// --- The load ---

constexpr uint32_t kLoaded = 0x826C5B8C;  // BootData::Load's once flag

// A callee through its adapter (a guest call from the original's frame with
// its return address; r3 when given): its r3.
using BootCall = std::function<uint32_t(uint32_t target, std::optional<uint32_t> r3,
                                        uint32_t return_address)>;
// sub_82105868 BootData::CreateObjects: SaveData::Create(0), RaceData::Create,
// RaceData::ResetGlobal, Defaults::Init, InputRecordings::Create,
// Replay::Create, DataTables::Init and DataTables::Load, each through `call`.
// Returns the last one's r3.
uint32_t CreateObjects(uint8_t* base, const BootCall& call);
// sub_82106618 BootData::Load: once (0x826C5B8C 0, then 1): Shaders::LoadAll,
// Text::Load, BootData::CreateObjects, LoadingScreen::Load, sub_82291328,
// Params::Init and Params::Load, each through `call`; both GlobalMemoryStatus
// calls (sub_82294DA0, Dropped: their buffer at `frame` + 80 is never read)
// left out. `frame` is the original's 128-byte frame. Returns r3 as the
// original leaves it (`frame` + 80), or nullopt when the flag was set (nothing
// done; r3 as it came).
std::optional<uint32_t> BootDataLoad(uint8_t* base, uint32_t frame, const BootCall& call);

}  // namespace data_load
