// The car parameter tables as plain C++: see car_specs.h.

#include "car_specs/car_specs.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <bit>
#include <climits>
#include <cmath>

namespace car_specs {

namespace {

// `count` words from `address` byte-reversed in place (the originals' lfs,
// stfs and four byte stores; see car_specs.h).
void SwapWords(uint8_t* base, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t a = address + 4 * i;
    guest::StoreU32(base, a, std::byteswap(guest::LoadU32(base, a)));
  }
}

void SwapHalf(uint8_t* base, uint32_t address) {
  guest::StoreU16(base, address, std::byteswap(guest::LoadU16(base, address)));
}

// The parameter object.
uint32_t Params(uint8_t* base) { return guest::LoadU32(base, kParams); }

// An asset's size from its TOC entry: the unpacked size, or the stored size
// when that's 0.
uint32_t AssetSize(uint8_t* base, uint32_t toc, uint32_t id) {
  const uint32_t unpacked = guest::LoadU32(base, toc + 16 * id + 24);
  return unpacked != 0 ? unpacked : guest::LoadU32(base, toc + 16 * id + 20);
}

// Whether memcpy (sub_825A7410) copies `size` bytes as the host's memcpy
// does with nothing for its override to record: a destination outside the
// physical views (the copies it records for the renderer), each range in
// one view, the two not sharing memory.
bool CopyFits(uint32_t destination, uint32_t source, uint32_t size) {
  return destination < 0xA0000000u && c_runtime::InOneView(destination, size) &&
         c_runtime::InOneView(source, size) &&
         !c_runtime::MayShare(destination, size, source, size);
}

void CountDown(uint8_t* base, uint32_t object) {
  guest::StoreU32(base, object, guest::LoadU32(base, object) - 1);
}

}  // namespace

uint32_t SwapVector(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820C9750 CarSpec::SwapVector");
  // +12 = the float at 0x82000A30 (1.0), stored as loaded.
  const uint32_t w = guest::LoadU32(base, 0x82000A30);
  SwapWords(base, p, 3);
  guest::StoreU32(base, p + 12, w);
  return p;
}

uint32_t SwapMatrix(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820C9F10 CarSpec::SwapMatrix");
  SwapWords(base, p, 16);
  return p;
}

uint32_t Swap5(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA480 CarSpec::Swap5");
  SwapWords(base, p, 5);
  return p;
}

uint32_t Swap3(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA550 CarSpec::Swap3");
  SwapWords(base, p, 3);
  return p;
}

uint32_t Swap4(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB3B0 CarSpec::Swap4");
  SwapWords(base, p, 4);
  return p;
}

uint32_t SwapColor(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB530 Wheel::SwapColor");
  SwapHalf(base, p);
  SwapHalf(base, p + 4);
  SwapWords(base, p + 8, 1);
  return p;
}

uint32_t SwapRecord83(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB658 Params::SwapTable83Record");
  SwapWords(base, p, 10);
  return p;
}

uint32_t SwapEngineStage(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAAB0 EngineStage::Swap");
  // Words +8 to +196; +0 to +7 and +200 to +203 left.
  SwapWords(base, p + 8, 48);
  return p;
}

uint32_t SwapTire(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAD78 Tire::Swap");
  // Words +8 to +88; +0 to +7 left.
  SwapWords(base, p + 8, 21);
  return p;
}

uint32_t SwapBlock1At368(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA198 CarSpec::SwapBlock1At368");
  SwapWords(base, p, 40);
  return p;
}

uint32_t SwapBlock2At8(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAFF0 CarSpec::SwapBlock2At8");
  // +20 to +52 = the float at 0x82000A34 (0.0), stored as loaded.
  const uint32_t w = guest::LoadU32(base, 0x82000A34);
  SwapWords(base, p, 5);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, p + 20 + 4 * i, w);
  }
  SwapWords(base, p + 56, 1);
  return p;
}

uint32_t SwapPaints(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB458 Paint::Swap");
  for (uint32_t slot = p + 8, n = 0; n < 13; n++, slot += 20) {
    for (uint32_t offset = 0; offset <= 16; offset += 4) {
      SwapHalf(base, slot + offset);
    }
    // The second shade at +12 by the type (+0).
    const uint16_t type = guest::LoadU16(base, slot);
    if (type == 0) {
      guest::StoreU16(base, slot + 12, 0);
      guest::StoreU8(base, slot + 14, 0);
      guest::StoreU8(base, slot + 15, 25);
    } else if (type == 1) {
      const int32_t saturation = guest::LoadU8(base, slot + 6);
      guest::StoreU16(base, slot + 12, guest::LoadU16(base, slot + 4));
      guest::StoreU8(base, slot + 14, uint8_t(saturation * 60 / 100));
      guest::StoreU8(base, slot + 15, 204);
    }
  }
  SwapWords(base, p + 268, 1);
  return p;
}

