// The mode manager's tick as plain C++ (mode_tick/modes_tick.h).

#include "mode_tick/modes_tick.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"
#include "schema/game_modes.h"
#include "schema/mode_tick.h"

#include <initializer_list>

namespace mode_tick {

namespace {

using G = schema::ModeTickGlobals;
constexpr uint32_t kMenuMusic = G::music_names_address();  // "BGM_SYSTEM"
constexpr uint32_t kSignInWatch = G::sign_in_watch_address();

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
  const schema::ModeHolder modes(base, holder);
  G::set_play_time_frames(base, G::play_time_frames(base) + 1);
  if (modes.mode<schema::View>().address == 0) {
    // The first mode: made by type, its settings, then its set-up (slot 0);
    // the layer's slot 1 asked twice.
    call(kGlobalMemoryStatus, {frame + 112});
    r3 = call(kModeCreate, {holder, G::mode_type(base)});
    modes.set_mode(uint32_t(r3));
    call(kSetChecks, {holder, G::mode_type(base)});
    call(kSetSceneTargetMode, {holder, G::mode_type(base)});
    r3 = call(kPresenceSetForMode, {G::mode_type(base)});
    const uint32_t mode = modes.mode<schema::View>().address;
    call(Slot(base, mode, 0), {mode});
    r3 = call(Slot(base, layer, 4), {layer});
    G::set_layer_answer_0(base, uint32_t(r3));
    r3 = call(Slot(base, layer, 4), {layer});
    G::set_layer_answer_1(base, uint32_t(r3));
  }
  // The menu music, once, on the first front-end pass.
  if (int32_t(G::music_started(base)) == 0 && int32_t(G::menu_mode(base)) == 1) {
    guest::StoreU8(base, frame + 80, 0);
    guest::StoreU32(base, frame + 88, 1);
    call(kMusicPlay, {frame + 88, kMenuMusic, frame + 80});
    G::set_music_started(base, 1);
  }
  call(kActionMappersUpdate, {G::mapper_clients(base)});
  G::set_network_counter(base, G::network_counter(base) - 1);
  call(kNetworkUpdate, {G::mode_type(base)});
  call(kSignInWatchUpdate, {kSignInWatch, modes.mode<schema::View>().address});
  call(kFlipRing, {});
  // The mode's delegate (+4, slot 0): an 8-byte answer, a byte (change the
  // mode) and the new type.
  const uint32_t delegate = guest::LoadU32(base, modes.mode<schema::View>().address + 4);
  call(Slot(base, delegate, 0), {frame + 104, delegate});
  guest::StoreU32(base, frame + 88, guest::LoadU32(base, frame + 104));
  guest::StoreU32(base, frame + 92, guest::LoadU32(base, frame + 108));
  r3 = call(kScreenFadesUpdate, {0});
  if (guest::LoadU8(base, frame + 88) != 0) {
    // The mode changes: the old one's slot 1 and slot 7 (deleted), the
    // next pass makes the new one.
    G::set_old_mode_type(base, G::mode_type(base));
    G::set_mode_type(base, guest::LoadU32(base, frame + 92));
    uint32_t mode = modes.mode<schema::View>().address;
    r3 = call(Slot(base, mode, 4), {mode});
    mode = modes.mode<schema::View>().address;
    r3 = mode;
    if (mode != 0) {
      r3 = call(Slot(base, mode, 28), {mode, 1});
    }
    modes.set_mode(0u);
    call(kOnModeChange, {kSignInWatch});
    call(kSetBlocked, {0});
    r3 = call(kGlobalMemoryStatus, {frame + 112});
  }
  // Whether the type is a front-end one (2, 4 to 10, 12); when that turns
  // off after the music started, the music player's slot 5 with 1.
  const int32_t type = int32_t(G::mode_type(base));
  const uint32_t menu = (type == 2 || (type >= 4 && type <= 10) || type == 12) ? 1 : 0;
  G::set_menu_mode(base, menu);
  if (int32_t(G::music_started(base)) == 1 && menu == 0) {
    guest::StoreU32(base, frame + 96, 1);
    const uint32_t player = G::music_player(base);
    r3 = call(Slot(base, player, 20), {player, frame + 96});
    G::set_music_started(base, 0);
  }
  return r3;
}

}  // namespace mode_tick
