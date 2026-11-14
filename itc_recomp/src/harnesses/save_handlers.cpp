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
#include "hooks/last_error.h"
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
  uint32_t overlapped;  // 28 bytes
  uint32_t result;
};

Layout LayoutOf(const Context& h) {
  return Layout{h.data + 0x000, h.data + 0x100, h.data + 0x600, h.data + 0x880,
                h.data + 0x900, h.data + 0x940, h.data + 0x980, h.data + 0x9C0,
                h.data + 0xA00, h.data + 0xA40};
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
  // When not 0, SaveSlot::WaitClose's stack pointer is this far below the
  // handler's (see MaskCloseHandleSaves).
  uint32_t wait_close_below = 0;
};

// The recompiled CloseHandle (sub_82294E20, stwu r1,-96 with its lr saved
// at -8) that SaveSlot::WaitClose's original calls leaves its back chain
// (WaitClose's sp - 96) and its saved lr (WaitClose's sp - 8: 0x820A50BC or
// 0x820A50EC) below the stack pointer; under --native_c_runtime_core the
// close is CloseHandle's plain C++ (hooks/save_interface.cpp's
// KernelCallsOf, since f664a88d), which uses no guest stack and leaves the
// earlier words there. Both are dead words below the stack pointer, left
// out of the compare (set to 0 after the call); everything else stays
// compared. `stack` is the harness's stack pointer, `below` how far
// WaitClose's is under it.
void MaskCloseHandleSaves(uint8_t* base, uint32_t stack, uint32_t below) {
  const uint32_t wait_close_sp = stack - below;
  guest::StoreU32(base, wait_close_sp - 96, 0);
  guest::StoreU32(base, wait_close_sp - 8, 0);
}

// WaitClose's frame (stwu r1,-96), and SaveStep's and LoadStep's (both
// stwu r1,-112) that call it.
constexpr uint32_t kWaitCloseFrame = 96;
constexpr uint32_t kStepFrame = 112;

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
      last_error::Set(h.base, t.r13.u32, uint32_t(random.Next()));
      after = random;
      ITC_FLAG(native_save_interface) = v == 1;
      (v == 0 ? handler.original : handler.native)(t, h.base);
      ITC_FLAG(native_save_interface) = true;
      if (handler.wait_close_below != 0) {
        MaskCloseHandleSaves(h.base, stack, handler.wait_close_below);
      }
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, stack - kStackBytes, kStackBytes));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32 - stack);
      out[v].Word(last_error::Get(h.base, t.r13.u32));
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

// An overlapped: its status (+0) one of the waits' cases, its information
// (+4), its event (+12) and +16 0 (no wait is made: the waits aren't taken
// in play, and a real one would block).
void ShapeOverlapped(uint8_t* base, const Layout& l, Random& r) {
  guest::StoreU32(base, l.overlapped, Pick(r, {0, 997, 259, 996, 2, 0xC0000034, uint32_t(r.Next())}));
  guest::StoreU32(base, l.overlapped + 12, 0);
  guest::StoreU32(base, l.overlapped + 16, 0);
}

// The machines' and waits' words: the claim (+72: free, this slot's or
// another's), the sub-steps, the error words, the thumbnail, the
// overlappeds, and the handles (none mostly, as B4's steps leave them). No
// save file is touched: a write or read step (+948 2) gets a create (+136)
// not done, and a delete (+948 9) no device.
void ShapeMachine(uint8_t* base, const Layout& l, Random& r) {
  ShapeSlot(base, l, r);
  const uint32_t s = l.slot;
  guest::StoreU32(base, l.devices + 72, Pick(r, {0, 0, s, s, uint32_t(r.Next())}));
  guest::StoreU32(base, s + 128, r.Below(6));
  guest::StoreU32(base, s + 132, r.Below(4));
  guest::StoreU32(base, s + 984, r.Below(4));
  const uint32_t step = Pick(r, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, uint32_t(r.Next())});
  guest::StoreU32(base, s + 948, step);
  guest::StoreU32(base, s + 952, Pick(r, {0, 0, 1, 2, uint32_t(r.Next())}));
  guest::StoreU32(base, s + 940, Pick(r, {0, 0, uint32_t(r.Next())}));
  guest::StoreU32(base, s + 100, Status(r));
  guest::StoreU32(base, s + 136, Pick(r, {0, 0, 997, 996, 2, 0xC0000034}));
  guest::StoreU32(base, s + 472, Pick(r, {0, 0, 259, 997, 0xC0000034, uint32_t(r.Next())}));
  const bool handles = r.Below(8) == 0;
  guest::StoreU32(base, s + 496, handles ? uint32_t(r.Next()) : 0xFFFFFFFF);
  guest::StoreU32(base, s + 492, handles ? uint32_t(r.Next()) : 0);
  guest::StoreU32(base, s + 504, 0x00410000);  // a display name "A"
  guest::StoreU32(base, s + 760, 0x41000000);  // a file name "A"
  if (step == 2) {
    guest::StoreU32(base, s + 136, Pick(r, {997, 996, 2}));
  }
  if (step == 9) {
    guest::StoreU32(base, s + 36, 0);
  }
}

