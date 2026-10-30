// The sound manager's construction natively (start_up.h,
// docs/functions/sound-cues.md: AudioManager::Create, AudioManager's
// constructor and AudioEngine::StartXAudio).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void StoreU8(uint8_t* base, uint32_t address, uint8_t value) {
  guest::StoreU8(base, address, value);
}

constexpr uint32_t kListener = 0x8259D478;       // Listener::Listener
constexpr uint32_t kWordVectorFill = 0x821194A0;  // WordVector::Fill
constexpr uint32_t kPtrList = 0x820AD058;        // PtrList::PtrList
constexpr uint32_t kStringFromText = 0x825931D0;  // std::string::string(const char*)
constexpr uint32_t kStart3D = 0x8259CE10;        // AudioEngine::Start3D
constexpr uint32_t kXAudioInitialize = 0x8245A760;
constexpr uint32_t kDebugPrint = 0x825A1318;

constexpr uint32_t kMemberDelegateVtable = 0x8200833C;  // the 16-byte delegate
constexpr uint32_t kEmptyFunction = 0x82106540;

// A 16-byte member delegate {vtable, object, function, 0} at `at`, the
// function first written at frame + 80 with a 0 after it (the original's
// ld from there), then the key at frame + 80.
void MemberDelegate(uint8_t* base, uint32_t frame, uint32_t at, uint32_t object,
                    uint32_t function) {
  Store(base, frame + 84, 0);
  Store(base, frame + 80, function);
  Store(base, at + 0, kMemberDelegateVtable);
  Store(base, at + 4, object);
  Store(base, at + 8, function);
  Store(base, at + 12, 0);
}

}  // namespace

uint32_t StartXAudio(uint8_t* base, uint32_t frame, const AudioManagerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8259CD50 AudioEngine::StartXAudio");
  // The parameters at frame + 88 (bytes 6, 1, 48; +4 the block at frame +
  // 80, +8 the block at frame + 104), the two blocks' words.
  Store(base, frame + 108, 0);
  Store(base, frame + 84, 0x820713E0);
  Store(base, frame + 112, 0);
  Store(base, frame + 104, 0x820713D8);
  StoreU8(base, frame + 88, 6);
  StoreU8(base, frame + 80, 7);
  StoreU8(base, frame + 89, 1);
  Store(base, frame + 96, frame + 104);
  Store(base, frame + 92, frame + 80);
  StoreU8(base, frame + 90, 48);
  uint32_t r3 = calls.call(kXAudioInitialize, frame, frame + 88, {}, {}, 0x8259CDBC);
  if (int32_t(r3) < 0) {
    // Never in play: the debug prints (the file's line 173, then the
    // error), then the original's write to address 0.
    const uint32_t error = r3;
    calls.call(kDebugPrint, frame, 0x8206C03C, 0x82071484, 173, 0x8259CDE0);
    r3 = calls.call(kDebugPrint, frame, 0x82071460, error, {}, 0x8259CDF0);
    Store(base, 0, 0);
  }
  return r3;
}

