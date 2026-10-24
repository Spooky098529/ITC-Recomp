// The video output's device lock (src/graphics/video_output.h,
// docs/functions/video-output.md) against the recompiled functions. The
// video output and the device's thread word are in the scratch area (a
// device whose +10760 lands there; nothing else of it is touched), never
// the game's: the native lock of a scratch video output is its own host
// semaphore, made by the native constructor. Lock and unlock run in pairs
// on this thread only, so nothing waits. The originals run with
// --native_video_output off (their callees' adapters then call the
// originals too). Everything is compared exactly (no floats); the guest
// semaphore's 40 bytes at +224, which the native constructor no longer
// writes (the host semaphore takes their place), are their own group,
// expected to differ.

#include "harness.h"

#include "base/flags.h"
#include "graphics/video_output.h"

#include <fmt/format.h>

#include <string>

ITC_FLAG_DECLARE(bool, native_video_output);

namespace harness {

namespace {

namespace vo = graphics::video_output;

constexpr uint32_t kObject = 0;      // the video output, in the scratch area
constexpr uint32_t kLocal = 512;     // the caller's local holding the device
constexpr uint32_t kThreadWord = 1024;  // the device's +10760

// Runs `body` with the native flag off (v == 0) or on.
template <typename Body>
void With(int v, Body body) {
  const bool saved = ITC_FLAG(native_video_output);
  ITC_FLAG(native_video_output) = v != 0;
  body();
  ITC_FLAG(native_video_output) = saved;
}

uint32_t Device(const Context& h) { return h.data + kThreadWord - vo::kDeviceThread; }

// The video output's bytes but the semaphore's.
void ObjectBytes(Context& h, Outputs& out) {
  const uint32_t object = h.data + kObject;
  out.Bytes(Read(h.base, object, vo::kLock));
  out.Bytes(Read(h.base, object + vo::kLock + vo::kLockBytes,
                 vo::kObjectBytes - vo::kLock - vo::kLockBytes));
}

void Construct(Context& h, int v) {
  PPCContext t = CallContext(h);
  t.r3.u64 = h.data + kObject;
  With(v, [&] {
    if (v == 0) {
      __imp__sub_8258DB18(t, h.base);
    } else {
      t.r3.u64 = vo::Construct(h.base, t.r3.u32);
    }
  });
}

}  // namespace

void VideoOutput(Context& h) {
  Random& r = h.random;

  {
    Results results("CurrentThreadId (sub_825B9A60)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        PPCContext t = CallContext(h);
        With(v, [&] {
          if (v == 0) {
            __imp__sub_825B9A60(t, h.base);
          } else {
            t.r3.u64 = vo::CurrentThreadId();
          }
        });
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }

  {
    Results results("Device::AcquireThreadOwnership (sub_8229D9D0)");
    for (int c = 0; c < 8; c++) {
      const uint32_t before = c < 2 ? 0 : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32(h.base, h.data + kThreadWord, before);
        PPCContext t = CallContext(h);
        t.r3.u64 = Device(h);
        With(v, [&] {
          if (v == 0) {
            __imp__sub_8229D9D0(t, h.base);
          } else {
            t.r3.u64 = vo::AcquireThreadOwnership(h.base, t.r3.u32);
          }
        });
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("the word before {:08X}", before));
    }
    results.Report();
  }

  {
    Results results("Device::ReleaseThreadOwnership (sub_8229DA10)");
    for (int c = 0; c < 8; c++) {
      const uint32_t before = c < 2 ? 0 : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        guest::StoreU32(h.base, h.data + kThreadWord, before);
        PPCContext t = CallContext(h);
        t.r3.u64 = Device(h);
        With(v, [&] {
          if (v == 0) {
            __imp__sub_8229DA10(t, h.base);
          } else {
            vo::ReleaseThreadOwnership(h.base, t.r3.u32);
          }
        });
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("the word before {:08X}", before));
    }
    results.Report();
  }

  // The constructor writes the vblank count (0x8296BD30): put back after.
  const uint32_t vblank_count = guest::LoadU32(h.base, vo::kVblankCount);
  {
    Results results("VideoOutput::VideoOutput (sub_8258DB18)");
    // The guest semaphore's bytes at +224: the native constructor leaves
    // them as they were (a host std::binary_semaphore takes their place, the
    // user's call, 2026-10-24); the original makes an event and a critical
    // section there.
    Results semaphore("VideoOutput::VideoOutput (sub_8258DB18), its semaphore at +224 "
                      "(expected to differ)");
    for (int c = 0; c < 4; c++) {
      Outputs out[2];
      Outputs lock[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes, uint8_t(0xCD + c));
        guest::StoreU32(h.base, vo::kVblankCount, 0x12345678u + c);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + kObject;
        With(v, [&] {
          if (v == 0) {
            __imp__sub_8258DB18(t, h.base);
          } else {
            t.r3.u64 = vo::Construct(h.base, t.r3.u32);
          }
        });
        ObjectBytes(h, out[v]);
        out[v].Bytes(Read(h.base, h.data + vo::kObjectBytes, kScratchBytes - vo::kObjectBytes));
        out[v].Word(guest::LoadU32(h.base, vo::kVblankCount));
        out[v].Word(t.r3.u32);
        lock[v].Bytes(Read(h.base, h.data + kObject + vo::kLock, vo::kLockBytes));
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
      semaphore.Check(lock[0], lock[1], fmt::format("case {}", c));
    }
    results.Report();
    semaphore.Report();
  }

  {
    // Lock, unlock, lock again (it doesn't wait: the count is back), unlock
    // again, on a video output just made by each side's constructor.
    Results results("VideoOutput::LockDevice (sub_8258D8C8) and UnlockDevice (sub_8258D908)");
    for (int c = 0; c < 6; c++) {
      const uint32_t before = c < 2 ? 0 : uint32_t(r.Next());
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        Construct(h, v);
        const uint32_t object = h.data + kObject;
        const uint32_t local = h.data + kLocal;
        guest::StoreU32(h.base, object + vo::kDevice, Device(h));
        guest::StoreU32(h.base, h.data + kThreadWord, before);
        for (int round = 0; round < 2; round++) {
          PPCContext t = CallContext(h);
          t.r3.u64 = object;
          With(v, [&] {
            if (v == 0) {
              __imp__sub_8258D8C8(t, h.base);
            } else {
              t.r3.u64 = vo::LockDevice(h.base, t.r3.u32);
            }
          });
          out[v].Word(t.r3.u32);
          out[v].Word(guest::LoadU32(h.base, h.data + kThreadWord));
          guest::StoreU32(h.base, local, t.r3.u32);
          t = CallContext(h);
          t.r3.u64 = object;
          t.r4.u64 = local;
          With(v, [&] {
            if (v == 0) {
              __imp__sub_8258D908(t, h.base);
            } else {
              vo::UnlockDevice(h.base, t.r3.u32, t.r4.u32);
            }
          });
          out[v].Word(guest::LoadU32(h.base, local));
          out[v].Word(guest::LoadU32(h.base, h.data + kThreadWord));
          ObjectBytes(h, out[v]);
        }
      }
      results.Check(out[0], out[1], fmt::format("the word before {:08X}", before));
    }
    results.Report();
  }
  guest::StoreU32(h.base, vo::kVblankCount, vblank_count);
}

}  // namespace harness