uint32_t SwapWheel(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB588 Wheel::Swap");
  SwapColor(base, p + 8);
  SwapColor(base, p + 20);
  SwapWords(base, p + 32, 4);
  return p + 20;
}

uint32_t SwapBlock1At264(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA2F8 CarSpec::SwapBlock1At264");
  Swap3(base, p);
  Swap3(base, p + 16);
  SwapWords(base, p + 32, 15);
  return p + 16;
}

uint32_t SwapBlock1(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA5D0 CarSpec::SwapBlock1");
  for (uint32_t offset = 16; offset <= 80; offset += 16) {
    SwapVector(base, p + offset);
  }
  SwapMatrix(base, p + 96);
  SwapMatrix(base, p + 160);
  Swap5(base, p + 224);
  Swap5(base, p + 244);
  SwapBlock1At264(base, p + 264);
  Swap3(base, p + 356);
  SwapBlock1At368(base, p + 368);
  SwapWords(base, p + 528, 28);
  return p + 368;
}

uint32_t SwapBlock2At132(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB0C0 CarSpec::SwapBlock2At132");
  SwapWords(base, p, 10);
  for (uint32_t offset = 40; offset < 100; offset += 20) {
    Swap5(base, p + offset);
  }
  SwapWords(base, p + 100, 17);
  // r3 ends as a byte register of the swaps after the Swap5s: the last
  // byte the +104 to +124 loop loaded, the word at +124's (now at +124).
  return guest::LoadU8(base, p + 124);
}

uint32_t SwapBlock2(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CBE70 CarSpec::SwapBlock2");
  SwapBlock2At8(base, p + 8);
  SwapWords(base, p + 72, 14);
  SwapBlock2At132(base, p + 132);
  Swap4(base, p + 300);
  return p + 300;
}

uint32_t SwapCarSpec(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CC168 CarSpec::Swap");
  SwapBlock1(base, p);
  SwapBlock2(base, p + 640);
  for (uint32_t i = 0; i < 16; i++) {
    SwapEngineStage(base, p + 980 + 204 * i);
  }
  for (uint32_t i = 0; i < 21; i++) {
    SwapTire(base, p + 4244 + 92 * i);
    SwapTire(base, p + 6176 + 92 * i);
  }
  SwapPaints(base, p + 8108);
  return SwapWheel(base, p + 8380);
}

uint32_t SwapCarSpecs(uint8_t* base, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820CC1F0 Params::SwapCarSpecs");
  // The count and the table read from the global each time, as the original.
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, Params(base) + 56)); i++) {
    r3 = SwapCarSpec(base, guest::LoadU32(base, Params(base) + 12) + kSpecBytes * uint32_t(i));
  }
  return r3;
}

uint32_t SwapWheels(uint8_t* base, uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820CBE10 Params::SwapWheels");
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, Params(base) + 60)); i++) {
    r3 = guest::LoadU32(base, Params(base) + 8) + kWheelBytes * uint32_t(i);
    if (r3 != 0) {
      r3 = SwapWheel(base, r3);
    }
  }
  return r3;
}

uint32_t OnCarSpecsLoaded(uint8_t* base, uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CC248 Params::OnCarSpecsLoaded");
  guest::StoreU32(base, object + 56, AssetSize(base, toc, 72) / kSpecBytes);
  const uint32_t r3 = SwapCarSpecs(base, toc);
  CountDown(base, object);
  return r3;
}

uint32_t OnWheelsLoaded(uint8_t* base, uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CC0A8 Params::OnWheelsLoaded");
  guest::StoreU32(base, object + 60, AssetSize(base, toc, 73) / kWheelBytes);
  const uint32_t r3 = SwapWheels(base, toc);
  // Each record's model (+40) up by one, and the first record of each
  // model at +64 + 4 × model (model 1's at +68 = 0 from the start).
  uint32_t model = 1;
  guest::StoreU32(base, object + 68, 0);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, object + 60)); i++) {
    const uint32_t record = guest::LoadU32(base, object + 8) + kWheelBytes * uint32_t(i);
    const uint32_t m = guest::LoadU32(base, record + 40) + 1;
    guest::StoreU32(base, record + 40, m);
    if (m != model) {
      model = m;
      guest::StoreU32(base, object + 4 * (m + 16), uint32_t(i));
    }
  }
  guest::StoreU32(base, object + 464, model + 1);
  CountDown(base, object);
  return r3;
}

