// The views of docs/functions/sound-cues.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/sound-cues.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Sound, Sound.+0
#pragma pack(push, 1)
struct PlayingSoundData {
  uint32_t owner = 0;  // +0
  uint32_t emitter = 0;  // +4
  uint32_t flags = 0;  // +8
  uint32_t handle = 0;  // +12
  uint32_t bank_index = 0;  // +16
  uint32_t cue_index = 0;  // +20
};
#pragma pack(pop)
static_assert(sizeof(PlayingSoundData) == 24);
inline constexpr memory::LayoutField kPlayingSoundFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}};
inline constexpr memory::Layout kPlayingSoundLayout = {kPlayingSoundFields, 6};

struct PlayingSound : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  PlayingSoundData* data() const { return static_cast<PlayingSoundData*>(memory::HandleObject(address)); }
  static PlayingSound New(uint8_t* base) {
    return PlayingSound(base, memory::MakeHandle(new PlayingSoundData(), sizeof(PlayingSoundData), "PlayingSound", &kPlayingSoundLayout));
  }
  template <uint32_t Stride = kSize>
  static PlayingSound NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(PlayingSoundData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<PlayingSoundData*>(::operator new(sizeof(PlayingSoundData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) PlayingSoundData();
    return PlayingSound(base, memory::MakeHandle(p, sizeof(PlayingSoundData) * count, "PlayingSound", &kPlayingSoundLayout, sizeof(PlayingSoundData)));
  }
  PlayingSound at(uint32_t i) const { return PlayingSound(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  PlayingSoundData* direct() const { return static_cast<PlayingSoundData*>(memory::HandleOwn(address, &kPlayingSoundLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kPlayingSoundLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the manager's +8 interface (its slot +4 GetSoundBank; +8 and +12 the 3D state and the listener)
  uint32_t owner_address() const { return address + 0; }
  uint32_t owner_bits() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_owner_bits(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t owner() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_owner(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // its emitter (300 bytes)
  uint32_t emitter_address() const { return address + 4; }
  uint32_t emitter_bits() const { if (auto* d = direct()) return d->emitter; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_emitter_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = SoundEmitter>
  T emitter() const { return T(base, emitter_bits()); }
  void set_emitter(const View& w) const { set_emitter_bits(w.address); }
  void set_emitter(uint32_t v) const { set_emitter_bits(v); }
  // bytes +8 to +11 as one word (Sound::Sound's clear)
  uint32_t flags_address() const { return address + 8; }
  uint32_t flags_bits() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_flags_bits(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t flags() const { if (auto* d = direct()) return d->flags; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_flags(uint32_t v) const { if (auto* d = direct()) { d->flags = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // 3D: X3DAudio's calculation each update (never set in play)
  uint32_t is_3d_address() const { return address + 8; }
  uint8_t is_3d_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 8, 1)); return uint8_t((memory::IsHandle(address + 8) ? uint8_t(memory::OwnerLoad(address + 8, 1)) : guest::LoadU8(base, address + 8))); }
  void set_is_3d_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 8, 1, v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 1, v); else guest::StoreU8(base, address + 8, v); }
  uint8_t is_3d() const { return uint8_t(is_3d_bits()); }
  void set_is_3d(uint8_t v) const { set_is_3d_bits(uint8_t(v)); }
  // played again once stopped
  uint32_t loop_address() const { return address + 9; }
  uint8_t loop_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 9, 1)); return uint8_t((memory::IsHandle(address + 9) ? uint8_t(memory::OwnerLoad(address + 9, 1)) : guest::LoadU8(base, address + 9))); }
  void set_loop_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 9, 1, v); return; } if (memory::IsHandle(address + 9)) memory::OwnerStore(address + 9, 1, v); else guest::StoreU8(base, address + 9, v); }
  uint8_t loop() const { return uint8_t(loop_bits()); }
  void set_loop(uint8_t v) const { set_loop_bits(uint8_t(v)); }
  uint32_t was_playing_address() const { return address + 10; }
  uint8_t was_playing_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 10, 1)); return uint8_t((memory::IsHandle(address + 10) ? uint8_t(memory::OwnerLoad(address + 10, 1)) : guest::LoadU8(base, address + 10))); }
  void set_was_playing_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 10, 1, v); return; } if (memory::IsHandle(address + 10)) memory::OwnerStore(address + 10, 1, v); else guest::StoreU8(base, address + 10, v); }
  uint8_t was_playing() const { return uint8_t(was_playing_bits()); }
  void set_was_playing(uint8_t v) const { set_was_playing_bits(uint8_t(v)); }
  // Sound::Start; cleared when Sound::Update plays the cue
  uint32_t start_requested_address() const { return address + 11; }
  uint8_t start_requested_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 11, 1)); return uint8_t((memory::IsHandle(address + 11) ? uint8_t(memory::OwnerLoad(address + 11, 1)) : guest::LoadU8(base, address + 11))); }
  void set_start_requested_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kPlayingSoundLayout, 11, 1, v); return; } if (memory::IsHandle(address + 11)) memory::OwnerStore(address + 11, 1, v); else guest::StoreU8(base, address + 11, v); }
  uint8_t start_requested() const { return uint8_t(start_requested_bits()); }
  void set_start_requested(uint8_t v) const { set_start_requested_bits(uint8_t(v)); }
  // Sound::SetHandle (only the handle list's Sounds, menu-sounds.md)
  uint32_t handle_address() const { return address + 12; }
  uint32_t handle_bits() const { if (auto* d = direct()) return d->handle; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_handle_bits(uint32_t v) const { if (auto* d = direct()) { d->handle = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t handle() const { if (auto* d = direct()) return d->handle; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_handle(uint32_t v) const { if (auto* d = direct()) { d->handle = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t bank_index_address() const { return address + 16; }
  uint32_t bank_index_bits() const { if (auto* d = direct()) return d->bank_index; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_bank_index_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_index = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t bank_index() const { if (auto* d = direct()) return d->bank_index; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_bank_index(uint32_t v) const { if (auto* d = direct()) { d->bank_index = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t cue_index_address() const { return address + 20; }
  uint32_t cue_index_bits() const { if (auto* d = direct()) return d->cue_index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_cue_index_bits(uint32_t v) const { if (auto* d = direct()) { d->cue_index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  uint32_t cue_index() const { if (auto* d = direct()) return d->cue_index; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_cue_index(uint32_t v) const { if (auto* d = direct()) { d->cue_index = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
};

// allocated; measure: Emitter, Emitter.+8
#pragma pack(push, 1)
struct SoundEmitterData {
  uint32_t cue = 0;  // +0
  float unknown_4 = 0;  // +4
  uint32_t cone = 0;  // +8
  float front_x = 0;  // +12
  float front_y = 0;  // +16
  float front_z = 0;  // +20
  float top_x = 0;  // +24
  float top_y = 0;  // +28
  float top_z = 0;  // +32
  float position_x = 0;  // +36
  float position_y = 0;  // +40
  float position_z = 0;  // +44
  float velocity_x = 0;  // +48
  float velocity_y = 0;  // +52
  float velocity_z = 0;  // +56
  uint32_t channel_count = 0;  // +60
  float channel_radius = 0;  // +64
  uint32_t channel_azimuths = 0;  // +68
  uint32_t volume_curve = 0;  // +72
  uint32_t lfe_curve = 0;  // +76
  uint32_t lpf_direct_curve = 0;  // +80
  uint32_t lpf_reverb_curve = 0;  // +84
  uint32_t reverb_curve = 0;  // +88
  float curve_distance_scaler = 0;  // +92
  float doppler_scaler = 0;  // +96
  float cone_inner_angle = 0;  // +100
  float cone_outer_angle = 0;  // +104
  float cone_inner_volume = 0;  // +108
  float cone_outer_volume = 0;  // +112
  float cone_inner_lpf = 0;  // +116
  float cone_outer_lpf = 0;  // +120
  float cone_inner_reverb = 0;  // +124
  float cone_outer_reverb = 0;  // +128
  float azimuth = 0;  // +132
  uint8_t cue_properties[14] = {};  // +136, guest order
  uint8_t pad_150[2] = {};
  float unknown_152 = 0;  // +152
  float unknown_156 = 0;  // +156
  uint8_t pad_160[4] = {};
  uint8_t map_0_index = 0;  // +164
  uint8_t pad_165[3] = {};
  float map_0_gain = 0;  // +168
  uint8_t map_1_index = 0;  // +172
  uint8_t pad_173[3] = {};
  float map_1_gain = 0;  // +176
  uint8_t map_2_index = 0;  // +180
  uint8_t pad_181[3] = {};
  float map_2_gain = 0;  // +184
  uint8_t map_3_index = 0;  // +188
  uint8_t pad_189[3] = {};
  float map_3_gain = 0;  // +192
  uint8_t map_4_index = 0;  // +196
  uint8_t pad_197[3] = {};
  float map_4_gain = 0;  // +200
  uint8_t map_5_index = 0;  // +204
  uint8_t pad_205[3] = {};
  float map_5_gain = 0;  // +208
  uint8_t map_count = 0;  // +212
  uint8_t pad_213[3] = {};
  uint32_t map = 0;  // +216
  uint8_t single_map_index = 0;  // +220
  uint8_t pad_221[3] = {};
  float single_map_gain = 0;  // +224
  uint8_t pad_228[40] = {};
  uint8_t single_map_count = 0;  // +268
  uint8_t pad_269[3] = {};
  uint32_t single_map = 0;  // +272
  uint8_t unknown_276 = 0;  // +276
  uint8_t pad_277[3] = {};
  uint32_t map_descriptor = 0;  // +280
  uint8_t unknown_284 = 0;  // +284
  uint8_t pad_285[3] = {};
  uint32_t single_map_descriptor = 0;  // +288
  uint8_t unknown_292 = 0;  // +292
  uint8_t pad_293[3] = {};
  uint32_t unknown_296 = 0;  // +296
};
#pragma pack(pop)
static_assert(sizeof(SoundEmitterData) == 300);
inline constexpr memory::LayoutField kSoundEmitterFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {152, 4}, {156, 4}, {168, 4}, {176, 4}, {184, 4}, {192, 4}, {200, 4}, {208, 4}, {216, 4}, {224, 4}, {272, 4}, {280, 4}, {288, 4}, {296, 4}};
inline constexpr memory::Layout kSoundEmitterLayout = {kSoundEmitterFields, 48};

struct SoundEmitter : View {
  using View::View;
  static constexpr uint32_t kSize = 300;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  SoundEmitterData* data() const { return static_cast<SoundEmitterData*>(memory::HandleObject(address)); }
  static SoundEmitter New(uint8_t* base) {
    return SoundEmitter(base, memory::MakeHandle(new SoundEmitterData(), sizeof(SoundEmitterData), "SoundEmitter", &kSoundEmitterLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundEmitter NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundEmitterData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundEmitterData*>(::operator new(sizeof(SoundEmitterData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundEmitterData();
    return SoundEmitter(base, memory::MakeHandle(p, sizeof(SoundEmitterData) * count, "SoundEmitter", &kSoundEmitterLayout, sizeof(SoundEmitterData)));
  }
  SoundEmitter at(uint32_t i) const { return SoundEmitter(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundEmitterData* direct() const { return static_cast<SoundEmitterData*>(memory::HandleOwn(address, &kSoundEmitterLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundEmitterLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundEmitterLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the XACT cue (0 none)
  uint32_t cue_address() const { return address + 0; }
  uint32_t cue_bits() const { if (auto* d = direct()) return d->cue; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_cue_bits(uint32_t v) const { if (auto* d = direct()) { d->cue = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  template <typename T = EmitterCue>
  T cue() const { return T(base, cue_bits()); }
  void set_cue(const View& w) const { set_cue_bits(w.address); }
  void set_cue(uint32_t v) const { set_cue_bits(v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_4_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_4); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_unknown_4_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_4 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  float unknown_4() const { return std::bit_cast<float>(unknown_4_bits()); }
  void set_unknown_4(float v) const { set_unknown_4_bits(std::bit_cast<uint32_t>(v)); }
  // the X3DAudio emitter's cone pointer: this emitter's +100
  uint32_t cone_address() const { return address + 8; }
  uint32_t cone_bits() const { if (auto* d = direct()) return d->cone; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_cone_bits(uint32_t v) const { if (auto* d = direct()) { d->cone = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t cone() const { if (auto* d = direct()) return d->cone; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_cone(uint32_t v) const { if (auto* d = direct()) { d->cone = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // front (0, 0, 1)
  uint32_t front_x_address() const { return address + 12; }
  uint32_t front_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_x); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_front_x_bits(uint32_t v) const { if (auto* d = direct()) { d->front_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  float front_x() const { return std::bit_cast<float>(front_x_bits()); }
  void set_front_x(float v) const { set_front_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_y_address() const { return address + 16; }
  uint32_t front_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_y); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_front_y_bits(uint32_t v) const { if (auto* d = direct()) { d->front_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  float front_y() const { return std::bit_cast<float>(front_y_bits()); }
  void set_front_y(float v) const { set_front_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_z_address() const { return address + 20; }
  uint32_t front_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_z); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_front_z_bits(uint32_t v) const { if (auto* d = direct()) { d->front_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  float front_z() const { return std::bit_cast<float>(front_z_bits()); }
  void set_front_z(float v) const { set_front_z_bits(std::bit_cast<uint32_t>(v)); }
  // top (0, 1, 0)
  uint32_t top_x_address() const { return address + 24; }
  uint32_t top_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_x); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_top_x_bits(uint32_t v) const { if (auto* d = direct()) { d->top_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  float top_x() const { return std::bit_cast<float>(top_x_bits()); }
  void set_top_x(float v) const { set_top_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_y_address() const { return address + 28; }
  uint32_t top_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_y); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_top_y_bits(uint32_t v) const { if (auto* d = direct()) { d->top_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  float top_y() const { return std::bit_cast<float>(top_y_bits()); }
  void set_top_y(float v) const { set_top_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_z_address() const { return address + 32; }
  uint32_t top_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_z); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_top_z_bits(uint32_t v) const { if (auto* d = direct()) { d->top_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  float top_z() const { return std::bit_cast<float>(top_z_bits()); }
  void set_top_z(float v) const { set_top_z_bits(std::bit_cast<uint32_t>(v)); }
  // Sound::SetPosition's (sub_8259CC80)
  uint32_t position_x_address() const { return address + 36; }
  uint32_t position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_x); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  float position_x() const { return std::bit_cast<float>(position_x_bits()); }
  void set_position_x(float v) const { set_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_y_address() const { return address + 40; }
  uint32_t position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_y); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  float position_y() const { return std::bit_cast<float>(position_y_bits()); }
  void set_position_y(float v) const { set_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_z_address() const { return address + 44; }
  uint32_t position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_z); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  float position_z() const { return std::bit_cast<float>(position_z_bits()); }
  void set_position_z(float v) const { set_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_x_address() const { return address + 48; }
  uint32_t velocity_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_x); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_velocity_x_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  float velocity_x() const { return std::bit_cast<float>(velocity_x_bits()); }
  void set_velocity_x(float v) const { set_velocity_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_y_address() const { return address + 52; }
  uint32_t velocity_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_y); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_velocity_y_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  float velocity_y() const { return std::bit_cast<float>(velocity_y_bits()); }
  void set_velocity_y(float v) const { set_velocity_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_z_address() const { return address + 56; }
  uint32_t velocity_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_z); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(base, address + 56))); }
  void set_velocity_z_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(base, address + 56, v); }
  float velocity_z() const { return std::bit_cast<float>(velocity_z_bits()); }
  void set_velocity_z(float v) const { set_velocity_z_bits(std::bit_cast<uint32_t>(v)); }
  // 1
  uint32_t channel_count_address() const { return address + 60; }
  uint32_t channel_count_bits() const { if (auto* d = direct()) return d->channel_count; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_channel_count_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_count = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  uint32_t channel_count() const { if (auto* d = direct()) return d->channel_count; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(base, address + 60))); }
  void set_channel_count(uint32_t v) const { if (auto* d = direct()) { d->channel_count = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(base, address + 60, v); }
  // 0
  uint32_t channel_radius_address() const { return address + 64; }
  uint32_t channel_radius_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->channel_radius); return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(base, address + 64))); }
  void set_channel_radius_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_radius = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(base, address + 64, v); }
  float channel_radius() const { return std::bit_cast<float>(channel_radius_bits()); }
  void set_channel_radius(float v) const { set_channel_radius_bits(std::bit_cast<uint32_t>(v)); }
  // this emitter's +132
  uint32_t channel_azimuths_address() const { return address + 68; }
  uint32_t channel_azimuths_bits() const { if (auto* d = direct()) return d->channel_azimuths; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(base, address + 68))); }
  void set_channel_azimuths_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_azimuths = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(base, address + 68, v); }
  uint32_t channel_azimuths() const { if (auto* d = direct()) return d->channel_azimuths; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(base, address + 68))); }
  void set_channel_azimuths(uint32_t v) const { if (auto* d = direct()) { d->channel_azimuths = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(base, address + 68, v); }
  // 0: the library's default
  uint32_t volume_curve_address() const { return address + 72; }
  uint32_t volume_curve_bits() const { if (auto* d = direct()) return d->volume_curve; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(base, address + 72))); }
  void set_volume_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->volume_curve = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(base, address + 72, v); }
  uint32_t volume_curve() const { if (auto* d = direct()) return d->volume_curve; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(base, address + 72))); }
  void set_volume_curve(uint32_t v) const { if (auto* d = direct()) { d->volume_curve = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(base, address + 72, v); }
  uint32_t lfe_curve_address() const { return address + 76; }
  uint32_t lfe_curve_bits() const { if (auto* d = direct()) return d->lfe_curve; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_lfe_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lfe_curve = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  uint32_t lfe_curve() const { if (auto* d = direct()) return d->lfe_curve; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_lfe_curve(uint32_t v) const { if (auto* d = direct()) { d->lfe_curve = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  uint32_t lpf_direct_curve_address() const { return address + 80; }
  uint32_t lpf_direct_curve_bits() const { if (auto* d = direct()) return d->lpf_direct_curve; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_lpf_direct_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_direct_curve = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t lpf_direct_curve() const { if (auto* d = direct()) return d->lpf_direct_curve; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_lpf_direct_curve(uint32_t v) const { if (auto* d = direct()) { d->lpf_direct_curve = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t lpf_reverb_curve_address() const { return address + 84; }
  uint32_t lpf_reverb_curve_bits() const { if (auto* d = direct()) return d->lpf_reverb_curve; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_lpf_reverb_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_reverb_curve = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t lpf_reverb_curve() const { if (auto* d = direct()) return d->lpf_reverb_curve; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_lpf_reverb_curve(uint32_t v) const { if (auto* d = direct()) { d->lpf_reverb_curve = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t reverb_curve_address() const { return address + 88; }
  uint32_t reverb_curve_bits() const { if (auto* d = direct()) return d->reverb_curve; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_reverb_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->reverb_curve = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t reverb_curve() const { if (auto* d = direct()) return d->reverb_curve; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_reverb_curve(uint32_t v) const { if (auto* d = direct()) { d->reverb_curve = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  // 1
  uint32_t curve_distance_scaler_address() const { return address + 92; }
  uint32_t curve_distance_scaler_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->curve_distance_scaler); return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_curve_distance_scaler_bits(uint32_t v) const { if (auto* d = direct()) { d->curve_distance_scaler = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  float curve_distance_scaler() const { return std::bit_cast<float>(curve_distance_scaler_bits()); }
  void set_curve_distance_scaler(float v) const { set_curve_distance_scaler_bits(std::bit_cast<uint32_t>(v)); }
  // 1
  uint32_t doppler_scaler_address() const { return address + 96; }
  uint32_t doppler_scaler_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->doppler_scaler); return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_doppler_scaler_bits(uint32_t v) const { if (auto* d = direct()) { d->doppler_scaler = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  float doppler_scaler() const { return std::bit_cast<float>(doppler_scaler_bits()); }
  void set_doppler_scaler(float v) const { set_doppler_scaler_bits(std::bit_cast<uint32_t>(v)); }
  // the cone (0, 0, 0, 1, 0, 1, 0, 1): probably no cone
  uint32_t cone_inner_angle_address() const { return address + 100; }
  uint32_t cone_inner_angle_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_inner_angle); return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_cone_inner_angle_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_inner_angle = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  float cone_inner_angle() const { return std::bit_cast<float>(cone_inner_angle_bits()); }
  void set_cone_inner_angle(float v) const { set_cone_inner_angle_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_outer_angle_address() const { return address + 104; }
  uint32_t cone_outer_angle_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_outer_angle); return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(base, address + 104))); }
  void set_cone_outer_angle_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_outer_angle = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(base, address + 104, v); }
  float cone_outer_angle() const { return std::bit_cast<float>(cone_outer_angle_bits()); }
  void set_cone_outer_angle(float v) const { set_cone_outer_angle_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_inner_volume_address() const { return address + 108; }
  uint32_t cone_inner_volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_inner_volume); return uint32_t((memory::IsHandle(address + 108) ? uint32_t(memory::OwnerLoad(address + 108, 4)) : guest::LoadU32(base, address + 108))); }
  void set_cone_inner_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_inner_volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 4, v); else guest::StoreU32(base, address + 108, v); }
  float cone_inner_volume() const { return std::bit_cast<float>(cone_inner_volume_bits()); }
  void set_cone_inner_volume(float v) const { set_cone_inner_volume_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_outer_volume_address() const { return address + 112; }
  uint32_t cone_outer_volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_outer_volume); return uint32_t((memory::IsHandle(address + 112) ? uint32_t(memory::OwnerLoad(address + 112, 4)) : guest::LoadU32(base, address + 112))); }
  void set_cone_outer_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_outer_volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 112)) memory::OwnerStore(address + 112, 4, v); else guest::StoreU32(base, address + 112, v); }
  float cone_outer_volume() const { return std::bit_cast<float>(cone_outer_volume_bits()); }
  void set_cone_outer_volume(float v) const { set_cone_outer_volume_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_inner_lpf_address() const { return address + 116; }
  uint32_t cone_inner_lpf_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_inner_lpf); return uint32_t((memory::IsHandle(address + 116) ? uint32_t(memory::OwnerLoad(address + 116, 4)) : guest::LoadU32(base, address + 116))); }
  void set_cone_inner_lpf_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_inner_lpf = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 116)) memory::OwnerStore(address + 116, 4, v); else guest::StoreU32(base, address + 116, v); }
  float cone_inner_lpf() const { return std::bit_cast<float>(cone_inner_lpf_bits()); }
  void set_cone_inner_lpf(float v) const { set_cone_inner_lpf_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_outer_lpf_address() const { return address + 120; }
  uint32_t cone_outer_lpf_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_outer_lpf); return uint32_t((memory::IsHandle(address + 120) ? uint32_t(memory::OwnerLoad(address + 120, 4)) : guest::LoadU32(base, address + 120))); }
  void set_cone_outer_lpf_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_outer_lpf = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 120)) memory::OwnerStore(address + 120, 4, v); else guest::StoreU32(base, address + 120, v); }
  float cone_outer_lpf() const { return std::bit_cast<float>(cone_outer_lpf_bits()); }
  void set_cone_outer_lpf(float v) const { set_cone_outer_lpf_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_inner_reverb_address() const { return address + 124; }
  uint32_t cone_inner_reverb_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_inner_reverb); return uint32_t((memory::IsHandle(address + 124) ? uint32_t(memory::OwnerLoad(address + 124, 4)) : guest::LoadU32(base, address + 124))); }
  void set_cone_inner_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_inner_reverb = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 124)) memory::OwnerStore(address + 124, 4, v); else guest::StoreU32(base, address + 124, v); }
  float cone_inner_reverb() const { return std::bit_cast<float>(cone_inner_reverb_bits()); }
  void set_cone_inner_reverb(float v) const { set_cone_inner_reverb_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_outer_reverb_address() const { return address + 128; }
  uint32_t cone_outer_reverb_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->cone_outer_reverb); return uint32_t((memory::IsHandle(address + 128) ? uint32_t(memory::OwnerLoad(address + 128, 4)) : guest::LoadU32(base, address + 128))); }
  void set_cone_outer_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->cone_outer_reverb = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 128)) memory::OwnerStore(address + 128, 4, v); else guest::StoreU32(base, address + 128, v); }
  float cone_outer_reverb() const { return std::bit_cast<float>(cone_outer_reverb_bits()); }
  void set_cone_outer_reverb(float v) const { set_cone_outer_reverb_bits(std::bit_cast<uint32_t>(v)); }
  // the one channel's azimuth (0)
  uint32_t azimuth_address() const { return address + 132; }
  uint32_t azimuth_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->azimuth); return uint32_t((memory::IsHandle(address + 132) ? uint32_t(memory::OwnerLoad(address + 132, 4)) : guest::LoadU32(base, address + 132))); }
  void set_azimuth_bits(uint32_t v) const { if (auto* d = direct()) { d->azimuth = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 132)) memory::OwnerStore(address + 132, 4, v); else guest::StoreU32(base, address + 132, v); }
  float azimuth() const { return std::bit_cast<float>(azimuth_bits()); }
  void set_azimuth(float v) const { set_azimuth_bits(std::bit_cast<uint32_t>(v)); }
  // the cue's GetProperties (xact.md's sub_825C5790): category, track count, priority, the first wave's index, a duration, the channel count
  uint32_t cue_properties_address() const { return address + 136; }
  uint8_t* cue_properties() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 136 : guest::Pointer(base, address + 136); }
  // 1.0 (not read)
  uint32_t unknown_152_address() const { return address + 152; }
  uint32_t unknown_152_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_152); return uint32_t((memory::IsHandle(address + 152) ? uint32_t(memory::OwnerLoad(address + 152, 4)) : guest::LoadU32(base, address + 152))); }
  void set_unknown_152_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_152 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 152)) memory::OwnerStore(address + 152, 4, v); else guest::StoreU32(base, address + 152, v); }
  float unknown_152() const { return std::bit_cast<float>(unknown_152_bits()); }
  void set_unknown_152(float v) const { set_unknown_152_bits(std::bit_cast<uint32_t>(v)); }
  // 1.0 (not read)
  uint32_t unknown_156_address() const { return address + 156; }
  uint32_t unknown_156_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->unknown_156); return uint32_t((memory::IsHandle(address + 156) ? uint32_t(memory::OwnerLoad(address + 156, 4)) : guest::LoadU32(base, address + 156))); }
  void set_unknown_156_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_156 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 156)) memory::OwnerStore(address + 156, 4, v); else guest::StoreU32(base, address + 156, v); }
  float unknown_156() const { return std::bit_cast<float>(unknown_156_bits()); }
  void set_unknown_156(float v) const { set_unknown_156_bits(std::bit_cast<uint32_t>(v)); }
  // six (index byte, 1.0) pairs from +164 (not read: probably an output map)
  uint32_t map_0_index_address() const { return address + 164; }
  uint8_t map_0_index_bits() const { if (auto* d = direct()) return d->map_0_index; return uint8_t((memory::IsHandle(address + 164) ? uint8_t(memory::OwnerLoad(address + 164, 1)) : guest::LoadU8(base, address + 164))); }
  void set_map_0_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_0_index = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 1, v); else guest::StoreU8(base, address + 164, v); }
  uint8_t map_0_index() const { if (auto* d = direct()) return d->map_0_index; return uint8_t((memory::IsHandle(address + 164) ? uint8_t(memory::OwnerLoad(address + 164, 1)) : guest::LoadU8(base, address + 164))); }
  void set_map_0_index(uint8_t v) const { if (auto* d = direct()) { d->map_0_index = v; return; } if (memory::IsHandle(address + 164)) memory::OwnerStore(address + 164, 1, v); else guest::StoreU8(base, address + 164, v); }
  uint32_t map_0_gain_address() const { return address + 168; }
  uint32_t map_0_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_0_gain); return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(base, address + 168))); }
  void set_map_0_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_0_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(base, address + 168, v); }
  float map_0_gain() const { return std::bit_cast<float>(map_0_gain_bits()); }
  void set_map_0_gain(float v) const { set_map_0_gain_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t map_1_index_address() const { return address + 172; }
  uint8_t map_1_index_bits() const { if (auto* d = direct()) return d->map_1_index; return uint8_t((memory::IsHandle(address + 172) ? uint8_t(memory::OwnerLoad(address + 172, 1)) : guest::LoadU8(base, address + 172))); }
  void set_map_1_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_1_index = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 1, v); else guest::StoreU8(base, address + 172, v); }
  uint8_t map_1_index() const { if (auto* d = direct()) return d->map_1_index; return uint8_t((memory::IsHandle(address + 172) ? uint8_t(memory::OwnerLoad(address + 172, 1)) : guest::LoadU8(base, address + 172))); }
  void set_map_1_index(uint8_t v) const { if (auto* d = direct()) { d->map_1_index = v; return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 1, v); else guest::StoreU8(base, address + 172, v); }
  uint32_t map_1_gain_address() const { return address + 176; }
  uint32_t map_1_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_1_gain); return uint32_t((memory::IsHandle(address + 176) ? uint32_t(memory::OwnerLoad(address + 176, 4)) : guest::LoadU32(base, address + 176))); }
  void set_map_1_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_1_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 176)) memory::OwnerStore(address + 176, 4, v); else guest::StoreU32(base, address + 176, v); }
  float map_1_gain() const { return std::bit_cast<float>(map_1_gain_bits()); }
  void set_map_1_gain(float v) const { set_map_1_gain_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t map_2_index_address() const { return address + 180; }
  uint8_t map_2_index_bits() const { if (auto* d = direct()) return d->map_2_index; return uint8_t((memory::IsHandle(address + 180) ? uint8_t(memory::OwnerLoad(address + 180, 1)) : guest::LoadU8(base, address + 180))); }
  void set_map_2_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_2_index = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 1, v); else guest::StoreU8(base, address + 180, v); }
  uint8_t map_2_index() const { if (auto* d = direct()) return d->map_2_index; return uint8_t((memory::IsHandle(address + 180) ? uint8_t(memory::OwnerLoad(address + 180, 1)) : guest::LoadU8(base, address + 180))); }
  void set_map_2_index(uint8_t v) const { if (auto* d = direct()) { d->map_2_index = v; return; } if (memory::IsHandle(address + 180)) memory::OwnerStore(address + 180, 1, v); else guest::StoreU8(base, address + 180, v); }
  uint32_t map_2_gain_address() const { return address + 184; }
  uint32_t map_2_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_2_gain); return uint32_t((memory::IsHandle(address + 184) ? uint32_t(memory::OwnerLoad(address + 184, 4)) : guest::LoadU32(base, address + 184))); }
  void set_map_2_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_2_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 184)) memory::OwnerStore(address + 184, 4, v); else guest::StoreU32(base, address + 184, v); }
  float map_2_gain() const { return std::bit_cast<float>(map_2_gain_bits()); }
  void set_map_2_gain(float v) const { set_map_2_gain_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t map_3_index_address() const { return address + 188; }
  uint8_t map_3_index_bits() const { if (auto* d = direct()) return d->map_3_index; return uint8_t((memory::IsHandle(address + 188) ? uint8_t(memory::OwnerLoad(address + 188, 1)) : guest::LoadU8(base, address + 188))); }
  void set_map_3_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_3_index = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 1, v); else guest::StoreU8(base, address + 188, v); }
  uint8_t map_3_index() const { if (auto* d = direct()) return d->map_3_index; return uint8_t((memory::IsHandle(address + 188) ? uint8_t(memory::OwnerLoad(address + 188, 1)) : guest::LoadU8(base, address + 188))); }
  void set_map_3_index(uint8_t v) const { if (auto* d = direct()) { d->map_3_index = v; return; } if (memory::IsHandle(address + 188)) memory::OwnerStore(address + 188, 1, v); else guest::StoreU8(base, address + 188, v); }
  uint32_t map_3_gain_address() const { return address + 192; }
  uint32_t map_3_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_3_gain); return uint32_t((memory::IsHandle(address + 192) ? uint32_t(memory::OwnerLoad(address + 192, 4)) : guest::LoadU32(base, address + 192))); }
  void set_map_3_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_3_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 192)) memory::OwnerStore(address + 192, 4, v); else guest::StoreU32(base, address + 192, v); }
  float map_3_gain() const { return std::bit_cast<float>(map_3_gain_bits()); }
  void set_map_3_gain(float v) const { set_map_3_gain_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t map_4_index_address() const { return address + 196; }
  uint8_t map_4_index_bits() const { if (auto* d = direct()) return d->map_4_index; return uint8_t((memory::IsHandle(address + 196) ? uint8_t(memory::OwnerLoad(address + 196, 1)) : guest::LoadU8(base, address + 196))); }
  void set_map_4_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_4_index = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 1, v); else guest::StoreU8(base, address + 196, v); }
  uint8_t map_4_index() const { if (auto* d = direct()) return d->map_4_index; return uint8_t((memory::IsHandle(address + 196) ? uint8_t(memory::OwnerLoad(address + 196, 1)) : guest::LoadU8(base, address + 196))); }
  void set_map_4_index(uint8_t v) const { if (auto* d = direct()) { d->map_4_index = v; return; } if (memory::IsHandle(address + 196)) memory::OwnerStore(address + 196, 1, v); else guest::StoreU8(base, address + 196, v); }
  uint32_t map_4_gain_address() const { return address + 200; }
  uint32_t map_4_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_4_gain); return uint32_t((memory::IsHandle(address + 200) ? uint32_t(memory::OwnerLoad(address + 200, 4)) : guest::LoadU32(base, address + 200))); }
  void set_map_4_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_4_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 200)) memory::OwnerStore(address + 200, 4, v); else guest::StoreU32(base, address + 200, v); }
  float map_4_gain() const { return std::bit_cast<float>(map_4_gain_bits()); }
  void set_map_4_gain(float v) const { set_map_4_gain_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t map_5_index_address() const { return address + 204; }
  uint8_t map_5_index_bits() const { if (auto* d = direct()) return d->map_5_index; return uint8_t((memory::IsHandle(address + 204) ? uint8_t(memory::OwnerLoad(address + 204, 1)) : guest::LoadU8(base, address + 204))); }
  void set_map_5_index_bits(uint8_t v) const { if (auto* d = direct()) { d->map_5_index = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 1, v); else guest::StoreU8(base, address + 204, v); }
  uint8_t map_5_index() const { if (auto* d = direct()) return d->map_5_index; return uint8_t((memory::IsHandle(address + 204) ? uint8_t(memory::OwnerLoad(address + 204, 1)) : guest::LoadU8(base, address + 204))); }
  void set_map_5_index(uint8_t v) const { if (auto* d = direct()) { d->map_5_index = v; return; } if (memory::IsHandle(address + 204)) memory::OwnerStore(address + 204, 1, v); else guest::StoreU8(base, address + 204, v); }
  uint32_t map_5_gain_address() const { return address + 208; }
  uint32_t map_5_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->map_5_gain); return uint32_t((memory::IsHandle(address + 208) ? uint32_t(memory::OwnerLoad(address + 208, 4)) : guest::LoadU32(base, address + 208))); }
  void set_map_5_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->map_5_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 208)) memory::OwnerStore(address + 208, 4, v); else guest::StoreU32(base, address + 208, v); }
  float map_5_gain() const { return std::bit_cast<float>(map_5_gain_bits()); }
  void set_map_5_gain(float v) const { set_map_5_gain_bits(std::bit_cast<uint32_t>(v)); }
  // 6: the map's count
  uint32_t map_count_address() const { return address + 212; }
  uint8_t map_count_bits() const { if (auto* d = direct()) return d->map_count; return uint8_t((memory::IsHandle(address + 212) ? uint8_t(memory::OwnerLoad(address + 212, 1)) : guest::LoadU8(base, address + 212))); }
  void set_map_count_bits(uint8_t v) const { if (auto* d = direct()) { d->map_count = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 1, v); else guest::StoreU8(base, address + 212, v); }
  uint8_t map_count() const { if (auto* d = direct()) return d->map_count; return uint8_t((memory::IsHandle(address + 212) ? uint8_t(memory::OwnerLoad(address + 212, 1)) : guest::LoadU8(base, address + 212))); }
  void set_map_count(uint8_t v) const { if (auto* d = direct()) { d->map_count = v; return; } if (memory::IsHandle(address + 212)) memory::OwnerStore(address + 212, 1, v); else guest::StoreU8(base, address + 212, v); }
  // this emitter's +164: the map's pairs
  uint32_t map_address() const { return address + 216; }
  uint32_t map_bits() const { if (auto* d = direct()) return d->map; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_map_bits(uint32_t v) const { if (auto* d = direct()) { d->map = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
  uint32_t map() const { if (auto* d = direct()) return d->map; return uint32_t((memory::IsHandle(address + 216) ? uint32_t(memory::OwnerLoad(address + 216, 4)) : guest::LoadU32(base, address + 216))); }
  void set_map(uint32_t v) const { if (auto* d = direct()) { d->map = v; return; } if (memory::IsHandle(address + 216)) memory::OwnerStore(address + 216, 4, v); else guest::StoreU32(base, address + 216, v); }
  // a one-pair map: index 0 (not read)
  uint32_t single_map_index_address() const { return address + 220; }
  uint8_t single_map_index_bits() const { if (auto* d = direct()) return d->single_map_index; return uint8_t((memory::IsHandle(address + 220) ? uint8_t(memory::OwnerLoad(address + 220, 1)) : guest::LoadU8(base, address + 220))); }
  void set_single_map_index_bits(uint8_t v) const { if (auto* d = direct()) { d->single_map_index = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 1, v); else guest::StoreU8(base, address + 220, v); }
  uint8_t single_map_index() const { if (auto* d = direct()) return d->single_map_index; return uint8_t((memory::IsHandle(address + 220) ? uint8_t(memory::OwnerLoad(address + 220, 1)) : guest::LoadU8(base, address + 220))); }
  void set_single_map_index(uint8_t v) const { if (auto* d = direct()) { d->single_map_index = v; return; } if (memory::IsHandle(address + 220)) memory::OwnerStore(address + 220, 1, v); else guest::StoreU8(base, address + 220, v); }
  // 1.0
  uint32_t single_map_gain_address() const { return address + 224; }
  uint32_t single_map_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->single_map_gain); return uint32_t((memory::IsHandle(address + 224) ? uint32_t(memory::OwnerLoad(address + 224, 4)) : guest::LoadU32(base, address + 224))); }
  void set_single_map_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->single_map_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 224)) memory::OwnerStore(address + 224, 4, v); else guest::StoreU32(base, address + 224, v); }
  float single_map_gain() const { return std::bit_cast<float>(single_map_gain_bits()); }
  void set_single_map_gain(float v) const { set_single_map_gain_bits(std::bit_cast<uint32_t>(v)); }
  // 1
  uint32_t single_map_count_address() const { return address + 268; }
  uint8_t single_map_count_bits() const { if (auto* d = direct()) return d->single_map_count; return uint8_t((memory::IsHandle(address + 268) ? uint8_t(memory::OwnerLoad(address + 268, 1)) : guest::LoadU8(base, address + 268))); }
  void set_single_map_count_bits(uint8_t v) const { if (auto* d = direct()) { d->single_map_count = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 1, v); else guest::StoreU8(base, address + 268, v); }
  uint8_t single_map_count() const { if (auto* d = direct()) return d->single_map_count; return uint8_t((memory::IsHandle(address + 268) ? uint8_t(memory::OwnerLoad(address + 268, 1)) : guest::LoadU8(base, address + 268))); }
  void set_single_map_count(uint8_t v) const { if (auto* d = direct()) { d->single_map_count = v; return; } if (memory::IsHandle(address + 268)) memory::OwnerStore(address + 268, 1, v); else guest::StoreU8(base, address + 268, v); }
  // this emitter's +220
  uint32_t single_map_address() const { return address + 272; }
  uint32_t single_map_bits() const { if (auto* d = direct()) return d->single_map; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(base, address + 272))); }
  void set_single_map_bits(uint32_t v) const { if (auto* d = direct()) { d->single_map = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(base, address + 272, v); }
  uint32_t single_map() const { if (auto* d = direct()) return d->single_map; return uint32_t((memory::IsHandle(address + 272) ? uint32_t(memory::OwnerLoad(address + 272, 4)) : guest::LoadU32(base, address + 272))); }
  void set_single_map(uint32_t v) const { if (auto* d = direct()) { d->single_map = v; return; } if (memory::IsHandle(address + 272)) memory::OwnerStore(address + 272, 4, v); else guest::StoreU32(base, address + 272, v); }
  // 0 (not read)
  uint32_t unknown_276_address() const { return address + 276; }
  uint8_t unknown_276_bits() const { if (auto* d = direct()) return d->unknown_276; return uint8_t((memory::IsHandle(address + 276) ? uint8_t(memory::OwnerLoad(address + 276, 1)) : guest::LoadU8(base, address + 276))); }
  void set_unknown_276_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_276 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 1, v); else guest::StoreU8(base, address + 276, v); }
  uint8_t unknown_276() const { if (auto* d = direct()) return d->unknown_276; return uint8_t((memory::IsHandle(address + 276) ? uint8_t(memory::OwnerLoad(address + 276, 1)) : guest::LoadU8(base, address + 276))); }
  void set_unknown_276(uint8_t v) const { if (auto* d = direct()) { d->unknown_276 = v; return; } if (memory::IsHandle(address + 276)) memory::OwnerStore(address + 276, 1, v); else guest::StoreU8(base, address + 276, v); }
  // this emitter's +212 (not read)
  uint32_t map_descriptor_address() const { return address + 280; }
  uint32_t map_descriptor_bits() const { if (auto* d = direct()) return d->map_descriptor; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(base, address + 280))); }
  void set_map_descriptor_bits(uint32_t v) const { if (auto* d = direct()) { d->map_descriptor = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(base, address + 280, v); }
  uint32_t map_descriptor() const { if (auto* d = direct()) return d->map_descriptor; return uint32_t((memory::IsHandle(address + 280) ? uint32_t(memory::OwnerLoad(address + 280, 4)) : guest::LoadU32(base, address + 280))); }
  void set_map_descriptor(uint32_t v) const { if (auto* d = direct()) { d->map_descriptor = v; return; } if (memory::IsHandle(address + 280)) memory::OwnerStore(address + 280, 4, v); else guest::StoreU32(base, address + 280, v); }
  // 1 (not read)
  uint32_t unknown_284_address() const { return address + 284; }
  uint8_t unknown_284_bits() const { if (auto* d = direct()) return d->unknown_284; return uint8_t((memory::IsHandle(address + 284) ? uint8_t(memory::OwnerLoad(address + 284, 1)) : guest::LoadU8(base, address + 284))); }
  void set_unknown_284_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_284 = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 1, v); else guest::StoreU8(base, address + 284, v); }
  uint8_t unknown_284() const { if (auto* d = direct()) return d->unknown_284; return uint8_t((memory::IsHandle(address + 284) ? uint8_t(memory::OwnerLoad(address + 284, 1)) : guest::LoadU8(base, address + 284))); }
  void set_unknown_284(uint8_t v) const { if (auto* d = direct()) { d->unknown_284 = v; return; } if (memory::IsHandle(address + 284)) memory::OwnerStore(address + 284, 1, v); else guest::StoreU8(base, address + 284, v); }
  // this emitter's +268 (not read)
  uint32_t single_map_descriptor_address() const { return address + 288; }
  uint32_t single_map_descriptor_bits() const { if (auto* d = direct()) return d->single_map_descriptor; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(base, address + 288))); }
  void set_single_map_descriptor_bits(uint32_t v) const { if (auto* d = direct()) { d->single_map_descriptor = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(base, address + 288, v); }
  uint32_t single_map_descriptor() const { if (auto* d = direct()) return d->single_map_descriptor; return uint32_t((memory::IsHandle(address + 288) ? uint32_t(memory::OwnerLoad(address + 288, 4)) : guest::LoadU32(base, address + 288))); }
  void set_single_map_descriptor(uint32_t v) const { if (auto* d = direct()) { d->single_map_descriptor = v; return; } if (memory::IsHandle(address + 288)) memory::OwnerStore(address + 288, 4, v); else guest::StoreU32(base, address + 288, v); }
  // 1 (not read)
  uint32_t unknown_292_address() const { return address + 292; }
  uint8_t unknown_292_bits() const { if (auto* d = direct()) return d->unknown_292; return uint8_t((memory::IsHandle(address + 292) ? uint8_t(memory::OwnerLoad(address + 292, 1)) : guest::LoadU8(base, address + 292))); }
  void set_unknown_292_bits(uint8_t v) const { if (auto* d = direct()) { d->unknown_292 = v; return; } if (memory::IsHandle(address + 292)) memory::OwnerStore(address + 292, 1, v); else guest::StoreU8(base, address + 292, v); }
  uint8_t unknown_292() const { if (auto* d = direct()) return d->unknown_292; return uint8_t((memory::IsHandle(address + 292) ? uint8_t(memory::OwnerLoad(address + 292, 1)) : guest::LoadU8(base, address + 292))); }
  void set_unknown_292(uint8_t v) const { if (auto* d = direct()) { d->unknown_292 = v; return; } if (memory::IsHandle(address + 292)) memory::OwnerStore(address + 292, 1, v); else guest::StoreU8(base, address + 292, v); }
  // this emitter's +276 (not read)
  uint32_t unknown_296_address() const { return address + 296; }
  uint32_t unknown_296_bits() const { if (auto* d = direct()) return d->unknown_296; return uint32_t((memory::IsHandle(address + 296) ? uint32_t(memory::OwnerLoad(address + 296, 4)) : guest::LoadU32(base, address + 296))); }
  void set_unknown_296_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_296 = v; return; } if (memory::IsHandle(address + 296)) memory::OwnerStore(address + 296, 4, v); else guest::StoreU32(base, address + 296, v); }
  uint32_t unknown_296() const { if (auto* d = direct()) return d->unknown_296; return uint32_t((memory::IsHandle(address + 296) ? uint32_t(memory::OwnerLoad(address + 296, 4)) : guest::LoadU32(base, address + 296))); }
  void set_unknown_296(uint32_t v) const { if (auto* d = direct()) { d->unknown_296 = v; return; } if (memory::IsHandle(address + 296)) memory::OwnerStore(address + 296, 4, v); else guest::StoreU32(base, address + 296, v); }
};

