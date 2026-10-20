// The graphics library's device set-up, natively and without its kernel
// video calls (Vd*): what our renderer needs from each function is kept, the
// rest left out. The steps that stay are the library's own (recompiled)
// functions, called as guest code. See B3 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include <bit>

#include "display/display.h"
#include "helpers/clock.h"

using guest_call::InFrame;

namespace {

// Device fields (see "The functions" under B3).
constexpr uint32_t kDeviceFlags = 10809;
constexpr uint32_t kDeviceFlags2 = 10810;
constexpr uint32_t kDeviceParameters = 13188;  // a copy of the 124 bytes
constexpr uint32_t kDeviceFrontBufferFormat = 13252;  // the copy's +64
constexpr uint32_t kDeviceDepthSurface = 13424;
constexpr uint32_t kDeviceFrontBuffer = 13428;
constexpr uint32_t kDeviceRenderTarget = 13432;
constexpr uint32_t kDeviceDisplayWidth = 19988;
constexpr uint32_t kDeviceDisplayHeight = 19992;
constexpr uint32_t kDeviceDisplayPitch = 19996;
constexpr uint32_t kDeviceDisplayFormat = 20000;
constexpr uint32_t kDeviceTimerRate = 20040;          // float
constexpr uint32_t kDeviceTimerPeriod = 20044;        // float

// The kernel's VdGlobalDevice, as the image's import table points to it:
// the device is stored there (by the game for the kernel; other game code
// may read it).
constexpr uint32_t kVdGlobalDeviceImport = 0x820007B0;
// Set when the kernel's EDRAM training failed (VdIsHSIOTrainingSucceeded):
// then Device::Init prints a warning. It never fails now.
constexpr uint32_t kTrainingFailed = 0x829537BC;
constexpr uint32_t kTrainingFailedMessage = 0x820141C8;
constexpr uint32_t kCheckFailedMessage = 0x82014180;

// The front buffer's format (parameters +64) as the device creates it: 0
// and 0x28280186 become 0x28280106, 0x282801B6 becomes 0x28280136.
uint32_t FrontBufferFormat(uint32_t requested) {
  if (requested == 0 || requested == 0x28280186) {
    return 0x28280106;
  }
  if (requested == 0x282801B6) {
    return 0x28280136;
  }
  return requested;
}

// The format the device keeps: 0x18280136 and 0x18280106 as 0x2828xxxx.
uint32_t StoredFrontBufferFormat(uint32_t format) {
  if (format == 0x18280136) {
    return 0x28280136;
  }
  if (format == 0x18280106) {
    return 0x28280106;
  }
  return format;
}

}  // namespace

