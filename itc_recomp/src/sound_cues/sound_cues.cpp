#include "sound_cues/sound_cues.h"

#include "guest.h"

#include "app_states/containers.h"
#include "base/log.h"
#include "memory/c_runtime_heap.h"
#include "schema/car_sounds.h"
#include "schema/menu_sounds.h"
#include "schema/sound_cues.h"

#include <bit>

namespace sound_cues {

namespace {

using G = schema::SoundCuesGlobals;

constexpr uint32_t kZero = G::float_zero_address();  // the game's float 0.0
constexpr uint32_t kOne = G::float_one_address();    // the game's float 1.0

// The manager's listener (AudioManager's embed field, menu-sounds.toml).
uint32_t ManagerListener(uint8_t* base, uint32_t manager) {
  return schema::AudioManager(base, manager).listener().address;
}

// A float moved as lfs and stfs move it (single to double and back).
uint32_t FloatBits(float value) { return std::bit_cast<uint32_t>(float(double(value))); }

// The float at `from` (an image constant) stored at `to`, inside `object`'s
// view (a host object's member, or guest memory: store_at picks).
template <typename View>
void CopyFloat(uint8_t* base, const View& object, uint32_t to, uint32_t from) {
  object.store_at(to - object.address, 4, FloatBits(guest::LoadFloat(base, from)));
}

// Three floats from a vector given by address to `to` and the two after it,
// inside `object`'s view; each loaded then stored, as the original does.
template <typename View>
void CopyVector3(uint8_t* base, const View& object, uint32_t to, uint32_t from) {
  const schema::SoundVector3 v(base, from);
  object.store_at(to - object.address + 0, 4, FloatBits(v.x()));
  object.store_at(to - object.address + 4, 4, FloatBits(v.y()));
  object.store_at(to - object.address + 8, 4, FloatBits(v.z()));
}

// The bank slot's word for the index at `index_address`, from the
// manager's bank vector (begin, end); false where the original traps.
bool BankSlot(uint8_t* base, const schema::AudioManager& manager, uint32_t index_address,
              uint32_t* pair) {
  const uint32_t begin = manager.bank_pairs_first();
  const uint32_t index = schema::SoundCuesBankIndex(base, index_address).index();
  if (begin == 0) {
    return false;
  }
  const uint32_t count = uint32_t(int32_t(manager.bank_pairs_end() - begin) >> 2);
  if (index >= count) {
    return false;
  }
  *pair = schema::BankPairSlot(base, begin + 4 * index).pair().address;
  return true;
}

}  // namespace

void ListenerConstruct(uint8_t* base, uint32_t listener) {
  ITC_NATIVE_RUNNING("sub_8259D478 Listener::Listener");
  // Front (0, 0, 1), top (0, 1, 0), position (0, 0, z), velocity 0.
  const schema::SoundListener l(base, listener);
  const uint32_t words[12] = {kZero, kZero, kOne,  kZero, kOne,  kZero,
                              kZero, kZero, G::listener_start_z_address(),
                              kZero, kZero, kZero};
  // front, top, position and velocity: the first 12 floats.
  for (uint32_t i = 0; i < 12; i++) {
    CopyFloat(base, l, l.front_x_address() + 4 * i, words[i]);
  }
  CopyFloat(base, l, l.param_0_address(), G::listener_start_param_0_address());
  CopyFloat(base, l, l.param_1_address(), G::listener_start_param_1_address());
}

void ListenerSetOrientation(uint8_t* base, uint32_t listener, uint32_t front, uint32_t top) {
  ITC_NATIVE_RUNNING("sub_8259D550 Listener::SetOrientation");
  const schema::SoundListener l(base, listener);
  CopyVector3(base, l, l.front_x_address(), front);
  CopyVector3(base, l, l.top_x_address(), top);
}

void ListenerSetPosition(uint8_t* base, uint32_t listener, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8259D588 Listener::SetPosition");
  const schema::SoundListener l(base, listener);
  CopyVector3(base, l, l.position_x_address(), position);
}

void SetVector36(uint8_t* base, uint32_t object, uint32_t vector) {
  ITC_NATIVE_RUNNING("sub_8259CC80 SetVector36");
  const schema::SoundVector36Target t(base, object);
  CopyVector3(base, t, t.vector_x_address(), vector);
}

void ManagerSetListenerPosition(uint8_t* base, uint32_t manager, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82117E08 AudioManager::SetListenerPosition");
  ListenerSetPosition(base, ManagerListener(base, manager), position);
}

void ManagerSetListenerOrientation(uint8_t* base, uint32_t manager, uint32_t front, uint32_t top) {
  ITC_NATIVE_RUNNING("sub_82117E10 AudioManager::SetListenerOrientation");
  ListenerSetOrientation(base, ManagerListener(base, manager), front, top);
}

void ManagerSetListenerVelocity(uint8_t* base, uint32_t manager, uint32_t velocity) {
  ITC_NATIVE_RUNNING("sub_82117E18 AudioManager::SetListenerVelocity");
  SetVector36(base, ManagerListener(base, manager), velocity);
}

uint32_t ManagerGetEngine(uint32_t interface) {
  ITC_NATIVE_RUNNING("sub_82117EA8 AudioManager::GetEngine");
  return interface + 12;
}

void BankPairRelease(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117760 BankPair::Release");
  schema::SoundBankPair(base, pair).set_release_requested(1);
}

uint32_t BankPairSoundBank(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117770 BankPair::SoundBank");
  return schema::SoundBankPair(base, pair).sound_bank_holder().address;
}

uint32_t BankPairIsLoading(uint8_t* base, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82117670 BankPair::IsLoading");
  const schema::SoundBankPair p(base, pair);
  return p.wave_bank_loaded() != 0 && p.sound_bank_loaded() != 0 ? 0 : 1;
}

bool ManagerReleaseBank(uint8_t* base, uint32_t manager, uint32_t index_address, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_82118190 AudioManager::ReleaseBank");
  uint32_t pair = 0;
  if (!BankSlot(base, schema::AudioManager(base, manager), index_address, &pair)) {
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
  if (!BankSlot(base, schema::AudioManager(base, manager), index_address, &pair)) {
    return false;
  }
  *result = pair == 0 ? 0 : BankPairIsLoading(base, pair);
  return true;
}

bool ManagerIsBankSlotEmpty(uint8_t* base, uint32_t manager, uint32_t index_address,
                            uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_821182E0 AudioManager::IsBankSlotEmpty");
  uint32_t pair = 0;
  if (!BankSlot(base, schema::AudioManager(base, manager), index_address, &pair)) {
    return false;
  }
  *result = pair == 0 ? 1 : 0;
  return true;
}

bool ManagerGetSoundBank(uint8_t* base, uint32_t interface, uint32_t index_address,
                         uint32_t* result) {
  ITC_NATIVE_RUNNING("sub_82118400 AudioManager::GetSoundBank");
  uint32_t pair = 0;
  // The interface is the manager's +8.
  if (!BankSlot(base, schema::AudioManager(base, interface - 8), index_address, &pair)) {
    return false;
  }
  *result = BankPairSoundBank(base, pair);
  return true;
}

void SoundStart(uint8_t* base, uint32_t sound) {
  ITC_NATIVE_RUNNING("sub_821173F0 Sound::Start");
  schema::PlayingSound(base, sound).set_start_requested(1);
}

void SoundSet3D(uint8_t* base, uint32_t sound, uint32_t value_address) {
  ITC_NATIVE_RUNNING("sub_82117568 Sound::Set3D");
  schema::PlayingSound(base, sound).set_is_3d(schema::Sound3DArg(base, value_address).value());
}

uint32_t SoundSetPosition(uint8_t* base, uint32_t sound, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_82117560 Sound::SetPosition");
  const uint32_t emitter = schema::PlayingSound(base, sound).emitter().address;
  SetVector36(base, emitter, position);
  return emitter;
}

uint32_t SoundIsBank(uint8_t* base, uint32_t sound, uint32_t index_address) {
  ITC_NATIVE_RUNNING("sub_821175B8 Sound::IsBank");
  return schema::PlayingSound(base, sound).bank_index() ==
                 schema::SoundCuesBankIndex(base, index_address).index()
             ? 1
             : 0;
}

uint32_t SoundIsCue(uint8_t* base, uint32_t sound, uint32_t index_address) {
  ITC_NATIVE_RUNNING("sub_82117578 Sound::IsCue");
  return schema::PlayingSound(base, sound).cue_index() ==
                 schema::SoundCuesCueIndex(base, index_address).index()
             ? 1
             : 0;
}

void EmitterConstruct(uint8_t* base, uint32_t emitter) {
  ITC_NATIVE_RUNNING("sub_8259C9B0 Emitter::Emitter");
  const schema::SoundEmitter e(base, emitter);
  auto zero = [&](uint32_t address) { CopyFloat(base, e, address, kZero); };
  auto one = [&](uint32_t address) { CopyFloat(base, e, address, kOne); };
  e.set_cue(0);  // no cue
  // The X3DAudio emitter at +8: the cone's pointer, front (0, 0, 1), top
  // (0, 1, 0), position and velocity 0, one channel, radius 0, the
  // azimuths' pointer, no curves, the distance and Doppler scalers 1.
  e.set_cone(e.cone_inner_angle_address());
  zero(e.front_x_address()), zero(e.front_y_address()), one(e.front_z_address());
  zero(e.top_x_address()), one(e.top_y_address()), zero(e.top_z_address());
  // The position and velocity (six floats).
  for (uint32_t i = 0; i < 6; i++) {
    zero(e.position_x_address() + 4 * i);
  }
  e.set_channel_count(1);
  zero(e.channel_radius_address());
  e.set_channel_azimuths(e.azimuth_address());
  e.set_volume_curve(0);
  e.set_lfe_curve(0);
  e.set_lpf_direct_curve(0);
  e.set_lpf_reverb_curve(0);
  e.set_reverb_curve(0);
  one(e.curve_distance_scaler_address()), one(e.doppler_scaler_address());
  // The cone (0, 0, 0, 1, 0, 1, 0, 1) and the one azimuth (0).
  for (uint32_t i = 0; i < 8; i++) {
    CopyFloat(base, e, e.cone_inner_angle_address() + 4 * i, i % 2 && i > 2 ? kOne : kZero);
  }
  zero(e.azimuth_address());
  // The cue's 3D settings: six (index byte, 1.0) pairs from +164.
  for (uint32_t i = 0; i < 6; i++) {
    one(e.map_0_gain_address() + 8 * i);
    e.store_at(e.map_0_index_address() - e.address + 8 * i, 1, uint8_t(i));
  }
  one(e.single_map_gain_address()), one(e.unknown_152_address()), one(e.unknown_156_address());
  e.set_unknown_284(1);
  e.set_unknown_292(1);
  e.set_single_map_index(0);
  e.set_single_map(e.single_map_index_address());
  e.set_single_map_count(1);
  e.set_unknown_276(0);
  e.set_map_count(6);
  e.set_map(e.map_0_index_address());
  e.set_map_descriptor(e.map_count_address());
  e.set_single_map_descriptor(e.single_map_count_address());
  e.set_unknown_296(e.unknown_276_address());
}

void SoundBankHolderConstruct(uint8_t* base, uint32_t holder) {
  ITC_NATIVE_RUNNING("sub_8259D3D8 SoundBankHolder::SoundBankHolder");
  const schema::SoundBankHolder h(base, holder);
  h.set_buffer(0);
  h.set_size(0);
  h.set_sound_bank(0);
}

void WaveBankHolderConstruct(uint8_t* base, uint32_t holder) {
  ITC_NATIVE_RUNNING("sub_8259D418 WaveBankHolder::WaveBankHolder");
  const schema::WaveBankHolder h(base, holder);
  h.set_buffer(0);
  h.set_size(0);
  h.set_wave_bank(0);
  h.set_handle(0xFFFFFFFFu);
}

bool WaveBankHolderDestroy(uint8_t* base, uint32_t holder, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_8259D438 WaveBankHolder::~WaveBankHolder");
  const uint32_t handle = schema::WaveBankHolder(base, holder).handle();
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
  const uint32_t end = schema::AudioManager(base, manager).sounds_head();
  for (uint32_t node = schema::SoundListNode(base, end).next().address; node != end;
       node = schema::SoundListNode(base, node).next().address) {
    const uint32_t sound = schema::SoundListNode(base, node).sound().address;
    if (SoundIsBank(base, sound, bank_address) != 0 && SoundIsCue(base, sound, cue_address) != 0) {
      return 1;
    }
  }
  return 0;
}

bool DelegateClone(uint8_t* base, uint32_t delegate, uint32_t* copy) {
  ITC_NATIVE_RUNNING("sub_82118720 DelegateClone");
  // In host memory (E5 step 5): operator new's 16 bytes as a host object.
  const schema::AudioManagerDelegate to = schema::AudioManagerDelegate::New(base);
  *copy = to.address;
  {
    const schema::AudioManagerDelegate from(base, delegate);
    to.set_vtable(0x8200833Cu);
    to.set_object(from.object());
    to.set_method(from.method());
    to.set_method_adjust(from.method_adjust());
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
  const schema::X3DAudioHandle h(base, handle);
  h.set_lfe_speaker(lfe);
  const uint32_t count = speaker_mask == 0 ? 1 : uint32_t(std::popcount(speaker_mask));
  const float speed = float(speed_of_sound);
  h.set_speed_of_sound(speed);
  h.set_speaker_count(count);
  // The float whose bits are one less (lfs widens it; stfs narrows it back).
  const float below = std::bit_cast<float>(std::bit_cast<uint32_t>(speed) - 1);
  h.set_doppler_limit(float(double(below)));
  return 0;
}

uint32_t AudioEngineStart3D(uint8_t* base, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_8259CE10 AudioEngine::Start3D");
  const schema::AudioEngine e(base, engine);
  const uint32_t r3 = X3DAudioInitialize(base, 63, double(G::speed_of_sound(base)),
                                         e.x3d_speaker_count_address());
  e.set_dsp_matrix(e.matrix_0_address());
  e.set_dsp_source_channels(1);
  e.set_dsp_destination_channels(6);
  // The matrix: six 1.0.
  for (uint32_t i = 0; i < 6; i++) {
    e.store_at(e.matrix_0_address() - e.address + 4 * i, 4, 0x3F800000u);
  }
  return r3;
}

std::optional<uint32_t> WordVectorAllocate(uint8_t* base, uint32_t vector, uint32_t count) {
  const schema::WordVector v(base, vector);
  v.set_first(0);
  v.set_last(0);
  v.set_end(0);
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
  v.set_first(*block);
  v.set_last(*block);
  v.set_end(*block + 4 * count);
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
    const schema::WordVector v(base, vector);
    const uint32_t first = v.first();
    for (uint32_t i = 0; i < count; i++) {
      guest::StoreU32(base, first + 4 * i, schema::WordVectorFillValue(base, value).value());
    }
    v.set_last(first + 4 * count);
  }
  return r3;
}

}  // namespace sound_cues
