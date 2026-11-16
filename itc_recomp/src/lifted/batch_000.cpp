// Lift sweep batch 000 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_000, true, "ITC",
              "Run lift sweep batch 000's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82090840: lifted from generated/ by lift.py.
uint64_t f82090840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090840 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x824936e8
  r3 = f824936E8(base, r3);
  return r3;
}

// sub_82090890: lifted from generated/ by lift.py.
uint64_t f82090890(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090890 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-20
  r3 = uint64_t(int64_t(int64_t(r3) + -20));
  // b 0x824936e8
  r3 = f824936E8(base, r3);
  return r3;
}

// sub_820951D8: lifted from generated/ by lift.py.
double f820951D8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820951D8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lwa r11,236(r3)
  r11 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r3) + 236))));
  // lfs f0,244(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 244);
  f0 = double(temp.f32);
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,-16(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // lfs f0,3860(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3860);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // blr
  return f1;
}

// sub_821128A0: lifted from generated/ by lift.py.
void f821128A0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821128A0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
loc_821128A8:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x821128d0
  if (cr6.lt) goto loc_821128D0;
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // clrlwi r9,r11,27
  r9 = uint32_t(r11) & 0x1F;
  // srawi r10,r11,5
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1F) != 0);
  r10 = uint64_t(int64_t(int32_t(r11) >> 5));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // slw r9,r8,r9
  r9 = uint8_t(r9) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r9) & 0x3F));
  // lwzx r8,r10,r3
  r8 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r3));
  // or r9,r9,r8
  r9 = r9 | r8;
  // stwx r9,r10,r3
  REX_STORE_U32(uint32_t(r10) + uint32_t(r3), uint32_t(r9));
loc_821128D0:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r11,24
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 24, so);
  // blt cr6,0x821128a8
  if (cr6.lt) goto loc_821128A8;
  // blr
  return;
}

