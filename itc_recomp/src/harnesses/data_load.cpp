// The data load at boot (src/data_load/, docs/functions/data-load.md)
// against the recompiled functions. The globals the functions write (the save
// defaults' bit sets, the session objects' slots) are the game's live ones:
// saved first and put back after the cases. Blocks the makers allocate are
// prefilled (as SaveData::Create's cases: the allocation layer's first fit
// hands the freed block back), compared by their bytes and freed. Everything
// is compared exactly (the records' floats are .rdata's 1.0 and 0.0 copied).

#include "harness.h"
#include "exit_table.h"

#include "base/flags.h"
#include "base/log.h"
#include "c_runtime/exit.h"
#include "data_load/data_load.h"
#include "hooks/app_start_up.h"
#include "hooks/data_load.h"
#include "hooks/archive.h"
#include "hooks/c_runtime.h"
#include "hooks/heap.h"
#include "memory/heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "memory/c_runtime_heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_data_load);
ITC_FLAG_DECLARE(bool, native_ui_movies_game);

namespace harness {

namespace {

namespace dl = data_load;

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& random) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(random.Next()));
  }
}

// The register steps of the functions without adapters.
void NativeBits224Fill(PPCContext& t, uint8_t* base) { dl::Bits224Fill(base, t.r3.u32); }
void NativeBits480Fill(PPCContext& t, uint8_t* base) { dl::Bits480Fill(base, t.r3.u32); }
void NativePart464(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Part464Construct(base, t.r3.u32);
}
void NativeMember4080(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Member4080Construct(base, t.r3.u32);
}
void NativeMember1136(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Member1136Construct(base, t.r3.u32);
}
void NativeRecord5232(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::Record5232Construct(base, t.r3.u32);
}
void NativeInputRecordingsConstruct(PPCContext& t, uint8_t* base) {
  t.fpscr.disableFlushMode();
  t.r3.u64 = dl::InputRecordingsConstruct(base, t.r3.u32);
}

// A fill (r3 = a set in the scratch area, random words around it).
void FillCase(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
          uint32_t words) {
  Results results(name);
  for (int c = 0; c < 8; c++) {
    const uint32_t set = h.data + 4 * h.random.Below((kScratchBytes - 4 * words) / 4);
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = set;
      (v == 0 ? recompiled : native)(t, h.base);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], fmt::format("set +{:X}", set - h.data));
  }
  results.Report();
}

void DefaultsInit(Context& h) {
  Results results("Defaults::Init (sub_820CCDC0)");
  const std::vector<uint8_t> saved = Read(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes);
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      if (c == 0) {
        harness::Fill(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes, 0);
      } else {
        WriteRandom(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes, random);
      }
      WriteRandom(h.base, h.data, kScratchBytes, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? __imp__sub_820CCDC0 : sub_820CCDC0)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      out[v].Bytes(Read(h.base, dl::kDefaultsFirst, dl::kDefaultsBytes));
      // The frame's five indices (the stack below the scratch area).
      out[v].Bytes(Read(h.base, t.r1.u32 - 128 + 80, 20));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], c == 0 ? "cleared" : fmt::format("random {}", c));
  }
  results.Report();
  Write(h.base, dl::kDefaultsFirst, saved.data(), uint32_t(saved.size()));
}

// A constructor on a heap block of `bytes` (random words), at `offset` in it.
void Constructor(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
                 uint32_t bytes) {
  Results results(name);
  const uint32_t room = bytes + 256;
  const auto block = memory::crt::OperatorNew(h.base, room);
  if (!block || *block == 0) {
    ITC_LOG_WARN("harness: data_load: {}: no block", name);
    return;
  }
  for (int c = 0; c < 6; c++) {
    const uint32_t offset = 4 * h.random.Below(64);
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, *block, room, random);
      if (v == 1) {
        h.random = random;
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = *block + offset;
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      out[v].Bytes(Read(h.base, *block, room));
      out[v].Word(t.r3.u32 - *block);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("at +{:X}", offset));
  }
  results.Report();
  memory::crt::OperatorDelete(*block);
}

// A block of `bytes` from operator new's plain C++, filled from `random` and
// freed again, for the next allocation of that size. The address, or 0.
uint32_t Prefill(Context& h, uint32_t bytes, Random& random) {
  const auto block = memory::crt::OperatorNew(h.base, bytes);
  if (!block || *block == 0) {
    return 0;
  }
  WriteRandom(h.base, *block, bytes, random);
  memory::crt::OperatorDelete(*block);
  return *block;
}