uint32_t AudioManagerConstruct(uint8_t* base, uint32_t manager, uint32_t owner, uint32_t name,
                               uint32_t count, uint32_t frame, uint32_t start_frame,
                               const AudioManagerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_821197E0 AudioManager::AudioManager");
  Store(base, manager + 4, 0x82000D48);
  Store(base, manager + 8, 0x82000D48);
  Store(base, manager + 0, 0x82008388);
  Store(base, manager + 4, 0x82008374);
  Store(base, manager + 8, 0x82008364);
  Store(base, manager + 12, owner);
  // The audio engine at +16: its constructor is the empty function
  // (sub_82106540, nothing to do).
  calls.call(kListener, frame, manager + 1456, {}, {}, 0x8211984C);
  const uint32_t banks = Load(base, count);
  Store(base, frame + 80, 0);
  calls.call(kWordVectorFill, frame, manager + 1512, banks, frame + 80, 0x82119868);
  calls.call(kPtrList, frame, manager + 1528, {}, {}, 0x82119870);
  calls.call(kPtrList, frame, manager + 1540, {}, {}, 0x82119878);
  Store(base, manager + 1552, 1);
  calls.call(kStringFromText, frame, manager + 1556, name, {}, 0x8211988C);
  // The handler object at +1584 (a command, as the movie class's at +12).
  const uint32_t command = manager + 1584;
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 0);
  Store(base, command + 4, 0x82000AB4);
  Store(base, command + 8, 0);
  Store(base, command + 12, 0);
  StoreU8(base, command + 16, 0);
  StoreU8(base, command + 17, 1);
  StoreU8(base, command + 18, 0);
  Store(base, command + 0, 0x820066DC);
  Store(base, command + 4, 0x82002908);
  Store(base, command + 24, kMemberDelegateVtable);
  Store(base, command + 28, 0);
  Store(base, command + 32, 0);
  Store(base, command + 36, 0);
  // StartXAudio's prologue: lr, r30 (the command) and r31 (the manager)
  // above its frame.
  const uint32_t above = start_frame + 144;
  Store(base, above - 8, 0x821198F0);
  Store(base, above - 24, 0);
  Store(base, above - 20, command);
  Store(base, above - 16, 0);
  Store(base, above - 12, manager);
  StartXAudio(base, start_frame, calls);
  calls.call(kStart3D, frame, manager + 16, {}, {}, 0x821198F8);
  Store(base, frame + 84, 0);
  Store(base, command + 8, 0);
  Store(base, command + 12, 0);
  StoreU8(base, command + 16, 0);
  StoreU8(base, command + 17, 1);
  StoreU8(base, command + 18, 0);
  Store(base, frame + 80, 0x82118638);
  // +20: the frame's +180, a word the original never writes (left as the
  // stack holds it: B7 changes no game behavior).
  Store(base, command + 20, Load(base, frame + 180));
  Store(base, command + 28, manager);
  Store(base, command + 32, 0x82118638);
  Store(base, command + 36, 0);
  // The owner's slot 2, then that object's slot 1 with the command.
  const uint32_t queue = calls.virtual_call(Load(base, manager + 12), 8, {}, {}, 0x82119944);
  calls.virtual_call(queue, 4, command, 0, 0x8211995C);
  // Four handlers on the owner's slot 1 object (slots 4, 3, 2 and 1: keys
  // 7, 1, 6 and 8), each a member delegate on the frame.
  struct Handler {
    uint32_t function, delegate, key, slot, before, after;
  };
  const Handler handlers[] = {
      {kEmptyFunction, frame + 96, 7, 16, 0x821199A0, 0x821199B8},
      {0x82117E88, frame + 112, 1, 12, 0x821199FC, 0x82119A14},
      {0x82119118, frame + 128, 6, 8, 0x82119A54, 0x82119A6C},
      {0x82119188, frame + 144, 8, 4, 0x82119AB4, 0x82119ACC},
  };
  for (int i = 0; i < 4; i++) {
    const Handler& h = handlers[i];
    MemberDelegate(base, frame, h.delegate, manager, h.function);
    // The words the original leaves in the frame between the delegates.
    if (i == 1) {
      Store(base, frame + 96, 0x82000ABC);
    } else if (i == 2) {
      Store(base, frame + 112, 0x82000ABC);
    } else if (i == 3) {
      Store(base, frame + 128, 0x82000ABC);
    }
    Store(base, frame + 80, h.key);
    const uint32_t handlers_object =
        calls.virtual_call(Load(base, manager + 12), 4, {}, {}, h.before);
    calls.virtual_call(handlers_object, h.slot, frame + 80, h.delegate, h.after);
  }
  // The bank vector's words cleared again (its bounds check's trap never
  // reached: the same condition).
  const uint32_t first = Load(base, manager + 1512 + 4);
  if (first != 0) {
    const uint32_t words = uint32_t(int32_t(Load(base, manager + 1512 + 8) - first) >> 2);
    for (uint32_t i = 0; i < words; i++) {
      Store(base, first + 4 * i, 0);
    }
  }
  return manager;
}

std::optional<uint32_t> AudioManagerCreate(uint8_t* base, uint32_t owner, uint32_t name,
                                           uint32_t count, uint32_t frame, uint32_t start_frame,
                                           const AudioManagerCalls& calls) {
  const auto block = memory::crt::OperatorNew(base, kAudioManagerBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82119B38 AudioManager::Create");
  if (*block == 0) {
    return 0;
  }
  return AudioManagerConstruct(base, *block, owner, name, count, frame, start_frame, calls);
}

}  // namespace app_start_up