void OnLoaded(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820C9DF0 Params::OnLoaded");
  CountDown(base, object);
}

uint32_t OnTable83Loaded(uint8_t* base, uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CBD90 Params::OnTable83Loaded");
  uint32_t r3 = toc;
  const uint32_t count = AssetSize(base, toc, 83) / 40;
  if (int32_t(count) > 0) {
    for (uint32_t i = 0; i < count; i++) {
      r3 = SwapRecord83(base, i * 40 + guest::LoadU32(base, object + 52));
    }
  }
  CountDown(base, object);
  return r3;
}

uint32_t GetCarSpec(uint8_t* base, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820C9E98 Params::GetCarSpec");
  return guest::LoadU32(base, Params(base) + 12) + kSpecBytes * id;
}

uint32_t GetWheel(uint8_t* base, int32_t n) {
  ITC_NATIVE_RUNNING("sub_820C9EB0 Params::GetWheel");
  if (n < 1) {
    return 0;
  }
  return guest::LoadU32(base, Params(base) + 8) + kWheelBytes * uint32_t(n) - kWheelBytes;
}

uint32_t FirstWheelOfModel(uint8_t* base, int32_t m) {
  ITC_NATIVE_RUNNING("sub_820C9EE0 Params::FirstWheelOfModel");
  if (m < 1) {
    return 0;
  }
  return guest::LoadU32(base, Params(base) + 4 * (uint32_t(m) + 16)) + 1;
}

void GetCarParts(uint8_t* base, uint32_t car, uint32_t parts[9]) {
  ITC_NATIVE_RUNNING("sub_820C9E00 Params::GetCarParts");
  static constexpr uint32_t kStrides[9] = {280, 280, 280, 280, 168, 168, 84, 108, 72};
  const uint32_t params = Params(base);
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = guest::LoadU32(base, params + 16 + 4 * i) + kStrides[i] * car;
  }
}

uint32_t GetTire(uint32_t spec, int32_t type, uint32_t grade, int32_t rear) {
  ITC_NATIVE_RUNNING("sub_820C9928 CarSpec::GetTire");
  const uint32_t index = type == 0 ? 0 : uint32_t(type) * 5 + grade - 5;
  return index * 92 + spec + (rear == 0 ? 4244 : 6176);
}

uint32_t Exists(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820C9DD8 Params::Exists");
  return Params(base) != 0 ? 1 : 0;
}

uint32_t SwapAeroPart(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB6C8 AeroPart::Swap");
  SwapWords(base, p, 7);
  return p;
}

uint32_t SwapAeroPart36(uint8_t* base, uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB7E8 AeroPart::Swap36");
  SwapAeroPart(base, p);
  SwapWords(base, p + 28, 2);
  return p;
}

namespace {

// An aero part getter: table `table` (its pointer at parts + 4 × table), its
// records `size` bytes; `use` whether the level picks a record.
bool AeroPart(uint8_t* base, uint32_t out, const uint32_t parts[9], int32_t level,
              uint32_t table, uint32_t size, bool use) {
  if (use) {
    const uint32_t from = parts[table] + size * uint32_t(level) - size;
    if (!CopyFits(out, from, size)) {
      return false;
    }
    c_runtime::Memcpy(base, out, from, size);
    if (size == 28) {
      SwapAeroPart(base, out);
    } else {
      SwapAeroPart36(base, out);
    }
    return true;
  }
  // The defaults: the floats at 0x82000A34 (0.0) and 0x82000A30 (1.0),
  // stored as loaded.
  const uint32_t zero = guest::LoadU32(base, 0x82000A34);
  const uint32_t one = guest::LoadU32(base, 0x82000A30);
  for (uint32_t offset = 0; offset < 16; offset += 4) {
    guest::StoreU32(base, out + offset, zero);
  }
  guest::StoreU32(base, out + 16, one);
  guest::StoreU32(base, out + 20, one);
  guest::StoreU32(base, out + 24, 0);
  if (size == 36) {
    guest::StoreU32(base, out + 28, 0);
    guest::StoreU32(base, out + 32, 0);
  }
  return true;
}

}  // namespace

