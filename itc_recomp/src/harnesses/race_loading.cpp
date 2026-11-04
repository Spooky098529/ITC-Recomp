// The race's loading (src/race_loading/, docs/functions/race-loading.md)
// against the recompiled functions. The originals run with
// --native_race_loading off, the plain C++ through the register steps
// (hooks/race_loading.h). Each case fills its blocks (the machine, the
// dispatcher) and the stack below the call's stack pointer with random words
// from the case's seed, shapes the fields the function tests, runs both
// versions, and compares the blocks, the globals they write (the screen
// fades', saved and put back after each side), the frames' back chains and
// r3, all exactly (nothing here is a float). Compared with harness::Outputs.
//
// CourseSetup::Init as RaceFlow::Init's cases (src/harnesses/race_flow.cpp):
// an empty machine or one with random transitions already, some with the
// keys Init adds; its frame (but its saved registers) compared too.
// WaitDraw's Archive::QueueLength reads the live queue: its cases hold the
// live queue lock (the loader thread can't change the queue meanwhile),
// take the archive's test semaphores on this thread (so the length's own
// lock doesn't wait on the held one), and set the queue's count for the
// case, put back after.
//
// The loading screen (Show, Run, Finish) in a UI movie world of the case's
// own, built as the in-race screens' harness builds it
// (src/harnesses/race_screens.cpp): the three movies the loading screen's
// words point to, their live, saved and file scenes one tree's root whose
// entries are TXT leaves (Show's nodes 2, 3, 9 and 10 are the root's), the
// chunks laid out one after another (each +4 its size) up to a TRM chunk for
// UiMovie::SetFrames; empty animation lists, 256 named animations over the
// root and one of 8 animations of no tracks; texts ids 1 to 7 and the
// string table of letters only (no argument codes: SetText's r7 to r10 are
// 0 from ours, the original's leftovers, so its frame's +48 to +79 are left
// out). The callees run for real on both sides; UiMovie::Run's update is the
// UI movies' stand-in's, recorded and answered; GetLocalTime answers with a
// made-up time (time_hooks::SetFakeNow).

#include "harness.h"

#include "hooks/ui_movies.h"
#include "ui_movies/game.h"

#include "archive/archive.h"
#include "archive/semaphores.h"
#include "base/flags.h"
#include "base/log.h"
#include "game_modes/game_modes.h"
#include "hooks/archive.h"
#include "hooks/race_loading.h"
#include "memory/heap.h"
#include "race_loading/race_loading.h"

#include <fmt/format.h>

#include <iterator>
#include <optional>
#include <vector>

ITC_FLAG_DECLARE(bool, native_race_loading);

namespace time_hooks {
void SetFakeNow(uint64_t now);
void ClearFakeNow();
}  // namespace time_hooks

