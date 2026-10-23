// The car parameter tables as plain C++: see car_specs.h.

#include "car_specs/car_specs.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <bit>

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

void GetCarParts(uint8_t* base, uint32_t out, uint32_t car) {
  ITC_NATIVE_RUNNING("sub_820C9E00 Params::GetCarParts");
  static constexpr uint32_t kStrides[9] = {280, 280, 280, 280, 168, 168, 84, 108, 72};
  // Every table read before any store, as the original.
  const uint32_t params = Params(base);
  uint32_t parts[9];
  for (uint32_t i = 0; i < 9; i++) {
    parts[i] = guest::LoadU32(base, params + 16 + 4 * i) + kStrides[i] * car;
  }
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, out + 4 * i, parts[i]);
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
bool AeroPart(uint8_t* base, uint32_t out, uint32_t parts, int32_t level, uint32_t table,
              uint32_t size, bool use) {
  if (use) {
    const uint32_t from =
        guest::LoadU32(base, parts + 4 * table) + size * uint32_t(level) - size;
    if (out >= 0xA0000000u || !c_runtime::InOneView(out, size) ||
        !c_runtime::InOneView(from, size) || c_runtime::MayShare(out, size, from, size)) {
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
  bool AeroPart##number(uint8_t* base, uint32_t out, uint32_t parts, int32_t level) { \
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

}  // namespace car_specs
