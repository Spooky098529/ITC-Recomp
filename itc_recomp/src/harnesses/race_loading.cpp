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

#include "harness.h"

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

#include <optional>

ITC_FLAG_DECLARE(bool, native_race_loading);

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
}

}  // namespace harness