// Device::SetUpDisplay (from Device::Init): r3 device, r4 the presentation
// parameters (+0 width, +4 height, +8 the render target's format, +16
// passed to both surfaces, +36 non-zero for a depth surface, +40 its format,
// +56 and +60 non-zero to skip the render target and the front buffer, +64
// the front buffer's format, +68 the display's format, +96 the scaler's;
// the game sets +60). Creates the front buffer and the
// default render target and depth surface, sets them, and keeps a copy of
// the parameters. Returns 1, or 0 if a surface couldn't be made. The
// display mode is ours (src/display/display.h) instead of VdQueryVideoMode.
extern "C" REX_FUNC(sub_822ACE00) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, parameters = ctx.r4.u32;
  auto parameter = [&](uint32_t offset) { return guest::LoadU32(base, parameters + offset); };
  const uint32_t width = parameter(0), height = parameter(4);
  const uint32_t format = FrontBufferFormat(parameter(64));
  guest::StoreU32(base, device + kDeviceDisplayWidth, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayHeight, display::kHeight);
  guest::StoreU32(base, device + kDeviceDisplayPitch, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayFormat, parameter(68));
  bool made = true;
  InFrame(ctx, base, 208, 0x822ACEC8, [&](uint32_t) {
    if (parameter(60) == 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = 1;
      ctx.r6.u64 = 1;
      ctx.r7.u64 = 0;
      ctx.r8.u64 = format;
      ctx.r9.u64 = 0;
      ctx.r10.u64 = 3;
      ctx.lr = 0x822ACEC8;
      sub_822A5D08(ctx, base);  // a texture
      if (!ctx.r3.u32) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceFrontBuffer, ctx.r3.u32);
    }
    if (parameter(56) == 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = parameter(8);
      ctx.r6.u64 = parameter(16);
      ctx.r7.u64 = 0;
      ctx.lr = 0x822ACF00;
      sub_822A5E28(ctx, base);  // a surface
      const uint32_t surface = ctx.r3.u32;
      if (!surface) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceRenderTarget, surface);
      ctx.r3.u64 = device;
      ctx.r4.u64 = 0;
      ctx.r5.u64 = surface;
      ctx.lr = 0x822ACF18;
      sub_822A3D30(ctx, base);  // set as render target 0
    }
    if (parameter(36) != 0) {
      ctx.r3.u64 = width;
      ctx.r4.u64 = height;
      ctx.r5.u64 = parameter(40);
      ctx.r6.u64 = parameter(16);
      ctx.r7.u64 = 0;
      ctx.lr = 0x822ACF3C;
      sub_822A5E28(ctx, base);
      const uint32_t surface = ctx.r3.u32;
      if (!surface) {
        made = false;
        return;
      }
      guest::StoreU32(base, device + kDeviceDepthSurface, surface);
      ctx.r3.u64 = device;
      ctx.r4.u64 = surface;
      ctx.lr = 0x822ACF50;
      sub_822A4060(ctx, base);  // set as the depth surface
    }
    // Through the game's memcpy, as the original (it's wrapped for the
    // renderer's memory mirrors).
    ctx.r3.u64 = device + kDeviceParameters;
    ctx.r4.u64 = parameters;
    ctx.r5.u64 = 124;
    ctx.lr = 0x822ACF60;
    sub_825A7410(ctx, base);
    guest::StoreU32(base, device + kDeviceFrontBufferFormat, StoredFrontBufferFormat(format));
    ctx.r3.u64 = device;
    ctx.r4.u64 = parameters + 96;
    ctx.lr = 0x822ACFA0;
    sub_822B42D0(ctx, base);  // the scaler's parameters
    // The display mode is set (sub_822AD0B0) at the next Swap.
    guest::StoreU8(base, device + kDeviceFlags,
                   guest::LoadU8(base, device + kDeviceFlags) | 0x10);
  });
  ctx.r3.u64 = made ? 1 : 0;
}

