// The car sounds natively (car_sounds.h, docs/functions/car-sounds.md).

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "models/memory_blocks.h"
#include "schema/car_sounds.h"
#include "sound_cues/sound_cues.h"

#include <array>
#include <bit>
#include <climits>
#include <cmath>

namespace car_sounds {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
double LoadF(uint32_t address) { return double(guest::LoadFloat(address)); }
void StoreF(uint32_t address, double value) {
  guest::StoreFloat(address, float(value));
}

using BankView = schema::CarSoundWaveBankView;
using Entry = schema::CarSoundWaveEntry;
using Sound = schema::CarSound;
using G = schema::CarSoundsGlobals;

// A wave's entry (24 bytes a wave from the view's entries).
Entry EntryAt(uint32_t entries, uint32_t wave) {
  return Entry(Entry::kSize * wave + entries);
}

bool PastCount(uint32_t view, uint32_t wave) {
  return wave >= BankView(view).bank_data().count();
}

// The ten parts' voice holders in CarSound::CarSound's order (the
// destructor walks them backward).
std::array<uint32_t, 10> HolderAddresses(uint32_t sound) {
  const Sound s(sound);
  return {schema::CarSoundHits(s.hits_address()).holder_address(),
          schema::CarSoundTires(s.tires_address()).holder_address(),
          schema::CarSoundGear(s.gear_address()).holder_address(),
          schema::CarSoundRoll(s.roll_address()).holder_address(),
          schema::CarSoundEvents(s.events_address()).holder_address(),
          schema::CarSoundFade(s.fade_address()).holder_address(),
          schema::CarSoundTurbo(s.turbo_address()).holder_address(),
          schema::CarSoundWind(s.wind_address()).holder_address(),
          schema::CarSoundHorn(s.horn_address()).holder_address(),
          schema::CarSoundPassBy(s.pass_by_address()).holder_address()};
}

// CarSounds::Destroy's vtables put back on one car sound: the holders from
// the last, then the pitched layer's and the layer's (each its own, then
// its holder's).
void RestoreVtables(uint32_t sound) {
  const auto holders = HolderAddresses(sound);
  for (auto it = holders.rbegin(); it != holders.rend(); ++it) {
    schema::CarSoundVoiceHolder(*it).set_vtable(kHolderVtable);
  }
  const schema::CarSoundEngine e(Sound(sound).engine_address());
  for (const uint32_t at : {e.pitched_layer_address(), e.layer_address()}) {
    const schema::CarSoundEngineLayer layer(at);
    layer.set_vtable(kLayerVtable);
    schema::CarSoundVoiceHolder(layer.holder_address()).set_vtable(kHolderVtable);
  }
}

// A voice holder's set-up as CarSound::CarSound and EngineSound's loops do
// it: the vtable, +20, then the voice words +4 to +16 and +24 to +36.
void ConstructHolder(uint32_t holder) {
  const schema::CarSoundVoiceHolder h(holder);
  h.set_vtable(kHolderVtable);
  h.set_voice_count(0);
  for (uint32_t i = 0; i < 4; i++) {
    h.store_at(4 + 4 * i, 4, 0);   // voices
    h.store_at(24 + 4 * i, 4, 0);  // paused
  }
}

// fctiwz as the translated code does it: NaN gives 0x80000000, at or above
// INT_MAX gives INT_MAX, else the truncation (cvttsd: 0x80000000 below
// INT_MIN).
int32_t ConvertToInt(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value <= double(INT_MIN) - 1.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

}  // namespace

uint32_t WaveBankViewInit(uint32_t view, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_8210C9D8 WaveBankView::Init");
  const BankView v(view);
  const schema::CarSoundWaveBank b(bank);
  v.set_bank(bank);
  v.set_bank_data(0);
  v.set_entries(0);
  v.set_wave_data(0);
  v.set_unknown_4(0);
  v.set_bank_copy(bank);
  v.set_bank_data(b.data_offset() + bank);
  v.set_entries(b.entries_offset() + bank);
  v.set_wave_data(b.wave_data_offset() + bank);
  return 0;
}

uint32_t WaveBankFormat(uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C850 WaveBankView::Format");
  const BankView v(view);
  const schema::CarSoundWaveFormat o(out);
  const auto data = v.bank_data();
  if (wave >= data.count()) {
    return 1;
  }
  // The compact bank's one format, or the wave's entry's.
  const bool compact = (data.flags() & 0x20000) != 0;
  const auto read = [&] {
    return compact ? v.bank_data().compact_format()
                   : EntryAt(v.entries(), wave).format();
  };
  const uint32_t format = compact ? data.compact_format() : EntryAt(v.entries(), wave).format();
  if ((format & 3) != 0) {
    return 1;
  }
  o.set_sample_type((format >> 31) != 0 ? 1 : 2);
  // Read again after the store, as the original does.
  const uint32_t again = read();
  o.set_rate((again >> 5) & 0x3FFFF);
  const uint32_t last = read();
  o.set_channels(uint8_t((last >> 2) & 7));
  return 0;
}

uint32_t WaveBankLength(uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C920 WaveBankView::Length");
  if (PastCount(view, wave)) {
    return 1;
  }
  schema::CarSoundWaveWord(out).set_value(EntryAt(BankView(view).entries(), wave).length());
  return 0;
}

uint32_t WaveBankData(uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C958 WaveBankView::Data");
  if (PastCount(view, wave)) {
    return 1;
  }
  const BankView v(view);
  const Entry entry = EntryAt(v.entries(), wave);
  const uint32_t start = v.wave_data();
  schema::CarSoundWaveWord(out).set_value(entry.data_offset() + start);
  return 0;
}

std::optional<uint32_t> WaveBankEntry(uint32_t view, uint32_t wave,
                                      uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C998 WaveBankView::Entry");
  const uint32_t entry = EntryAt(BankView(view).entries(), wave).address;
  if (!c_runtime::XMemCpy(out, entry, Entry::kSize)) {
    return std::nullopt;
  }
  return 0;
}

void SetBanks(uint32_t sound, uint32_t i, uint32_t j) {
  ITC_NATIVE_RUNNING("sub_8210CB48 CarSounds::SetBanks");
  const uint32_t parameters =
      Load(Load((i << 4) + G::sound_parameters_address()) + (j << 2));
  const uint32_t engine = parameters + 820;
  const Sound s(sound);
  const schema::CarSoundEngine e(s.engine_address());
  const schema::CarSoundEngineLayer pitched(e.pitched_layer_address());
  const schema::CarSoundEngineLayer layer(e.layer_address());
  using Table = schema::CarSoundEngineTable;
  pitched.set_table(parameters);
  pitched.set_table_volume(Table(parameters).volume_scale());
  pitched.set_table_copy(parameters);
  pitched.set_table_volume_copy(Table(parameters).volume_scale());
  layer.set_table(engine);
  layer.set_table_volume(Table(engine).volume_scale());
  layer.set_table_copy(engine);
  layer.set_table_volume_copy(Table(engine).volume_scale());
  s.set_live(1);
}

void TireSoundAmount(uint32_t part, double f1) {
  ITC_NATIVE_RUNNING("sub_8210CA28 TireSound::Amount");
  const schema::CarSoundTires t(part);
  const double limit = LoadF(0x82007FC8);
  double amount = double(t.slip_time());
  if (amount < LoadF(0x820067B0)) {
    amount = double(float(amount * LoadF(0x82002A60)));
    t.set_volume(float(amount));
    if (amount > limit) {
      amount = limit;
    }
  } else {
    amount = limit;
  }
  t.set_volume(float(amount));
  const double one = LoadF(0x82000A34);
  if (f1 == one) {
    t.set_volume(float(double(float(double(t.volume()) * LoadF(0x8200295C)))));
  }
  // The target: 8192 - 14 x +70 for small +70, else f1 scaled, negated
  // between `one` and [0x82007FEC] and below `one`.
  const uint16_t step = t.slip_steps();
  int32_t target;
  if (step < 7) {
    target = int16_t(uint32_t(8192) - uint32_t(int32_t(int16_t(step)) * 14));
  } else {
    const double scaled = double(float(f1 * LoadF(0x8200801C)));
    target = int16_t(uint32_t(ConvertToInt(scaled)) & 0xFFFF);
    bool negate;
    if (f1 > one) {
      negate = !(f1 > LoadF(0x82007FEC));
    } else {
      negate = f1 < one;
    }
    if (negate) {
      target = int16_t(uint32_t(-target));
    }
  }
  target = int16_t(target);
  if (target < -1170) {
    target = -1170;
  } else if (target > 1170) {
    target = 1170;
  }
  const uint16_t current = t.pitch();
  const int32_t difference = int32_t(int16_t(target)) - int32_t(int16_t(current));
  t.set_pitch(uint16_t(uint32_t(difference / 3) + current));
}

void EngineSoundConstruct(uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_8210CBA0 EngineSound::EngineSound");
  const schema::CarSoundEngine e(engine);
  for (const uint32_t at : {e.layer_address(), e.pitched_layer_address()}) {
    const schema::CarSoundEngineLayer layer(at);
    layer.set_vtable(kLayerVtable);
    ConstructHolder(layer.holder_address());
  }
}

uint32_t CarSoundConstruct(uint32_t sound) {
  ITC_NATIVE_RUNNING("sub_8210CC18 CarSound::CarSound");
  const Sound s(sound);
  EngineSoundConstruct(s.engine_address());
  for (const uint32_t holder : HolderAddresses(sound)) {
    ConstructHolder(holder);
  }
  return sound;
}

Destroyed CarSoundsDestroy(uint32_t sound, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8210DCA8 CarSounds::Destroy");
  if ((flags & 2) != 0) {
    // operator new[]'s count, 16 bytes before the first sound: the array's.
    const uint32_t block = Sound(sound).array_count_address();
    const uint32_t count = schema::CarSoundArray(block).count();
    // From the last element down; the loop's count is signed.
    uint32_t at = sound + count * Sound::kSize;
    for (int32_t left = int32_t(count - 1); left >= 0; left--) {
      at -= Sound::kSize;
      RestoreVtables(at);
    }
    return {block, (flags & 1) != 0 ? block : 0, true};
  }
  RestoreVtables(sound);
  return {sound, (flags & 1) != 0 ? sound : 0, false};
}

uint32_t SetUp(uint32_t sound, uint32_t matrix) {
  ITC_NATIVE_RUNNING("sub_8210DDC8 CarSound::SetUp");
  constexpr uint32_t kA = 0x82000A34;
  constexpr uint32_t kB = 0x82007FC8;
  const Sound s(sound);
  // The floats move as lfs and stfs move them, into the sound (in host
  // memory since E5 step 4: stored through its view at their offsets).
  const auto copy = [&](uint32_t to, uint32_t from) {
    s.store_at(to - sound, 4, std::bit_cast<uint32_t>(float(LoadF(from))));
  };
  const schema::X3DAudioListener l(s.listener_address());
  const schema::X3DAudioEmitter e(s.emitter_address());
  const schema::X3DAudioDspSettings dsp(s.dsp_address());
  const schema::X3DAudioCurve curve(s.volume_curve_address());
  // The volume curve's floats (six {distance, value} points).
  const auto curve_float = [&](uint32_t i) { return s.volume_curve_points_address() + 4 * i; };
  // The listener's front, top, position and velocity; the emitter's start.
  copy(l.velocity_x_address(), kA), copy(l.velocity_y_address(), kA);
  copy(l.front_x_address(), kA), copy(l.velocity_z_address(), kA);
  copy(l.front_y_address(), kA), copy(e.position_x_address(), kA);
  copy(l.front_z_address(), kB);
  copy(e.position_y_address(), kA), copy(l.top_x_address(), kA);
  e.set_cone(s.cone_address());
  copy(l.top_y_address(), kB);
  copy(e.position_z_address(), kA), copy(l.top_z_address(), kA);
  copy(e.velocity_x_address(), kA), copy(l.position_x_address(), kA);
  copy(e.velocity_y_address(), kA), copy(l.position_y_address(), kA);
  copy(e.velocity_z_address(), kA);
  copy(l.position_z_address(), 0x8200277C);
  copy(e.front_x_address(), kA), copy(e.front_y_address(), kA);
  copy(e.front_z_address(), kB);
  copy(e.top_x_address(), kA);
  copy(e.top_y_address(), kB);
  copy(e.top_z_address(), kA);
  // The cone at +144, through the emitter's pointer (+52).
  copy(schema::X3DAudioCone(s.cone_address()).inner_angle_address(), kA);
  copy(e.cone().outer_angle_address(), kA), copy(e.cone().inner_volume_address(), kA);
  copy(e.cone().outer_volume_address(), kB), copy(e.cone().inner_lpf_address(), kA);
  copy(e.cone().outer_lpf_address(), kB), copy(e.cone().inner_reverb_address(), kA);
  copy(e.cone().outer_reverb_address(), kB);
  copy(curve_float(1), kB);
  copy(curve_float(2), 0x82002208);
  copy(curve_float(3), kB);
  curve.set_count(6);
  copy(curve_float(4), 0x82002A10);
  e.set_channel_count(1);
  copy(curve_float(5), 0x82002C50);
  e.set_lfe_curve(0);
  copy(curve_float(6), 0x82007FD4);
  e.set_lpf_direct_curve(0);
  copy(curve_float(7), 0x82002208);
  e.set_lpf_reverb_curve(0);
  copy(curve_float(8), 0x82001070);
  e.set_reverb_curve(0);
  copy(curve_float(9), 0x82001FBC);
  e.set_channel_azimuths(G::channel_azimuths_address());
  copy(curve_float(10), kB);
  dsp.set_source_channels(1);
  copy(curve_float(11), kA);
  dsp.set_destination_channels(6);
  copy(e.channel_radius_address(), kA);
  curve.set_points(s.volume_curve_points_address());
  copy(e.curve_distance_scaler_address(), 0x82002A20);
  e.set_volume_curve(s.volume_curve_address());
  copy(e.doppler_scaler_address(), kB);
  copy(curve_float(0), kA);
  dsp.set_matrix(matrix);
  // The two vectors (x, y, z, w): +368 all [0x82000A34], +384 the same
  // with z [0x8200277C]; stvx stores at the address rounded down to 16.
  const uint32_t one = guest::LoadU32(kA);
  const uint32_t z = guest::LoadU32(0x8200277C);
  const uint32_t first = s.emitter_position_x_address() & ~15u;
  const uint32_t second = s.listener_position_x_address() & ~15u;
  for (uint32_t i = 0; i < 4; i++) {
    s.store_at(first - sound + 4 * i, 4, one);
  }
  for (uint32_t i = 0; i < 4; i++) {
    s.store_at(second - sound + 4 * i, 4, i == 2 ? z : one);
  }
  return matrix;
}

void SoundGlobalsInit(uint32_t music_player) {
  ITC_NATIVE_RUNNING("sub_82110258 SoundGlobals::Init");
  const auto volume = [&](uint32_t address) { StoreF(address, LoadF(0x82000A30)); };
  volume(G::music_volume_address());
  G::set_sound_manager(music_player);
  volume(G::volume_c34_address());
  G::set_music_byte(0);
  volume(G::effects_volume_address());
  volume(G::effects_volume_2_address());
  G::set_unknown_826C5C85(0);
  for (uint32_t i = 0; i < 7; i++) {
    Store(G::sound_picks_b_address() + 4 * i, 0);
  }
  for (uint32_t i = 0; i < 7; i++) {
    Store(G::sound_picks_a_address() + 4 * i, 0);
  }
  // Bank pair 2's (the race's effects) files' buffers, the original's
  // Memory::AllocPhysicalEx (512, 0, -1, 0, 4 and 3 MiB, 0, -1, 2048, 4):
  // memory blocks whose header and data are blobs in PC memory (E5 step 7).
  // BankPair::Load reads the sound bank (.xsb, used in place by XACT) and the
  // wave bank (its XMA data read by our FFmpeg through guest::Pointer) into
  // them; the renderer never reads them. Freed by sub_82110388's
  // Memory::FreeLater (FreeDue's XPhysicalFree releases the blobs).
  G::set_bank_pair_small_buffer(models::AllocPhysicalExBlob(512, "SoundBankFile"));
  G::set_bank_pair_buffer(models::AllocPhysicalExBlob(3145728, "CarSoundWaveBank"));
}

uint32_t SoundGlobalsInit3D() {
  return sound_cues::X3DAudioInitialize(63, LoadF(0x820080F8), G::x3daudio_address());
}

}  // namespace car_sounds