#define ITC_AERO_PART(number, address, table, size, use)                       \
  bool AeroPart##number(uint8_t* base, uint32_t out, const uint32_t parts[9], int32_t level) { \
    ITC_NATIVE_RUNNING("sub_" #address " AeroParts::Get" #number);             \
    return AeroPart(base, out, parts, level, table, size, use);                \
  }

ITC_AERO_PART(74, 820CB848, 0, 28, level != 0)
ITC_AERO_PART(75, 820CB8D0, 1, 28, level != 0)
ITC_AERO_PART(76, 820CB958, 2, 28, level != 0)
ITC_AERO_PART(77, 820CB9E0, 3, 28, level > 0)
ITC_AERO_PART(78, 820CBA68, 4, 28, level != 0)
ITC_AERO_PART(79, 820CBAF0, 5, 28, level != 0)
ITC_AERO_PART(80, 820CBB78, 6, 28, level != 0)
ITC_AERO_PART(81, 820CBC00, 7, 36, level != 0)
ITC_AERO_PART(82, 820CBC90, 8, 36, level != 0)

#undef ITC_AERO_PART

uint32_t CountBodyKits(uint8_t* base, uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820CBD20 CountBodyKits");
  // The original copies each record to the stack and swaps it there; only
  // the first float is read.
  const float zero = std::bit_cast<float>(guest::LoadU32(base, 0x82000A34));
  uint32_t count = 0;
  for (uint32_t offset = 0; offset < 72; offset += 36) {
    const uint32_t word = guest::LoadU32(base, guest::LoadU32(base, parts + 32) + offset);
    if (double(std::bit_cast<float>(std::byteswap(word))) != double(zero)) {
      count++;
    }
  }
  return count;
}

