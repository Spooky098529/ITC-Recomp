// The car parameter tables as plain C++: see car_specs.h.

#include "car_specs/car_specs.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "memory/c_runtime_heap.h"
#include "guest.h"
#include "schema/car_specs.h"
#include "schema/garage_screen.h"
#include "schema/race_setup.h"

#include <bit>
#include <climits>
#include <cmath>
#include <cstdlib>

namespace car_specs {

using G = schema::CarSpecsGlobals;
using schema::AeroPartRecord;
using schema::CarEngineStage;
using schema::CarPaintSlot;
using schema::CarPaints;
using schema::CarParams;
using schema::CarPartTables;
using schema::CarTire;
using schema::CarWheelRecord;
using schema::CarSpec;
using schema::Car;

namespace {

// `count` words from `address` byte-reversed in place (the originals' lfs,
// stfs and four byte stores; see car_specs.h).
void SwapWords(uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t a = address + 4 * i;
    guest::StoreU32(a, std::byteswap(guest::LoadU32(a)));
  }
}

void SwapHalf(uint32_t address) {
  guest::StoreU16(address, std::byteswap(guest::LoadU16(address)));
}

// The parameter object.
uint32_t Params() { return G::params().address; }

// An asset's size from its TOC entry: the unpacked size, or the stored size
// when that's 0.
uint32_t AssetSize(uint32_t toc, uint32_t id) {
  const uint32_t unpacked = guest::LoadU32(toc + 16 * id + 24);
  return unpacked != 0 ? unpacked : guest::LoadU32(toc + 16 * id + 20);
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

void CountDown(uint32_t object) {
  const CarParams params(object);
  params.set_loads_pending(params.loads_pending() - 1);
}

}  // namespace

uint32_t SwapVector(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820C9750 CarSpec::SwapVector");
  // +12 = the float at 0x82000A30 (1.0), stored as loaded.
  const uint32_t w = G::float_one();
  SwapWords(p, 3);
  guest::StoreU32(p + 12, w);
  return p;
}

uint32_t SwapMatrix(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820C9F10 CarSpec::SwapMatrix");
  SwapWords(p, 16);
  return p;
}

uint32_t Swap5(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA480 CarSpec::Swap5");
  SwapWords(p, 5);
  return p;
}

uint32_t Swap3(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA550 CarSpec::Swap3");
  SwapWords(p, 3);
  return p;
}

uint32_t Swap4(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB3B0 CarSpec::Swap4");
  SwapWords(p, 4);
  return p;
}

uint32_t SwapColor(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB530 Wheel::SwapColor");
  SwapHalf(p);
  SwapHalf(p + 4);
  SwapWords(p + 8, 1);
  return p;
}

uint32_t SwapRecord83(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB658 Params::SwapTable83Record");
  SwapWords(p, 10);
  return p;
}

uint32_t SwapEngineStage(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAAB0 EngineStage::Swap");
  // Words +8 to +196; +0 to +7 and +200 to +203 left.
  SwapWords(CarEngineStage(p).unknown_8_address(), 48);
  return p;
}

uint32_t SwapTire(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAD78 Tire::Swap");
  // Words +8 to +88; +0 to +7 left.
  SwapWords(CarTire(p).unknown_8_address(), 21);
  return p;
}

uint32_t SwapBlock1At368(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA198 CarSpec::SwapBlock1At368");
  SwapWords(p, 40);
  return p;
}

uint32_t SwapBlock2At8(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CAFF0 CarSpec::SwapBlock2At8");
  // +20 to +52 = the float at 0x82000A34 (0.0), stored as loaded.
  const uint32_t w = G::float_zero();
  SwapWords(p, 5);
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(p + 20 + 4 * i, w);
  }
  SwapWords(p + 56, 1);
  return p;
}

