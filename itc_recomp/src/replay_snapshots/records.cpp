// The replay snapshots' record parts (src/replay_snapshots/replay_snapshots.h,
// docs/functions/replay-snapshots.md): each original's stores and calls in
// its order, read from generated/ field by field.

#include "replay_snapshots/replay_snapshots.h"

#include "base/log.h"
#include "car_physics/car_physics.h"
#include "car_physics/float_ops.h"
#include "guest.h"
#include "native/buffers.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vectors.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace replay_snapshots {

namespace {

void Byte(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreU8(base, to, guest::LoadU8(base, from));
}
void Word(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreU32(base, to, guest::LoadU32(base, from));
}
// An ld then std pair: the eight bytes read, then written.
void Dword(uint8_t* base, uint32_t to, uint32_t from) {
  const uint32_t hi = guest::LoadU32(base, from), lo = guest::LoadU32(base, from + 4);
  guest::StoreU32(base, to, hi);
  guest::StoreU32(base, to + 4, lo);
}
// An lfs then stfs pair: the word's bits.
void FloatWord(uint8_t* base, uint32_t to, uint32_t from) {
  car_physics::ops::CopyWord(base, from, to);
}
// lhz then stw, and lwz then sth (the word's low halfword).
void HalfToWord(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreU32(base, to, guest::LoadU16(base, from));
}
void WordToHalf(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreU16(base, to, uint16_t(guest::LoadU32(base, from)));
}
void Vec4(uint8_t* base, uint32_t to, uint32_t from) {
  vector_library::Vec4Copy(base, to, from);
}
void Matrix(uint8_t* base, uint32_t to, uint32_t from) {
  car_physics::MatrixCopyRows(base, to, from);
}
void Wheel(uint8_t* base, uint32_t to, uint32_t from) {
  car_physics::WheelCopy(base, to, from);
}
void WheelRecord2(uint8_t* base, uint32_t to, uint32_t from) {
  car_physics::WheelRecord2Copy(base, to, from);
}
void TrackPosition(uint8_t* base, uint32_t to, uint32_t from) {
  vehicle::TrackPositionCopy(base, to, from);
}
// memcpy (sub_825A7410): its plain C++ (the adapters check
// native::Buffers_NativeMemcpy first), or the same bytes one by one where
// it refuses (ranges that may share memory: never for a record and a
// vehicle).
void Memcpy(uint8_t* base, uint32_t to, uint32_t from, uint32_t bytes, uint32_t caller) {
  if (native::Buffers_Memcpy(base, to, from, bytes, caller)) {
    return;
  }
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, to + i, guest::LoadU8(base, from + i));
  }
}

}  // namespace

// --- The restores ---

void Record5232RestoreTrack(uint8_t* base, uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104310 Record5232::RestoreTrack");
  const uint32_t to = vehicle, from = record;
  Word(base, to + 6148, from + 0);
  Word(base, to + 6152, from + 4);
  Memcpy(base, to + 6160, from + 16, 112, 0x82104344);
  for (uint32_t i = 0; i < 5; i++) {
    TrackPosition(base, to + 5232 + 96 * i, from + 128 + 96 * i);
  }
}

void BodyStateRestore(uint8_t* base, uint32_t state, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_821031A0 BodyState::Restore");
  const uint32_t to = body, from = state;
  Byte(base, to + 0, from + 0);
  FloatWord(base, to + 4, from + 4);
  FloatWord(base, to + 8, from + 8);
  Vec4(base, to + 16, from + 16);
  Vec4(base, to + 32, from + 32);
  FloatWord(base, to + 48, from + 48);
  Vec4(base, to + 64, from + 64);
  Dword(base, to + 80, from + 80);
  Dword(base, to + 88, from + 88);
  Vec4(base, to + 96, from + 96);
  FloatWord(base, to + 112, from + 112);
  Vec4(base, to + 128, from + 128);
  Vec4(base, to + 144, from + 144);
  FloatWord(base, to + 160, from + 160);
  FloatWord(base, to + 164, from + 164);
  for (uint32_t at = 176; at < 224; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 224, from + 224);
  Matrix(base, to + 288, from + 288);
  Vec4(base, to + 352, from + 352);
  Vec4(base, to + 368, from + 368);
  FloatWord(base, to + 384, from + 384);
  Vec4(base, to + 400, from + 400);
  Matrix(base, to + 416, from + 416);
  Vec4(base, to + 480, from + 480);
  Vec4(base, to + 496, from + 496);
  FloatWord(base, to + 512, from + 512);
}