// A maker of a session object: `slot` its global, `bytes` the block's size.
void Maker(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
           uint32_t slot, uint32_t bytes) {
  Results results(name);
  const uint32_t saved = guest::LoadU32(h.base, slot);
  int reused = 0;
  for (int c = 0; c < 3; c++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      const uint32_t prefilled = Prefill(h, bytes, random);
      if (v == 1) {
        h.random = random;
      }
      guest::StoreU32(h.base, slot, 0xA5A5A5A5u);
      PPCContext t = CallContext(h);
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      const uint32_t made = guest::LoadU32(h.base, slot);
      out[v].Word(made == prefilled && made != 0 ? 1 : 0);
      reused += made == prefilled && made != 0;
      if (made != 0) {
        out[v].Bytes(Read(h.base, made, bytes));
      }
      out[v].Word(t.r3.u32 - made);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      if (made != 0) {
        memory::crt::OperatorDelete(made);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  ITC_LOG_INFO("harness: {}: {} of 6 runs got the prefilled block back", name, reused);
  results.Report();
  guest::StoreU32(h.base, slot, saved);
}

// --- The movie sets ---

// The globals the movie sets write: the two lists with their counts and the
// loading screen's shown flag, the nine words and the two after them, and
// UiMovie::SetRenderer's two.
struct Saved {
  uint32_t address;
  uint32_t bytes;
};
constexpr Saved kMovieGlobals[] = {{dl::kLoadingShown, 0x50},
                                   {dl::kOtherWords, 0x88},
                                   {dl::kOtherWord8130, 4},
                                   {ui_movies::kRenderer, 8}};

// LoadingScreen::Load and sub_82291328 with the lists' words random (the
// other set's count small), the movies made for real (the recompiled side
// with --native_ui_movies_game off, so its UiMovie::Load is the original's
// too), the loads recorded instead of queued: the globals with each new movie
// labeled, the movies' first 28 bytes and loaded flag, the calls (the files
// by whether they're pages) and r3; the movies and files freed, the globals
// put back.
void MovieSets(Context& h) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Saved& g : kMovieGlobals) {
    saved.push_back(Read(h.base, g.address, g.bytes));
  }
  for (const bool loading : {true, false}) {
    Results results(loading ? "LoadingScreen::Load (sub_82247B98; LoadingScreen::AddMovie sub_82247AA0)"
                            : "sub_82291328 (sub_822912A0)");
    const uint32_t list = loading ? dl::kLoadingMovies : dl::kOtherMovies;
    for (int c = 0; c < 4; c++) {
      std::vector<int64_t> serials;
      for (int i = 0; i < 4; i++) {
        serials.push_back(h.random.Below(4) == 0 ? -1 : int64_t(h.random.Below(1000000)));
      }
      std::vector<std::vector<uint32_t>> words;
      for (const Saved& g : kMovieGlobals) {
        std::vector<uint32_t> w;
        for (uint32_t k = 0; k < g.bytes; k += 4) {
          w.push_back(uint32_t(h.random.Next()));
        }
        words.push_back(w);
      }
      const uint32_t old_count = h.random.Below(4);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        for (size_t g = 0; g < std::size(kMovieGlobals); g++) {
          for (size_t k = 0; k < words[g].size(); k++) {
            guest::StoreU32(h.base, kMovieGlobals[g].address + 4 * uint32_t(k), words[g][k]);
          }
        }
        guest::StoreU32(h.base, list + 64, old_count);
        ArchiveLoads loads(serials);
        PPCContext t = CallContext(h);
        if (v == 0) {
          ITC_FLAG(native_ui_movies_game) = false;
          (loading ? __imp__sub_82247B98 : __imp__sub_82291328)(t, h.base);
          ITC_FLAG(native_ui_movies_game) = true;
        } else {
          (loading ? hooks::LoadingScreenLoadSteps : hooks::OtherMoviesLoadSteps)(t, h.base);
        }
        const uint32_t count = guest::LoadU32(h.base, list + 64);
        const uint32_t first = loading ? 0 : old_count;
        std::vector<uint32_t> movies;
        for (uint32_t i = first; i < count && i < 16; i++) {
          movies.push_back(guest::LoadU32(h.base, list + 4 * i));
        }
        const auto label = [&](uint32_t word) {
          const auto at = std::find(movies.begin(), movies.end(), word);
          return at != movies.end() && word != 0 ? 0x7E000000u + uint32_t(at - movies.begin()) : word;
        };
        for (const Saved& g : kMovieGlobals) {
          for (uint32_t k = 0; k < g.bytes; k += 4) {
            out[v].Word(label(guest::LoadU32(h.base, g.address + k)));
          }
        }
        const std::vector<uint32_t>& calls = loads.calls();
        out[v].Word(uint32_t(calls.size()));
        for (size_t k = 0; k < calls.size(); k++) {
          if (k % 6 == 1 && calls[k] != 0 && memory::IsPages(calls[k])) {
            out[v].Word(1);
            memory::FreePages(calls[k]);
            continue;
          }
          out[v].Word(label(calls[k]));
        }
        for (const uint32_t movie : movies) {
          if (movie == 0) {
            continue;
          }
          out[v].Bytes(Read(h.base, movie, 28));
          out[v].Word(guest::LoadU32(h.base, movie + ui_movies::kMovieLoaded));
          ui_movies::DestroyMovie(h.base, movie);
          memory::crt::OperatorDelete(movie);
        }
        out[v].Word(label(t.r3.u32));
        out[v].Word(uint32_t(t.r3.u64 >> 32));
      }
      results.Check(out[0], out[1], fmt::format("case {}: {} in the list", c, old_count));
    }
    results.Report();
  }
  for (size_t g = 0; g < std::size(kMovieGlobals); g++) {
    Write(h.base, kMovieGlobals[g].address, saved[g].data(), kMovieGlobals[g].bytes);
  }
}

