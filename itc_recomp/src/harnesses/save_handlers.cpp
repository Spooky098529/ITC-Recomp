// The save devices' slot handlers (src/save_interface/slot_handlers.h,
// docs/functions/save-interface.md) against the recompiled functions. Each
// case builds made-up save devices and a 1,000-byte slot in the scratch area
// over random words (the slot's state words, requests, sub-steps and
// overlappeds chosen to cover the branches), random words on the stack
// below the stack pointer and in r27 to r31, and runs the original (with
// --native_save_interface off, so its callees are the originals too; the B4
// steps are the same adapters in both) or the register step
// (hooks/save_interface.h). Compared, all exactly (nothing here is a float):
// the scratch area, the 1,024 bytes below the stack pointer (the prologues'
// saves and back chains of the handler and what it calls), r3 (64 bits),
// r1 and r27 to r31 after.
//
// No case reaches the save file: the steps that would write, read or delete
// it (WriteFile, ReadFile, Delete) only get slots whose create isn't done or
// that have no device.

#include "harness.h"

#include "base/flags.h"
#include "hooks/save_interface.h"
#include "save_interface/slot_handlers.h"

#include <fmt/format.h>

#include <functional>
#include <string>

ITC_FLAG_DECLARE(bool, native_save_interface);

namespace harness {

namespace {

namespace si = save_interface;

constexpr uint32_t kStackBytes = 1024;

// Where things go in the scratch area.
struct Layout {
  uint32_t devices;  // 128 bytes
  uint32_t slot;     // 1,000 bytes
  uint32_t records;  // the enumeration buffer, 2 x 308 bytes
  uint32_t flags;    // the enumerated saves' flags, 16 words
  uint32_t loop;     // a run loop and its vtable
  uint32_t loop_vtable;
  uint32_t event;
  uint32_t object;  // the framework's +56 interface
};

Layout LayoutOf(const Context& h) {
  return Layout{h.data + 0x000, h.data + 0x100, h.data + 0x600, h.data + 0x880,
                h.data + 0x900, h.data + 0x940, h.data + 0x980, h.data + 0x9C0};
}

void WriteRandom(uint8_t* base, uint32_t address, uint32_t words, Random& random) {
  for (uint32_t i = 0; i < words; i++) {
    guest::StoreU32(base, address + 4 * i, uint32_t(random.Next()));
  }
}

// One of `values`.
uint32_t Pick(Random& r, std::initializer_list<uint32_t> values) {
  return *(values.begin() + r.Below(uint32_t(values.size())));
}

// An overlapped's status: done, pending (997), incomplete (996), no more
// files (1627), an error, or a random word.
uint32_t Status(Random& r) {
  return Pick(r, {0, 0, 997, 996, 1627, 2, uint32_t(r.Next())});
}

// The slot's words the handlers read, over the random ones.
void ShapeSlot(uint8_t* base, const Layout& l, Random& r) {
  const uint32_t s = l.slot;
  guest::StoreU32(base, s + 36, Pick(r, {0, 1, 1, uint32_t(r.Next())}));
  guest::StoreU32(base, s + 956, Pick(r, {0xFFFFFFFF, 0, 1, 2, 3, 4, 7, uint32_t(r.Next())}));
  for (uint32_t at = 960; at <= 980; at += 4) {
    guest::StoreU32(base, s + at, r.Below(3) == 0 ? Pick(r, {1, uint32_t(r.Next())}) : 0);
  }
  guest::StoreU32(base, s + 96, Pick(r, {0, 1, 2, 2, 3, uint32_t(r.Next())}));
  guest::StoreU32(base, s + 8, Status(r));
  guest::StoreU32(base, s + 48, Status(r));
  guest::StoreU32(base, s + 52, r.Below(3));  // the count found
  guest::StoreU32(base, s + 80, l.records);
  guest::StoreU32(base, s + 84, l.flags);
  guest::StoreU32(base, l.devices + 68, Pick(r, {0, 1, 100}));
}

// One handler's cases: `shape` writes the inputs over the random words,
// `args` sets r3 to r5.
struct Handler {
  const char* name;
  int cases;
  GuestFunction original;
  GuestFunction native;
  std::function<void(uint8_t*, const Layout&, Random&)> shape;
  std::function<void(PPCContext&, const Layout&)> args;
};

void Run(Context& h, const Handler& handler) {
  const Layout l = LayoutOf(h);
  Results results(handler.name);
  for (int c = 0; c < handler.cases; c++) {
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random random = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, random);
      PPCContext t = CallContext(h);
      const uint32_t stack = t.r1.u32;
      WriteRandom(h.base, stack - kStackBytes, kStackBytes / 4, random);
      handler.shape(h.base, l, random);
      for (int r = 27; r < 32; r++) {
        (&t.r14)[r - 14].u64 = random.Next();
      }
      t.r3.u64 = l.devices;
      t.r4.u64 = l.slot;
      handler.args(t, l);
      t.lr = 0x820A47D8;  // SaveDevices::Update's call
      after = random;
      ITC_FLAG(native_save_interface) = v == 1;
      (v == 0 ? handler.original : handler.native)(t, h.base);
      ITC_FLAG(native_save_interface) = true;
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, stack - kStackBytes, kStackBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32 - stack);
      for (int r = 27; r < 32; r++) {
        out[v].Word(uint32_t((&t.r14)[r - 14].u64));
        out[v].Word(uint32_t((&t.r14)[r - 14].u64 >> 32));
      }
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

void NoArgs(PPCContext&, const Layout&) {}

void Shape(uint8_t* base, const Layout& l, Random& r) { ShapeSlot(base, l, r); }

// Recompiled functions as the run loop's slots 3 and 4: a getter (lwz
// r3,24(r3)) and SaveSlot::WaitForDevice, which saves r31 (the run loop) in
// its prologue and reads the key as a slot.
constexpr uint32_t kReturnsWord24 = 0x82099BA0;

}  // namespace

void SaveHandlers(Context& h) {
  Run(h, {"SaveSlot::WaitForUser (sub_820A4870)", 24, __imp__sub_820A4870,
          hooks::SlotWaitForUserSteps, Shape, NoArgs});
  Run(h, {"SaveSlot::WaitForDevice (sub_820A5878)", 24, __imp__sub_820A5878,
          hooks::SlotWaitForDeviceSteps, Shape, NoArgs});
  Run(h, {"SaveSlot::Failed (sub_820A4890)", 24, __imp__sub_820A4890, hooks::SlotFailedSteps,
          Shape, NoArgs});
  Run(h, {"SaveSlot::Ready (sub_820A58D8)", 48, __imp__sub_820A58D8, hooks::SlotReadySteps,
          Shape, NoArgs});
  Run(h, {"SaveSlot::EnumerateStep (sub_820A59C0)", 48, __imp__sub_820A59C0,
          hooks::SlotEnumerateStepSteps, Shape, NoArgs});
  Run(h, {"SaveDevices::OnEvent (sub_820A4800)", 24, __imp__sub_820A4800,
          hooks::DevicesOnEventSteps,
          [](uint8_t* base, const Layout& l, Random& r) {
            ShapeSlot(base, l, r);
            guest::StoreU32(base, l.loop, l.loop_vtable);
            guest::StoreU32(base, l.loop_vtable + 12, kReturnsWord24);
            guest::StoreU32(base, l.loop_vtable + 16, si::kSlotWaitForDevice);
            // The key (devices + 4) read as a slot: its +960 and +36.
            guest::StoreU32(base, l.devices + 4 + 960, r.Below(2));
            guest::StoreU32(base, l.devices + 4 + 36, r.Below(2));
            guest::StoreU32(base, l.event, Pick(r, {1, 1, 0, 2, uint32_t(r.Next())}));
          },
          [](PPCContext& t, const Layout& l) {
            t.r4.u64 = l.loop;
            t.r5.u64 = l.event;
          }});
  Run(h, {"Framework::DeviceList (sub_820908D8)", 8, __imp__sub_820908D8,
          hooks::FrameworkDeviceListSteps, Shape,
          [](PPCContext& t, const Layout& l) { t.r3.u64 = l.object; }});
}

}  // namespace harness
