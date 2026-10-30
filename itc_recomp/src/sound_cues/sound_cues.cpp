#include "sound_cues/sound_cues.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"

#include <bit>

namespace sound_cues {

namespace {

constexpr uint32_t kZero = 0x82000A34;  // the game's float 0.0
constexpr uint32_t kOne = 0x82000A30;   // the game's float 1.0
constexpr uint32_t kListenerZ = 0x82001FD8;
constexpr uint32_t kListener48 = 0x82002A0C;
constexpr uint32_t kListener52 = 0x820027A4;

constexpr uint32_t kListener = 1456;  // the manager's listener
constexpr uint32_t kBanks = 1516;     // the manager's bank vector (begin, end)

// A float moved as lfs and stfs move it (single to double and back).
void CopyFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}

void CopyVector3(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t i = 0; i < 12; i += 4) {
    CopyFloat(base, to + i, from + i);
  }
}

// The bank slot's word for the index at `index_address`, from the vector
// at `vector` (begin, end); false where the original traps.
bool BankSlot(uint8_t* base, uint32_t vector, uint32_t index_address, uint32_t* pair) {
  const uint32_t begin = guest::LoadU32(base, vector);
  const uint32_t index = guest::LoadU32(base, index_address);
  if (begin == 0) {
    return false;
  }
  const uint32_t count = uint32_t(int32_t(guest::LoadU32(base, vector + 4) - begin) >> 2);
  if (index >= count) {
    return false;
  }
  *pair = guest::LoadU32(base, begin + 4 * index);
  return true;
}

}  // namespace

void ListenerConstruct(uint8_t* base, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_8259D478 Listener::Listener");
  // Front (0, 0, 1), top (0, 1, 0), position (0, 0, z), velocity 0.
  const uint32_t words[12] = {kZero, kZero, kOne, kZero, kOne,       kZero,
                              kZero, kZero, kListenerZ, kZero, kZero, kZero};
  for (uint32_t i = 0; i < 12; i++) {
    CopyFloat(base, listener + 4 * i, words[i]);
  }
  CopyFloat(base, listener + 48, kListener48);
  CopyFloat(base, listener + 52, kListener52);
}

void ListenerSetOrientation(uint8_t* base, uint32_t listener, uint32_t front, uint32_t top) {
  ITC_NATIVE_RUNNING("sub_8259D550 Listener::SetOrientation");
  CopyVector3(base, listener, front);
  CopyVector3(base, listener + 12, top);
}

void ListenerSetPosition(uint8_t* base, uint32_t listener, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8259D588 Listener::SetPosition");
  CopyVector3(base, listener + 24, position);
}

void SetVector36(uint8_t* base, uint32_t object, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_8259CC80 SetVector36");
  CopyVector3(base, object + 36, vector);
}

void ManagerSetListenerPosition(uint8_t* base, uint32_t manager, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82117E08 AudioManager::SetListenerPosition");
  ListenerSetPosition(base, manager + kListener, position);
}

void ManagerSetListenerOrientation(uint8_t* base, uint32_t manager, uint32_t front, uint32_t top) {
  ITC_NATIVE_RUNNING("sub_82117E10 AudioManager::SetListenerOrientation");
  ListenerSetOrientation(base, manager + kListener, front, top);
}

void ManagerSetListenerVelocity(uint8_t* base, uint32_t manager, uint32_t velocity) {
  ITC_NATIVE_RUNNING("sub_82117E18 AudioManager::SetListenerVelocity");
  SetVector36(base, manager + kListener, velocity);
}

uint32_t ManagerGetEngine(uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_82117EA8 AudioManager::GetEngine");
  return interface + 12;
}

void BankPairRelease(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117760 BankPair::Release");
  guest::StoreU8(base, pair + 69, 1);
}

uint32_t BankPairSoundBank(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117770 BankPair::SoundBank");
  return guest::LoadU32(base, pair + 76);
}