namespace {

// The translated code's float steps: a single widened to double, a result
// rounded to single (frsp and the single-precision instructions).
double Single(double x) { return double(float(x)); }
double LoadSingle(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void StoreSingle(uint8_t* base, uint32_t address, double x) {
  guest::StoreFloat(base, address, float(x));
}
// A constant of the game's at 0x82000000 + offset.
double Constant(uint8_t* base, uint32_t offset) { return LoadSingle(base, 0x82000000u + offset); }
double ConstantDouble(uint8_t* base, uint32_t offset) {
  const uint64_t bits = uint64_t(guest::LoadU32(base, 0x82000000u + offset)) << 32 |
                        guest::LoadU32(base, 0x82000000u + offset + 4);
  return std::bit_cast<double>(bits);
}
// fctiwz as the translated code does it: a NaN gives 0x80000000, INT_MAX and
// up INT_MAX, else truncated (cvttsd2si: 0x80000000 below INT_MIN).
int32_t Fctiwz(double x) {
  if (std::isnan(x)) {
    return INT32_MIN;
  }
  if (x >= double(INT32_MAX)) {
    return INT32_MAX;
  }
  if (x < double(INT32_MIN)) {
    return INT32_MIN;
  }
  return int32_t(x);
}

}  // namespace

void InchUp(uint8_t* base, uint32_t tire, int32_t n) {
  ITC_NATIVE_RUNNING("sub_820C46B8 Tire::InchUp");
  if (n == 0) {
    return;
  }
  // The registers' steps, in the original's order (f0 to f13 as named there).
  double f0 = LoadSingle(base, tire + 48);   // the width
  double f13 = LoadSingle(base, tire + 80);  // the rim
  double f11 = LoadSingle(base, tire + 84);  // the aspect
  f11 = Single(f0 * f11);
  double f4 = LoadSingle(base, tire + 44);  // the radius
  double f3 = LoadSingle(base, tire + 64);  // the grip
  double f10 = Constant(base, 8708);
  const double f9 = Constant(base, 8148);
  const double f8 = Constant(base, 11536);
  double f7 = Constant(base, 8680);
  const double f6 = Constant(base, 11532);
  double f5 = Single(f13 * f6);
  f11 = Single(std::fma(f11, f9, f5));
  f5 = Constant(base, 11468);
  double f2 = Single(f11 * f5);
  f11 = Constant(base, 2608);
  double f12 = Single(double(int64_t(n)));
  f13 = Single(f12 + f13);
  StoreSingle(base, tire + 80, f13);
  f12 = Single(std::fma(f12, f10, f11));
  f10 = Single(f2 * f0);
  f12 = Single(f12 * f0);
  f0 = Single(std::sqrt(f10));
  f10 = Constant(base, 7132);
  f2 = Single(f12 * f10);
  f0 = Single(std::fma(f0, f8, f11));
  f12 = Constant(base, 11560);
  f12 = Single(f13 * f12);
  const int32_t width = Fctiwz(f2);
  f4 = Single(std::fma(f4, f7, -f12));
  f12 = Constant(base, 7140);
  f0 = Single(f0 * f12);
  double f1 = Single(f4 * f12);
  f4 = Constant(base, 10716);
  f0 = Single(f0 * f4);
  f2 = Single(f3 / f0);
  // The width in mm down to a multiple of 5 (divw, then mulli).
  const int32_t width5 = int32_t(uint32_t(width / 5) * 5u);
  f3 = Single(double(int64_t(width5)));
  f0 = Constant(base, 10712);
  f0 = Single(f3 * f0);
  StoreSingle(base, tire + 48, f0);
  f3 = Single(f1 / f0);
  StoreSingle(base, tire + 84, f3);
  const bool below = f0 < Constant(base, 11556);
  f3 = ConstantDouble(base, 8888);
  f1 = Constant(base, 11552);
  f1 = Single(std::fma(f0, f10, -f1));
  f10 = Constant(base, 8124);
  f1 = Single(f1 * f10);
  f10 = std::fma(f1, f3, ConstantDouble(base, 11544));
  if (!below) {
    f10 = f10 - f3;
  }
  f10 = Single(f10);
  StoreSingle(base, tire + 88, f10);
  f13 = Single(f13 * f6);
  f10 = Single(f10 * f7);
  f7 = LoadSingle(base, tire + 84);
  f7 = Single(f0 * f7);
  const int32_t rim = Fctiwz(f10);
  f13 = Single(std::fma(f7, f9, f13));
  f13 = Single(f13 * f5);
  f0 = Single(f13 * f0);
  f0 = Single(std::sqrt(f0));
  f0 = Single(std::fma(f0, f8, f11));
  // The rim width rounded down to a half (× 5 / 10, mulli on the zero-
  // extended word, then divw).
  const int32_t rim5 = int32_t(uint32_t(uint64_t(uint32_t(rim)) * 5u)) / 10;
  f0 = Single(f0 * f12);
  StoreSingle(base, tire + 88, Single(double(int64_t(rim5))));
  f0 = Single(f0 * f2);
  f0 = Single(f0 * f4);
  StoreSingle(base, tire + 64, f0);
  StoreSingle(base, tire + 68, f0);
}

uint32_t CopySpec(uint8_t* base, uint32_t car, uint32_t spec) {
  ITC_NATIVE_RUNNING("sub_820C4888 Car::CopySpec");
  const uint32_t zero = guest::LoadU32(base, 0x82000A34);
  guest::StoreU32(base, car + 3876, zero);
  guest::StoreU32(base, car + 3880, zero);
  c_runtime::Memcpy(base, car + 2240, spec, 640);
  c_runtime::Memcpy(base, car + 1888, spec + 640, 340);
  c_runtime::Memcpy(base, car + 2880,
                    guest::LoadU32(base, car + 1656) * 204 + spec + 980, 204);
  c_runtime::Memcpy(base, car + 3364, spec + 8108, 272);
  c_runtime::Memcpy(base, car + 3084,
                    GetTire(spec, int32_t(guest::LoadU32(base, car + 1712)),
                            guest::LoadU32(base, car + 1716), 0),
                    92);
  c_runtime::Memcpy(base, car + 3176,
                    GetTire(spec, int32_t(guest::LoadU32(base, car + 1720)),
                            guest::LoadU32(base, car + 1724), 1),
                    92);
  const double front = LoadSingle(base, car + 3132);
  const double rear = LoadSingle(base, car + 3224);
  InchUp(base, car + 3084, int32_t(guest::LoadU32(base, car + 1732)));
  InchUp(base, car + 3176, int32_t(guest::LoadU32(base, car + 1740)));
  StoreSingle(base, car + 3876, Single(LoadSingle(base, car + 3132) - front));
  StoreSingle(base, car + 3880, Single(LoadSingle(base, car + 3224) - rear));
  return car + 3176;
}

uint32_t CopyAeroParts(uint8_t* base, uint32_t car, const uint32_t parts[9]) {
  ITC_NATIVE_RUNNING("sub_820C4568 Car::CopyAeroParts");
  auto level = [&](uint32_t offset) { return int32_t(guest::LoadU32(base, car + offset)); };
  AeroPart74(base, car + 3636, parts, level(1744));
  AeroPart75(base, car + 3664, parts, level(1748));
  AeroPart76(base, car + 3692, parts, level(1752));
  AeroPart77(base, car + 3720, parts, level(1756) == -1 ? 0 : level(1756));
  AeroPart80(base, car + 3776, parts, level(1768));
  AeroPart81(base, car + 3804, parts, level(1780));
  AeroPart82(base, car + 3840, parts, level(1784));
  if (guest::LoadU32(base, car + 1764) == 0) {
    AeroPart78(base, car + 3748, parts, level(1760));
  } else {
    AeroPart79(base, car + 3748, parts, level(1760));
  }
  return car + 3748;
}

namespace {

// A wheel record for the car's wheel number at `offset`: the spec's stock
// wheel for 0, else Params::GetWheel's.
uint32_t WheelOf(uint8_t* base, uint32_t car, uint32_t offset, uint32_t spec) {
  const int32_t n = int32_t(guest::LoadU32(base, car + offset));
  return n == 0 ? spec + 8380 : GetWheel(base, n);
}

}  // namespace

bool CarRefresh(uint8_t* base, uint32_t car, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820C4998 Car::Refresh");
  const uint32_t spec = GetCarSpec(base, guest::LoadU32(base, car + 12));
  uint32_t parts[9];
  GetCarParts(base, guest::LoadU8(base, spec + 771) != 0 ? 0 : guest::LoadU32(base, car + 12),
              parts);
  // Every copy's ranges first (the original's order of reads only differs
  // from these where the car's written span, +1888 to +3884, shares memory
  // with what's read after it, which is checked too).
  struct Copy {
    uint32_t to, from, size;
  };
  auto aero = [&](uint32_t to, uint32_t table, uint32_t size, int32_t level, bool use) {
    return Copy{to, use ? parts[table] + size * uint32_t(level) - size : to, use ? size : 0};
  };
  auto level = [&](uint32_t offset) { return int32_t(guest::LoadU32(base, car + offset)); };
  const int32_t level77 = level(1756) == -1 ? 0 : level(1756);
  const bool use79 = guest::LoadU32(base, car + 1764) != 0;
  const Copy copies[] = {
      {car + 2240, spec, 640},
      {car + 1888, spec + 640, 340},
      {car + 2880, guest::LoadU32(base, car + 1656) * 204 + spec + 980, 204},
      {car + 3364, spec + 8108, 272},
      {car + 3084,
       GetTire(spec, level(1712), guest::LoadU32(base, car + 1716), 0), 92},
      {car + 3176,
       GetTire(spec, level(1720), guest::LoadU32(base, car + 1724), 1), 92},
      aero(car + 3636, 0, 28, level(1744), level(1744) != 0),
      aero(car + 3664, 1, 28, level(1748), level(1748) != 0),
      aero(car + 3692, 2, 28, level(1752), level(1752) != 0),
      aero(car + 3720, 3, 28, level77, level77 > 0),
      aero(car + 3776, 6, 28, level(1768), level(1768) != 0),
      aero(car + 3804, 7, 36, level(1780), level(1780) != 0),
      aero(car + 3840, 8, 36, level(1784), level(1784) != 0),
      aero(car + 3748, use79 ? 5 : 4, 28, level(1760), level(1760) != 0),
      {car + 3268, WheelOf(base, car, 1728, spec), 48},
      {car + 3316, WheelOf(base, car, 1736, spec), 48},
  };
  const uint32_t span = car + 1888, span_bytes = 3884 - 1888;
  if (span >= 0xA0000000u || !c_runtime::InOneView(span, span_bytes) ||
      c_runtime::MayShare(span, span_bytes, spec, kSpecBytes) ||
      c_runtime::MayShare(span, span_bytes, Params(base), 468)) {
    return false;
  }
  for (const Copy& copy : copies) {
    if (copy.size != 0 && (!CopyFits(copy.to, copy.from, copy.size) ||
                           c_runtime::MayShare(span, span_bytes, copy.from, copy.size))) {
      return false;
    }
  }
  CopySpec(base, car, spec);
  CopyAeroParts(base, car, parts);
  c_runtime::Memcpy(base, car + 3268, WheelOf(base, car, 1728, spec), 48);
  c_runtime::Memcpy(base, car + 3316, WheelOf(base, car, 1736, spec), 48);
  *r3 = car + 3316;
  return true;
}

}  // namespace car_specs
