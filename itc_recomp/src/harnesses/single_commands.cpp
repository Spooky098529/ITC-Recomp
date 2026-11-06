// The render command replay's single commands (src/graphics/single_commands.h,
// docs/functions/single-commands.md) against the recompiled handlers. Run
// under the game's device lock, with the globals the handlers read (the
// device [0x8296BD90], the drawer [0x82970BB8] and its state [0x82970BBC])
// pointed at made-up ones and put back after: the drawer, its state and the
// slots in the scratch area over random words, the device a block of its
// own. The drawer's vtable slots are a recorder (sub_825780D0, a leaf that
// stores r4, r5, r6 and r7 at its r3's +0, +4, +8 and +16 and 0 at +12, so
// the arguments passed are compared) or RenderQueue::Flip (sub_8258E5E0,
// which toggles the word its r3's +4 points to). Compared: the scratch area,
// the device block and r3.

#include "harness.h"

#include "base/log.h"
#include "graphics/single_commands.h"
#include "hooks/single_commands.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kDeviceGlobal = 0x8296BD90;
constexpr uint32_t kDrawerGlobal = 0x82970BB8;
constexpr uint32_t kStateGlobal = 0x82970BBC;

constexpr uint32_t kRecorder = 0x825780D0;
constexpr uint32_t kFlip = 0x8258E5E0;

// The scratch area's layout.
constexpr uint32_t kDrawer = 0;          // +0 its vtable, +4 Flip's word pointer
constexpr uint32_t kDrawerVtable = 64;   // 8 slots
constexpr uint32_t kToggle = 96;         // Flip's state: its +0 toggled
constexpr uint32_t kState = 128;         // the drawer's state, 32 bytes
constexpr uint32_t kSlots = 512;         // the slot and its data

constexpr uint32_t kDeviceBytes = 13312;

// The game's video output, as the device's users reach it ([0x8266D088]'s
// slot 7, then that object's slot 3).
uint32_t GameVideoOutput(Context& h) {
  PPCContext t = CallContext(h);
  t.r3.u64 = guest::LoadU32(h.base, 0x8266D088);
  uint8_t* base = h.base;
  {
    PPCContext& ctx = t;
    ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 28);
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    ctx.ctr.u64 = guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 12);
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  }
  return t.r3.u32;
}

uint32_t EdgeOrRandom(Random& r) {
  const uint32_t edges[] = {0, 1, 2, 3, 4, 5, 6, 0xFF, 0x100, 0xFFFF, 0x0AAAAAAA, 0x0AAAAAAB,
                            0x7FFFFFFF, 0x80000000u, 0xFFFFFFFFu};
  return r.Below(3) == 0 ? edges[r.Below(sizeof(edges) / 4)] : uint32_t(r.Next());
}

struct Case {
  std::vector<uint32_t> scratch;
  std::vector<uint32_t> device;
};

// Random words, the drawer with its vtable (the recorder or Flip in each
// slot) and Flip's word.
Case MakeCase(Context& h, Random& r) {
  Case c;
  c.scratch.resize(kScratchBytes / 4);
  for (uint32_t& w : c.scratch) w = uint32_t(r.Next());
  c.device.resize(kDeviceBytes / 4);
  for (uint32_t& w : c.device) w = uint32_t(r.Next());
  auto at = [&](uint32_t offset) -> uint32_t& { return c.scratch[offset / 4]; };
  at(kDrawer + 0) = h.data + kDrawerVtable;
  at(kDrawer + 4) = h.data + kToggle;
  for (uint32_t slot = 0; slot < 8; slot++) {
    at(kDrawerVtable + 4 * slot) = r.Below(2) == 0 ? kRecorder : kFlip;
  }
  at(kToggle) = r.Below(2);
  return c;
}

class Runner {
 public:
  Runner(Context& h, uint32_t device) : h_(h), device_(device) {}