// Device::Init (from sub_8229DB38): r3 device, r4 the presentation
// parameters. Returns 1, or 0 if a step failed. Left out: Device::InitEngines
// and RetrainEdram (dropped), VdIsHSIOTrainingSucceeded (it never fails), and
// handing two of the device's fields to the kernel's debug monitor data
// (KeDebugMonitorData: none without a debugger). Kept: the rest, as calls to
// the library's own functions, the console setting read
// (ExGetXConfigSetting, B5) and the critical sections (the core).
extern "C" REX_FUNC(sub_822AD298) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, parameters = ctx.r4.u32;
  bool made = false;
  InFrame(ctx, base, 128, 0x822AD2B8, [&](uint32_t frame) {
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD2B8;
    sub_822AD228(ctx, base);  // the device's fields and a physical block
    if (!ctx.r3.u32) {
      return;
    }
    ctx.r3.u64 = device + 13548;
    ctx.lr = 0x822AD2D0;
    __imp__RtlInitializeCriticalSection(ctx, base);
    ctx.r3.u64 = device + 13576;
    ctx.lr = 0x822AD2D8;
    __imp__RtlInitializeCriticalSection(ctx, base);
    guest::StoreU8(base, device + kDeviceFlags2, guest::LoadU8(base, device + kDeviceFlags2) | 8);
    guest::StoreU32(base, guest::LoadU32(base, kVdGlobalDeviceImport), device);
    guest::StoreU16(base, frame + 80, 0);
    ctx.r3.u64 = 3;
    ctx.r4.u64 = 10;
    ctx.r5.u64 = device + 15164;
    ctx.r6.u64 = 4;
    ctx.r7.u64 = frame + 80;
    ctx.lr = 0x822AD324;
    __imp__ExGetXConfigSetting(ctx, base);
    if (ctx.r3.s32 < 0) {
      return;
    }
    ctx.r3.u64 = device;
    ctx.r4.u64 = parameters + 72;
    ctx.lr = 0x822AD338;
    sub_8229F280(ctx, base);  // the ring and command buffer
    if (ctx.r3.u32) {
      return;
    }
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD348;
    sub_822B6D70(ctx, base);  // the worker threads
    if (!ctx.r3.u32) {
      return;
    }
    guest::StoreU32(base, device + 20008, 0xFFFFFFFF);
    guest::StoreU32(base, device + 20012, 0xFFFFFFFF);
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD364;
    sub_822AC2A0(ctx, base);
    ctx.r3.u64 = device;
    ctx.r4.u64 = parameters;
    ctx.lr = 0x822AD370;
    sub_822ACE00(ctx, base);  // Device::SetUpDisplay
    if (!ctx.r3.u32) {
      return;
    }
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD380;
    sub_822ACB90(ctx, base);
    ctx.r3.u64 = 0;
    ctx.r4.u64 = 0xB1800000;
    ctx.lr = 0x822AD38C;
    sub_82297790(ctx, base);
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD3E8;
    sub_8229EF48(ctx, base);  // ends the command segment
    // The performance counter's rate, and its period, as floats.
    const float rate = float(double(helpers::kPerformanceFrequency));
    guest::StoreU32(base, device + kDeviceTimerRate, std::bit_cast<uint32_t>(rate));
    guest::StoreU32(base, device + kDeviceTimerPeriod, std::bit_cast<uint32_t>(1.0f / rate));
    ctx.lr = 0x822AD41C;
    sub_822B1E70(ctx, base);  // hands debug callbacks to the debug monitor
    ctx.r3.u64 = device;
    ctx.r4.u64 = 0;
    ctx.lr = 0x822AD438;
    sub_822A3B90(ctx, base);
    if (guest::LoadU32(base, kTrainingFailed)) {
      ctx.r3.u64 = kTrainingFailedMessage;
      ctx.lr = 0x822AD468;
      sub_822AC530(ctx, base);  // prints it
    }
    ctx.r3.u64 = device;
    ctx.lr = 0x822AD470;
    sub_822B43C0(ctx, base);
    if (!ctx.r3.u32) {
      ctx.r3.u64 = kCheckFailedMessage;
      ctx.lr = 0x822AD484;
      sub_822AC530(ctx, base);
    }
    made = true;
  });
  ctx.r3.u64 = made ? 1 : 0;
}

namespace {

// A physical address as the library works it out from a virtual one (the
// 0xE... range is offset by 0x1000).
uint32_t PhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFF) + ((((address >> 20) & 0xFFF) + 512) & 0x1000);
}

void Free(PPCContext& ctx, uint8_t* base, uint32_t block, uint32_t return_address) {
  ctx.r3.u64 = block;
  ctx.r4.u64 = 0xB1800000;
  ctx.lr = return_address;
  sub_82297790(ctx, base);
}

uint32_t Allocate(PPCContext& ctx, uint8_t* base, uint32_t bytes, uint32_t flags,
                  uint32_t return_address) {
  ctx.r3.u64 = bytes;
  ctx.r4.u64 = flags;
  ctx.lr = return_address;
  sub_822976F8(ctx, base);
  return ctx.r3.u32;
}

constexpr uint32_t kDeviceCommandWrite = 40;      // where the next packet goes, minus 4
constexpr uint32_t kDeviceSegmentEnd = 44;
constexpr uint32_t kDeviceSegmentLimit = 48;      // past it, the segment is ended
constexpr uint32_t kDeviceProgressBlock = 10768;  // the fences our kick writes
constexpr uint32_t kDeviceCallbackBlock = 10772;  // the inserted callbacks'
constexpr uint32_t kDeviceCommandPosition = 10780;
constexpr uint32_t kDeviceRingWrite = 10820;
constexpr uint32_t kDeviceRingAllocated = 13436;
constexpr uint32_t kDeviceCommandAllocated = 13440;
constexpr uint32_t kDeviceRing = 13496;
constexpr uint32_t kDeviceRingMask = 13500;
constexpr uint32_t kDeviceCommandBuffer = 13504;
constexpr uint32_t kDeviceCommandLast = 13508;
constexpr uint32_t kDeviceSegmentBytes = 13512;
constexpr uint32_t kDeviceCommandWords = 13536;
constexpr uint32_t kOutOfMemory = 0x8007000E;

}  // namespace

