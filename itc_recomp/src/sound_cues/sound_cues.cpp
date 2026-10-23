#include "sound_cues/sound_cues.h"

#include "guest.h"

#include "base/log.h"

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

}  // namespace sound_cues