  // Runs `c` through both versions (with `slot` in r3) and compares.
  void Run(Results& results, const Case& c, uint32_t slot, GuestFunction recompiled,
           GuestFunction native, const std::string& what) {
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32s(h_.base, h_.data, c.scratch.data(), uint32_t(c.scratch.size()));
      guest::StoreU32s(h_.base, device_, c.device.data(), uint32_t(c.device.size()));
      guest::StoreU32(h_.base, kDeviceGlobal, device_);
      guest::StoreU32(h_.base, kDrawerGlobal, h_.data + kDrawer);
      guest::StoreU32(h_.base, kStateGlobal, h_.data + kState);
      PPCContext t = CallContext(h_);
      t.r3.u64 = slot;
      (v == 0 ? recompiled : native)(t, h_.base);
      out[v].Bytes(Read(h_.base, h_.data, kScratchBytes));
      out[v].Bytes(Read(h_.base, device_, kDeviceBytes));
      out[v].Bytes(Read(h_.base, kDeviceGlobal, 4));
      out[v].Bytes(Read(h_.base, kDrawerGlobal, 8));
      out[v].Word(t.r3.u32);
    }
    results.Check(out[0], out[1], what);
  }

 private:
  Context& h_;
  uint32_t device_;
};

void InlineVertices(Context& h, Runner& run) {
  Random& r = h.random;
  Results results("Single::InlineVertices (sub_8258E8E8)");
  for (int i = 0; i < 64; i++) {
    Case c = MakeCase(h, r);
    const uint32_t slot = kSlots + 16 * r.Below(8);
    // The minimap's triangle: 3 vertices; then edges and random counts.
    const uint32_t count = i == 0 ? 3 : EdgeOrRandom(r);
    c.scratch[(slot + 4) / 4] = count;
    run.Run(results, c, h.data + slot, __imp__sub_8258E8E8, hooks::NativeSingleInlineVertices,
            fmt::format("case {}: count {:08X}", i, count));
  }
  results.Report();
}

void SetUpDrawer(Context& h, Runner& run) {
  Random& r = h.random;
  Results results("Single::SetUpDrawer (sub_8258E958)");
  for (int i = 0; i < 64; i++) {
    Case c = MakeCase(h, r);
    const uint32_t slot = kSlots + 16 * r.Below(8);
    if (i == 0) {  // the minimap's triangle: [13, 4, 0, 0]
      c.scratch[slot / 4] = 13;
      c.scratch[(slot + 4) / 4] = 4;
      c.scratch[(slot + 8) / 4] = 0;
      c.scratch[(slot + 12) / 4] = 0;
    } else {
      for (uint32_t k = 1; k < 4; k++) c.scratch[(slot + 4 * k) / 4] = EdgeOrRandom(r);
    }
    run.Run(results, c, h.data + slot, __imp__sub_8258E958, hooks::NativeSingleSetUpDrawer,
            fmt::format("case {}: slot +{}, set-shaders {:08X}", i, slot,
                        c.scratch[(kDrawerVtable + 8) / 4]));
  }
  results.Report();
}

}  // namespace

void SingleCommands(Context& h) {
  const uint32_t device = memory::Alloc(kDeviceBytes, 0);
  if (device == 0) {
    ITC_LOG_INFO("harness: single_commands: no block for the device");
    return;
  }
  const uint32_t video_output = GameVideoOutput(h);
  PPCContext lock = CallContext(h);
  lock.r3.u64 = video_output;
  sub_8258D8C8(lock, h.base);  // VideoOutput::LockDevice
  const uint32_t locked_device = lock.r3.u32;
  const std::vector<uint8_t> saved_device = Read(h.base, kDeviceGlobal, 4);
  const std::vector<uint8_t> saved_drawer = Read(h.base, kDrawerGlobal, 8);

  Runner run(h, device);
  InlineVertices(h, run);
  SetUpDrawer(h, run);

  Write(h.base, kDeviceGlobal, saved_device.data(), 4);
  Write(h.base, kDrawerGlobal, saved_drawer.data(), 8);
  guest::StoreU32(h.base, h.data, locked_device);
  lock.r3.u64 = video_output;
  lock.r4.u64 = h.data;
  sub_8258D908(lock, h.base);  // VideoOutput::UnlockDevice
  memory::Free(device);
}

}  // namespace harness