// Device::SetUpRing (from Device::Init; with no parameters, from
// Device::Release, only frees): r3 device, r4 parameters (+4 the ring's
// size, +8 a ring, +12 the command buffer's size, +16 a command buffer, +20
// its segments; 0 for the defaults: 32 KB, 2 MB, 32). Allocates the primary
// ring, the command buffer, the progress block (the fences our kick writes,
// which every wait reads) and the callback block (the inserted callbacks'),
// sets the device's fields for them, and writes the scratch registers' set-up
// into the command buffer (our kick needs it for the callbacks). Returns 0,
// or 0x8007000E. Left out: the kernel's ring calls (VdInitializeRingBuffer,
// VdEnableRingBufferRPtrWriteBack, VdSetSystemCommandBufferGpuIdentifierAddress,
// and VdVerifyMEInitCommand, imported under the wrong name
// KiApcNormalRoutineNop) and the ME_INIT packet written into the primary
// ring, which nothing reads: our kick hands the GPU no ring.
extern "C" REX_FUNC(sub_8229F280) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, parameters = ctx.r4.u32;
  auto field = [&](uint32_t offset) { return guest::LoadU32(base, device + offset); };
  auto set = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, device + offset, value); };
  uint32_t result = 0;
  InFrame(ctx, base, 320, 0x8229F2A4, [&](uint32_t) {
    if (field(kDeviceCommandPosition)) {
      ctx.r3.u64 = device;
      ctx.lr = 0x8229F2A4;
      sub_8229F1B0(ctx, base);  // waits for the GPU
    }
    if (const uint32_t buffer = field(kDeviceCommandBuffer)) {
      ctx.r3.u64 = PhysicalAddress(buffer) - 0x40000000;
      ctx.r4.u64 = PhysicalAddress(field(kDeviceCommandLast)) - 0x40000000;
      ctx.lr = 0x8229F2F4;
      sub_822AC470(ctx, base);
    }
    Free(ctx, base, field(kDeviceRingAllocated), 0x8229F304);
    Free(ctx, base, field(kDeviceCommandAllocated), 0x8229F310);
    set(kDeviceRingAllocated, 0);
    set(kDeviceCommandAllocated, 0);
    set(kDeviceCommandWrite, 0);
    set(kDeviceSegmentEnd, 0);
    Free(ctx, base, field(kDeviceCallbackBlock), 0x8229F338);
    Free(ctx, base, field(kDeviceProgressBlock), 0x8229F344);
    if (!parameters) {
      return;
    }
    auto parameter = [&](uint32_t offset) {
      return guest::LoadU32(base, parameters + offset);
    };
    const uint32_t ring_bytes = parameter(4) ? parameter(4) : 0x8000;
    const uint32_t command_bytes = parameter(12) ? parameter(12) : 0x200000;
    const uint32_t segments = parameter(20) ? parameter(20) : 32;
    const uint32_t segment_bytes = command_bytes / segments;
    uint32_t ring = parameter(8);
    if (!ring) {
      ring = Allocate(ctx, base, ring_bytes, 0xB5800000, 0x8229F3AC);
      set(kDeviceRingAllocated, ring);
    }
    uint32_t buffer = parameter(16);
    if (!buffer) {
      buffer = Allocate(ctx, base, command_bytes,
                        command_bytes < 0x80000 ? 0xB7800000 : 0x87800000, 0x8229F3D8);
      set(kDeviceCommandAllocated, buffer);
    }
    result = kOutOfMemory;
    if (!ring || !buffer) {
      return;
    }
    const uint32_t progress = Allocate(ctx, base, 96, 0xA5800000, 0x8229F3FC);
    set(kDeviceProgressBlock, progress);
    if (!progress) {
      return;
    }
    const uint32_t callbacks = Allocate(ctx, base, 32, 0x95800000, 0x8229F414);
    set(kDeviceCallbackBlock, callbacks);
    if (!callbacks) {
      return;
    }
    for (uint32_t i = 0; i < 96; i += 4) {
      guest::StoreU32(base, progress + i, 0);
    }
    for (uint32_t i = 0; i < 32; i += 4) {
      guest::StoreU32(base, callbacks + i, 0);
    }
    const uint32_t segment_end = buffer + (segment_bytes & ~3u);
    const uint32_t last = buffer + (command_bytes & ~3u) - 4;
    guest::StoreU32(base, last, 0x0BADF00D);
    set(kDeviceCommandWords, command_bytes >> 2);
    set(kDeviceRing, ring);
    set(kDeviceCommandBuffer, buffer);
    set(13524, buffer);
    set(kDeviceCommandLast, last);
    set(13528, buffer);
    set(kDeviceSegmentBytes, segment_bytes);
    set(kDeviceRingMask, (ring_bytes >> 2) - 1);
    guest::StoreU32(base, progress + 60, 0);  // the ring's read pointer
    set(kDeviceRingWrite, 0);
    set(kDeviceCommandWrite, buffer - 4);
    set(kDeviceSegmentEnd, segment_end);
    set(kDeviceSegmentLimit, segment_end - 160);
    set(13516, 0);
    if (!field(kDeviceCommandPosition)) {
      set(kDeviceCommandPosition, 3);
    }
    guest::StoreU32(base, progress, field(kDeviceCommandPosition) - 2);
    guest::StoreU32(base, progress + 4, (field(13520) & 3) | field(kDeviceCommandWrite));
    // The scratch registers' set-up and three other register writes, into the
    // command buffer.
    uint32_t write = field(kDeviceCommandWrite);
    if (write > field(kDeviceSegmentLimit)) {
      ctx.r3.u64 = device;
      ctx.lr = 0x8229F5E8;
      sub_8229EF48(ctx, base);  // ends the segment, returns where to write
      write = ctx.r3.u32;
    }
    const uint32_t words[] = {
        0x00000D02, 0x00010800,                                      // register 0xD02
        0x00030A02, 0xC0100000, 0x07F00000, 0xC0000000, 0x00100000,  // 0xA02..0xA05
        0x000001DD, PhysicalAddress(callbacks),                      // SCRATCH_ADDR
        0x000001DC, 0x00020033,                                      // SCRATCH_UMSK
    };
    for (uint32_t word : words) {
      write += 4;
      guest::StoreU32(base, write, word);
    }
    set(kDeviceCommandWrite, write);
    ctx.r3.u64 = device;
    ctx.lr = 0x8229F668;
    sub_8229F0E0(ctx, base);  // WAIT_UNTIL and register 0xD04
    result = 0;
  });
  ctx.r3.u64 = result;
}