// sub_8214B2C8: lifted from generated/ by lift.py.
uint64_t f8214B2C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8214B2C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r4,r3
  r4 = r3;
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // lwz r10,4(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8214b2ec
  if (!cr6.gt) goto loc_8214B2EC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B2EC:
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8214b304
  if (!cr6.gt) goto loc_8214B304;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B304:
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // bl 0x8214b188
  // (left out: dead in C++)
  r3 = f8214B188(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_822ADA28: lifted from generated/ by lift.py.
uint64_t f822ADA28(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822ADA28 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r10,10420(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 10420);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // rlwinm. r9,r10,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x822ada90
  if (cr0.eq) goto loc_822ADA90;
  // rlwinm. r9,r10,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x822ada90
  if (cr0.eq) goto loc_822ADA90;
  // rlwinm r11,r10,28,29,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 28) & 0x7;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // beq cr6,0x822ada8c
  if (cr6.eq) goto loc_822ADA8C;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // beq cr6,0x822ada8c
  if (cr6.eq) goto loc_822ADA8C;
  // lwz r10,10432(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 10432);
  // rlwinm. r10,r10,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822ada78
  if (!cr0.eq) goto loc_822ADA78;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x822ada8c
  if (cr6.eq) goto loc_822ADA8C;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // beq cr6,0x822ada8c
  if (cr6.eq) goto loc_822ADA8C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x822ada90
  goto loc_822ADA90;
loc_822ADA78:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x822ada8c
  if (cr6.eq) goto loc_822ADA8C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // bne cr6,0x822ada90
  if (!cr6.eq) goto loc_822ADA90;
loc_822ADA8C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_822ADA90:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_822B43C0: lifted from generated/ by lift.py.
uint64_t f822B43C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822B43C0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, ctr = 0;
  // addi r11,r3,20136
  r11 = uint64_t(int64_t(int64_t(r3) + 20136));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_822B43D0:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x822b43d0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822B43D0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_822B6070: lifted from generated/ by lift.py.
lifted::Result f822B6070(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822B6070 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lis r5,2
  r5 = uint64_t(int64_t(131072));
  // lis r4,32528
  r4 = uint64_t(int64_t(2131755008));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbb94
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeLockL2, LIFTED_ARGS(0x822B6094, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_822B6098:
  // lis r10,32528
  r10 = uint64_t(int64_t(2131755008));
  // dcbzl r11,r10
  ea = (uint32_t(r11) + uint32_t(r10)) & ~127;
  memset((void*)REX_RAW_ADDR(ea), 0, 128);
  // addi r11,r11,128
  r11 = uint64_t(int64_t(int64_t(r11) + 128));
  // lis r10,2
  r10 = uint64_t(int64_t(131072));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x822b6098
  if (cr6.lt) goto loc_822B6098;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82578470: lifted from generated/ by lift.py.
uint64_t f82578470(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82578470 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // cmplw cr6,r9,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r4), so);
  // bge cr6,0x82578494
  if (!cr6.lt) goto loc_82578494;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,122
  r3 = r3 | 122;
  // blr
  return r3;
loc_82578494:
  // add r10,r10,r4
  r10 = r10 + r4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // blr
  return r3;
}

// sub_8257D6F8: lifted from generated/ by lift.py.
void f8257D6F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257D6F8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lwz r11,200(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 200);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,200(r3)
  REX_STORE_U32(uint32_t(r3) + 200, uint32_t(r11));
  // bnelr
  if (!cr0.eq) return;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,140(r3)
  REX_STORE_U32(uint32_t(r3) + 140, uint32_t(r11));
  // blr
  return;
}

// sub_825BFCC8: lifted from generated/ by lift.py.
uint64_t f825BFCC8(uint8_t* base, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825BFCC8 (lifted)");
  uint64_t r0 = 0, r3 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,0(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 0);
  // lbz r10,0(r5)
  r10 = REX_LOAD_U8(uint32_t(r5) + 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825bfd90
  if (!cr6.eq) goto loc_825BFD90;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplwi cr6,r11,14
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 14, so);
  // bgt cr6,0x825bfd90
  if (cr6.gt) goto loc_825BFD90;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,-772
  r12 = uint64_t(int64_t(int64_t(r12) + -772));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825BFD38;
  case 1:
  goto loc_825BFD38;
  case 2:
  goto loc_825BFD38;
  case 3:
  goto loc_825BFD38;
  case 4:
  goto loc_825BFD38;
  case 5:
  goto loc_825BFD78;
  case 6:
  goto loc_825BFD98;
  case 7:
  goto loc_825BFD38;
  case 8:
  goto loc_825BFDF0;
  case 9:
  goto loc_825BFDF0;
  case 10:
  goto loc_825BFDF0;
  case 11:
  goto loc_825BFDA0;
  case 12:
  goto loc_825BFDA0;
  case 13:
  goto loc_825BFD98;
  case 14:
  goto loc_825BFD98;
  default:
  std::abort();
  }
loc_825BFD38:
  // lwz r11,2(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 2);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bfd64
  if (cr6.eq) goto loc_825BFD64;
  // lwz r10,11(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 11);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825bfd90
  if (!cr6.eq) goto loc_825BFD90;
  // lhz r11,14(r5)
  r11 = REX_LOAD_U16(uint32_t(r5) + 14);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
  // lhz r10,9(r4)
  r10 = REX_LOAD_U16(uint32_t(r4) + 9);
  // b 0x825bfd88
  goto loc_825BFD88;
loc_825BFD64:
  // lwz r11,10(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 10);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
  // lwz r10,15(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 15);
  // b 0x825bfd88
  goto loc_825BFD88;
loc_825BFD78:
  // lwz r11,2(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 2);
loc_825BFD7C:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
loc_825BFD84:
  // lwz r10,9(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 9);
loc_825BFD88:
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
loc_825BFD90:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_825BFD98:
  // lwz r11,6(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 6);
  // b 0x825bfd7c
  goto loc_825BFD7C;
loc_825BFDA0:
  // lwz r11,2(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 2);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bfdcc
  if (cr6.eq) goto loc_825BFDCC;
  // lwz r10,17(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 17);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825bfd90
  if (!cr6.eq) goto loc_825BFD90;
  // lhz r11,14(r5)
  r11 = REX_LOAD_U16(uint32_t(r5) + 14);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
  // lhz r10,15(r4)
  r10 = REX_LOAD_U16(uint32_t(r4) + 15);
  // b 0x825bfd88
  goto loc_825BFD88;
loc_825BFDCC:
  // lwz r11,6(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 6);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bfd84
  if (!cr6.eq) goto loc_825BFD84;
  // lwz r11,10(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 10);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bfdf0
  if (cr6.eq) goto loc_825BFDF0;
  // lwz r10,21(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 21);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825bfd90
  if (!cr6.eq) goto loc_825BFD90;
loc_825BFDF0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_825C09A8: lifted from generated/ by lift.py.
lifted::Result f825C09A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C09A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,-22856
  r3 = uint64_t(int64_t(int64_t(r11) + -22856));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // addi r29,r11,21344
  r29 = uint64_t(int64_t(int64_t(r11) + 21344));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // addi r28,r11,21320
  r28 = uint64_t(int64_t(int64_t(r11) + 21320));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r27,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r27));
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // addi r10,r31,28
  r10 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // addi r9,r31,44
  r9 = uint64_t(int64_t(int64_t(r31) + 44));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // addi r8,r31,56
  r8 = uint64_t(int64_t(int64_t(r31) + 56));
  // stw r30,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r30));
  // stw r26,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r26));
  // addi r7,r31,76
  r7 = uint64_t(int64_t(int64_t(r31) + 76));
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // addi r6,r31,88
  r6 = uint64_t(int64_t(int64_t(r31) + 88));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addi r5,r31,100
  r5 = uint64_t(int64_t(int64_t(r31) + 100));
  // stw r10,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r10));
  // addi r4,r31,112
  r4 = uint64_t(int64_t(int64_t(r31) + 112));
  // stw r10,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r10));
  // addi r10,r31,128
  r10 = uint64_t(int64_t(int64_t(r31) + 128));
  // stw r30,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r30));
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // stw r9,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r9));
  // stw r9,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r9));
  // addi r9,r31,140
  r9 = uint64_t(int64_t(int64_t(r31) + 140));
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // stw r8,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r8));
  // addi r3,r9,4
  r3 = uint64_t(int64_t(int64_t(r9) + 4));
  // stw r8,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r8));
  // lis r8,-32251
  r8 = uint64_t(int64_t(-2113601536));
  // stw r30,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r30));
  // stw r30,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r30));
  // addi r8,r8,-22424
  r8 = uint64_t(int64_t(int64_t(r8) + -22424));
  // stw r11,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r11));
  // stw r7,4(r7)
  REX_STORE_U32(uint32_t(r7) + 4, uint32_t(r7));
  // stw r7,8(r7)
  REX_STORE_U32(uint32_t(r7) + 8, uint32_t(r7));
  // stw r11,0(r6)
  REX_STORE_U32(uint32_t(r6) + 0, uint32_t(r11));
  // stw r6,4(r6)
  REX_STORE_U32(uint32_t(r6) + 4, uint32_t(r6));
  // stw r6,8(r6)
  REX_STORE_U32(uint32_t(r6) + 8, uint32_t(r6));
  // stw r11,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r11));
  // stw r5,4(r5)
  REX_STORE_U32(uint32_t(r5) + 4, uint32_t(r5));
  // stw r5,8(r5)
  REX_STORE_U32(uint32_t(r5) + 8, uint32_t(r5));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // stw r4,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r4));
  // stw r4,8(r4)
  REX_STORE_U32(uint32_t(r4) + 8, uint32_t(r4));
  // stw r30,124(r31)
  REX_STORE_U32(uint32_t(r31) + 124, uint32_t(r30));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r10,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r10));
  // stw r10,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r10));
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS(0x825C0AAC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32215
  r11 = uint64_t(int64_t(-2111242240));
  // stw r30,180(r31)
  REX_STORE_U32(uint32_t(r31) + 180, uint32_t(r30));
  // lis r10,-32215
  r10 = uint64_t(int64_t(-2111242240));
  // stw r30,184(r31)
  REX_STORE_U32(uint32_t(r31) + 184, uint32_t(r30));
  // addi r11,r11,24352
  r11 = uint64_t(int64_t(int64_t(r11) + 24352));
  // lis r8,32767
  r8 = uint64_t(int64_t(2147418112));
  // li r9,42
  r9 = uint64_t(int64_t(42));
  // ori r8,r8,65535
  r8 = r8 | 65535;
  // addi r10,r10,24520
  r10 = uint64_t(int64_t(int64_t(r10) + 24520));
  // stw r11,192(r31)
  REX_STORE_U32(uint32_t(r31) + 192, uint32_t(r11));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,5248
  r30 = uint64_t(int64_t(int64_t(r11) + 5248));
  // stw r9,172(r31)
  REX_STORE_U32(uint32_t(r31) + 172, uint32_t(r9));
  // stw r8,176(r31)
  REX_STORE_U32(uint32_t(r31) + 176, uint32_t(r8));
  // stw r10,188(r31)
  REX_STORE_U32(uint32_t(r31) + 188, uint32_t(r10));
  // ld r11,0(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 0);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x825c0b3c
  if (!cr6.eq) goto loc_825C0B3C;
  // mr r3,r30
  r3 = r30;
  // bl 0x825b9a70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9A70, LIFTED_ARGS(0x825C0AFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // stw r3,-12816(r11)
  REX_STORE_U32(uint32_t(r11) + -12816, uint32_t(r3));
  // mr r3,r31
  r3 = r31;
  // beq cr6,0x825c0b40
  if (cr6.eq) goto loc_825C0B40;
  // ld r11,0(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 0);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f0,-30712(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -30712)));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // fdiv f0,f0,f13
  f0 = f0 / f13;
  // stfd f0,5256(r11)
  REX_STORE_U64(uint32_t(r11) + 5256, std::bit_cast<uint64_t>(f0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0B3C:
  // mr r3,r31
  r3 = r31;
loc_825C0B40:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CD358: lifted from generated/ by lift.py.
void f825CD358(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825CD358 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lfs f0,20(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f1
  cr6 = lifted::CompareFloat(f0, f1);
  // beqlr cr6
  if (cr6.eq) return;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stfs f1,20(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // stw r11,24(r3)
  REX_STORE_U32(uint32_t(r3) + 24, uint32_t(r11));
  // blr
  return;
}

// sub_825CFF70: lifted from generated/ by lift.py.
void f825CFF70(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825CFF70 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r3,308
  r11 = uint64_t(int64_t(int64_t(r3) + 308));
  // stw r11,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,8(r4)
  REX_STORE_U32(uint32_t(r4) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r4,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r4));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
  // blr
  return;
}

// sub_825D36F0: lifted from generated/ by lift.py.
uint64_t f825D36F0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D36F0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // clrlwi r8,r4,16
  r8 = uint32_t(r4) & 0xFFFF;
  // lhz r11,8(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 8);
  // cmplwi cr6,r8,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 65535, so);
  // bne cr6,0x825d3708
  if (!cr6.eq) goto loc_825D3708;
  // mullw r11,r11,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r11))));
  // b 0x825d370c
  goto loc_825D370C;
loc_825D3708:
  // mullw r11,r11,r8
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r8))));
loc_825D370C:
  // clrlwi r9,r5,16
  r9 = uint32_t(r5) & 0xFFFF;
  // clrlwi r7,r11,16
  r7 = uint32_t(r11) & 0xFFFF;
  // cmplwi cr6,r9,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 65535, so);
  // bne cr6,0x825d3728
  if (!cr6.eq) goto loc_825D3728;
  // lhz r11,8(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 8);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x825d372c
  goto loc_825D372C;
