// The views of docs/functions/car-sounds.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/car-sounds.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: CarSound, CarSounds, CarSound::UpdateVoices.r3, CarSound::Pause.r3, CarSound::Clear.r3, CarSound::Resume.r3
#pragma pack(push, 1)
struct CarSoundData {
  uint32_t tunnel_reverb = 0;  // +0
  float x3d_listener_front_x = 0;  // +4
  float x3d_listener_front_y = 0;  // +8
  float x3d_listener_front_z = 0;  // +12
  float x3d_listener_top_x = 0;  // +16
  float x3d_listener_top_y = 0;  // +20
  float x3d_listener_top_z = 0;  // +24
  float x3d_listener_position_x = 0;  // +28
  float x3d_listener_position_y = 0;  // +32
  float x3d_listener_position_z = 0;  // +36
  float x3d_listener_velocity_x = 0;  // +40
  float x3d_listener_velocity_y = 0;  // +44
  float x3d_listener_velocity_z = 0;  // +48
  uint8_t emitter[92] = {};  // +52, guest order
  uint8_t cone[32] = {};  // +144, guest order
  uint8_t dsp[48] = {};  // +176, guest order
  uint8_t reverb_map[48] = {};  // +224, guest order
  uint8_t output_map[48] = {};  // +272, guest order
  uint8_t reverb_map_count = 0;  // +320
  uint8_t pad_321[3] = {};
  uint32_t reverb_map_pointer = 0;  // +324
  uint8_t output_map_count = 0;  // +328
  uint8_t pad_329[3] = {};
  uint32_t output_map_pointer = 0;  // +332
  uint8_t sends_0_kind = 0;  // +336
  uint8_t pad_337[3] = {};
  uint32_t sends_0 = 0;  // +340
  uint8_t sends_1_kind = 0;  // +344
  uint8_t pad_345[3] = {};
  uint32_t sends_1 = 0;  // +348
  uint64_t output = 0;  // +352
  uint8_t pad_360[8] = {};
  float emitter_position_x = 0;  // +368
  float emitter_position_y = 0;  // +372
  float emitter_position_z = 0;  // +376
  float emitter_position_w = 0;  // +380
  float listener_position_x = 0;  // +384
  float listener_position_y = 0;  // +388
  float listener_position_z = 0;  // +392
  float listener_position_w = 0;  // +396
  uint32_t vehicle = 0;  // +400
  uint8_t index = 0;  // +404
  uint8_t pad_405[3] = {};
  uint8_t engine[176] = {};  // +408, guest order
  uint8_t hits[68] = {};  // +584, guest order
  uint8_t tires[84] = {};  // +652, guest order
  uint8_t gear[56] = {};  // +736, guest order
  uint8_t roll[48] = {};  // +792, guest order
  uint8_t events[48] = {};  // +840, guest order
  uint8_t fade[64] = {};  // +888, guest order
  uint8_t turbo[64] = {};  // +952, guest order
  uint8_t wind[56] = {};  // +1016, guest order
  uint8_t horn[64] = {};  // +1072, guest order
  uint8_t pass_by[56] = {};  // +1136, guest order
  uint8_t volume_curve_points[48] = {};  // +1192, guest order
  uint8_t volume_curve[8] = {};  // +1240, guest order
  uint32_t live = 0;  // +1248
  float engine_gain = 0;  // +1252
  float last_doppler = 0;  // +1256
  uint32_t passed_close = 0;  // +1260
};
#pragma pack(pop)
static_assert(sizeof(CarSoundData) == 1264);
inline constexpr memory::LayoutField kCarSoundFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}, {324, 4}, {332, 4}, {340, 4}, {348, 4}, {352, 8}, {368, 4}, {372, 4}, {376, 4}, {380, 4}, {384, 4}, {388, 4}, {392, 4}, {396, 4}, {400, 4}, {408, 4}, {412, 4}, {416, 4}, {420, 4}, {424, 4}, {452, 4}, {472, 4}, {492, 4}, {496, 4}, {500, 4}, {504, 4}, {508, 4}, {536, 4}, {556, 4}, {576, 4}, {580, 4}, {584, 4}, {604, 4}, {624, 4}, {632, 4}, {636, 4}, {640, 4}, {644, 4}, {648, 4}, {652, 4}, {656, 4}, {664, 4}, {668, 4}, {676, 4}, {696, 4}, {716, 4}, {722, 2}, {724, 4}, {728, 2}, {732, 4}, {736, 4}, {756, 4}, {776, 4}, {784, 4}, {788, 4}, {792, 4}, {812, 4}, {832, 4}, {840, 4}, {860, 4}, {880, 4}, {888, 4}, {908, 4}, {928, 4}, {936, 4}, {940, 4}, {944, 4}, {948, 4}, {952, 4}, {972, 4}, {992, 4}, {1000, 4}, {1004, 4}, {1008, 2}, {1012, 4}, {1016, 4}, {1036, 4}, {1056, 4}, {1064, 4}, {1068, 4}, {1072, 4}, {1092, 4}, {1112, 4}, {1120, 4}, {1124, 4}, {1128, 4}, {1132, 4}, {1136, 4}, {1156, 4}, {1176, 4}, {1184, 4}, {1188, 4}, {1240, 4}, {1244, 4}, {1248, 4}, {1252, 4}, {1256, 4}, {1260, 4}};
inline constexpr memory::Layout kCarSoundLayout = {kCarSoundFields, 153};

struct CarSound : View {
  using View::View;
  static constexpr uint32_t kSize = 1264;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundData* data() const { return static_cast<CarSoundData*>(memory::HandleObject(address)); }
  static CarSound New() {
    return CarSound(memory::MakeHandle(new CarSoundData(), sizeof(CarSoundData), "CarSound", &kCarSoundLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSound NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundData*>(::operator new(sizeof(CarSoundData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundData();
    return CarSound(memory::MakeHandle(p, sizeof(CarSoundData) * count, "CarSound", &kCarSoundLayout, sizeof(CarSoundData)));
  }
  CarSound at(uint32_t i) const { return CarSound(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundData* direct() const { return static_cast<CarSoundData*>(memory::HandleOwn(address, &kCarSoundLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the first element only: CarSoundArray's count (7), read as the array's count (no member: outside the sound; CarSoundArray(array_count_address()).count())
  uint32_t array_count_address() const { return address + -16; }
  uint8_t* array_count() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + -16 : guest::Pointer(address + -16); }
  // the submix's reverb state: 1 the block 0x826166F8 set (the vehicle's +5314 bit 1, probably a tunnel), 0 the block 0x82616638
  uint32_t tunnel_reverb_address() const { return address + 0; }
  uint32_t tunnel_reverb_bits() const { if (auto* d = direct()) return d->tunnel_reverb; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_tunnel_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->tunnel_reverb = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t tunnel_reverb() const { if (auto* d = direct()) return d->tunnel_reverb; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_tunnel_reverb(uint32_t v) const { if (auto* d = direct()) { d->tunnel_reverb = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // an X3DAudioListener; its cone pointer is the emitter's +0 (+52). Not an embed (the emitter's cone would be a second layout entry at +52): its floats below, which X3DAudioListener's dual view reads
  uint32_t listener_address() const { return address + 4; }
  uint8_t* listener() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4 : guest::Pointer(address + 4); }
  uint32_t x3d_listener_front_x_address() const { return address + 4; }
  uint32_t x3d_listener_front_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_front_x); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_x3d_listener_front_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_front_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float x3d_listener_front_x() const { return std::bit_cast<float>(x3d_listener_front_x_bits()); }
  void set_x3d_listener_front_x(float v) const { set_x3d_listener_front_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_front_y_address() const { return address + 8; }
  uint32_t x3d_listener_front_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_front_y); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_x3d_listener_front_y_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_front_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float x3d_listener_front_y() const { return std::bit_cast<float>(x3d_listener_front_y_bits()); }
  void set_x3d_listener_front_y(float v) const { set_x3d_listener_front_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_front_z_address() const { return address + 12; }
  uint32_t x3d_listener_front_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_front_z); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_x3d_listener_front_z_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_front_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float x3d_listener_front_z() const { return std::bit_cast<float>(x3d_listener_front_z_bits()); }
  void set_x3d_listener_front_z(float v) const { set_x3d_listener_front_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_top_x_address() const { return address + 16; }
  uint32_t x3d_listener_top_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_top_x); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_x3d_listener_top_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_top_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float x3d_listener_top_x() const { return std::bit_cast<float>(x3d_listener_top_x_bits()); }
  void set_x3d_listener_top_x(float v) const { set_x3d_listener_top_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_top_y_address() const { return address + 20; }
  uint32_t x3d_listener_top_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_top_y); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_x3d_listener_top_y_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_top_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float x3d_listener_top_y() const { return std::bit_cast<float>(x3d_listener_top_y_bits()); }
  void set_x3d_listener_top_y(float v) const { set_x3d_listener_top_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_top_z_address() const { return address + 24; }
  uint32_t x3d_listener_top_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_top_z); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_x3d_listener_top_z_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_top_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float x3d_listener_top_z() const { return std::bit_cast<float>(x3d_listener_top_z_bits()); }
  void set_x3d_listener_top_z(float v) const { set_x3d_listener_top_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_position_x_address() const { return address + 28; }
  uint32_t x3d_listener_position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_position_x); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_x3d_listener_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float x3d_listener_position_x() const { return std::bit_cast<float>(x3d_listener_position_x_bits()); }
  void set_x3d_listener_position_x(float v) const { set_x3d_listener_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_position_y_address() const { return address + 32; }
  uint32_t x3d_listener_position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_position_y); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_x3d_listener_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float x3d_listener_position_y() const { return std::bit_cast<float>(x3d_listener_position_y_bits()); }
  void set_x3d_listener_position_y(float v) const { set_x3d_listener_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_position_z_address() const { return address + 36; }
  uint32_t x3d_listener_position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_position_z); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_x3d_listener_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float x3d_listener_position_z() const { return std::bit_cast<float>(x3d_listener_position_z_bits()); }
  void set_x3d_listener_position_z(float v) const { set_x3d_listener_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_velocity_x_address() const { return address + 40; }
  uint32_t x3d_listener_velocity_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_velocity_x); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_x3d_listener_velocity_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_velocity_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float x3d_listener_velocity_x() const { return std::bit_cast<float>(x3d_listener_velocity_x_bits()); }
  void set_x3d_listener_velocity_x(float v) const { set_x3d_listener_velocity_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_velocity_y_address() const { return address + 44; }
  uint32_t x3d_listener_velocity_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_velocity_y); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_x3d_listener_velocity_y_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_velocity_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float x3d_listener_velocity_y() const { return std::bit_cast<float>(x3d_listener_velocity_y_bits()); }
  void set_x3d_listener_velocity_y(float v) const { set_x3d_listener_velocity_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t x3d_listener_velocity_z_address() const { return address + 48; }
  uint32_t x3d_listener_velocity_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x3d_listener_velocity_z); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_x3d_listener_velocity_z_bits(uint32_t v) const { if (auto* d = direct()) { d->x3d_listener_velocity_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float x3d_listener_velocity_z() const { return std::bit_cast<float>(x3d_listener_velocity_z_bits()); }
  void set_x3d_listener_velocity_z(float v) const { set_x3d_listener_velocity_z_bits(std::bit_cast<uint32_t>(v)); }
  // an X3DAudioEmitter (its cone pointer +52 points at +144)
  uint32_t emitter_address() const { return address + 52; }
  uint8_t* emitter() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 52 : guest::Pointer(address + 52); }
  template <typename T = X3DAudioEmitter>
  T emitter_object() const { return T(address + 52); }
  // an X3DAudioCone
  uint32_t cone_address() const { return address + 144; }
  uint8_t* cone() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 144 : guest::Pointer(address + 144); }
  template <typename T = X3DAudioCone>
  T cone_object() const { return T(address + 144); }
  // an X3DAudioDspSettings: +176 the matrix (24 bytes from operator new[]), +184 1 source, +188 6 destination channels, +200 the reverb level, +204 the Doppler factor
  uint32_t dsp_address() const { return address + 176; }
  uint8_t* dsp() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 176 : guest::Pointer(address + 176); }
  template <typename T = X3DAudioDspSettings>
  T dsp_object() const { return T(address + 176); }
  // six {channel byte, gain float} pairs: the reverb level times the volume
  uint32_t reverb_map_address() const { return address + 224; }
  uint8_t* reverb_map() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 224 : guest::Pointer(address + 224); }
  // six {channel byte, gain float} pairs: the matrix times the volume
  uint32_t output_map_address() const { return address + 272; }
  uint8_t* output_map() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 272 : guest::Pointer(address + 272); }
  // 6
  uint32_t reverb_map_count_address() const { return address + 320; }
  uint8_t reverb_map_count_bits() const { if (auto* d = direct()) return d->reverb_map_count; return uint8_t((memory::IsHandle(address + 320) ? uint8_t(memory::OwnerLoad(address + 320, 1)) : guest::LoadU8(address + 320))); }
  void set_reverb_map_count_bits(uint8_t v) const { if (auto* d = direct()) { d->reverb_map_count = v; return; } if (memory::IsHandle(address + 320)) memory::OwnerStore(address + 320, 1, v); else guest::StoreU8(address + 320, v); }
  uint8_t reverb_map_count() const { if (auto* d = direct()) return d->reverb_map_count; return uint8_t((memory::IsHandle(address + 320) ? uint8_t(memory::OwnerLoad(address + 320, 1)) : guest::LoadU8(address + 320))); }
  void set_reverb_map_count(uint8_t v) const { if (auto* d = direct()) { d->reverb_map_count = v; return; } if (memory::IsHandle(address + 320)) memory::OwnerStore(address + 320, 1, v); else guest::StoreU8(address + 320, v); }
  // +224
  uint32_t reverb_map_pointer_address() const { return address + 324; }
  uint32_t reverb_map_pointer_bits() const { if (auto* d = direct()) return d->reverb_map_pointer; return uint32_t((memory::IsHandle(address + 324) ? uint32_t(memory::OwnerLoad(address + 324, 4)) : guest::LoadU32(address + 324))); }
  void set_reverb_map_pointer_bits(uint32_t v) const { if (auto* d = direct()) { d->reverb_map_pointer = v; return; } if (memory::IsHandle(address + 324)) memory::OwnerStore(address + 324, 4, v); else guest::StoreU32(address + 324, v); }
  uint32_t reverb_map_pointer() const { if (auto* d = direct()) return d->reverb_map_pointer; return uint32_t((memory::IsHandle(address + 324) ? uint32_t(memory::OwnerLoad(address + 324, 4)) : guest::LoadU32(address + 324))); }
  void set_reverb_map_pointer(uint32_t v) const { if (auto* d = direct()) { d->reverb_map_pointer = v; return; } if (memory::IsHandle(address + 324)) memory::OwnerStore(address + 324, 4, v); else guest::StoreU32(address + 324, v); }
  // 6
  uint32_t output_map_count_address() const { return address + 328; }
  uint8_t output_map_count_bits() const { if (auto* d = direct()) return d->output_map_count; return uint8_t((memory::IsHandle(address + 328) ? uint8_t(memory::OwnerLoad(address + 328, 1)) : guest::LoadU8(address + 328))); }
  void set_output_map_count_bits(uint8_t v) const { if (auto* d = direct()) { d->output_map_count = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 1, v); else guest::StoreU8(address + 328, v); }
  uint8_t output_map_count() const { if (auto* d = direct()) return d->output_map_count; return uint8_t((memory::IsHandle(address + 328) ? uint8_t(memory::OwnerLoad(address + 328, 1)) : guest::LoadU8(address + 328))); }
  void set_output_map_count(uint8_t v) const { if (auto* d = direct()) { d->output_map_count = v; return; } if (memory::IsHandle(address + 328)) memory::OwnerStore(address + 328, 1, v); else guest::StoreU8(address + 328, v); }
  // +272
  uint32_t output_map_pointer_address() const { return address + 332; }
  uint32_t output_map_pointer_bits() const { if (auto* d = direct()) return d->output_map_pointer; return uint32_t((memory::IsHandle(address + 332) ? uint32_t(memory::OwnerLoad(address + 332, 4)) : guest::LoadU32(address + 332))); }
  void set_output_map_pointer_bits(uint32_t v) const { if (auto* d = direct()) { d->output_map_pointer = v; return; } if (memory::IsHandle(address + 332)) memory::OwnerStore(address + 332, 4, v); else guest::StoreU32(address + 332, v); }
  uint32_t output_map_pointer() const { if (auto* d = direct()) return d->output_map_pointer; return uint32_t((memory::IsHandle(address + 332) ? uint32_t(memory::OwnerLoad(address + 332, 4)) : guest::LoadU32(address + 332))); }
  void set_output_map_pointer(uint32_t v) const { if (auto* d = direct()) { d->output_map_pointer = v; return; } if (memory::IsHandle(address + 332)) memory::OwnerStore(address + 332, 4, v); else guest::StoreU32(address + 332, v); }
  // 0
  uint32_t sends_0_kind_address() const { return address + 336; }
  uint8_t sends_0_kind_bits() const { if (auto* d = direct()) return d->sends_0_kind; return uint8_t((memory::IsHandle(address + 336) ? uint8_t(memory::OwnerLoad(address + 336, 1)) : guest::LoadU8(address + 336))); }
  void set_sends_0_kind_bits(uint8_t v) const { if (auto* d = direct()) { d->sends_0_kind = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 1, v); else guest::StoreU8(address + 336, v); }
  uint8_t sends_0_kind() const { if (auto* d = direct()) return d->sends_0_kind; return uint8_t((memory::IsHandle(address + 336) ? uint8_t(memory::OwnerLoad(address + 336, 1)) : guest::LoadU8(address + 336))); }
  void set_sends_0_kind(uint8_t v) const { if (auto* d = direct()) { d->sends_0_kind = v; return; } if (memory::IsHandle(address + 336)) memory::OwnerStore(address + 336, 1, v); else guest::StoreU8(address + 336, v); }
  // +328
  uint32_t sends_0_address() const { return address + 340; }
  uint32_t sends_0_bits() const { if (auto* d = direct()) return d->sends_0; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(address + 340))); }
  void set_sends_0_bits(uint32_t v) const { if (auto* d = direct()) { d->sends_0 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(address + 340, v); }
  uint32_t sends_0() const { if (auto* d = direct()) return d->sends_0; return uint32_t((memory::IsHandle(address + 340) ? uint32_t(memory::OwnerLoad(address + 340, 4)) : guest::LoadU32(address + 340))); }
  void set_sends_0(uint32_t v) const { if (auto* d = direct()) { d->sends_0 = v; return; } if (memory::IsHandle(address + 340)) memory::OwnerStore(address + 340, 4, v); else guest::StoreU32(address + 340, v); }
  // 1
  uint32_t sends_1_kind_address() const { return address + 344; }
  uint8_t sends_1_kind_bits() const { if (auto* d = direct()) return d->sends_1_kind; return uint8_t((memory::IsHandle(address + 344) ? uint8_t(memory::OwnerLoad(address + 344, 1)) : guest::LoadU8(address + 344))); }
  void set_sends_1_kind_bits(uint8_t v) const { if (auto* d = direct()) { d->sends_1_kind = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 1, v); else guest::StoreU8(address + 344, v); }
  uint8_t sends_1_kind() const { if (auto* d = direct()) return d->sends_1_kind; return uint8_t((memory::IsHandle(address + 344) ? uint8_t(memory::OwnerLoad(address + 344, 1)) : guest::LoadU8(address + 344))); }
  void set_sends_1_kind(uint8_t v) const { if (auto* d = direct()) { d->sends_1_kind = v; return; } if (memory::IsHandle(address + 344)) memory::OwnerStore(address + 344, 1, v); else guest::StoreU8(address + 344, v); }
  // +320
  uint32_t sends_1_address() const { return address + 348; }
  uint32_t sends_1_bits() const { if (auto* d = direct()) return d->sends_1; return uint32_t((memory::IsHandle(address + 348) ? uint32_t(memory::OwnerLoad(address + 348, 4)) : guest::LoadU32(address + 348))); }
  void set_sends_1_bits(uint32_t v) const { if (auto* d = direct()) { d->sends_1 = v; return; } if (memory::IsHandle(address + 348)) memory::OwnerStore(address + 348, 4, v); else guest::StoreU32(address + 348, v); }
  uint32_t sends_1() const { if (auto* d = direct()) return d->sends_1; return uint32_t((memory::IsHandle(address + 348) ? uint32_t(memory::OwnerLoad(address + 348, 4)) : guest::LoadU32(address + 348))); }
  void set_sends_1(uint32_t v) const { if (auto* d = direct()) { d->sends_1 = v; return; } if (memory::IsHandle(address + 348)) memory::OwnerStore(address + 348, 4, v); else guest::StoreU32(address + 348, v); }
  // the DSP output: the 8 bytes the parts' updates are given as r5 (+352 2, +356 +336)
  uint32_t output_address() const { return address + 352; }
  uint64_t output_bits() const { if (auto* d = direct()) return d->output; return uint64_t((memory::IsHandle(address + 352) ? uint64_t(memory::OwnerLoad(address + 352, 8)) : guest::LoadU64(address + 352))); }
  void set_output_bits(uint64_t v) const { if (auto* d = direct()) { d->output = v; return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 8, v); else guest::StoreU64(address + 352, v); }
  uint64_t output() const { if (auto* d = direct()) return d->output; return uint64_t((memory::IsHandle(address + 352) ? uint64_t(memory::OwnerLoad(address + 352, 8)) : guest::LoadU64(address + 352))); }
  void set_output(uint64_t v) const { if (auto* d = direct()) { d->output = v; return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 8, v); else guest::StoreU64(address + 352, v); }
  // 2
  uint32_t output_count_address() const { return address + 352; }
  uint8_t output_count_bits() const { if (auto* d = direct()) return uint8_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundLayout, 352, 1)); return uint8_t((memory::IsHandle(address + 352) ? uint8_t(memory::OwnerLoad(address + 352, 1)) : guest::LoadU8(address + 352))); }
  void set_output_count_bits(uint8_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundLayout, 352, 1, v); return; } if (memory::IsHandle(address + 352)) memory::OwnerStore(address + 352, 1, v); else guest::StoreU8(address + 352, v); }
  uint8_t output_count() const { return uint8_t(output_count_bits()); }
  void set_output_count(uint8_t v) const { set_output_count_bits(uint8_t(v)); }
  // +336
  uint32_t output_sends_address() const { return address + 356; }
  uint32_t output_sends_bits() const { if (auto* d = direct()) return uint32_t(memory::LoadOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundLayout, 356, 4)); return uint32_t((memory::IsHandle(address + 356) ? uint32_t(memory::OwnerLoad(address + 356, 4)) : guest::LoadU32(address + 356))); }
  void set_output_sends_bits(uint32_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundLayout, 356, 4, v); return; } if (memory::IsHandle(address + 356)) memory::OwnerStore(address + 356, 4, v); else guest::StoreU32(address + 356, v); }
  uint32_t output_sends() const { return uint32_t(output_sends_bits()); }
  void set_output_sends(uint32_t v) const { set_output_sends_bits(uint32_t(v)); }
  // the emitter's position as a vector (x, y, z, w; stvx and lvx), last tick's for the velocity
  uint32_t emitter_position_x_address() const { return address + 368; }
  uint32_t emitter_position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_position_x); return uint32_t((memory::IsHandle(address + 368) ? uint32_t(memory::OwnerLoad(address + 368, 4)) : guest::LoadU32(address + 368))); }
  void set_emitter_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 368)) memory::OwnerStore(address + 368, 4, v); else guest::StoreU32(address + 368, v); }
  float emitter_position_x() const { return std::bit_cast<float>(emitter_position_x_bits()); }
  void set_emitter_position_x(float v) const { set_emitter_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t emitter_position_y_address() const { return address + 372; }
  uint32_t emitter_position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_position_y); return uint32_t((memory::IsHandle(address + 372) ? uint32_t(memory::OwnerLoad(address + 372, 4)) : guest::LoadU32(address + 372))); }
  void set_emitter_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 372)) memory::OwnerStore(address + 372, 4, v); else guest::StoreU32(address + 372, v); }
  float emitter_position_y() const { return std::bit_cast<float>(emitter_position_y_bits()); }
  void set_emitter_position_y(float v) const { set_emitter_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t emitter_position_z_address() const { return address + 376; }
  uint32_t emitter_position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_position_z); return uint32_t((memory::IsHandle(address + 376) ? uint32_t(memory::OwnerLoad(address + 376, 4)) : guest::LoadU32(address + 376))); }
  void set_emitter_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 376)) memory::OwnerStore(address + 376, 4, v); else guest::StoreU32(address + 376, v); }
  float emitter_position_z() const { return std::bit_cast<float>(emitter_position_z_bits()); }
  void set_emitter_position_z(float v) const { set_emitter_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t emitter_position_w_address() const { return address + 380; }
  uint32_t emitter_position_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_position_w); return uint32_t((memory::IsHandle(address + 380) ? uint32_t(memory::OwnerLoad(address + 380, 4)) : guest::LoadU32(address + 380))); }
  void set_emitter_position_w_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_position_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 380)) memory::OwnerStore(address + 380, 4, v); else guest::StoreU32(address + 380, v); }
  float emitter_position_w() const { return std::bit_cast<float>(emitter_position_w_bits()); }
  void set_emitter_position_w(float v) const { set_emitter_position_w_bits(std::bit_cast<uint32_t>(v)); }
  // the listener's position as a vector (x, y, z, w), last tick's for the velocity
  uint32_t listener_position_x_address() const { return address + 384; }
  uint32_t listener_position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->listener_position_x); return uint32_t((memory::IsHandle(address + 384) ? uint32_t(memory::OwnerLoad(address + 384, 4)) : guest::LoadU32(address + 384))); }
  void set_listener_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->listener_position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 384)) memory::OwnerStore(address + 384, 4, v); else guest::StoreU32(address + 384, v); }
  float listener_position_x() const { return std::bit_cast<float>(listener_position_x_bits()); }
  void set_listener_position_x(float v) const { set_listener_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t listener_position_y_address() const { return address + 388; }
  uint32_t listener_position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->listener_position_y); return uint32_t((memory::IsHandle(address + 388) ? uint32_t(memory::OwnerLoad(address + 388, 4)) : guest::LoadU32(address + 388))); }
  void set_listener_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->listener_position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 388)) memory::OwnerStore(address + 388, 4, v); else guest::StoreU32(address + 388, v); }
  float listener_position_y() const { return std::bit_cast<float>(listener_position_y_bits()); }
  void set_listener_position_y(float v) const { set_listener_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t listener_position_z_address() const { return address + 392; }
  uint32_t listener_position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->listener_position_z); return uint32_t((memory::IsHandle(address + 392) ? uint32_t(memory::OwnerLoad(address + 392, 4)) : guest::LoadU32(address + 392))); }
  void set_listener_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->listener_position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 392)) memory::OwnerStore(address + 392, 4, v); else guest::StoreU32(address + 392, v); }
  float listener_position_z() const { return std::bit_cast<float>(listener_position_z_bits()); }
  void set_listener_position_z(float v) const { set_listener_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t listener_position_w_address() const { return address + 396; }
  uint32_t listener_position_w_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->listener_position_w); return uint32_t((memory::IsHandle(address + 396) ? uint32_t(memory::OwnerLoad(address + 396, 4)) : guest::LoadU32(address + 396))); }
  void set_listener_position_w_bits(uint32_t v) const { if (auto* d = direct()) { d->listener_position_w = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 396)) memory::OwnerStore(address + 396, 4, v); else guest::StoreU32(address + 396, v); }
  float listener_position_w() const { return std::bit_cast<float>(listener_position_w_bits()); }
  void set_listener_position_w(float v) const { set_listener_position_w_bits(std::bit_cast<uint32_t>(v)); }
  // 0 when the car sound has no racer
  uint32_t vehicle_address() const { return address + 400; }
  uint32_t vehicle_bits() const { if (auto* d = direct()) return d->vehicle; return uint32_t((memory::IsHandle(address + 400) ? uint32_t(memory::OwnerLoad(address + 400, 4)) : guest::LoadU32(address + 400))); }
  void set_vehicle_bits(uint32_t v) const { if (auto* d = direct()) { d->vehicle = v; return; } if (memory::IsHandle(address + 400)) memory::OwnerStore(address + 400, 4, v); else guest::StoreU32(address + 400, v); }
  template <typename T = Vehicle>
  T vehicle() const { return T(vehicle_bits()); }
  void set_vehicle(const View& w) const { set_vehicle_bits(w.address); }
  void set_vehicle(uint32_t v) const { set_vehicle_bits(v); }
  // the car's index, 0 to 6
  uint32_t index_address() const { return address + 404; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 404) ? uint8_t(memory::OwnerLoad(address + 404, 1)) : guest::LoadU8(address + 404))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 1, v); else guest::StoreU8(address + 404, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 404) ? uint8_t(memory::OwnerLoad(address + 404, 1)) : guest::LoadU8(address + 404))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 404)) memory::OwnerStore(address + 404, 1, v); else guest::StoreU8(address + 404, v); }
  // a CarSoundEngine
  uint32_t engine_address() const { return address + 408; }
  uint8_t* engine() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 408 : guest::Pointer(address + 408); }
  template <typename T = CarSoundEngine>
  T engine_object() const { return T(address + 408); }
  // a CarSoundHits
  uint32_t hits_address() const { return address + 584; }
  uint8_t* hits() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 584 : guest::Pointer(address + 584); }
  template <typename T = CarSoundHits>
  T hits_object() const { return T(address + 584); }
  // a CarSoundTires (its voice holder at +676)
  uint32_t tires_address() const { return address + 652; }
  uint8_t* tires() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 652 : guest::Pointer(address + 652); }
  template <typename T = CarSoundTires>
  T tires_object() const { return T(address + 652); }
  // a CarSoundGear (sub_8210EBF0, probably the gear change)
  uint32_t gear_address() const { return address + 736; }
  uint8_t* gear() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 736 : guest::Pointer(address + 736); }
  template <typename T = CarSoundGear>
  T gear_object() const { return T(address + 736); }
  // a CarSoundRoll
  uint32_t roll_address() const { return address + 792; }
  uint8_t* roll() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 792 : guest::Pointer(address + 792); }
  template <typename T = CarSoundRoll>
  T roll_object() const { return T(address + 792); }
  // a CarSoundEvents (sub_8210ECF0)
  uint32_t events_address() const { return address + 840; }
  uint8_t* events() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 840 : guest::Pointer(address + 840); }
  template <typename T = CarSoundEvents>
  T events_object() const { return T(address + 840); }
  // a CarSoundFade (sub_8210EEE0)
  uint32_t fade_address() const { return address + 888; }
  uint8_t* fade() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 888 : guest::Pointer(address + 888); }
  template <typename T = CarSoundFade>
  T fade_object() const { return T(address + 888); }
  // a CarSoundTurbo (probably)
  uint32_t turbo_address() const { return address + 952; }
  uint8_t* turbo() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 952 : guest::Pointer(address + 952); }
  template <typename T = CarSoundTurbo>
  T turbo_object() const { return T(address + 952); }
  // a CarSoundWind
  uint32_t wind_address() const { return address + 1016; }
  uint8_t* wind() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1016 : guest::Pointer(address + 1016); }
  template <typename T = CarSoundWind>
  T wind_object() const { return T(address + 1016); }
  // a CarSoundHorn (probably)
  uint32_t horn_address() const { return address + 1072; }
  uint8_t* horn() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1072 : guest::Pointer(address + 1072); }
  template <typename T = CarSoundHorn>
  T horn_object() const { return T(address + 1072); }
  // a CarSoundPassBy
  uint32_t pass_by_address() const { return address + 1136; }
  uint8_t* pass_by() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1136 : guest::Pointer(address + 1136); }
  template <typename T = CarSoundPassBy>
  T pass_by_object() const { return T(address + 1136); }
  // the emitter's volume curve: six {distance, value} float pairs
  uint32_t volume_curve_points_address() const { return address + 1192; }
  uint8_t* volume_curve_points() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1192 : guest::Pointer(address + 1192); }
  // an X3DAudioCurve over +1192 (six points), the emitter's +64
  uint32_t volume_curve_address() const { return address + 1240; }
  uint8_t* volume_curve() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 1240 : guest::Pointer(address + 1240); }
  template <typename T = X3DAudioCurve>
  T volume_curve_object() const { return T(address + 1240); }
  // 1 once CarSounds::SetBanks gave it its banks; 0 stopped
  uint32_t live_address() const { return address + 1248; }
  uint32_t live_bits() const { if (auto* d = direct()) return d->live; return uint32_t((memory::IsHandle(address + 1248) ? uint32_t(memory::OwnerLoad(address + 1248, 4)) : guest::LoadU32(address + 1248))); }
  void set_live_bits(uint32_t v) const { if (auto* d = direct()) { d->live = v; return; } if (memory::IsHandle(address + 1248)) memory::OwnerStore(address + 1248, 4, v); else guest::StoreU32(address + 1248, v); }
  uint32_t live() const { if (auto* d = direct()) return d->live; return uint32_t((memory::IsHandle(address + 1248) ? uint32_t(memory::OwnerLoad(address + 1248, 4)) : guest::LoadU32(address + 1248))); }
  void set_live(uint32_t v) const { if (auto* d = direct()) { d->live = v; return; } if (memory::IsHandle(address + 1248)) memory::OwnerStore(address + 1248, 4, v); else guest::StoreU32(address + 1248, v); }
  // 1.0; EngineSound::Update's f2
  uint32_t engine_gain_address() const { return address + 1252; }
  uint32_t engine_gain_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->engine_gain); return uint32_t((memory::IsHandle(address + 1252) ? uint32_t(memory::OwnerLoad(address + 1252, 4)) : guest::LoadU32(address + 1252))); }
  void set_engine_gain_bits(uint32_t v) const { if (auto* d = direct()) { d->engine_gain = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 1252)) memory::OwnerStore(address + 1252, 4, v); else guest::StoreU32(address + 1252, v); }
  float engine_gain() const { return std::bit_cast<float>(engine_gain_bits()); }
  void set_engine_gain(float v) const { set_engine_gain_bits(std::bit_cast<uint32_t>(v)); }
  // the last tick's Doppler factor (+204)
  uint32_t last_doppler_address() const { return address + 1256; }
  uint32_t last_doppler_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->last_doppler); return uint32_t((memory::IsHandle(address + 1256) ? uint32_t(memory::OwnerLoad(address + 1256, 4)) : guest::LoadU32(address + 1256))); }
  void set_last_doppler_bits(uint32_t v) const { if (auto* d = direct()) { d->last_doppler = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 1256)) memory::OwnerStore(address + 1256, 4, v); else guest::StoreU32(address + 1256, v); }
  float last_doppler() const { return std::bit_cast<float>(last_doppler_bits()); }
  void set_last_doppler(float v) const { set_last_doppler_bits(std::bit_cast<uint32_t>(v)); }
  // 1 the tick a car passed close (PassBySound::Update's r6)
  uint32_t passed_close_address() const { return address + 1260; }
  uint32_t passed_close_bits() const { if (auto* d = direct()) return d->passed_close; return uint32_t((memory::IsHandle(address + 1260) ? uint32_t(memory::OwnerLoad(address + 1260, 4)) : guest::LoadU32(address + 1260))); }
  void set_passed_close_bits(uint32_t v) const { if (auto* d = direct()) { d->passed_close = v; return; } if (memory::IsHandle(address + 1260)) memory::OwnerStore(address + 1260, 4, v); else guest::StoreU32(address + 1260, v); }
  uint32_t passed_close() const { if (auto* d = direct()) return d->passed_close; return uint32_t((memory::IsHandle(address + 1260) ? uint32_t(memory::OwnerLoad(address + 1260, 4)) : guest::LoadU32(address + 1260))); }
  void set_passed_close(uint32_t v) const { if (auto* d = direct()) { d->passed_close = v; return; } if (memory::IsHandle(address + 1260)) memory::OwnerStore(address + 1260, 4, v); else guest::StoreU32(address + 1260, v); }
};