// Device::SetDisplayMode (from Swap, once, when Device::SetUpDisplay asked
// for it): r3 device, r4 the presentation parameters (the device's copy),
// r5 non-zero to set the mode. Runs the default gamma ramp through the
// display's gamma (sub_822B0E08 and sub_822A3A20, or for formats 7 and 54
// the piecewise-linear sub_822B0E60 and sub_822A3AA8), and puts the
// display's size in the device's fields. Left out: VdSetDisplayMode (the SDK
// ignores it) and VdGetCurrentDisplayInformation: the fields come from our
// display mode (src/display/display.h), as the SDK's answer did (the front
// buffer's color format 0).
extern "C" REX_FUNC(sub_822AD0B0) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, parameters = ctx.r4.u32;
  const bool set_mode = ctx.r5.u32 != 0;
  InFrame(ctx, base, 1744, 0x822AD0F4, [&](uint32_t frame) {
    if (!set_mode) {
      return;
    }
    const uint32_t ramp = frame + 176;
    const uint32_t format = guest::LoadU32(base, parameters + 64) & 0x3F;
    const bool piecewise_linear = format == 7 || format == 54;
    ctx.r3.u64 = ramp;
    ctx.r4.u64 = 0;
    ctx.lr = piecewise_linear ? 0x822AD114 : 0x822AD0F4;
    (piecewise_linear ? sub_822B0E60 : sub_822B0E08)(ctx, base);  // the default ramp
    ctx.r3.u64 = device;
    ctx.r4.u64 = 0;
    ctx.r5.u64 = ramp;
    ctx.lr = piecewise_linear ? 0x822AD128 : 0x822AD104;
    (piecewise_linear ? sub_822A3AA8 : sub_822A3A20)(ctx, base);  // set as the gamma ramp
  });
  guest::StoreU32(base, device + kDeviceDisplayWidth, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayHeight, display::kHeight);
  guest::StoreU32(base, device + kDeviceDisplayPitch, display::kWidth);
  guest::StoreU32(base, device + kDeviceDisplayFormat, 0);
}