loc_825D3728:
  // mr r11,r9
  r11 = r9;
loc_825D372C:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // mr r10,r11
  r10 = r11;
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // ble cr6,0x825d3750
  if (!cr6.gt) goto loc_825D3750;
  // cmplwi cr6,r9,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 65535, so);
  // beq cr6,0x825d3750
  if (cr6.eq) goto loc_825D3750;
  // addis r11,r10,1
  r11 = uint64_t(int64_t(int64_t(r10) + 65536));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
loc_825D3750:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // clrlwi r9,r7,16
  r9 = uint32_t(r7) & 0xFFFF;
  // add r11,r11,r9
  r11 = r11 + r9;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // mulli r11,r11,26
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(26))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // blr
  return r3;
}

// sub_825D94C8: lifted from generated/ by lift.py.
void f825D94C8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D94C8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lfs f0,12968(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 12968);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,27036(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 27036);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23380(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23380, temp.u32);
  // blr
  return;
}

// sub_825D99A0: lifted from generated/ by lift.py.
void f825D99A0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D99A0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // addi r11,r11,24008
  r11 = uint64_t(int64_t(int64_t(r11) + 24008));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
loc_825D99B4:
  // stw r9,-4(r11)
  REX_STORE_U32(uint32_t(r11) + -4, uint32_t(r9));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // addi r11,r11,36
  r11 = uint64_t(int64_t(int64_t(r11) + 36));
  // bge 0x825d99b4
  if (!cr0.lt) goto loc_825D99B4;
  // blr
  return;
}