uint32_t SwapPaints(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB458 Paint::Swap");
  const CarPaints paints(p);
  for (uint32_t slot = paints.slots_address(), n = 0; n < 13; n++, slot += CarPaintSlot::kSize) {
    const CarPaintSlot s(slot);
    for (uint32_t offset = 0; offset <= 16; offset += 4) {
      SwapHalf(slot + offset);
    }
    // The second shade at +12 by the type (+0).
    const uint16_t type = s.type();
    if (type == 0) {
      s.set_hue2(0);
      s.set_saturation2(0);
      s.set_value2(25);
    } else if (type == 1) {
      const int32_t saturation = s.saturation0();
      s.set_hue2(s.hue0());
      s.set_saturation2(uint8_t(saturation * 60 / 100));
      s.set_value2(204);
    }
  }
  SwapWords(paints.count_address(), 1);
  return p;
}

uint32_t SwapWheel(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB588 Wheel::Swap");
  const CarWheelRecord wheel(p);
  SwapColor(wheel.color_a_half0_address());
  SwapColor(wheel.color_b_half0_address());
  SwapWords(wheel.unknown_32_address(), 4);
  return wheel.color_b_half0_address();
}

uint32_t SwapBlock1At264(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA2F8 CarSpec::SwapBlock1At264");
  Swap3(p);
  Swap3(p + 16);
  SwapWords(p + 32, 15);
  return p + 16;
}

uint32_t SwapBlock1(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CA5D0 CarSpec::SwapBlock1");
  for (uint32_t offset = 16; offset <= 80; offset += 16) {
    SwapVector(p + offset);
  }
  SwapMatrix(p + 96);
  SwapMatrix(p + 160);
  Swap5(p + 224);
  Swap5(p + 244);
  SwapBlock1At264(p + 264);
  Swap3(p + 356);
  SwapBlock1At368(p + 368);
  SwapWords(p + 528, 28);
  return p + 368;
}

uint32_t SwapBlock2At132(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB0C0 CarSpec::SwapBlock2At132");
  SwapWords(p, 10);
  for (uint32_t offset = 40; offset < 100; offset += 20) {
    Swap5(p + offset);
  }
  SwapWords(p + 100, 17);
  // r3 ends as a byte register of the swaps after the Swap5s: the last
  // byte the +104 to +124 loop loaded, the word at +124's (now at +124).
  return guest::LoadU8(p + 124);
}

uint32_t SwapBlock2(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CBE70 CarSpec::SwapBlock2");
  SwapBlock2At8(p + 8);
  SwapWords(p + 72, 14);
  SwapBlock2At132(p + 132);
  Swap4(p + 300);
  return p + 300;
}

uint32_t SwapCarSpec(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CC168 CarSpec::Swap");
  SwapBlock1(p);
  const CarSpec spec(p);
  SwapBlock2(spec.block2_id_address());
  for (uint32_t i = 0; i < 16; i++) {
    SwapEngineStage(spec.engine_stages_address() + 204 * i);
  }
  for (uint32_t i = 0; i < 21; i++) {
    SwapTire(spec.front_tires_address() + 92 * i);
    SwapTire(spec.rear_tires_address() + 92 * i);
  }
  SwapPaints(spec.paints_address());
  return SwapWheel(spec.stock_wheel_address());
}

uint32_t SwapCarSpecs(uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820CC1F0 Params::SwapCarSpecs");
  // The count and the table read from the global each time, as the original.
  for (int32_t i = 0; i < int32_t(G::params().spec_count()); i++) {
    r3 = SwapCarSpec(G::params().specs() + kSpecBytes * uint32_t(i));
  }
  return r3;
}

uint32_t SwapWheels(uint32_t r3) {
  ITC_NATIVE_RUNNING("sub_820CBE10 Params::SwapWheels");
  for (int32_t i = 0; i < int32_t(G::params().wheel_count()); i++) {
    r3 = G::params().wheels().address + kWheelBytes * uint32_t(i);
    if (r3 != 0) {
      r3 = SwapWheel(r3);
    }
  }
  return r3;
}

uint32_t OnCarSpecsLoaded(uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CC248 Params::OnCarSpecsLoaded");
  CarParams(object).set_spec_count(AssetSize(toc, 72) / kSpecBytes);
  const uint32_t r3 = SwapCarSpecs(toc);
  CountDown(object);
  return r3;
}