uint32_t BankPairIsLoading(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117670 BankPair::IsLoading");
  return guest::LoadU8(base, pair + 108) != 0 && guest::LoadU8(base, pair + 109) != 0 ? 0 : 1;
}

bool ManagerReleaseBank(uint8_t* base, uint32_t manager, uint32_t index_address, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82118190 AudioManager::ReleaseBank");
  uint32_t pair = 0;
  if (!BankSlot(base, manager + kBanks, index_address, &pair)) {
    return false;
  }
  *r3 = manager;
  if (pair != 0) {
    BankPairRelease(base, pair);
    *r3 = pair;
  }
  return true;
}

bool ManagerIsBankLoading(uint8_t* base, uint32_t manager, uint32_t index_address,
                          uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_82118200 AudioManager::IsBankLoading");
  uint32_t pair = 0;
  if (!BankSlot(base, manager + kBanks, index_address, &pair)) {
    return false;
  }
  *result = pair == 0 ? 0 : BankPairIsLoading(base, pair);
  return true;
}

bool ManagerIsBankSlotEmpty(uint8_t* base, uint32_t manager, uint32_t index_address,
                            uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_821182E0 AudioManager::IsBankSlotEmpty");
  uint32_t pair = 0;
  if (!BankSlot(base, manager + kBanks, index_address, &pair)) {
    return false;
  }
  *result = pair == 0 ? 1 : 0;
  return true;
}

bool ManagerGetSoundBank(uint8_t* base, uint32_t interface, uint32_t index_address,
                         uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_82118400 AudioManager::GetSoundBank");
  uint32_t pair = 0;
  if (!BankSlot(base, interface + kBanks - 8, index_address, &pair)) {
    return false;
  }
  *result = BankPairSoundBank(base, pair);
  return true;
}

void SoundStart(uint8_t* base, uint32_t sound) {
  ITC_NATIVE_RUNNING("sub_821173F0 Sound::Start");
  guest::StoreU8(base, sound + 11, 1);
}

void SoundSet3D(uint8_t* base, uint32_t sound, uint32_t value_address) {
  ITC_NATIVE_RUNNING("sub_82117568 Sound::Set3D");
  guest::StoreU8(base, sound + 8, guest::LoadU8(base, value_address));
}

uint32_t SoundSetPosition(uint8_t* base, uint32_t sound, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82117560 Sound::SetPosition");
  const uint32_t emitter = guest::LoadU32(base, sound + 4);
  SetVector36(base, emitter, position);
  return emitter;
}

uint32_t SoundIsBank(uint8_t* base, uint32_t sound, uint32_t index_address) {
  ITC_NATIVE_RUNNING("sub_821175B8 Sound::IsBank");
  return guest::LoadU32(base, sound + 16) == guest::LoadU32(base, index_address) ? 1 : 0;
}

uint32_t SoundIsCue(uint8_t* base, uint32_t sound, uint32_t index_address) {
  ITC_NATIVE_RUNNING("sub_82117578 Sound::IsCue");
  return guest::LoadU32(base, sound + 20) == guest::LoadU32(base, index_address) ? 1 : 0;
}