// sub_825D9C40: lifted from generated/ by lift.py.
uint64_t f825D9C40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D9C40 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // addi r3,r11,29464
  r3 = uint64_t(int64_t(int64_t(r11) + 29464));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x825a71d0
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825DA0A0: lifted from generated/ by lift.py.
uint64_t f825DA0A0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DA0A0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0;
  double f1 = 0.0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r4,r11,19924
  r4 = uint64_t(int64_t(int64_t(r11) + 19924));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r3,r11,19960
  r3 = uint64_t(int64_t(int64_t(r11) + 19960));
  // b 0x8240fba0
  f8240FBA0(base, r3, r4);
  return r3;
}

// sub_825DA5D0: lifted from generated/ by lift.py.
void f825DA5D0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DA5D0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r10,r11,-19680
  r10 = uint64_t(int64_t(int64_t(r11) + -19680));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,3884
  r11 = uint64_t(int64_t(int64_t(r11) + 3884));
  // lwz r9,-4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -4);
  // mr r8,r9
  r8 = r9;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
loc_825DA5EC:
  // lbz r7,0(r9)
  r7 = REX_LOAD_U8(uint32_t(r9) + 0);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // bne cr6,0x825da5ec
  if (!cr6.eq) goto loc_825DA5EC;
  // subf r8,r8,r9
  r8 = r9 - r8;
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // addi r9,r8,-1
  r9 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // mr r9,r10
  r9 = r10;