void Member1136Restore(uint8_t* base, uint32_t part, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82102C60 Member1136::Restore");
  const uint32_t to = vehicle, from = part;
  for (uint32_t at = 0; at < 48; at += 16) {
    Vec4(base, to + at, from + at);
  }
  FloatWord(base, to + 48, from + 48);
  FloatWord(base, to + 52, from + 52);
  Vec4(base, to + 64, from + 64);
  for (uint32_t at = 80; at < 96; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  for (uint32_t at = 96; at < 144; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 144, from + 144);
  Matrix(base, to + 208, from + 208);
  Byte(base, to + 276, from + 272);
  for (uint32_t i = 0; i < 14; i++) {
    FloatWord(base, to + 280 + 4 * i, from + 276 + 4 * i);
  }
  Word(base, to + 348, from + 332);
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 352 + 16 * i, from + 336 + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Wheel(base, to + 416 + 368 * i, from + 400 + 368 * i);
  }
  WheelRecord2(base, to + 1888, from + 1872);
  WheelRecord2(base, to + 2016, from + 2000);
  for (uint32_t i = 0; i < 4; i++) {
    Word(base, to + 2176 + 4 * i, from + 2128 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2192 + 4 * i, from + 2144 + 4 * i);
  }
  Memcpy(base, to + 2204, from + 2156, 144, 0x82102E4C);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 2348 + 4 * i, from + 2300 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Word(base, to + 2368 + 4 * i, from + 2320 + 4 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Byte(base, to + 2380 + 1 * i, from + 2332 + 1 * i);
  }
  FloatWord(base, to + 2388, from + 2340);
  Word(base, to + 2392, from + 2344);
  FloatWord(base, to + 2396, from + 2348);
  Byte(base, to + 2400, from + 2352);
  Byte(base, to + 2401, from + 2353);
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2404 + 4 * i, from + 2356 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2428 + 4 * i, from + 2368 + 4 * i);
  }
  FloatWord(base, to + 2416, from + 2380);
  FloatWord(base, to + 2420, from + 2384);
  Byte(base, to + 2424, from + 2388);
  FloatWord(base, to + 2440, from + 2392);
  Byte(base, to + 2444, from + 2396);
  Byte(base, to + 2445, from + 2397);
  FloatWord(base, to + 2448, from + 2400);
  FloatWord(base, to + 2452, from + 2404);
  Byte(base, to + 2456, from + 2408);
  Vec4(base, to + 2464, from + 2416);
  Vec4(base, to + 2480, from + 2432);
  Word(base, to + 2496, from + 2448);
  FloatWord(base, to + 2500, from + 2452);
  Byte(base, to + 2504, from + 2456);
  FloatWord(base, to + 2508, from + 2460);
  Byte(base, to + 2512, from + 2464);
  FloatWord(base, to + 2516, from + 2468);
  FloatWord(base, to + 2520, from + 2472);
  Word(base, to + 2524, from + 2476);
  for (uint32_t i = 0; i < 6; i++) {
    Vec4(base, to + 2528 + 16 * i, from + 2480 + 16 * i);
  }
  Byte(base, to + 2624, from + 2576);
  Byte(base, to + 2625, from + 2577);
  for (uint32_t i = 0; i < 9; i++) {
    FloatWord(base, to + 2628 + 4 * i, from + 2580 + 4 * i);
  }
  Byte(base, to + 2664, from + 2616);
  FloatWord(base, to + 2924, from + 2620);
  Byte(base, to + 2928, from + 2624);
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(base, to + 2932 + 4 * i, from + 2628 + 4 * i);
  }
  Byte(base, to + 2948, from + 2644);
  FloatWord(base, to + 2952, from + 2648);
  FloatWord(base, to + 2956, from + 2652);
  Byte(base, to + 2960, from + 2656);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 2964 + 4 * i, from + 2660 + 4 * i);
  }
  Word(base, to + 2984, from + 2680);
  Memcpy(base, to + 2988, from + 2684, 60, 0x821030D8);
  Memcpy(base, to + 3048, from + 2744, 60, 0x821030D8);
  for (uint32_t i = 0; i < 6; i++) {
    FloatWord(base, to + 3108 + 4 * i, from + 2804 + 4 * i);
  }
  Byte(base, to + 3132, from + 2828);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 3136 + 4 * i, from + 2832 + 4 * i);
  }
  Word(base, to + 3156, from + 2852);
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 3168 + 16 * i, from + 2864 + 16 * i);
  }
  FloatWord(base, to + 3232, from + 2928);
  FloatWord(base, to + 3236, from + 2932);
  Byte(base, to + 3240, from + 2936);
  Byte(base, to + 3241, from + 2937);
}