void EmitterConstruct(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8259C9B0 Emitter::Emitter");
  auto zero = [&](uint32_t offset) { CopyFloat(base, emitter + offset, kZero); };
  auto one = [&](uint32_t offset) { CopyFloat(base, emitter + offset, kOne); };
  guest::StoreU32(base, emitter + 0, 0);  // no cue
  // The X3DAudio emitter at +8: the cone's pointer, front (0, 0, 1), top
  // (0, 1, 0), position and velocity 0, one channel, radius 0, the
  // azimuths' pointer, no curves, the distance and Doppler scalers 1.
  guest::StoreU32(base, emitter + 8, emitter + 100);
  zero(12), zero(16), one(20);
  zero(24), one(28), zero(32);
  for (uint32_t offset = 36; offset < 60; offset += 4) {
    zero(offset);
  }
  guest::StoreU32(base, emitter + 60, 1);
  zero(64);
  guest::StoreU32(base, emitter + 68, emitter + 132);
  for (uint32_t offset = 72; offset <= 88; offset += 4) {
    guest::StoreU32(base, emitter + offset, 0);
  }
  one(92), one(96);
  // The cone (0, 0, 0, 1, 0, 1, 0, 1) and the one azimuth (0).
  for (uint32_t i = 0; i < 8; i++) {
    CopyFloat(base, emitter + 100 + 4 * i, i % 2 && i > 2 ? kOne : kZero);
  }
  zero(132);
  // The cue's 3D settings: six (index byte, 1.0) pairs from +164.
  for (uint32_t i = 0; i < 6; i++) {
    one(168 + 8 * i);
    guest::StoreU8(base, emitter + 164 + 8 * i, uint8_t(i));
  }
  one(224), one(152), one(156);
  guest::StoreU8(base, emitter + 284, 1);
  guest::StoreU8(base, emitter + 292, 1);
  guest::StoreU8(base, emitter + 220, 0);
  guest::StoreU32(base, emitter + 272, emitter + 220);
  guest::StoreU8(base, emitter + 268, 1);
  guest::StoreU8(base, emitter + 276, 0);
  guest::StoreU8(base, emitter + 212, 6);
  guest::StoreU32(base, emitter + 216, emitter + 164);
  guest::StoreU32(base, emitter + 280, emitter + 212);
  guest::StoreU32(base, emitter + 288, emitter + 268);
  guest::StoreU32(base, emitter + 296, emitter + 276);
}

void SoundBankHolderConstruct(uint8_t* base, uint32_t holder) {
  ITC_NATIVE_RUNNING("sub_8259D3D8 SoundBankHolder::SoundBankHolder");
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    guest::StoreU32(base, holder + offset, 0);
  }
}

void WaveBankHolderConstruct(uint8_t* base, uint32_t holder) {
  ITC_NATIVE_RUNNING("sub_8259D418 WaveBankHolder::WaveBankHolder");
  for (uint32_t offset = 0; offset < 12; offset += 4) {
    guest::StoreU32(base, holder + offset, 0);
  }
  guest::StoreU32(base, holder + 12, 0xFFFFFFFFu);
}

bool WaveBankHolderDestroy(uint8_t* base, uint32_t holder, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259D438 WaveBankHolder::~WaveBankHolder");
  const uint32_t handle = guest::LoadU32(base, holder + 12);
  if (handle != 0xFFFFFFFFu) {
    return false;
  }
  *r3 = handle;
  return true;
}

bool WaveBankHolderDelete(uint8_t* base, uint32_t slot, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82117AD8 WaveBankHolderDelete");
  const uint32_t holder = guest::LoadU32(base, slot);
  if (holder == 0) {
    *r3 = slot;
    return true;
  }
  uint32_t ignored = 0;
  if (!WaveBankHolderDestroy(base, holder, &ignored) || !memory::crt::OperatorDelete(holder)) {
    return false;
  }
  guest::StoreU32(base, slot, 0);
  *r3 = holder;
  return true;
}

bool SoundBankHolderDelete(uint8_t* base, uint32_t slot, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82117B30 SoundBankHolderDelete");
  const uint32_t holder = guest::LoadU32(base, slot);
  if (holder == 0) {
    *r3 = slot;
    return true;
  }
  if (!memory::crt::OperatorDelete(holder)) {
    return false;
  }
  guest::StoreU32(base, slot, 0);
  *r3 = holder;
  return true;
}