// --- The load ---

// The callees' calls, recorded by their adapters (hooks::StartUpRecorded)
// and answered with random words instead of running.
struct BootCalls {
  std::vector<uint32_t> targets;
  std::vector<uint32_t> answers;
  size_t next = 0;
  std::vector<uint32_t> made;  // each call's address and r3
};
BootCalls* g_boot_calls = nullptr;

bool RecordBootCall(PPCContext& t, uint32_t address) {
  BootCalls* calls = g_boot_calls;
  if (calls == nullptr ||
      std::find(calls->targets.begin(), calls->targets.end(), address) == calls->targets.end()) {
    return false;
  }
  calls->made.push_back(address);
  calls->made.push_back(t.r3.u32);
  t.r3.u64 = calls->next < calls->answers.size() ? calls->answers[calls->next++] : 0;
  return true;
}

// BootData::CreateObjects and BootData::Load (its once flag 0 or set) with
// every callee recorded instead of run: the calls in order, the flag and r3.
void Load(Context& h) {
  const uint32_t saved_flag = guest::LoadU32(h.base, dl::kLoaded);
  const auto saved_recorder = hooks::g_start_up_recorder;
  for (const bool load : {false, true}) {
    Results results(load ? "BootData::Load (sub_82106618)" : "BootData::CreateObjects (sub_82105868)");
    BootCalls calls;
    calls.targets = load ? std::vector<uint32_t>{0x820A08A0, 0x8210ADF8, 0x82105868, 0x82247B98,
                                                 0x82291328, 0x820C9960, 0x820CC2A8}
                         : std::vector<uint32_t>{0x821057C8, 0x82102208, 0x821021A8, 0x820CCDC0,
                                                 0x82105508, 0x82105560, 0x82116DB0, 0x82117240};
    size_t made = 0;
    for (int c = 0; c < 8; c++) {
      std::vector<uint32_t> answers;
      for (int i = 0; i < 8; i++) {
        answers.push_back(uint32_t(h.random.Next()));
      }
      const uint32_t flag = c % 2 == 0 ? 0 : uint32_t(h.random.Next()) | 1;
      const uint64_t r3 = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        calls.answers = answers;
        calls.next = 0;
        calls.made.clear();
        guest::StoreU32(h.base, dl::kLoaded, flag);
        g_boot_calls = &calls;
        hooks::g_start_up_recorder = RecordBootCall;
        PPCContext t = CallContext(h);
        t.r3.u64 = r3;
        if (load) {
          (v == 0 ? __imp__sub_82106618 : hooks::BootDataLoadSteps)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_82105868 : hooks::CreateObjectsSteps)(t, h.base);
        }
        hooks::g_start_up_recorder = saved_recorder;
        g_boot_calls = nullptr;
        made = std::max(made, calls.made.size() / 2);
        out[v].Word(uint32_t(calls.made.size()));
        for (const uint32_t w : calls.made) {
          out[v].Word(w);
        }
        out[v].Word(guest::LoadU32(h.base, dl::kLoaded));
        out[v].Word(t.r3.u32 - t.r1.u32);
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        out[v].Word(t.r1.u32);
        out[v].Word(uint32_t(t.lr));
      }
      results.Check(out[0], out[1], fmt::format("case {}: the flag {:X}", c, flag));
    }
    ITC_LOG_INFO("harness: data_load: {}'s cases record up to {} calls a run",
                 load ? "BootData::Load" : "BootData::CreateObjects", made);
    results.Report();
  }
  guest::StoreU32(h.base, dl::kLoaded, saved_flag);
}