namespace {

constexpr uint32_t kDeviceFlags3 = 10811;
constexpr uint32_t kDeviceScaler = 13312;           // 56 bytes
constexpr uint32_t kDeviceFrontBufferHeader = 13444;  // a copy of its 52 bytes
constexpr uint32_t kDeviceFence = 13540;            // the last Swap's command position
constexpr uint32_t kDeviceFenceBefore = 13544;      // the one before

}  // namespace

// Device::Swap (once a frame, from Renderer::Present's 8258DCA4; also from
// the movies' sub_822A0E50, the tiling worker and sub_8209ACB8): r3 device,
// r4 the front buffer (a texture header), r5 parameters (0, or the device's
// own +13284). Keeps the front buffer's header (sub_822A0718 reads its
// address every frame), sets the display mode at the first Swap, ends the
// command segment (the kick our renderer reads), takes the next one and
// waits for the last Swap's fence (sub_8229EC00, the frame throttle),
// records the fences, and marks every register block dirty (sub_822A3B90)
// for the next frame's first draw. r3 is left as sub_822A3B90 leaves it, as
// the original. Left out (see "Swap, step by step" under B3): the GPU
// timestamps (sub_822B1F68; only with a debugger), the segment end when
// 829537C0 is set (no code sets it), the capture check (sub_822B5C58), the
// kernel's VdGetSystemCommandBuffer and VdSwap and the 64 words reserved for
// it (+20120, +20432), the parameters' compare (sub_822B42D0; they never
// differ), the scaler's and gamma's display packets (Device::SetUpScaler,
// sub_822A05A8), the front buffer register 0x1844 (+20004) and the 7FC86110
// write, the display persistence (REG_RMW 0x1E4E, VdPersistDisplay,
// MmFreePhysicalMemory), RetrainEdram (dropped) and the frame statistics
// for a debugger (sub_822B20D8). The flags those steps clear are cleared.
extern "C" REX_FUNC(sub_822A08A0) {
  REX_FUNC_PROLOGUE();
  const uint32_t device = ctx.r3.u32, front_buffer = ctx.r4.u32;
  auto field = [&](uint32_t offset) { return guest::LoadU32(base, device + offset); };
  auto set = [&](uint32_t offset, uint32_t value) { guest::StoreU32(base, device + offset, value); };
  auto clear_bits = [&](uint32_t offset, uint8_t bits) {
    guest::StoreU8(base, device + offset, guest::LoadU8(base, device + offset) & ~bits);
  };
  InFrame(ctx, base, 384, 0x822A08C4, [&](uint32_t) {
    // Through the game's memcpy, as the original (it's wrapped for the
    // renderer's memory mirrors).
    ctx.r3.u64 = device + kDeviceFrontBufferHeader;
    ctx.r4.u64 = front_buffer;
    ctx.r5.u64 = 52;
    ctx.lr = 0x822A08C4;
    sub_825A7410(ctx, base);
    if (guest::LoadU8(base, device + kDeviceFlags) & 0x10) {
      ctx.r3.u64 = device;
      ctx.r4.u64 = device + kDeviceParameters;
      ctx.r5.u64 = 1;
      ctx.lr = 0x822A0AAC;
      sub_822AD0B0(ctx, base);  // Device::SetDisplayMode
      clear_bits(kDeviceFlags, 0x10);
    }
    if (guest::LoadU8(base, device + kDeviceFlags2) & 0x20) {
      for (uint32_t i = 0; i < 56; i += 4) {
        set(kDeviceScaler + i, 0);
      }
      clear_bits(kDeviceFlags2, 0x20);
    }
    clear_bits(kDeviceFlags3, 0x80);
    const uint32_t position = field(kDeviceCommandPosition);
    ctx.r3.u64 = device;
    ctx.lr = 0x822A0DFC;
    sub_8229EF48(ctx, base);  // ends the command segment
    ctx.r3.u64 = device;
    ctx.r4.u64 = field(kDeviceSegmentBytes) >> 2;
    ctx.lr = 0x822A0E1C;
    sub_8229EC00(ctx, base);  // the next segment, and waits for the last Swap's fence
    clear_bits(kDeviceFlags, 0x20);
    set(kDeviceFenceBefore, field(kDeviceFence));
    set(kDeviceFence, position);
    ctx.r3.u64 = device;
    ctx.r4.u64 = 0;
    ctx.lr = 0x822A0E48;
    sub_822A3B90(ctx, base);  // every register block dirty
  });
}