uint32_t ManagerIsCuePlaying(uint8_t* base, uint32_t manager, uint32_t bank_address,
                             uint32_t cue_address) {
  ITC_NATIVE_RUNNING("sub_82118B58 AudioManager::IsCuePlaying");
  const uint32_t end = guest::LoadU32(base, manager + 1528 + 4);
  for (uint32_t node = guest::LoadU32(base, end); node != end;
       node = guest::LoadU32(base, node)) {
    const uint32_t sound = guest::LoadU32(base, node + 8);
    if (SoundIsBank(base, sound, bank_address) != 0 && SoundIsCue(base, sound, cue_address) != 0) {
      return 1;
    }
  }
  return 0;
}

bool DelegateClone(uint8_t* base, uint32_t delegate, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_82118720 DelegateClone");
  const auto block = memory::crt::OperatorNew(base, 16);
  if (!block) {
    return false;
  }
  *copy = *block;
  if (*block != 0) {
    guest::StoreU32(base, *block + 0, 0x8200833Cu);
    for (uint32_t offset = 4; offset < 16; offset += 4) {
      guest::StoreU32(base, *block + offset, guest::LoadU32(base, delegate + offset));
    }
  }
  return true;
}

uint32_t X3DAudioInitialize(uint8_t* base, uint32_t speaker_mask, double speed_of_sound,
                            uint32_t handle) {
  ITC_NATIVE_RUNNING("sub_82480710 X3DAudioInitialize");
  uint32_t lfe = 0xFFFFFFFFu;
  if ((speaker_mask & 8) != 0) {
    lfe = (speaker_mask & 1) + ((speaker_mask >> 1) & 1) + ((speaker_mask >> 2) & 1);
  }
  guest::StoreU32(base, handle + 4, lfe);
  const uint32_t count = speaker_mask == 0 ? 1 : uint32_t(std::popcount(speaker_mask));
  const float speed = float(speed_of_sound);
  guest::StoreFloat(base, handle + 8, speed);
  guest::StoreU32(base, handle + 0, count);
  // The float whose bits are one less (lfs widens it; stfs narrows it back).
  const float below = std::bit_cast<float>(std::bit_cast<uint32_t>(speed) - 1);
  guest::StoreFloat(base, handle + 12, float(double(below)));
  return 0;
}

uint32_t AudioEngineStart3D(uint8_t* base, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_8259CE10 AudioEngine::Start3D");
  const uint32_t r3 = X3DAudioInitialize(
      base, 63, double(guest::LoadFloat(base, 0x8207145C)), engine + 12);
  guest::StoreU32(base, engine + 28, engine + 76);
  guest::StoreU32(base, engine + 36, 1);
  guest::StoreU32(base, engine + 40, 6);
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, engine + 76 + 4 * i, 0x3F800000u);
  }
  return r3;
}

std::optional<uint32_t> WordVectorAllocate(uint8_t* base, uint32_t vector, uint32_t count) {
  guest::StoreU32(base, vector + 4, 0);
  guest::StoreU32(base, vector + 8, 0);
  guest::StoreU32(base, vector + 12, 0);
  if (count == 0) {
    ITC_NATIVE_RUNNING("sub_821193C0 WordVector::Allocate");
    return 0;
  }
  if (count > 0x3FFFFFFF) {
    return std::nullopt;
  }
  const auto block = app_states::MessagesAllocate(base, count);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821193C0 WordVector::Allocate");
  guest::StoreU32(base, vector + 4, *block);
  guest::StoreU32(base, vector + 8, *block);
  guest::StoreU32(base, vector + 12, *block + 4 * count);
  return 1;
}

std::optional<uint32_t> WordVectorFill(uint8_t* base, uint32_t vector, uint32_t count,
                                       uint32_t value) {
  const auto r3 = WordVectorAllocate(base, vector, count);
  if (!r3) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821194A0 WordVector::Fill");
  if ((*r3 & 0xFF) != 0) {
    const uint32_t first = guest::LoadU32(base, vector + 4);
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, first + 4 * i, guest::LoadU32(base, value));
    }
    guest::StoreU32(base, vector + 8, first + 4 * count);
  }
  return r3;
}

}  // namespace sound_cues