uint32_t OnWheelsLoaded(uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CC0A8 Params::OnWheelsLoaded");
  const CarParams params(object);
  params.set_wheel_count(AssetSize(toc, 73) / kWheelBytes);
  const uint32_t r3 = SwapWheels(toc);
  // Each record's model (+40) up by one, and the first record of each
  // model at +64 + 4 × model (model 1's at +68 = 0 from the start).
  uint32_t model = 1;
  params.set_model1_first_wheel(0);
  for (int32_t i = 0; i < int32_t(params.wheel_count()); i++) {
    const CarWheelRecord record(params.wheels().address + kWheelBytes * uint32_t(i));
    const uint32_t m = record.model() + 1;
    record.set_model(m);
    if (m != model) {
      model = m;
      params.store_at(64 + 4 * m, 4, uint32_t(i));
    }
  }
  params.set_model_end(model + 1);
  CountDown(object);
  return r3;
}

void OnLoaded(uint32_t object) {
  ITC_NATIVE_RUNNING("sub_820C9DF0 Params::OnLoaded");
  CountDown(object);
}

uint32_t OnTable83Loaded(uint32_t object, uint32_t toc) {
  ITC_NATIVE_RUNNING("sub_820CBD90 Params::OnTable83Loaded");
  uint32_t r3 = toc;
  const uint32_t count = AssetSize(toc, 83) / 40;
  if (int32_t(count) > 0) {
    for (uint32_t i = 0; i < count; i++) {
      r3 = SwapRecord83(i * 40 + CarParams(object).table83());
    }
  }
  CountDown(object);
  return r3;
}

uint32_t GetCarSpec(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_820C9E98 Params::GetCarSpec");
  return G::params().specs() + kSpecBytes * id;
}

uint32_t GetWheel(int32_t n) {
  ITC_NATIVE_RUNNING("sub_820C9EB0 Params::GetWheel");
  if (n < 1) {
    return 0;
  }
  return G::params().wheels().address + kWheelBytes * uint32_t(n) - kWheelBytes;
}

uint32_t FirstWheelOfModel(int32_t m) {
  ITC_NATIVE_RUNNING("sub_820C9EE0 Params::FirstWheelOfModel");
  if (m < 1) {
    return 0;
  }
  return uint32_t(G::params().load_at(64 + 4 * uint32_t(m), 4)) + 1;
}

void GetCarParts(uint32_t car, uint32_t parts[9]) {
  ITC_NATIVE_RUNNING("sub_820C9E00 Params::GetCarParts");
  static constexpr uint32_t kStrides[9] = {280, 280, 280, 280, 168, 168, 84, 108, 72};
  const CarParams params = G::params();
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = uint32_t(params.load_at(16 + 4 * i, 4)) + kStrides[i] * car;
  }
}

uint32_t GetTire(uint32_t spec, int32_t type, uint32_t grade, int32_t rear) {
  ITC_NATIVE_RUNNING("sub_820C9928 CarSpec::GetTire");
  const uint32_t index = type == 0 ? 0 : uint32_t(type) * 5 + grade - 5;
  return index * 92 + spec + (rear == 0 ? 4244 : 6176);
}

uint32_t Exists() {
  ITC_NATIVE_RUNNING("sub_820C9DD8 Params::Exists");
  return Params() != 0 ? 1 : 0;
}

uint32_t SwapAeroPart(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB6C8 AeroPart::Swap");
  SwapWords(p, 7);
  return p;
}

uint32_t SwapAeroPart36(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_820CB7E8 AeroPart::Swap36");
  SwapAeroPart(p);
  SwapWords(p + 28, 2);
  return p;
}