// allocated; measure: new<8864>@CarSounds::Init
#pragma pack(push, 1)
struct CarSoundArrayData {
  uint32_t count = 0;  // +0
  uint8_t pad_4[12] = {};
  uint8_t sounds[8848] = {};  // +16, guest order
};
#pragma pack(pop)
static_assert(sizeof(CarSoundArrayData) == 8864);
inline constexpr memory::LayoutField kCarSoundArrayFields[] = {{0, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {104, 4}, {108, 4}, {112, 4}, {116, 4}, {120, 4}, {124, 4}, {128, 4}, {132, 4}, {136, 4}, {140, 4}, {144, 4}, {148, 4}, {152, 4}, {156, 4}, {160, 4}, {164, 4}, {168, 4}, {172, 4}, {176, 4}, {180, 4}, {184, 4}, {188, 4}, {192, 4}, {196, 4}, {200, 4}, {204, 4}, {208, 4}, {212, 4}, {216, 4}, {220, 4}, {224, 4}, {228, 4}, {232, 4}, {236, 4}, {340, 4}, {348, 4}, {356, 4}, {364, 4}, {368, 8}, {384, 4}, {388, 4}, {392, 4}, {396, 4}, {400, 4}, {404, 4}, {408, 4}, {412, 4}, {416, 4}, {424, 4}, {428, 4}, {432, 4}, {436, 4}, {440, 4}, {468, 4}, {488, 4}, {508, 4}, {512, 4}, {516, 4}, {520, 4}, {524, 4}, {552, 4}, {572, 4}, {592, 4}, {596, 4}, {600, 4}, {620, 4}, {640, 4}, {648, 4}, {652, 4}, {656, 4}, {660, 4}, {664, 4}, {668, 4}, {672, 4}, {680, 4}, {684, 4}, {692, 4}, {712, 4}, {732, 4}, {738, 2}, {740, 4}, {744, 2}, {748, 4}, {752, 4}, {772, 4}, {792, 4}, {800, 4}, {804, 4}, {808, 4}, {828, 4}, {848, 4}, {856, 4}, {876, 4}, {896, 4}, {904, 4}, {924, 4}, {944, 4}, {952, 4}, {956, 4}, {960, 4}, {964, 4}, {968, 4}, {988, 4}, {1008, 4}, {1016, 4}, {1020, 4}, {1024, 2}, {1028, 4}, {1032, 4}, {1052, 4}, {1072, 4}, {1080, 4}, {1084, 4}, {1088, 4}, {1108, 4}, {1128, 4}, {1136, 4}, {1140, 4}, {1144, 4}, {1148, 4}, {1152, 4}, {1172, 4}, {1192, 4}, {1200, 4}, {1204, 4}, {1256, 4}, {1260, 4}, {1264, 4}, {1268, 4}, {1272, 4}, {1276, 4}, {1280, 4}, {1284, 4}, {1288, 4}, {1292, 4}, {1296, 4}, {1300, 4}, {1304, 4}, {1308, 4}, {1312, 4}, {1316, 4}, {1320, 4}, {1324, 4}, {1328, 4}, {1332, 4}, {1336, 4}, {1340, 4}, {1344, 4}, {1348, 4}, {1352, 4}, {1356, 4}, {1360, 4}, {1364, 4}, {1368, 4}, {1372, 4}, {1376, 4}, {1380, 4}, {1384, 4}, {1388, 4}, {1392, 4}, {1396, 4}, {1400, 4}, {1404, 4}, {1408, 4}, {1412, 4}, {1416, 4}, {1420, 4}, {1424, 4}, {1428, 4}, {1432, 4}, {1436, 4}, {1440, 4}, {1444, 4}, {1448, 4}, {1452, 4}, {1456, 4}, {1460, 4}, {1464, 4}, {1468, 4}, {1472, 4}, {1476, 4}, {1480, 4}, {1484, 4}, {1488, 4}, {1492, 4}, {1496, 4}, {1500, 4}, {1604, 4}, {1612, 4}, {1620, 4}, {1628, 4}, {1632, 8}, {1648, 4}, {1652, 4}, {1656, 4}, {1660, 4}, {1664, 4}, {1668, 4}, {1672, 4}, {1676, 4}, {1680, 4}, {1688, 4}, {1692, 4}, {1696, 4}, {1700, 4}, {1704, 4}, {1732, 4}, {1752, 4}, {1772, 4}, {1776, 4}, {1780, 4}, {1784, 4}, {1788, 4}, {1816, 4}, {1836, 4}, {1856, 4}, {1860, 4}, {1864, 4}, {1884, 4}, {1904, 4}, {1912, 4}, {1916, 4}, {1920, 4}, {1924, 4}, {1928, 4}, {1932, 4}, {1936, 4}, {1944, 4}, {1948, 4}, {1956, 4}, {1976, 4}, {1996, 4}, {2002, 2}, {2004, 4}, {2008, 2}, {2012, 4}, {2016, 4}, {2036, 4}, {2056, 4}, {2064, 4}, {2068, 4}, {2072, 4}, {2092, 4}, {2112, 4}, {2120, 4}, {2140, 4}, {2160, 4}, {2168, 4}, {2188, 4}, {2208, 4}, {2216, 4}, {2220, 4}, {2224, 4}, {2228, 4}, {2232, 4}, {2252, 4}, {2272, 4}, {2280, 4}, {2284, 4}, {2288, 2}, {2292, 4}, {2296, 4}, {2316, 4}, {2336, 4}, {2344, 4}, {2348, 4}, {2352, 4}, {2372, 4}, {2392, 4}, {2400, 4}, {2404, 4}, {2408, 4}, {2412, 4}, {2416, 4}, {2436, 4}, {2456, 4}, {2464, 4}, {2468, 4}, {2520, 4}, {2524, 4}, {2528, 4}, {2532, 4}, {2536, 4}, {2540, 4}, {2544, 4}, {2548, 4}, {2552, 4}, {2556, 4}, {2560, 4}, {2564, 4}, {2568, 4}, {2572, 4}, {2576, 4}, {2580, 4}, {2584, 4}, {2588, 4}, {2592, 4}, {2596, 4}, {2600, 4}, {2604, 4}, {2608, 4}, {2612, 4}, {2616, 4}, {2620, 4}, {2624, 4}, {2628, 4}, {2632, 4}, {2636, 4}, {2640, 4}, {2644, 4}, {2648, 4}, {2652, 4}, {2656, 4}, {2660, 4}, {2664, 4}, {2668, 4}, {2672, 4}, {2676, 4}, {2680, 4}, {2684, 4}, {2688, 4}, {2692, 4}, {2696, 4}, {2700, 4}, {2704, 4}, {2708, 4}, {2712, 4}, {2716, 4}, {2720, 4}, {2724, 4}, {2728, 4}, {2732, 4}, {2736, 4}, {2740, 4}, {2744, 4}, {2748, 4}, {2752, 4}, {2756, 4}, {2760, 4}, {2764, 4}, {2868, 4}, {2876, 4}, {2884, 4}, {2892, 4}, {2896, 8}, {2912, 4}, {2916, 4}, {2920, 4}, {2924, 4}, {2928, 4}, {2932, 4}, {2936, 4}, {2940, 4}, {2944, 4}, {2952, 4}, {2956, 4}, {2960, 4}, {2964, 4}, {2968, 4}, {2996, 4}, {3016, 4}, {3036, 4}, {3040, 4}, {3044, 4}, {3048, 4}, {3052, 4}, {3080, 4}, {3100, 4}, {3120, 4}, {3124, 4}, {3128, 4}, {3148, 4}, {3168, 4}, {3176, 4}, {3180, 4}, {3184, 4}, {3188, 4}, {3192, 4}, {3196, 4}, {3200, 4}, {3208, 4}, {3212, 4}, {3220, 4}, {3240, 4}, {3260, 4}, {3266, 2}, {3268, 4}, {3272, 2}, {3276, 4}, {3280, 4}, {3300, 4}, {3320, 4}, {3328, 4}, {3332, 4}, {3336, 4}, {3356, 4}, {3376, 4}, {3384, 4}, {3404, 4}, {3424, 4}, {3432, 4}, {3452, 4}, {3472, 4}, {3480, 4}, {3484, 4}, {3488, 4}, {3492, 4}, {3496, 4}, {3516, 4}, {3536, 4}, {3544, 4}, {3548, 4}, {3552, 2}, {3556, 4}, {3560, 4}, {3580, 4}, {3600, 4}, {3608, 4}, {3612, 4}, {3616, 4}, {3636, 4}, {3656, 4}, {3664, 4}, {3668, 4}, {3672, 4}, {3676, 4}, {3680, 4}, {3700, 4}, {3720, 4}, {3728, 4}, {3732, 4}, {3784, 4}, {3788, 4}, {3792, 4}, {3796, 4}, {3800, 4}, {3804, 4}, {3808, 4}, {3812, 4}, {3816, 4}, {3820, 4}, {3824, 4}, {3828, 4}, {3832, 4}, {3836, 4}, {3840, 4}, {3844, 4}, {3848, 4}, {3852, 4}, {3856, 4}, {3860, 4}, {3864, 4}, {3868, 4}, {3872, 4}, {3876, 4}, {3880, 4}, {3884, 4}, {3888, 4}, {3892, 4}, {3896, 4}, {3900, 4}, {3904, 4}, {3908, 4}, {3912, 4}, {3916, 4}, {3920, 4}, {3924, 4}, {3928, 4}, {3932, 4}, {3936, 4}, {3940, 4}, {3944, 4}, {3948, 4}, {3952, 4}, {3956, 4}, {3960, 4}, {3964, 4}, {3968, 4}, {3972, 4}, {3976, 4}, {3980, 4}, {3984, 4}, {3988, 4}, {3992, 4}, {3996, 4}, {4000, 4}, {4004, 4}, {4008, 4}, {4012, 4}, {4016, 4}, {4020, 4}, {4024, 4}, {4028, 4}, {4132, 4}, {4140, 4}, {4148, 4}, {4156, 4}, {4160, 8}, {4176, 4}, {4180, 4}, {4184, 4}, {4188, 4}, {4192, 4}, {4196, 4}, {4200, 4}, {4204, 4}, {4208, 4}, {4216, 4}, {4220, 4}, {4224, 4}, {4228, 4}, {4232, 4}, {4260, 4}, {4280, 4}, {4300, 4}, {4304, 4}, {4308, 4}, {4312, 4}, {4316, 4}, {4344, 4}, {4364, 4}, {4384, 4}, {4388, 4}, {4392, 4}, {4412, 4}, {4432, 4}, {4440, 4}, {4444, 4}, {4448, 4}, {4452, 4}, {4456, 4}, {4460, 4}, {4464, 4}, {4472, 4}, {4476, 4}, {4484, 4}, {4504, 4}, {4524, 4}, {4530, 2}, {4532, 4}, {4536, 2}, {4540, 4}, {4544, 4}, {4564, 4}, {4584, 4}, {4592, 4}, {4596, 4}, {4600, 4}, {4620, 4}, {4640, 4}, {4648, 4}, {4668, 4}, {4688, 4}, {4696, 4}, {4716, 4}, {4736, 4}, {4744, 4}, {4748, 4}, {4752, 4}, {4756, 4}, {4760, 4}, {4780, 4}, {4800, 4}, {4808, 4}, {4812, 4}, {4816, 2}, {4820, 4}, {4824, 4}, {4844, 4}, {4864, 4}, {4872, 4}, {4876, 4}, {4880, 4}, {4900, 4}, {4920, 4}, {4928, 4}, {4932, 4}, {4936, 4}, {4940, 4}, {4944, 4}, {4964, 4}, {4984, 4}, {4992, 4}, {4996, 4}, {5048, 4}, {5052, 4}, {5056, 4}, {5060, 4}, {5064, 4}, {5068, 4}, {5072, 4}, {5076, 4}, {5080, 4}, {5084, 4}, {5088, 4}, {5092, 4}, {5096, 4}, {5100, 4}, {5104, 4}, {5108, 4}, {5112, 4}, {5116, 4}, {5120, 4}, {5124, 4}, {5128, 4}, {5132, 4}, {5136, 4}, {5140, 4}, {5144, 4}, {5148, 4}, {5152, 4}, {5156, 4}, {5160, 4}, {5164, 4}, {5168, 4}, {5172, 4}, {5176, 4}, {5180, 4}, {5184, 4}, {5188, 4}, {5192, 4}, {5196, 4}, {5200, 4}, {5204, 4}, {5208, 4}, {5212, 4}, {5216, 4}, {5220, 4}, {5224, 4}, {5228, 4}, {5232, 4}, {5236, 4}, {5240, 4}, {5244, 4}, {5248, 4}, {5252, 4}, {5256, 4}, {5260, 4}, {5264, 4}, {5268, 4}, {5272, 4}, {5276, 4}, {5280, 4}, {5284, 4}, {5288, 4}, {5292, 4}, {5396, 4}, {5404, 4}, {5412, 4}, {5420, 4}, {5424, 8}, {5440, 4}, {5444, 4}, {5448, 4}, {5452, 4}, {5456, 4}, {5460, 4}, {5464, 4}, {5468, 4}, {5472, 4}, {5480, 4}, {5484, 4}, {5488, 4}, {5492, 4}, {5496, 4}, {5524, 4}, {5544, 4}, {5564, 4}, {5568, 4}, {5572, 4}, {5576, 4}, {5580, 4}, {5608, 4}, {5628, 4}, {5648, 4}, {5652, 4}, {5656, 4}, {5676, 4}, {5696, 4}, {5704, 4}, {5708, 4}, {5712, 4}, {5716, 4}, {5720, 4}, {5724, 4}, {5728, 4}, {5736, 4}, {5740, 4}, {5748, 4}, {5768, 4}, {5788, 4}, {5794, 2}, {5796, 4}, {5800, 2}, {5804, 4}, {5808, 4}, {5828, 4}, {5848, 4}, {5856, 4}, {5860, 4}, {5864, 4}, {5884, 4}, {5904, 4}, {5912, 4}, {5932, 4}, {5952, 4}, {5960, 4}, {5980, 4}, {6000, 4}, {6008, 4}, {6012, 4}, {6016, 4}, {6020, 4}, {6024, 4}, {6044, 4}, {6064, 4}, {6072, 4}, {6076, 4}, {6080, 2}, {6084, 4}, {6088, 4}, {6108, 4}, {6128, 4}, {6136, 4}, {6140, 4}, {6144, 4}, {6164, 4}, {6184, 4}, {6192, 4}, {6196, 4}, {6200, 4}, {6204, 4}, {6208, 4}, {6228, 4}, {6248, 4}, {6256, 4}, {6260, 4}, {6312, 4}, {6316, 4}, {6320, 4}, {6324, 4}, {6328, 4}, {6332, 4}, {6336, 4}, {6340, 4}, {6344, 4}, {6348, 4}, {6352, 4}, {6356, 4}, {6360, 4}, {6364, 4}, {6368, 4}, {6372, 4}, {6376, 4}, {6380, 4}, {6384, 4}, {6388, 4}, {6392, 4}, {6396, 4}, {6400, 4}, {6404, 4}, {6408, 4}, {6412, 4}, {6416, 4}, {6420, 4}, {6424, 4}, {6428, 4}, {6432, 4}, {6436, 4}, {6440, 4}, {6444, 4}, {6448, 4}, {6452, 4}, {6456, 4}, {6460, 4}, {6464, 4}, {6468, 4}, {6472, 4}, {6476, 4}, {6480, 4}, {6484, 4}, {6488, 4}, {6492, 4}, {6496, 4}, {6500, 4}, {6504, 4}, {6508, 4}, {6512, 4}, {6516, 4}, {6520, 4}, {6524, 4}, {6528, 4}, {6532, 4}, {6536, 4}, {6540, 4}, {6544, 4}, {6548, 4}, {6552, 4}, {6556, 4}, {6660, 4}, {6668, 4}, {6676, 4}, {6684, 4}, {6688, 8}, {6704, 4}, {6708, 4}, {6712, 4}, {6716, 4}, {6720, 4}, {6724, 4}, {6728, 4}, {6732, 4}, {6736, 4}, {6744, 4}, {6748, 4}, {6752, 4}, {6756, 4}, {6760, 4}, {6788, 4}, {6808, 4}, {6828, 4}, {6832, 4}, {6836, 4}, {6840, 4}, {6844, 4}, {6872, 4}, {6892, 4}, {6912, 4}, {6916, 4}, {6920, 4}, {6940, 4}, {6960, 4}, {6968, 4}, {6972, 4}, {6976, 4}, {6980, 4}, {6984, 4}, {6988, 4}, {6992, 4}, {7000, 4}, {7004, 4}, {7012, 4}, {7032, 4}, {7052, 4}, {7058, 2}, {7060, 4}, {7064, 2}, {7068, 4}, {7072, 4}, {7092, 4}, {7112, 4}, {7120, 4}, {7124, 4}, {7128, 4}, {7148, 4}, {7168, 4}, {7176, 4}, {7196, 4}, {7216, 4}, {7224, 4}, {7244, 4}, {7264, 4}, {7272, 4}, {7276, 4}, {7280, 4}, {7284, 4}, {7288, 4}, {7308, 4}, {7328, 4}, {7336, 4}, {7340, 4}, {7344, 2}, {7348, 4}, {7352, 4}, {7372, 4}, {7392, 4}, {7400, 4}, {7404, 4}, {7408, 4}, {7428, 4}, {7448, 4}, {7456, 4}, {7460, 4}, {7464, 4}, {7468, 4}, {7472, 4}, {7492, 4}, {7512, 4}, {7520, 4}, {7524, 4}, {7576, 4}, {7580, 4}, {7584, 4}, {7588, 4}, {7592, 4}, {7596, 4}, {7600, 4}, {7604, 4}, {7608, 4}, {7612, 4}, {7616, 4}, {7620, 4}, {7624, 4}, {7628, 4}, {7632, 4}, {7636, 4}, {7640, 4}, {7644, 4}, {7648, 4}, {7652, 4}, {7656, 4}, {7660, 4}, {7664, 4}, {7668, 4}, {7672, 4}, {7676, 4}, {7680, 4}, {7684, 4}, {7688, 4}, {7692, 4}, {7696, 4}, {7700, 4}, {7704, 4}, {7708, 4}, {7712, 4}, {7716, 4}, {7720, 4}, {7724, 4}, {7728, 4}, {7732, 4}, {7736, 4}, {7740, 4}, {7744, 4}, {7748, 4}, {7752, 4}, {7756, 4}, {7760, 4}, {7764, 4}, {7768, 4}, {7772, 4}, {7776, 4}, {7780, 4}, {7784, 4}, {7788, 4}, {7792, 4}, {7796, 4}, {7800, 4}, {7804, 4}, {7808, 4}, {7812, 4}, {7816, 4}, {7820, 4}, {7924, 4}, {7932, 4}, {7940, 4}, {7948, 4}, {7952, 8}, {7968, 4}, {7972, 4}, {7976, 4}, {7980, 4}, {7984, 4}, {7988, 4}, {7992, 4}, {7996, 4}, {8000, 4}, {8008, 4}, {8012, 4}, {8016, 4}, {8020, 4}, {8024, 4}, {8052, 4}, {8072, 4}, {8092, 4}, {8096, 4}, {8100, 4}, {8104, 4}, {8108, 4}, {8136, 4}, {8156, 4}, {8176, 4}, {8180, 4}, {8184, 4}, {8204, 4}, {8224, 4}, {8232, 4}, {8236, 4}, {8240, 4}, {8244, 4}, {8248, 4}, {8252, 4}, {8256, 4}, {8264, 4}, {8268, 4}, {8276, 4}, {8296, 4}, {8316, 4}, {8322, 2}, {8324, 4}, {8328, 2}, {8332, 4}, {8336, 4}, {8356, 4}, {8376, 4}, {8384, 4}, {8388, 4}, {8392, 4}, {8412, 4}, {8432, 4}, {8440, 4}, {8460, 4}, {8480, 4}, {8488, 4}, {8508, 4}, {8528, 4}, {8536, 4}, {8540, 4}, {8544, 4}, {8548, 4}, {8552, 4}, {8572, 4}, {8592, 4}, {8600, 4}, {8604, 4}, {8608, 2}, {8612, 4}, {8616, 4}, {8636, 4}, {8656, 4}, {8664, 4}, {8668, 4}, {8672, 4}, {8692, 4}, {8712, 4}, {8720, 4}, {8724, 4}, {8728, 4}, {8732, 4}, {8736, 4}, {8756, 4}, {8776, 4}, {8784, 4}, {8788, 4}, {8840, 4}, {8844, 4}, {8848, 4}, {8852, 4}, {8856, 4}, {8860, 4}};
inline constexpr memory::Layout kCarSoundArrayLayout = {kCarSoundArrayFields, 1072};

struct CarSoundArray : View {
  using View::View;
  static constexpr uint32_t kSize = 8864;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundArrayData* data() const { return static_cast<CarSoundArrayData*>(memory::HandleObject(address)); }
  static CarSoundArray New() {
    return CarSoundArray(memory::MakeHandle(new CarSoundArrayData(), sizeof(CarSoundArrayData), "CarSoundArray", &kCarSoundArrayLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundArray NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundArrayData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundArrayData*>(::operator new(sizeof(CarSoundArrayData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundArrayData();
    return CarSoundArray(memory::MakeHandle(p, sizeof(CarSoundArrayData) * count, "CarSoundArray", &kCarSoundArrayLayout, sizeof(CarSoundArrayData)));
  }
  CarSoundArray at(uint32_t i) const { return CarSoundArray(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundArrayData* direct() const { return static_cast<CarSoundArrayData*>(memory::HandleOwn(address, &kCarSoundArrayLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundArrayLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundArrayLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 7 (operator new[]'s count; CarSounds::Destroy reads it at the first sound's -16)
  uint32_t count_address() const { return address + 0; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // seven CarSounds (the embedded sound)
  uint32_t sounds_address() const { return address + 16; }
  uint8_t* sounds() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 16 : guest::Pointer(address + 16); }
  template <typename T = CarSound>
  T sound(uint32_t i) const { return T(address + 16 + i * T::kSize); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarSoundVoiceHolderData {
  uint32_t vtable = 0;  // +0
  uint8_t voices[16] = {};  // +4, guest order
  uint32_t voice_count = 0;  // +20
  uint8_t paused[16] = {};  // +24, guest order
};
#pragma pack(pop)
static_assert(sizeof(CarSoundVoiceHolderData) == 40);
inline constexpr memory::LayoutField kCarSoundVoiceHolderFields[] = {{0, 4}, {20, 4}};
inline constexpr memory::Layout kCarSoundVoiceHolderLayout = {kCarSoundVoiceHolderFields, 2};

struct CarSoundVoiceHolder : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82010B5C;
  CarSoundVoiceHolderData* data() const { return static_cast<CarSoundVoiceHolderData*>(memory::HandleObject(address)); }
  static CarSoundVoiceHolder New() {
    return CarSoundVoiceHolder(memory::MakeHandle(new CarSoundVoiceHolderData(), sizeof(CarSoundVoiceHolderData), "CarSoundVoiceHolder", &kCarSoundVoiceHolderLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundVoiceHolder NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundVoiceHolderData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundVoiceHolderData*>(::operator new(sizeof(CarSoundVoiceHolderData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundVoiceHolderData();
    return CarSoundVoiceHolder(memory::MakeHandle(p, sizeof(CarSoundVoiceHolderData) * count, "CarSoundVoiceHolder", &kCarSoundVoiceHolderLayout, sizeof(CarSoundVoiceHolderData)));
  }
  CarSoundVoiceHolder at(uint32_t i) const { return CarSoundVoiceHolder(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundVoiceHolderData* direct() const { return static_cast<CarSoundVoiceHolderData*>(memory::HandleOwn(address, &kCarSoundVoiceHolderLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundVoiceHolderLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundVoiceHolderLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82010B5C
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // four XAudio source voices by slot (0 a free slot)
  uint32_t voices_address() const { return address + 4; }
  uint8_t* voices() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 4 : guest::Pointer(address + 4); }
  uint32_t voice_count_address() const { return address + 20; }
  uint32_t voice_count_bits() const { if (auto* d = direct()) return d->voice_count; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_voice_count_bits(uint32_t v) const { if (auto* d = direct()) { d->voice_count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  uint32_t voice_count() const { if (auto* d = direct()) return d->voice_count; return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_voice_count(uint32_t v) const { if (auto* d = direct()) { d->voice_count = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  // four words: which voices were playing when paused
  uint32_t paused_address() const { return address + 24; }
  uint8_t* paused() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 24 : guest::Pointer(address + 24); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarSoundPartData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarSoundPartData) == 48);
inline constexpr memory::LayoutField kCarSoundPartFields[] = {{0, 4}, {20, 4}, {40, 4}};
inline constexpr memory::Layout kCarSoundPartLayout = {kCarSoundPartFields, 3};

struct CarSoundPart : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundPartData* data() const { return static_cast<CarSoundPartData*>(memory::HandleObject(address)); }
  static CarSoundPart New() {
    return CarSoundPart(memory::MakeHandle(new CarSoundPartData(), sizeof(CarSoundPartData), "CarSoundPart", &kCarSoundPartLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundPart NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundPartData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundPartData*>(::operator new(sizeof(CarSoundPartData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundPartData();
    return CarSoundPart(memory::MakeHandle(p, sizeof(CarSoundPartData) * count, "CarSoundPart", &kCarSoundPartLayout, sizeof(CarSoundPartData)));
  }
  CarSoundPart at(uint32_t i) const { return CarSoundPart(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundPartData* direct() const { return static_cast<CarSoundPartData*>(memory::HandleOwn(address, &kCarSoundPartLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundPartLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundPartLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  // the effects' bank
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  // the car's index
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
};

// embedded; measure: new<8864>@CarSounds::Init@+16@+408, CarSound@+408
#pragma pack(push, 1)
struct CarSoundEngineData {
  uint8_t layer[84] = {};  // +0, guest order
  uint8_t pitched_layer[84] = {};  // +84, guest order
  float load = 0;  // +168
  float idle_speed = 0;  // +172
};
#pragma pack(pop)
static_assert(sizeof(CarSoundEngineData) == 176);
inline constexpr memory::LayoutField kCarSoundEngineFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {44, 4}, {64, 4}, {84, 4}, {88, 4}, {92, 4}, {96, 4}, {100, 4}, {128, 4}, {148, 4}, {168, 4}, {172, 4}};
inline constexpr memory::Layout kCarSoundEngineLayout = {kCarSoundEngineFields, 16};

struct CarSoundEngine : View {
  using View::View;
  static constexpr uint32_t kSize = 176;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82008018;
  CarSoundEngineData* data() const { return static_cast<CarSoundEngineData*>(memory::HandleObject(address)); }
  static CarSoundEngine New() {
    return CarSoundEngine(memory::MakeHandle(new CarSoundEngineData(), sizeof(CarSoundEngineData), "CarSoundEngine", &kCarSoundEngineLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundEngine NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundEngineData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundEngineData*>(::operator new(sizeof(CarSoundEngineData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundEngineData();
    return CarSoundEngine(memory::MakeHandle(p, sizeof(CarSoundEngineData) * count, "CarSoundEngine", &kCarSoundEngineLayout, sizeof(CarSoundEngineData)));
  }
  CarSoundEngine at(uint32_t i) const { return CarSoundEngine(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundEngineData* direct() const { return static_cast<CarSoundEngineData*>(memory::HandleOwn(address, &kCarSoundEngineLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundEngineLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundEngineLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundEngineLayer (from the second wave bank view)
  uint32_t layer_address() const { return address + 0; }
  uint8_t* layer() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  // a CarSoundEngineLayer (from the first wave bank view)
  uint32_t pitched_layer_address() const { return address + 84; }
  uint8_t* pitched_layer() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 84 : guest::Pointer(address + 84); }
  template <typename T = CarSoundEngineLayer>
  T layer_object() const { return T(address + 0); }
  template <typename T = CarSoundEngineLayer>
  T pitched_layer_object() const { return T(address + 84); }
  // the load, eased toward the throttle's target (probably)
  uint32_t load_address() const { return address + 168; }
  uint32_t load_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->load); return uint32_t((memory::IsHandle(address + 168) ? uint32_t(memory::OwnerLoad(address + 168, 4)) : guest::LoadU32(address + 168))); }
  void set_load_bits(uint32_t v) const { if (auto* d = direct()) { d->load = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 168)) memory::OwnerStore(address + 168, 4, v); else guest::StoreU32(address + 168, v); }
  float load() const { return std::bit_cast<float>(load_bits()); }
  void set_load(float v) const { set_load_bits(std::bit_cast<uint32_t>(v)); }
  // the speed without a vehicle: up by 10 a tick, back to 800 past 7,000
  uint32_t idle_speed_address() const { return address + 172; }
  uint32_t idle_speed_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->idle_speed); return uint32_t((memory::IsHandle(address + 172) ? uint32_t(memory::OwnerLoad(address + 172, 4)) : guest::LoadU32(address + 172))); }
  void set_idle_speed_bits(uint32_t v) const { if (auto* d = direct()) { d->idle_speed = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 172)) memory::OwnerStore(address + 172, 4, v); else guest::StoreU32(address + 172, v); }
  float idle_speed() const { return std::bit_cast<float>(idle_speed_bits()); }
  void set_idle_speed(float v) const { set_idle_speed_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: EngineLayer, EngineLayer::Update.r3, EngineLayer::Start.r3
#pragma pack(push, 1)
struct CarSoundEngineLayerData {
  uint32_t vtable = 0;  // +0
  uint32_t table = 0;  // +4
  uint32_t table_copy = 0;  // +8
  uint32_t table_volume = 0;  // +12
  uint32_t table_volume_copy = 0;  // +16
  uint8_t amounts[24] = {};  // +20, guest order
  uint8_t holder[40] = {};  // +44, guest order
};
#pragma pack(pop)
static_assert(sizeof(CarSoundEngineLayerData) == 84);
inline constexpr memory::LayoutField kCarSoundEngineLayerFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {44, 4}, {64, 4}};
inline constexpr memory::Layout kCarSoundEngineLayerLayout = {kCarSoundEngineLayerFields, 7};

struct CarSoundEngineLayer : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  static constexpr uint32_t kVtable = 0x82008018;
  CarSoundEngineLayerData* data() const { return static_cast<CarSoundEngineLayerData*>(memory::HandleObject(address)); }
  static CarSoundEngineLayer New() {
    return CarSoundEngineLayer(memory::MakeHandle(new CarSoundEngineLayerData(), sizeof(CarSoundEngineLayerData), "CarSoundEngineLayer", &kCarSoundEngineLayerLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundEngineLayer NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundEngineLayerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundEngineLayerData*>(::operator new(sizeof(CarSoundEngineLayerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundEngineLayerData();
    return CarSoundEngineLayer(memory::MakeHandle(p, sizeof(CarSoundEngineLayerData) * count, "CarSoundEngineLayer", &kCarSoundEngineLayerLayout, sizeof(CarSoundEngineLayerData)));
  }
  CarSoundEngineLayer at(uint32_t i) const { return CarSoundEngineLayer(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundEngineLayerData* direct() const { return static_cast<CarSoundEngineLayerData*>(memory::HandleOwn(address, &kCarSoundEngineLayerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundEngineLayerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundEngineLayerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0x82008018
  uint32_t vtable_address() const { return address + 0; }
  uint32_t vtable_bits() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable_bits(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t vtable() const { if (auto* d = direct()) return d->vtable; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_vtable(uint32_t v) const { if (auto* d = direct()) { d->vtable = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the car's sound parameters (CarSounds::SetBanks; 0 none)
  uint32_t table_address() const { return address + 4; }
  uint32_t table_bits() const { if (auto* d = direct()) return d->table; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_table_bits(uint32_t v) const { if (auto* d = direct()) { d->table = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = CarSoundEngineTable>
  T table() const { return T(table_bits()); }
  void set_table(const View& w) const { set_table_bits(w.address); }
  void set_table(uint32_t v) const { set_table_bits(v); }
  // the same pointer
  uint32_t table_copy_address() const { return address + 8; }
  uint32_t table_copy_bits() const { if (auto* d = direct()) return d->table_copy; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_table_copy_bits(uint32_t v) const { if (auto* d = direct()) { d->table_copy = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t table_copy() const { if (auto* d = direct()) return d->table_copy; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_table_copy(uint32_t v) const { if (auto* d = direct()) { d->table_copy = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  // the table's first word
  uint32_t table_volume_address() const { return address + 12; }
  uint32_t table_volume_bits() const { if (auto* d = direct()) return d->table_volume; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_table_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->table_volume = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t table_volume() const { if (auto* d = direct()) return d->table_volume; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_table_volume(uint32_t v) const { if (auto* d = direct()) { d->table_volume = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  // the same word
  uint32_t table_volume_copy_address() const { return address + 16; }
  uint32_t table_volume_copy_bits() const { if (auto* d = direct()) return d->table_volume_copy; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_table_volume_copy_bits(uint32_t v) const { if (auto* d = direct()) { d->table_volume_copy = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t table_volume_copy() const { if (auto* d = direct()) return d->table_volume_copy; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_table_volume_copy(uint32_t v) const { if (auto* d = direct()) { d->table_volume_copy = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  // a float per table entry: the volume's amount from its first curve
  uint32_t amounts_address() const { return address + 20; }
  uint8_t* amounts() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 20 : guest::Pointer(address + 20); }
  // a CarSoundVoiceHolder: one voice per table entry
  uint32_t holder_address() const { return address + 44; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 44 : guest::Pointer(address + 44); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 44); }
};

// image; measure: EngineLayer::Update.r3.+4
struct CarSoundEngineTable : View {
  using View::View;
  static constexpr uint32_t kSize = 548;
  // an integer: the volume's scale (times 1/128)
  uint32_t volume_scale() const { return guest::LoadU32(address + 0); }
  void set_volume_scale(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t volume_scale_address() const { return address + 0; }
  // CarSoundEngineEntry records of 136 bytes, while the entry's id is above -1
  uint32_t entries_address() const { return address + 4; }
};

// image; measure: -
struct CarSoundEngineEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 136;
  // -1 (or below) ends the table
  uint32_t id() const { return guest::LoadU32(address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  // an integer, SetPitchScaled's r9
  uint32_t pitch_scale() const { return guest::LoadU32(address + 4); }
  void set_pitch_scale(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t pitch_scale_address() const { return address + 4; }
  // eight (x, y) float pairs, ended by an x at most -1.0
  uint32_t volume_curve_address() const { return address + 8; }
  // eight (x, y) float pairs
  uint32_t pitch_curve_address() const { return address + 72; }
};

// embedded; measure: EngineSound::Start.r5
struct CarSoundEngineViews : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  template <typename T = CarSoundWaveBankView>
  T pitched_layer_view() const { return T(guest::LoadU32(address + 0)); }
  void set_pitched_layer_view(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_pitched_layer_view(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t pitched_layer_view_address() const { return address + 0; }
  template <typename T = CarSoundWaveBankView>
  T layer_view() const { return T(guest::LoadU32(address + 4)); }
  void set_layer_view(const View& v) const { guest::StoreU32(address + 4, v.address); }
  void set_layer_view(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t layer_view_address() const { return address + 4; }
};

// embedded; measure: CarSound@+584
#pragma pack(push, 1)
struct CarSoundHitsData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t wait_ticks = 0;  // +48
  uint32_t scrape_ticks = 0;  // +52
  uint32_t scrape_slot = 0;  // +56
  uint32_t strength = 0;  // +60
  uint32_t since_impact = 0;  // +64
};
#pragma pack(pop)
static_assert(sizeof(CarSoundHitsData) == 68);
inline constexpr memory::LayoutField kCarSoundHitsFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kCarSoundHitsLayout = {kCarSoundHitsFields, 8};

struct CarSoundHits : View {
  using View::View;
  static constexpr uint32_t kSize = 68;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundHitsData* data() const { return static_cast<CarSoundHitsData*>(memory::HandleObject(address)); }
  static CarSoundHits New() {
    return CarSoundHits(memory::MakeHandle(new CarSoundHitsData(), sizeof(CarSoundHitsData), "CarSoundHits", &kCarSoundHitsLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundHits NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundHitsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundHitsData*>(::operator new(sizeof(CarSoundHitsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundHitsData();
    return CarSoundHits(memory::MakeHandle(p, sizeof(CarSoundHitsData) * count, "CarSoundHits", &kCarSoundHitsLayout, sizeof(CarSoundHitsData)));
  }
  CarSoundHits at(uint32_t i) const { return CarSoundHits(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundHitsData* direct() const { return static_cast<CarSoundHitsData*>(memory::HandleOwn(address, &kCarSoundHitsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundHitsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundHitsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  // the effects' bank
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  // the car's index
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // ticks to wait before the next hit (unsigned)
  uint32_t wait_ticks_address() const { return address + 48; }
  uint32_t wait_ticks_bits() const { if (auto* d = direct()) return d->wait_ticks; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_wait_ticks_bits(uint32_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t wait_ticks() const { if (auto* d = direct()) return d->wait_ticks; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_wait_ticks(uint32_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // ticks out of contact while the scrape plays
  uint32_t scrape_ticks_address() const { return address + 52; }
  uint32_t scrape_ticks_bits() const { if (auto* d = direct()) return d->scrape_ticks; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_scrape_ticks_bits(uint32_t v) const { if (auto* d = direct()) { d->scrape_ticks = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t scrape_ticks() const { if (auto* d = direct()) return d->scrape_ticks; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_scrape_ticks(uint32_t v) const { if (auto* d = direct()) { d->scrape_ticks = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // the looping wave 10's slot, -1 none
  uint32_t scrape_slot_address() const { return address + 56; }
  uint32_t scrape_slot_bits() const { if (auto* d = direct()) return d->scrape_slot; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_scrape_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->scrape_slot = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t scrape_slot() const { if (auto* d = direct()) return d->scrape_slot; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_scrape_slot(uint32_t v) const { if (auto* d = direct()) { d->scrape_slot = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  // the last impact's strength (fctiwz)
  uint32_t strength_address() const { return address + 60; }
  uint32_t strength_bits() const { if (auto* d = direct()) return d->strength; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_strength_bits(uint32_t v) const { if (auto* d = direct()) { d->strength = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t strength() const { if (auto* d = direct()) return d->strength; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_strength(uint32_t v) const { if (auto* d = direct()) { d->strength = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // ticks since the last impact, up to 32 (then -1)
  uint32_t since_impact_address() const { return address + 64; }
  uint32_t since_impact_bits() const { if (auto* d = direct()) return d->since_impact; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_since_impact_bits(uint32_t v) const { if (auto* d = direct()) { d->since_impact = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  uint32_t since_impact() const { if (auto* d = direct()) return d->since_impact; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_since_impact(uint32_t v) const { if (auto* d = direct()) { d->since_impact = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
};

// embedded; measure: CarSound@+652
#pragma pack(push, 1)
struct CarSoundTiresData {
  uint32_t slot = 0;  // +0
  uint32_t was_active = 0;  // +4
  uint8_t slip_ticks = 0;  // +8
  uint8_t ramp_step = 0;  // +9
  uint8_t ramp_length = 0;  // +10
  uint8_t pad_11[1] = {};
  uint32_t slot_2 = 0;  // +12
  uint32_t was_active_2 = 0;  // +16
  uint8_t slip_ticks_2 = 0;  // +20
  uint8_t ramp_step_2 = 0;  // +21
  uint8_t ramp_length_2 = 0;  // +22
  uint8_t pad_23[1] = {};
  uint8_t holder[40] = {};  // +24, guest order
  uint32_t view = 0;  // +64
  uint8_t index = 0;  // +68
  uint8_t pad_69[1] = {};
  uint16_t slip_steps = 0;  // +70
  float slip_time = 0;  // +72
  uint16_t pitch = 0;  // +76
  uint8_t pad_78[2] = {};
  float volume = 0;  // +80
};
#pragma pack(pop)
static_assert(sizeof(CarSoundTiresData) == 84);
inline constexpr memory::LayoutField kCarSoundTiresFields[] = {{0, 4}, {4, 4}, {12, 4}, {16, 4}, {24, 4}, {44, 4}, {64, 4}, {70, 2}, {72, 4}, {76, 2}, {80, 4}};
inline constexpr memory::Layout kCarSoundTiresLayout = {kCarSoundTiresFields, 11};

struct CarSoundTires : View {
  using View::View;
  static constexpr uint32_t kSize = 84;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundTiresData* data() const { return static_cast<CarSoundTiresData*>(memory::HandleObject(address)); }
  static CarSoundTires New() {
    return CarSoundTires(memory::MakeHandle(new CarSoundTiresData(), sizeof(CarSoundTiresData), "CarSoundTires", &kCarSoundTiresLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundTires NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundTiresData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundTiresData*>(::operator new(sizeof(CarSoundTiresData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundTiresData();
    return CarSoundTires(memory::MakeHandle(p, sizeof(CarSoundTiresData) * count, "CarSoundTires", &kCarSoundTiresLayout, sizeof(CarSoundTiresData)));
  }
  CarSoundTires at(uint32_t i) const { return CarSoundTires(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundTiresData* direct() const { return static_cast<CarSoundTiresData*>(memory::HandleOwn(address, &kCarSoundTiresLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundTiresLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundTiresLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // the layer record (TireLayer::Update's r4): the voice's slot
  uint32_t slot_address() const { return address + 0; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  // the last tick's `active`
  uint32_t was_active_address() const { return address + 4; }
  uint32_t was_active_bits() const { if (auto* d = direct()) return d->was_active; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_was_active_bits(uint32_t v) const { if (auto* d = direct()) { d->was_active = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t was_active() const { if (auto* d = direct()) return d->was_active; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_was_active(uint32_t v) const { if (auto* d = direct()) { d->was_active = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  // the ticks slipped
  uint32_t slip_ticks_address() const { return address + 8; }
  uint8_t slip_ticks_bits() const { if (auto* d = direct()) return d->slip_ticks; return uint8_t((memory::IsHandle(address + 8) ? uint8_t(memory::OwnerLoad(address + 8, 1)) : guest::LoadU8(address + 8))); }
  void set_slip_ticks_bits(uint8_t v) const { if (auto* d = direct()) { d->slip_ticks = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 1, v); else guest::StoreU8(address + 8, v); }
  uint8_t slip_ticks() const { if (auto* d = direct()) return d->slip_ticks; return uint8_t((memory::IsHandle(address + 8) ? uint8_t(memory::OwnerLoad(address + 8, 1)) : guest::LoadU8(address + 8))); }
  void set_slip_ticks(uint8_t v) const { if (auto* d = direct()) { d->slip_ticks = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 1, v); else guest::StoreU8(address + 8, v); }
  uint32_t ramp_step_address() const { return address + 9; }
  uint8_t ramp_step_bits() const { if (auto* d = direct()) return d->ramp_step; return uint8_t((memory::IsHandle(address + 9) ? uint8_t(memory::OwnerLoad(address + 9, 1)) : guest::LoadU8(address + 9))); }
  void set_ramp_step_bits(uint8_t v) const { if (auto* d = direct()) { d->ramp_step = v; return; } if (memory::IsHandle(address + 9)) memory::OwnerStore(address + 9, 1, v); else guest::StoreU8(address + 9, v); }
  uint8_t ramp_step() const { if (auto* d = direct()) return d->ramp_step; return uint8_t((memory::IsHandle(address + 9) ? uint8_t(memory::OwnerLoad(address + 9, 1)) : guest::LoadU8(address + 9))); }
  void set_ramp_step(uint8_t v) const { if (auto* d = direct()) { d->ramp_step = v; return; } if (memory::IsHandle(address + 9)) memory::OwnerStore(address + 9, 1, v); else guest::StoreU8(address + 9, v); }
  uint32_t ramp_length_address() const { return address + 10; }
  uint8_t ramp_length_bits() const { if (auto* d = direct()) return d->ramp_length; return uint8_t((memory::IsHandle(address + 10) ? uint8_t(memory::OwnerLoad(address + 10, 1)) : guest::LoadU8(address + 10))); }
  void set_ramp_length_bits(uint8_t v) const { if (auto* d = direct()) { d->ramp_length = v; return; } if (memory::IsHandle(address + 10)) memory::OwnerStore(address + 10, 1, v); else guest::StoreU8(address + 10, v); }
  uint8_t ramp_length() const { if (auto* d = direct()) return d->ramp_length; return uint8_t((memory::IsHandle(address + 10) ? uint8_t(memory::OwnerLoad(address + 10, 1)) : guest::LoadU8(address + 10))); }
  void set_ramp_length(uint8_t v) const { if (auto* d = direct()) { d->ramp_length = v; return; } if (memory::IsHandle(address + 10)) memory::OwnerStore(address + 10, 1, v); else guest::StoreU8(address + 10, v); }
  // the second layer record's (set 1 by TireSound::Start)
  uint32_t slot_2_address() const { return address + 12; }
  uint32_t slot_2_bits() const { if (auto* d = direct()) return d->slot_2; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_slot_2_bits(uint32_t v) const { if (auto* d = direct()) { d->slot_2 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t slot_2() const { if (auto* d = direct()) return d->slot_2; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_slot_2(uint32_t v) const { if (auto* d = direct()) { d->slot_2 = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t was_active_2_address() const { return address + 16; }
  uint32_t was_active_2_bits() const { if (auto* d = direct()) return d->was_active_2; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_was_active_2_bits(uint32_t v) const { if (auto* d = direct()) { d->was_active_2 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t was_active_2() const { if (auto* d = direct()) return d->was_active_2; return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_was_active_2(uint32_t v) const { if (auto* d = direct()) { d->was_active_2 = v; return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  uint32_t slip_ticks_2_address() const { return address + 20; }
  uint8_t slip_ticks_2_bits() const { if (auto* d = direct()) return d->slip_ticks_2; return uint8_t((memory::IsHandle(address + 20) ? uint8_t(memory::OwnerLoad(address + 20, 1)) : guest::LoadU8(address + 20))); }
  void set_slip_ticks_2_bits(uint8_t v) const { if (auto* d = direct()) { d->slip_ticks_2 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 1, v); else guest::StoreU8(address + 20, v); }
  uint8_t slip_ticks_2() const { if (auto* d = direct()) return d->slip_ticks_2; return uint8_t((memory::IsHandle(address + 20) ? uint8_t(memory::OwnerLoad(address + 20, 1)) : guest::LoadU8(address + 20))); }
  void set_slip_ticks_2(uint8_t v) const { if (auto* d = direct()) { d->slip_ticks_2 = v; return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 1, v); else guest::StoreU8(address + 20, v); }
  uint32_t ramp_step_2_address() const { return address + 21; }
  uint8_t ramp_step_2_bits() const { if (auto* d = direct()) return d->ramp_step_2; return uint8_t((memory::IsHandle(address + 21) ? uint8_t(memory::OwnerLoad(address + 21, 1)) : guest::LoadU8(address + 21))); }
  void set_ramp_step_2_bits(uint8_t v) const { if (auto* d = direct()) { d->ramp_step_2 = v; return; } if (memory::IsHandle(address + 21)) memory::OwnerStore(address + 21, 1, v); else guest::StoreU8(address + 21, v); }
  uint8_t ramp_step_2() const { if (auto* d = direct()) return d->ramp_step_2; return uint8_t((memory::IsHandle(address + 21) ? uint8_t(memory::OwnerLoad(address + 21, 1)) : guest::LoadU8(address + 21))); }
  void set_ramp_step_2(uint8_t v) const { if (auto* d = direct()) { d->ramp_step_2 = v; return; } if (memory::IsHandle(address + 21)) memory::OwnerStore(address + 21, 1, v); else guest::StoreU8(address + 21, v); }
  uint32_t ramp_length_2_address() const { return address + 22; }
  uint8_t ramp_length_2_bits() const { if (auto* d = direct()) return d->ramp_length_2; return uint8_t((memory::IsHandle(address + 22) ? uint8_t(memory::OwnerLoad(address + 22, 1)) : guest::LoadU8(address + 22))); }
  void set_ramp_length_2_bits(uint8_t v) const { if (auto* d = direct()) { d->ramp_length_2 = v; return; } if (memory::IsHandle(address + 22)) memory::OwnerStore(address + 22, 1, v); else guest::StoreU8(address + 22, v); }
  uint8_t ramp_length_2() const { if (auto* d = direct()) return d->ramp_length_2; return uint8_t((memory::IsHandle(address + 22) ? uint8_t(memory::OwnerLoad(address + 22, 1)) : guest::LoadU8(address + 22))); }
  void set_ramp_length_2(uint8_t v) const { if (auto* d = direct()) { d->ramp_length_2 = v; return; } if (memory::IsHandle(address + 22)) memory::OwnerStore(address + 22, 1, v); else guest::StoreU8(address + 22, v); }
  // a CarSoundVoiceHolder (+28 and +32 its voices)
  uint32_t holder_address() const { return address + 24; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 24 : guest::Pointer(address + 24); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 24); }
  uint32_t view_address() const { return address + 64; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 68; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 68) ? uint8_t(memory::OwnerLoad(address + 68, 1)) : guest::LoadU8(address + 68))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 1, v); else guest::StoreU8(address + 68, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 68) ? uint8_t(memory::OwnerLoad(address + 68, 1)) : guest::LoadU8(address + 68))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 1, v); else guest::StoreU8(address + 68, v); }
  // up by one a slipping tick
  uint32_t slip_steps_address() const { return address + 70; }
  uint16_t slip_steps_bits() const { if (auto* d = direct()) return d->slip_steps; return uint16_t((memory::IsHandle(address + 70) ? uint16_t(memory::OwnerLoad(address + 70, 2)) : guest::LoadU16(address + 70))); }
  void set_slip_steps_bits(uint16_t v) const { if (auto* d = direct()) { d->slip_steps = v; return; } if (memory::IsHandle(address + 70)) memory::OwnerStore(address + 70, 2, v); else guest::StoreU16(address + 70, v); }
  uint16_t slip_steps() const { if (auto* d = direct()) return d->slip_steps; return uint16_t((memory::IsHandle(address + 70) ? uint16_t(memory::OwnerLoad(address + 70, 2)) : guest::LoadU16(address + 70))); }
  void set_slip_steps(uint16_t v) const { if (auto* d = direct()) { d->slip_steps = v; return; } if (memory::IsHandle(address + 70)) memory::OwnerStore(address + 70, 2, v); else guest::StoreU16(address + 70, v); }
  // up by 1.0 a slipping tick
  uint32_t slip_time_address() const { return address + 72; }
  uint32_t slip_time_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->slip_time); return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_slip_time_bits(uint32_t v) const { if (auto* d = direct()) { d->slip_time = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  float slip_time() const { return std::bit_cast<float>(slip_time_bits()); }
  void set_slip_time(float v) const { set_slip_time_bits(std::bit_cast<uint32_t>(v)); }
  // a signed half-word, stepped a third of the way to its target
  uint32_t pitch_address() const { return address + 76; }
  uint16_t pitch_bits() const { if (auto* d = direct()) return d->pitch; return uint16_t((memory::IsHandle(address + 76) ? uint16_t(memory::OwnerLoad(address + 76, 2)) : guest::LoadU16(address + 76))); }
  void set_pitch_bits(uint16_t v) const { if (auto* d = direct()) { d->pitch = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 2, v); else guest::StoreU16(address + 76, v); }
  uint16_t pitch() const { if (auto* d = direct()) return d->pitch; return uint16_t((memory::IsHandle(address + 76) ? uint16_t(memory::OwnerLoad(address + 76, 2)) : guest::LoadU16(address + 76))); }
  void set_pitch(uint16_t v) const { if (auto* d = direct()) { d->pitch = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 2, v); else guest::StoreU16(address + 76, v); }
  uint32_t volume_address() const { return address + 80; }
  uint32_t volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->volume); return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  float volume() const { return std::bit_cast<float>(volume_bits()); }
  void set_volume(float v) const { set_volume_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarSoundGearData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t wait_ticks = 0;  // +48
  uint32_t slot = 0;  // +52
};
#pragma pack(pop)
static_assert(sizeof(CarSoundGearData) == 56);
inline constexpr memory::LayoutField kCarSoundGearFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}};
inline constexpr memory::Layout kCarSoundGearLayout = {kCarSoundGearFields, 5};

struct CarSoundGear : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundGearData* data() const { return static_cast<CarSoundGearData*>(memory::HandleObject(address)); }
  static CarSoundGear New() {
    return CarSoundGear(memory::MakeHandle(new CarSoundGearData(), sizeof(CarSoundGearData), "CarSoundGear", &kCarSoundGearLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundGear NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundGearData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundGearData*>(::operator new(sizeof(CarSoundGearData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundGearData();
    return CarSoundGear(memory::MakeHandle(p, sizeof(CarSoundGearData) * count, "CarSoundGear", &kCarSoundGearLayout, sizeof(CarSoundGearData)));
  }
  CarSoundGear at(uint32_t i) const { return CarSoundGear(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundGearData* direct() const { return static_cast<CarSoundGearData*>(memory::HandleOwn(address, &kCarSoundGearLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundGearLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundGearLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // 32 when played, down by one while the vehicle's byte +2948 is clear
  uint32_t wait_ticks_address() const { return address + 48; }
  uint32_t wait_ticks_bits() const { if (auto* d = direct()) return d->wait_ticks; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_wait_ticks_bits(uint32_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t wait_ticks() const { if (auto* d = direct()) return d->wait_ticks; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_wait_ticks(uint32_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // -1 none
  uint32_t slot_address() const { return address + 52; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
};

// embedded; measure: CarSound@+792
#pragma pack(push, 1)
struct CarSoundRollData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarSoundRollData) == 48);
inline constexpr memory::LayoutField kCarSoundRollFields[] = {{0, 4}, {20, 4}, {40, 4}};
inline constexpr memory::Layout kCarSoundRollLayout = {kCarSoundRollFields, 3};

struct CarSoundRoll : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundRollData* data() const { return static_cast<CarSoundRollData*>(memory::HandleObject(address)); }
  static CarSoundRoll New() {
    return CarSoundRoll(memory::MakeHandle(new CarSoundRollData(), sizeof(CarSoundRollData), "CarSoundRoll", &kCarSoundRollLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundRoll NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundRollData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundRollData*>(::operator new(sizeof(CarSoundRollData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundRollData();
    return CarSoundRoll(memory::MakeHandle(p, sizeof(CarSoundRollData) * count, "CarSoundRoll", &kCarSoundRollLayout, sizeof(CarSoundRollData)));
  }
  CarSoundRoll at(uint32_t i) const { return CarSoundRoll(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundRollData* direct() const { return static_cast<CarSoundRollData*>(memory::HandleOwn(address, &kCarSoundRollLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundRollLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundRollLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder (+4 the voice)
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarSoundEventsData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t wave18_wait = 0;  // +45
  uint8_t pad_46[2] = {};
};
#pragma pack(pop)
static_assert(sizeof(CarSoundEventsData) == 48);
inline constexpr memory::LayoutField kCarSoundEventsFields[] = {{0, 4}, {20, 4}, {40, 4}};
inline constexpr memory::Layout kCarSoundEventsLayout = {kCarSoundEventsFields, 3};

struct CarSoundEvents : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundEventsData* data() const { return static_cast<CarSoundEventsData*>(memory::HandleObject(address)); }
  static CarSoundEvents New() {
    return CarSoundEvents(memory::MakeHandle(new CarSoundEventsData(), sizeof(CarSoundEventsData), "CarSoundEvents", &kCarSoundEventsLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundEvents NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundEventsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundEventsData*>(::operator new(sizeof(CarSoundEventsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundEventsData();
    return CarSoundEvents(memory::MakeHandle(p, sizeof(CarSoundEventsData) * count, "CarSoundEvents", &kCarSoundEventsLayout, sizeof(CarSoundEventsData)));
  }
  CarSoundEvents at(uint32_t i) const { return CarSoundEvents(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundEventsData* direct() const { return static_cast<CarSoundEventsData*>(memory::HandleOwn(address, &kCarSoundEventsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundEventsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundEventsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // 2 when wave 18 played, down by one a tick
  uint32_t wave18_wait_address() const { return address + 45; }
  uint8_t wave18_wait_bits() const { if (auto* d = direct()) return d->wave18_wait; return uint8_t((memory::IsHandle(address + 45) ? uint8_t(memory::OwnerLoad(address + 45, 1)) : guest::LoadU8(address + 45))); }
  void set_wave18_wait_bits(uint8_t v) const { if (auto* d = direct()) { d->wave18_wait = v; return; } if (memory::IsHandle(address + 45)) memory::OwnerStore(address + 45, 1, v); else guest::StoreU8(address + 45, v); }
  uint8_t wave18_wait() const { if (auto* d = direct()) return d->wave18_wait; return uint8_t((memory::IsHandle(address + 45) ? uint8_t(memory::OwnerLoad(address + 45, 1)) : guest::LoadU8(address + 45))); }
  void set_wave18_wait(uint8_t v) const { if (auto* d = direct()) { d->wave18_wait = v; return; } if (memory::IsHandle(address + 45)) memory::OwnerStore(address + 45, 1, v); else guest::StoreU8(address + 45, v); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct CarSoundFadeData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t playing = 0;  // +48
  uint32_t slot = 0;  // +52
  float fade_out = 0;  // +56
  float fade_in = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(CarSoundFadeData) == 64);
inline constexpr memory::LayoutField kCarSoundFadeFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}};
inline constexpr memory::Layout kCarSoundFadeLayout = {kCarSoundFadeFields, 7};

struct CarSoundFade : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundFadeData* data() const { return static_cast<CarSoundFadeData*>(memory::HandleObject(address)); }
  static CarSoundFade New() {
    return CarSoundFade(memory::MakeHandle(new CarSoundFadeData(), sizeof(CarSoundFadeData), "CarSoundFade", &kCarSoundFadeLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundFade NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundFadeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundFadeData*>(::operator new(sizeof(CarSoundFadeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundFadeData();
    return CarSoundFade(memory::MakeHandle(p, sizeof(CarSoundFadeData) * count, "CarSoundFade", &kCarSoundFadeLayout, sizeof(CarSoundFadeData)));
  }
  CarSoundFade at(uint32_t i) const { return CarSoundFade(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundFadeData* direct() const { return static_cast<CarSoundFadeData*>(memory::HandleOwn(address, &kCarSoundFadeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundFadeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundFadeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint32_t playing_address() const { return address + 48; }
  uint32_t playing_bits() const { if (auto* d = direct()) return d->playing; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_playing_bits(uint32_t v) const { if (auto* d = direct()) { d->playing = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t playing() const { if (auto* d = direct()) return d->playing; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_playing(uint32_t v) const { if (auto* d = direct()) { d->playing = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // -1 none (used without a check)
  uint32_t slot_address() const { return address + 52; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  // -1.0 none
  uint32_t fade_out_address() const { return address + 56; }
  uint32_t fade_out_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->fade_out); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_fade_out_bits(uint32_t v) const { if (auto* d = direct()) { d->fade_out = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float fade_out() const { return std::bit_cast<float>(fade_out_bits()); }
  void set_fade_out(float v) const { set_fade_out_bits(std::bit_cast<uint32_t>(v)); }
  // -1.0 none
  uint32_t fade_in_address() const { return address + 60; }
  uint32_t fade_in_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->fade_in); return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_fade_in_bits(uint32_t v) const { if (auto* d = direct()) { d->fade_in = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  float fade_in() const { return std::bit_cast<float>(fade_in_bits()); }
  void set_fade_in(float v) const { set_fade_in_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: CarSound@+952
#pragma pack(push, 1)
struct CarSoundTurboData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t slot = 0;  // +48
  float last_boost = 0;  // +52
  uint16_t pitch = 0;  // +56
  uint8_t wait_ticks = 0;  // +58
  uint8_t volume_steps = 0;  // +59
  float volume = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(CarSoundTurboData) == 64);
inline constexpr memory::LayoutField kCarSoundTurboFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}, {56, 2}, {60, 4}};
inline constexpr memory::Layout kCarSoundTurboLayout = {kCarSoundTurboFields, 7};

struct CarSoundTurbo : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundTurboData* data() const { return static_cast<CarSoundTurboData*>(memory::HandleObject(address)); }
  static CarSoundTurbo New() {
    return CarSoundTurbo(memory::MakeHandle(new CarSoundTurboData(), sizeof(CarSoundTurboData), "CarSoundTurbo", &kCarSoundTurboLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundTurbo NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundTurboData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundTurboData*>(::operator new(sizeof(CarSoundTurboData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundTurboData();
    return CarSoundTurbo(memory::MakeHandle(p, sizeof(CarSoundTurboData) * count, "CarSoundTurbo", &kCarSoundTurboLayout, sizeof(CarSoundTurboData)));
  }
  CarSoundTurbo at(uint32_t i) const { return CarSoundTurbo(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundTurboData* direct() const { return static_cast<CarSoundTurboData*>(memory::HandleOwn(address, &kCarSoundTurboLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundTurboLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundTurboLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // the looping wave 3's slot, -1 none
  uint32_t slot_address() const { return address + 48; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t last_boost_address() const { return address + 52; }
  uint32_t last_boost_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->last_boost); return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_last_boost_bits(uint32_t v) const { if (auto* d = direct()) { d->last_boost = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  float last_boost() const { return std::bit_cast<float>(last_boost_bits()); }
  void set_last_boost(float v) const { set_last_boost_bits(std::bit_cast<uint32_t>(v)); }
  // 0 to 3072 (+ 8192 for SetPitch)
  uint32_t pitch_address() const { return address + 56; }
  uint16_t pitch_bits() const { if (auto* d = direct()) return d->pitch; return uint16_t((memory::IsHandle(address + 56) ? uint16_t(memory::OwnerLoad(address + 56, 2)) : guest::LoadU16(address + 56))); }
  void set_pitch_bits(uint16_t v) const { if (auto* d = direct()) { d->pitch = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 2, v); else guest::StoreU16(address + 56, v); }
  uint16_t pitch() const { if (auto* d = direct()) return d->pitch; return uint16_t((memory::IsHandle(address + 56) ? uint16_t(memory::OwnerLoad(address + 56, 2)) : guest::LoadU16(address + 56))); }
  void set_pitch(uint16_t v) const { if (auto* d = direct()) { d->pitch = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 2, v); else guest::StoreU16(address + 56, v); }
  // 16 when started, counted down
  uint32_t wait_ticks_address() const { return address + 58; }
  uint8_t wait_ticks_bits() const { if (auto* d = direct()) return d->wait_ticks; return uint8_t((memory::IsHandle(address + 58) ? uint8_t(memory::OwnerLoad(address + 58, 1)) : guest::LoadU8(address + 58))); }
  void set_wait_ticks_bits(uint8_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 58)) memory::OwnerStore(address + 58, 1, v); else guest::StoreU8(address + 58, v); }
  uint8_t wait_ticks() const { if (auto* d = direct()) return d->wait_ticks; return uint8_t((memory::IsHandle(address + 58) ? uint8_t(memory::OwnerLoad(address + 58, 1)) : guest::LoadU8(address + 58))); }
  void set_wait_ticks(uint8_t v) const { if (auto* d = direct()) { d->wait_ticks = v; return; } if (memory::IsHandle(address + 58)) memory::OwnerStore(address + 58, 1, v); else guest::StoreU8(address + 58, v); }
  uint32_t volume_steps_address() const { return address + 59; }
  uint8_t volume_steps_bits() const { if (auto* d = direct()) return d->volume_steps; return uint8_t((memory::IsHandle(address + 59) ? uint8_t(memory::OwnerLoad(address + 59, 1)) : guest::LoadU8(address + 59))); }
  void set_volume_steps_bits(uint8_t v) const { if (auto* d = direct()) { d->volume_steps = v; return; } if (memory::IsHandle(address + 59)) memory::OwnerStore(address + 59, 1, v); else guest::StoreU8(address + 59, v); }
  uint8_t volume_steps() const { if (auto* d = direct()) return d->volume_steps; return uint8_t((memory::IsHandle(address + 59) ? uint8_t(memory::OwnerLoad(address + 59, 1)) : guest::LoadU8(address + 59))); }
  void set_volume_steps(uint8_t v) const { if (auto* d = direct()) { d->volume_steps = v; return; } if (memory::IsHandle(address + 59)) memory::OwnerStore(address + 59, 1, v); else guest::StoreU8(address + 59, v); }
  uint32_t volume_address() const { return address + 60; }
  uint32_t volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->volume); return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  float volume() const { return std::bit_cast<float>(volume_bits()); }
  void set_volume(float v) const { set_volume_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: CarSound@+1016
#pragma pack(push, 1)
struct CarSoundWindData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t played = 0;  // +48
  uint32_t unknown_52 = 0;  // +52
};
#pragma pack(pop)
static_assert(sizeof(CarSoundWindData) == 56);
inline constexpr memory::LayoutField kCarSoundWindFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}};
inline constexpr memory::Layout kCarSoundWindLayout = {kCarSoundWindFields, 5};

struct CarSoundWind : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundWindData* data() const { return static_cast<CarSoundWindData*>(memory::HandleObject(address)); }
  static CarSoundWind New() {
    return CarSoundWind(memory::MakeHandle(new CarSoundWindData(), sizeof(CarSoundWindData), "CarSoundWind", &kCarSoundWindLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundWind NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundWindData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundWindData*>(::operator new(sizeof(CarSoundWindData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundWindData();
    return CarSoundWind(memory::MakeHandle(p, sizeof(CarSoundWindData) * count, "CarSoundWind", &kCarSoundWindLayout, sizeof(CarSoundWindData)));
  }
  CarSoundWind at(uint32_t i) const { return CarSoundWind(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundWindData* direct() const { return static_cast<CarSoundWindData*>(memory::HandleOwn(address, &kCarSoundWindLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundWindLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundWindLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // 1 once wave 5 played
  uint32_t played_address() const { return address + 48; }
  uint32_t played_bits() const { if (auto* d = direct()) return d->played; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_played_bits(uint32_t v) const { if (auto* d = direct()) { d->played = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t played() const { if (auto* d = direct()) return d->played; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_played(uint32_t v) const { if (auto* d = direct()) { d->played = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // -1 from CarSound::Start; not read
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_52_bits() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t unknown_52() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
};

// embedded; measure: CarSound@+1072
#pragma pack(push, 1)
struct CarSoundHornData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t slot = 0;  // +48
  uint32_t slot_2 = 0;  // +52
  uint32_t playing = 0;  // +56
  uint32_t playing_2 = 0;  // +60
};
#pragma pack(pop)
static_assert(sizeof(CarSoundHornData) == 64);
inline constexpr memory::LayoutField kCarSoundHornFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}};
inline constexpr memory::Layout kCarSoundHornLayout = {kCarSoundHornFields, 7};

struct CarSoundHorn : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundHornData* data() const { return static_cast<CarSoundHornData*>(memory::HandleObject(address)); }
  static CarSoundHorn New() {
    return CarSoundHorn(memory::MakeHandle(new CarSoundHornData(), sizeof(CarSoundHornData), "CarSoundHorn", &kCarSoundHornLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundHorn NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundHornData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundHornData*>(::operator new(sizeof(CarSoundHornData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundHornData();
    return CarSoundHorn(memory::MakeHandle(p, sizeof(CarSoundHornData) * count, "CarSoundHorn", &kCarSoundHornLayout, sizeof(CarSoundHornData)));
  }
  CarSoundHorn at(uint32_t i) const { return CarSoundHorn(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundHornData* direct() const { return static_cast<CarSoundHornData*>(memory::HandleOwn(address, &kCarSoundHornLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundHornLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundHornLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // -1 none
  uint32_t slot_address() const { return address + 48; }
  uint32_t slot_bits() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_slot_bits(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t slot() const { if (auto* d = direct()) return d->slot; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_slot(uint32_t v) const { if (auto* d = direct()) { d->slot = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // race mode 8's other record's
  uint32_t slot_2_address() const { return address + 52; }
  uint32_t slot_2_bits() const { if (auto* d = direct()) return d->slot_2; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot_2_bits(uint32_t v) const { if (auto* d = direct()) { d->slot_2 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t slot_2() const { if (auto* d = direct()) return d->slot_2; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_slot_2(uint32_t v) const { if (auto* d = direct()) { d->slot_2 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t playing_address() const { return address + 56; }
  uint32_t playing_bits() const { if (auto* d = direct()) return d->playing; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_playing_bits(uint32_t v) const { if (auto* d = direct()) { d->playing = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t playing() const { if (auto* d = direct()) return d->playing; return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_playing(uint32_t v) const { if (auto* d = direct()) { d->playing = v; return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  uint32_t playing_2_address() const { return address + 60; }
  uint32_t playing_2_bits() const { if (auto* d = direct()) return d->playing_2; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_playing_2_bits(uint32_t v) const { if (auto* d = direct()) { d->playing_2 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t playing_2() const { if (auto* d = direct()) return d->playing_2; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_playing_2(uint32_t v) const { if (auto* d = direct()) { d->playing_2 = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
};

// embedded; measure: CarSound@+1136
#pragma pack(push, 1)
struct CarSoundPassByData {
  uint8_t holder[40] = {};  // +0, guest order
  uint32_t view = 0;  // +40
  uint8_t index = 0;  // +44
  uint8_t pad_45[3] = {};
  uint32_t unknown_48 = 0;  // +48
  uint32_t unknown_52 = 0;  // +52
};
#pragma pack(pop)
static_assert(sizeof(CarSoundPassByData) == 56);
inline constexpr memory::LayoutField kCarSoundPassByFields[] = {{0, 4}, {20, 4}, {40, 4}, {48, 4}, {52, 4}};
inline constexpr memory::Layout kCarSoundPassByLayout = {kCarSoundPassByFields, 5};

struct CarSoundPassBy : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  CarSoundPassByData* data() const { return static_cast<CarSoundPassByData*>(memory::HandleObject(address)); }
  static CarSoundPassBy New() {
    return CarSoundPassBy(memory::MakeHandle(new CarSoundPassByData(), sizeof(CarSoundPassByData), "CarSoundPassBy", &kCarSoundPassByLayout));
  }
  template <uint32_t Stride = kSize>
  static CarSoundPassBy NewArray(uint32_t count) {
    static_assert(sizeof(CarSoundPassByData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<CarSoundPassByData*>(::operator new(sizeof(CarSoundPassByData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) CarSoundPassByData();
    return CarSoundPassBy(memory::MakeHandle(p, sizeof(CarSoundPassByData) * count, "CarSoundPassBy", &kCarSoundPassByLayout, sizeof(CarSoundPassByData)));
  }
  CarSoundPassBy at(uint32_t i) const { return CarSoundPassBy(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  CarSoundPassByData* direct() const { return static_cast<CarSoundPassByData*>(memory::HandleOwn(address, &kCarSoundPassByLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kCarSoundPassByLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kCarSoundPassByLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // a CarSoundVoiceHolder
  uint32_t holder_address() const { return address + 0; }
  uint8_t* holder() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 0 : guest::Pointer(address + 0); }
  template <typename T = CarSoundVoiceHolder>
  T voice_holder() const { return T(address + 0); }
  uint32_t view_address() const { return address + 40; }
  uint32_t view_bits() const { if (auto* d = direct()) return d->view; return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_view_bits(uint32_t v) const { if (auto* d = direct()) { d->view = v; return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  template <typename T = CarSoundWaveBankView>
  T view() const { return T(view_bits()); }
  void set_view(const View& w) const { set_view_bits(w.address); }
  void set_view(uint32_t v) const { set_view_bits(v); }
  uint32_t index_address() const { return address + 44; }
  uint8_t index_bits() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index_bits(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  uint8_t index() const { if (auto* d = direct()) return d->index; return uint8_t((memory::IsHandle(address + 44) ? uint8_t(memory::OwnerLoad(address + 44, 1)) : guest::LoadU8(address + 44))); }
  void set_index(uint8_t v) const { if (auto* d = direct()) { d->index = v; return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 1, v); else guest::StoreU8(address + 44, v); }
  // -1 from CarSound::Start; not read
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_48_bits() const { if (auto* d = direct()) return d->unknown_48; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_unknown_48_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  uint32_t unknown_48() const { if (auto* d = direct()) return d->unknown_48; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_unknown_48(uint32_t v) const { if (auto* d = direct()) { d->unknown_48 = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  // 0 from CarSound::Start; not read
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_52_bits() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52_bits(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t unknown_52() const { if (auto* d = direct()) return d->unknown_52; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_unknown_52(uint32_t v) const { if (auto* d = direct()) { d->unknown_52 = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
};

// allocated; measure: new<?>@RaceSounds::Begin
struct CarSoundWaveBank : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  static CarSoundWaveBank New(uint32_t size) { return CarSoundWaveBank(memory::NewBlob(size, "CarSoundWaveBank")); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  // the bank data's offset
  uint32_t data_offset() const { return guest::LoadU32(address + 8); }
  void set_data_offset(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t data_offset_address() const { return address + 8; }
  // the entries' offset
  uint32_t entries_offset() const { return guest::LoadU32(address + 16); }
  void set_entries_offset(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t entries_offset_address() const { return address + 16; }
  // the wave data's offset
  uint32_t wave_data_offset() const { return guest::LoadU32(address + 40); }
  void set_wave_data_offset(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t wave_data_offset_address() const { return address + 40; }
};

// allocated; measure: WaveBankView
struct CarSoundWaveBankView : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  template <typename T = CarSoundWaveBank>
  T bank() const { return T(guest::LoadU32(address + 0)); }
  void set_bank(const View& v) const { guest::StoreU32(address + 0, v.address); }
  void set_bank(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t bank_address() const { return address + 0; }
  // 0; not read
  uint32_t unknown_4() const { return guest::LoadU32(address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  template <typename T = CarSoundWaveBank>
  T bank_copy() const { return T(guest::LoadU32(address + 8)); }
  void set_bank_copy(const View& v) const { guest::StoreU32(address + 8, v.address); }
  void set_bank_copy(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t bank_copy_address() const { return address + 8; }
  template <typename T = CarSoundWaveBankData>
  T bank_data() const { return T(guest::LoadU32(address + 12)); }
  void set_bank_data(const View& v) const { guest::StoreU32(address + 12, v.address); }
  void set_bank_data(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t bank_data_address() const { return address + 12; }
  // CarSoundWaveEntry records, 24 bytes a wave
  uint32_t entries() const { return guest::LoadU32(address + 16); }
  void set_entries(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t entries_address() const { return address + 16; }
  // the wave data's start
  uint32_t wave_data() const { return guest::LoadU32(address + 20); }
  void set_wave_data(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t wave_data_address() const { return address + 20; }
};

// allocated; measure: WaveBankView.+12
struct CarSoundWaveBankData : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  // 0x20000 compact (one format for every wave, +84)
  uint32_t flags() const { return guest::LoadU32(address + 0); }
  void set_flags(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t flags_address() const { return address + 0; }
  // the waves
  uint32_t count() const { return guest::LoadU32(address + 4); }
  void set_count(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t count_address() const { return address + 4; }
  uint32_t compact_format() const { return guest::LoadU32(address + 84); }
  void set_compact_format(uint32_t v) const { guest::StoreU32(address + 84, v); }
  uint32_t compact_format_address() const { return address + 84; }
};

// allocated; measure: WaveBankView.+16
struct CarSoundWaveEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 24;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // bits 0-1 the codec (0 PCM), 2-4 the channels, 5-22 the rate, 31 16-bit samples
  uint32_t format() const { return guest::LoadU32(address + 4); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t format_address() const { return address + 4; }
  // from the wave data's start
  uint32_t data_offset() const { return guest::LoadU32(address + 8); }
  void set_data_offset(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t data_offset_address() const { return address + 8; }
  // probably the play region's length
  uint32_t length() const { return guest::LoadU32(address + 12); }
  void set_length(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t length_address() const { return address + 12; }
  // the loop region's start, in sample frames
  uint32_t loop_start() const { return guest::LoadU32(address + 16); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t loop_start_address() const { return address + 16; }
  // the loop region's length, in sample frames
  uint32_t loop_length() const { return guest::LoadU32(address + 20); }
  void set_loop_length(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t loop_length_address() const { return address + 20; }
};

// embedded; measure: WaveBankView::Format.r5
struct CarSoundWaveFormat : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // 1 16-bit, 2 8-bit
  uint8_t sample_type() const { return guest::LoadU8(address + 0); }
  void set_sample_type(uint8_t v) const { guest::StoreU8(address + 0, v); }
  uint32_t sample_type_address() const { return address + 0; }
  uint8_t channels() const { return guest::LoadU8(address + 4); }
  void set_channels(uint8_t v) const { guest::StoreU8(address + 4, v); }
  uint32_t channels_address() const { return address + 4; }
  uint32_t rate() const { return guest::LoadU32(address + 8); }
  void set_rate(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t rate_address() const { return address + 8; }
};

// embedded; measure: WaveBankView::Length.r5, WaveBankView::Data.r5
struct CarSoundWaveWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t value() const { return guest::LoadU32(address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// image; measure: X3DAudio
struct X3DAudioHandle : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // 6 (5.1)
  uint32_t speaker_count() const { return guest::LoadU32(address + 0); }
  void set_speaker_count(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t speaker_count_address() const { return address + 0; }
  // -1 none
  uint32_t lfe_speaker() const { return guest::LoadU32(address + 4); }
  void set_lfe_speaker(uint32_t v) const { guest::StoreU32(address + 4, v); }
  uint32_t lfe_speaker_address() const { return address + 4; }
  float speed_of_sound() const { return guest::LoadFloat(address + 8); }
  void set_speed_of_sound(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t speed_of_sound_address() const { return address + 8; }
  // what the Doppler velocities are clamped to
  float doppler_limit() const { return guest::LoadFloat(address + 12); }
  void set_doppler_limit(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t doppler_limit_address() const { return address + 12; }
};

// embedded; measure: -
#pragma pack(push, 1)
struct X3DAudioListenerData {
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
  uint32_t cone = 0;  // +48
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioListenerData) == 52);
inline constexpr memory::LayoutField kX3DAudioListenerFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}};
inline constexpr memory::Layout kX3DAudioListenerLayout = {kX3DAudioListenerFields, 13};

struct X3DAudioListener : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioListenerData* data() const { return static_cast<X3DAudioListenerData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioListener New() {
    return X3DAudioListener(memory::MakeHandle(new X3DAudioListenerData(), sizeof(X3DAudioListenerData), "X3DAudioListener", &kX3DAudioListenerLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioListener NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioListenerData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioListenerData*>(::operator new(sizeof(X3DAudioListenerData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioListenerData();
    return X3DAudioListener(memory::MakeHandle(p, sizeof(X3DAudioListenerData) * count, "X3DAudioListener", &kX3DAudioListenerLayout, sizeof(X3DAudioListenerData)));
  }
  X3DAudioListener at(uint32_t i) const { return X3DAudioListener(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioListenerData* direct() const { return static_cast<X3DAudioListenerData*>(memory::HandleOwn(address, &kX3DAudioListenerLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioListenerLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioListenerLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t front_x_address() const { return address + 0; }
  uint32_t front_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_x); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_front_x_bits(uint32_t v) const { if (auto* d = direct()) { d->front_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  float front_x() const { return std::bit_cast<float>(front_x_bits()); }
  void set_front_x(float v) const { set_front_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_y_address() const { return address + 4; }
  uint32_t front_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_y); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_front_y_bits(uint32_t v) const { if (auto* d = direct()) { d->front_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float front_y() const { return std::bit_cast<float>(front_y_bits()); }
  void set_front_y(float v) const { set_front_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_z_address() const { return address + 8; }
  uint32_t front_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_z); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_front_z_bits(uint32_t v) const { if (auto* d = direct()) { d->front_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float front_z() const { return std::bit_cast<float>(front_z_bits()); }
  void set_front_z(float v) const { set_front_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_x_address() const { return address + 12; }
  uint32_t top_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_x); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_top_x_bits(uint32_t v) const { if (auto* d = direct()) { d->top_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float top_x() const { return std::bit_cast<float>(top_x_bits()); }
  void set_top_x(float v) const { set_top_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_y_address() const { return address + 16; }
  uint32_t top_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_y); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_top_y_bits(uint32_t v) const { if (auto* d = direct()) { d->top_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float top_y() const { return std::bit_cast<float>(top_y_bits()); }
  void set_top_y(float v) const { set_top_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_z_address() const { return address + 20; }
  uint32_t top_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_z); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_top_z_bits(uint32_t v) const { if (auto* d = direct()) { d->top_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float top_z() const { return std::bit_cast<float>(top_z_bits()); }
  void set_top_z(float v) const { set_top_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_x_address() const { return address + 24; }
  uint32_t position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_x); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float position_x() const { return std::bit_cast<float>(position_x_bits()); }
  void set_position_x(float v) const { set_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_y_address() const { return address + 28; }
  uint32_t position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_y); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float position_y() const { return std::bit_cast<float>(position_y_bits()); }
  void set_position_y(float v) const { set_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_z_address() const { return address + 32; }
  uint32_t position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_z); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float position_z() const { return std::bit_cast<float>(position_z_bits()); }
  void set_position_z(float v) const { set_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_x_address() const { return address + 36; }
  uint32_t velocity_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_x); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_velocity_x_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float velocity_x() const { return std::bit_cast<float>(velocity_x_bits()); }
  void set_velocity_x(float v) const { set_velocity_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_y_address() const { return address + 40; }
  uint32_t velocity_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_y); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_velocity_y_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float velocity_y() const { return std::bit_cast<float>(velocity_y_bits()); }
  void set_velocity_y(float v) const { set_velocity_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_z_address() const { return address + 44; }
  uint32_t velocity_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_z); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_velocity_z_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float velocity_z() const { return std::bit_cast<float>(velocity_z_bits()); }
  void set_velocity_z(float v) const { set_velocity_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t cone_address() const { return address + 48; }
  uint32_t cone_bits() const { if (auto* d = direct()) return d->cone; return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_cone_bits(uint32_t v) const { if (auto* d = direct()) { d->cone = v; return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  template <typename T = X3DAudioCone>
  T cone() const { return T(cone_bits()); }
  void set_cone(const View& w) const { set_cone_bits(w.address); }
  void set_cone(uint32_t v) const { set_cone_bits(v); }
};

// embedded; measure: X3DAudio::Delay.r5, X3DAudioCalculate.r5
#pragma pack(push, 1)
struct X3DAudioEmitterData {
  uint32_t cone = 0;  // +0
  float front_x = 0;  // +4
  float front_y = 0;  // +8
  float front_z = 0;  // +12
  float top_x = 0;  // +16
  float top_y = 0;  // +20
  float top_z = 0;  // +24
  float position_x = 0;  // +28
  float position_y = 0;  // +32
  float position_z = 0;  // +36
  float velocity_x = 0;  // +40
  float velocity_y = 0;  // +44
  float velocity_z = 0;  // +48
  uint32_t channel_count = 0;  // +52
  float channel_radius = 0;  // +56
  uint32_t channel_azimuths = 0;  // +60
  uint32_t volume_curve = 0;  // +64
  uint32_t lfe_curve = 0;  // +68
  uint32_t lpf_direct_curve = 0;  // +72
  uint32_t lpf_reverb_curve = 0;  // +76
  uint32_t reverb_curve = 0;  // +80
  float curve_distance_scaler = 0;  // +84
  float doppler_scaler = 0;  // +88
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioEmitterData) == 92);
inline constexpr memory::LayoutField kX3DAudioEmitterFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}, {68, 4}, {72, 4}, {76, 4}, {80, 4}, {84, 4}, {88, 4}};
inline constexpr memory::Layout kX3DAudioEmitterLayout = {kX3DAudioEmitterFields, 23};

struct X3DAudioEmitter : View {
  using View::View;
  static constexpr uint32_t kSize = 92;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioEmitterData* data() const { return static_cast<X3DAudioEmitterData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioEmitter New() {
    return X3DAudioEmitter(memory::MakeHandle(new X3DAudioEmitterData(), sizeof(X3DAudioEmitterData), "X3DAudioEmitter", &kX3DAudioEmitterLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioEmitter NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioEmitterData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioEmitterData*>(::operator new(sizeof(X3DAudioEmitterData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioEmitterData();
    return X3DAudioEmitter(memory::MakeHandle(p, sizeof(X3DAudioEmitterData) * count, "X3DAudioEmitter", &kX3DAudioEmitterLayout, sizeof(X3DAudioEmitterData)));
  }
  X3DAudioEmitter at(uint32_t i) const { return X3DAudioEmitter(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioEmitterData* direct() const { return static_cast<X3DAudioEmitterData*>(memory::HandleOwn(address, &kX3DAudioEmitterLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioEmitterLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioEmitterLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // 0 none
  uint32_t cone_address() const { return address + 0; }
  uint32_t cone_bits() const { if (auto* d = direct()) return d->cone; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_cone_bits(uint32_t v) const { if (auto* d = direct()) { d->cone = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = X3DAudioCone>
  T cone() const { return T(cone_bits()); }
  void set_cone(const View& w) const { set_cone_bits(w.address); }
  void set_cone(uint32_t v) const { set_cone_bits(v); }
  uint32_t front_x_address() const { return address + 4; }
  uint32_t front_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_x); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_front_x_bits(uint32_t v) const { if (auto* d = direct()) { d->front_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float front_x() const { return std::bit_cast<float>(front_x_bits()); }
  void set_front_x(float v) const { set_front_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_y_address() const { return address + 8; }
  uint32_t front_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_y); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_front_y_bits(uint32_t v) const { if (auto* d = direct()) { d->front_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float front_y() const { return std::bit_cast<float>(front_y_bits()); }
  void set_front_y(float v) const { set_front_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t front_z_address() const { return address + 12; }
  uint32_t front_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->front_z); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_front_z_bits(uint32_t v) const { if (auto* d = direct()) { d->front_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float front_z() const { return std::bit_cast<float>(front_z_bits()); }
  void set_front_z(float v) const { set_front_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_x_address() const { return address + 16; }
  uint32_t top_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_x); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_top_x_bits(uint32_t v) const { if (auto* d = direct()) { d->top_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float top_x() const { return std::bit_cast<float>(top_x_bits()); }
  void set_top_x(float v) const { set_top_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_y_address() const { return address + 20; }
  uint32_t top_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_y); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_top_y_bits(uint32_t v) const { if (auto* d = direct()) { d->top_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float top_y() const { return std::bit_cast<float>(top_y_bits()); }
  void set_top_y(float v) const { set_top_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t top_z_address() const { return address + 24; }
  uint32_t top_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->top_z); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_top_z_bits(uint32_t v) const { if (auto* d = direct()) { d->top_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float top_z() const { return std::bit_cast<float>(top_z_bits()); }
  void set_top_z(float v) const { set_top_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_x_address() const { return address + 28; }
  uint32_t position_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_x); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_position_x_bits(uint32_t v) const { if (auto* d = direct()) { d->position_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float position_x() const { return std::bit_cast<float>(position_x_bits()); }
  void set_position_x(float v) const { set_position_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_y_address() const { return address + 32; }
  uint32_t position_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_y); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_position_y_bits(uint32_t v) const { if (auto* d = direct()) { d->position_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float position_y() const { return std::bit_cast<float>(position_y_bits()); }
  void set_position_y(float v) const { set_position_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t position_z_address() const { return address + 36; }
  uint32_t position_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->position_z); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_position_z_bits(uint32_t v) const { if (auto* d = direct()) { d->position_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float position_z() const { return std::bit_cast<float>(position_z_bits()); }
  void set_position_z(float v) const { set_position_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_x_address() const { return address + 40; }
  uint32_t velocity_x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_x); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_velocity_x_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float velocity_x() const { return std::bit_cast<float>(velocity_x_bits()); }
  void set_velocity_x(float v) const { set_velocity_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_y_address() const { return address + 44; }
  uint32_t velocity_y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_y); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_velocity_y_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float velocity_y() const { return std::bit_cast<float>(velocity_y_bits()); }
  void set_velocity_y(float v) const { set_velocity_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t velocity_z_address() const { return address + 48; }
  uint32_t velocity_z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->velocity_z); return uint32_t((memory::IsHandle(address + 48) ? uint32_t(memory::OwnerLoad(address + 48, 4)) : guest::LoadU32(address + 48))); }
  void set_velocity_z_bits(uint32_t v) const { if (auto* d = direct()) { d->velocity_z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 48)) memory::OwnerStore(address + 48, 4, v); else guest::StoreU32(address + 48, v); }
  float velocity_z() const { return std::bit_cast<float>(velocity_z_bits()); }
  void set_velocity_z(float v) const { set_velocity_z_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t channel_count_address() const { return address + 52; }
  uint32_t channel_count_bits() const { if (auto* d = direct()) return d->channel_count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_channel_count_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t channel_count() const { if (auto* d = direct()) return d->channel_count; return uint32_t((memory::IsHandle(address + 52) ? uint32_t(memory::OwnerLoad(address + 52, 4)) : guest::LoadU32(address + 52))); }
  void set_channel_count(uint32_t v) const { if (auto* d = direct()) { d->channel_count = v; return; } if (memory::IsHandle(address + 52)) memory::OwnerStore(address + 52, 4, v); else guest::StoreU32(address + 52, v); }
  uint32_t channel_radius_address() const { return address + 56; }
  uint32_t channel_radius_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->channel_radius); return uint32_t((memory::IsHandle(address + 56) ? uint32_t(memory::OwnerLoad(address + 56, 4)) : guest::LoadU32(address + 56))); }
  void set_channel_radius_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_radius = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 56)) memory::OwnerStore(address + 56, 4, v); else guest::StoreU32(address + 56, v); }
  float channel_radius() const { return std::bit_cast<float>(channel_radius_bits()); }
  void set_channel_radius(float v) const { set_channel_radius_bits(std::bit_cast<uint32_t>(v)); }
  // a float per channel (X3DAUDIO_2PI the LFE)
  uint32_t channel_azimuths_address() const { return address + 60; }
  uint32_t channel_azimuths_bits() const { if (auto* d = direct()) return d->channel_azimuths; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_channel_azimuths_bits(uint32_t v) const { if (auto* d = direct()) { d->channel_azimuths = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  uint32_t channel_azimuths() const { if (auto* d = direct()) return d->channel_azimuths; return uint32_t((memory::IsHandle(address + 60) ? uint32_t(memory::OwnerLoad(address + 60, 4)) : guest::LoadU32(address + 60))); }
  void set_channel_azimuths(uint32_t v) const { if (auto* d = direct()) { d->channel_azimuths = v; return; } if (memory::IsHandle(address + 60)) memory::OwnerStore(address + 60, 4, v); else guest::StoreU32(address + 60, v); }
  // 0: the library's default
  uint32_t volume_curve_address() const { return address + 64; }
  uint32_t volume_curve_bits() const { if (auto* d = direct()) return d->volume_curve; return uint32_t((memory::IsHandle(address + 64) ? uint32_t(memory::OwnerLoad(address + 64, 4)) : guest::LoadU32(address + 64))); }
  void set_volume_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->volume_curve = v; return; } if (memory::IsHandle(address + 64)) memory::OwnerStore(address + 64, 4, v); else guest::StoreU32(address + 64, v); }
  template <typename T = X3DAudioCurve>
  T volume_curve() const { return T(volume_curve_bits()); }
  void set_volume_curve(const View& w) const { set_volume_curve_bits(w.address); }
  void set_volume_curve(uint32_t v) const { set_volume_curve_bits(v); }
  uint32_t lfe_curve_address() const { return address + 68; }
  uint32_t lfe_curve_bits() const { if (auto* d = direct()) return d->lfe_curve; return uint32_t((memory::IsHandle(address + 68) ? uint32_t(memory::OwnerLoad(address + 68, 4)) : guest::LoadU32(address + 68))); }
  void set_lfe_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lfe_curve = v; return; } if (memory::IsHandle(address + 68)) memory::OwnerStore(address + 68, 4, v); else guest::StoreU32(address + 68, v); }
  template <typename T = X3DAudioCurve>
  T lfe_curve() const { return T(lfe_curve_bits()); }
  void set_lfe_curve(const View& w) const { set_lfe_curve_bits(w.address); }
  void set_lfe_curve(uint32_t v) const { set_lfe_curve_bits(v); }
  uint32_t lpf_direct_curve_address() const { return address + 72; }
  uint32_t lpf_direct_curve_bits() const { if (auto* d = direct()) return d->lpf_direct_curve; return uint32_t((memory::IsHandle(address + 72) ? uint32_t(memory::OwnerLoad(address + 72, 4)) : guest::LoadU32(address + 72))); }
  void set_lpf_direct_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_direct_curve = v; return; } if (memory::IsHandle(address + 72)) memory::OwnerStore(address + 72, 4, v); else guest::StoreU32(address + 72, v); }
  template <typename T = X3DAudioCurve>
  T lpf_direct_curve() const { return T(lpf_direct_curve_bits()); }
  void set_lpf_direct_curve(const View& w) const { set_lpf_direct_curve_bits(w.address); }
  void set_lpf_direct_curve(uint32_t v) const { set_lpf_direct_curve_bits(v); }
  uint32_t lpf_reverb_curve_address() const { return address + 76; }
  uint32_t lpf_reverb_curve_bits() const { if (auto* d = direct()) return d->lpf_reverb_curve; return uint32_t((memory::IsHandle(address + 76) ? uint32_t(memory::OwnerLoad(address + 76, 4)) : guest::LoadU32(address + 76))); }
  void set_lpf_reverb_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_reverb_curve = v; return; } if (memory::IsHandle(address + 76)) memory::OwnerStore(address + 76, 4, v); else guest::StoreU32(address + 76, v); }
  template <typename T = X3DAudioCurve>
  T lpf_reverb_curve() const { return T(lpf_reverb_curve_bits()); }
  void set_lpf_reverb_curve(const View& w) const { set_lpf_reverb_curve_bits(w.address); }
  void set_lpf_reverb_curve(uint32_t v) const { set_lpf_reverb_curve_bits(v); }
  uint32_t reverb_curve_address() const { return address + 80; }
  uint32_t reverb_curve_bits() const { if (auto* d = direct()) return d->reverb_curve; return uint32_t((memory::IsHandle(address + 80) ? uint32_t(memory::OwnerLoad(address + 80, 4)) : guest::LoadU32(address + 80))); }
  void set_reverb_curve_bits(uint32_t v) const { if (auto* d = direct()) { d->reverb_curve = v; return; } if (memory::IsHandle(address + 80)) memory::OwnerStore(address + 80, 4, v); else guest::StoreU32(address + 80, v); }
  template <typename T = X3DAudioCurve>
  T reverb_curve() const { return T(reverb_curve_bits()); }
  void set_reverb_curve(const View& w) const { set_reverb_curve_bits(w.address); }
  void set_reverb_curve(uint32_t v) const { set_reverb_curve_bits(v); }
  uint32_t curve_distance_scaler_address() const { return address + 84; }
  uint32_t curve_distance_scaler_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->curve_distance_scaler); return uint32_t((memory::IsHandle(address + 84) ? uint32_t(memory::OwnerLoad(address + 84, 4)) : guest::LoadU32(address + 84))); }
  void set_curve_distance_scaler_bits(uint32_t v) const { if (auto* d = direct()) { d->curve_distance_scaler = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 84)) memory::OwnerStore(address + 84, 4, v); else guest::StoreU32(address + 84, v); }
  float curve_distance_scaler() const { return std::bit_cast<float>(curve_distance_scaler_bits()); }
  void set_curve_distance_scaler(float v) const { set_curve_distance_scaler_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t doppler_scaler_address() const { return address + 88; }
  uint32_t doppler_scaler_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->doppler_scaler); return uint32_t((memory::IsHandle(address + 88) ? uint32_t(memory::OwnerLoad(address + 88, 4)) : guest::LoadU32(address + 88))); }
  void set_doppler_scaler_bits(uint32_t v) const { if (auto* d = direct()) { d->doppler_scaler = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 88)) memory::OwnerStore(address + 88, 4, v); else guest::StoreU32(address + 88, v); }
  float doppler_scaler() const { return std::bit_cast<float>(doppler_scaler_bits()); }
  void set_doppler_scaler(float v) const { set_doppler_scaler_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: CarSound.+52, X3DAudio::Delay.r5.+0
#pragma pack(push, 1)
struct X3DAudioConeData {
  float inner_angle = 0;  // +0
  float outer_angle = 0;  // +4
  float inner_volume = 0;  // +8
  float outer_volume = 0;  // +12
  float inner_lpf = 0;  // +16
  float outer_lpf = 0;  // +20
  float inner_reverb = 0;  // +24
  float outer_reverb = 0;  // +28
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioConeData) == 32);
inline constexpr memory::LayoutField kX3DAudioConeFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}};
inline constexpr memory::Layout kX3DAudioConeLayout = {kX3DAudioConeFields, 8};

struct X3DAudioCone : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioConeData* data() const { return static_cast<X3DAudioConeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioCone New() {
    return X3DAudioCone(memory::MakeHandle(new X3DAudioConeData(), sizeof(X3DAudioConeData), "X3DAudioCone", &kX3DAudioConeLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioCone NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioConeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioConeData*>(::operator new(sizeof(X3DAudioConeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioConeData();
    return X3DAudioCone(memory::MakeHandle(p, sizeof(X3DAudioConeData) * count, "X3DAudioCone", &kX3DAudioConeLayout, sizeof(X3DAudioConeData)));
  }
  X3DAudioCone at(uint32_t i) const { return X3DAudioCone(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioConeData* direct() const { return static_cast<X3DAudioConeData*>(memory::HandleOwn(address, &kX3DAudioConeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioConeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioConeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t inner_angle_address() const { return address + 0; }
  uint32_t inner_angle_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inner_angle); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_inner_angle_bits(uint32_t v) const { if (auto* d = direct()) { d->inner_angle = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  float inner_angle() const { return std::bit_cast<float>(inner_angle_bits()); }
  void set_inner_angle(float v) const { set_inner_angle_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t outer_angle_address() const { return address + 4; }
  uint32_t outer_angle_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->outer_angle); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_outer_angle_bits(uint32_t v) const { if (auto* d = direct()) { d->outer_angle = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float outer_angle() const { return std::bit_cast<float>(outer_angle_bits()); }
  void set_outer_angle(float v) const { set_outer_angle_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inner_volume_address() const { return address + 8; }
  uint32_t inner_volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inner_volume); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_inner_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->inner_volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float inner_volume() const { return std::bit_cast<float>(inner_volume_bits()); }
  void set_inner_volume(float v) const { set_inner_volume_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t outer_volume_address() const { return address + 12; }
  uint32_t outer_volume_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->outer_volume); return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_outer_volume_bits(uint32_t v) const { if (auto* d = direct()) { d->outer_volume = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  float outer_volume() const { return std::bit_cast<float>(outer_volume_bits()); }
  void set_outer_volume(float v) const { set_outer_volume_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inner_lpf_address() const { return address + 16; }
  uint32_t inner_lpf_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inner_lpf); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_inner_lpf_bits(uint32_t v) const { if (auto* d = direct()) { d->inner_lpf = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float inner_lpf() const { return std::bit_cast<float>(inner_lpf_bits()); }
  void set_inner_lpf(float v) const { set_inner_lpf_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t outer_lpf_address() const { return address + 20; }
  uint32_t outer_lpf_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->outer_lpf); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_outer_lpf_bits(uint32_t v) const { if (auto* d = direct()) { d->outer_lpf = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float outer_lpf() const { return std::bit_cast<float>(outer_lpf_bits()); }
  void set_outer_lpf(float v) const { set_outer_lpf_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t inner_reverb_address() const { return address + 24; }
  uint32_t inner_reverb_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->inner_reverb); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_inner_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->inner_reverb = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float inner_reverb() const { return std::bit_cast<float>(inner_reverb_bits()); }
  void set_inner_reverb(float v) const { set_inner_reverb_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t outer_reverb_address() const { return address + 28; }
  uint32_t outer_reverb_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->outer_reverb); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_outer_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->outer_reverb = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float outer_reverb() const { return std::bit_cast<float>(outer_reverb_bits()); }
  void set_outer_reverb(float v) const { set_outer_reverb_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: -
#pragma pack(push, 1)
struct X3DAudioCurveData {
  uint32_t points = 0;  // +0
  uint32_t count = 0;  // +4
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioCurveData) == 8);
inline constexpr memory::LayoutField kX3DAudioCurveFields[] = {{0, 4}, {4, 4}};
inline constexpr memory::Layout kX3DAudioCurveLayout = {kX3DAudioCurveFields, 2};

struct X3DAudioCurve : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioCurveData* data() const { return static_cast<X3DAudioCurveData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioCurve New() {
    return X3DAudioCurve(memory::MakeHandle(new X3DAudioCurveData(), sizeof(X3DAudioCurveData), "X3DAudioCurve", &kX3DAudioCurveLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioCurve NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioCurveData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioCurveData*>(::operator new(sizeof(X3DAudioCurveData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioCurveData();
    return X3DAudioCurve(memory::MakeHandle(p, sizeof(X3DAudioCurveData) * count, "X3DAudioCurve", &kX3DAudioCurveLayout, sizeof(X3DAudioCurveData)));
  }
  X3DAudioCurve at(uint32_t i) const { return X3DAudioCurve(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioCurveData* direct() const { return static_cast<X3DAudioCurveData*>(memory::HandleOwn(address, &kX3DAudioCurveLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioCurveLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioCurveLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // {distance, value} float pairs
  uint32_t points_address() const { return address + 0; }
  uint32_t points_bits() const { if (auto* d = direct()) return d->points; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_points_bits(uint32_t v) const { if (auto* d = direct()) { d->points = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t points() const { if (auto* d = direct()) return d->points; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_points(uint32_t v) const { if (auto* d = direct()) { d->points = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  uint32_t count_address() const { return address + 4; }
  uint32_t count_bits() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_count_bits(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  uint32_t count() const { if (auto* d = direct()) return d->count; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_count(uint32_t v) const { if (auto* d = direct()) { d->count = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
};

// embedded; measure: X3DAudio::Delay.r6, X3DAudioCalculate.r7
#pragma pack(push, 1)
struct X3DAudioDspSettingsData {
  uint32_t matrix = 0;  // +0
  uint32_t delays = 0;  // +4
  uint32_t source_channels = 0;  // +8
  uint32_t destination_channels = 0;  // +12
  float lpf_direct = 0;  // +16
  float lpf_reverb = 0;  // +20
  float reverb_level = 0;  // +24
  float doppler_factor = 0;  // +28
  float emitter_angle = 0;  // +32
  float distance = 0;  // +36
  float emitter_velocity = 0;  // +40
  float listener_velocity = 0;  // +44
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioDspSettingsData) == 48);
inline constexpr memory::LayoutField kX3DAudioDspSettingsFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}};
inline constexpr memory::Layout kX3DAudioDspSettingsLayout = {kX3DAudioDspSettingsFields, 12};

struct X3DAudioDspSettings : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioDspSettingsData* data() const { return static_cast<X3DAudioDspSettingsData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioDspSettings New() {
    return X3DAudioDspSettings(memory::MakeHandle(new X3DAudioDspSettingsData(), sizeof(X3DAudioDspSettingsData), "X3DAudioDspSettings", &kX3DAudioDspSettingsLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioDspSettings NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioDspSettingsData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioDspSettingsData*>(::operator new(sizeof(X3DAudioDspSettingsData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioDspSettingsData();
    return X3DAudioDspSettings(memory::MakeHandle(p, sizeof(X3DAudioDspSettingsData) * count, "X3DAudioDspSettings", &kX3DAudioDspSettingsLayout, sizeof(X3DAudioDspSettingsData)));
  }
  X3DAudioDspSettings at(uint32_t i) const { return X3DAudioDspSettings(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioDspSettingsData* direct() const { return static_cast<X3DAudioDspSettingsData*>(memory::HandleOwn(address, &kX3DAudioDspSettingsLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioDspSettingsLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioDspSettingsLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  // destination rows of source-channel floats
  uint32_t matrix_address() const { return address + 0; }
  uint32_t matrix_bits() const { if (auto* d = direct()) return d->matrix; return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_matrix_bits(uint32_t v) const { if (auto* d = direct()) { d->matrix = v; return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  template <typename T = X3DAudioMatrix>
  T matrix() const { return T(matrix_bits()); }
  void set_matrix(const View& w) const { set_matrix_bits(w.address); }
  void set_matrix(uint32_t v) const { set_matrix_bits(v); }
  uint32_t delays_address() const { return address + 4; }
  uint32_t delays_bits() const { if (auto* d = direct()) return d->delays; return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_delays_bits(uint32_t v) const { if (auto* d = direct()) { d->delays = v; return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  template <typename T = X3DAudioDelays>
  T delays() const { return T(delays_bits()); }
  void set_delays(const View& w) const { set_delays_bits(w.address); }
  void set_delays(uint32_t v) const { set_delays_bits(v); }
  uint32_t source_channels_address() const { return address + 8; }
  uint32_t source_channels_bits() const { if (auto* d = direct()) return d->source_channels; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_source_channels_bits(uint32_t v) const { if (auto* d = direct()) { d->source_channels = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t source_channels() const { if (auto* d = direct()) return d->source_channels; return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_source_channels(uint32_t v) const { if (auto* d = direct()) { d->source_channels = v; return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  uint32_t destination_channels_address() const { return address + 12; }
  uint32_t destination_channels_bits() const { if (auto* d = direct()) return d->destination_channels; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_destination_channels_bits(uint32_t v) const { if (auto* d = direct()) { d->destination_channels = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t destination_channels() const { if (auto* d = direct()) return d->destination_channels; return uint32_t((memory::IsHandle(address + 12) ? uint32_t(memory::OwnerLoad(address + 12, 4)) : guest::LoadU32(address + 12))); }
  void set_destination_channels(uint32_t v) const { if (auto* d = direct()) { d->destination_channels = v; return; } if (memory::IsHandle(address + 12)) memory::OwnerStore(address + 12, 4, v); else guest::StoreU32(address + 12, v); }
  uint32_t lpf_direct_address() const { return address + 16; }
  uint32_t lpf_direct_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->lpf_direct); return uint32_t((memory::IsHandle(address + 16) ? uint32_t(memory::OwnerLoad(address + 16, 4)) : guest::LoadU32(address + 16))); }
  void set_lpf_direct_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_direct = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 16)) memory::OwnerStore(address + 16, 4, v); else guest::StoreU32(address + 16, v); }
  float lpf_direct() const { return std::bit_cast<float>(lpf_direct_bits()); }
  void set_lpf_direct(float v) const { set_lpf_direct_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t lpf_reverb_address() const { return address + 20; }
  uint32_t lpf_reverb_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->lpf_reverb); return uint32_t((memory::IsHandle(address + 20) ? uint32_t(memory::OwnerLoad(address + 20, 4)) : guest::LoadU32(address + 20))); }
  void set_lpf_reverb_bits(uint32_t v) const { if (auto* d = direct()) { d->lpf_reverb = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 20)) memory::OwnerStore(address + 20, 4, v); else guest::StoreU32(address + 20, v); }
  float lpf_reverb() const { return std::bit_cast<float>(lpf_reverb_bits()); }
  void set_lpf_reverb(float v) const { set_lpf_reverb_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t reverb_level_address() const { return address + 24; }
  uint32_t reverb_level_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->reverb_level); return uint32_t((memory::IsHandle(address + 24) ? uint32_t(memory::OwnerLoad(address + 24, 4)) : guest::LoadU32(address + 24))); }
  void set_reverb_level_bits(uint32_t v) const { if (auto* d = direct()) { d->reverb_level = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 24)) memory::OwnerStore(address + 24, 4, v); else guest::StoreU32(address + 24, v); }
  float reverb_level() const { return std::bit_cast<float>(reverb_level_bits()); }
  void set_reverb_level(float v) const { set_reverb_level_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t doppler_factor_address() const { return address + 28; }
  uint32_t doppler_factor_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->doppler_factor); return uint32_t((memory::IsHandle(address + 28) ? uint32_t(memory::OwnerLoad(address + 28, 4)) : guest::LoadU32(address + 28))); }
  void set_doppler_factor_bits(uint32_t v) const { if (auto* d = direct()) { d->doppler_factor = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 28)) memory::OwnerStore(address + 28, 4, v); else guest::StoreU32(address + 28, v); }
  float doppler_factor() const { return std::bit_cast<float>(doppler_factor_bits()); }
  void set_doppler_factor(float v) const { set_doppler_factor_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t emitter_angle_address() const { return address + 32; }
  uint32_t emitter_angle_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_angle); return uint32_t((memory::IsHandle(address + 32) ? uint32_t(memory::OwnerLoad(address + 32, 4)) : guest::LoadU32(address + 32))); }
  void set_emitter_angle_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_angle = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 32)) memory::OwnerStore(address + 32, 4, v); else guest::StoreU32(address + 32, v); }
  float emitter_angle() const { return std::bit_cast<float>(emitter_angle_bits()); }
  void set_emitter_angle(float v) const { set_emitter_angle_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t distance_address() const { return address + 36; }
  uint32_t distance_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->distance); return uint32_t((memory::IsHandle(address + 36) ? uint32_t(memory::OwnerLoad(address + 36, 4)) : guest::LoadU32(address + 36))); }
  void set_distance_bits(uint32_t v) const { if (auto* d = direct()) { d->distance = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 36)) memory::OwnerStore(address + 36, 4, v); else guest::StoreU32(address + 36, v); }
  float distance() const { return std::bit_cast<float>(distance_bits()); }
  void set_distance(float v) const { set_distance_bits(std::bit_cast<uint32_t>(v)); }
  // the emitter's velocity along the line to the listener
  uint32_t emitter_velocity_address() const { return address + 40; }
  uint32_t emitter_velocity_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->emitter_velocity); return uint32_t((memory::IsHandle(address + 40) ? uint32_t(memory::OwnerLoad(address + 40, 4)) : guest::LoadU32(address + 40))); }
  void set_emitter_velocity_bits(uint32_t v) const { if (auto* d = direct()) { d->emitter_velocity = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 40)) memory::OwnerStore(address + 40, 4, v); else guest::StoreU32(address + 40, v); }
  float emitter_velocity() const { return std::bit_cast<float>(emitter_velocity_bits()); }
  void set_emitter_velocity(float v) const { set_emitter_velocity_bits(std::bit_cast<uint32_t>(v)); }
  // the listener's
  uint32_t listener_velocity_address() const { return address + 44; }
  uint32_t listener_velocity_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->listener_velocity); return uint32_t((memory::IsHandle(address + 44) ? uint32_t(memory::OwnerLoad(address + 44, 4)) : guest::LoadU32(address + 44))); }
  void set_listener_velocity_bits(uint32_t v) const { if (auto* d = direct()) { d->listener_velocity = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 44)) memory::OwnerStore(address + 44, 4, v); else guest::StoreU32(address + 44, v); }
  float listener_velocity() const { return std::bit_cast<float>(listener_velocity_bits()); }
  void set_listener_velocity(float v) const { set_listener_velocity_bits(std::bit_cast<uint32_t>(v)); }
};

// allocated; measure: X3DAudio::Delay.r6.+0, X3DAudio.+0
struct X3DAudioMatrix : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  float coefficient_0() const { return guest::LoadFloat(address + 0); }
  void set_coefficient_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t coefficient_0_address() const { return address + 0; }
  float coefficient_1() const { return guest::LoadFloat(address + 4); }
  void set_coefficient_1(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t coefficient_1_address() const { return address + 4; }
  float coefficient_2() const { return guest::LoadFloat(address + 8); }
  void set_coefficient_2(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t coefficient_2_address() const { return address + 8; }
  float coefficient_3() const { return guest::LoadFloat(address + 12); }
  void set_coefficient_3(float v) const { guest::StoreFloat(address + 12, v); }
  uint32_t coefficient_3_address() const { return address + 12; }
  float coefficient_4() const { return guest::LoadFloat(address + 16); }
  void set_coefficient_4(float v) const { guest::StoreFloat(address + 16, v); }
  uint32_t coefficient_4_address() const { return address + 16; }
  float coefficient_5() const { return guest::LoadFloat(address + 20); }
  void set_coefficient_5(float v) const { guest::StoreFloat(address + 20, v); }
  uint32_t coefficient_5_address() const { return address + 20; }
  float coefficient_6() const { return guest::LoadFloat(address + 24); }
  void set_coefficient_6(float v) const { guest::StoreFloat(address + 24, v); }
  uint32_t coefficient_6_address() const { return address + 24; }
  float coefficient_7() const { return guest::LoadFloat(address + 28); }
  void set_coefficient_7(float v) const { guest::StoreFloat(address + 28, v); }
  uint32_t coefficient_7_address() const { return address + 28; }
  float coefficient_8() const { return guest::LoadFloat(address + 32); }
  void set_coefficient_8(float v) const { guest::StoreFloat(address + 32, v); }
  uint32_t coefficient_8_address() const { return address + 32; }
};

// allocated; measure: X3DAudio::Delay.r6.+4
struct X3DAudioDelays : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  float delay_0() const { return guest::LoadFloat(address + 0); }
  void set_delay_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t delay_0_address() const { return address + 0; }
  float delay_1() const { return guest::LoadFloat(address + 4); }
  void set_delay_1(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t delay_1_address() const { return address + 4; }
};

// embedded; measure: X3DVec, X3DVec::Cross.r4, X3DVec::Cross.r5, X3DVec::Dot.r4, X3DVec::Sub.r4, X3DVec::Sub.r5, X3DAudio::Matrix.r4, X3DVec::Sub.r3
#pragma pack(push, 1)
struct X3DAudioVectorData {
  float x = 0;  // +0
  float y = 0;  // +4
  float z = 0;  // +8
};
#pragma pack(pop)
static_assert(sizeof(X3DAudioVectorData) == 12);
inline constexpr memory::LayoutField kX3DAudioVectorFields[] = {{0, 4}, {4, 4}, {8, 4}};
inline constexpr memory::Layout kX3DAudioVectorLayout = {kX3DAudioVectorFields, 3};

struct X3DAudioVector : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  X3DAudioVectorData* data() const { return static_cast<X3DAudioVectorData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static X3DAudioVector New() {
    return X3DAudioVector(memory::MakeHandle(new X3DAudioVectorData(), sizeof(X3DAudioVectorData), "X3DAudioVector", &kX3DAudioVectorLayout));
  }
  template <uint32_t Stride = kSize>
  static X3DAudioVector NewArray(uint32_t count) {
    static_assert(sizeof(X3DAudioVectorData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<X3DAudioVectorData*>(::operator new(sizeof(X3DAudioVectorData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) X3DAudioVectorData();
    return X3DAudioVector(memory::MakeHandle(p, sizeof(X3DAudioVectorData) * count, "X3DAudioVector", &kX3DAudioVectorLayout, sizeof(X3DAudioVectorData)));
  }
  X3DAudioVector at(uint32_t i) const { return X3DAudioVector(address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  X3DAudioVectorData* direct() const { return static_cast<X3DAudioVectorData*>(memory::HandleOwn(address, &kX3DAudioVectorLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kX3DAudioVectorLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(address + offset) : width == 2 ? guest::LoadU16(address + offset) : width == 4 ? guest::LoadU32(address + offset) : guest::LoadU64(address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kX3DAudioVectorLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(address + offset, uint32_t(v)); else guest::StoreU64(address + offset, v); }
  uint32_t x_address() const { return address + 0; }
  uint32_t x_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->x); return uint32_t((memory::IsHandle(address + 0) ? uint32_t(memory::OwnerLoad(address + 0, 4)) : guest::LoadU32(address + 0))); }
  void set_x_bits(uint32_t v) const { if (auto* d = direct()) { d->x = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 0)) memory::OwnerStore(address + 0, 4, v); else guest::StoreU32(address + 0, v); }
  float x() const { return std::bit_cast<float>(x_bits()); }
  void set_x(float v) const { set_x_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t y_address() const { return address + 4; }
  uint32_t y_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->y); return uint32_t((memory::IsHandle(address + 4) ? uint32_t(memory::OwnerLoad(address + 4, 4)) : guest::LoadU32(address + 4))); }
  void set_y_bits(uint32_t v) const { if (auto* d = direct()) { d->y = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 4)) memory::OwnerStore(address + 4, 4, v); else guest::StoreU32(address + 4, v); }
  float y() const { return std::bit_cast<float>(y_bits()); }
  void set_y(float v) const { set_y_bits(std::bit_cast<uint32_t>(v)); }
  uint32_t z_address() const { return address + 8; }
  uint32_t z_bits() const { if (auto* d = direct()) return std::bit_cast<uint32_t>(d->z); return uint32_t((memory::IsHandle(address + 8) ? uint32_t(memory::OwnerLoad(address + 8, 4)) : guest::LoadU32(address + 8))); }
  void set_z_bits(uint32_t v) const { if (auto* d = direct()) { d->z = std::bit_cast<float>(v); return; } if (memory::IsHandle(address + 8)) memory::OwnerStore(address + 8, 4, v); else guest::StoreU32(address + 8, v); }
  float z() const { return std::bit_cast<float>(z_bits()); }
  void set_z(float v) const { set_z_bits(std::bit_cast<uint32_t>(v)); }
};

// embedded; measure: CarSound@+736
struct CarSoundAt736 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// embedded; measure: CarSound@+840
struct CarSoundAt840 : View {
  using View::View;
  static constexpr uint32_t kSize = 46;
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint8_t unknown_45() const { return guest::LoadU8(address + 45); }
  void set_unknown_45(uint8_t v) const { guest::StoreU8(address + 45, v); }
  uint32_t unknown_45_address() const { return address + 45; }
};

// embedded; measure: CarSound@+888
struct CarSoundAt888 : View {
  using View::View;
  static constexpr uint32_t kSize = 64;
  uint32_t unknown_40() const { return guest::LoadU32(address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_48() const { return guest::LoadU32(address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  float unknown_56() const { return guest::LoadFloat(address + 56); }
  void set_unknown_56(float v) const { guest::StoreFloat(address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  float unknown_60() const { return guest::LoadFloat(address + 60); }
  void set_unknown_60(float v) const { guest::StoreFloat(address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
};

// embedded; measure: X3DAudio::Delay.r4@+12
struct X3DAudioVectorAt12 : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  float unknown_0() const { return guest::LoadFloat(address + 0); }
  void set_unknown_0(float v) const { guest::StoreFloat(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  float unknown_4() const { return guest::LoadFloat(address + 4); }
  void set_unknown_4(float v) const { guest::StoreFloat(address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  float unknown_8() const { return guest::LoadFloat(address + 8); }
  void set_unknown_8(float v) const { guest::StoreFloat(address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// embedded; measure: new<8864>@CarSounds::Init@+16
struct CarSoundAt16 : View {
  using View::View;
  static constexpr uint32_t kSize = 1164;
  uint32_t unknown_584() const { return guest::LoadU32(address + 584); }
  void set_unknown_584(uint32_t v) const { guest::StoreU32(address + 584, v); }
  uint32_t unknown_584_address() const { return address + 584; }
  uint32_t unknown_588() const { return guest::LoadU32(address + 588); }
  void set_unknown_588(uint32_t v) const { guest::StoreU32(address + 588, v); }
  uint32_t unknown_588_address() const { return address + 588; }
  uint32_t unknown_604() const { return guest::LoadU32(address + 604); }
  void set_unknown_604(uint32_t v) const { guest::StoreU32(address + 604, v); }
  uint32_t unknown_604_address() const { return address + 604; }
  uint32_t unknown_608() const { return guest::LoadU32(address + 608); }
  void set_unknown_608(uint32_t v) const { guest::StoreU32(address + 608, v); }
  uint32_t unknown_608_address() const { return address + 608; }
  uint32_t unknown_676() const { return guest::LoadU32(address + 676); }
  void set_unknown_676(uint32_t v) const { guest::StoreU32(address + 676, v); }
  uint32_t unknown_676_address() const { return address + 676; }
  uint32_t unknown_680() const { return guest::LoadU32(address + 680); }
  void set_unknown_680(uint32_t v) const { guest::StoreU32(address + 680, v); }
  uint32_t unknown_680_address() const { return address + 680; }
  uint32_t unknown_696() const { return guest::LoadU32(address + 696); }
  void set_unknown_696(uint32_t v) const { guest::StoreU32(address + 696, v); }
  uint32_t unknown_696_address() const { return address + 696; }
  uint32_t unknown_700() const { return guest::LoadU32(address + 700); }
  void set_unknown_700(uint32_t v) const { guest::StoreU32(address + 700, v); }
  uint32_t unknown_700_address() const { return address + 700; }
  uint32_t unknown_736() const { return guest::LoadU32(address + 736); }
  void set_unknown_736(uint32_t v) const { guest::StoreU32(address + 736, v); }
  uint32_t unknown_736_address() const { return address + 736; }
  uint32_t unknown_740() const { return guest::LoadU32(address + 740); }
  void set_unknown_740(uint32_t v) const { guest::StoreU32(address + 740, v); }
  uint32_t unknown_740_address() const { return address + 740; }
  uint32_t unknown_756() const { return guest::LoadU32(address + 756); }
  void set_unknown_756(uint32_t v) const { guest::StoreU32(address + 756, v); }
  uint32_t unknown_756_address() const { return address + 756; }
  uint32_t unknown_760() const { return guest::LoadU32(address + 760); }
  void set_unknown_760(uint32_t v) const { guest::StoreU32(address + 760, v); }
  uint32_t unknown_760_address() const { return address + 760; }
  uint32_t unknown_792() const { return guest::LoadU32(address + 792); }
  void set_unknown_792(uint32_t v) const { guest::StoreU32(address + 792, v); }
  uint32_t unknown_792_address() const { return address + 792; }
  uint32_t unknown_796() const { return guest::LoadU32(address + 796); }
  void set_unknown_796(uint32_t v) const { guest::StoreU32(address + 796, v); }
  uint32_t unknown_796_address() const { return address + 796; }
  uint32_t unknown_812() const { return guest::LoadU32(address + 812); }
  void set_unknown_812(uint32_t v) const { guest::StoreU32(address + 812, v); }
  uint32_t unknown_812_address() const { return address + 812; }
  uint32_t unknown_816() const { return guest::LoadU32(address + 816); }
  void set_unknown_816(uint32_t v) const { guest::StoreU32(address + 816, v); }
  uint32_t unknown_816_address() const { return address + 816; }
  uint32_t unknown_840() const { return guest::LoadU32(address + 840); }
  void set_unknown_840(uint32_t v) const { guest::StoreU32(address + 840, v); }
  uint32_t unknown_840_address() const { return address + 840; }
  uint32_t unknown_844() const { return guest::LoadU32(address + 844); }
  void set_unknown_844(uint32_t v) const { guest::StoreU32(address + 844, v); }
  uint32_t unknown_844_address() const { return address + 844; }
  uint32_t unknown_860() const { return guest::LoadU32(address + 860); }
  void set_unknown_860(uint32_t v) const { guest::StoreU32(address + 860, v); }
  uint32_t unknown_860_address() const { return address + 860; }
  uint32_t unknown_864() const { return guest::LoadU32(address + 864); }
  void set_unknown_864(uint32_t v) const { guest::StoreU32(address + 864, v); }
  uint32_t unknown_864_address() const { return address + 864; }
  uint32_t unknown_888() const { return guest::LoadU32(address + 888); }
  void set_unknown_888(uint32_t v) const { guest::StoreU32(address + 888, v); }
  uint32_t unknown_888_address() const { return address + 888; }
  uint32_t unknown_892() const { return guest::LoadU32(address + 892); }
  void set_unknown_892(uint32_t v) const { guest::StoreU32(address + 892, v); }
  uint32_t unknown_892_address() const { return address + 892; }
  uint32_t unknown_908() const { return guest::LoadU32(address + 908); }
  void set_unknown_908(uint32_t v) const { guest::StoreU32(address + 908, v); }
  uint32_t unknown_908_address() const { return address + 908; }
  uint32_t unknown_912() const { return guest::LoadU32(address + 912); }
  void set_unknown_912(uint32_t v) const { guest::StoreU32(address + 912, v); }
  uint32_t unknown_912_address() const { return address + 912; }
  uint32_t unknown_952() const { return guest::LoadU32(address + 952); }
  void set_unknown_952(uint32_t v) const { guest::StoreU32(address + 952, v); }
  uint32_t unknown_952_address() const { return address + 952; }
  uint32_t unknown_956() const { return guest::LoadU32(address + 956); }
  void set_unknown_956(uint32_t v) const { guest::StoreU32(address + 956, v); }
  uint32_t unknown_956_address() const { return address + 956; }
  uint32_t unknown_972() const { return guest::LoadU32(address + 972); }
  void set_unknown_972(uint32_t v) const { guest::StoreU32(address + 972, v); }
  uint32_t unknown_972_address() const { return address + 972; }
  uint32_t unknown_976() const { return guest::LoadU32(address + 976); }
  void set_unknown_976(uint32_t v) const { guest::StoreU32(address + 976, v); }
  uint32_t unknown_976_address() const { return address + 976; }
  uint32_t unknown_1016() const { return guest::LoadU32(address + 1016); }
  void set_unknown_1016(uint32_t v) const { guest::StoreU32(address + 1016, v); }
  uint32_t unknown_1016_address() const { return address + 1016; }
  uint32_t unknown_1020() const { return guest::LoadU32(address + 1020); }
  void set_unknown_1020(uint32_t v) const { guest::StoreU32(address + 1020, v); }
  uint32_t unknown_1020_address() const { return address + 1020; }
  uint32_t unknown_1036() const { return guest::LoadU32(address + 1036); }
  void set_unknown_1036(uint32_t v) const { guest::StoreU32(address + 1036, v); }
  uint32_t unknown_1036_address() const { return address + 1036; }
  uint32_t unknown_1040() const { return guest::LoadU32(address + 1040); }
  void set_unknown_1040(uint32_t v) const { guest::StoreU32(address + 1040, v); }
  uint32_t unknown_1040_address() const { return address + 1040; }
  uint32_t unknown_1072() const { return guest::LoadU32(address + 1072); }
  void set_unknown_1072(uint32_t v) const { guest::StoreU32(address + 1072, v); }
  uint32_t unknown_1072_address() const { return address + 1072; }
  uint32_t unknown_1076() const { return guest::LoadU32(address + 1076); }
  void set_unknown_1076(uint32_t v) const { guest::StoreU32(address + 1076, v); }
  uint32_t unknown_1076_address() const { return address + 1076; }
  uint32_t unknown_1092() const { return guest::LoadU32(address + 1092); }
  void set_unknown_1092(uint32_t v) const { guest::StoreU32(address + 1092, v); }
  uint32_t unknown_1092_address() const { return address + 1092; }
  uint32_t unknown_1096() const { return guest::LoadU32(address + 1096); }
  void set_unknown_1096(uint32_t v) const { guest::StoreU32(address + 1096, v); }
  uint32_t unknown_1096_address() const { return address + 1096; }
  uint32_t unknown_1136() const { return guest::LoadU32(address + 1136); }
  void set_unknown_1136(uint32_t v) const { guest::StoreU32(address + 1136, v); }
  uint32_t unknown_1136_address() const { return address + 1136; }
  uint32_t unknown_1140() const { return guest::LoadU32(address + 1140); }
  void set_unknown_1140(uint32_t v) const { guest::StoreU32(address + 1140, v); }
  uint32_t unknown_1140_address() const { return address + 1140; }
  uint32_t unknown_1156() const { return guest::LoadU32(address + 1156); }
  void set_unknown_1156(uint32_t v) const { guest::StoreU32(address + 1156, v); }
  uint32_t unknown_1156_address() const { return address + 1156; }
  uint32_t unknown_1160() const { return guest::LoadU32(address + 1160); }
  void set_unknown_1160(uint32_t v) const { guest::StoreU32(address + 1160, v); }
  uint32_t unknown_1160_address() const { return address + 1160; }
};

// The image words docs/functions/car-sounds.md's code uses.
struct CarSoundsGlobals {
  // seven pointers to CarInputRecords, by the car's index
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // the seven car sounds (an array, the count 16 bytes before)
  template <typename T = CarSound>
  static T sounds() { return T(guest::LoadU32(0x826C5C3Cu)); }
  static void set_sounds(const View& v) { guest::StoreU32(0x826C5C3Cu, v.address); }
  static void set_sounds(uint32_t v) { guest::StoreU32(0x826C5C3Cu, v); }
  static constexpr uint32_t sounds_address() { return 0x826C5C3Cu; }
  // 0 by CarSounds::Init; not read further
  static uint32_t unknown_826C5C40() { return guest::LoadU32(0x826C5C40u); }
  static void set_unknown_826C5C40(uint32_t v) { guest::StoreU32(0x826C5C40u, v); }
  static constexpr uint32_t unknown_826C5C40_address() { return 0x826C5C40u; }
  // the six-channel 48 kHz XAudio submix voice every car's voices send to
  static uint32_t submix() { return guest::LoadU32(0x826C5C44u); }
  static void set_submix(uint32_t v) { guest::StoreU32(0x826C5C44u, v); }
  static constexpr uint32_t submix_address() { return 0x826C5C44u; }
  // seven words cleared by SoundGlobals::Init (the scene models' sound picks)
  static constexpr uint32_t sound_picks_a_address() { return 0x826C5C48u; }
  // seven words cleared by SoundGlobals::Init (the scene models' sound picks)
  static constexpr uint32_t sound_picks_b_address() { return 0x826C5C64u; }
  // the sound manager (AudioManager, app + 124)
  static uint32_t sound_manager() { return guest::LoadU32(0x826C5C80u); }
  static void set_sound_manager(uint32_t v) { guest::StoreU32(0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 0 by SoundGlobals::Init and the race's music fades
  static uint8_t music_byte() { return guest::LoadU8(0x826C5C84u); }
  static void set_music_byte(uint8_t v) { guest::StoreU8(0x826C5C84u, v); }
  static constexpr uint32_t music_byte_address() { return 0x826C5C84u; }
  // 0 by SoundGlobals::Init
  static uint8_t unknown_826C5C85() { return guest::LoadU8(0x826C5C85u); }
  static void set_unknown_826C5C85(uint8_t v) { guest::StoreU8(0x826C5C85u, v); }
  static constexpr uint32_t unknown_826C5C85_address() { return 0x826C5C85u; }
  // bank pair 2's 3 MiB wave bank buffer (a memory block, a blob since E5 step 7)
  static uint32_t bank_pair_buffer() { return guest::LoadU32(0x826C5C88u); }
  static void set_bank_pair_buffer(uint32_t v) { guest::StoreU32(0x826C5C88u, v); }
  static constexpr uint32_t bank_pair_buffer_address() { return 0x826C5C88u; }
  // bank pair 2's 512-byte sound bank buffer (a memory block, a blob since E5 step 7)
  static uint32_t bank_pair_small_buffer() { return guest::LoadU32(0x826C5C8Cu); }
  static void set_bank_pair_small_buffer(uint32_t v) { guest::StoreU32(0x826C5C8Cu, v); }
  static constexpr uint32_t bank_pair_small_buffer_address() { return 0x826C5C8Cu; }
  // an X3DAudioHandle (X3DAudioInitialize's, 5.1)
  static constexpr uint32_t x3daudio_address() { return 0x826C5C90u; }
  // the emitter's one channel azimuth
  static float channel_azimuths() { return guest::LoadFloat(0x826C5C38u); }
  static void set_channel_azimuths(float v) { guest::StoreFloat(0x826C5C38u, v); }
  static constexpr uint32_t channel_azimuths_address() { return 0x826C5C38u; }
  // a CarInputRecord: race mode 8's other player's
  static constexpr uint32_t horn_record_address() { return 0x826C62CCu; }
  // the scene models' sound assets by block (16-byte entries): +0 a list of pointers to the cars' sound parameters
  static constexpr uint32_t sound_parameters_address() { return 0x82616318u; }
  // the music's volume (1.0 by SoundGlobals::Init)
  static float music_volume() { return guest::LoadFloat(0x82616BECu); }
  static void set_music_volume(float v) { guest::StoreFloat(0x82616BECu, v); }
  static constexpr uint32_t music_volume_address() { return 0x82616BECu; }
  // a volume, 1.0 by SoundGlobals::Init
  static float volume_c34() { return guest::LoadFloat(0x82616C34u); }
  static void set_volume_c34(float v) { guest::StoreFloat(0x82616C34u, v); }
  static constexpr uint32_t volume_c34_address() { return 0x82616C34u; }
  // the effects' volume option's gain (the car sounds' output maps times it)
  static float effects_volume() { return guest::LoadFloat(0x82616C38u); }
  static void set_effects_volume(float v) { guest::StoreFloat(0x82616C38u, v); }
  static constexpr uint32_t effects_volume_address() { return 0x82616C38u; }
  // the effects' volume option's gain
  static float effects_volume_2() { return guest::LoadFloat(0x82616C3Cu); }
  static void set_effects_volume_2(float v) { guest::StoreFloat(0x82616C3Cu, v); }
  static constexpr uint32_t effects_volume_2_address() { return 0x82616C3Cu; }
  // the submix's reverb parameters outside a tunnel (probably)
  static constexpr uint32_t reverb_off_address() { return 0x82616638u; }
  // the submix's reverb parameters in a tunnel (probably)
  static constexpr uint32_t reverb_tunnel_address() { return 0x826166F8u; }
};

}  // namespace schema