void Part464Restore(uint8_t* base, uint32_t part, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82102AA8 Part464::Restore");
  const uint32_t to = shape, from = part;
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 16 + 4 * i, from + 352 + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 48 + 16 * i, from + 384 + 16 * i);
  }
  WordToHalf(base, to + 112, from + 448);
  Vec4(base, to + 128, from + 0);
  Vec4(base, to + 144, from + 16);
  for (uint32_t i = 0; i < 8; i++) {
    Vec4(base, to + 176 + 16 * i, from + 32 + 16 * i);
  }
  FloatWord(base, to + 304, from + 160);
  Vec4(base, to + 320, from + 176);
  FloatWord(base, to + 336, from + 192);
  Vec4(base, to + 352, from + 208);
  FloatWord(base, to + 368, from + 224);
  Vec4(base, to + 384, from + 240);
  FloatWord(base, to + 400, from + 256);
  Vec4(base, to + 416, from + 272);
  FloatWord(base, to + 432, from + 288);
  Vec4(base, to + 448, from + 304);
  FloatWord(base, to + 464, from + 320);
  Vec4(base, to + 480, from + 336);
}

uint32_t Member4080Restore(uint8_t* base, uint32_t part, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82102B98 Member4080::Restore");
  if (guest::LoadU32(base, part) == 0 || object == 0) {
    return part;
  }
  const uint32_t to = object, from = part;
  Vec4(base, to + 48, from + 944);
  Vec4(base, to + 64, from + 960);
  Matrix(base, to + 80, from + 976);
  // The shape (+20) and the second placed copy (+40) are read before the
  // stores, the first copy (+36) after them, as the original.
  const uint32_t shape = guest::LoadU32(base, to + 20);
  const uint32_t second = guest::LoadU32(base, to + 40);
  FloatWord(base, to + 144, from + 1040);
  const uint32_t first = guest::LoadU32(base, to + 36);
  guest::StoreU32(base, to + 32, shape);
  Part464Restore(base, from + 16, first);
  Part464Restore(base, from + 480, second);
  Vec4(base, to + 160, from + 1056);
  Vec4(base, to + 176, from + 1072);
  Word(base, to + 192, from + 1088);
  Vec4(base, to + 208, from + 1104);
  FloatWord(base, to + 224, from + 1120);
  Word(base, to + 288, from + 1124);
  Vec4(base, to + 304, from + 1136);
  return to + 304;
}

uint32_t Record5232Restore(uint8_t* base, uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_821044D8 Record5232::Restore");
  Record5232RestoreTrack(base, record, vehicle);
  BodyStateRestore(base, record + 608, guest::LoadU32(base, vehicle + 336));
  Member1136Restore(base, record + 1136, vehicle);
  return Member4080Restore(base, record + 4080, guest::LoadU32(base, vehicle + 340));
}

// --- The saves ---

void Record5232SaveTrack(uint8_t* base, uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104380 Record5232::SaveTrack");
  const uint32_t to = record, from = vehicle;
  Word(base, to + 0, from + 6148);
  Word(base, to + 4, from + 6152);
  Memcpy(base, to + 16, from + 6160, 112, 0x821043B4);
  for (uint32_t i = 0; i < 5; i++) {
    TrackPosition(base, to + 128 + 96 * i, from + 5232 + 96 * i);
  }
}

void BodyStateSave(uint8_t* base, uint32_t state, uint32_t body) {
  ITC_NATIVE_RUNNING("sub_821039F0 BodyState::Save");
  const uint32_t to = state, from = body;
  Byte(base, to + 0, from + 0);
  FloatWord(base, to + 4, from + 4);
  FloatWord(base, to + 8, from + 8);
  Vec4(base, to + 16, from + 16);
  Vec4(base, to + 32, from + 32);
  FloatWord(base, to + 48, from + 48);
  Vec4(base, to + 64, from + 64);
  Dword(base, to + 80, from + 80);
  Dword(base, to + 88, from + 88);
  Vec4(base, to + 96, from + 96);
  FloatWord(base, to + 112, from + 112);
  Vec4(base, to + 128, from + 128);
  Vec4(base, to + 144, from + 144);
  FloatWord(base, to + 160, from + 160);
  FloatWord(base, to + 164, from + 164);
  for (uint32_t at = 176; at < 224; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 224, from + 224);
  Matrix(base, to + 288, from + 288);
  Vec4(base, to + 352, from + 352);
  Vec4(base, to + 368, from + 368);
  FloatWord(base, to + 384, from + 384);
  Vec4(base, to + 400, from + 400);
  Matrix(base, to + 416, from + 416);
  Vec4(base, to + 480, from + 480);
  Vec4(base, to + 496, from + 496);
  FloatWord(base, to + 512, from + 512);
}

