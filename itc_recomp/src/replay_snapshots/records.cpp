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

}  // namespace replay_snapshots
