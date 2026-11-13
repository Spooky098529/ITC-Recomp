// The game's Thread class and the XAPI thread wrappers (thread.h,
// docs/functions/frame-driver.md): step for step as the recompiled functions,
// the kernel imports guest calls.

#include "frame_driver/thread.h"

#include "guest.h"

#include "base/log.h"

namespace frame_driver {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

constexpr uint32_t kInvalidParameter = 0xC000000D;

}  // namespace

void ThreadConstruct(uint8_t* base, uint32_t thread) {
  ITC_NATIVE_RUNNING("sub_8258DE88 Thread::Thread");
  Store(base, thread + kThreadObject, 0);
  Store(base, thread + 0, kThreadVtable);
  Store(base, thread + kThreadHandle, 0xFFFFFFFFu);
  Store(base, thread + kThreadProcessor, 0);
  Store(base, thread + kThreadPriority, 2);
}

uint32_t CreateThread(uint8_t* base, uint32_t stack, uint32_t entry, uint32_t parameter,
                      uint32_t flags, uint32_t processor, uint32_t id,
                      const ThreadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA380 CreateThread");
  uint32_t kernel_flags = (flags >> 2) & 1;
  if (processor != 0xFFFFFFFFu) {
    const uint32_t bit = (processor & 0x20) ? 0 : (1u << (processor & 0x1F));
    kernel_flags |= (bit << 24) & 0xFF000000u;
  }
  uint32_t handle = 0;
  calls.in_frame(96, 0x825BA3C8, [&](uint32_t frame) {
    const uint32_t status =
        calls.import(Import::kExCreateThread,
                     {frame + 80, stack, id, kXapiThreadStart, entry, parameter, kernel_flags},
                     0x825BA3C8);
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);
      handle = 0;
      return;
    }
    handle = Load(base, frame + 80);
  });
  return handle;
}

uint32_t SetThreadPriority(uint8_t* base, uint32_t handle, int32_t priority,
                           const ThreadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822977B0 SetThreadPriority");
  uint32_t result = 0;
  calls.in_frame(112, 0x822977D4, [&](uint32_t frame) {
    const uint32_t status =
        calls.import(Import::kObReferenceObjectByHandle,
                     {handle, Load(base, kThreadObjectType), frame + 80}, 0x822977D4);
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);  // sub_82296E68, a branch to it
      result = 0;
      return;
    }
    const int32_t level = priority == 15 ? 16 : priority == -15 ? -16 : priority;
    calls.import(Import::kKeSetBasePriorityThread,
                 {Load(base, frame + 80), uint64_t(int64_t(level))}, 0x82297804);
    calls.import(Import::kObDereferenceObject, {Load(base, frame + 80)}, 0x8229780C);
    result = 1;
  });
  return result;
}

uint32_t XSetThreadProcessor(uint8_t* base, uint32_t handle, uint32_t processor,
                             const ThreadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82297888 XSetThreadProcessor");
  uint32_t result = 0;
  calls.in_frame(112, 0x822978B8, [&](uint32_t frame) {
    uint32_t status = kInvalidParameter;
    if (processor < 6) {
      status = calls.import(Import::kObReferenceObjectByHandle,
                            {handle, Load(base, kThreadObjectType), frame + 80}, 0x822978B8);
      if (int32_t(status) >= 0) {
        const uint32_t mask = (processor & 0x20) ? 0 : (1u << (processor & 0x1F));
        status = calls.import(Import::kKeSetAffinityThread,
                              {Load(base, frame + 80), mask, frame + 84}, 0x822978D4);
        calls.import(Import::kObDereferenceObject, {Load(base, frame + 80)}, 0x822978E0);
      }
    }
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);  // sub_82296E68, a branch to it
      result = 0xFFFFFFFFu;
      return;
    }
    const uint32_t old = Load(base, frame + 84);
    result = old == 0 ? 0 : 31 - uint32_t(__builtin_clz(old));
  });
  return result;
}

uint32_t ResumeThread(uint8_t* base, uint32_t handle, const ThreadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825BA118 ResumeThread");
  uint32_t result = 0;
  calls.in_frame(96, 0x825BA12C, [&](uint32_t frame) {
    const uint32_t status =
        calls.import(Import::kNtResumeThread, {handle, frame + 80}, 0x825BA12C);
    if (int32_t(status) < 0) {
      calls.set_last_error_from_status(status);
      result = 0xFFFFFFFFu;
      return;
    }
    result = Load(base, frame + 80);
  });
  return result;
}

uint32_t ThreadBegin(uint8_t* base, uint32_t thread, uint32_t object, uint32_t processor,
                     uint32_t priority, const ThreadCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8258DF18 Thread::Begin");
  uint32_t r3 = 0;
  calls.in_frame(128, 0x8258DF7C, [&](uint32_t frame) {
    Store(base, thread + kThreadObject, object != 0 ? object : thread);
    Store(base, thread + kThreadProcessor, Load(base, processor));
    Store(base, thread + kThreadPriority, Load(base, priority));
    // sub_825BA158: CreateThread without a processor (a branch: its frame
    // directly below this one).
    ITC_NATIVE_RUNNING("sub_825BA158 CreateThread (no processor)");
    const uint32_t handle = CreateThread(base, 0, kThreadStart, thread, 4, 0xFFFFFFFFu, 0, calls);
    Store(base, thread + kThreadHandle, handle);
    r3 = handle;
    if (handle == 0xFFFFFFFFu) {
      return;
    }
    const int32_t table[] = {2, 1, 0, -1, -2};
    for (uint32_t i = 0; i < 5; i++) {
      Store(base, frame + 80 + 4 * i, uint32_t(table[i]));
    }
    const uint32_t index = Load(base, thread + kThreadPriority);
    SetThreadPriority(base, handle, int32_t(Load(base, frame + 80 + ((index << 2) & 0xFFFFFFFCu))),
                      calls);
    XSetThreadProcessor(base, Load(base, thread + kThreadHandle),
                        Load(base, thread + kThreadProcessor), calls);
    r3 = ResumeThread(base, Load(base, thread + kThreadHandle), calls);
  });
  return r3;
}

}  // namespace frame_driver