void Member1136Save(uint8_t* base, uint32_t part, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_821034B8 Member1136::Save");
  const uint32_t to = part, from = vehicle;
  for (uint32_t at = 0; at < 48; at += 16) {
    Vec4(base, to + at, from + at);
  }
  FloatWord(base, to + 48, from + 48);
  FloatWord(base, to + 52, from + 52);
  Vec4(base, to + 64, from + 64);
  for (uint32_t at = 80; at < 96; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  for (uint32_t at = 96; at < 144; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 144, from + 144);
  Matrix(base, to + 208, from + 208);
  Byte(base, to + 272, from + 276);
  for (uint32_t i = 0; i < 14; i++) {
    FloatWord(base, to + 276 + 4 * i, from + 280 + 4 * i);
  }
  Word(base, to + 332, from + 348);
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 336 + 16 * i, from + 352 + 16 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Wheel(base, to + 400 + 368 * i, from + 416 + 368 * i);
  }
  WheelRecord2(base, to + 1872, from + 1888);
  WheelRecord2(base, to + 2000, from + 2016);
  for (uint32_t i = 0; i < 4; i++) {
    Word(base, to + 2128 + 4 * i, from + 2176 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2144 + 4 * i, from + 2192 + 4 * i);
  }
  Memcpy(base, to + 2156, from + 2204, 144, 0x8210369C);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 2300 + 4 * i, from + 2348 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Word(base, to + 2320 + 4 * i, from + 2368 + 4 * i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    Byte(base, to + 2332 + 1 * i, from + 2380 + 1 * i);
  }
  FloatWord(base, to + 2340, from + 2388);
  Word(base, to + 2344, from + 2392);
  FloatWord(base, to + 2348, from + 2396);
  Byte(base, to + 2352, from + 2400);
  Byte(base, to + 2353, from + 2401);
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2356 + 4 * i, from + 2404 + 4 * i);
  }
  for (uint32_t i = 0; i < 3; i++) {
    FloatWord(base, to + 2368 + 4 * i, from + 2428 + 4 * i);
  }
  FloatWord(base, to + 2380, from + 2416);
  FloatWord(base, to + 2384, from + 2420);
  Byte(base, to + 2388, from + 2424);
  FloatWord(base, to + 2392, from + 2440);
  Byte(base, to + 2396, from + 2444);
  Byte(base, to + 2397, from + 2445);
  FloatWord(base, to + 2400, from + 2448);
  FloatWord(base, to + 2404, from + 2452);
  Byte(base, to + 2408, from + 2456);
  Vec4(base, to + 2416, from + 2464);
  Vec4(base, to + 2432, from + 2480);
  Word(base, to + 2448, from + 2496);
  FloatWord(base, to + 2452, from + 2500);
  Byte(base, to + 2456, from + 2504);
  FloatWord(base, to + 2460, from + 2508);
  Byte(base, to + 2464, from + 2512);
  FloatWord(base, to + 2468, from + 2516);
  FloatWord(base, to + 2472, from + 2520);
  Word(base, to + 2476, from + 2524);
  for (uint32_t i = 0; i < 6; i++) {
    Vec4(base, to + 2480 + 16 * i, from + 2528 + 16 * i);
  }
  Byte(base, to + 2576, from + 2624);
  Byte(base, to + 2577, from + 2625);
  for (uint32_t i = 0; i < 9; i++) {
    FloatWord(base, to + 2580 + 4 * i, from + 2628 + 4 * i);
  }
  Byte(base, to + 2616, from + 2664);
  FloatWord(base, to + 2620, from + 2924);
  Byte(base, to + 2624, from + 2928);
  for (uint32_t i = 0; i < 4; i++) {
    FloatWord(base, to + 2628 + 4 * i, from + 2932 + 4 * i);
  }
  Byte(base, to + 2644, from + 2948);
  FloatWord(base, to + 2648, from + 2952);
  FloatWord(base, to + 2652, from + 2956);
  Byte(base, to + 2656, from + 2960);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 2660 + 4 * i, from + 2964 + 4 * i);
  }
  Word(base, to + 2680, from + 2984);
  Memcpy(base, to + 2684, from + 2988, 60, 0x82103928);
  Memcpy(base, to + 2744, from + 3048, 60, 0x82103928);
  for (uint32_t i = 0; i < 6; i++) {
    FloatWord(base, to + 2804 + 4 * i, from + 3108 + 4 * i);
  }
  Byte(base, to + 2828, from + 3132);
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 2832 + 4 * i, from + 3136 + 4 * i);
  }
  Word(base, to + 2852, from + 3156);
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 2864 + 16 * i, from + 3168 + 16 * i);
  }
  FloatWord(base, to + 2928, from + 3232);
  FloatWord(base, to + 2932, from + 3236);
  Byte(base, to + 2936, from + 3240);
  Byte(base, to + 2937, from + 3241);
}

