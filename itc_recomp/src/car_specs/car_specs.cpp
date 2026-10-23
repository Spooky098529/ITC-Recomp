// The car parameter tables as plain C++: see car_specs.h.

#include "car_specs/car_specs.h"

#include "base/log.h"
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

}  // namespace car_specs