namespace harness {

namespace {

namespace rl = race_loading;

constexpr uint32_t kMachineBytes = 0x2600;
constexpr uint32_t kDispatcherBytes = 0x400;
constexpr uint32_t kStackBytes = 1024;  // below the call's stack pointer
// The screen fades' words (0x826A50AC to 0x826A50EB: both layers'
// durations, colors, alphas and counts).
constexpr uint32_t kFades = 0x826A50AC;
constexpr uint32_t kFadesBytes = 0x40;

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t bytes;
  uint32_t address;
};

void WriteRandom(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

// Runs the recompiled version (v == 0, with the flag off) or the register
// step.
void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  if (v == 0) {
    ITC_FLAG(native_race_loading) = false;
    recompiled(t, base);
    ITC_FLAG(native_race_loading) = true;
  } else {
    native(t, base);
  }
}

void Inits(Context& h, const Block& machine) {
  namespace gm = game_modes;
  constexpr uint32_t kSaves = 56;  // __savegprlr_26: r26 at -56, lr at -8
  Results results("CourseSetup::Init (sub_821517D8)");
  const uint32_t frame = CallContext(h).r1.u32 - rl::kInitFrame;
  // Keys Init adds, planted in the odd cases.
  constexpr uint32_t kPlanted[][2] = {{1, 0}, {4, 4}, {7, 49}, {37, 50}};
  for (uint64_t seed = 0; seed < 12; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Random r(seed * 157 + 71);
      WriteRandom(h.base, h.data, kScratchBytes, r);
      WriteRandom(h.base, machine.address, machine.bytes, r);
      WriteRandom(h.base, frame, rl::kInitFrame, r);
      if (seed % 2 == 0) {
        gm::StateMachineConstruct(h.base, machine.address);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, 0);
      } else {
        const uint32_t count = 8 + r.Below(180);
        guest::StoreU32(h.base, machine.address + gm::kTransitionCount, count);
        for (uint32_t i = 0; i < 200; i++) {  // keys of small numbers, some matching
          for (uint32_t k = 0; k < 4; k++) {
            guest::StoreU32(h.base, machine.address + 48 * i + 4 * k,
                            r.Below(k < 2 ? 60 : 2));
          }
        }
        for (const auto& key : kPlanted) {
          if (seed % 4 == 1 && key[0] == 37) {
            continue;  // seeds 1, 5 and 9: the last Add makes a new transition
          }
          const uint32_t at = machine.address + 48 * r.Below(count);
          guest::StoreU32(h.base, at, key[0]);
          guest::StoreU32(h.base, at + 4, key[1]);
          guest::StoreU32(h.base, at + 8, 0);
          guest::StoreU32(h.base, at + 12, 0);
        }
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = machine.address;
      Run(v, __imp__sub_821517D8, hooks::CourseSetupInit, t, h.base);
      out[v].Bytes(Read(h.base, machine.address, machine.bytes));
      out[v].Bytes(Read(h.base, frame + 80, rl::kInitFrame - 80 - kSaves));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

// The dispatcher's ring's count and start: small (within the 50 slots, the
// count up to 49 so a post fits), at the wrap, or any word.
void ShapeRing(uint8_t* base, uint32_t dispatcher, Random& r) {
  const uint32_t ring = dispatcher + rl::kRing;
  const uint32_t kind = r.Below(6);
  uint32_t count, start;
  if (kind == 0) {
    count = uint32_t(r.Next());
    start = uint32_t(r.Next());
  } else if (kind == 1) {
    count = 49;
    start = 49;
  } else {
    count = r.Below(50);
    start = r.Below(50);
  }
  guest::StoreU32(base, ring + rl::kRingCount, count);
  guest::StoreU32(base, ring + rl::kRingStart, start);
}

// The live queue's count set for a case: the live lock held and the
// archive's test semaphores taken on this thread while alive (see the top);
// the count put back at the end. Only with the queue native and its
// semaphores host ones (hooks::NativeArchiveQueue()).
class QueueControl {
 public:
  explicit QueueControl(Context& h) : h_(h) {
    PPCContext t = CallContext(h_);
    t.r3.u64 = archive::kQueueLock;
    sub_82592670(t, h_.base);  // Semaphore::Wait: the live lock
    scope_.emplace(h_.base);
    scope_->Set().queue_lock.SetCount(h_.base, 1);
    saved_ = guest::LoadU32(h_.base, archive::kQueue + 8);
  }
  ~QueueControl() {
    guest::StoreU32(h_.base, archive::kQueue + 8, saved_);
    scope_.reset();
    PPCContext t = CallContext(h_);
    t.r3.u64 = archive::kQueueLock;
    sub_825926E8(t, h_.base);  // Semaphore::Signal
  }
  QueueControl(const QueueControl&) = delete;
  QueueControl& operator=(const QueueControl&) = delete;
  void Set(uint32_t count) {
    scope_->Set().queue_lock.SetCount(h_.base, 1);
    guest::StoreU32(h_.base, archive::kQueue + 8, count);
  }

 private:
  Context& h_;
  std::optional<archive::ScopedTestSemaphores> scope_;
  uint32_t saved_ = 0;
};

// The handlers: r3 the machine, r4 the dispatcher; the machine, the
// dispatcher, the fades' words, the frame's back chain (`frame` bytes below
// the stack pointer, 0 for the leaves) and r3 compared.
struct Handler {
  const char* name;
  GuestFunction recompiled, native;
  uint32_t frame;
};

void Handlers(Context& h, const Block& machine, const Block& dispatcher) {
  const Handler handlers[] = {
      {"CourseSetup::Start (sub_82151408)", __imp__sub_82151408, hooks::CourseSetupStart, 112},
      {"CourseSetup::Begin (sub_82150DE0)", __imp__sub_82150DE0, hooks::CourseSetupBegin, 0},
      {"CourseSetup::OnCourseReady (sub_82150DD0)", __imp__sub_82150DD0,
       hooks::CourseSetupOnCourseReady, 0},
      {"CourseSetup::WaitDraw (sub_821515A0)", __imp__sub_821515A0, hooks::CourseSetupWaitDraw,
       96},
  };
  const auto saved_fades = Read(h.base, kFades, kFadesBytes);
  const bool queue_native = hooks::NativeArchiveQueue();
  if (!queue_native) {
    ITC_LOG_INFO("harness: race_loading: the archive's queue isn't native: WaitDraw's cases "
                 "with step 0 skipped");
  }
  for (const Handler& handler : handlers) {
    const bool wait_draw = handler.native == hooks::CourseSetupWaitDraw;
    Results results(handler.name);
    std::optional<QueueControl> queue;
    if (wait_draw && queue_native) {
      queue.emplace(h);
    }
    for (uint64_t seed = 0; seed < 200; seed++) {
      // WaitDraw's shape: the step 0, 1 or any word, layer 0's alpha 255 or
      // not, the queue's count 0 or not.
      Random shape(seed * 977 + 13);
      const uint32_t steps[3] = {0, 1, uint32_t(shape.Next())};
      uint32_t step = steps[shape.Below(3)];
      if (step == 0 && wait_draw && !queue_native) {
        step = 1;
      }
      const uint32_t alpha = shape.Below(2) ? 255 : (shape.Below(2) ? shape.Below(256)
                                                                    : uint32_t(shape.Next()));
      const uint32_t queued = shape.Below(2) ? 0 : 1 + shape.Below(20);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Random r(seed * 131 + 7);
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
        WriteRandom(h.base, machine.address, machine.bytes, r);
        WriteRandom(h.base, dispatcher.address, dispatcher.bytes, r);
        WriteRandom(h.base, kFades, kFadesBytes, r);
        ShapeRing(h.base, dispatcher.address, r);
        guest::StoreU32(h.base, machine.address + rl::kDrawStep, step);
        guest::StoreU32(h.base, rl::kLayer0Alpha, alpha);
        if (queue) {
          queue->Set(queued);
        }
        t.r3.u64 = machine.address;
        t.r4.u64 = dispatcher.address;
        Run(v, handler.recompiled, handler.native, t, h.base);
        out[v].Bytes(Read(h.base, machine.address, machine.bytes));
        out[v].Bytes(Read(h.base, dispatcher.address, dispatcher.bytes));
        out[v].Bytes(Read(h.base, kFades, kFadesBytes));
        if (handler.frame != 0) {
          out[v].Word(guest::LoadU32(h.base, sp - handler.frame));
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        Write(h.base, kFades, saved_fades.data(), kFadesBytes);
      }
      results.Check(out[0], out[1],
                    fmt::format("seed {}: step {:X} alpha {:X} queued {}", seed, step, alpha,
                                queued));
    }
    results.Report();
  }
}

// --- The loading screen ---

constexpr uint32_t kMovieHeaderBytes = 0xA000;
constexpr uint32_t kMovieCount = 3;
constexpr uint32_t kTreeNodeBytes = 0x400;
constexpr uint32_t kLeaves = 32, kAnimations = 8;
constexpr uint32_t kRootAt = kMovieCount * kMovieHeaderBytes;
constexpr uint32_t kLeavesAt = kRootAt + kTreeNodeBytes;
constexpr uint32_t kTrmAt = kLeavesAt + kLeaves * kTreeNodeBytes;
constexpr uint32_t kAnimationsAt = kTrmAt + 0x100;
constexpr uint32_t kTexturesAt = kAnimationsAt + kAnimations * 64;
constexpr uint32_t kMovieWorldBytes = kTexturesAt + 0x100;
constexpr uint32_t kTextsPerMovie = 8;
constexpr uint32_t kTextBuffersAt = 0x1000;
constexpr uint32_t kTextWorldBytes = kTextBuffersAt + kMovieCount * kTextsPerMovie * 0x1000;
constexpr uint32_t kStringWorldBytes = 0x10000;
constexpr uint32_t kMissingText = 0x826C5CD8;  // TextMap::Find's answer when not found
constexpr uint32_t kStringTable = 0x826C5C34;
constexpr uint32_t kTagTxt = 0x54585400;
constexpr uint32_t kTagSce = 0x53434500;
constexpr uint32_t kTagTrm = 0x54524D00;
constexpr uint32_t kTagPrm = 0x50524D00;
constexpr uint32_t kTagPic = 0x50494300;

struct MovieWorld {
  MovieWorld() : movies(kMovieWorldBytes), texts(kTextWorldBytes), strings(kStringWorldBytes) {}
  bool ok() const { return movies.address && texts.address && strings.address; }
  uint32_t movie(uint32_t m) const { return movies.address + kMovieHeaderBytes * m; }
  uint32_t root() const { return movies.address + kRootAt; }
  uint32_t leaf(uint32_t k) const { return movies.address + kLeavesAt + kTreeNodeBytes * k; }
  uint32_t animation(uint32_t k) const { return movies.address + kAnimationsAt + 64 * k; }
  Block movies;
  Block texts;
  Block strings;
};

// The string table: 17 files' pointers to one container of entries enough
// for the widest file (the bounds at 0x82007E18), each one of 8 strings of
// letters (no argument codes), as the in-race screens' harness builds it.
void BuildStringTable(uint8_t* base, const MovieWorld& w) {
  constexpr uint32_t kBounds = 0x82007E18;
  uint32_t span = 1;
  for (uint32_t i = 0; i < 17; i++) {
    const int32_t from = int32_t(guest::LoadU32(base, kBounds + 4 * i));
    const int32_t to = int32_t(guest::LoadU32(base, kBounds + 4 * i + 4));
    if (to > from && uint32_t(to - from) > span) {
      span = uint32_t(to - from);
    }
  }
  const uint32_t at = w.strings.address, container = at + 0x100;
  if (container + 4 + 4 * span + 8 * 64 > at + w.strings.bytes) {
    ITC_LOG_ERROR("harness: race_loading: a file of {} strings doesn't fit", span);
    span = (w.strings.bytes - 0x100 - 8 * 64 - 4) / 4;
  }
  for (uint32_t i = 0; i < 18; i++) {
    guest::StoreU32(base, at + 4 * i, container);
  }
  guest::StoreU32(base, container, __builtin_bswap32(span));
  Random r(0x4C6F6164ull);  // "Load"
  const uint32_t text = container + 4 + 4 * span;
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t length = r.Below(30);
    for (uint32_t c = 0; c < length; c++) {
      guest::StoreU16(base, text + 64 * k + 2 * c, uint16_t('A' + r.Below(26)));
    }
    guest::StoreU16(base, text + 64 * k + 2 * length, 0);
  }
  for (uint32_t i = 0; i < span; i++) {
    guest::StoreU32(base, container + 4 + 4 * i,
                    __builtin_bswap32(text + 64 * r.Below(8) - container));
  }
}

void ShapeMovieWorld(uint8_t* base, const MovieWorld& w, Random& r) {
  WriteRandom(base, w.movies.address, w.movies.bytes, r);
  WriteRandom(base, w.texts.address, w.texts.bytes, r);
  for (uint32_t m = 0; m < kMovieCount; m++) {
    const uint32_t movie = w.movie(m);
    ui_movies::InitAnimationLists(base, movie);
    for (const uint32_t field : {16u, 20u, 24u}) {
      guest::StoreU32(base, movie + field, w.root());
    }
    guest::StoreU32(base, movie + 28, w.movies.address + kTexturesAt);
    for (uint32_t k = 0; k < 256; k++) {
      const uint32_t named = movie + 176 + 36 * k;
      guest::StoreU32(base, named + 28, w.root());
      guest::StoreU32(base, named + 32, w.animation(r.Below(kAnimations)));
    }
    // The texts: ids 1 to 7, each a record of a 4 KB buffer of its own.
    for (uint32_t k = 0; k < kTextsPerMovie; k++) {
      const uint32_t record = w.texts.address + 0x100 * m + 32 * k;
      const uint32_t buffer = w.texts.address + kTextBuffersAt + 0x1000 * (kTextsPerMovie * m + k);
      guest::StoreU32(base, record, buffer);
      guest::StoreU32(base, record + 20, r.Below(5) - 1);
      const uint32_t length = r.Below(60);
      for (uint32_t c = 0; c < length; c++) {
        guest::StoreU16(base, buffer + 2 * c,
                        r.Below(6) == 0 ? 0xFFD1 : uint16_t('a' + r.Below(26)));
      }
      guest::StoreU16(base, buffer + 2 * length, 0);
      if (k + 1 < kTextsPerMovie) {
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k, k + 1);
        guest::StoreU32(base, movie + ui_movies::kMovieTexts + 8 * k + 4, record);
      }
    }
    guest::StoreU32(base, movie + ui_movies::kMovieTexts + 1024, kTextsPerMovie - 1);
    guest::StoreU32(base, movie + ui_movies::kMovieTextCount, kTextsPerMovie);
  }
  guest::StoreU32(base, kMissingText, w.texts.address + 32 * (kTextsPerMovie - 1));
  for (uint32_t k = 0; k < kAnimations; k++) {
    for (uint32_t i = 0; i < 64; i += 4) {
      guest::StoreU32(base, w.animation(k) + i, 0);
    }
    guest::StoreFloat(base, w.animation(k) + 12, float(r.Below(100)) / 4.0f);
  }
  // The root and its leaves, one chunk after another (+4 the size), then the
  // TRM chunk.
  constexpr uint32_t kEntries = (kTreeNodeBytes - 128) / 8;
  guest::StoreU32(base, w.root(), kTagSce);
  guest::StoreU32(base, w.root() + 4, kTreeNodeBytes);
  guest::StoreU32(base, w.root() + 100, 0);  // the pose resets walk children 0 to 3
  guest::StoreU32(base, w.root() + 104, r.Below(5));
  guest::StoreU32(base, w.root() + 108, r.Below(4));
  guest::StoreU32(base, w.root() + 116, kEntries);
  for (uint32_t i = 0; i < kEntries; i++) {
    guest::StoreU32(base, w.root() + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
  }
  for (uint32_t k = 0; k < kLeaves; k++) {
    const uint32_t leaf = w.leaf(k);
    // TXT (SetText's lookups need them); the cases make some PRM or PIC.
    guest::StoreU32(base, leaf, kTagTxt);
    guest::StoreU32(base, leaf + 4, kTreeNodeBytes);
    guest::StoreU32(base, leaf + 100, r.Below(16));
    guest::StoreU32(base, leaf + 104, r.Below(8) == 0 ? 0 : r.Below(7));
    guest::StoreU32(base, leaf + 108, r.Below(9));
    for (uint32_t i = 0; i < kEntries; i++) {
      guest::StoreU32(base, leaf + 128 + 8 * i, w.leaf(r.Below(kLeaves)));
    }
  }
  guest::StoreU32(base, w.movies.address + kTrmAt, kTagTrm);
  guest::StoreU32(base, kStringTable, w.strings.address);
  for (uint32_t m = 0; m < kMovieCount; m++) {
    guest::StoreU32(base, rl::kLoadingMovies + 4 * m, w.movie(m));
  }
}

// UiMovie::Run's update calls, kept and answered.
struct UpdateCall {
  uint32_t lr, r1;
  uint64_t r3, r4;
};
std::vector<UpdateCall>* g_updates = nullptr;
Random g_update_answers;

bool UpdateStandIn(uint32_t function, PPCContext& ctx, uint8_t*) {
  if (g_updates == nullptr || function != 0x8211D2F8) {
    return false;
  }
  g_updates->push_back({uint32_t(ctx.lr), ctx.r1.u32, ctx.r3.u64, ctx.r4.u64});
  ctx.r3.u64 = g_update_answers.Below(2) == 0 ? 0 : g_update_answers.Next();
  return true;
}

// SetFrames (no adapter: only Show calls it in play) through its plain C++:
// r3 = movie, r4, r5 (r3 left).
void NativeSetFrames(PPCContext& ctx, uint8_t* base) {
  rl::SetFrames(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// The loading screen's globals the cases set (the shown word, the current
// movie, the three movies; the three animation words; the string table and
// the missing text; the system request and sub_82293358's two words), put
// back after each side.
struct SavedGlobals {
  static constexpr uint32_t kAddresses[] = {
      rl::kLoadingShown, rl::kLoadingCurrent, rl::kLoadingMovies, rl::kLoadingMovies + 4,
      rl::kLoadingMovies + 8, rl::kLoadingAnimations, rl::kLoadingAnimations + 4,
      rl::kLoadingAnimations + 8, kStringTable, kMissingText, rl::kSystemRequest,
      0x826C7958, 0x826C7944};
  uint32_t words[std::size(kAddresses)];
  explicit SavedGlobals(uint8_t* base) : base_(base) {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      words[i] = guest::LoadU32(base, kAddresses[i]);
    }
  }
  void Restore() const {
    for (size_t i = 0; i < std::size(kAddresses); i++) {
      guest::StoreU32(base_, kAddresses[i], words[i]);
    }
  }

 private:
  uint8_t* base_;
};

enum class Kind { kShow, kRun, kFinish, kSetFrames };

void LoadingScreen(Context& h, const MovieWorld& w) {
  BuildStringTable(h.base, w);
  struct Case {
    const char* name;
    Kind kind;
    GuestFunction recompiled, native;
  };
  const Case kinds[] = {
      {"LoadingScreen::Show (sub_822478D0)", Kind::kShow, __imp__sub_822478D0,
       hooks::LoadingScreenShow},
      {"LoadingScreen::Run (sub_82247898)", Kind::kRun, __imp__sub_82247898,
       hooks::LoadingScreenRun},
      {"LoadingScreen::Finish (sub_82247A60)", Kind::kFinish, __imp__sub_82247A60,
       hooks::LoadingScreenFinish},
      {"UiMovie::SetFrames (sub_82290B30)", Kind::kSetFrames, __imp__sub_82290B30,
       NativeSetFrames},
  };
  constexpr uint32_t kShowFrame = 144, kShowSaves = 40;  // __savegprlr_28
  for (const Case& kind : kinds) {
    Results results(kind.name);
    for (uint64_t seed = 0; seed < 150; seed++) {
      Outputs out[2];
      std::string what;
      for (int v = 0; v < 2; v++) {
        const SavedGlobals saved(h.base);
        Random r(seed * 389 + 29 + uint64_t(kind.kind) * 7919);
        ShapeMovieWorld(h.base, w, r);
        // A few leaves PRM or PIC for SetFrames (any but those the root's
        // entries for its children name: the lookups need TXT ones).
        if (kind.kind == Kind::kSetFrames || r.Below(3) == 0) {
          for (uint32_t k = 0; k < kLeaves; k++) {
            if (r.Below(4) == 0) {
              bool named = false;
              for (uint32_t i = 0; i < 16; i++) {
                named |= guest::LoadU32(h.base, w.root() + 128 + 8 * i) == w.leaf(k);
              }
              if (!named) {
                guest::StoreU32(h.base, w.leaf(k), r.Below(2) ? kTagPrm : kTagPic);
              }
            }
          }
        }
        PPCContext t = CallContext(h);
        const uint32_t sp = t.r1.u32;
        WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
        t.r3.u64 = r.Next();
        t.r4.u64 = r.Next();
        t.r5.u64 = r.Next();
        t.r6.u64 = r.Next();
        t.r7.u64 = r.Next();
        t.r8.u64 = r.Next();
        t.r9.u64 = r.Next();
        t.r10.u64 = r.Next();
        t.lr = 0x82000000 | (r.Below(0x10000) << 2);
        // The animation words: named animations 0 to 255 (13 and 0 in the
        // image).
        for (uint32_t i = 0; i < 3; i++) {
          guest::StoreU32(h.base, rl::kLoadingAnimations + 4 * i,
                          r.Below(2) ? (i == 1 ? 13 : 0) : r.Below(256));
        }
        // The current movie: 0 to 2 (Run's and Finish's -1 read it).
        guest::StoreU32(h.base, rl::kLoadingCurrent, r.Below(3));
        guest::StoreU32(h.base, rl::kLoadingShown, uint32_t(r.Next()));
        const uint32_t which = r.Below(8) == 0 ? (r.Below(2) ? 3 + r.Below(5)
                                                             : uint32_t(r.Next()) | 0x80000000u)
                                               : r.Below(3);
        switch (kind.kind) {
          case Kind::kShow: {
            // r3 the tip (0, 1 or any word), r4 which (0 to 2, now and then
            // 3 or more, unsigned), r5 the frames (-1, small or any word).
            const uint32_t tips[3] = {0, 1, uint32_t(r.Next())};
            t.r3.u64 = (r.Next() & 0xFFFFFFFF00000000ull) | tips[r.Below(3)];
            t.r4.u64 = r.Below(8) == 0 ? 3 + r.Below(1000) : r.Below(3);
            const uint32_t frames[3] = {0xFFFFFFFFu, r.Below(1000), uint32_t(r.Next())};
            t.r5.u64 = (r.Next() & 0xFFFFFFFF00000000ull) | frames[r.Below(3)];
            break;
          }
          case Kind::kRun:
          case Kind::kFinish:
            // r3 which: 0 to 2, -1, 3 or more (signed).
            t.r3.u64 = r.Below(4) == 0 ? 0xFFFFFFFFFFFFFFFFull
                                       : (which & 0x80000000u ? 3 + r.Below(100) : which);
            break;
          case Kind::kSetFrames:
            t.r3.u64 = w.movie(r.Below(kMovieCount));
            break;
        }
        what = fmt::format("seed {}: r3 {:X} r4 {:X} r5 {:X}", seed, t.r3.u64, t.r4.u64,
                           t.r5.u64);
        // A made-up time: its milliseconds pick the tip.
        time_hooks::SetFakeNow(0x01D0000000000000ull + r.Next() % 0x0010000000000000ull);
        g_update_answers = Random(r.Next());
        std::vector<UpdateCall> updates;
        g_updates = &updates;
        const ui_movies_hooks::UpdateCalleeStandIn saved_stand_in =
            ui_movies_hooks::g_update_callee_stand_in;
        ui_movies_hooks::g_update_callee_stand_in = UpdateStandIn;
        Run(v, kind.recompiled, kind.native, t, h.base);
        ui_movies_hooks::g_update_callee_stand_in = saved_stand_in;
        g_updates = nullptr;
        time_hooks::ClearFakeNow();
        out[v].Bytes(Read(h.base, w.movies.address, w.movies.bytes));
        out[v].Bytes(Read(h.base, w.texts.address, w.texts.bytes));
        for (const uint32_t address : {rl::kLoadingShown, rl::kLoadingCurrent}) {
          out[v].Word(guest::LoadU32(h.base, address));
        }
        if (kind.kind == Kind::kShow) {
          // Show's frame but its register saves and SetText's varargs' words
          // (+48 to +79).
          const uint32_t frame = sp - kShowFrame;
          out[v].Bytes(Read(h.base, frame, 48));
          out[v].Bytes(Read(h.base, frame + 80, kShowFrame - 80 - kShowSaves));
        }
        out[v].Word(uint32_t(updates.size()));
        for (const UpdateCall& u : updates) {
          out[v].Word(u.lr);
          out[v].Word(u.r1);
          for (const uint64_t x : {u.r3, u.r4}) {
            out[v].Word(uint32_t(x >> 32));
            out[v].Word(uint32_t(x));
          }
        }
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        saved.Restore();
      }
      results.Check(out[0], out[1], what);
    }
    results.Report();
  }
}

// sub_82293460: the system request 0, or set with sub_82293358's first test
// failing ([0x826C7958] below [0x826C7944]: its debug print, then 0), so its
// unseen callees aren't reached; the frame's back chain and r3 compared.
void SystemRequests(Context& h) {
  Results results("sub_82293460 (CheckSystemRequest)");
  for (uint64_t seed = 0; seed < 40; seed++) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      const SavedGlobals saved(h.base);
      Random r(seed * 61 + 5);
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      WriteRandom(h.base, sp - kStackBytes, kStackBytes, r);
      t.r3.u64 = r.Next();
      guest::StoreU32(h.base, rl::kSystemRequest, seed % 2 ? uint32_t(r.Next()) | 1 : 0);
      const uint32_t limit = 1 + r.Below(1000);
      guest::StoreU32(h.base, 0x826C7944, limit);
      guest::StoreU32(h.base, 0x826C7958, r.Below(limit));
      Run(v, __imp__sub_82293460, hooks::CheckSystemRequest, t, h.base);
      out[v].Word(guest::LoadU32(h.base, sp - 96));
      out[v].Word(guest::LoadU32(h.base, rl::kSystemRequest));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      saved.Restore();
    }
    results.Check(out[0], out[1], fmt::format("seed {}", seed));
  }
  results.Report();
}

}  // namespace

void RaceLoading(Context& h) {
  const Block machine(kMachineBytes);
  const Block dispatcher(kDispatcherBytes);
  if (!machine.address || !dispatcher.address) {
    ITC_LOG_ERROR("harness: race_loading: no block from the allocation layer");
    return;
  }
  Inits(h, machine);
  Handlers(h, machine, dispatcher);
  const MovieWorld world;
  if (!world.ok()) {
    ITC_LOG_ERROR("harness: race_loading: no movie world from the allocation layer");
    return;
  }
  LoadingScreen(h, world);
  SystemRequests(h);
}

}  // namespace harness