void Part464Save(uint8_t* base, uint32_t part, uint32_t shape) {
  ITC_NATIVE_RUNNING("sub_82103300 Part464::Save");
  const uint32_t to = part, from = shape;
  for (uint32_t i = 0; i < 5; i++) {
    FloatWord(base, to + 352 + 4 * i, from + 16 + 4 * i);
  }
  for (uint32_t i = 0; i < 4; i++) {
    Vec4(base, to + 384 + 16 * i, from + 48 + 16 * i);
  }
  HalfToWord(base, to + 448, from + 112);
  Vec4(base, to + 0, from + 128);
  Vec4(base, to + 16, from + 144);
  for (uint32_t i = 0; i < 8; i++) {
    Vec4(base, to + 32 + 16 * i, from + 176 + 16 * i);
  }
  FloatWord(base, to + 160, from + 304);
  Vec4(base, to + 176, from + 320);
  FloatWord(base, to + 192, from + 336);
  Vec4(base, to + 208, from + 352);
  FloatWord(base, to + 224, from + 368);
  Vec4(base, to + 240, from + 384);
  FloatWord(base, to + 256, from + 400);
  Vec4(base, to + 272, from + 416);
  FloatWord(base, to + 288, from + 432);
  Vec4(base, to + 304, from + 448);
  FloatWord(base, to + 320, from + 464);
  Vec4(base, to + 336, from + 480);
}

uint32_t Member4080Save(uint8_t* base, uint32_t part, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_821033F0 Member4080::Save");
  if (object == 0) {
    guest::StoreU32(base, part, 0);
    return part;
  }
  const uint32_t to = part, from = object;
  guest::StoreU32(base, to, 1);
  Vec4(base, to + 944, from + 48);
  Vec4(base, to + 960, from + 64);
  Matrix(base, to + 976, from + 80);
  FloatWord(base, to + 1040, from + 144);
  const uint32_t second = guest::LoadU32(base, from + 40);
  const uint32_t first = guest::LoadU32(base, from + 36);
  Part464Save(base, to + 16, first);
  Part464Save(base, to + 480, second);
  Vec4(base, to + 1056, from + 160);
  Vec4(base, to + 1072, from + 176);
  Word(base, to + 1088, from + 192);
  Vec4(base, to + 1104, from + 208);
  FloatWord(base, to + 1120, from + 224);
  Word(base, to + 1124, from + 288);
  Vec4(base, to + 1136, from + 304);
  return to + 1136;
}

uint32_t Record5232Save(uint8_t* base, uint32_t record, uint32_t vehicle) {
  ITC_NATIVE_RUNNING("sub_82104538 Record5232::Save");
  Record5232SaveTrack(base, record, vehicle);
  BodyStateSave(base, record + 608, guest::LoadU32(base, vehicle + 336));
  Member1136Save(base, record + 1136, vehicle);
  return Member4080Save(base, record + 4080, guest::LoadU32(base, vehicle + 340));
}

uint32_t RaceSnapshotSave(uint8_t* base, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82104AB8 RaceSnapshot::Save");
  guest::StoreU32(base, block + 2292, 0);
  for (uint32_t i = 0; i < 7; i++) {
    const uint32_t vehicle = vehicle_accessors::VehicleGet(base, i);
    if (guest::LoadU32(base, vehicle + 6148) != 0) {
      Record5232Save(base, block + 2304 + kRecordBytes * i, vehicle);
    }
  }
  // The manager's entries and counts into the block's head.
  const uint32_t manager = guest::LoadU32(base, kManager);
  for (uint32_t i = 0; i < 77; i++) {
    Memcpy(base, block + 28 * i, manager + 4 + 28 * i, 28, 0x82104B28);
  }
  for (uint32_t i = 0; i < 3; i++) {
    Memcpy(base, block + 2156 + 28 * i, manager + 2160 + 28 * i, 28, 0x82104B54);
  }
  for (uint32_t i = 0; i < 7; i++) {
    Word(base, block + 2240 + 4 * i, manager + 2336 + 4 * i);
  }
  Word(base, block + 2268, manager + 2364);
  Word(base, block + 2272, manager + 2368);
  Word(base, block + 2276, manager + 2372);
  Word(base, block + 2280, manager + 2376);
  Word(base, block + 2284, manager + 2380);
  Word(base, block + 2288, manager + 2388);
  return block + 2156 + 28 * 2;  // the last memcpy's r3
}