namespace {

// An aero part getter: table `table` (its pointer at parts + 4 × table), its
// records `size` bytes; `use` whether the level picks a record.
bool AeroPart(uint32_t out, const uint32_t parts[9], int32_t level,
              uint32_t table, uint32_t size, bool use) {
  if (use) {
    const uint32_t from = parts[table] + size * uint32_t(level) - size;
    if (!CopyFits(out, from, size)) {
      return false;
    }
    c_runtime::Memcpy(out, from, size);
    if (size == 28) {
      SwapAeroPart(out);
    } else {
      SwapAeroPart36(out);
    }
    return true;
  }
  // The defaults: the floats at 0x82000A34 (0.0) and 0x82000A30 (1.0),
  // stored as loaded.
  const AeroPartRecord record(out);
  const uint32_t zero = G::float_zero();
  const uint32_t one = G::float_one();
  guest::StoreU32(record.weight_change_address(), zero);
  guest::StoreU32(record.weight_change_2_address(), zero);
  guest::StoreU32(record.lift_address(), zero);
  guest::StoreU32(record.lift_2_address(), zero);
  guest::StoreU32(record.factor_16_address(), one);
  guest::StoreU32(record.factor_20_address(), one);
  record.set_flags(0);
  if (size == 36) {
    guest::StoreU32(record.extra_28_address(), 0);
    guest::StoreU32(record.extra_32_address(), 0);
  }
  return true;
}

}  // namespace

