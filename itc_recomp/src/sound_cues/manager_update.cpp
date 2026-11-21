// AudioManager::Update's callees (docs/functions/sound-cues.md) as plain C++:
// XACT's per-frame work, the released banks freed, the Sounds updated and
// the stopped ones removed. Their callees (lifted, XACT through its vtables)
// are called through the dispatch table (hooks/lifted_call.h). Adapters:
// the --find_menu_selection wraps in src/debug/menu_selection_scan.cpp.

#include "sound_cues/manager_update.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"
#include "schema/menu_sounds.h"
#include "schema/sound_cues.h"

namespace sound_cues {

namespace {

constexpr uint32_t kCanFree = 0x821176E8;        // BankPair::CanFree
constexpr uint32_t kDeletePair = 0x82118438;     // the bank pair slot's delete
constexpr uint32_t kRemoveIf = 0x82118850;       // the list's remove_if
constexpr uint32_t kForEach = 0x82118F50;        // std::for_each over a list
constexpr uint32_t kDeleteIfStopped = 0x821185C8;  // AudioManager::DeleteIfStopped
constexpr uint32_t kSoundUpdate = 0x82117438;    // Sound::Update

// The Sounds' list: AudioManager (menu-sounds.toml) has no field for it yet.
constexpr uint32_t kSounds = 1528;

// A doubleword on the frame (ld).
uint64_t LoadDoubleword(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}

// The vector's size as the original works it out (srawi by 2 of the
// difference, compared unsigned).
uint32_t Count(uint32_t begin, uint32_t end) {
  return uint32_t(int32_t(end - begin) >> 2);
}

// The original's bounds check before an element's read: a trap (logged, run
// past) when the vector is empty or `i` is past its end.
uint32_t CheckedBegin(uint8_t* base, uint32_t manager, uint32_t end, uint32_t i) {
  const uint32_t begin = schema::AudioManager(base, manager).bank_pairs_first();
  if (begin == 0 || i >= Count(begin, end)) {
    lifted::Trap(22);
  }
  return begin;
}

}  // namespace

uint64_t AudioEngineDoWork(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_8259D1A8 AudioEngine::DoWork");
  // A tail call: slot +16 of the XACT engine (+8), from this stack pointer.
  const schema::XactEngine xact = schema::AudioEngine(base, engine).xact_engine();
  const uint32_t work = xact.vtable().do_work();
  return lifted_call::Call(base, work, sp, r13, {xact.address}).r3;
}

uint64_t ManagerFreeReleasedBanks(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_821184E8 AudioManager::FreeReleasedBanks");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 112);
  uint64_t r3 = manager;
  for (uint32_t i = 0, offset = 0;; i++, offset += 4) {
    const uint32_t begin = schema::AudioManager(base, manager).bank_pairs_first();
    if (begin == 0) {
      break;
    }
    const uint32_t end = schema::AudioManager(base, manager).bank_pairs_end();
    if (i >= Count(begin, end)) {
      break;
    }
    // Each read of an element re-reads the vector and checks the index.
    if (!schema::BankPairSlot(base, CheckedBegin(base, manager, end, i) + offset).pair()) {
      continue;
    }
    const uint32_t pair_begin =
        CheckedBegin(base, manager, schema::AudioManager(base, manager).bank_pairs_end(), i);
    r3 = lifted_call::Call(base, kCanFree, frame, r13,
                           {schema::BankPairSlot(base, pair_begin + offset).pair().address})
             .r3;
    if ((r3 & 0xFF) == 0) {
      continue;
    }
    const uint32_t slot_begin =
        CheckedBegin(base, manager, schema::AudioManager(base, manager).bank_pairs_end(), i);
    r3 = lifted_call::Call(base, kDeletePair, frame, r13, {slot_begin + offset}).r3;
  }
  return r3;
}

uint64_t ManagerRemoveStoppedSounds(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_82118C18 AudioManager::RemoveStoppedSounds");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 112);
  // The predicate, a member-function pointer (DeleteIfStopped, this
  // adjustment 0), and the manager built on the frame and passed as two
  // doublewords: r5's low word is the frame's +92, which the original never
  // writes (read as it leaves it).
  guest::StoreU32(base, frame + 84, 0);
  guest::StoreU32(base, frame + 80, kDeleteIfStopped);
  guest::StoreU32(base, frame + 88, manager);
  const uint64_t r4 = LoadDoubleword(base, frame + 80);
  const uint64_t r5 = LoadDoubleword(base, frame + 88);
  return lifted_call::Call(base, kRemoveIf, frame, r13, {manager + kSounds, r4, r5}).r3;
}

uint64_t ManagerUpdateSounds(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_82118FB0 AudioManager::UpdateSounds");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 112);
  // std::for_each(list.begin(), list.end(), Sound::Update): the iterators
  // (list, node) built on the frame as the original does, the result (the
  // function) left at +80.
  const uint32_t list = manager + kSounds;
  const uint32_t head = schema::SoundList(base, list).head().address;
  const uint32_t first = schema::SoundListNode(base, head).next().address;
  guest::StoreU32(base, frame + 88, list);
  guest::StoreU32(base, frame + 80, list);
  guest::StoreU32(base, frame + 84, head);
  guest::StoreU32(base, frame + 92, first);
  const uint64_t end = LoadDoubleword(base, frame + 80);
  const uint64_t begin = LoadDoubleword(base, frame + 88);
  return lifted_call::Call(base, kForEach, frame, r13, {frame + 80, begin, end, kSoundUpdate}).r3;
}

}  // namespace sound_cues