loc_825DA61C:
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x825da61c
  if (!cr6.eq) goto loc_825DA61C;
  // subf r9,r9,r10
  r9 = r10 - r9;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // addi r10,r9,-1
  r10 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // blr
  return;
}

// sub_825DAD90: lifted from generated/ by lift.py.
void f825DAD90(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DAD90 (lifted)");
  // blr
  return;
}

namespace {

lifted::Result e82090840(uint8_t* base, const lifted::Args& a) {
  return {f82090840(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82090890(uint8_t* base, const lifted::Args& a) {
  return {f82090890(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820951D8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820951D8(base, a.r1, a.r[0])};
}

lifted::Result e821128A0(uint8_t* base, const lifted::Args& a) {
  f821128A0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8214B2C8(uint8_t* base, const lifted::Args& a) {
  return {f8214B2C8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e822ADA28(uint8_t* base, const lifted::Args& a) {
  return {f822ADA28(base, a.r[0]), a.f[0]};
}

lifted::Result e822B43C0(uint8_t* base, const lifted::Args& a) {
  return {f822B43C0(base, a.r[0]), a.f[0]};
}

lifted::Result e822B6070(uint8_t* base, const lifted::Args& a) {
  return f822B6070(base, a.r1, a.r13, a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82578470(uint8_t* base, const lifted::Args& a) {
  return {f82578470(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257D6F8(uint8_t* base, const lifted::Args& a) {
  f8257D6F8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825BFCC8(uint8_t* base, const lifted::Args& a) {
  return {f825BFCC8(base, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825C09A8(uint8_t* base, const lifted::Args& a) {
  return f825C09A8(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e825CD358(uint8_t* base, const lifted::Args& a) {
  f825CD358(base, a.r[0], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825CFF70(uint8_t* base, const lifted::Args& a) {
  f825CFF70(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D36F0(uint8_t* base, const lifted::Args& a) {
  return {f825D36F0(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825D94C8(uint8_t* base, const lifted::Args& a) {
  f825D94C8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D99A0(uint8_t* base, const lifted::Args& a) {
  f825D99A0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9C40(uint8_t* base, const lifted::Args& a) {
  return {f825D9C40(base, a.r1, a.r13, a.lr), a.f[0]};
}

lifted::Result e825DA0A0(uint8_t* base, const lifted::Args& a) {
  return {f825DA0A0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DA5D0(uint8_t* base, const lifted::Args& a) {
  f825DA5D0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAD90(uint8_t* base, const lifted::Args& a) {
  f825DAD90(base);
  return {a.r[0], a.f[0]};
}

const Registration kEntries[] = {
    {0x82090840, e82090840},
    {0x82090890, e82090890},
    {0x820951D8, e820951D8},
    {0x821128A0, e821128A0},
    {0x8214B2C8, e8214B2C8},
    {0x822ADA28, e822ADA28},
    {0x822B43C0, e822B43C0},
    {0x822B6070, e822B6070},
    {0x82578470, e82578470},
    {0x8257D6F8, e8257D6F8},
    {0x825BFCC8, e825BFCC8},
    {0x825C09A8, e825C09A8},
    {0x825CD358, e825CD358},
    {0x825CFF70, e825CFF70},
    {0x825D36F0, e825D36F0},
    {0x825D94C8, e825D94C8},
    {0x825D99A0, e825D99A0},
    {0x825D9C40, e825D9C40},
    {0x825DA0A0, e825DA0A0},
    {0x825DA5D0, e825DA5D0},
    {0x825DAD90, e825DAD90},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_000)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_82090840) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_82090840(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82090840);
}

extern "C" REX_FUNC(sub_82090890) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_82090890(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82090890);
}

extern "C" REX_FUNC(sub_820951D8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_820951D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820951D8);
}

extern "C" REX_FUNC(sub_821128A0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_821128A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821128A0);
}

extern "C" REX_FUNC(sub_8214B2C8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_8214B2C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8214B2C8);
}

extern "C" REX_FUNC(sub_822ADA28) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_822ADA28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e822ADA28);
}

extern "C" REX_FUNC(sub_822B43C0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_822B43C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e822B43C0);
}

extern "C" REX_FUNC(sub_822B6070) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_822B6070(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e822B6070);
}

extern "C" REX_FUNC(sub_82578470) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_82578470(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578470);
}

extern "C" REX_FUNC(sub_8257D6F8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_8257D6F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D6F8);
}

extern "C" REX_FUNC(sub_825BFCC8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825BFCC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825BFCC8);
}

extern "C" REX_FUNC(sub_825C09A8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825C09A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825C09A8);
}

extern "C" REX_FUNC(sub_825CD358) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825CD358(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825CD358);
}

extern "C" REX_FUNC(sub_825CFF70) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825CFF70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825CFF70);
}

extern "C" REX_FUNC(sub_825D36F0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825D36F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D36F0);
}

extern "C" REX_FUNC(sub_825D94C8) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825D94C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D94C8);
}

extern "C" REX_FUNC(sub_825D99A0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825D99A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D99A0);
}

extern "C" REX_FUNC(sub_825D9C40) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825D9C40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9C40);
}

extern "C" REX_FUNC(sub_825DA0A0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825DA0A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA0A0);
}

extern "C" REX_FUNC(sub_825DA5D0) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825DA5D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DA5D0);
}

extern "C" REX_FUNC(sub_825DAD90) {
  if (!ITC_FLAG(native_lifted_000)) {
    __imp__sub_825DAD90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DAD90);
}