// allocated; measure: Emitter.+0
struct EmitterCue : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  // the cue's vtable (and, through the fault, its slot 0: probably Play)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // the vtable's slot +4: probably Stop (Emitter::Stop, with 1: at once)
  uint32_t fault_stop() const { return guest::LoadU32(base, address + 4); }
  void set_fault_stop(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t fault_stop_address() const { return address + 4; }
  // the vtable's slot +8: probably GetState (Emitter::IsNotStopped, IsPrepared)
  uint32_t fault_get_state() const { return guest::LoadU32(base, address + 8); }
  void set_fault_get_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t fault_get_state_address() const { return address + 8; }
  // the vtable's slot +12: probably Destroy (Emitter::DestroyCue)
  uint32_t fault_destroy() const { return guest::LoadU32(base, address + 12); }
  void set_fault_destroy(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_destroy_address() const { return address + 12; }
  // the vtable's slot +60: GetProperties (Emitter::Prepare, into the emitter's +136)
  uint32_t fault_get_properties() const { return guest::LoadU32(base, address + 60); }
  void set_fault_get_properties(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t fault_get_properties_address() const { return address + 60; }
};

// allocated; measure: Emitter::Prepare.r4.+8
struct EmitterSoundBank : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = EmitterSoundBankVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: Emitter::Prepare.r4.+8.+0
struct EmitterSoundBankVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // probably Prepare (the cue index, flags 0, time 0, the cue into the emitter's +0)
  uint32_t prepare() const { return guest::LoadU32(base, address + 4); }
  void set_prepare(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t prepare_address() const { return address + 4; }
};

// embedded; measure: Emitter::Prepare.r5
struct EmitterCueIndex16 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint16_t index() const { return guest::LoadU16(base, address + 0); }
  void set_index(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// allocated; measure: BankPair
#pragma pack(push, 1)
struct SoundBankPairData {
  uint32_t vtable = 0;  // +0
  uint32_t vtable2 = 0;  // +4
  uint32_t command_8 = 0;  // +8
  uint32_t command_12 = 0;  // +12
  uint8_t command_flag_16 = 0;  // +16
  uint8_t command_flag_17 = 0;  // +17
  uint8_t command_flag_18 = 0;  // +18
  uint8_t pad_19[5] = {};
  uint32_t delegate_vtable = 0;  // +24
  uint32_t delegate_28 = 0;  // +28
  uint32_t delegate_32 = 0;  // +32
  uint32_t owner = 0;  // +36
  uint8_t name[28] = {};  // +40, guest order
  uint8_t skip_wave_bank = 0;  // +68
  uint8_t release_requested = 0;  // +69
  uint8_t pad_70[2] = {};
  uint32_t wave_bank_holder = 0;  // +72
  uint32_t sound_bank_holder = 0;  // +76
  uint32_t bank_index = 0;  // +80
  uint32_t wave_bank_file_id = 0;  // +84
  uint32_t sound_bank_file_id = 0;  // +88
  uint32_t wave_bank_size = 0;  // +92
  uint32_t sound_bank_size = 0;  // +96
  uint32_t wave_bank_buffer = 0;  // +100
  uint32_t sound_bank_buffer = 0;  // +104
  uint8_t wave_bank_loaded = 0;  // +108
  uint8_t sound_bank_loaded = 0;  // +109
  uint8_t pad_110[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(SoundBankPairData) == 112);
inline constexpr memory::LayoutField kSoundBankPairFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}};
inline constexpr memory::Layout kSoundBankPairLayout = {kSoundBankPairFields, 17};

struct SoundBankPair : View {
  using View::View;
  static constexpr uint32_t kSize = 112;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x820066DC;
  SoundBankPairData* data() const { return static_cast<SoundBankPairData*>(memory::HandleObject(address)); }
  static SoundBankPair New(uint8_t* base) {
    return SoundBankPair(base, memory::MakeHandle(new SoundBankPairData(), sizeof(SoundBankPairData), "SoundBankPair", &kSoundBankPairLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundBankPair NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundBankPairData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundBankPairData*>(::operator new(sizeof(SoundBankPairData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundBankPairData();
    return SoundBankPair(base, memory::MakeHandle(p, sizeof(SoundBankPairData) * count, "SoundBankPair", &kSoundBankPairLayout, sizeof(SoundBankPairData)));
  }
  SoundBankPair at(uint32_t i) const { return SoundBankPair(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundBankPairData* direct() const { return static_cast<SoundBankPairData*>(memory::HandleOwn(address, &kSoundBankPairLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundBankPairLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundBankPairLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x820066DC (0x82000EB8, the base's, first)
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // 0x82002908 (0x82000AB4, then 0x82000BA8, first)
  uint32_t vtable2_address() const { return address + 4; }
  uint32_t vtable2_bits() const { if (auto* d = direct()) return d->vtable2; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_vtable2_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable2 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t vtable2() const { if (auto* d = direct()) return d->vtable2; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_vtable2(uint32_t v) const { if (auto* d = direct()) { d->vtable2 = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the command base's +8, 0
  uint32_t command_8_address() const { return address + 8; }
  uint32_t command_8_bits() const { if (auto* d = direct()) return d->command_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_command_8_bits(uint32_t v) const { if (auto* d = direct()) { d->command_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t command_8() const { if (auto* d = direct()) return d->command_8; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_command_8(uint32_t v) const { if (auto* d = direct()) { d->command_8 = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the command base's +12, 0 (the manager's handler runs sub_82094B10 at 3)
  uint32_t command_12_address() const { return address + 12; }
  uint32_t command_12_bits() const { if (auto* d = direct()) return d->command_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_command_12_bits(uint32_t v) const { if (auto* d = direct()) { d->command_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t command_12() const { if (auto* d = direct()) return d->command_12; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_command_12(uint32_t v) const { if (auto* d = direct()) { d->command_12 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  // 0
  uint32_t command_flag_16_address() const { return address + 16; }
  uint8_t command_flag_16_bits() const { if (auto* d = direct()) return d->command_flag_16; return uint8_t((memory::IsHandle(address + 16) ? uint8_t(memory::OwnerLoad(address + 16, 1)) : guest::LoadU8(base, address + 16))); }
  void set_command_flag_16_bits(uint8_t v) const { if (auto* d = direct()) { d->command_flag_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 1, v); else guest::StoreU8(base, address + 16, v); }
  uint8_t command_flag_16() const { if (auto* d = direct()) return d->command_flag_16; return uint8_t((memory::IsHandle(address + 16) ? uint8_t(memory::OwnerLoad(address + 16, 1)) : guest::LoadU8(base, address + 16))); }
  void set_command_flag_16(uint8_t v) const { if (auto* d = direct()) { d->command_flag_16 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 1, v); else guest::StoreU8(base, address + 16, v); }
  // 1
  uint32_t command_flag_17_address() const { return address + 17; }
  uint8_t command_flag_17_bits() const { if (auto* d = direct()) return d->command_flag_17; return uint8_t((memory::IsHandle(address + 17) ? uint8_t(memory::OwnerLoad(address + 17, 1)) : guest::LoadU8(base, address + 17))); }
  void set_command_flag_17_bits(uint8_t v) const { if (auto* d = direct()) { d->command_flag_17 = v; return; } if (memory::IsHandle(address + 17)) memory::OwnerStore(address + 17, 1, v); else guest::StoreU8(base, address + 17, v); }
  uint8_t command_flag_17() const { if (auto* d = direct()) return d->command_flag_17; return uint8_t((memory::IsHandle(address + 17) ? uint8_t(memory::OwnerLoad(address + 17, 1)) : guest::LoadU8(base, address + 17))); }
  void set_command_flag_17(uint8_t v) const { if (auto* d = direct()) { d->command_flag_17 = v; return; } if (memory::IsHandle(address + 17)) memory::OwnerStore(address + 17, 1, v); else guest::StoreU8(base, address + 17, v); }
  // 0
  uint32_t command_flag_18_address() const { return address + 18; }
  uint8_t command_flag_18_bits() const { if (auto* d = direct()) return d->command_flag_18; return uint8_t((memory::IsHandle(address + 18) ? uint8_t(memory::OwnerLoad(address + 18, 1)) : guest::LoadU8(base, address + 18))); }
  void set_command_flag_18_bits(uint8_t v) const { if (auto* d = direct()) { d->command_flag_18 = v; return; } if (memory::IsHandle(address + 18)) memory::OwnerStore(address + 18, 1, v); else guest::StoreU8(base, address + 18, v); }
  uint8_t command_flag_18() const { if (auto* d = direct()) return d->command_flag_18; return uint8_t((memory::IsHandle(address + 18) ? uint8_t(memory::OwnerLoad(address + 18, 1)) : guest::LoadU8(base, address + 18))); }
  void set_command_flag_18(uint8_t v) const { if (auto* d = direct()) { d->command_flag_18 = v; return; } if (memory::IsHandle(address + 18)) memory::OwnerStore(address + 18, 1, v); else guest::StoreU8(base, address + 18, v); }
  // 0x82002850 (0x82000ABC first)
  uint32_t delegate_vtable_address() const { return address + 24; }
  uint32_t delegate_vtable_bits() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_delegate_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  uint32_t delegate_vtable() const { if (auto* d = direct()) return d->delegate_vtable; return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_delegate_vtable(uint32_t v) const { if (auto* d = direct()) { d->delegate_vtable = v; return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  // 0
  uint32_t delegate_28_address() const { return address + 28; }
  uint32_t delegate_28_bits() const { if (auto* d = direct()) return d->delegate_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_delegate_28_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t delegate_28() const { if (auto* d = direct()) return d->delegate_28; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_delegate_28(uint32_t v) const { if (auto* d = direct()) { d->delegate_28 = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // 0
  uint32_t delegate_32_address() const { return address + 32; }
  uint32_t delegate_32_bits() const { if (auto* d = direct()) return d->delegate_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_delegate_32_bits(uint32_t v) const { if (auto* d = direct()) { d->delegate_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  uint32_t delegate_32() const { if (auto* d = direct()) return d->delegate_32; return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_delegate_32(uint32_t v) const { if (auto* d = direct()) { d->delegate_32 = v; return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  // the manager's +4 interface (its slot +12 GetEngine)
  uint32_t owner_address() const { return address + 36; }
  uint32_t owner_bits() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_owner_bits(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t owner() const { if (auto* d = direct()) return d->owner; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_owner(uint32_t v) const { if (auto* d = direct()) { d->owner = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // a std::string
  uint32_t name_address() const { return address + 40; }
  uint8_t* name() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 40 : guest::Pointer(base, address + 40); }
  // 1 skips the wave bank's load (probably one loaded elsewhere)
  uint32_t skip_wave_bank_address() const { return address + 68; }
  uint8_t skip_wave_bank_bits() const { if (auto* d = direct()) return d->skip_wave_bank; return uint8_t((memory::IsHandle(address + 68) ? uint8_t(memory::OwnerLoad(address + 68, 1)) : guest::LoadU8(base, address + 68))); }
  void set_skip_wave_bank_bits(uint8_t v) const { if (auto* d = direct()) { d->skip_wave_bank = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 1, v); else guest::StoreU8(base, address + 68, v); }
  uint8_t skip_wave_bank() const { if (auto* d = direct()) return d->skip_wave_bank; return uint8_t((memory::IsHandle(address + 68) ? uint8_t(memory::OwnerLoad(address + 68, 1)) : guest::LoadU8(base, address + 68))); }
  void set_skip_wave_bank(uint8_t v) const { if (auto* d = direct()) { d->skip_wave_bank = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 1, v); else guest::StoreU8(base, address + 68, v); }
  // BankPair::Release
  uint32_t release_requested_address() const { return address + 69; }
  uint8_t release_requested_bits() const { if (auto* d = direct()) return d->release_requested; return uint8_t((memory::IsHandle(address + 69) ? uint8_t(memory::OwnerLoad(address + 69, 1)) : guest::LoadU8(base, address + 69))); }
  void set_release_requested_bits(uint8_t v) const { if (auto* d = direct()) { d->release_requested = v; return; } if (memory::IsHandle(address + 69)) memory::OwnerStore(address + 69, 1, v); else guest::StoreU8(base, address + 69, v); }
  uint8_t release_requested() const { if (auto* d = direct()) return d->release_requested; return uint8_t((memory::IsHandle(address + 69) ? uint8_t(memory::OwnerLoad(address + 69, 1)) : guest::LoadU8(base, address + 69))); }
  void set_release_requested(uint8_t v) const { if (auto* d = direct()) { d->release_requested = v; return; } if (memory::IsHandle(address + 69)) memory::OwnerStore(address + 69, 1, v); else guest::StoreU8(base, address + 69, v); }
  uint32_t wave_bank_holder_address() const { return address + 72; }
  uint32_t wave_bank_holder_bits() const { if (auto* d = direct()) return d->wave_bank_holder; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(base, address + 72))); }
  void set_wave_bank_holder_bits(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_holder = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(base, address + 72, v); }
  template <typename T = WaveBankHolder>
  T wave_bank_holder() const { return T(base, wave_bank_holder_bits()); }
  void set_wave_bank_holder(const View& w) const { set_wave_bank_holder_bits(w.address); }
  void set_wave_bank_holder(uint32_t v) const { set_wave_bank_holder_bits(v); }
  uint32_t sound_bank_holder_address() const { return address + 76; }
  uint32_t sound_bank_holder_bits() const { if (auto* d = direct()) return d->sound_bank_holder; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_sound_bank_holder_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_holder = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  template <typename T = SoundBankHolder>
  T sound_bank_holder() const { return T(base, sound_bank_holder_bits()); }
  void set_sound_bank_holder(const View& w) const { set_sound_bank_holder_bits(w.address); }
  void set_sound_bank_holder(uint32_t v) const { set_sound_bank_holder_bits(v); }
  uint32_t bank_index_address() const { return address + 80; }
  uint32_t bank_index_bits() const { if (auto* d = direct()) return d->bank_index; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_bank_index_bits(uint32_t v) const { if (auto* d = direct()) { d->bank_index = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t bank_index() const { if (auto* d = direct()) return d->bank_index; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_bank_index(uint32_t v) const { if (auto* d = direct()) { d->bank_index = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t wave_bank_file_id_address() const { return address + 84; }
  uint32_t wave_bank_file_id_bits() const { if (auto* d = direct()) return d->wave_bank_file_id; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_wave_bank_file_id_bits(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_file_id = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t wave_bank_file_id() const { if (auto* d = direct()) return d->wave_bank_file_id; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_wave_bank_file_id(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_file_id = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t sound_bank_file_id_address() const { return address + 88; }
  uint32_t sound_bank_file_id_bits() const { if (auto* d = direct()) return d->sound_bank_file_id; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_sound_bank_file_id_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_file_id = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t sound_bank_file_id() const { if (auto* d = direct()) return d->sound_bank_file_id; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_sound_bank_file_id(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_file_id = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t wave_bank_size_address() const { return address + 92; }
  uint32_t wave_bank_size_bits() const { if (auto* d = direct()) return d->wave_bank_size; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_wave_bank_size_bits(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_size = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t wave_bank_size() const { if (auto* d = direct()) return d->wave_bank_size; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_wave_bank_size(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_size = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t sound_bank_size_address() const { return address + 96; }
  uint32_t sound_bank_size_bits() const { if (auto* d = direct()) return d->sound_bank_size; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_sound_bank_size_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_size = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  uint32_t sound_bank_size() const { if (auto* d = direct()) return d->sound_bank_size; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_sound_bank_size(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_size = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  // XPhysicalAlloc's (bank 2: [0x826C5C88])
  uint32_t wave_bank_buffer_address() const { return address + 100; }
  uint32_t wave_bank_buffer_bits() const { if (auto* d = direct()) return d->wave_bank_buffer; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_wave_bank_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_buffer = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  uint32_t wave_bank_buffer() const { if (auto* d = direct()) return d->wave_bank_buffer; return uint32_t((memory::IsHandle(address + 100) ? uint32_t(memory::OwnerLoad(address + 100, 4)) : guest::LoadU32(base, address + 100))); }
  void set_wave_bank_buffer(uint32_t v) const { if (auto* d = direct()) { d->wave_bank_buffer = v; return; } if (memory::IsHandle(address + 100)) memory::OwnerStore(address + 100, 4, v); else guest::StoreU32(base, address + 100, v); }
  // operator new[]'s, a SoundBankFile blob (bank 2: [0x826C5C8C], physical)
  uint32_t sound_bank_buffer_address() const { return address + 104; }
  uint32_t sound_bank_buffer_bits() const { if (auto* d = direct()) return d->sound_bank_buffer; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(base, address + 104))); }
  void set_sound_bank_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_buffer = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(base, address + 104, v); }
  uint32_t sound_bank_buffer() const { if (auto* d = direct()) return d->sound_bank_buffer; return uint32_t((memory::IsHandle(address + 104) ? uint32_t(memory::OwnerLoad(address + 104, 4)) : guest::LoadU32(base, address + 104))); }
  void set_sound_bank_buffer(uint32_t v) const { if (auto* d = direct()) { d->sound_bank_buffer = v; return; } if (memory::IsHandle(address + 104)) memory::OwnerStore(address + 104, 4, v); else guest::StoreU32(base, address + 104, v); }
  uint32_t wave_bank_loaded_address() const { return address + 108; }
  uint8_t wave_bank_loaded_bits() const { if (auto* d = direct()) return d->wave_bank_loaded; return uint8_t((memory::IsHandle(address + 108) ? uint8_t(memory::OwnerLoad(address + 108, 1)) : guest::LoadU8(base, address + 108))); }
  void set_wave_bank_loaded_bits(uint8_t v) const { if (auto* d = direct()) { d->wave_bank_loaded = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 1, v); else guest::StoreU8(base, address + 108, v); }
  uint8_t wave_bank_loaded() const { if (auto* d = direct()) return d->wave_bank_loaded; return uint8_t((memory::IsHandle(address + 108) ? uint8_t(memory::OwnerLoad(address + 108, 1)) : guest::LoadU8(base, address + 108))); }
  void set_wave_bank_loaded(uint8_t v) const { if (auto* d = direct()) { d->wave_bank_loaded = v; return; } if (memory::IsHandle(address + 108)) memory::OwnerStore(address + 108, 1, v); else guest::StoreU8(base, address + 108, v); }
  uint32_t sound_bank_loaded_address() const { return address + 109; }
  uint8_t sound_bank_loaded_bits() const { if (auto* d = direct()) return d->sound_bank_loaded; return uint8_t((memory::IsHandle(address + 109) ? uint8_t(memory::OwnerLoad(address + 109, 1)) : guest::LoadU8(base, address + 109))); }
  void set_sound_bank_loaded_bits(uint8_t v) const { if (auto* d = direct()) { d->sound_bank_loaded = v; return; } if (memory::IsHandle(address + 109)) memory::OwnerStore(address + 109, 1, v); else guest::StoreU8(base, address + 109, v); }
  uint8_t sound_bank_loaded() const { if (auto* d = direct()) return d->sound_bank_loaded; return uint8_t((memory::IsHandle(address + 109) ? uint8_t(memory::OwnerLoad(address + 109, 1)) : guest::LoadU8(base, address + 109))); }
  void set_sound_bank_loaded(uint8_t v) const { if (auto* d = direct()) { d->sound_bank_loaded = v; return; } if (memory::IsHandle(address + 109)) memory::OwnerStore(address + 109, 1, v); else guest::StoreU8(base, address + 109, v); }
};

// allocated; measure: WaveBankHolder, BankPair.+72
#pragma pack(push, 1)
struct WaveBankHolderData {
  uint32_t buffer = 0;  // +0
  uint32_t size = 0;  // +4
  uint32_t wave_bank = 0;  // +8
  uint32_t handle = 0;  // +12
  uint8_t pad_16[12] = {};
};
#pragma pack(pop)
static_assert(sizeof(WaveBankHolderData) == 28);
inline constexpr memory::LayoutField kWaveBankHolderFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kWaveBankHolderLayout = {kWaveBankHolderFields, 4};

struct WaveBankHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  WaveBankHolderData* data() const { return static_cast<WaveBankHolderData*>(memory::HandleObject(address)); }
  static WaveBankHolder New(uint8_t* base) {
    return WaveBankHolder(base, memory::MakeHandle(new WaveBankHolderData(), sizeof(WaveBankHolderData), "WaveBankHolder", &kWaveBankHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static WaveBankHolder NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(WaveBankHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<WaveBankHolderData*>(::operator new(sizeof(WaveBankHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) WaveBankHolderData();
    return WaveBankHolder(base, memory::MakeHandle(p, sizeof(WaveBankHolderData) * count, "WaveBankHolder", &kWaveBankHolderLayout, sizeof(WaveBankHolderData)));
  }
  WaveBankHolder at(uint32_t i) const { return WaveBankHolder(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  WaveBankHolderData* direct() const { return static_cast<WaveBankHolderData*>(memory::HandleOwn(address, &kWaveBankHolderLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kWaveBankHolderLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kWaveBankHolderLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t buffer_address() const { return address + 0; }
  uint32_t buffer_bits() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t buffer() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_buffer(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 4; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the XACT wave bank
  uint32_t wave_bank_address() const { return address + 8; }
  uint32_t wave_bank_bits() const { if (auto* d = direct()) return d->wave_bank; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_wave_bank_bits(uint32_t v) const { if (auto* d = direct()) { d->wave_bank = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t wave_bank() const { if (auto* d = direct()) return d->wave_bank; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_wave_bank(uint32_t v) const { if (auto* d = direct()) { d->wave_bank = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // a file handle, -1 for none (always, in play)
  uint32_t handle_address() const { return address + 12; }
  uint32_t handle_bits() const { if (auto* d = direct()) return d->handle; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_handle_bits(uint32_t v) const { if (auto* d = direct()) { d->handle = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t handle() const { if (auto* d = direct()) return d->handle; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_handle(uint32_t v) const { if (auto* d = direct()) { d->handle = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
};

// allocated; measure: SoundBankHolder, Emitter::Prepare.r4
#pragma pack(push, 1)
struct SoundBankHolderData {
  uint32_t buffer = 0;  // +0
  uint32_t size = 0;  // +4
  uint32_t sound_bank = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(SoundBankHolderData) == 12);
inline constexpr memory::LayoutField kSoundBankHolderFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kSoundBankHolderLayout = {kSoundBankHolderFields, 3};

struct SoundBankHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  SoundBankHolderData* data() const { return static_cast<SoundBankHolderData*>(memory::HandleObject(address)); }
  static SoundBankHolder New(uint8_t* base) {
    return SoundBankHolder(base, memory::MakeHandle(new SoundBankHolderData(), sizeof(SoundBankHolderData), "SoundBankHolder", &kSoundBankHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundBankHolder NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundBankHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundBankHolderData*>(::operator new(sizeof(SoundBankHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundBankHolderData();
    return SoundBankHolder(base, memory::MakeHandle(p, sizeof(SoundBankHolderData) * count, "SoundBankHolder", &kSoundBankHolderLayout, sizeof(SoundBankHolderData)));
  }
  SoundBankHolder at(uint32_t i) const { return SoundBankHolder(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundBankHolderData* direct() const { return static_cast<SoundBankHolderData*>(memory::HandleOwn(address, &kSoundBankHolderLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundBankHolderLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundBankHolderLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t buffer_address() const { return address + 0; }
  uint32_t buffer_bits() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_buffer_bits(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t buffer() const { if (auto* d = direct()) return d->buffer; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_buffer(uint32_t v) const { if (auto* d = direct()) { d->buffer = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t size_address() const { return address + 4; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the XACT sound bank
  uint32_t sound_bank_address() const { return address + 8; }
  uint32_t sound_bank_bits() const { if (auto* d = direct()) return d->sound_bank; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_sound_bank_bits(uint32_t v) const { if (auto* d = direct()) { d->sound_bank = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  template <typename T = EmitterSoundBank>
  T sound_bank() const { return T(base, sound_bank_bits()); }
  void set_sound_bank(const View& w) const { set_sound_bank_bits(w.address); }
  void set_sound_bank(uint32_t v) const { set_sound_bank_bits(v); }
};

// allocated; measure: -
struct SoundBankFile : View {
  using View::View;
  static constexpr uint32_t kSize = 0;
  static SoundBankFile New(uint8_t* base, uint32_t size) { return SoundBankFile(base, memory::NewBlob(size, "SoundBankFile")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
};

// embedded; measure: sub_82118438.r3, sub_82118930.r3.+1516
struct BankPairSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = SoundBankPair>
  T pair() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_pair(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_pair(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t pair_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+1456, AudioManager::SetListenerOrientation.r3@+1456, AudioManager::SetListenerPosition.r3@+1456, AudioManager::SetListenerParams.r3@+1456
#pragma pack(push, 1)
struct SoundListenerData {
  float front_x = 0;  // +0
  float front_y = 0;  // +4
  float front_z = 0;  // +8
  float top_x = 0;  // +12
  float top_y = 0;  // +16
  float top_z = 0;  // +20
  float position_x = 0;  // +24
  float position_y = 0;  // +28
  float position_z = 0;  // +32
  float velocity_x = 0;  // +36
  float velocity_y = 0;  // +40
  float velocity_z = 0;  // +44
  float param_0 = 0;  // +48
  float param_1 = 0;  // +52
};
#pragma pack(pop)
static_assert(sizeof(SoundListenerData) == 56);
inline constexpr memory::LayoutField kSoundListenerFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}};
inline constexpr memory::Layout kSoundListenerLayout = {kSoundListenerFields, 14};

struct SoundListener : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SoundListenerData* data() const { return static_cast<SoundListenerData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SoundListener New(uint8_t* base) {
    return SoundListener(base, memory::MakeHandle(new SoundListenerData(), sizeof(SoundListenerData), "SoundListener", &kSoundListenerLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundListener NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundListenerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundListenerData*>(::operator new(sizeof(SoundListenerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundListenerData();
    return SoundListener(base, memory::MakeHandle(p, sizeof(SoundListenerData) * count, "SoundListener", &kSoundListenerLayout, sizeof(SoundListenerData)));
  }
  SoundListener at(uint32_t i) const { return SoundListener(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundListenerData* direct() const { return static_cast<SoundListenerData*>(memory::HandleOwn(address, &kSoundListenerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundListenerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundListenerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // front (0, 0, 1)
  uint32_t front_x_address() const { return address + 0; }
  uint32_t front_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_x); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_front_x_bits(uint32_t v) const { if (auto* d = direct()) { d->front_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  float front_x() const { return std::bit_cast<float>(front_x_bits()); }
  void set_front_x(float v) const { set_front_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_y_address() const { return address + 4; }
  uint32_t front_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_y); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_front_y_bits(uint32_t v) const { if (auto* d = direct()) { d->front_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  float front_y() const { return std::bit_cast<float>(front_y_bits()); }
  void set_front_y(float v) const { set_front_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_z_address() const { return address + 8; }
  uint32_t front_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_z); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_front_z_bits(uint32_t v) const { if (auto* d = direct()) { d->front_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  float front_z() const { return std::bit_cast<float>(front_z_bits()); }
  void set_front_z(float v) const { set_front_z_bits(std::bit_cast<uint32_t>(v)); }
  // top (0, 1, 0)
  uint32_t top_x_address() const { return address + 12; }
  uint32_t top_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_x); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_top_x_bits(uint32_t v) const { if (auto* d = direct()) { d->top_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  float top_x() const { return std::bit_cast<float>(top_x_bits()); }
  void set_top_x(float v) const { set_top_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_y_address() const { return address + 16; }
  uint32_t top_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_y); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_top_y_bits(uint32_t v) const { if (auto* d = direct()) { d->top_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  float top_y() const { return std::bit_cast<float>(top_y_bits()); }
  void set_top_y(float v) const { set_top_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_z_address() const { return address + 20; }
  uint32_t top_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_z); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_top_z_bits(uint32_t v) const { if (auto* d = direct()) { d->top_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  float top_z() const { return std::bit_cast<float>(top_z_bits()); }
  void set_top_z(float v) const { set_top_z_bits(std::bit_cast<uint32_t>(v)); }
  // (0, 0, [0x82001FD8]) at first
  uint32_t position_x_address() const { return address + 24; }
  uint32_t position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_x); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  float position_x() const { return std::bit_cast<float>(position_x_bits()); }
  void set_position_x(float v) const { set_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_y_address() const { return address + 28; }
  uint32_t position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_y); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  float position_y() const { return std::bit_cast<float>(position_y_bits()); }
  void set_position_y(float v) const { set_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_z_address() const { return address + 32; }
  uint32_t position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_z); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(base, address + 32))); }
  void set_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(base, address + 32, v); }
  float position_z() const { return std::bit_cast<float>(position_z_bits()); }
  void set_position_z(float v) const { set_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_x_address() const { return address + 36; }
  uint32_t velocity_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_x); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_velocity_x_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  float velocity_x() const { return std::bit_cast<float>(velocity_x_bits()); }
  void set_velocity_x(float v) const { set_velocity_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_y_address() const { return address + 40; }
  uint32_t velocity_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_y); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_velocity_y_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  float velocity_y() const { return std::bit_cast<float>(velocity_y_bits()); }
  void set_velocity_y(float v) const { set_velocity_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_z_address() const { return address + 44; }
  uint32_t velocity_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_z); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_velocity_z_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  float velocity_z() const { return std::bit_cast<float>(velocity_z_bits()); }
  void set_velocity_z(float v) const { set_velocity_z_bits(std::bit_cast<uint32_t>(v)); }
  // [0x82002A0C] at first; AudioManager::SetListenerParams's (slot 28, not seen in play)
  uint32_t param_0_address() const { return address + 48; }
  uint32_t param_0_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->param_0); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(base, address + 48))); }
  void set_param_0_bits(uint32_t v) const { if (auto* d = direct()) { d->param_0 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(base, address + 48, v); }
  float param_0() const { return std::bit_cast<float>(param_0_bits()); }
  void set_param_0(float v) const { set_param_0_bits(std::bit_cast<uint32_t>(v)); }
  // [0x820027A4] at first; Listener::SetFloat52's
  uint32_t param_1_address() const { return address + 52; }
  uint32_t param_1_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->param_1); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(base, address + 52))); }
  void set_param_1_bits(uint32_t v) const { if (auto* d = direct()) { d->param_1 = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(base, address + 52, v); }
  float param_1() const { return std::bit_cast<float>(param_1_bits()); }
  void set_param_1(float v) const { set_param_1_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: sub_8259CC80.r3
#pragma pack(push, 1)
struct SoundVector36TargetData {
  uint8_t pad_0[36] = {};
  float vector_x = 0;  // +36
  float vector_y = 0;  // +40
  float vector_z = 0;  // +44
};
#pragma pack(pop)
static_assert(sizeof(SoundVector36TargetData) == 48);
inline constexpr memory::LayoutField kSoundVector36TargetFields[] = {{36, 4}, {40, 4}, {44, 4}};
inline constexpr memory::Layout kSoundVector36TargetLayout = {kSoundVector36TargetFields, 3};

struct SoundVector36Target : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SoundVector36TargetData* data() const { return static_cast<SoundVector36TargetData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SoundVector36Target New(uint8_t* base) {
    return SoundVector36Target(base, memory::MakeHandle(new SoundVector36TargetData(), sizeof(SoundVector36TargetData), "SoundVector36Target", &kSoundVector36TargetLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundVector36Target NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundVector36TargetData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundVector36TargetData*>(::operator new(sizeof(SoundVector36TargetData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundVector36TargetData();
    return SoundVector36Target(base, memory::MakeHandle(p, sizeof(SoundVector36TargetData) * count, "SoundVector36Target", &kSoundVector36TargetLayout, sizeof(SoundVector36TargetData)));
  }
  SoundVector36Target at(uint32_t i) const { return SoundVector36Target(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundVector36TargetData* direct() const { return static_cast<SoundVector36TargetData*>(memory::HandleOwn(address, &kSoundVector36TargetLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundVector36TargetLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundVector36TargetLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t vector_x_address() const { return address + 36; }
  uint32_t vector_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->vector_x); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_vector_x_bits(uint32_t v) const { if (auto* d = direct()) { d->vector_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  float vector_x() const { return std::bit_cast<float>(vector_x_bits()); }
  void set_vector_x(float v) const { set_vector_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t vector_y_address() const { return address + 40; }
  uint32_t vector_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->vector_y); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_vector_y_bits(uint32_t v) const { if (auto* d = direct()) { d->vector_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  float vector_y() const { return std::bit_cast<float>(vector_y_bits()); }
  void set_vector_y(float v) const { set_vector_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t vector_z_address() const { return address + 44; }
  uint32_t vector_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->vector_z); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(base, address + 44))); }
  void set_vector_z_bits(uint32_t v) const { if (auto* d = direct()) { d->vector_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(base, address + 44, v); }
  float vector_z() const { return std::bit_cast<float>(vector_z_bits()); }
  void set_vector_z(float v) const { set_vector_z_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: sub_8259CC80.r4, Listener::SetOrientation.r4, Listener::SetOrientation.r5, Listener::SetPosition.r4
#pragma pack(push, 1)
struct SoundVector3Data {
  float x = 0;  // +0
  float y = 0;  // +4
  float z = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(SoundVector3Data) == 12);
inline constexpr memory::LayoutField kSoundVector3Fields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kSoundVector3Layout = {kSoundVector3Fields, 3};

struct SoundVector3 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SoundVector3Data* data() const { return static_cast<SoundVector3Data*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SoundVector3 New(uint8_t* base) {
    return SoundVector3(base, memory::MakeHandle(new SoundVector3Data(), sizeof(SoundVector3Data), "SoundVector3", &kSoundVector3Layout));
  }
  template <uint32_t Stride = kSize>
  static SoundVector3 NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundVector3Data) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundVector3Data*>(::operator new(sizeof(SoundVector3Data) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundVector3Data();
    return SoundVector3(base, memory::MakeHandle(p, sizeof(SoundVector3Data) * count, "SoundVector3", &kSoundVector3Layout, sizeof(SoundVector3Data)));
  }
  SoundVector3 at(uint32_t i) const { return SoundVector3(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundVector3Data* direct() const { return static_cast<SoundVector3Data*>(memory::HandleOwn(address, &kSoundVector3Layout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundVector3Layout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundVector3Layout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  uint32_t x_address() const { return address + 0; }
  uint32_t x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  float x() const { return std::bit_cast<float>(x_bits()); }
  void set_x(float v) const { set_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t y_address() const { return address + 4; }
  uint32_t y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->y); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_y_bits(uint32_t v) const { if (auto* d = direct()) { d->y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  float y() const { return std::bit_cast<float>(y_bits()); }
  void set_y(float v) const { set_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t z_address() const { return address + 8; }
  uint32_t z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->z); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_z_bits(uint32_t v) const { if (auto* d = direct()) { d->z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  float z() const { return std::bit_cast<float>(z_bits()); }
  void set_z(float v) const { set_z_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: sub_82118638.r3@+16, sub_82118638.r3@+16@+8, AudioManager@+16, AudioManager::GetCategory.r3@+16, AudioManager::SetCategoryVolume.r3@+16, AudioManager::PauseCategory.r3@+16, AudioManager::ResumeCategory.r3@+16, AudioEngine::DoWork.r3
#pragma pack(push, 1)
struct AudioEngineData {
  uint32_t settings = 0;  // +0
  uint32_t settings_size = 0;  // +4
  uint32_t xact_engine = 0;  // +8
  uint32_t x3d_speaker_count = 0;  // +12
  uint32_t x3d_lfe_speaker = 0;  // +16
  float x3d_speed_of_sound = 0;  // +20
  float x3d_doppler_limit = 0;  // +24
  uint32_t dsp_matrix = 0;  // +28
  uint8_t pad_32[4] = {};
  uint32_t dsp_source_channels = 0;  // +36
  uint32_t dsp_destination_channels = 0;  // +40
  uint8_t pad_44[32] = {};
  uint32_t matrix_0 = 0;  // +76
  uint32_t matrix_1 = 0;  // +80
  uint32_t matrix_2 = 0;  // +84
  uint32_t matrix_3 = 0;  // +88
  uint32_t matrix_4 = 0;  // +92
  uint32_t matrix_5 = 0;  // +96
};
#pragma pack(pop)
static_assert(sizeof(AudioEngineData) == 100);
inline constexpr memory::LayoutField kAudioEngineFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {36, 4}, {40, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}};
inline constexpr memory::Layout kAudioEngineLayout = {kAudioEngineFields, 16};

struct AudioEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 100;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  AudioEngineData* data() const { return static_cast<AudioEngineData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static AudioEngine New(uint8_t* base) {
    return AudioEngine(base, memory::MakeHandle(new AudioEngineData(), sizeof(AudioEngineData), "AudioEngine", &kAudioEngineLayout));
  }
  template <uint32_t Stride = kSize>
  static AudioEngine NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AudioEngineData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AudioEngineData*>(::operator new(sizeof(AudioEngineData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AudioEngineData();
    return AudioEngine(base, memory::MakeHandle(p, sizeof(AudioEngineData) * count, "AudioEngine", &kAudioEngineLayout, sizeof(AudioEngineData)));
  }
  AudioEngine at(uint32_t i) const { return AudioEngine(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AudioEngineData* direct() const { return static_cast<AudioEngineData*>(memory::HandleOwn(address, &kAudioEngineLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAudioEngineLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAudioEngineLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // the global settings (TXR_XACT.xgs) read whole (also the fault's handle: speaker count)
  uint32_t settings_address() const { return address + 0; }
  uint32_t settings_bits() const { if (auto* d = direct()) return d->settings; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_settings_bits(uint32_t v) const { if (auto* d = direct()) { d->settings = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t settings() const { if (auto* d = direct()) return d->settings; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_settings(uint32_t v) const { if (auto* d = direct()) { d->settings = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // its size (also the fault's handle: LFE speaker)
  uint32_t settings_size_address() const { return address + 4; }
  uint32_t settings_size_bits() const { if (auto* d = direct()) return d->settings_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_settings_size_bits(uint32_t v) const { if (auto* d = direct()) { d->settings_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t settings_size() const { if (auto* d = direct()) return d->settings_size; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_settings_size(uint32_t v) const { if (auto* d = direct()) { d->settings_size = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // XACT3CreateEngine's
  uint32_t xact_engine_address() const { return address + 8; }
  uint32_t xact_engine_bits() const { if (auto* d = direct()) return d->xact_engine; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_xact_engine_bits(uint32_t v) const { if (auto* d = direct()) { d->xact_engine = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  template <typename T = XactEngine>
  T xact_engine() const { return T(base, xact_engine_bits()); }
  void set_xact_engine(const View& w) const { set_xact_engine_bits(w.address); }
  void set_xact_engine(uint32_t v) const { set_xact_engine_bits(v); }
  // the X3DAudio handle (16 bytes, car-sounds' X3DAudioHandle): 6 (5.1)
  uint32_t x3d_speaker_count_address() const { return address + 12; }
  uint32_t x3d_speaker_count_bits() const { if (auto* d = direct()) return d->x3d_speaker_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_x3d_speaker_count_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_speaker_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t x3d_speaker_count() const { if (auto* d = direct()) return d->x3d_speaker_count; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_x3d_speaker_count(uint32_t v) const { if (auto* d = direct()) { d->x3d_speaker_count = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t x3d_lfe_speaker_address() const { return address + 16; }
  uint32_t x3d_lfe_speaker_bits() const { if (auto* d = direct()) return d->x3d_lfe_speaker; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_x3d_lfe_speaker_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_lfe_speaker = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t x3d_lfe_speaker() const { if (auto* d = direct()) return d->x3d_lfe_speaker; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(base, address + 16))); }
  void set_x3d_lfe_speaker(uint32_t v) const { if (auto* d = direct()) { d->x3d_lfe_speaker = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(base, address + 16, v); }
  uint32_t x3d_speed_of_sound_address() const { return address + 20; }
  uint32_t x3d_speed_of_sound_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_speed_of_sound); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(base, address + 20))); }
  void set_x3d_speed_of_sound_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_speed_of_sound = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(base, address + 20, v); }
  float x3d_speed_of_sound() const { return std::bit_cast<float>(x3d_speed_of_sound_bits()); }
  void set_x3d_speed_of_sound(float v) const { set_x3d_speed_of_sound_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_doppler_limit_address() const { return address + 24; }
  uint32_t x3d_doppler_limit_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_doppler_limit); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(base, address + 24))); }
  void set_x3d_doppler_limit_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_doppler_limit = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(base, address + 24, v); }
  float x3d_doppler_limit() const { return std::bit_cast<float>(x3d_doppler_limit_bits()); }
  void set_x3d_doppler_limit(float v) const { set_x3d_doppler_limit_bits(std::bit_cast<uint32_t>(v)); }
  // the DSP settings' matrix pointer: this engine's +76
  uint32_t dsp_matrix_address() const { return address + 28; }
  uint32_t dsp_matrix_bits() const { if (auto* d = direct()) return d->dsp_matrix; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_dsp_matrix_bits(uint32_t v) const { if (auto* d = direct()) { d->dsp_matrix = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  uint32_t dsp_matrix() const { if (auto* d = direct()) return d->dsp_matrix; return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(base, address + 28))); }
  void set_dsp_matrix(uint32_t v) const { if (auto* d = direct()) { d->dsp_matrix = v; return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(base, address + 28, v); }
  // 1
  uint32_t dsp_source_channels_address() const { return address + 36; }
  uint32_t dsp_source_channels_bits() const { if (auto* d = direct()) return d->dsp_source_channels; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_dsp_source_channels_bits(uint32_t v) const { if (auto* d = direct()) { d->dsp_source_channels = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  uint32_t dsp_source_channels() const { if (auto* d = direct()) return d->dsp_source_channels; return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(base, address + 36))); }
  void set_dsp_source_channels(uint32_t v) const { if (auto* d = direct()) { d->dsp_source_channels = v; return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(base, address + 36, v); }
  // 6
  uint32_t dsp_destination_channels_address() const { return address + 40; }
  uint32_t dsp_destination_channels_bits() const { if (auto* d = direct()) return d->dsp_destination_channels; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_dsp_destination_channels_bits(uint32_t v) const { if (auto* d = direct()) { d->dsp_destination_channels = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  uint32_t dsp_destination_channels() const { if (auto* d = direct()) return d->dsp_destination_channels; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(base, address + 40))); }
  void set_dsp_destination_channels(uint32_t v) const { if (auto* d = direct()) { d->dsp_destination_channels = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(base, address + 40, v); }
  // six 1.0 (written as words)
  uint32_t matrix_0_address() const { return address + 76; }
  uint32_t matrix_0_bits() const { if (auto* d = direct()) return d->matrix_0; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_matrix_0_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_0 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  uint32_t matrix_0() const { if (auto* d = direct()) return d->matrix_0; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(base, address + 76))); }
  void set_matrix_0(uint32_t v) const { if (auto* d = direct()) { d->matrix_0 = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(base, address + 76, v); }
  uint32_t matrix_1_address() const { return address + 80; }
  uint32_t matrix_1_bits() const { if (auto* d = direct()) return d->matrix_1; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_matrix_1_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_1 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t matrix_1() const { if (auto* d = direct()) return d->matrix_1; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(base, address + 80))); }
  void set_matrix_1(uint32_t v) const { if (auto* d = direct()) { d->matrix_1 = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(base, address + 80, v); }
  uint32_t matrix_2_address() const { return address + 84; }
  uint32_t matrix_2_bits() const { if (auto* d = direct()) return d->matrix_2; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_matrix_2_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_2 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t matrix_2() const { if (auto* d = direct()) return d->matrix_2; return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(base, address + 84))); }
  void set_matrix_2(uint32_t v) const { if (auto* d = direct()) { d->matrix_2 = v; return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(base, address + 84, v); }
  uint32_t matrix_3_address() const { return address + 88; }
  uint32_t matrix_3_bits() const { if (auto* d = direct()) return d->matrix_3; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_matrix_3_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_3 = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t matrix_3() const { if (auto* d = direct()) return d->matrix_3; return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(base, address + 88))); }
  void set_matrix_3(uint32_t v) const { if (auto* d = direct()) { d->matrix_3 = v; return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(base, address + 88, v); }
  uint32_t matrix_4_address() const { return address + 92; }
  uint32_t matrix_4_bits() const { if (auto* d = direct()) return d->matrix_4; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_matrix_4_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_4 = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t matrix_4() const { if (auto* d = direct()) return d->matrix_4; return uint32_t((memory::IsHandle(address + 92) ? uint32_t(memory::OwnerLoad(address + 92, 4)) : guest::LoadU32(base, address + 92))); }
  void set_matrix_4(uint32_t v) const { if (auto* d = direct()) { d->matrix_4 = v; return; } if (memory::IsHandle(address + 92)) memory::OwnerStore(address + 92, 4, v); else guest::StoreU32(base, address + 92, v); }
  uint32_t matrix_5_address() const { return address + 96; }
  uint32_t matrix_5_bits() const { if (auto* d = direct()) return d->matrix_5; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_matrix_5_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix_5 = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
  uint32_t matrix_5() const { if (auto* d = direct()) return d->matrix_5; return uint32_t((memory::IsHandle(address + 96) ? uint32_t(memory::OwnerLoad(address + 96, 4)) : guest::LoadU32(base, address + 96))); }
  void set_matrix_5(uint32_t v) const { if (auto* d = direct()) { d->matrix_5 = v; return; } if (memory::IsHandle(address + 96)) memory::OwnerStore(address + 96, 4, v); else guest::StoreU32(base, address + 96, v); }
};

// allocated; measure: new<196>@XACT3CreateEngine, sub_82118638.r3@+16.+8, AudioManager::GetCategory.r3@+16.+8, AudioManager::SetCategoryVolume.r3@+16.+8, AudioManager::PauseCategory.r3@+16.+8, AudioManager::ResumeCategory.r3@+16.+8, AudioEngine::DoWork.r3.+8, AudioManager@+16.+8
struct XactEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 196;
  // 0x82075360
  template <typename T = XactEngineInterface>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // 0x82075348
  uint32_t vtable2() const { return guest::LoadU32(base, address + 4); }
  void set_vtable2(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t vtable2_address() const { return address + 4; }
  // 1 at first
  uint32_t ref_count() const { return guest::LoadU32(base, address + 8); }
  void set_ref_count(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t ref_count_address() const { return address + 8; }
  uint32_t renderer() const { return guest::LoadU32(base, address + 12); }
  void set_renderer(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t renderer_address() const { return address + 12; }
  // Initialize's parameters' +28
  uint32_t notification_callback() const { return guest::LoadU32(base, address + 16); }
  void set_notification_callback(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t notification_callback_address() const { return address + 16; }
  // the notification callback's thread id (-1)
  uint32_t notification_thread() const { return guest::LoadU32(base, address + 20); }
  void set_notification_thread(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t notification_thread_address() const { return address + 20; }
  // the audio callback's time (the clock + [0x8266CDF4])
  uint32_t callback_time() const { return guest::LoadU32(base, address + 24); }
  void set_callback_time(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t callback_time_address() const { return address + 24; }
  // the cues' list (nodes at cue + 12)
  uint32_t cues() const { return guest::LoadU32(base, address + 28); }
  void set_cues(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t cues_address() const { return address + 28; }
  uint32_t cues_next() const { return guest::LoadU32(base, address + 32); }
  void set_cues_next(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t cues_next_address() const { return address + 32; }
  uint32_t cues_previous() const { return guest::LoadU32(base, address + 36); }
  void set_cues_previous(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t cues_previous_address() const { return address + 36; }
  // the parameters' +0 (probably a look-ahead in ms)
  uint32_t look_ahead() const { return guest::LoadU32(base, address + 40); }
  void set_look_ahead(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t look_ahead_address() const { return address + 40; }
  // the sound banks' list (nodes at bank + 12)
  uint32_t sound_banks() const { return guest::LoadU32(base, address + 44); }
  void set_sound_banks(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t sound_banks_address() const { return address + 44; }
  uint32_t sound_banks_next() const { return guest::LoadU32(base, address + 48); }
  void set_sound_banks_next(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t sound_banks_next_address() const { return address + 48; }
  uint32_t sound_banks_previous() const { return guest::LoadU32(base, address + 52); }
  void set_sound_banks_previous(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t sound_banks_previous_address() const { return address + 52; }
  // the wave banks' list (nodes at bank + 8)
  uint32_t wave_banks() const { return guest::LoadU32(base, address + 56); }
  void set_wave_banks(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t wave_banks_address() const { return address + 56; }
  uint32_t wave_banks_next() const { return guest::LoadU32(base, address + 60); }
  void set_wave_banks_next(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t wave_banks_next_address() const { return address + 60; }
  uint32_t wave_banks_previous() const { return guest::LoadU32(base, address + 64); }
  void set_wave_banks_previous(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t wave_banks_previous_address() const { return address + 64; }
  // set by Initialize
  uint32_t initialized() const { return guest::LoadU32(base, address + 68); }
  void set_initialized(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t initialized_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  // objects to delete in DoWork
  uint32_t deletes() const { return guest::LoadU32(base, address + 76); }
  void set_deletes(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t deletes_address() const { return address + 76; }
  uint32_t deletes_next() const { return guest::LoadU32(base, address + 80); }
  void set_deletes_next(uint32_t v) const { guest::StoreU32(base, address + 80, v); }
  uint32_t deletes_next_address() const { return address + 80; }
  uint32_t deletes_previous() const { return guest::LoadU32(base, address + 84); }
  void set_deletes_previous(uint32_t v) const { guest::StoreU32(base, address + 84, v); }
  uint32_t deletes_previous_address() const { return address + 84; }
  // a list head (not read)
  uint32_t list_88() const { return guest::LoadU32(base, address + 88); }
  void set_list_88(uint32_t v) const { guest::StoreU32(base, address + 88, v); }
  uint32_t list_88_address() const { return address + 88; }
  uint32_t list_88_next() const { return guest::LoadU32(base, address + 92); }
  void set_list_88_next(uint32_t v) const { guest::StoreU32(base, address + 92, v); }
  uint32_t list_88_next_address() const { return address + 92; }
  uint32_t list_88_previous() const { return guest::LoadU32(base, address + 96); }
  void set_list_88_previous(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t list_88_previous_address() const { return address + 96; }
  // the notification registrations
  uint32_t notifications() const { return guest::LoadU32(base, address + 100); }
  void set_notifications(uint32_t v) const { guest::StoreU32(base, address + 100, v); }
  uint32_t notifications_address() const { return address + 100; }
  uint32_t notifications_next() const { return guest::LoadU32(base, address + 104); }
  void set_notifications_next(uint32_t v) const { guest::StoreU32(base, address + 104, v); }
  uint32_t notifications_next_address() const { return address + 104; }
  uint32_t notifications_previous() const { return guest::LoadU32(base, address + 108); }
  void set_notifications_previous(uint32_t v) const { guest::StoreU32(base, address + 108, v); }
  uint32_t notifications_previous_address() const { return address + 108; }
  // objects to delete in DoWork
  uint32_t deletes_2() const { return guest::LoadU32(base, address + 112); }
  void set_deletes_2(uint32_t v) const { guest::StoreU32(base, address + 112, v); }
  uint32_t deletes_2_address() const { return address + 112; }
  uint32_t deletes_2_next() const { return guest::LoadU32(base, address + 116); }
  void set_deletes_2_next(uint32_t v) const { guest::StoreU32(base, address + 116, v); }
  uint32_t deletes_2_next_address() const { return address + 116; }
  uint32_t deletes_2_previous() const { return guest::LoadU32(base, address + 120); }
  void set_deletes_2_previous(uint32_t v) const { guest::StoreU32(base, address + 120, v); }
  uint32_t deletes_2_previous_address() const { return address + 120; }
  uint32_t global_settings() const { return guest::LoadU32(base, address + 124); }
  void set_global_settings(uint32_t v) const { guest::StoreU32(base, address + 124, v); }
  uint32_t global_settings_address() const { return address + 124; }
  // cues waiting on an instance limit (never filled in play)
  uint32_t waiting_cues() const { return guest::LoadU32(base, address + 128); }
  void set_waiting_cues(uint32_t v) const { guest::StoreU32(base, address + 128, v); }
  uint32_t waiting_cues_address() const { return address + 128; }
  uint32_t waiting_cues_next() const { return guest::LoadU32(base, address + 132); }
  void set_waiting_cues_next(uint32_t v) const { guest::StoreU32(base, address + 132, v); }
  uint32_t waiting_cues_next_address() const { return address + 132; }
  uint32_t waiting_cues_previous() const { return guest::LoadU32(base, address + 136); }
  void set_waiting_cues_previous(uint32_t v) const { guest::StoreU32(base, address + 136, v); }
  uint32_t waiting_cues_previous_address() const { return address + 136; }
  uint32_t unknown_140() const { return guest::LoadU32(base, address + 140); }
  void set_unknown_140(uint32_t v) const { guest::StoreU32(base, address + 140, v); }
  uint32_t unknown_140_address() const { return address + 140; }
  // a critical section (RtlInitializeCriticalSection)
  uint32_t lock_address() const { return address + 144; }
  uint32_t start_time() const { return guest::LoadU32(base, address + 172); }
  void set_start_time(uint32_t v) const { guest::StoreU32(base, address + 172, v); }
  uint32_t start_time_address() const { return address + 172; }
  uint32_t unknown_176() const { return guest::LoadU32(base, address + 176); }
  void set_unknown_176(uint32_t v) const { guest::StoreU32(base, address + 176, v); }
  uint32_t unknown_176_address() const { return address + 176; }
  // the audio callback does nothing when set
  uint32_t callback_disabled() const { return guest::LoadU32(base, address + 180); }
  void set_callback_disabled(uint32_t v) const { guest::StoreU32(base, address + 180, v); }
  uint32_t callback_disabled_address() const { return address + 180; }
  // deferred voices to start
  uint32_t deferred_voices() const { return guest::LoadU32(base, address + 184); }
  void set_deferred_voices(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t deferred_voices_address() const { return address + 184; }
  // the parameters' +20 (probably a file I/O callback; 0x82295FC8)
  uint32_t file_callback_0() const { return guest::LoadU32(base, address + 188); }
  void set_file_callback_0(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t file_callback_0_address() const { return address + 188; }
  // the parameters' +24 (0x82295F20)
  uint32_t file_callback_1() const { return guest::LoadU32(base, address + 192); }
  void set_file_callback_1(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t file_callback_1_address() const { return address + 192; }
};

// image; measure: sub_82118638.r3@+16.+8.+0, AudioManager::GetCategory.r3@+16.+8.+0, AudioManager::SetCategoryVolume.r3@+16.+8.+0, AudioManager::PauseCategory.r3@+16.+8.+0, AudioManager::ResumeCategory.r3@+16.+8.+0, AudioEngine::DoWork.r3.+8.+0, AudioManager@+16.+8.+0
struct XactEngineInterface : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // called by sub_8259D370; not read
  uint32_t slot_1() const { return guest::LoadU32(base, address + 4); }
  void set_slot_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t slot_1_address() const { return address + 4; }
  // probably Initialize (sub_8259D1F0)
  uint32_t initialize() const { return guest::LoadU32(base, address + 8); }
  void set_initialize(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t initialize_address() const { return address + 8; }
  // probably DoWork (AudioEngine::DoWork)
  uint32_t do_work() const { return guest::LoadU32(base, address + 16); }
  void set_do_work(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t do_work_address() const { return address + 16; }
  // probably CreateSoundBank
  uint32_t create_sound_bank() const { return guest::LoadU32(base, address + 20); }
  void set_create_sound_bank(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t create_sound_bank_address() const { return address + 20; }
  // probably CreateInMemoryWaveBank
  uint32_t create_in_memory_wave_bank() const { return guest::LoadU32(base, address + 24); }
  void set_create_in_memory_wave_bank(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t create_in_memory_wave_bank_address() const { return address + 24; }
  // probably GetCategory
  uint32_t get_category() const { return guest::LoadU32(base, address + 40); }
  void set_get_category(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t get_category_address() const { return address + 40; }
  // probably SetVolume (a category's)
  uint32_t set_volume() const { return guest::LoadU32(base, address + 48); }
  void set_set_volume(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t set_volume_address() const { return address + 48; }
  // probably Pause (a category's: 1 pause, 0 resume)
  uint32_t pause() const { return guest::LoadU32(base, address + 52); }
  void set_pause(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t pause_address() const { return address + 52; }
};

// allocated; measure: AudioManager.+12
struct AudioManagerOwner : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AudioManagerOwnerVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: AudioManager.+12.+0
struct AudioManagerOwnerVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // slot 1: the object the manager's delegates are registered with
  uint32_t handler_registry() const { return guest::LoadU32(base, address + 4); }
  void set_handler_registry(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t handler_registry_address() const { return address + 4; }
  // slot 2: the object the manager's command (+1584) is queued on
  uint32_t command_queue() const { return guest::LoadU32(base, address + 8); }
  void set_command_queue(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t command_queue_address() const { return address + 8; }
};

// allocated; measure: sub_82119188.r4
struct AudioManagerMessageSource : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  template <typename T = AudioManagerMessageSourceVtable>
  T vtable() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_vtable(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
};

// image; measure: sub_82119188.r4.+0
struct AudioManagerMessageSourceVtable : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // with 8 (probably a subscription)
  uint32_t slot_20() const { return guest::LoadU32(base, address + 20); }
  void set_slot_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t slot_20_address() const { return address + 20; }
  // with 6
  uint32_t slot_24() const { return guest::LoadU32(base, address + 24); }
  void set_slot_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t slot_24_address() const { return address + 24; }
  // with 1
  uint32_t slot_28() const { return guest::LoadU32(base, address + 28); }
  void set_slot_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t slot_28_address() const { return address + 28; }
  // with 7
  uint32_t slot_32() const { return guest::LoadU32(base, address + 32); }
  void set_slot_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t slot_32_address() const { return address + 32; }
};

// embedded; measure: sub_82119188.r5
struct AudioManagerMessage : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // 5 or 6 handled
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// allocated; measure: new<16>@sub_82118720, sub_82118720.r3
#pragma pack(push, 1)
struct AudioManagerDelegateData {
  uint32_t vtable = 0;  // +0
  uint32_t object = 0;  // +4
  uint32_t method = 0;  // +8
  uint32_t method_adjust = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(AudioManagerDelegateData) == 16);
inline constexpr memory::LayoutField kAudioManagerDelegateFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kAudioManagerDelegateLayout = {kAudioManagerDelegateFields, 4};

struct AudioManagerDelegate : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  AudioManagerDelegateData* data() const { return static_cast<AudioManagerDelegateData*>(memory::HandleObject(address)); }
  static AudioManagerDelegate New(uint8_t* base) {
    return AudioManagerDelegate(base, memory::MakeHandle(new AudioManagerDelegateData(), sizeof(AudioManagerDelegateData), "AudioManagerDelegate", &kAudioManagerDelegateLayout));
  }
  template <uint32_t Stride = kSize>
  static AudioManagerDelegate NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(AudioManagerDelegateData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<AudioManagerDelegateData*>(::operator new(sizeof(AudioManagerDelegateData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) AudioManagerDelegateData();
    return AudioManagerDelegate(base, memory::MakeHandle(p, sizeof(AudioManagerDelegateData) * count, "AudioManagerDelegate", &kAudioManagerDelegateLayout, sizeof(AudioManagerDelegateData)));
  }
  AudioManagerDelegate at(uint32_t i) const { return AudioManagerDelegate(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  AudioManagerDelegateData* direct() const { return static_cast<AudioManagerDelegateData*>(memory::HandleOwn(address, &kAudioManagerDelegateLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kAudioManagerDelegateLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kAudioManagerDelegateLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 0x8200833C
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the manager
  uint32_t object_address() const { return address + 4; }
  uint32_t object_bits() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object_bits(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t object() const { if (auto* d = direct()) return d->object; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_object(uint32_t v) const { if (auto* d = direct()) { d->object = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  // the member pointer's function
  uint32_t method_address() const { return address + 8; }
  uint32_t method_bits() const { if (auto* d = direct()) return d->method; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_method_bits(uint32_t v) const { if (auto* d = direct()) { d->method = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t method() const { if (auto* d = direct()) return d->method; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_method(uint32_t v) const { if (auto* d = direct()) { d->method = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the member pointer's this adjustment
  uint32_t method_adjust_address() const { return address + 12; }
  uint32_t method_adjust_bits() const { if (auto* d = direct()) return d->method_adjust; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_method_adjust_bits(uint32_t v) const { if (auto* d = direct()) { d->method_adjust = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t method_adjust() const { if (auto* d = direct()) return d->method_adjust; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_method_adjust(uint32_t v) const { if (auto* d = direct()) { d->method_adjust = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
};

// embedded; measure: AudioManager::RemoveStoppedSounds.r3@+1528
#pragma pack(push, 1)
struct SoundListData {
  uint32_t allocator = 0;  // +0
  uint32_t head = 0;  // +4
  uint32_t size = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(SoundListData) == 12);
inline constexpr memory::LayoutField kSoundListFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kSoundListLayout = {kSoundListFields, 3};

struct SoundList : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  SoundListData* data() const { return static_cast<SoundListData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static SoundList New(uint8_t* base) {
    return SoundList(base, memory::MakeHandle(new SoundListData(), sizeof(SoundListData), "SoundList", &kSoundListLayout));
  }
  template <uint32_t Stride = kSize>
  static SoundList NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(SoundListData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<SoundListData*>(::operator new(sizeof(SoundListData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) SoundListData();
    return SoundList(base, memory::MakeHandle(p, sizeof(SoundListData) * count, "SoundList", &kSoundListLayout, sizeof(SoundListData)));
  }
  SoundList at(uint32_t i) const { return SoundList(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  SoundListData* direct() const { return static_cast<SoundListData*>(memory::HandleOwn(address, &kSoundListLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kSoundListLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kSoundListLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // probably the allocator word
  uint32_t allocator_address() const { return address + 0; }
  uint32_t allocator_bits() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator_bits(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t allocator() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // the sentinel node
  uint32_t head_address() const { return address + 4; }
  uint32_t head_bits() const { if (auto* d = direct()) return d->head; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_head_bits(uint32_t v) const { if (auto* d = direct()) { d->head = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  template <typename T = SoundListNode>
  T head() const { return T(base, head_bits()); }
  void set_head(const View& w) const { set_head_bits(w.address); }
  void set_head(uint32_t v) const { set_head_bits(v); }
  uint32_t size_address() const { return address + 8; }
  uint32_t size_bits() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size_bits(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t size() const { if (auto* d = direct()) return d->size; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_size(uint32_t v) const { if (auto* d = direct()) { d->size = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
};

// allocated; measure: AudioManager.+1532, AudioManager.+1532.+0, sub_82119018.r3.+1532, AudioManager::RemoveStoppedSounds.r3@+1528.+4, AudioManager::RemoveStoppedSounds.r3@+1528.+4.+0
struct SoundListNode : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  template <typename T = SoundListNode>
  T next() const { return T(base, guest::LoadU32(base, address + 0)); }
  void set_next(const View& v) const { guest::StoreU32(base, address + 0, v.address); }
  void set_next(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t next_address() const { return address + 0; }
  template <typename T = SoundListNode>
  T previous() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_previous(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_previous(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t previous_address() const { return address + 4; }
  template <typename T = PlayingSound>
  T sound() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_sound(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_sound(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t sound_address() const { return address + 8; }
};

// embedded; measure: sub_82118F50.r3
struct SoundForEachResult : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t function() const { return guest::LoadU32(base, address + 0); }
  void set_function(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t function_address() const { return address + 0; }
};

// embedded; measure: WordVector
#pragma pack(push, 1)
struct WordVectorData {
  uint32_t allocator = 0;  // +0
  uint32_t first = 0;  // +4
  uint32_t last = 0;  // +8
  uint32_t end = 0;  // +12
};
#pragma pack(pop)
static_assert(sizeof(WordVectorData) == 16);
inline constexpr memory::LayoutField kWordVectorFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}};
inline constexpr memory::Layout kWordVectorLayout = {kWordVectorFields, 4};

struct WordVector : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  WordVectorData* data() const { return static_cast<WordVectorData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static WordVector New(uint8_t* base) {
    return WordVector(base, memory::MakeHandle(new WordVectorData(), sizeof(WordVectorData), "WordVector", &kWordVectorLayout));
  }
  template <uint32_t Stride = kSize>
  static WordVector NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(WordVectorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<WordVectorData*>(::operator new(sizeof(WordVectorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) WordVectorData();
    return WordVector(base, memory::MakeHandle(p, sizeof(WordVectorData) * count, "WordVector", &kWordVectorLayout, sizeof(WordVectorData)));
  }
  WordVector at(uint32_t i) const { return WordVector(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  WordVectorData* direct() const { return static_cast<WordVectorData*>(memory::HandleOwn(address, &kWordVectorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kWordVectorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kWordVectorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // probably the allocator word (never written here)
  uint32_t allocator_address() const { return address + 0; }
  uint32_t allocator_bits() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator_bits(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  uint32_t allocator() const { if (auto* d = direct()) return d->allocator; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(base, address + 0))); }
  void set_allocator(uint32_t v) const { if (auto* d = direct()) { d->allocator = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(base, address + 0, v); }
  // read as a doubleword (probably a measure fault: an iterator pair joined)
  uint32_t fault_0_address() const { return address + 0; }
  uint64_t fault_0_bits() const { if (auto* d = direct()) return uint64_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kWordVectorLayout, 0, 8)); return uint64_t((memory::IsHandle(address + 0) ? uint64_t(memory::OwnerLoad(address + 0, 8)) : guest::LoadU64(base, address + 0))); }
  void set_fault_0_bits(uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kWordVectorLayout, 0, 8, v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 8, v); else guest::StoreU64(base, address + 0, v); }
  uint64_t fault_0() const { return uint64_t(fault_0_bits()); }
  void set_fault_0(uint64_t v) const { set_fault_0_bits(uint64_t(v)); }
  // MessageVector::Allocate's block (cameras' NewAny_MessageVector_Allocate)
  uint32_t first_address() const { return address + 4; }
  uint32_t first_bits() const { if (auto* d = direct()) return d->first; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_first_bits(uint32_t v) const { if (auto* d = direct()) { d->first = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t first() const { if (auto* d = direct()) return d->first; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(base, address + 4))); }
  void set_first(uint32_t v) const { if (auto* d = direct()) { d->first = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(base, address + 4, v); }
  uint32_t last_address() const { return address + 8; }
  uint32_t last_bits() const { if (auto* d = direct()) return d->last; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_last_bits(uint32_t v) const { if (auto* d = direct()) { d->last = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  uint32_t last() const { if (auto* d = direct()) return d->last; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(base, address + 8))); }
  void set_last(uint32_t v) const { if (auto* d = direct()) { d->last = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(base, address + 8, v); }
  // the capacity's end
  uint32_t end_address() const { return address + 12; }
  uint32_t end_bits() const { if (auto* d = direct()) return d->end; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_end_bits(uint32_t v) const { if (auto* d = direct()) { d->end = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
  uint32_t end() const { if (auto* d = direct()) return d->end; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(base, address + 12))); }
  void set_end(uint32_t v) const { if (auto* d = direct()) { d->end = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(base, address + 12, v); }
};

// embedded; measure: AudioManager::LoadBank.r4, AudioManager::ReleaseBank.r4, AudioManager::IsBankLoading.r4, AudioManager::IsBankSlotEmpty.r4, AudioManager::GetSoundBank.r4, AudioManager::PlayCue.r4, Sound::Sound.r4, Sound::IsBank.r4, AudioManager::FindFreeBankSlot.r4, AudioManager::GetCueIndex.r4, AudioManager::IsBankLoaded.r4, AudioManager::PlayHandle.r4
struct SoundCuesBankIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: AudioManager::PlayCue.r5, Sound::Sound.r5, Sound::IsCue.r4, AudioManager::PlayHandle.r5
struct SoundCuesCueIndex : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
};

// embedded; measure: AudioManager::LoadBank.r7, AudioManager::LoadBank.r8
struct BankFileId : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
};

// embedded; measure: BankPair::BankPair.r5
struct BankSkipWaveArg : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(base, address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: AudioManager::AudioManager.r6
struct AudioManagerBankCount : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
};

// embedded; measure: WordVector::Fill.r5
struct WordVectorFillValue : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: Sound::Set3D.r4
struct Sound3DArg : View {
  using View::View;
  static constexpr uint32_t kSize = 1;
  uint8_t value() const { return guest::LoadU8(base, address + 0); }
  void set_value(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: AudioEngine::SetCategoryVolume.r4, AudioEngine::PauseCategory.r4, AudioEngine::ResumeCategory.r4
struct AudioCategoryArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t category() const { return guest::LoadU32(base, address + 0); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t category_address() const { return address + 0; }
};

// embedded; measure: AudioEngine::SetCategoryVolume.r5
struct AudioVolumeArg : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  float volume() const { return guest::LoadFloat(base, address + 0); }
  void set_volume(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t volume_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+16@+12
struct AudioEngineAt12 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(base, address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  float unknown_12() const { return guest::LoadFloat(base, address + 12); }
  void set_unknown_12(float v) const { guest::StoreFloat(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: AudioManager.+1544.+0@+8
struct PlayingSoundAt8 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: BankPair@+72
struct SoundBankPairAt72 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: BankPair@+76
struct SoundBankPairAt76 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: Sound@+4
struct PlayingSoundAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: AudioManager@+1512
struct WordVectorAt1512 : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
};

// embedded; measure: sub_82090858.r3@-12
struct SoundBankPairOuter12 : View {
  using View::View;
  static constexpr uint32_t kSize = 80;
  uint32_t unknown_76() const { return guest::LoadU32(base, address + 76); }
  void set_unknown_76(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
};

// The image words docs/functions/sound-cues.md's code uses.
struct SoundCuesGlobals {
  // 0.0 (copied as a float)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // 1.0 (copied as a float)
  static uint32_t float_one(uint8_t* base) { return guest::LoadU32(base, 0x82000A30u); }
  static void set_float_one(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A30u, v); }
  static constexpr uint32_t float_one_address() { return 0x82000A30u; }
  // the listener's first position's z
  static float listener_start_z(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_listener_start_z(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t listener_start_z_address() { return 0x82001FD8u; }
  // the listener's +48 at first
  static float listener_start_param_0(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_listener_start_param_0(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t listener_start_param_0_address() { return 0x82002A0Cu; }
  // the listener's +52 at first
  static float listener_start_param_1(uint8_t* base) { return guest::LoadFloat(base, 0x820027A4u); }
  static void set_listener_start_param_1(uint8_t* base, float v) { guest::StoreFloat(base, 0x820027A4u, v); }
  static constexpr uint32_t listener_start_param_1_address() { return 0x820027A4u; }
  // AudioEngine::Start3D's speed of sound
  static float speed_of_sound(uint8_t* base) { return guest::LoadFloat(base, 0x8207145Cu); }
  static void set_speed_of_sound(uint8_t* base, float v) { guest::StoreFloat(base, 0x8207145Cu, v); }
  static constexpr uint32_t speed_of_sound_address() { return 0x8207145Cu; }
  // bank pair 2's wave bank buffer (3 MiB, SoundGlobals::Init's)
  static uint32_t bank2_wave_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C88u); }
  static void set_bank2_wave_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C88u, v); }
  static constexpr uint32_t bank2_wave_buffer_address() { return 0x826C5C88u; }
  // bank pair 2's sound bank buffer (512 bytes)
  static uint32_t bank2_sound_buffer(uint8_t* base) { return guest::LoadU32(base, 0x826C5C8Cu); }
  static void set_bank2_sound_buffer(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C8Cu, v); }
  static constexpr uint32_t bank2_sound_buffer_address() { return 0x826C5C8Cu; }
  // XACT's engine, made once by XACT3CreateEngine
  template <typename T = XactEngine>
  static T xact_engine(uint8_t* base) { return T(base, guest::LoadU32(base, 0x82971478u)); }
  static void set_xact_engine(uint8_t* base, const View& v) { guest::StoreU32(base, 0x82971478u, v.address); }
  static void set_xact_engine(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82971478u, v); }
  static constexpr uint32_t xact_engine_address() { return 0x82971478u; }
};

}  // namespace schema