// --- The copies (record to record) ---

uint32_t Record5232CopyTrack(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821043F0 Record5232::CopyTrack");
  const uint32_t to = out, from = record;
  Word(base, to + 0, from + 0);
  Word(base, to + 4, from + 4);
  Memcpy(base, to + 16, from + 16, 112, 0x82104424);
  for (uint32_t at = 128; at < 608; at += 96) {
    TrackPosition(base, to + at, from + at);
  }
  return out;
}

uint32_t BodyStateCopy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82103B50 BodyState::Copy");
  const uint32_t to = out, from = record;
  Byte(base, to + 0, from + 0);
  FloatWord(base, to + 4, from + 4);
  FloatWord(base, to + 8, from + 8);
  Vec4(base, to + 16, from + 16);
  Vec4(base, to + 32, from + 32);
  FloatWord(base, to + 48, from + 48);
  Vec4(base, to + 64, from + 64);
  Dword(base, to + 80, from + 80);
  Dword(base, to + 88, from + 88);
  Vec4(base, to + 96, from + 96);
  FloatWord(base, to + 112, from + 112);
  Vec4(base, to + 128, from + 128);
  Vec4(base, to + 144, from + 144);
  FloatWord(base, to + 160, from + 160);
  FloatWord(base, to + 164, from + 164);
  for (uint32_t at = 176; at < 224; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 224, from + 224);
  Matrix(base, to + 288, from + 288);
  Vec4(base, to + 352, from + 352);
  Vec4(base, to + 368, from + 368);
  FloatWord(base, to + 384, from + 384);
  Vec4(base, to + 400, from + 400);
  Matrix(base, to + 416, from + 416);
  Vec4(base, to + 480, from + 480);
  Vec4(base, to + 496, from + 496);
  FloatWord(base, to + 512, from + 512);
  return out;
}

