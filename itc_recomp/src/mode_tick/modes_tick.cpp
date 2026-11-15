// The mode manager's tick as plain C++ (mode_tick/modes_tick.h).

#include "mode_tick/modes_tick.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

#include <initializer_list>

namespace mode_tick {

namespace {

constexpr uint32_t kPlayTimeFrames = 0x826C5B90;
constexpr uint32_t kMusicStarted = 0x826C5B94;  // the menu music asked for once
constexpr uint32_t kMenuMode = 0x826C5B98;      // whether the type is a front-end one
constexpr uint32_t kLayerAnswers = 0x826C5B60;  // the layer's slot 1, asked twice
constexpr uint32_t kSignInWatch = 0x826C5B9C;
constexpr uint32_t kModeType = 0x826032DC;
constexpr uint32_t kOldModeType = 0x826032E0;
constexpr uint32_t kMapperClients = 0x826BAB7C;
constexpr uint32_t kNetworkCounter = 0x8260185C;
constexpr uint32_t kMusicPlayer = 0x826C5C80;
constexpr uint32_t kMenuMusic = 0x82006A18;  // "BGM_SYSTEM"

constexpr uint32_t kGlobalMemoryStatus = 0x82294DA0;
constexpr uint32_t kModeCreate = 0x821077A8;
constexpr uint32_t kSetChecks = 0x82106868;
constexpr uint32_t kSetSceneTargetMode = 0x82106740;
constexpr uint32_t kPresenceSetForMode = 0x822934B8;
constexpr uint32_t kMusicPlay = 0x821063B0;
constexpr uint32_t kActionMappersUpdate = 0x8210B018;
constexpr uint32_t kNetworkUpdate = 0x8214E268;
constexpr uint32_t kSignInWatchUpdate = 0x8209EEC8;
constexpr uint32_t kFlipRing = 0x8209BD18;
constexpr uint32_t kScreenFadesUpdate = 0x8209D130;
constexpr uint32_t kOnModeChange = 0x8209E7E8;
constexpr uint32_t kSetBlocked = 0x8209F318;

uint32_t Slot(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset);
}

}  // namespace

uint64_t ModesTick(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t holder, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_82107910 Modes::Tick");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 208);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args) {
    return lifted_call::Call(base, function, frame, r13, args).r3;
  };
  uint64_t r3 = 0;
  guest::StoreU32(base, kPlayTimeFrames, guest::LoadU32(base, kPlayTimeFrames) + 1);
  if (guest::LoadU32(base, holder + 4) == 0) {
    // The first mode: made by type, its settings, then its set-up (slot 0);
    // the layer's slot 1 asked twice.
    call(kGlobalMemoryStatus, {frame + 112});
    r3 = call(kModeCreate, {holder, guest::LoadU32(base, kModeType)});
    guest::StoreU32(base, holder + 4, uint32_t(r3));
    call(kSetChecks, {holder, guest::LoadU32(base, kModeType)});
    call(kSetSceneTargetMode, {holder, guest::LoadU32(base, kModeType)});
    r3 = call(kPresenceSetForMode, {guest::LoadU32(base, kModeType)});
    const uint32_t mode = guest::LoadU32(base, holder + 4);
    call(Slot(base, mode, 0), {mode});
    r3 = call(Slot(base, layer, 4), {layer});
    guest::StoreU32(base, kLayerAnswers, uint32_t(r3));
    r3 = call(Slot(base, layer, 4), {layer});
    guest::StoreU32(base, kLayerAnswers + 4, uint32_t(r3));
  }
  // The menu music, once, on the first front-end pass.
  if (int32_t(guest::LoadU32(base, kMusicStarted)) == 0 &&
      int32_t(guest::LoadU32(base, kMenuMode)) == 1) {
    guest::StoreU8(base, frame + 80, 0);
    guest::StoreU32(base, frame + 88, 1);
    call(kMusicPlay, {frame + 88, kMenuMusic, frame + 80});
    guest::StoreU32(base, kMusicStarted, 1);
  }
  call(kActionMappersUpdate, {guest::LoadU32(base, kMapperClients)});
  guest::StoreU32(base, kNetworkCounter, guest::LoadU32(base, kNetworkCounter) - 1);
  call(kNetworkUpdate, {guest::LoadU32(base, kModeType)});
  call(kSignInWatchUpdate, {kSignInWatch, guest::LoadU32(base, holder + 4)});
  call(kFlipRing, {});
  // The mode's delegate (+4, slot 0): an 8-byte answer, a byte (change the
  // mode) and the new type.
  const uint32_t delegate = guest::LoadU32(base, guest::LoadU32(base, holder + 4) + 4);
  call(Slot(base, delegate, 0), {frame + 104, delegate});
  guest::StoreU32(base, frame + 88, guest::LoadU32(base, frame + 104));
  guest::StoreU32(base, frame + 92, guest::LoadU32(base, frame + 108));
  r3 = call(kScreenFadesUpdate, {0});
  if (guest::LoadU8(base, frame + 88) != 0) {
    // The mode changes: the old one's slot 1 and slot 7 (deleted), the
    // next pass makes the new one.
    guest::StoreU32(base, kOldModeType, guest::LoadU32(base, kModeType));
    guest::StoreU32(base, kModeType, guest::LoadU32(base, frame + 92));
    uint32_t mode = guest::LoadU32(base, holder + 4);
    r3 = call(Slot(base, mode, 4), {mode});
    mode = guest::LoadU32(base, holder + 4);
    r3 = mode;
    if (mode != 0) {
      r3 = call(Slot(base, mode, 28), {mode, 1});
    }
    guest::StoreU32(base, holder + 4, 0);
    call(kOnModeChange, {kSignInWatch});
    call(kSetBlocked, {0});
    r3 = call(kGlobalMemoryStatus, {frame + 112});
  }
  // Whether the type is a front-end one (2, 4 to 10, 12); when that turns
  // off after the music started, the music player's slot 5 with 1.
  const int32_t type = int32_t(guest::LoadU32(base, kModeType));
  const uint32_t menu = (type == 2 || (type >= 4 && type <= 10) || type == 12) ? 1 : 0;
  guest::StoreU32(base, kMenuMode, menu);
  if (int32_t(guest::LoadU32(base, kMusicStarted)) == 1 && menu == 0) {
    guest::StoreU32(base, frame + 96, 1);
    const uint32_t player = guest::LoadU32(base, kMusicPlayer);
    r3 = call(Slot(base, player, 20), {player, frame + 96});
    guest::StoreU32(base, kMusicStarted, 0);
  }
  return r3;
}

}  // namespace mode_tick