#define ITC_AERO_PART(number, address, table, size, use)                       \
  bool AeroPart##number(uint32_t out, const uint32_t parts[9], int32_t level) { \
    ITC_NATIVE_RUNNING("sub_" #address " AeroParts::Get" #number);             \
    return AeroPart(out, parts, level, table, size, use);                \
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

uint32_t CountBodyKits(uint32_t parts) {
  ITC_NATIVE_RUNNING("sub_820CBD20 CountBodyKits");
  // The original copies each record to the stack and swaps it there; only
  // the first float is read.
  const float zero = std::bit_cast<float>(G::float_zero());
  uint32_t count = 0;
  for (uint32_t offset = 0; offset < 72; offset += 36) {
    const uint32_t word =
        guest::LoadU32(CarPartTables(parts).table82().address + offset);
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
double LoadSingle(uint32_t address) {
  return double(guest::LoadFloat(address));
}
void StoreSingle(uint32_t address, double x) {
  guest::StoreFloat(address, float(x));
}
// A constant of the game's at 0x82000000 + offset.
double Constant(uint32_t offset) { return LoadSingle(0x82000000u + offset); }
double ConstantDouble(uint32_t offset) {
  const uint64_t bits = uint64_t(guest::LoadU32(0x82000000u + offset)) << 32 |
                        guest::LoadU32(0x82000000u + offset + 4);
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

void InchUp(uint32_t tire, int32_t n) {
  ITC_NATIVE_RUNNING("sub_820C46B8 Tire::InchUp");
  if (n == 0) {
    return;
  }
  // The registers' steps, in the original's order (f0 to f13 as named there).
  const CarTire t(tire);
  double f0 = t.width();
  double f13 = t.rim();
  double f11 = t.aspect();
  f11 = Single(f0 * f11);
  double f4 = t.radius();
  double f3 = t.grip();
  double f10 = Constant(8708);
  const double f9 = Constant(8148);
  const double f8 = Constant(11536);
  double f7 = Constant(8680);
  const double f6 = Constant(11532);
  double f5 = Single(f13 * f6);
  f11 = Single(std::fma(f11, f9, f5));
  f5 = Constant(11468);
  double f2 = Single(f11 * f5);
  f11 = Constant(2608);
  double f12 = Single(double(int64_t(n)));
  f13 = Single(f12 + f13);
  t.set_rim(float(f13));
  f12 = Single(std::fma(f12, f10, f11));
  f10 = Single(f2 * f0);
  f12 = Single(f12 * f0);
  f0 = Single(std::sqrt(f10));
  f10 = Constant(7132);
  f2 = Single(f12 * f10);
  f0 = Single(std::fma(f0, f8, f11));
  f12 = Constant(11560);
  f12 = Single(f13 * f12);
  const int32_t width = Fctiwz(f2);
  f4 = Single(std::fma(f4, f7, -f12));
  f12 = Constant(7140);
  f0 = Single(f0 * f12);
  double f1 = Single(f4 * f12);
  f4 = Constant(10716);
  f0 = Single(f0 * f4);
  f2 = Single(f3 / f0);
  // The width in mm down to a multiple of 5 (divw, then mulli).
  const int32_t width5 = int32_t(uint32_t(width / 5) * 5u);
  f3 = Single(double(int64_t(width5)));
  f0 = Constant(10712);
  f0 = Single(f3 * f0);
  t.set_width(float(f0));
  f3 = Single(f1 / f0);
  t.set_aspect(float(f3));
  const bool below = f0 < Constant(11556);
  f3 = ConstantDouble(8888);
  f1 = Constant(11552);
  f1 = Single(std::fma(f0, f10, -f1));
  f10 = Constant(8124);
  f1 = Single(f1 * f10);
  f10 = std::fma(f1, f3, ConstantDouble(11544));
  if (!below) {
    f10 = f10 - f3;
  }
  f10 = Single(f10);
  t.set_rim_width(float(f10));
  f13 = Single(f13 * f6);
  f10 = Single(f10 * f7);
  f7 = t.aspect();
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
  t.set_rim_width(float(Single(double(int64_t(rim5)))));
  f0 = Single(f0 * f2);
  f0 = Single(f0 * f4);
  t.set_grip(float(f0));
  t.set_grip_2(float(f0));
}

uint32_t CopySpec(uint32_t car, uint32_t spec) {
  ITC_NATIVE_RUNNING("sub_820C4888 Car::CopySpec");
  const Car c(car);
  const CarSpec s(spec);
  const uint32_t zero = G::float_zero();
  guest::StoreU32(c.front_tire_widening_address(), zero);
  guest::StoreU32(c.rear_tire_widening_address(), zero);
  c_runtime::Memcpy(c.block1_address(), spec, 640);
  c_runtime::Memcpy(c.block2_address(), s.block2_id_address(), 340);
  c_runtime::Memcpy(c.engine_address(),
                    c.engine_stage() * 204 + s.engine_stages_address(), 204);
  c_runtime::Memcpy(c.paints_address(), s.paints_address(), 272);
  c_runtime::Memcpy(c.front_tire_address(),
                    GetTire(spec, int32_t(c.front_tire_type()), c.front_tire_grade(), 0),
                    92);
  c_runtime::Memcpy(c.rear_tire_address(),
                    GetTire(spec, int32_t(c.rear_tire_type()), c.rear_tire_grade(), 1),
                    92);
  const double front = double(c.front_tire_width());
  const double rear = double(c.rear_tire_width());
  InchUp(c.front_tire_address(), int32_t(c.front_inch_up()));
  InchUp(c.rear_tire_address(), int32_t(c.rear_inch_up()));
  c.set_front_tire_widening(float(Single(double(c.front_tire_width()) - front)));
  c.set_rear_tire_widening(float(Single(double(c.rear_tire_width()) - rear)));
  return c.rear_tire_address();
}

uint32_t CopyAeroParts(uint32_t car, const uint32_t parts[9]) {
  ITC_NATIVE_RUNNING("sub_820C4568 Car::CopyAeroParts");
  const Car c(car);
  AeroPart74(c.aero_parts_address() + 0, parts, int32_t(c.aero_0()));
  AeroPart75(c.aero_parts_address() + 28, parts, int32_t(c.aero_1()));
  AeroPart76(c.aero_parts_address() + 56, parts, int32_t(c.aero_2()));
  AeroPart77(c.aero_parts_address() + 84, parts, int32_t(c.aero_3()) == -1 ? 0 : int32_t(c.aero_3()));
  AeroPart80(c.aero_parts_address() + 140, parts, int32_t(c.aero_6()));
  AeroPart81(c.aero_parts_address() + 168, parts, int32_t(c.aero_9()));
  AeroPart82(c.aero_parts_address() + 204, parts, int32_t(c.aero_10()));
  if (c.bonnet_color_kind() == 0) {
    AeroPart78(c.aero_parts_address() + 112, parts, int32_t(c.bonnet()));
  } else {
    AeroPart79(c.aero_parts_address() + 112, parts, int32_t(c.bonnet()));
  }
  return c.aero_parts_address() + 112;
}

namespace {

// A wheel record for the car's wheel number at `number` (its address): the
// spec's stock wheel for 0, else Params::GetWheel's.
uint32_t WheelOf(uint32_t number, uint32_t spec) {
  const int32_t n = int32_t(guest::LoadU32(number));
  return n == 0 ? CarSpec(spec).stock_wheel_address() : GetWheel(n);
}

}  // namespace

bool CarRefresh(uint32_t car, uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820C4998 Car::Refresh");
  const Car c(car);
  const uint32_t spec = GetCarSpec(c.car_id());
  const CarSpec s(spec);
  uint32_t parts[9];
  GetCarParts(s.list_kind() != 0 ? 0 : c.car_id(), parts);
  // Every copy's ranges first (the original's order of reads only differs
  // from these where the car's written span, +1888 to +3884, shares memory
  // with what's read after it, which is checked too).
  struct Copy {
    uint32_t to, from, size;
  };
  auto aero = [&](uint32_t to, uint32_t table, uint32_t size, int32_t level, bool use) {
    return Copy{to, use ? parts[table] + size * uint32_t(level) - size : to, use ? size : 0};
  };
  const int32_t level77 = int32_t(c.aero_3()) == -1 ? 0 : int32_t(c.aero_3());
  const bool use79 = c.bonnet_color_kind() != 0;
  const Copy copies[] = {
      {c.block1_address(), spec, 640},
      {c.block2_address(), s.block2_id_address(), 340},
      {c.engine_address(), c.engine_stage() * 204 + s.engine_stages_address(), 204},
      {c.paints_address(), s.paints_address(), 272},
      {c.front_tire_address(),
       GetTire(spec, int32_t(c.front_tire_type()), c.front_tire_grade(), 0), 92},
      {c.rear_tire_address(),
       GetTire(spec, int32_t(c.rear_tire_type()), c.rear_tire_grade(), 1), 92},
      aero(c.aero_parts_address() + 0, 0, 28, int32_t(c.aero_0()), int32_t(c.aero_0()) != 0),
      aero(c.aero_parts_address() + 28, 1, 28, int32_t(c.aero_1()), int32_t(c.aero_1()) != 0),
      aero(c.aero_parts_address() + 56, 2, 28, int32_t(c.aero_2()), int32_t(c.aero_2()) != 0),
      aero(c.aero_parts_address() + 84, 3, 28, level77, level77 > 0),
      aero(c.aero_parts_address() + 140, 6, 28, int32_t(c.aero_6()), int32_t(c.aero_6()) != 0),
      aero(c.aero_parts_address() + 168, 7, 36, int32_t(c.aero_9()), int32_t(c.aero_9()) != 0),
      aero(c.aero_parts_address() + 204, 8, 36, int32_t(c.aero_10()), int32_t(c.aero_10()) != 0),
      aero(c.aero_parts_address() + 112, use79 ? 5 : 4, 28, int32_t(c.bonnet()), int32_t(c.bonnet()) != 0),
      {c.front_wheel_spec_address(), WheelOf(c.front_wheel_address(), spec), 48},
      {c.rear_wheel_spec_address(), WheelOf(c.rear_wheel_address(), spec), 48},
  };
  const uint32_t span = c.block2_address(), span_bytes = 3884 - 1888;
  if (span >= 0xA0000000u || !c_runtime::InOneView(span, span_bytes) ||
      c_runtime::MayShare(span, span_bytes, spec, kSpecBytes) ||
      c_runtime::MayShare(span, span_bytes, Params(), CarParams::kSize)) {
    return false;
  }
  for (const Copy& copy : copies) {
    if (copy.size != 0 && (!CopyFits(copy.to, copy.from, copy.size) ||
                           c_runtime::MayShare(span, span_bytes, copy.from, copy.size))) {
      return false;
    }
  }
  CopySpec(car, spec);
  CopyAeroParts(car, parts);
  c_runtime::Memcpy(c.front_wheel_spec_address(), WheelOf(c.front_wheel_address(), spec), 48);
  c_runtime::Memcpy(c.rear_wheel_spec_address(), WheelOf(c.rear_wheel_address(), spec), 48);
  *r3 = c.rear_wheel_spec_address();
  return true;
}

bool ParamsInit(const uint32_t tocs[12], uint32_t* r3) {
  ITC_NATIVE_RUNNING("sub_820C9960 Params::Init");
  uint32_t sizes[12];
  uint32_t total = 0;
  for (uint32_t k = 0; k < 12; k++) {
    sizes[k] = AssetSize(tocs[k], 72 + k);
    total += sizes[k];
  }
  // operator new's and new[]'s failures (the new handler, std::bad_alloc)
  // and the original's endless loop for a total of 0 stay the original's.
  constexpr uint32_t kMaxAllocation = 0xFFFFF000;
  if (total == 0 || total > kMaxAllocation || memory::crt::GetProcessHeap() == 0) {
    return false;
  }
  // The object in host memory and the block a blob (E5 steps 4 and 5): the
  // original's operator new and new[] (the checks above stay theirs).
  const uint32_t object = CarParams::New().address;
  G::set_params(object);
  CarParams(object).set_block(0);
  G::params().set_spec_count(0);
  G::params().set_wheel_count(0);
  for (uint32_t i = 0; i < 100; i++) {
    G::params().store_at(64 + 4 * i, 4, 0xFFFFFFFFu);
  }
  G::params().set_loads_pending(0);
  // Two passes (the original's loop until the block is made): the first
  // with no block (every pointer 0) for the total, the second with it.
  // The table pointers' slots by asset: 72 at +12, 73 at +8, 74 to 82 at
  // +16 to +48, 83 at +52.
  // By offset (the view's store_at: the object is in host memory).
  static constexpr uint32_t kSlots[12] = {12, 8, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52};
  uint32_t r30 = 0;
  while (G::params().block() == 0) {
    if (r30 != 0) {
      G::params().set_block(schema::CarParamsBlock::New(r30).address);
    }
    r30 = G::params().block();
    G::params().set_loads_pending(0);
    uint32_t sum = 0;
    for (uint32_t k = 0; k < 12; k++) {
      if (k > 0) {
        r30 = r30 != 0 ? G::params().block() + sum : 0;
      }
      const CarParams params = G::params();
      params.set_loads_pending(params.loads_pending() + 1);
      params.store_at(kSlots[k], 4, r30);
      sum += sizes[k];
    }
    r30 = sum;
  }
  *r3 = tocs[11];
  return true;
}

int64_t ParamsLoad(const archive::LoadCall& load) {
  ITC_NATIVE_RUNNING("sub_820CC2A8 Params::Load");
  struct Asset {
    uint32_t slot;  // the table's pointer's offset in the object (host memory: load_at)
    uint32_t callback;
  };
  static constexpr Asset kAssets[12] = {
      {12, 0x820CC248}, {8, 0x820CC0A8},  {16, 0x820C9DF0}, {20, 0x820C9DF0},
      {24, 0x820C9DF0}, {28, 0x820C9DF0}, {32, 0x820C9DF0}, {36, 0x820C9DF0},
      {40, 0x820C9DF0}, {44, 0x820C9DF0}, {48, 0x820C9DF0}, {52, 0x820CBD90}};
  int64_t serial = 0;
  for (uint32_t k = 0; k < 12; k++) {
    const CarParams params = G::params();
    serial = load(72 + k, uint32_t(params.load_at(kAssets[k].slot, 4)), kAssets[k].callback,
                  params.address);
  }
  return serial;
}

}  // namespace car_specs