uint32_t Member1136Copy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82103CB8 Member1136::Copy");
  const uint32_t to = out, from = record;
  for (uint32_t at = 0; at < 48; at += 16) {
    Vec4(base, to + at, from + at);
  }
  FloatWord(base, to + 48, from + 48);
  FloatWord(base, to + 52, from + 52);
  Vec4(base, to + 64, from + 64);
  for (uint32_t at = 80; at < 96; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  for (uint32_t at = 96; at < 144; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Matrix(base, to + 144, from + 144);
  Matrix(base, to + 208, from + 208);
  Byte(base, to + 272, from + 272);
  for (uint32_t at = 276; at < 332; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Word(base, to + 332, from + 332);
  for (uint32_t at = 336; at < 400; at += 16) {
    Vec4(base, to + at, from + at);
  }
  for (uint32_t at = 400; at < 1872; at += 368) {
    Wheel(base, to + at, from + at);
  }
  WheelRecord2(base, to + 1872, from + 1872);
  WheelRecord2(base, to + 2000, from + 2000);
  for (uint32_t at = 2128; at < 2144; at += 4) {
    Word(base, to + at, from + at);
  }
  for (uint32_t at = 2144; at < 2156; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Memcpy(base, to + 2156, from + 2156, 144, 0x82103E90);
  for (uint32_t at = 2300; at < 2320; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  for (uint32_t at = 2320; at < 2332; at += 4) {
    Word(base, to + at, from + at);
  }
  for (uint32_t at = 2332; at < 2340; at += 1) {
    Byte(base, to + at, from + at);
  }
  FloatWord(base, to + 2340, from + 2340);
  Word(base, to + 2344, from + 2344);
  FloatWord(base, to + 2348, from + 2348);
  Byte(base, to + 2352, from + 2352);
  Byte(base, to + 2353, from + 2353);
  for (uint32_t at = 2356; at < 2388; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Byte(base, to + 2388, from + 2388);
  FloatWord(base, to + 2392, from + 2392);
  Byte(base, to + 2396, from + 2396);
  Byte(base, to + 2397, from + 2397);
  FloatWord(base, to + 2400, from + 2400);
  FloatWord(base, to + 2404, from + 2404);
  Byte(base, to + 2408, from + 2408);
  Vec4(base, to + 2416, from + 2416);
  Vec4(base, to + 2432, from + 2432);
  Word(base, to + 2448, from + 2448);
  FloatWord(base, to + 2452, from + 2452);
  Byte(base, to + 2456, from + 2456);
  FloatWord(base, to + 2460, from + 2460);
  Byte(base, to + 2464, from + 2464);
  FloatWord(base, to + 2468, from + 2468);
  FloatWord(base, to + 2472, from + 2472);
  Word(base, to + 2476, from + 2476);
  for (uint32_t at = 2480; at < 2576; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Byte(base, to + 2576, from + 2576);
  Byte(base, to + 2577, from + 2577);
  for (uint32_t at = 2580; at < 2616; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Byte(base, to + 2616, from + 2616);
  FloatWord(base, to + 2620, from + 2620);
  Byte(base, to + 2624, from + 2624);
  for (uint32_t at = 2628; at < 2644; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Byte(base, to + 2644, from + 2644);
  FloatWord(base, to + 2648, from + 2648);
  FloatWord(base, to + 2652, from + 2652);
  Byte(base, to + 2656, from + 2656);
  for (uint32_t at = 2660; at < 2680; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Word(base, to + 2680, from + 2680);
  Memcpy(base, to + 2684, from + 2684, 60, 0x82104108);
  Memcpy(base, to + 2744, from + 2744, 60, 0x82104108);
  for (uint32_t at = 2804; at < 2828; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Byte(base, to + 2828, from + 2828);
  for (uint32_t at = 2832; at < 2852; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  Word(base, to + 2852, from + 2852);
  for (uint32_t at = 2864; at < 2928; at += 16) {
    Vec4(base, to + at, from + at);
  }
  FloatWord(base, to + 2928, from + 2928);
  FloatWord(base, to + 2932, from + 2932);
  Byte(base, to + 2936, from + 2936);
  Byte(base, to + 2937, from + 2937);
  return out;
}

uint32_t Part464Copy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821041C0 Part464::Copy");
  const uint32_t to = out, from = record;
  for (uint32_t at = 0; at < 160; at += 16) {
    Vec4(base, to + at, from + at);
  }
  FloatWord(base, to + 160, from + 160);
  Vec4(base, to + 176, from + 176);
  FloatWord(base, to + 192, from + 192);
  Vec4(base, to + 208, from + 208);
  FloatWord(base, to + 224, from + 224);
  Vec4(base, to + 240, from + 240);
  FloatWord(base, to + 256, from + 256);
  Vec4(base, to + 272, from + 272);
  FloatWord(base, to + 288, from + 288);
  Vec4(base, to + 304, from + 304);
  FloatWord(base, to + 320, from + 320);
  Vec4(base, to + 336, from + 336);
  for (uint32_t at = 352; at < 372; at += 4) {
    FloatWord(base, to + at, from + at);
  }
  for (uint32_t at = 384; at < 448; at += 16) {
    Vec4(base, to + at, from + at);
  }
  Word(base, to + 448, from + 448);
  return out;
}

uint32_t Member4080CopyShapes(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104460 Member4080::CopyShapes");
  const uint32_t to = out, from = record;
  Part464Copy(base, to + 0, from + 0);
  Part464Copy(base, to + 464, from + 464);
  Vec4(base, to + 928, from + 928);
  Vec4(base, to + 944, from + 944);
  Matrix(base, to + 960, from + 960);
  FloatWord(base, to + 1024, from + 1024);
  return out;
}

uint32_t Member4080Copy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104598 Member4080::Copy");
  const uint32_t to = out, from = record;
  Word(base, to + 0, from + 0);
  Member4080CopyShapes(base, to + 16, from + 16);
  Vec4(base, to + 1056, from + 1056);
  Vec4(base, to + 1072, from + 1072);
  Word(base, to + 1088, from + 1088);
  Vec4(base, to + 1104, from + 1104);
  FloatWord(base, to + 1120, from + 1120);
  Word(base, to + 1124, from + 1124);
  Vec4(base, to + 1136, from + 1136);
  return out;
}

uint32_t Record5232Copy(uint8_t* base, uint32_t out, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_82104BC0 Record5232::Copy");
  Record5232CopyTrack(base, out, record);
  BodyStateCopy(base, out + 608, record + 608);
  Member1136Copy(base, out + 1136, record + 1136);
  Member4080Copy(base, out + 4080, record + 4080);
  return out;
}

uint32_t RaceSnapshotCopy(uint8_t* base, uint32_t out, uint32_t block) {
  ITC_NATIVE_RUNNING("sub_82104EA8 RaceSnapshot::Copy");
  Memcpy(base, out, block, 2292, 0x82104EC4);
  Word(base, out + 2292, block + 2292);
  for (uint32_t i = 0; i < 7; i++) {
    Record5232Copy(base, out + 2304 + kRecordBytes * i, block + 2304 + kRecordBytes * i);
  }
  return out;
}

// --- The kept replay ---

uint32_t ReplayKeep(uint8_t* base, uint32_t replay, uint32_t race, uint32_t recordings,
                    uint32_t save, uint32_t from_stage, uint32_t frame,
                    const StagesCopyOriginal& stages_copy) {
  ITC_NATIVE_RUNNING("sub_82104F68 Replay::Keep");
  const uint32_t r = replay, d = race;
  Memcpy(base, r + 52, d + 4, 32, 0x82104FB0);
  // The car count and the three words after it (stored 84, 92, 96, 88).
  Word(base, r + 84, d + 0x59610);
  Word(base, r + 92, d + 0x59618);
  Word(base, r + 96, d + 0x5961C);
  Word(base, r + 88, d + 0x59614);
  Word(base, r + 0x2B5E8, d + 0xA3AE0);
  // The 93 racer records' entries (their source through the empty
  // sub_82106540: nothing done).
  for (uint32_t k = 0; k < 93; k++) {
    const uint32_t record = d + 48 + 3936 * k;
    const uint32_t entry = r + 100 + 1908 * k;
    Memcpy(base, entry + 8, record, 1884, 0x8210502C);
    guest::StoreU16(base, entry + 1892, guest::LoadU16(base, record + 3904));
    guest::StoreU16(base, entry + 1894, guest::LoadU16(base, record + 3906));
    FloatWord(base, entry + 1896, record + 3908);
    Word(base, entry + 1900, record + 3912);
    FloatWord(base, entry + 1904, record + 3920);
    Word(base, entry + 0, record + 3888);
    Word(base, entry + 4, record + 3892);
  }
  // The stage: the last (-1), or the list's current - 1 with `from_stage`,
  // into the frame's +80, then to +0x2B588.
  const uint32_t stages = d + 0x59620;
  const uint32_t stage =
      from_stage != 0 ? guest::LoadU32(base, stages + 804) - 1 : 0xFFFFFFFFu;
  if (!vehicle_accessors::StagesCopy(base, frame + 80, stages, stage)) {
    stages_copy(frame + 80, stages, stage, frame);
  }
  Memcpy(base, r + 0x2B588, frame + 80, 80, 0x821050B4);
  // The last kept result's +3916 to +3928.
  const uint32_t last = vehicle_accessors::RacerRecordsAt(
      base, guest::LoadU32(base, kRaceData) + 0x59AE8, 0xFFFFFFFFu);
  for (uint32_t i = 0; i < 4; i++) {
    Word(base, r + 0x2B5D8 + 4 * i, last + 3916 + 4 * i);
  }
  for (uint32_t i = 0; i < 5; i++) {
    Word(base, r + 0x2B5EC + 4 * i, d + 0xA3AE4 + 4 * i);
  }
  RaceSnapshotCopy(base, r + 0x2B610, recordings + 0x20F590);
  Memcpy(base, r + 0x34E20, recordings, 0x107AC4, 0x82105160);
  {
    const uint32_t w0 = guest::LoadU32(base, r + 52);
    const uint32_t w1 = guest::LoadU32(base, r + 76);
    const uint32_t w2 = guest::LoadU32(base, r + 80);
    guest::StoreU32(base, r + 0, w0);
    guest::StoreU32(base, r + 4, w1);
    guest::StoreU32(base, r + 8, w2);
  }
  // The four racers the stage lists: their entries' +20.
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t racer = guest::LoadU32(base, r + 0x2B5B4 + 8 * i);
    guest::StoreU32(base, r + 12 + 4 * i, guest::LoadU32(base, r + 1908 * racer + 120));
  }
  Word(base, r + 28, r + 0x2B588);
  if (save != 0) {
    for (uint32_t i = 0; i < 5; i++) {
      Word(base, r + 32 + 4 * i, save + 8 + 4 * i);
    }
  } else {
    guest::StoreU8(base, r + 32, 0);
    guest::StoreU32(base, r + 36, 6);
    guest::StoreU16(base, r + 40, 0);
    guest::StoreU8(base, r + 42, 1);
    guest::StoreU32(base, r + 44, 6);
    guest::StoreU16(base, r + 48, 0);
    guest::StoreU8(base, r + 50, 1);
  }
  guest::StoreU32(base, r + 0x2B600, 0);
  return r + 0x34E20;  // the last memcpy's r3
}

}  // namespace replay_snapshots