void NoArgs(PPCContext&, const Layout&) {}

void Shape(uint8_t* base, const Layout& l, Random& r) { ShapeSlot(base, l, r); }

// Recompiled functions as the run loop's slots 3 and 4: a getter (lwz
// r3,24(r3)) and SaveSlot::WaitForDevice, which saves r31 (the run loop) in
// its prologue and reads the key as a slot.
constexpr uint32_t kReturnsWord24 = 0x82099BA0;

// SaveDevices::Update over made-up save devices with the game's targets:
// the device list (the framework's +56 interface's slot 1, sub_820908D8,
// reading the users at +72), the users (Users::SignedIn, sub_820996D8),
// SaveSlot::State (+28) and the ten handlers at +76, now and then one
// replaced by a recompiled function that isn't ours (a guest call), and
// three slots shaped as the machines' cases, each in a state 0 to 9.
void UpdateCases(Context& h) {
  const uint32_t devices = h.data + 0x000;
  const uint32_t users = h.data + 0x080;  // to +220, and +8 + 40 x 3
  const uint32_t object = h.data + 0x160;
  const uint32_t records = h.data + 0x1C0;
  const uint32_t flags = h.data + 0x300;
  const uint32_t slots = h.data + 0x340;  // three, to 0xF18
  const uint32_t devices_vtable = h.data + 0xF20;
  const uint32_t object_vtable = h.data + 0xF60;
  const uint32_t users_vtable = h.data + 0xF80;
  constexpr uint32_t kHandlers[10] = {0x820A4870, 0x820A5878, 0x820A4890, 0x820A58D8,
                                      0x820A48B8, 0x820A59C0, 0x820A5A88, 0x820A5BE8,
                                      0x820A5E68, 0x820A6010};
  constexpr uint32_t kReturns1 = 0x824457CC;
  Results results("SaveDevices::Update (sub_820A4610) with its handlers");
  for (int c = 0; c < 96; c++) {
    Outputs out[2];
    const Random start = h.random;
    Random after;
    for (int v = 0; v < 2; v++) {
      Random r = start;
      WriteRandom(h.base, h.data, kScratchBytes / 4, r);
      PPCContext t = CallContext(h);
      const uint32_t stack = t.r1.u32;
      WriteRandom(h.base, stack - kStackBytes, kStackBytes / 4, r);
      const uint32_t count = c < 4 ? uint32_t(c) : 1 + r.Below(3);
      guest::StoreU32(h.base, devices, devices_vtable);
      guest::StoreU32(h.base, devices + 12, object);
      guest::StoreU32(h.base, devices + 44, slots);
      guest::StoreU32(h.base, devices + 48, count);
      guest::StoreU32(h.base, devices + 56, c == 4 ? 0 : 0x4E4F4C53);
      guest::StoreU32(h.base, devices + 68, Pick(r, {0, 1, 100}));
      guest::StoreU32(h.base, devices + 72, Pick(r, {0, 0, slots, uint32_t(r.Next())}));
      for (int k = 0; k < 10; k++) {
        guest::StoreU32(h.base, devices + 76 + 4 * k, r.Below(12) == 0 ? kReturns1 : kHandlers[k]);
      }
      guest::StoreU32(h.base, devices_vtable + 28, 0x820A57F0);
      guest::StoreU32(h.base, object, object_vtable);
      // The device list now and then a recompiled getter (lwz r3,24(r3)).
      guest::StoreU32(h.base, object_vtable + 4, r.Below(8) == 0 ? kReturnsWord24 : 0x820908D8);
      guest::StoreU32(h.base, object + 24, users);
      guest::StoreU32(h.base, object + 72, users);  // sub_82493700: (object - 20) + 92
      guest::StoreU32(h.base, users, users_vtable);
      guest::StoreU32(h.base, users_vtable + 8, 0x820996D8);
      guest::StoreU32(h.base, users + 216, r.Below(5));
      for (uint32_t i = 0; i < 4; i++) {
        guest::StoreU32(h.base, users + 200 + 4 * i, r.Below(4));
        guest::StoreU32(h.base, users + 8 + 40 * i, r.Below(3) == 0 ? 0 : 1);
      }
      for (uint32_t i = 0; i < 3; i++) {
        Layout l = LayoutOf(h);
        l.devices = devices;
        l.slot = slots + 1000 * i;
        l.records = records;
        l.flags = flags;
        ShapeMachine(h.base, l, r);
        guest::StoreU32(h.base, l.slot + 984, r.Below(3));
        guest::StoreU32(h.base, l.slot + 988, r.Below(10));
        // A slot that may reach the delete step gets no device either
        // (Update doesn't run a handler twice, but the ready state sets
        // +948 for the next frame only).
        if (guest::LoadU32(h.base, l.slot + 948) == 9 ||
            guest::LoadU32(h.base, l.slot + 948) == 2) {
          guest::StoreU32(h.base, l.slot + 36, 0);
        }
      }
      // The users and the claim word ShapeMachine wrote over: put back.
      guest::StoreU32(h.base, devices + 44, slots);
      for (int r27 = 27; r27 < 32; r27++) {
        (&t.r14)[r27 - 14].u64 = r.Next();
      }
      t.r3.u64 = devices;
      t.r4.u64 = r.Next();
      t.lr = 0x820ACE38;
      last_error::Set(h.base, t.r13.u32, uint32_t(r.Next()));
      after = r;
      ITC_FLAG(native_save_interface) = v == 1;
      (v == 0 ? __imp__sub_820A4610 : hooks::SaveDevicesUpdateSteps)(t, h.base);
      ITC_FLAG(native_save_interface) = true;
      // SaveStep's and LoadStep's WaitClose under Update's frame (stwu
      // r1,-144).
      MaskCloseHandleSaves(h.base, stack, 144 + kStepFrame + kWaitCloseFrame);
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      // The stack but XNotifyGetNext's two outputs in Update's frame (r1 +
      // 80 and + 84, the recompiled side's call into the SDK).
      out[v].Bytes(Read(h.base, stack - kStackBytes, kStackBytes - 0x40));
      out[v].Bytes(Read(h.base, stack - 0x38, 0x38));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32 - stack);
      out[v].Word(last_error::Get(h.base, t.r13.u32));
      for (int k = 27; k < 32; k++) {
        out[v].Word(uint32_t((&t.r14)[k - 14].u64));
        out[v].Word(uint32_t((&t.r14)[k - 14].u64 >> 32));
      }
    }
    h.random = after;
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

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
  // The overlapped results: no wait, or a wait asked for without an event
  // (XGetOverlappedResult only: GetOverlappedResult would wait on the
  // handle). Their wait paths are never taken in play.
  Run(h, {"XGetOverlappedResult (sub_82295C98)", 48, __imp__sub_82295C98,
          hooks::XGetOverlappedResultSteps,
          [](uint8_t* base, const Layout& l, Random& r) { ShapeOverlapped(base, l, r); },
          [](PPCContext& t, const Layout& l) {
            const uint32_t pick = uint32_t(t.r27.u64) % 4;
            t.r3.u64 = l.overlapped;
            t.r4.u64 = pick == 0 ? 0 : l.result;
            t.r5.u64 = pick == 1 ? 1 : 0;
          }});
  // The waits (r4 = slot) and the machines (r3 = the save devices).
  struct Entry {
    const char* name;
    GuestFunction original, native;
    uint32_t wait_close_below = 0;
  };
  for (const Entry& e : {
           Entry{"SaveSlot::WaitCloseItem (sub_820A4C08)", __imp__sub_820A4C08,
                 hooks::SlotWaitCloseItemSteps},
           Entry{"SaveSlot::WaitDelete (sub_820A5290)", __imp__sub_820A5290,
                 hooks::SlotWaitDeleteSteps},
           Entry{"SaveSlot::WaitThumbnail (sub_820A5190)", __imp__sub_820A5190,
                 hooks::SlotWaitThumbnailSteps},
           Entry{"SaveSlot::WaitClose (sub_820A5070)", __imp__sub_820A5070,
                 hooks::SlotWaitCloseSteps, kWaitCloseFrame},
           Entry{"SaveSlot::WaitWrite (sub_820A4E68)", __imp__sub_820A4E68,
                 hooks::SlotWaitWriteSteps},
           Entry{"SaveSlot::WaitRead (sub_820A4FF8)", __imp__sub_820A4FF8,
                 hooks::SlotWaitReadSteps},
       }) {
    Run(h, {e.name, 32, e.original, e.native, ShapeMachine, NoArgs, e.wait_close_below});
  }
  for (const Entry& e : {
           Entry{"SaveSlot::CheckStep (sub_820A5A88)", __imp__sub_820A5A88,
                 hooks::SlotCheckStepSteps},
           Entry{"SaveSlot::SaveStep (sub_820A5BE8)", __imp__sub_820A5BE8,
                 hooks::SlotSaveStepSteps, kStepFrame + kWaitCloseFrame},
           Entry{"SaveSlot::LoadStep (sub_820A5E68)", __imp__sub_820A5E68,
                 hooks::SlotLoadStepSteps, kStepFrame + kWaitCloseFrame},
           Entry{"SaveSlot::DeleteStep (sub_820A6010)", __imp__sub_820A6010,
                 hooks::SlotDeleteStepSteps},
       }) {
    Run(h, {e.name, 96, e.original, e.native, ShapeMachine, NoArgs, e.wait_close_below});
  }
  UpdateCases(h);
  Run(h, {"GetOverlappedResult (sub_82295F20)", 48, __imp__sub_82295F20,
          hooks::GetOverlappedResultSteps,
          [](uint8_t* base, const Layout& l, Random& r) { ShapeOverlapped(base, l, r); },
          [](PPCContext& t, const Layout& l) {
            t.r3.u64 = uint32_t(t.r28.u64);
            t.r4.u64 = l.overlapped;
            t.r5.u64 = l.result;
            t.r6.u64 = 0;
          }});
}

}  // namespace harness
