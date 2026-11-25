// The kernel's events, semaphores and waits, for the game (E1, step 2:
// docs/rexglue-removal.md, "E1: the plan").
//
// Kernel objects in the game's memory (KEVENT, KSEMAPHORE) keep their state
// where the original kernel kept it: the dispatcher header's type (+0: 0 a
// notification event, 1 a synchronization (auto-reset) event, 5 a
// semaphore), its signal state (+4, big-endian; a semaphore's count) and a
// semaphore's limit (+0x10). Events made by handle (NtCreateEvent) are host
// records under handles of ours (threads/handles.h). One host mutex guards
// them all and one condition variable wakes every waiter on any signal; a
// waiter checks its objects again.
//
// What isn't ours went to the SDK (lifted::ImportFromSdk): waits on other
// objects (threads), on SDK handles, named events. Logged, then (since E2:
// no SDK runtime) the program stops; none happens in play.

#include "threads/handles.h"
#include "imports/native_imports.h"
#include "threads/threads.h"

#include "base/log.h"
#include "guest.h"

#include <chrono>
#include <condition_variable>
#include <iterator>
#include <mutex>
#include <optional>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <vector>

namespace threads {

namespace {

constexpr uint32_t kStatusSuccess = 0;
constexpr uint32_t kStatusTimeout = 0x102;
constexpr uint32_t kStatusInvalidHandle = 0xC0000008;

constexpr uint8_t kNotificationEvent = 0;
constexpr uint8_t kSynchronizationEvent = 1;
constexpr uint8_t kSemaphore = 5;

constexpr uint32_t kSignalState = 4;
constexpr uint32_t kSemaphoreLimit = 0x10;


struct HostEvent {
  uint8_t type;
  int32_t state;
};

struct State {
  std::mutex mutex;
  std::condition_variable signaled;
  std::unordered_map<uint32_t, HostEvent> events;  // by handle
  uint32_t next_handle = kEventHandleBase;
};

State& Get() {
  static State state;
  return state;
}

// A waitable: a kernel object in guest memory, or a host event by handle.
struct Object {
  uint32_t address;  // the guest object, or 0
  HostEvent* event;  // the host event, or null