// --- The static replays' set-ups (on the game's static objects and exit
// table, each put back after a side; the recompiled side with the flag off,
// so its Record5232 calls are the recompiled ones too). ---

void StaticReplayInit(Context& h, const char* name, GuestFunction recompiled,
                      GuestFunction native, uint32_t object) {
  Results results(name);
  std::lock_guard<std::recursive_mutex> lock(c_runtime::ExitMutex());
  // The records, and 256 bytes on each side of them.
  const uint32_t from = object + 0x2BF10 - 256;
  const uint32_t bytes = 7 * dl::kRecordBytes + 512;
  const std::vector<uint8_t> saved = Read(h.base, from, bytes);
  for (int c = 0; c < 4; c++) {
    Outputs out[2];
    const Random start = h.random;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      if (c == 0) {
        Write(h.base, from, saved.data(), bytes);
      } else {
        WriteRandom(h.base, from, bytes, random);
      }
      if (v == 1) {
        h.random = random;
      }
      const uint32_t mark = exit_table::Mark(h.base);
      PPCContext t = CallContext(h);
      ITC_FLAG(native_data_load) = v == 1;
      (v == 0 ? recompiled : native)(t, h.base);
      ITC_FLAG(native_data_load) = true;
      out[v].Bytes(Read(h.base, from, bytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      exit_table::Take(h.base, mark, out[v]);
    }
    results.Check(out[0], out[1], c == 0 ? "as at boot" : fmt::format("random {}", c));
  }
  Write(h.base, from, saved.data(), bytes);
  results.Report();
}

void StaticReplayInits(Context& h) {
  if (!hooks::NativeAtexit()) {
    ITC_LOG_WARN("harness: data_load: native atexit off: the static replays' cases skipped");
    return;
  }
  StaticReplayInit(h, "Replay826D8520::StaticInit (sub_825D9E60)", __imp__sub_825D9E60,
                   sub_825D9E60, dl::kStaticReplayA);
  StaticReplayInit(h, "Replay82816E50::StaticInit (sub_825D9F68)", __imp__sub_825D9F68,
                   sub_825D9F68, dl::kStaticReplayB);
}

}  // namespace

void DataLoad(Context& h) {
  FillCase(h, "Bits224::Fill (sub_820CCD10)", __imp__sub_820CCD10, NativeBits224Fill, 7);
  FillCase(h, "Bits480::Fill (sub_820CCD68)", __imp__sub_820CCD68, NativeBits480Fill, 15);
  DefaultsInit(h);
  Constructor(h, "Part464::Part464 (sub_821042A0)", __imp__sub_821042A0, NativePart464, 464);
  Constructor(h, "Member4080::Member4080 (sub_82104690)", __imp__sub_82104690, NativeMember4080,
              1152);
  Constructor(h, "Member1136::Member1136 (sub_82102888)", __imp__sub_82102888, NativeMember1136,
              2944);
  Constructor(h, "Record5232::Record5232 (sub_82104C20)", __imp__sub_82104C20, NativeRecord5232,
              dl::kRecordBytes);
  Constructor(h, "InputRecordings::InputRecordings (sub_821054A8)", __imp__sub_821054A8,
              NativeInputRecordingsConstruct, dl::kInputRecordingsBytes);
  StaticReplayInits(h);
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: data_load: the makers need --native_heap");
    return;
  }
  Maker(h, "RaceData::Create (sub_82102208)", __imp__sub_82102208, sub_82102208,
        dl::kRaceDataSlot, dl::kRaceDataBytes);
  Maker(h, "InputRecordings::Create (sub_82105508)", __imp__sub_82105508, sub_82105508,
        dl::kInputRecordingsSlot, dl::kInputRecordingsBytes);
  Maker(h, "Replay::Create (sub_82105560)", __imp__sub_82105560, sub_82105560, dl::kReplaySlot,
        dl::kReplayBytes);
  if (!hooks::NativeArchiveQueue()) {
    ITC_LOG_WARN("harness: data_load: the movie sets and the load need the archive's queue native");
    return;
  }
  MovieSets(h);
  Load(h);
}

}  // namespace harness