  uint8_t Type() const { return event ? event->type : guest::LoadU8(address); }
  int32_t Signal() const {
    return event ? event->state : int32_t(guest::LoadU32(address + kSignalState));
  }
  void SetSignal(int32_t value) {
    if (event) {
      event->state = value;
    } else {
      guest::StoreU32(address + kSignalState, uint32_t(value));
    }
  }
  bool Signaled() const { return Signal() > 0; }
  // A satisfied wait's effect: an auto-reset event resets, a semaphore's
  // count drops.
  void Consume() {
    const uint8_t type = Type();
    if (type == kSynchronizationEvent) {
      SetSignal(0);
    } else if (type == kSemaphore) {
      SetSignal(Signal() - 1);
    }
  }
};

bool OurType(uint8_t type) {
  return type == kNotificationEvent || type == kSynchronizationEvent || type == kSemaphore;
}

void LogFallbackOnce(const char* what, uint32_t value) {
  static std::mutex mutex;
  static std::unordered_map<std::string_view, bool> logged;
  std::lock_guard lock(mutex);
  if (!logged[what]) {
    logged[what] = true;
    ITC_LOG_INFO("threads: {} {:08X} not handled by ours (the SDK's runtime is gone: stopping)", what, value);
  }
}

// The deadline from a wait's timeout pointer (0: none, wait for ever): a
// big-endian count of 100 ns, negative relative, positive absolute (system
// time, from 1601), 0 a poll.
std::optional<std::chrono::steady_clock::time_point> Deadline(uint32_t pointer) {
  const auto now = std::chrono::steady_clock::now();
  if (pointer == 0) {
    return std::nullopt;
  }
  const int64_t value = int64_t((uint64_t(guest::LoadU32(pointer)) << 32) |
                                guest::LoadU32(pointer + 4));
  using Ticks = std::chrono::duration<int64_t, std::ratio<1, 10'000'000>>;
  if (value <= 0) {
    return now + std::chrono::duration_cast<std::chrono::steady_clock::duration>(Ticks(-value));
  }
  constexpr int64_t kUnixEpochIn1601Ticks = 116444736000000000;
  const int64_t system_now =
      std::chrono::duration_cast<Ticks>(std::chrono::system_clock::now().time_since_epoch())
          .count() +
      kUnixEpochIn1601Ticks;
  const int64_t left = value > system_now ? value - system_now : 0;
  return now + std::chrono::duration_cast<std::chrono::steady_clock::duration>(Ticks(left));
}

// Waits for all (`all`) or any of `objects`; the status (0 + the index for
// any; kStatusTimeout).
uint32_t Wait(std::vector<Object>& objects, bool all,
              std::optional<std::chrono::steady_clock::time_point> deadline) {
  State& state = Get();
  std::unique_lock lock(state.mutex);
  bool timed_out = false;
  for (;;) {
    if (all) {
      bool ready = true;
      for (const Object& object : objects) {
        ready = ready && object.Signaled();
      }
      if (ready) {
        for (Object& object : objects) {
          object.Consume();
        }
        return kStatusSuccess;
      }
    } else {
      for (size_t i = 0; i < objects.size(); i++) {
        if (objects[i].Signaled()) {
          objects[i].Consume();
          return kStatusSuccess + uint32_t(i);
        }
      }
    }
    if (timed_out) {
      return kStatusTimeout;
    }
    if (!deadline) {
      state.signaled.wait(lock);
    } else if (state.signaled.wait_until(lock, *deadline) == std::cv_status::timeout) {
      timed_out = true;  // one last look first
    }
  }
}

// A guest object of ours at `address`, else nothing.
std::optional<Object> GuestObject(uint32_t address) {
  if (address == 0 || !OurType(guest::LoadU8(address))) {
    return std::nullopt;
  }
  return Object{address, nullptr};
}

void Notify() {
  Get().signaled.notify_all();
}

// KeSetEvent(event, increment, wait): the previous state.
lifted::Result SetEvent(const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeSetEvent");
  Object object{uint32_t(args.r[0]), nullptr};
  int32_t previous;
  {
    std::lock_guard lock(Get().mutex);
    previous = object.Signal();
    object.SetSignal(1);
  }
  Notify();
  return {uint32_t(previous), 0.0};
}

// KeResetEvent(event): the previous state.
lifted::Result ResetEvent(const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeResetEvent");
  Object object{uint32_t(args.r[0]), nullptr};
  std::lock_guard lock(Get().mutex);
  const int32_t previous = object.Signal();
  object.SetSignal(0);
  return {uint32_t(previous), 0.0};
}

// KeInitializeSemaphore(semaphore, count, limit).
lifted::Result InitializeSemaphore(const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeInitializeSemaphore");
  const uint32_t address = uint32_t(args.r[0]);
  std::lock_guard lock(Get().mutex);
  guest::StoreU8(address, kSemaphore);
  guest::StoreU32(address + kSignalState, uint32_t(args.r[1]));
  guest::StoreU32(address + kSemaphoreLimit, uint32_t(args.r[2]));
  return {0, 0.0};
}

// KeReleaseSemaphore(semaphore, increment, adjustment, wait): the previous
// count; past the limit nothing changes (as the SDK).
lifted::Result ReleaseSemaphore(const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeReleaseSemaphore");
  const uint32_t address = uint32_t(args.r[0]);
  const int32_t adjustment = int32_t(uint32_t(args.r[2]));
  Object object{address, nullptr};
  int32_t previous;
  {
    std::lock_guard lock(Get().mutex);
    previous = object.Signal();
    const int32_t limit = int32_t(guest::LoadU32(address + kSemaphoreLimit));
    if (adjustment > 0 && int64_t(previous) + adjustment <= limit) {
      object.SetSignal(previous + adjustment);
    } else {
      ITC_LOG_ERROR("threads: semaphore {:08X} released past its limit ({} + {} > {})", address,
                    previous, adjustment, limit);
    }
  }
  Notify();
  return {uint32_t(previous), 0.0};
}

// KeWaitForSingleObject(object, reason, mode, alertable, timeout).
lifted::Result WaitForSingleObject(const lifted::Args& args) {
  const uint32_t address = uint32_t(args.r[0]);
  std::optional<Object> object = GuestObject(address);
  if (!object) {
    LogFallbackOnce("KeWaitForSingleObject on the object at", address);
    return lifted::ImportFromSdk(__imp__KeWaitForSingleObject, args);
  }
  ITC_NATIVE_RUNNING("import KeWaitForSingleObject");
  std::vector<Object> objects{*object};
  return {Wait(objects, true, Deadline(uint32_t(args.r[4]))), 0.0};
}

// KeWaitForMultipleObjects(count, objects, type (0 all, 1 any), reason,
// mode, alertable, timeout, wait blocks).
lifted::Result WaitForMultipleObjects(const lifted::Args& args) {
  const uint32_t count = uint32_t(args.r[0]);
  const uint32_t list = uint32_t(args.r[1]);
  std::vector<Object> objects;
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t address = guest::LoadU32(list + 4 * i);
    std::optional<Object> object = GuestObject(address);
    if (!object) {
      LogFallbackOnce("KeWaitForMultipleObjects on the object at", address);
      return lifted::ImportFromSdk(__imp__KeWaitForMultipleObjects, args);
    }
    objects.push_back(*object);
  }
  ITC_NATIVE_RUNNING("import KeWaitForMultipleObjects");
  return {Wait(objects, uint32_t(args.r[2]) == 0, Deadline(uint32_t(args.r[6]))), 0.0};
}

// NtCreateEvent(handle out, attributes, type, initial state): named events
// (the attributes' name) are the SDK's.
lifted::Result CreateEvent(const lifted::Args& args) {
  const uint32_t attributes = uint32_t(args.r[1]);
  if (attributes != 0 && guest::LoadU32(attributes + 4) != 0) {
    LogFallbackOnce("NtCreateEvent of a named event, attributes", attributes);
    return lifted::ImportFromSdk(__imp__NtCreateEvent, args);
  }
  ITC_NATIVE_RUNNING("import NtCreateEvent");
  State& state = Get();
  uint32_t handle;
  {
    std::lock_guard lock(state.mutex);
    handle = state.next_handle;
    state.next_handle += 4;
    state.events[handle] =
        HostEvent{uint8_t(uint32_t(args.r[2]) == 0 ? kNotificationEvent : kSynchronizationEvent),
                  uint32_t(args.r[3]) != 0 ? 1 : 0};
  }
  if (const uint32_t out = uint32_t(args.r[0])) {
    guest::StoreU32(out, handle);
  }
  return {kStatusSuccess, 0.0};
}

HostEvent* FindEvent(uint32_t handle) {
  auto& events = Get().events;
  const auto it = events.find(handle);
  return it == events.end() ? nullptr : &it->second;
}

// NtSetEvent(handle, previous state out).
lifted::Result NtSetEvent(const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  if (!OurHandle(handle)) {
    LogFallbackOnce("NtSetEvent on handle", handle);
    return lifted::ImportFromSdk(__imp__NtSetEvent, args);
  }
  ITC_NATIVE_RUNNING("import NtSetEvent");
  int32_t previous;
  {
    std::lock_guard lock(Get().mutex);
    HostEvent* event = FindEvent(handle);
    if (!event) {
      return {kStatusInvalidHandle, 0.0};
    }
    previous = event->state;
    event->state = 1;
  }
  Notify();
  if (const uint32_t out = uint32_t(args.r[1])) {
    guest::StoreU32(out, uint32_t(previous));
  }
  return {kStatusSuccess, 0.0};
}

// NtClearEvent(handle).
lifted::Result NtClearEvent(const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  if (!OurHandle(handle)) {
    LogFallbackOnce("NtClearEvent on handle", handle);
    return lifted::ImportFromSdk(__imp__NtClearEvent, args);
  }
  ITC_NATIVE_RUNNING("import NtClearEvent");
  std::lock_guard lock(Get().mutex);
  HostEvent* event = FindEvent(handle);
  if (!event) {
    return {kStatusInvalidHandle, 0.0};
  }
  event->state = 0;
  return {kStatusSuccess, 0.0};
}

// NtWaitForSingleObjectEx(handle, mode, alertable, timeout).
lifted::Result NtWaitForSingleObjectEx(const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  if (!OurHandle(handle)) {
    LogFallbackOnce("NtWaitForSingleObjectEx on handle", handle);
    return lifted::ImportFromSdk(__imp__NtWaitForSingleObjectEx, args);
  }
  ITC_NATIVE_RUNNING("import NtWaitForSingleObjectEx");
  HostEvent* event;
  {
    std::lock_guard lock(Get().mutex);
    event = FindEvent(handle);
  }
  if (!event) {
    ITC_LOG_ERROR("threads: NtWaitForSingleObjectEx on {:08X}, not an event of ours", handle);
    return {kStatusInvalidHandle, 0.0};
  }
  std::vector<Object> objects{Object{0, event}};
  return {Wait(objects, true, Deadline(uint32_t(args.r[3]))), 0.0};
}

// NtClose(handle): ours forgotten, others the SDK's.
lifted::Result NtClose(const lifted::Args& args) {
  const uint32_t handle = uint32_t(args.r[0]);
  if (!OurHandle(handle)) {
    return lifted::ImportFromSdk(__imp__NtClose, args);
  }
  ITC_NATIVE_RUNNING("import NtClose");
  if (ThreadHandle(handle)) {
    return {CloseThreadHandle(handle) ? kStatusSuccess : kStatusInvalidHandle, 0.0};
  }
  std::lock_guard lock(Get().mutex);
  return {Get().events.erase(handle) != 0 ? kStatusSuccess : kStatusInvalidHandle, 0.0};
}

// KeDelayExecutionThread(mode, alertable, interval): a sleep (no APCs).
lifted::Result DelayExecutionThread(const lifted::Args& args) {
  ITC_NATIVE_RUNNING("import KeDelayExecutionThread");
  const auto deadline = Deadline(uint32_t(args.r[2]));
  if (deadline) {
    std::this_thread::sleep_until(*deadline);
  }
  std::this_thread::yield();
  return {kStatusSuccess, 0.0};
}

const imports::NativeImportEntry kEntries[] = {
    {__imp__KeSetEvent, SetEvent},
    {__imp__KeResetEvent, ResetEvent},
    {__imp__KeInitializeSemaphore, InitializeSemaphore},
    {__imp__KeReleaseSemaphore, ReleaseSemaphore},
    {__imp__KeWaitForSingleObject, WaitForSingleObject},
    {__imp__KeWaitForMultipleObjects, WaitForMultipleObjects},
    {__imp__NtCreateEvent, CreateEvent},
    {__imp__NtSetEvent, NtSetEvent},
    {__imp__NtClearEvent, NtClearEvent},
    {__imp__NtWaitForSingleObjectEx, NtWaitForSingleObjectEx},
    {__imp__NtClose, NtClose},
    {__imp__KeDelayExecutionThread, DelayExecutionThread},
};

const bool kRegistered = imports::RegisterNativeImports(kEntries, std::size(kEntries), Enabled);

}  // namespace

}  // namespace threads
