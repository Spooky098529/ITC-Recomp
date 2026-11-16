// Lift sweep batch 005 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_005, true, "ITC",
              "Run lift sweep batch 005's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_820F0E00: lifted from generated/ by lift.py.
lifted::Result f820F0E00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820F0E00 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x820f0db8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F0DB8, LIFTED_ARGS(0x820F0E20, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820f0e30
  if (cr0.eq) goto loc_820F0E30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820F0E30, r31)); r3 = o.r3; f1 = o.f1; }
loc_820F0E30:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820F1098: lifted from generated/ by lift.py.
lifted::Result f820F1098(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820F1098 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lwz r30,21780(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 21780);
  // bl 0x820fb998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FB998, LIFTED_ARGS(0x820F10BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,32(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 32);
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // lwz r4,184(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 184);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // addi r5,r11,-17984
  r5 = uint64_t(int64_t(int64_t(r11) + -17984));
  // lwz r3,0(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r4,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r4));
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x820F10E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820F1100: lifted from generated/ by lift.py.
uint64_t f820F1100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820F1100 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x820fb998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FB998, LIFTED_ARGS(0x820F1118, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820fb9a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FB9A8, LIFTED_ARGS(0x820F1120, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_820F77A0: lifted from generated/ by lift.py.
lifted::Result f820F77A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820F77A0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x820F77C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // mr r4,r3
  r4 = r3;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295398, LIFTED_ARGS(0x820F77DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lis r11,-32241
  r11 = uint64_t(int64_t(-2112946176));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // addi r5,r11,27056
  r5 = uint64_t(int64_t(int64_t(r11) + 27056));
  // stw r4,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x820F7800, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820F7D70: lifted from generated/ by lift.py.
uint64_t f820F7D70(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820F7D70 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // extsh r10,r5
  r10 = uint64_t(int64_t(int16_t(r5)));
  // lfs f13,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f12,48(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 48);
  f12 = double(temp.f32);
  // lfs f0,8152(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8152);
  f0 = double(temp.f32);
  // fsubs f12,f12,f0
  f12 = double(float(f12 - f0));
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bge cr6,0x820f7dac
  if (!cr6.lt) goto loc_820F7DAC;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x820f7dc0
  goto loc_820F7DC0;
loc_820F7DAC:
  // lfs f12,56(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 56);
  f12 = double(temp.f32);
  // fadds f12,f12,f0
  f12 = double(float(f12 + f0));
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // ble cr6,0x820f7dc0
  if (!cr6.gt) goto loc_820F7DC0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_820F7DC0:
  // lfs f13,44(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 44);
  f13 = double(temp.f32);
  // lfs f12,40(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 40);
  f12 = double(temp.f32);
  // fsubs f10,f13,f12
  f10 = double(float(f13 - f12));
  // lfs f11,4(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f11 = double(temp.f32);
  // fsubs f10,f10,f0
  f10 = double(float(f10 - f0));
  // fcmpu cr6,f11,f10
  cr6 = lifted::CompareFloat(f11, f10);
  // bge cr6,0x820f7de4
  if (!cr6.lt) goto loc_820F7DE4;
  // ori r3,r3,4
  r3 = r3 | 4;
  // b 0x820f7df8
  goto loc_820F7DF8;
loc_820F7DE4:
  // fadds f13,f12,f13
  lifted::ScalarFloatMode();
  f13 = double(float(f12 + f13));
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // ble cr6,0x820f7df8
  if (!cr6.gt) goto loc_820F7DF8;
  // ori r3,r3,8
  r3 = r3 | 8;
loc_820F7DF8:
  // lfs f12,52(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 52);
  f12 = double(temp.f32);
  // fsubs f12,f12,f0
  f12 = double(float(f12 - f0));
  // lfs f13,8(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bge cr6,0x820f7e14
  if (!cr6.lt) goto loc_820F7E14;
  // ori r3,r3,16
  r3 = r3 | 16;
  // blr
  return r3;
loc_820F7E14:
  // lfs f12,60(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 60);
  f12 = double(temp.f32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blelr cr6
  if (!cr6.gt) return r3;
  // ori r3,r3,32
  r3 = r3 | 32;
  // blr
  return r3;
}

// sub_820F7EE8: lifted from generated/ by lift.py.
uint64_t f820F7EE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820F7EE8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r23,-1
  r23 = uint64_t(int64_t(-1));
  // addi r25,r11,23300
  r25 = uint64_t(int64_t(int64_t(r11) + 23300));
  // mr r22,r3
  r22 = r3;
  // mr r24,r4
  r24 = r4;
  // mr r26,r5
  r26 = r5;
  // li r20,0
  r20 = uint64_t(int64_t(0));
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // mr r10,r25
  r10 = r25;
  // mr r9,r23
  r9 = r23;
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // mtctr r11
  ctr = r11;
loc_820F7F24:
  // sth r9,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r9));
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // bdnz 0x820f7f24
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820F7F24;
  // lha r11,0(r22)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r22) + 0))));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x820f7fe4
  if (cr6.eq) goto loc_820F7FE4;
  // lwz r10,4(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 4);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // mr r28,r25
  r28 = r25;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // addi r30,r11,6
  r30 = uint64_t(int64_t(int64_t(r11) + 6));
loc_820F7F58:
  // lhz r11,-2(r30)
  r11 = REX_LOAD_U16(uint32_t(r30) + -2);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x820f7fb4
  if (cr6.eq) goto loc_820F7FB4;
  // lha r31,0(r30)
  r31 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r30) + 0))));
  // mr r4,r24
  r4 = r24;
  // mr r3,r26
  r3 = r26;
  // mr r29,r31
  r29 = r31;
  // mr r5,r29
  r5 = r29;
  // bl 0x820f7d70
  // (left out: dead in C++)
  r3 = f820F7D70(base, r3, r4, r5);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820f7fb4
  if (!cr0.eq) goto loc_820F7FB4;
  // lwz r11,4(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 4);
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r24
  r5 = r24;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x820f6be8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F6BE8, LIFTED_ARGS(0x820F7FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x820f7fc8
  if (!cr6.eq) goto loc_820F7FC8;
  // sth r29,0(r28)
  REX_STORE_U16(uint32_t(r28) + 0, uint16_t(r29));
  // addi r21,r21,1
  r21 = uint64_t(int64_t(int64_t(r21) + 1));
  // addi r28,r28,2
  r28 = uint64_t(int64_t(int64_t(r28) + 2));
loc_820F7FB4:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r30,r30,6
  r30 = uint64_t(int64_t(int64_t(r30) + 6));
  // cmpwi cr6,r27,6
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 6, so);
  // blt cr6,0x820f7f58
  if (cr6.lt) goto loc_820F7F58;
  // b 0x820f7fd4
  goto loc_820F7FD4;
loc_820F7FC8:
  // li r20,1
  r20 = uint64_t(int64_t(1));
  // sth r3,2(r22)
  REX_STORE_U16(uint32_t(r22) + 2, uint16_t(r3));
  // sth r31,0(r22)
  REX_STORE_U16(uint32_t(r22) + 0, uint16_t(r31));
loc_820F7FD4:
  // rlwinm r11,r21,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r21) | (r21 << 32), 1) & 0xFFFFFFFE;
  // cmpwi cr6,r20,0
  cr6 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // sthx r23,r11,r25
  REX_STORE_U16(uint32_t(r11) + uint32_t(r25), uint16_t(r23));
  // bne cr6,0x820f8080
  if (!cr6.eq) goto loc_820F8080;
loc_820F7FE4:
  // lwz r30,4(r26)
  r30 = REX_LOAD_U32(uint32_t(r26) + 4);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x820f8080
  if (!cr6.gt) goto loc_820F8080;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_820F7FFC:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r21,0
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // ble cr6,0x820f8028
  if (!cr6.gt) goto loc_820F8028;
  // mr r11,r25
  r11 = r25;
loc_820F800C:
  // lha r9,0(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 0))));
  // cmpw cr6,r9,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r31), so);
  // beq cr6,0x820f805c
  if (cr6.eq) goto loc_820F805C;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmpw cr6,r10,r21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r21), so);
  // blt cr6,0x820f800c
  if (cr6.lt) goto loc_820F800C;
loc_820F8028:
  // extsh r5,r31
  r5 = uint64_t(int64_t(int16_t(r31)));
  // mr r4,r24
  r4 = r24;
  // mr r3,r26
  r3 = r26;
  // bl 0x820f7d70
  // (left out: dead in C++)
  r3 = f820F7D70(base, r3, r4, r5);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820f805c
  if (!cr0.eq) goto loc_820F805C;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r5,r24
  r5 = r24;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwzx r3,r11,r29
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r29));
  // bl 0x820f6be8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F6BE8, LIFTED_ARGS(0x820F8054, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x820f8074
  if (!cr6.eq) goto loc_820F8074;
loc_820F805C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x820f7ffc
  if (cr6.lt) goto loc_820F7FFC;
  // b 0x820f8080
  goto loc_820F8080;
loc_820F8074:
  // li r20,1
  r20 = uint64_t(int64_t(1));
  // sth r3,2(r22)
  REX_STORE_U16(uint32_t(r22) + 2, uint16_t(r3));
  // sth r31,0(r22)
  REX_STORE_U16(uint32_t(r22) + 0, uint16_t(r31));
loc_820F8080:
  // mr r3,r20
  r3 = r20;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return r3;
}

// sub_820F9DD0: lifted from generated/ by lift.py.
lifted::Result f820F9DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_820F9DD0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r3,r5
  r3 = r5;
  // mr r30,r4
  r30 = r4;
  // bl 0x820ab498
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AB498, LIFTED_ARGS(0x820F9DF4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lha r6,82(r1)
  r6 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 82))));
  // lha r5,80(r1)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 80))));
  // bl 0x820f9768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F9768, LIFTED_ARGS(0x820F9E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820F9E28: lifted from generated/ by lift.py.
lifted::Result f820F9E28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820F9E28 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x820F9E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // mr r4,r3
  r4 = r3;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295398, LIFTED_ARGS(0x820F9E64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // addi r5,r11,-28592
  r5 = uint64_t(int64_t(int64_t(r11) + -28592));
  // stw r4,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x820F9E88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820FA8F8: lifted from generated/ by lift.py.
lifted::Result f820FA8F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_820FA8F8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-40(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f31,f1
  f31 = f1;
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // addi r11,r11,23312
  r11 = uint64_t(int64_t(int64_t(r11) + 23312));
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addi r31,r11,-1
  r31 = uint64_t(int64_t(int64_t(r11) + -1));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8128(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8128);
  f0 = double(temp.f32);
  // b 0x820fa94c
  goto loc_820FA94C;
loc_820FA944:
  // fsubs f31,f31,f0
  lifted::ScalarFloatMode();
  f31 = double(float(f31 - f0));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
loc_820FA94C:
  // fcmpu cr6,f31,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x820fa944
  if (cr6.gt) goto loc_820FA944;
  // mr r5,r31
  r5 = r31;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820f9dd0
  // (left out: dead in C++)
  { const lifted::Result o = f820F9DD0(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // addi r5,r31,-1
  r5 = uint64_t(int64_t(int64_t(r31) + -1));
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820f9dd0
  // (left out: dead in C++)
  { const lifted::Result o = f820F9DD0(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x820f8fa0
  // (left out: dead in C++)
  f1 = f820F8FA0(base, r1, r3, r4, r5);
  // lfs f0,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f13,100(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f12,104(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f12 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f11,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f11 = double(temp.f32);
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // lfs f0,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f0 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,4(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 4, temp.u32);
  // lfs f0,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // stfs f0,8(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 8, temp.u32);
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820FB9C0: lifted from generated/ by lift.py.
void f820FB9C0(uint8_t* base, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_820FB9C0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r10,32(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 32);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // rlwimi r10,r11,3,30,30
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0x2) | (r10 & 0xFFFFFFFFFFFFFFFD);
  // rlwimi r10,r11,3,28,28
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0x8) | (r10 & 0xFFFFFFFFFFFFFFF7);
  // stw r10,32(r6)
  REX_STORE_U32(uint32_t(r6) + 32, uint32_t(r10));
  // blr
  return;
}

// sub_820FBB68: lifted from generated/ by lift.py.
lifted::Result f820FBB68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820FBB68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x820FBB8C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // addi r10,r3,63
  r10 = uint64_t(int64_t(int64_t(r3) + 63));
  // lwz r9,0(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r11,-17984
  r5 = uint64_t(int64_t(int64_t(r11) + -17984));
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // rlwinm r10,r10,0,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFC0;
  // add r4,r9,r11
  r4 = r9 + r11;
  // add r11,r11,r10
  r11 = r11 + r10;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // rlwinm r11,r11,0,30,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFC3;
  // stw r11,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r11));
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x820FBBD0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820FF368: lifted from generated/ by lift.py.
lifted::Result f820FF368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820FF368 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820FF37C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820ff390
  if (cr0.eq) goto loc_820FF390;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x820ff394
  goto loc_820FF394;
loc_820FF390:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820FF394:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,23336
  r31 = uint64_t(int64_t(int64_t(r11) + 23336));
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // bl 0x820ff250
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF250, LIFTED_ARGS(0x820FF3A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x820ff3e0
  if (cr6.eq) goto loc_820FF3E0;
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x820ff3e0
  if (cr6.eq) goto loc_820FF3E0;
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x820ff3e0
  if (cr6.eq) goto loc_820FF3E0;
  // lwz r4,32(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820ff3e8
  goto loc_820FF3E8;
loc_820FF3E0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820FF3E8:
  // bl 0x8228f090
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8228F090, LIFTED_ARGS(0x820FF3EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,23324
  r31 = uint64_t(int64_t(int64_t(r11) + 23324));
  // mr r3,r31
  r3 = r31;
  // bl 0x820f6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F6940, LIFTED_ARGS(0x820FF400, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,23312
  r30 = uint64_t(int64_t(int64_t(r11) + 23312));
  // mr r3,r30
  r3 = r30;
  // bl 0x820f8618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8618, LIFTED_ARGS(0x820FF410, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x820f68e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F68E8, LIFTED_ARGS(0x820FF418, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820f8600
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8600, LIFTED_ARGS(0x820FF420, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // rlwinm r10,r29,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 3) & 0xFFFFFFF8;
  // addi r11,r11,11936
  r11 = uint64_t(int64_t(int64_t(r11) + 11936));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwzx r31,r10,r11
  r31 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x820f77a0
  // (left out: dead in C++)
  { const lifted::Result o = f820F77A0(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x820f9e28
  // (left out: dead in C++)
  { const lifted::Result o = f820F9E28(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820FF4D0: lifted from generated/ by lift.py.
uint64_t f820FF4D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820FF4D0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r28,r3
  r28 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,60(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 60);
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820ff540
  if (cr0.eq) goto loc_820FF540;
  // mr r30,r29
  r30 = r29;
loc_820FF500:
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = f820FF790(base, r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x820ff458
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF458, LIFTED_ARGS(0x820FF528, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 60);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x820ff500
  if (cr6.lt) goto loc_820FF500;
loc_820FF540:
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = f820FF790(base, r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // mr r3,r28
  r3 = r28;
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_820FF5A0: lifted from generated/ by lift.py.
void f820FF5A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820FF5A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x820FF5C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r10,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r10));
  // stw r10,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r10));
  // stw r10,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r10));
  // stw r10,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r10));
loc_820FF5D8:
  // lwz r10,60(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lis r9,10
  r9 = uint64_t(int64_t(655360));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r9,r10,r31
  REX_STORE_U32(uint32_t(r10) + uint32_t(r31), uint32_t(r9));
  // lwz r10,60(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 60);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r10));
  // bne 0x820ff5d8
  if (!cr0.eq) goto loc_820FF5D8;
  // lis r11,-32759
  r11 = uint64_t(int64_t(-2146893824));
  // ori r10,r11,61415
  r10 = r11 | 61415;
  // li r11,99
  r11 = uint64_t(int64_t(99));
  // stw r10,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r10));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return;
}

// sub_820FF628: lifted from generated/ by lift.py.
uint64_t f820FF628(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820FF628 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // bl 0x820ff4d0
  // (left out: dead in C++)
  r3 = f820FF4D0(base, r1, r13, r3, r4);
  // addi r4,r30,64
  r4 = uint64_t(int64_t(int64_t(r30) + 64));
  // bl 0x820ff458
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF458, LIFTED_ARGS(0x820FF654, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820ff790
  // (left out: dead in C++)
  r3 = f820FF790(base, r1, r13, r3);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_820FF680: lifted from generated/ by lift.py.
uint64_t f820FF680(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820FF680 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rlwinm r9,r11,0,0,0
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  // rlwinm r8,r11,16,17,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0x7FFF;
  // subfic r9,r9,0
  ca = uint32_t(r9) <= 0;
  r9 = static_cast<uint64_t>(0) - r9;
  // rlwinm r5,r10,16,17,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0x7FFF;
  // rlwinm r4,r10,0,0,0
  r4 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000;
  // subfe r3,r9,r9
  temp.u8 = (~uint32_t(r9) + uint32_t(r9) < ~uint32_t(r9)) | (~uint32_t(r9) + uint32_t(r9) + ca < ca);
  r3 = ~r9 + r9 + ca;
  ca = temp.u8;
  // mulli r7,r8,60
  r7 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(60))));
  // mulli r9,r5,60
  r9 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(60))));
  // rlwinm r6,r11,22,26,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x3F;
  // rlwinm r8,r10,22,26,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 22) & 0x3F;
  // subfic r5,r4,0
  ca = uint32_t(r4) <= 0;
  r5 = static_cast<uint64_t>(0) - r4;
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r7,r7,r6
  r7 = r7 + r6;
  // subfe r5,r5,r5
  temp.u8 = (~uint32_t(r5) + uint32_t(r5) < ~uint32_t(r5)) | (~uint32_t(r5) + uint32_t(r5) + ca < ca);
  r5 = ~r5 + r5 + ca;
  ca = temp.u8;
  // clrlwi r6,r11,22
  r6 = uint32_t(r11) & 0x3FF;
  // mulli r9,r9,1000
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(1000))));
  // mulli r7,r7,1000
  r7 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(1000))));
  // rlwinm r8,r3,0,0,30
  r8 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0xFFFFFFFE;
  // clrlwi r10,r10,22
  r10 = uint32_t(r10) & 0x3FF;
  // rlwinm r11,r5,0,0,30
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 0) & 0xFFFFFFFE;
  // add r10,r9,r10
  r10 = r9 + r10;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r7,r7,r6
  r7 = r7 + r6;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // mullw r9,r7,r8
  r9 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r8))));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bgt cr6,0x820ff700
  if (cr6.gt) goto loc_820FF700;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_820FF700:
  // clrlwi r3,r11,24
  r3 = uint32_t(r11) & 0xFF;
  // blr
  return r3;
}

// sub_820FF790: lifted from generated/ by lift.py.
uint64_t f820FF790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820FF790 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,26916
  r31 = uint64_t(int64_t(int64_t(r11) + 26916));
  // addi r3,r1,132
  r3 = uint64_t(int64_t(int64_t(r1) + 132));
  // mr r4,r31
  r4 = r31;
  // stw r30,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r30));
  // bl 0x820ff708
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF708, LIFTED_ARGS(0x820FF7C0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820ff7d0
  if (cr0.eq) goto loc_820FF7D0;
loc_820FF7C8:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // b 0x820ff7f0
  goto loc_820FF7F0;
loc_820FF7D0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,132
  r3 = uint64_t(int64_t(int64_t(r1) + 132));
  // addi r31,r11,26920
  r31 = uint64_t(int64_t(int64_t(r11) + 26920));
  // mr r4,r31
  r4 = r31;
  // bl 0x820ff680
  // (left out: dead in C++)
  r3 = f820FF680(base, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820ff7c8
  if (!cr0.eq) goto loc_820FF7C8;
  // mr r3,r30
  r3 = r30;
loc_820FF7F0:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_820FF908: lifted from generated/ by lift.py.
lifted::Result f820FF908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820FF908 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23340(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // b 0x825a71c0
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820FF958: lifted from generated/ by lift.py.
uint64_t f820FF958(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820FF958 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // blr
  return r3;
}

// sub_820FFA88: lifted from generated/ by lift.py.
void f820FFA88(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820FFA88 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpwi cr6,r5,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), -1, so);
  // bne cr6,0x820ffa9c
  if (!cr6.eq) goto loc_820FFA9C;
  // lwz r11,800(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 800);
  // mulli r11,r11,80
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(80))));
  // b 0x820ffaa0
  goto loc_820FFAA0;
loc_820FFA9C:
  // mulli r11,r5,80
  r11 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(80))));
loc_820FFAA0:
  // add r11,r11,r3
  r11 = r11 + r3;
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // blr
  return;
}

// sub_820FFB60: lifted from generated/ by lift.py.
uint64_t f820FFB60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820FFB60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,212(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 212);
  // mr r31,r3
  r31 = r3;
  // std r4,128(r1)
  REX_STORE_U64(uint32_t(r1) + 128, r4);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // std r5,136(r1)
  REX_STORE_U64(uint32_t(r1) + 136, r5);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // std r6,144(r1)
  REX_STORE_U64(uint32_t(r1) + 144, r6);
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // std r7,152(r1)
  REX_STORE_U64(uint32_t(r1) + 152, r7);
  // std r8,160(r1)
  REX_STORE_U64(uint32_t(r1) + 160, r8);
  // std r9,168(r1)
  REX_STORE_U64(uint32_t(r1) + 168, r9);
  // std r10,176(r1)
  REX_STORE_U64(uint32_t(r1) + 176, r10);
  // bne cr6,0x820ffbc4
  if (!cr6.eq) goto loc_820FFBC4;
  // lwz r11,800(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 800);
  // mulli r11,r11,80
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(80))));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820FFBB4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,800(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 800);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,800(r31)
  REX_STORE_U32(uint32_t(r31) + 800, uint32_t(r11));
  // b 0x820ffbd0
  goto loc_820FFBD0;
loc_820FFBC4:
  // mulli r11,r11,80
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(80))));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x820FFBD0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820FFBD0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82101940: lifted from generated/ by lift.py.
uint64_t f82101940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82101940 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r11,r3
  r11 = r3;
  // lwz r10,800(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 800);
  // addi r9,r10,-1
  r9 = uint64_t(int64_t(int64_t(r10) + -1));
  // lwz r10,804(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 804);
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bge cr6,0x82101974
  if (!cr6.lt) goto loc_82101974;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r10,804(r11)
  REX_STORE_U32(uint32_t(r11) + 804, uint32_t(r10));
  // b 0x82101980
  goto loc_82101980;
loc_82101974:
  // mr r3,r11
  r3 = r11;
  // bl 0x82101860
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101860, LIFTED_ARGS(0x8210197C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82101980:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821022B0: lifted from generated/ by lift.py.
uint64_t f821022B0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821022B0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r31,7
  r31 = uint64_t(int64_t(7));
  // addi r30,r11,23344
  r30 = uint64_t(int64_t(int64_t(r11) + 23344));
loc_821022D0:
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x820c96e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C96E8, LIFTED_ARGS(0x821022D8, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bne 0x821022d0
  if (!cr0.eq) goto loc_821022D0;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82102A28: lifted from generated/ by lift.py.
uint64_t f82102A28(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82102A28 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,16
  r11 = uint64_t(int64_t(1048576));
  // lis r6,-32148
  r6 = uint64_t(int64_t(-2106851328));
  // lis r10,16
  r10 = uint64_t(int64_t(1048576));
  // ori r11,r11,31428
  r11 = r11 | 31428;
  // ori r7,r10,31424
  r7 = r10 | 31424;
  // mullw r11,r4,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r11))));
  // lwz r10,23372(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 23372);
  // add r9,r11,r10
  r9 = r11 + r10;
  // lwzx r9,r9,r7
  r9 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r7));
  // cmplwi cr6,r9,54000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 54000, so);
  // bge cr6,0x82102a9c
  if (!cr6.lt) goto loc_82102A9C;
  // mulli r9,r9,20
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(20))));
  // add r8,r9,r11
  r8 = r9 + r11;
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // add r10,r8,r10
  r10 = r8 + r10;
  // mtctr r9
  ctr = r9;
loc_82102A68:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r9,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r9));
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // bdnz 0x82102a68
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82102A68;
  // lwz r10,23372(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 23372);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r11,r11,r7
  r11 = r11 + r7;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_82102A9C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82104630: lifted from generated/ by lift.py.
lifted::Result f82104630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82104630 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addis r11,r3,1
  r11 = uint64_t(int64_t(int64_t(r3) + 65536));
  // li r30,6
  r30 = uint64_t(int64_t(6));
  // addi r11,r11,-26608
  r11 = uint64_t(int64_t(int64_t(r11) + -26608));
  // addi r31,r11,224
  r31 = uint64_t(int64_t(int64_t(r11) + 224));
loc_82104654:
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // addi r31,r31,-5232
  r31 = uint64_t(int64_t(int64_t(r31) + -5232));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // addi r6,r11,25920
  r6 = uint64_t(int64_t(int64_t(r11) + 25920));
  // li r4,96
  r4 = uint64_t(int64_t(96));
  // mr r3,r31
  r3 = r31;
  // bl 0x820900c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820900C0, LIFTED_ARGS(0x82104670, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x82104654
  if (!cr0.lt) goto loc_82104654;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82104CE0: lifted from generated/ by lift.py.
lifted::Result f82104CE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82104CE0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // addis r3,r31,34
  r3 = uint64_t(int64_t(int64_t(r31) + 2228224));
  // addi r3,r3,-29280
  r3 = uint64_t(int64_t(int64_t(r3) + -29280));
  // bl 0x82104630
  // (left out: dead in C++)
  { const lifted::Result o = f82104630(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,33
  r3 = uint64_t(int64_t(int64_t(r31) + 2162688));
  // addi r3,r3,-2672
  r3 = uint64_t(int64_t(int64_t(r3) + -2672));
  // bl 0x82104630
  // (left out: dead in C++)
  { const lifted::Result o = f82104630(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82104d24
  if (cr0.eq) goto loc_82104D24;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82104D24, r31)); r3 = o.r3; f1 = o.f1; }
loc_82104D24:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82104F00: lifted from generated/ by lift.py.
lifted::Result f82104F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82104F00 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23372(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23372);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x82104ce0
  { const lifted::Result o = f82104CE0(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82104F20: lifted from generated/ by lift.py.
lifted::Result f82104F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82104F20 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r31,23376(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 23376);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82104f54
  if (cr6.eq) goto loc_82104F54;
  // addis r3,r31,3
  r3 = uint64_t(int64_t(int64_t(r31) + 196608));
  // addi r3,r3,-18928
  r3 = uint64_t(int64_t(int64_t(r3) + -18928));
  // bl 0x82104630
  // (left out: dead in C++)
  { const lifted::Result o = f82104630(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82104F54, r31)); r3 = o.r3; f1 = o.f1; }
loc_82104F54:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82105228: lifted from generated/ by lift.py.
lifted::Result f82105228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82105228 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-8048(r1)
  ea = -8048 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // mr r31,r3
  r31 = r3;
  // ori r11,r11,46592
  r11 = r11 | 46592;
  // mr r29,r4
  r29 = r4;
  // mr r23,r5
  r23 = r5;
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82105260
  if (cr6.eq) goto loc_82105260;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821054a0
  goto loc_821054A0;
loc_82105260:
  // addis r25,r29,6
  r25 = uint64_t(int64_t(int64_t(r29) + 393216));
  // addis r24,r29,6
  r24 = uint64_t(int64_t(int64_t(r29) + 393216));
  // addi r27,r29,48
  r27 = uint64_t(int64_t(int64_t(r29) + 48));
  // addi r25,r25,-27104
  r25 = uint64_t(int64_t(int64_t(r25) + -27104));
  // addi r24,r24,-25880
  r24 = uint64_t(int64_t(int64_t(r24) + -25880));
  // bl 0x821021a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821021A8, LIFTED_ARGS(0x82105278, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r25
  r3 = r25;
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101900, LIFTED_ARGS(0x82105280, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r10,r31,3
  r10 = uint64_t(int64_t(int64_t(r31) + 196608));
  // addis r9,r29,10
  r9 = uint64_t(int64_t(int64_t(r29) + 655360));
  // addi r10,r10,-18964
  r10 = uint64_t(int64_t(int64_t(r10) + -18964));
  // addi r9,r9,15076
  r9 = uint64_t(int64_t(int64_t(r9) + 15076));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // mtctr r11
  ctr = r11;
loc_82105298:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82105298
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82105298;
  // addi r4,r31,52
  r4 = uint64_t(int64_t(int64_t(r31) + 52));
  // addi r3,r29,4
  r3 = uint64_t(int64_t(int64_t(r29) + 4));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x821052BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // lis r10,10
  r10 = uint64_t(int64_t(655360));
  // ori r11,r11,46568
  r11 = r11 | 46568;
  // ori r10,r10,15072
  r10 = r10 | 15072;
  // addi r28,r27,3908
  r28 = uint64_t(int64_t(int64_t(r27) + 3908));
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
  // li r26,93
  r26 = uint64_t(int64_t(93));
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // stwx r11,r29,r10
  REX_STORE_U32(uint32_t(r29) + uint32_t(r10), uint32_t(r11));
loc_821052E0:
  // addi r4,r30,8
  r4 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x821052F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r11,4016(r1)
  REX_STORE_U32(uint32_t(r1) + 4016, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // stw r11,4020(r1)
  REX_STORE_U32(uint32_t(r1) + 4020, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS(0x82105308, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r28,-3908
  r3 = uint64_t(int64_t(int64_t(r28) + -3908));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r5,3904
  r5 = uint64_t(int64_t(3904));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82105318, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1892
  r11 = uint64_t(int64_t(int64_t(r30) + 1892));
  // addi r10,r28,-4
  r10 = uint64_t(int64_t(int64_t(r28) + -4));
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // lhz r9,0(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 0);
  // sth r9,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r9));
  // lhz r11,2(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 2);
  // sth r11,2(r10)
  REX_STORE_U16(uint32_t(r10) + 2, uint16_t(r11));
  // lfs f0,1896(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 1896);
  f0 = double(temp.f32);
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lwz r11,1900(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 1900);
  // stw r11,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r11));
  // lfs f0,1904(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 1904);
  f0 = double(temp.f32);
  // stfs f0,12(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 12, temp.u32);
  // addi r30,r30,1908
  r30 = uint64_t(int64_t(int64_t(r30) + 1908));
  // addi r28,r28,3936
  r28 = uint64_t(int64_t(int64_t(r28) + 3936));
  // bne 0x821052e0
  if (!cr0.eq) goto loc_821052E0;
  // lis r10,5
  r10 = uint64_t(int64_t(327680));
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // lis r9,5
  r9 = uint64_t(int64_t(327680));
  // ori r10,r10,38368
  r10 = r10 | 38368;
  // ori r9,r9,38376
  r9 = r9 | 38376;
  // lis r8,5
  r8 = uint64_t(int64_t(327680));
  // lis r7,5
  r7 = uint64_t(int64_t(327680));
  // ori r8,r8,38380
  r8 = r8 | 38380;
  // stwx r11,r27,r10
  REX_STORE_U32(uint32_t(r27) + uint32_t(r10), uint32_t(r11));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // lwz r10,92(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 92);
  // addis r30,r31,3
  r30 = uint64_t(int64_t(int64_t(r31) + 196608));
  // ori r7,r7,38372
  r7 = r7 | 38372;
  // addi r30,r30,-19064
  r30 = uint64_t(int64_t(int64_t(r30) + -19064));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // stwx r10,r27,r9
  REX_STORE_U32(uint32_t(r27) + uint32_t(r9), uint32_t(r10));
  // addi r4,r30,56
  r4 = uint64_t(int64_t(int64_t(r30) + 56));
  // lwz r11,96(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 96);
  // stwx r11,r27,r8
  REX_STORE_U32(uint32_t(r27) + uint32_t(r8), uint32_t(r11));
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stwx r11,r27,r7
  REX_STORE_U32(uint32_t(r27) + uint32_t(r7), uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x821053B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r25
  r3 = r25;
  // ld r4,0(r30)
  r4 = REX_LOAD_U64(uint32_t(r30) + 0);
  // ld r5,8(r30)
  r5 = REX_LOAD_U64(uint32_t(r30) + 8);
  // ld r6,16(r30)
  r6 = REX_LOAD_U64(uint32_t(r30) + 16);
  // ld r7,24(r30)
  r7 = REX_LOAD_U64(uint32_t(r30) + 24);
  // ld r8,32(r30)
  r8 = REX_LOAD_U64(uint32_t(r30) + 32);
  // ld r9,40(r30)
  r9 = REX_LOAD_U64(uint32_t(r30) + 40);
  // ld r10,48(r30)
  r10 = REX_LOAD_U64(uint32_t(r30) + 48);
  // bl 0x820ffb60
  // (left out: dead in C++)
  r3 = f820FFB60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // addi r11,r1,4828
  r11 = uint64_t(int64_t(int64_t(r1) + 4828));
  // lfs f0,2612(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2612);
  f0 = double(temp.f32);
loc_821053EC:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // addi r11,r11,972
  r11 = uint64_t(int64_t(int64_t(r11) + 972));
  // bge 0x821053ec
  if (!cr0.lt) goto loc_821053EC;
  // addi r3,r1,4032
  r3 = uint64_t(int64_t(int64_t(r1) + 4032));
  // bl 0x82101f68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101F68, LIFTED_ARGS(0x8210540C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // lis r10,2
  r10 = uint64_t(int64_t(131072));
  // ori r11,r11,46552
  r11 = r11 | 46552;
  // ori r10,r10,46556
  r10 = r10 | 46556;
  // lis r9,2
  r9 = uint64_t(int64_t(131072));
  // lis r8,2
  r8 = uint64_t(int64_t(131072));
  // ori r9,r9,46560
  r9 = r9 | 46560;
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // ori r8,r8,46564
  r8 = r8 | 46564;
  // addi r4,r1,4032
  r4 = uint64_t(int64_t(int64_t(r1) + 4032));
  // mr r3,r24
  r3 = r24;
  // stw r11,7948(r1)
  REX_STORE_U32(uint32_t(r1) + 7948, uint32_t(r11));
  // lwzx r11,r31,r10
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r10));
  // stw r11,7952(r1)
  REX_STORE_U32(uint32_t(r1) + 7952, uint32_t(r11));
  // lwzx r11,r31,r9
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r9));
  // stw r11,7956(r1)
  REX_STORE_U32(uint32_t(r1) + 7956, uint32_t(r11));
  // lwzx r11,r31,r8
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r8));
  // stw r11,7960(r1)
  REX_STORE_U32(uint32_t(r1) + 7960, uint32_t(r11));
  // bl 0x820ffe08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE08, LIFTED_ARGS(0x82105458, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r4,r31,3
  r4 = uint64_t(int64_t(int64_t(r31) + 196608));
  // addis r3,r23,33
  r3 = uint64_t(int64_t(int64_t(r23) + 2162688));
  // addi r4,r4,-18928
  r4 = uint64_t(int64_t(int64_t(r4) + -18928));
  // addi r3,r3,-2672
  r3 = uint64_t(int64_t(int64_t(r3) + -2672));
  // bl 0x82104ea8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82104EA8, LIFTED_ARGS(0x8210546C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,16
  r5 = uint64_t(int64_t(1048576));
  // addis r4,r31,3
  r4 = uint64_t(int64_t(int64_t(r31) + 196608));
  // ori r5,r5,31428
  r5 = r5 | 31428;
  // addi r4,r4,20000
  r4 = uint64_t(int64_t(int64_t(r4) + 20000));
  // mr r3,r23
  r3 = r23;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82105484, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // stwx r9,r11,r10
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), uint32_t(r9));
loc_821054A0:
  // addi r1,r1,8048
  r1 = uint64_t(int64_t(int64_t(r1) + 8048));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82105688: lifted from generated/ by lift.py.
lifted::Result f82105688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82105688 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x82104f00
  // (left out: dead in C++)
  { const lifted::Result o = f82104F00(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82104f20
  // (left out: dead in C++)
  { const lifted::Result o = f82104F20(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x820ff908
  // (left out: dead in C++)
  { const lifted::Result o = f820FF908(base, r1, r13, r6, r7, r8, r9, r10, f1, 0x821056A0); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821055d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821055D8, LIFTED_ARGS(0x821056A8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82116fb0
  // (left out: dead in C++)
  { const lifted::Result o = f82116FB0(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82106678: lifted from generated/ by lift.py.
lifted::Result f82106678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82106678 (lifted)");
  uint64_t r3 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x82106688, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x8210668C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8214e240
  // (left out: dead in C++)
  { const lifted::Result o = f8214E240(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // bl 0x820a0008
  // (left out: dead in C++)
  f820A0008(base);
  // bl 0x82110388
  // (left out: dead in C++)
  { const lifted::Result o = f82110388(base, r1, r13, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209c158
  // (left out: dead in C++)
  { const lifted::Result o = f8209C158(base, r1, r13, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x820a4398
  // (left out: dead in C++)
  f820A4398(base);
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x821066A4, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209fbc8
  // (left out: dead in C++)
  { const lifted::Result o = f8209FBC8(base, r1, r13, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209f138
  // (left out: dead in C++)
  { const lifted::Result o = f8209F138(base, r1, r13, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209d3b0
  // (left out: dead in C++)
  { const lifted::Result o = f8209D3B0(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209bcb8
  // (left out: dead in C++)
  { const lifted::Result o = f8209BCB8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x8209f590
  // (left out: dead in C++)
  f8209F590(base);
  // bl 0x8209ac40
  // (left out: dead in C++)
  f8209AC40(base);
  // bl 0x8209ac60
  // (left out: dead in C++)
  f8209AC60(base);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821066D0: lifted from generated/ by lift.py.
lifted::Result f821066D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821066D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23436(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23436);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82106704
  if (cr6.eq) goto loc_82106704;
  // bl 0x820c9d90
  // (left out: dead in C++)
  { const lifted::Result o = f820C9D90(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82291408
  // (left out: dead in C++)
  { const lifted::Result o = f82291408(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82247b28
  // (left out: dead in C++)
  { const lifted::Result o = f82247B28(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82105688
  // (left out: dead in C++)
  { const lifted::Result o = f82105688(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x8210ae38
  // (left out: dead in C++)
  { const lifted::Result o = f8210AE38(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x82106704, r31)); r3 = o.r3; f1 = o.f1; }
loc_82106704:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82106718: lifted from generated/ by lift.py.
lifted::Result f82106718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82106718 (lifted)");
  uint64_t r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x821066d0
  // (left out: dead in C++)
  { const lifted::Result o = f821066D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x82106678
  // (left out: dead in C++)
  { const lifted::Result o = f82106678(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82106B60: lifted from generated/ by lift.py.
lifted::Result f82106B60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82106B60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x82106af0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106AF0, LIFTED_ARGS(0x82106B80, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82106b90
  if (cr0.eq) goto loc_82106B90;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82106B90, r31)); r3 = o.r3; f1 = o.f1; }
loc_82106B90:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82107518: lifted from generated/ by lift.py.
lifted::Result f82107518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82107518 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27956
  r11 = uint64_t(int64_t(int64_t(r11) + 27956));
  // addi r10,r10,27944
  r10 = uint64_t(int64_t(int64_t(r10) + 27944));
  // addis r3,r31,2
  r3 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r3,r3,26480
  r3 = uint64_t(int64_t(int64_t(r3) + 26480));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r10));
  // bl 0x8215e1d0
  // (left out: dead in C++)
  { const lifted::Result o = f8215E1D0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addis r3,r31,2
  r3 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r3,r3,16820
  r3 = uint64_t(int64_t(int64_t(r3) + 16820));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x8210755C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,10512
  r3 = uint64_t(int64_t(int64_t(r31) + 10512));
  // bl 0x82106af0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106AF0, LIFTED_ARGS(0x82107564, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,27116
  r11 = uint64_t(int64_t(int64_t(r11) + 27116));
  // addi r10,r10,27148
  r10 = uint64_t(int64_t(int64_t(r10) + 27148));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82107590: lifted from generated/ by lift.py.
lifted::Result f82107590(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82107590 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x82107518
  // (left out: dead in C++)
  { const lifted::Result o = f82107518(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821075c0
  if (cr0.eq) goto loc_821075C0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x821075C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_821075C0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82107C20: lifted from generated/ by lift.py.
uint64_t f82107C20(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82107C20 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpwi cr6,r4,2
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 2, so);
  // blt cr6,0x82107c34
  if (cr6.lt) goto loc_82107C34;
  // cmpwi cr6,r4,3
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 3, so);
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // ble cr6,0x82107c38
  if (!cr6.gt) goto loc_82107C38;
loc_82107C34:
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_82107C38:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,23464(r10)
  REX_STORE_U32(uint32_t(r10) + 23464, uint32_t(r11));
  // blr
  return r3;
}

// sub_82107C48: lifted from generated/ by lift.py.
void f82107C48(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82107C48 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,13
  r11 = uint64_t(int64_t(13));
  // stw r11,23464(r10)
  REX_STORE_U32(uint32_t(r10) + 23464, uint32_t(r11));
  // blr
  return;
}

// sub_82107D80: lifted from generated/ by lift.py.
void f82107D80(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82107D80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r3,208
  r11 = uint64_t(int64_t(int64_t(r3) + 208));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r10,r5,r6
  r10 = r6 - r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // blr
  return;
}

// sub_82108A08: lifted from generated/ by lift.py.
lifted::Result f82108A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82108A08 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r3,16560(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16560);
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x82108A28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16564(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 16564);
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x82108A30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82108A34:
  // addi r11,r31,4142
  r11 = uint64_t(int64_t(int64_t(r31) + 4142));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r30
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r30));
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x82108A44, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r31,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 4, so);
  // blt cr6,0x82108a34
  if (cr6.lt) goto loc_82108A34;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82108A70: lifted from generated/ by lift.py.
uint64_t f82108A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82108A70 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lbz r11,0(r5)
  r11 = REX_LOAD_U8(uint32_t(r5) + 0);
  // mr r28,r4
  r28 = r4;
  // mr r31,r5
  r31 = r5;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82108b14
  if (cr6.eq) goto loc_82108B14;
  // mr r30,r28
  r30 = r28;
loc_82108A98:
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // mr r10,r11
  r10 = r11;
  // cmplwi cr6,r10,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 128, so);
  // ble cr6,0x82108ad8
  if (!cr6.gt) goto loc_82108AD8;
  // cmplwi cr6,r10,159
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 159, so);
  // ble cr6,0x82108ac0
  if (!cr6.gt) goto loc_82108AC0;
  // cmplwi cr6,r10,223
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 223, so);
  // ble cr6,0x82108ad8
  if (!cr6.gt) goto loc_82108AD8;
  // cmplwi cr6,r10,254
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 254, so);
  // bgt cr6,0x82108ad8
  if (cr6.gt) goto loc_82108AD8;
loc_82108AC0:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // stb r11,81(r1)
  REX_STORE_U8(uint32_t(r1) + 81, uint8_t(r11));
  // lhz r11,80(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 80);
  // b 0x82108adc
  goto loc_82108ADC;
loc_82108AD8:
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
loc_82108ADC:
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // cmplwi cr6,r3,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 10, so);
  // bne cr6,0x82108af4
  if (!cr6.eq) goto loc_82108AF4;
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // sth r11,0(r30)
  REX_STORE_U16(uint32_t(r30) + 0, uint16_t(r11));
  // b 0x82108afc
  goto loc_82108AFC;
loc_82108AF4:
  // bl 0x8259c6c0
  // (left out: dead in C++)
  r3 = f8259C6C0(base, r1, r13, r3);
  // sth r3,0(r30)
  REX_STORE_U16(uint32_t(r30) + 0, uint16_t(r3));
loc_82108AFC:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,2
  r30 = uint64_t(int64_t(int64_t(r30) + 2));
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82108a98
  if (!cr6.eq) goto loc_82108A98;
loc_82108B14:
  // rlwinm r11,r29,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 1) & 0xFFFFFFFE;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // sthx r10,r11,r28
  REX_STORE_U16(uint32_t(r11) + uint32_t(r28), uint16_t(r10));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82109160: lifted from generated/ by lift.py.
lifted::Result f82109160(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82109160 (lifted)");
  uint64_t r3 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r31,r6
  r31 = r6;
  // bl 0x82294730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294730, LIFTED_ARGS(0x8210918C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,24
  r3 = uint64_t(int64_t(24));
  // addi r10,r11,4
  r10 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x821091A4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821091b8
  if (cr0.eq) goto loc_821091B8;
  // bl 0x82596e68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82596E68, LIFTED_ARGS(0x821091B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x821091bc
  goto loc_821091BC;
loc_821091B8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821091BC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x82294730
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294730, LIFTED_ARGS(0x821091D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // bl 0x82596f68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82596F68, LIFTED_ARGS(0x821091DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // bl 0x82597118
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82597118, LIFTED_ARGS(0x821091E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r3,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r3));
  // bl 0x822a5b78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A5B78, LIFTED_ARGS(0x821091F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 104);
  // lwz r10,108(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 108);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // stw r9,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r9));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82109400: lifted from generated/ by lift.py.
lifted::Result f82109400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82109400 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82109414:
  // addi r11,r30,5
  r11 = uint64_t(int64_t(int64_t(r30) + 5));
  // rlwinm r29,r11,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r29,r31
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82109438
  if (cr0.eq) goto loc_82109438;
  // addi r3,r11,8
  r3 = uint64_t(int64_t(int64_t(r11) + 8));
  // bl 0x82108860
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82108860, LIFTED_ARGS(0x82109430, r31)); r3 = o.r3; f1 = o.f1; }
  // lwzx r3,r29,r31
  r3 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82109438, r31)); r3 = o.r3; f1 = o.f1; }
loc_82109438:
  // addi r11,r30,10
  r11 = uint64_t(int64_t(int64_t(r30) + 10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r31
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8210945c
  if (cr0.eq) goto loc_8210945C;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295330, LIFTED_ARGS(0x8210945C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210945C:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r30,r11,24
  r30 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x82109414
  if (cr6.lt) goto loc_82109414;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8258d8c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D8C8, LIFTED_ARGS(0x82109474, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // bl 0x82108a08
  // (left out: dead in C++)
  { const lifted::Result o = f82108A08(base, r1, r13, r3, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8258d908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D908, LIFTED_ARGS(0x82109490, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82109A40: lifted from generated/ by lift.py.
lifted::Result f82109A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82109A40 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,23552(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 23552);
  // bl 0x82109400
  // (left out: dead in C++)
  { const lifted::Result o = f82109400(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r3,23552(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 23552);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82109a84
  if (cr6.eq) goto loc_82109A84;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295330, LIFTED_ARGS(0x82109A78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,23552(r31)
  REX_STORE_U32(uint32_t(r31) + 23552, uint32_t(r11));
  // b 0x82109a90
  goto loc_82109A90;
loc_82109A84:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r11,31872
  r3 = uint64_t(int64_t(int64_t(r11) + 31872));
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296270, LIFTED_ARGS(0x82109A90, r31)); r3 = o.r3; f1 = o.f1; }
loc_82109A90:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210A4D8: lifted from generated/ by lift.py.
lifted::Result f8210A4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8210A4D8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x82109a40
  // (left out: dead in C++)
  { const lifted::Result o = f82109A40(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,23604(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 23604);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8210a518
  if (cr6.eq) goto loc_8210A518;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295330, LIFTED_ARGS(0x8210A50C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,23604(r31)
  REX_STORE_U32(uint32_t(r31) + 23604, uint32_t(r11));
  // b 0x8210a524
  goto loc_8210A524;
loc_8210A518:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r11,32352
  r3 = uint64_t(int64_t(int64_t(r11) + 32352));
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296270, LIFTED_ARGS(0x8210A524, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210A524:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210AE38: lifted from generated/ by lift.py.
lifted::Result f8210AE38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8210AE38 (lifted)");
  uint64_t r3 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x82109a40
  // (left out: dead in C++)
  { const lifted::Result o = f82109A40(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x8210a4d8
  // (left out: dead in C++)
  { const lifted::Result o = f8210A4D8(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8210AEA0: lifted from generated/ by lift.py.
void f8210AEA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8210AEA0 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,32472
  r11 = uint64_t(int64_t(int64_t(r11) + 32472));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x8259c6f0
  f8259C6F0(base, r3);
  return;
}

// sub_8210AFA0: lifted from generated/ by lift.py.
lifted::Result f8210AFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210AFA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r30,r3
  r30 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // bl 0x8259c868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259C868, LIFTED_ARGS(0x8210AFC8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8210affc
  if (cr0.eq) goto loc_8210AFFC;
loc_8210AFD0:
  // mr r4,r31
  r4 = r31;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x8259c818
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259C818, LIFTED_ARGS(0x8210AFDC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8210b388
  // (left out: dead in C++)
  { const lifted::Result o = f8210B388(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bl 0x8259c868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259C868, LIFTED_ARGS(0x8210AFF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r31,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r3), so);
  // blt cr6,0x8210afd0
  if (cr6.lt) goto loc_8210AFD0;
loc_8210AFFC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210B090: lifted from generated/ by lift.py.
lifted::Result f8210B090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B090 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,32472
  r11 = uint64_t(int64_t(int64_t(r11) + 32472));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8259c6f0
  // (left out: dead in C++)
  f8259C6F0(base, r3);
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8210b0cc
  if (cr0.eq) goto loc_8210B0CC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8210B0CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210B0CC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210B0E8: lifted from generated/ by lift.py.
lifted::Result f8210B0E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B0E8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,32536
  r11 = uint64_t(int64_t(int64_t(r11) + 32536));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8259c7f8
  // (left out: dead in C++)
  f8259C7F8(base, r3);
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8210b124
  if (cr0.eq) goto loc_8210B124;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8210B124, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210B124:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210B388: lifted from generated/ by lift.py.
lifted::Result f8210B388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B388 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,2884(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 2884);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8210b420
  if (cr0.eq) goto loc_8210B420;
  // lwz r11,2888(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 2888);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8210b3fc
  if (cr6.lt) goto loc_8210B3FC;
  // beq cr6,0x8210b3ec
  if (cr6.eq) goto loc_8210B3EC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x8210b3dc
  if (cr6.lt) goto loc_8210B3DC;
  // bne cr6,0x8210b41c
  if (!cr6.eq) goto loc_8210B41C;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // b 0x8210b404
  goto loc_8210B404;
loc_8210B3DC:
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // b 0x8210b404
  goto loc_8210B404;
loc_8210B3EC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // b 0x8210b404
  goto loc_8210B404;
loc_8210B3FC:
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // addi r4,r1,92
  r4 = uint64_t(int64_t(int64_t(r1) + 92));
loc_8210B404:
  // lwz r3,2892(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 2892);
  // addi r5,r31,2464
  r5 = uint64_t(int64_t(int64_t(r31) + 2464));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8210B41C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210B41C:
  // stb r30,2884(r31)
  REX_STORE_U8(uint32_t(r31) + 2884, uint8_t(r30));
loc_8210B420:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210B650: lifted from generated/ by lift.py.
lifted::Result f8210B650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B650 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,32632
  r11 = uint64_t(int64_t(int64_t(r11) + 32632));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8210b388
  // (left out: dead in C++)
  { const lifted::Result o = f8210B388(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,2464
  r3 = uint64_t(int64_t(int64_t(r31) + 2464));
  // bl 0x8258cea0
  // (left out: dead in C++)
  f8258CEA0(base, r1, r13, r3);
  // mr r3,r31
  r3 = r31;
  // bl 0x8210aea0
  // (left out: dead in C++)
  f8210AEA0(base, r1, r13, r3);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210B6E8: lifted from generated/ by lift.py.
uint64_t f8210B6E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8210B6E8 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // bl 0x8210b140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8210B140, LIFTED_ARGS(0x8210B6F8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,30
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 30, so);
  // li r3,32767
  r3 = uint64_t(int64_t(32767));
  // bne cr6,0x8210b708
  if (!cr6.eq) goto loc_8210B708;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8210B708:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8210B908: lifted from generated/ by lift.py.
lifted::Result f8210B908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B908 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x8210b650
  // (left out: dead in C++)
  { const lifted::Result o = f8210B650(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8210b938
  if (cr0.eq) goto loc_8210B938;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8210B938, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210B938:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210BDB8: lifted from generated/ by lift.py.
lifted::Result f8210BDB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8210BDB8 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r31,r3,448
  r31 = uint64_t(int64_t(int64_t(r3) + 448));
  // mr r3,r31
  r3 = r31;
  // bl 0x8210b8a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8210B8A8, LIFTED_ARGS(0x8210BDD4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,624
  r3 = uint64_t(int64_t(int64_t(r31) + 624));
  // bl 0x8210b438
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8210B438, LIFTED_ARGS(0x8210BDDC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8210CDE8: lifted from generated/ by lift.py.
lifted::Result f8210CDE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8210CDE8 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-32744
  r11 = uint64_t(int64_t(int64_t(r11) + -32744));
  // addi r10,r10,2908
  r10 = uint64_t(int64_t(int64_t(r10) + 2908));
  // clrlwi. r9,r4,31
  r9 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r10));
  // beq 0x8210ce20
  if (cr0.eq) goto loc_8210CE20;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8210CE20, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210CE20:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82110388: lifted from generated/ by lift.py.
lifted::Result f82110388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82110388 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,23680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 23680);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821103BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23688(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23688);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,23680(r31)
  REX_STORE_U32(uint32_t(r31) + 23680, uint32_t(r11));
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x821103D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23692);
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x821103DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821116E0: lifted from generated/ by lift.py.
uint64_t f821116E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821116E0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r30,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), -1, so);
  // beq cr6,0x82111778
  if (cr6.eq) goto loc_82111778;
  // addis r29,r31,2
  r29 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r29,r29,-26120
  r29 = uint64_t(int64_t(int64_t(r29) + -26120));
  // mr r3,r29
  r3 = r29;
  // bl 0x82116da8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82116DA8, LIFTED_ARGS(0x8211170C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,8746
  r11 = uint64_t(int64_t(int64_t(r30) + 8746));
  // mr r4,r30
  r4 = r30;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // mr r3,r29
  r3 = r29;
  // add r30,r11,r31
  r30 = r11 + r31;
  // bl 0x82116da8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82116DA8, LIFTED_ARGS(0x82111724, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,48
  r11 = uint64_t(int64_t(int64_t(r31) + 48));
  // lwz r9,6104(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 6104);
  // lha r10,20(r11)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 20))));
  // cmpwi cr6,r10,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9999, so);
  // bge cr6,0x82111740
  if (!cr6.lt) goto loc_82111740;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // sth r10,20(r11)
  REX_STORE_U16(uint32_t(r11) + 20, uint16_t(r10));
loc_82111740:
  // addi r10,r9,414
  r10 = uint64_t(int64_t(int64_t(r9) + 414));
  // mulli r10,r10,14
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(14))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lha r10,12(r11)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 12))));
  // cmpwi cr6,r10,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9999, so);
  // bge cr6,0x82111760
  if (!cr6.lt) goto loc_82111760;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // sth r10,12(r11)
  REX_STORE_U16(uint32_t(r11) + 12, uint16_t(r10));
loc_82111760:
  // lha r11,6(r30)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r30) + 6))));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r11,999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 999, so);
  // blt cr6,0x82111774
  if (cr6.lt) goto loc_82111774;
  // li r11,999
  r11 = uint64_t(int64_t(999));
loc_82111774:
  // sth r11,6(r30)
  REX_STORE_U16(uint32_t(r30) + 6, uint16_t(r11));
loc_82111778:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82113400: lifted from generated/ by lift.py.
void f82113400(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82113400 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x82113410
  if (!cr6.eq) goto loc_82113410;
  // addi r9,r3,204
  r9 = uint64_t(int64_t(int64_t(r3) + 204));
  // b 0x82113430
  goto loc_82113430;
loc_82113410:
  // lha r10,204(r3)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r3) + 204))));
  // addic. r11,r4,-1
  ca = uint32_t(r4) > 0;
  r11 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // b 0x82113424
  goto loc_82113424;
loc_8211341C:
  // lha r10,0(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_82113424:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r9,r10,r3
  r9 = r10 + r3;
  // bne 0x8211341c
  if (!cr0.eq) goto loc_8211341C;
loc_82113430:
  // lhz r10,0(r9)
  r10 = REX_LOAD_U16(uint32_t(r9) + 0);
  // extsh r11,r10
  r11 = uint64_t(int64_t(int16_t(r10)));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lhzx r8,r11,r3
  r8 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r3));
  // sthx r10,r11,r3
  REX_STORE_U16(uint32_t(r11) + uint32_t(r3), uint16_t(r10));
  // sth r8,0(r9)
  REX_STORE_U16(uint32_t(r9) + 0, uint16_t(r8));
  // lhzx r9,r11,r3
  r9 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r3));
  // lhz r10,200(r3)
  r10 = REX_LOAD_U16(uint32_t(r3) + 200);
  // sth r9,200(r3)
  REX_STORE_U16(uint32_t(r3) + 200, uint16_t(r9));
  // sthx r10,r11,r3
  REX_STORE_U16(uint32_t(r11) + uint32_t(r3), uint16_t(r10));
  // lhz r11,208(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 208);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // sth r11,208(r3)
  REX_STORE_U16(uint32_t(r3) + 208, uint16_t(r11));
  // blr
  return;
}

// sub_82116BE0: lifted from generated/ by lift.py.
uint64_t f82116BE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82116BE0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // addi r3,r31,6120
  r3 = uint64_t(int64_t(int64_t(r31) + 6120));
  // bl 0x82113400
  // (left out: dead in C++)
  f82113400(base, r3, r4);
  // addis r11,r31,2
  r11 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r11,r11,-26336
  r11 = uint64_t(int64_t(int64_t(r11) + -26336));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r30,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r10), so);
  // bge cr6,0x82116c20
  if (!cr6.lt) goto loc_82116C20;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_82116C20:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82116D88: lifted from generated/ by lift.py.
uint64_t f82116D88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82116D88 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r3,48
  r11 = uint64_t(int64_t(int64_t(r3) + 48));
  // lha r10,6006(r11)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 6006))));
  // cmpwi cr6,r10,9999
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9999, so);
  // bge cr6,0x82116da0
  if (!cr6.lt) goto loc_82116DA0;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // sth r10,6006(r11)
  REX_STORE_U16(uint32_t(r11) + 6006, uint16_t(r10));
loc_82116DA0:
  // b 0x82116be0
  r3 = f82116BE0(base, r1, r13, r3, r4);
  return r3;
}

// sub_82116FB0: lifted from generated/ by lift.py.
lifted::Result f82116FB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82116FB0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r11,23712(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23712);
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x82116FD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,23712(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 23712);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82116FD8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117348: lifted from generated/ by lift.py.
lifted::Result f82117348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82117348 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r3,300
  r3 = uint64_t(int64_t(300));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x8211736C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117380
  if (cr0.eq) goto loc_82117380;
  // bl 0x8259c9b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259C9B0, LIFTED_ARGS(0x8211737C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82117384
  goto loc_82117384;
loc_82117380:
  // mr r3,r30
  r3 = r30;
loc_82117384:
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // mr r4,r29
  r4 = r29;
  // stb r30,8(r31)
  REX_STORE_U8(uint32_t(r31) + 8, uint8_t(r30));
  // stb r30,9(r31)
  REX_STORE_U8(uint32_t(r31) + 9, uint8_t(r30));
  // stb r30,10(r31)
  REX_STORE_U8(uint32_t(r31) + 10, uint8_t(r30));
  // stb r30,11(r31)
  REX_STORE_U8(uint32_t(r31) + 11, uint8_t(r30));
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r11
  r3 = r11;
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // lwz r10,0(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 0);
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821173C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r4,r3
  r4 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq 0x821173e4
  if (cr0.eq) goto loc_821173E4;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // sth r11,80(r1)
  REX_STORE_U16(uint32_t(r1) + 80, uint16_t(r11));
  // bl 0x8259cb48
  // (left out: dead in C++)
  { const lifted::Result o = f8259CB48(base, r1, r13, r3, r4, r5, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821173E4:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82117400: lifted from generated/ by lift.py.
lifted::Result f82117400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117400 (lifted)");
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // b 0x8259cc58
  { const lifted::Result o = f8259CC58(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117408: lifted from generated/ by lift.py.
lifted::Result f82117408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82117408 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // bl 0x8259cca0
  // (left out: dead in C++)
  { const lifted::Result o = f8259CCA0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82117438: lifted from generated/ by lift.py.
lifted::Result f82117438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82117438 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lbz r11,11(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 11);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117474
  if (cr0.eq) goto loc_82117474;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259ccf8
  // (left out: dead in C++)
  { const lifted::Result o = f8259CCF8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82117474
  if (cr0.eq) goto loc_82117474;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stb r29,11(r31)
  REX_STORE_U8(uint32_t(r31) + 11, uint8_t(r29));
  // bl 0x8259cc38
  // (left out: dead in C++)
  { const lifted::Result o = f8259CC38(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82117474); r3 = o.r3; f1 = o.f1; }
loc_82117474:
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821174cc
  if (cr0.eq) goto loc_821174CC;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259cca0
  // (left out: dead in C++)
  { const lifted::Result o = f8259CCA0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821174cc
  if (cr0.eq) goto loc_821174CC;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821174A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,8(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821174C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259d160
  // (left out: dead in C++)
  { const lifted::Result o = f8259D160(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
loc_821174CC:
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259cca0
  // (left out: dead in C++)
  { const lifted::Result o = f8259CCA0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821174e8
  if (cr0.eq) goto loc_821174E8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,10(r31)
  REX_STORE_U8(uint32_t(r31) + 10, uint8_t(r11));
  // b 0x82117548
  goto loc_82117548;
loc_821174E8:
  // lbz r11,10(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117544
  if (cr0.eq) goto loc_82117544;
  // lbz r11,9(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 9);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117544
  if (cr0.eq) goto loc_82117544;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259cbe8
  // (left out: dead in C++)
  { const lifted::Result o = f8259CBE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r4,r31,16
  r4 = uint64_t(int64_t(int64_t(r31) + 16));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117520, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r4,r3
  r4 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq 0x82117544
  if (cr0.eq) goto loc_82117544;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // sth r11,80(r1)
  REX_STORE_U16(uint32_t(r1) + 80, uint16_t(r11));
  // bl 0x8259cb48
  // (left out: dead in C++)
  { const lifted::Result o = f8259CB48(base, r1, r13, r3, r4, r5, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8259cc38
  // (left out: dead in C++)
  { const lifted::Result o = f8259CC38(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82117544); r3 = o.r3; f1 = o.f1; }
loc_82117544:
  // stb r29,10(r31)
  REX_STORE_U8(uint32_t(r31) + 10, uint8_t(r29));
loc_82117548:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82117550: lifted from generated/ by lift.py.
void f82117550(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82117550 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r11,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r11));
  // blr
  return;
}

// sub_82117598: lifted from generated/ by lift.py.
uint64_t f82117598(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82117598 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // clrlwi r3,r11,24
  r3 = uint32_t(r11) & 0xFF;
  // blr
  return r3;
}

// sub_821175D8: lifted from generated/ by lift.py.
lifted::Result f821175D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821175D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r31,0(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82117614
  if (cr0.eq) goto loc_82117614;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259cbe8
  // (left out: dead in C++)
  { const lifted::Result o = f8259CBE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8211760C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_82117614:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117630: lifted from generated/ by lift.py.
lifted::Result f82117630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82117630 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r31,r3,4
  r31 = uint64_t(int64_t(int64_t(r3) + 4));
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8211765c
  if (cr0.eq) goto loc_8211765C;
  // bl 0x8259cbe8
  // (left out: dead in C++)
  { const lifted::Result o = f8259CBE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821175d8
  // (left out: dead in C++)
  { const lifted::Result o = f821175D8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8211765C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117698: lifted from generated/ by lift.py.
uint64_t f82117698(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82117698 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,108(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 108);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x821176b4
  if (!cr6.eq) goto loc_821176B4;
  // lbz r11,109(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 109);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beqlr cr6
  if (cr6.eq) return r3;
loc_821176B4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_821176C0: lifted from generated/ by lift.py.
lifted::Result f821176C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821176C0 (lifted)");
  uint64_t r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r3,76(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 76);
  // bl 0x8259d3f0
  // (left out: dead in C++)
  { const lifted::Result o = f8259D3F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x821176D4); r3 = o.r3; f1 = o.f1; }
  // clrlwi r3,r3,16
  r3 = uint32_t(r3) & 0xFFFF;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821176E8: lifted from generated/ by lift.py.
lifted::Result f821176E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821176E8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lbz r11,69(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 69);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82117708
  if (!cr0.eq) goto loc_82117708;
loc_82117700:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82117750
  goto loc_82117750;
loc_82117708:
  // lwz r11,76(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 76);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117700
  if (cr0.eq) goto loc_82117700;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82117700
  if (cr6.eq) goto loc_82117700;
  // lwz r11,76(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 76);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8211773C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82117700
  if (cr0.lt) goto loc_82117700;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // not r11,r11
  r11 = ~r11;
  // clrlwi r3,r11,31
  r3 = uint32_t(r11) & 0x1;
loc_82117750:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82117778: lifted from generated/ by lift.py.
lifted::Result f82117778(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82117778 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r6
  r31 = r6;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821177A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r6,92(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 92);
  // lwz r5,100(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 100);
  // lwz r4,72(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 72);
  // bl 0x8259ce78
  // (left out: dead in C++)
  { const lifted::Result o = f8259CE78(base, r1, r13, r3, r4, r5, r6, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,108(r31)
  REX_STORE_U8(uint32_t(r31) + 108, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821177D0: lifted from generated/ by lift.py.
lifted::Result f821177D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_821177D0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r6
  r31 = r6;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821177F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r6,96(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 96);
  // lwz r5,104(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 104);
  // lwz r4,76(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 76);
  // bl 0x8259cf10
  // (left out: dead in C++)
  { const lifted::Result o = f8259CF10(base, r1, r13, r3, r4, r5, r6, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,109(r31)
  REX_STORE_U8(uint32_t(r31) + 109, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117828: lifted from generated/ by lift.py.
lifted::Result f82117828(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82117828 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r6
  r31 = r6;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117850, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r5,92(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 92);
  // lwz r4,72(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 72);
  // bl 0x8259cfa8
  // (left out: dead in C++)
  { const lifted::Result o = f8259CFA8(base, r1, r13, r3, r4, r5, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,108(r31)
  REX_STORE_U8(uint32_t(r31) + 108, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117878: lifted from generated/ by lift.py.
lifted::Result f82117878(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82117878 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r6
  r31 = r6;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821178A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r5,96(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 96);
  // lwz r4,76(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 76);
  // bl 0x8259d038
  // (left out: dead in C++)
  { const lifted::Result o = f8259D038(base, r1, r13, r3, r4, r5, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,109(r31)
  REX_STORE_U8(uint32_t(r31) + 109, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821178C8: lifted from generated/ by lift.py.
lifted::Result f821178C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821178C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r3,28
  r3 = uint64_t(int64_t(28));
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stb r30,108(r31)
  REX_STORE_U8(uint32_t(r31) + 108, uint8_t(r30));
  // stb r30,109(r31)
  REX_STORE_U8(uint32_t(r31) + 109, uint8_t(r30));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82117940
  if (cr6.eq) goto loc_82117940;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82117900, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117914
  if (cr0.eq) goto loc_82117914;
  // bl 0x8259d418
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D418, LIFTED_ARGS(0x8211790C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x82117918
  goto loc_82117918;
loc_82117914:
  // mr r11,r30
  r11 = r30;
loc_82117918:
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82117924, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117934
  if (cr0.eq) goto loc_82117934;
  // bl 0x8259d3d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D3D8, LIFTED_ARGS(0x82117930, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82117938
  goto loc_82117938;
loc_82117934:
  // mr r3,r30
  r3 = r30;
loc_82117938:
  // stw r3,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r3));
  // b 0x821179a0
  goto loc_821179A0;
loc_82117940:
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82117944, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117958
  if (cr0.eq) goto loc_82117958;
  // bl 0x8259d418
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D418, LIFTED_ARGS(0x82117950, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x8211795c
  goto loc_8211795C;
loc_82117958:
  // mr r11,r30
  r11 = r30;
loc_8211795C:
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82117968, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117978
  if (cr0.eq) goto loc_82117978;
  // bl 0x8259d3d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D3D8, LIFTED_ARGS(0x82117974, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8211797c
  goto loc_8211797C;
loc_82117978:
  // mr r3,r30
  r3 = r30;
loc_8211797C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r3,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r3));
  // lwz r10,72(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,23688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23688);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r10,76(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 76);
  // lwz r11,23692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23692);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821179A0:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820a6a00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A6A00, LIFTED_ARGS(0x821179A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82117a4c
  if (cr6.eq) goto loc_82117A4C;
  // lbz r11,68(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 68);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82117a14
  if (!cr0.eq) goto loc_82117A14;
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x821179C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r11,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r11));
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x821179D8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,2047
  r11 = uint64_t(int64_t(int64_t(r3) + 2047));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // rlwinm r3,r11,0,0,20
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFF800;
  // bl 0x822964c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822964C8, LIFTED_ARGS(0x821179F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lis r11,-32239
  r11 = uint64_t(int64_t(-2112815104));
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // addi r5,r11,30584
  r5 = uint64_t(int64_t(int64_t(r11) + 30584));
  // stw r4,100(r31)
  REX_STORE_U32(uint32_t(r31) + 100, uint32_t(r4));
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x82117A14, r31)); r3 = o.r3; f1 = o.f1; }
loc_82117A14:
  // lwz r3,88(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 88);
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x82117A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,88(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r11,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r11));
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x82117A2C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,2047
  r11 = uint64_t(int64_t(int64_t(r3) + 2047));
  // rlwinm r3,r11,0,0,20
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFF800;
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6940, LIFTED_ARGS(0x82117A38, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lis r11,-32239
  r11 = uint64_t(int64_t(-2112815104));
  // addi r5,r11,30672
  r5 = uint64_t(int64_t(int64_t(r11) + 30672));
  // stw r4,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r4));
  // b 0x82117aa0
  goto loc_82117AA0;
loc_82117A4C:
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x82117A54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r9,72(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lis r10,-32239
  r10 = uint64_t(int64_t(-2112815104));
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // addi r5,r10,30760
  r5 = uint64_t(int64_t(int64_t(r10) + 30760));
  // stw r11,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r11));
  // lwz r4,0(r9)
  r4 = REX_LOAD_U32(uint32_t(r9) + 0);
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x82117A80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 88);
  // bl 0x820a0678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A0678, LIFTED_ARGS(0x82117A88, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r9,76(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 76);
  // lis r10,-32239
  r10 = uint64_t(int64_t(-2112815104));
  // addi r5,r10,30840
  r5 = uint64_t(int64_t(int64_t(r10) + 30840));
  // stw r11,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r11));
  // lwz r4,0(r9)
  r4 = REX_LOAD_U32(uint32_t(r9) + 0);
loc_82117AA0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r3,88(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 88);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r6,r31
  r6 = r31;
  // bl 0x8209de48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209DE48, LIFTED_ARGS(0x82117AB4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820a6a40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A6A40, LIFTED_ARGS(0x82117ABC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117B88: lifted from generated/ by lift.py.
lifted::Result f82117B88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82117B88 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82117c48
  if (cr6.eq) goto loc_82117C48;
  // addi r27,r31,72
  r27 = uint64_t(int64_t(int64_t(r31) + 72));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117bd8
  if (cr6.eq) goto loc_82117BD8;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117BD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
loc_82117BD8:
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117bf8
  if (cr6.eq) goto loc_82117BF8;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // bl 0x822965c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822965C8, LIFTED_ARGS(0x82117BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
loc_82117BF8:
  // addi r30,r31,76
  r30 = uint64_t(int64_t(int64_t(r31) + 76));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117c28
  if (cr6.eq) goto loc_82117C28;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117C20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
loc_82117C28:
  // lwz r28,0(r30)
  r28 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r3,0(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82117c40
  if (cr0.eq) goto loc_82117C40;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82117C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r29));
loc_82117C40:
  // mr r3,r27
  r3 = r27;
  // b 0x82117cac
  goto loc_82117CAC;
loc_82117C48:
  // addi r28,r31,72
  r28 = uint64_t(int64_t(int64_t(r31) + 72));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117c78
  if (cr6.eq) goto loc_82117C78;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117C70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
loc_82117C78:
  // addi r30,r31,76
  r30 = uint64_t(int64_t(int64_t(r31) + 76));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117ca8
  if (cr6.eq) goto loc_82117CA8;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82117CA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
loc_82117CA8:
  // mr r3,r28
  r3 = r28;
loc_82117CAC:
  // bl 0x82117ad8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117AD8, LIFTED_ARGS(0x82117CB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x82117b30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117B30, LIFTED_ARGS(0x82117CB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r31,40
  r3 = uint64_t(int64_t(int64_t(r31) + 40));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS(0x82117CC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // addi r10,r10,2984
  r10 = uint64_t(int64_t(int64_t(r10) + 2984));
  // addi r9,r9,2740
  r9 = uint64_t(int64_t(int64_t(r9) + 2740));
  // addi r8,r8,3768
  r8 = uint64_t(int64_t(int64_t(r8) + 3768));
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
  // stw r8,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r8));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82117D00: lifted from generated/ by lift.py.
lifted::Result f82117D00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82117D00 (lifted)");
  // addi r3,r3,40
  r3 = uint64_t(int64_t(int64_t(r3) + 40));
  // b 0x820a6e00
  { const lifted::Result o = f820A6E00(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117D08: lifted from generated/ by lift.py.
lifted::Result f82117D08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82117D08 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r26,r9
  r26 = r9;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r27,r8
  r27 = r8;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // mr r28,r7
  r28 = r7;
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r10,r10,26332
  r10 = uint64_t(int64_t(int64_t(r10) + 26332));
  // addi r9,r9,10504
  r9 = uint64_t(int64_t(int64_t(r9) + 10504));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // addi r8,r8,10320
  r8 = uint64_t(int64_t(int64_t(r8) + 10320));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // addi r3,r31,40
  r3 = uint64_t(int64_t(int64_t(r31) + 40));
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // mr r29,r5
  r29 = r5;
  // stb r30,16(r31)
  REX_STORE_U8(uint32_t(r31) + 16, uint8_t(r30));
  // stb r7,17(r31)
  REX_STORE_U8(uint32_t(r31) + 17, uint8_t(r7));
  // stb r30,18(r31)
  REX_STORE_U8(uint32_t(r31) + 18, uint8_t(r30));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
  // stw r8,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r8));
  // stw r30,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r30));
  // stw r30,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r30));
  // stw r6,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r6));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825931D0, LIFTED_ARGS(0x82117D88, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 0);
  // mr r3,r31
  r3 = r31;
  // stb r30,69(r31)
  REX_STORE_U8(uint32_t(r31) + 69, uint8_t(r30));
  // stw r30,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r30));
  // stw r30,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r30));
  // stw r28,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r28));
  // stb r11,68(r31)
  REX_STORE_U8(uint32_t(r31) + 68, uint8_t(r11));
  // stw r27,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r27));
  // stw r26,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r26));
  // bl 0x821178c8
  // (left out: dead in C++)
  { const lifted::Result o = f821178C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82117DC0: lifted from generated/ by lift.py.
lifted::Result f82117DC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82117DC0 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-32072
  r11 = uint64_t(int64_t(int64_t(r11) + -32072));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82117dec
  if (cr0.eq) goto loc_82117DEC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82117DEC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82117DEC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82117E20: lifted from generated/ by lift.py.
uint64_t f82117E20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82117E20 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r31,r3,1456
  r31 = uint64_t(int64_t(int64_t(r3) + 1456));
  // mr r30,r5
  r30 = r5;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d5a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D5A8, LIFTED_ARGS(0x82117E44, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d5b8
  // (left out: dead in C++)
  f8259D5B8(base, r3, r4);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82117E68: lifted from generated/ by lift.py.
lifted::Result f82117E68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82117E68 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8259d0c8
  { const lifted::Result o = f8259D0C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117E70: lifted from generated/ by lift.py.
lifted::Result f82117E70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117E70 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8259d100
  { const lifted::Result o = f8259D100(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117E78: lifted from generated/ by lift.py.
lifted::Result f82117E78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117E78 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8259d120
  { const lifted::Result o = f8259D120(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117E80: lifted from generated/ by lift.py.
lifted::Result f82117E80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117E80 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8259d140
  { const lifted::Result o = f8259D140(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117E88: lifted from generated/ by lift.py.
lifted::Result f82117E88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117E88 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8259d1a8
  { const lifted::Result o = lifted::Call(base, 0x8259D1A8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117E90: lifted from generated/ by lift.py.
lifted::Result f82117E90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82117E90 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82117EB0: lifted from generated/ by lift.py.
uint64_t f82117EB0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82117EB0 (lifted)");
  // addi r3,r3,1448
  r3 = uint64_t(int64_t(int64_t(r3) + 1448));
  // blr
  return r3;
}

// sub_82117EB8: lifted from generated/ by lift.py.
uint64_t f82117EB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82117EB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82117ED0:
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117f68
  if (cr0.eq) goto loc_82117F68;
  // lwz r10,1520(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x82117f68
  if (!cr6.lt) goto loc_82117F68;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117f10
  if (cr0.eq) goto loc_82117F10;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x82117f14
  if (cr6.lt) goto loc_82117F14;
loc_82117F10:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82117F14:
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82117f54
  if (cr6.eq) goto loc_82117F54;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82117f40
  if (cr0.eq) goto loc_82117F40;
  // lwz r10,1520(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x82117f44
  if (cr6.lt) goto loc_82117F44;
loc_82117F40:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82117F44:
  // lwzx r3,r29,r11
  r3 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // bl 0x82117670
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117670, LIFTED_ARGS(0x82117F4C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82117f60
  if (!cr0.eq) goto loc_82117F60;
loc_82117F54:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // b 0x82117ed0
  goto loc_82117ED0;
loc_82117F60:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82117f6c
  goto loc_82117F6C;
loc_82117F68:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82117F6C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82117F78: lifted from generated/ by lift.py.
uint64_t f82117F78(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82117F78 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_82117F80:
  // lwz r10,1516(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 1516);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x82117fe4
  if (cr0.eq) goto loc_82117FE4;
  // lwz r9,1520(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 1520);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x82117fe4
  if (!cr6.lt) goto loc_82117FE4;
  // lwz r10,1516(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 1516);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x82117fc0
  if (cr0.eq) goto loc_82117FC0;
  // rotlwi r9,r9,0
  r9 = __builtin_rotateleft32(uint32_t(r9), 0);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x82117fc4
  if (cr6.lt) goto loc_82117FC4;
loc_82117FC0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82117FC4:
  // lwzx r10,r8,r10
  r10 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r10));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82117fdc
  if (cr6.eq) goto loc_82117FDC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // b 0x82117f80
  goto loc_82117F80;
loc_82117FDC:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
loc_82117FE4:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // blr
  return r3;
}

// sub_82117FF0: lifted from generated/ by lift.py.
lifted::Result f82117FF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82117FF0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_8211800C:
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821180a8
  if (cr0.eq) goto loc_821180A8;
  // lwz r10,1520(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x821180a8
  if (!cr6.lt) goto loc_821180A8;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8211804c
  if (cr0.eq) goto loc_8211804C;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x82118050
  if (cr6.lt) goto loc_82118050;
loc_8211804C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118050:
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82118094
  if (cr6.eq) goto loc_82118094;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8211807c
  if (cr0.eq) goto loc_8211807C;
  // lwz r10,1520(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x82118080
  if (cr6.lt) goto loc_82118080;
loc_8211807C:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118080:
  // mr r4,r28
  r4 = r28;
  // lwzx r3,r29,r11
  r3 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // bl 0x82117d00
  // (left out: dead in C++)
  { const lifted::Result o = f82117D00(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821180a0
  if (!cr0.eq) goto loc_821180A0;
loc_82118094:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // b 0x8211800c
  goto loc_8211800C;
loc_821180A0:
  // mr r3,r30
  r3 = r30;
  // b 0x821180ac
  goto loc_821180AC;
loc_821180A8:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_821180AC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821180B8: lifted from generated/ by lift.py.
lifted::Result f821180B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821180B8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // mr r26,r8
  r26 = r8;
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82118100
  if (cr0.eq) goto loc_82118100;
  // lwz r9,1520(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x82118104
  if (cr6.lt) goto loc_82118104;
loc_82118100:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118104:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82118184
  if (!cr6.eq) goto loc_82118184;
  // li r3,112
  r3 = uint64_t(int64_t(112));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x8211811C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82118150
  if (cr0.eq) goto loc_82118150;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r6,r31,4
  r6 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne cr6,0x82118134
  if (!cr6.eq) goto loc_82118134;
  // li r6,0
  r6 = uint64_t(int64_t(0));
loc_82118134:
  // mr r5,r28
  r5 = r28;
  // lwz r9,0(r26)
  r9 = REX_LOAD_U32(uint32_t(r26) + 0);
  // mr r4,r29
  r4 = r29;
  // lwz r8,0(r27)
  r8 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r7,0(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x82117d08
  // (left out: dead in C++)
  { const lifted::Result o = f82117D08(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82118154
  goto loc_82118154;
loc_82118150:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82118154:
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82118178
  if (cr0.eq) goto loc_82118178;
  // lwz r9,1520(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x8211817c
  if (cr6.lt) goto loc_8211817C;
loc_82118178:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8211817C:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r3,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r3));
loc_82118184:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118270: lifted from generated/ by lift.py.
uint64_t f82118270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82118270 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,1516(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 1516);
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x82118294
  if (cr0.eq) goto loc_82118294;
  // lwz r9,1520(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 1520);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x82118298
  if (cr6.lt) goto loc_82118298;
loc_82118294:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118298:
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r9,r10
  r10 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r10));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x821182d8
  if (cr6.eq) goto loc_821182D8;
  // lwz r10,1516(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 1516);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x821182c8
  if (cr0.eq) goto loc_821182C8;
  // lwz r9,1520(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 1520);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x821182cc
  if (cr6.lt) goto loc_821182CC;
loc_821182C8:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821182CC:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // b 0x82117698
  r3 = f82117698(base, r3);
  return r3;
loc_821182D8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82118320: lifted from generated/ by lift.py.
lifted::Result f82118320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82118320 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82118360
  if (cr0.eq) goto loc_82118360;
  // lwz r9,1520(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x82118364
  if (cr6.lt) goto loc_82118364;
loc_82118360:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118364:
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821183f0
  if (cr6.eq) goto loc_821183F0;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82118394
  if (cr0.eq) goto loc_82118394;
  // lwz r9,1520(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x82118398
  if (cr6.lt) goto loc_82118398;
loc_82118394:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82118398:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x821176e8
  // (left out: dead in C++)
  { const lifted::Result o = f821176E8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821183f0
  if (!cr0.eq) goto loc_821183F0;
  // lwz r11,1516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1516);
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821183d0
  if (cr0.eq) goto loc_821183D0;
  // lwz r9,1520(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 1520);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x821183d4
  if (cr6.lt) goto loc_821183D4;
loc_821183D0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_821183D4:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r29
  r4 = r29;
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x821176c0
  // (left out: dead in C++)
  { const lifted::Result o = f821176C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 65535, so);
  // beq cr6,0x821183f0
  if (cr6.eq) goto loc_821183F0;
  // mr r28,r3
  r28 = r3;
loc_821183F0:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82118438: lifted from generated/ by lift.py.
lifted::Result f82118438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118438 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r31,0(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82118474
  if (cr0.eq) goto loc_82118474;
  // mr r3,r31
  r3 = r31;
  // bl 0x82117b88
  // (left out: dead in C++)
  { const lifted::Result o = f82117B88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8211846C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_82118474:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82118490: lifted from generated/ by lift.py.
lifted::Result f82118490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118490 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r31,0(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x821184cc
  if (cr0.eq) goto loc_821184CC;
  // mr r3,r31
  r3 = r31;
  // bl 0x82117630
  // (left out: dead in C++)
  { const lifted::Result o = f82117630(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x821184C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821184CC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821185C8: lifted from generated/ by lift.py.
lifted::Result f821185C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821185C8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0;
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
  // mr r3,r4
  r3 = r4;
  // stw r4,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r4));
  // bl 0x82117408
  // (left out: dead in C++)
  { const lifted::Result o = f82117408(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821185f8
  if (cr0.eq) goto loc_821185F8;
  // addi r3,r1,124
  r3 = uint64_t(int64_t(int64_t(r1) + 124));
  // bl 0x82118490
  // (left out: dead in C++)
  { const lifted::Result o = f82118490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x821185fc
  goto loc_821185FC;
loc_821185F8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_821185FC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82118610: lifted from generated/ by lift.py.
lifted::Result f82118610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82118610 (lifted)");
  uint64_t r3 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r1,124
  r3 = uint64_t(int64_t(int64_t(r1) + 124));
  // stw r4,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r4));
  // bl 0x82118490
  // (left out: dead in C++)
  { const lifted::Result o = f82118490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

namespace {

lifted::Result e820F0E00(uint8_t* base, const lifted::Args& a) {
  return f820F0E00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820F1098(uint8_t* base, const lifted::Args& a) {
  return f820F1098(base, a.r1, a.r13, a.r[0], a.r[6]);
}

lifted::Result e820F1100(uint8_t* base, const lifted::Args& a) {
  return {f820F1100(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820F77A0(uint8_t* base, const lifted::Args& a) {
  return f820F77A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820F7D70(uint8_t* base, const lifted::Args& a) {
  return {f820F7D70(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820F7EE8(uint8_t* base, const lifted::Args& a) {
  return {f820F7EE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820F9DD0(uint8_t* base, const lifted::Args& a) {
  return f820F9DD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e820F9E28(uint8_t* base, const lifted::Args& a) {
  return f820F9E28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820FA8F8(uint8_t* base, const lifted::Args& a) {
  return f820FA8F8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e820FB9C0(uint8_t* base, const lifted::Args& a) {
  f820FB9C0(base, a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820FBB68(uint8_t* base, const lifted::Args& a) {
  return f820FBB68(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e820FF368(uint8_t* base, const lifted::Args& a) {
  return f820FF368(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820FF4D0(uint8_t* base, const lifted::Args& a) {
  return {f820FF4D0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820FF5A0(uint8_t* base, const lifted::Args& a) {
  f820FF5A0(base, a.r1, a.r13, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820FF628(uint8_t* base, const lifted::Args& a) {
  return {f820FF628(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820FF680(uint8_t* base, const lifted::Args& a) {
  return {f820FF680(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820FF790(uint8_t* base, const lifted::Args& a) {
  return {f820FF790(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820FF908(uint8_t* base, const lifted::Args& a) {
  return f820FF908(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e820FF958(uint8_t* base, const lifted::Args& a) {
  return {f820FF958(base), a.f[0]};
}

lifted::Result e820FFA88(uint8_t* base, const lifted::Args& a) {
  f820FFA88(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820FFB60(uint8_t* base, const lifted::Args& a) {
  return {f820FFB60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e82101940(uint8_t* base, const lifted::Args& a) {
  return {f82101940(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821022B0(uint8_t* base, const lifted::Args& a) {
  return {f821022B0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82102A28(uint8_t* base, const lifted::Args& a) {
  return {f82102A28(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82104630(uint8_t* base, const lifted::Args& a) {
  return f82104630(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82104CE0(uint8_t* base, const lifted::Args& a) {
  return f82104CE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82104F00(uint8_t* base, const lifted::Args& a) {
  return f82104F00(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82104F20(uint8_t* base, const lifted::Args& a) {
  return f82104F20(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82105228(uint8_t* base, const lifted::Args& a) {
  return f82105228(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82105688(uint8_t* base, const lifted::Args& a) {
  return f82105688(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82106678(uint8_t* base, const lifted::Args& a) {
  return f82106678(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821066D0(uint8_t* base, const lifted::Args& a) {
  return f821066D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82106718(uint8_t* base, const lifted::Args& a) {
  return f82106718(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82106B60(uint8_t* base, const lifted::Args& a) {
  return f82106B60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82107518(uint8_t* base, const lifted::Args& a) {
  return f82107518(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82107590(uint8_t* base, const lifted::Args& a) {
  return f82107590(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82107C20(uint8_t* base, const lifted::Args& a) {
  return {f82107C20(base, a.r[1]), a.f[0]};
}

lifted::Result e82107C48(uint8_t* base, const lifted::Args& a) {
  f82107C48(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82107D80(uint8_t* base, const lifted::Args& a) {
  f82107D80(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82108A08(uint8_t* base, const lifted::Args& a) {
  return f82108A08(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82108A70(uint8_t* base, const lifted::Args& a) {
  return {f82108A70(base, a.r1, a.r13, a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82109160(uint8_t* base, const lifted::Args& a) {
  return f82109160(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82109400(uint8_t* base, const lifted::Args& a) {
  return f82109400(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82109A40(uint8_t* base, const lifted::Args& a) {
  return f82109A40(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8210A4D8(uint8_t* base, const lifted::Args& a) {
  return f8210A4D8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8210AE38(uint8_t* base, const lifted::Args& a) {
  return f8210AE38(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8210AEA0(uint8_t* base, const lifted::Args& a) {
  f8210AEA0(base, a.r1, a.r13, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8210AFA0(uint8_t* base, const lifted::Args& a) {
  return f8210AFA0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210B090(uint8_t* base, const lifted::Args& a) {
  return f8210B090(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210B0E8(uint8_t* base, const lifted::Args& a) {
  return f8210B0E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210B388(uint8_t* base, const lifted::Args& a) {
  return f8210B388(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210B650(uint8_t* base, const lifted::Args& a) {
  return f8210B650(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210B6E8(uint8_t* base, const lifted::Args& a) {
  return {f8210B6E8(base, a.r1, a.r13, a.r[1]), a.f[0]};
}

lifted::Result e8210B908(uint8_t* base, const lifted::Args& a) {
  return f8210B908(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8210BDB8(uint8_t* base, const lifted::Args& a) {
  return f8210BDB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8210CDE8(uint8_t* base, const lifted::Args& a) {
  return f8210CDE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82110388(uint8_t* base, const lifted::Args& a) {
  return f82110388(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821116E0(uint8_t* base, const lifted::Args& a) {
  return {f821116E0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82113400(uint8_t* base, const lifted::Args& a) {
  f82113400(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82116BE0(uint8_t* base, const lifted::Args& a) {
  return {f82116BE0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82116D88(uint8_t* base, const lifted::Args& a) {
  return {f82116D88(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82116FB0(uint8_t* base, const lifted::Args& a) {
  return f82116FB0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117348(uint8_t* base, const lifted::Args& a) {
  return f82117348(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117400(uint8_t* base, const lifted::Args& a) {
  return f82117400(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82117408(uint8_t* base, const lifted::Args& a) {
  return f82117408(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117438(uint8_t* base, const lifted::Args& a) {
  return f82117438(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117550(uint8_t* base, const lifted::Args& a) {
  f82117550(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82117598(uint8_t* base, const lifted::Args& a) {
  return {f82117598(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821175D8(uint8_t* base, const lifted::Args& a) {
  return f821175D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117630(uint8_t* base, const lifted::Args& a) {
  return f82117630(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117698(uint8_t* base, const lifted::Args& a) {
  return {f82117698(base, a.r[0]), a.f[0]};
}

lifted::Result e821176C0(uint8_t* base, const lifted::Args& a) {
  return f821176C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821176E8(uint8_t* base, const lifted::Args& a) {
  return f821176E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117778(uint8_t* base, const lifted::Args& a) {
  return f82117778(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821177D0(uint8_t* base, const lifted::Args& a) {
  return f821177D0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82117828(uint8_t* base, const lifted::Args& a) {
  return f82117828(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82117878(uint8_t* base, const lifted::Args& a) {
  return f82117878(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821178C8(uint8_t* base, const lifted::Args& a) {
  return f821178C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82117B88(uint8_t* base, const lifted::Args& a) {
  return f82117B88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82117D00(uint8_t* base, const lifted::Args& a) {
  return f82117D00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82117D08(uint8_t* base, const lifted::Args& a) {
  return f82117D08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82117DC0(uint8_t* base, const lifted::Args& a) {
  return f82117DC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82117E20(uint8_t* base, const lifted::Args& a) {
  return {f82117E20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82117E68(uint8_t* base, const lifted::Args& a) {
  return f82117E68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82117E70(uint8_t* base, const lifted::Args& a) {
  return f82117E70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82117E78(uint8_t* base, const lifted::Args& a) {
  return f82117E78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82117E80(uint8_t* base, const lifted::Args& a) {
  return f82117E80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82117E88(uint8_t* base, const lifted::Args& a) {
  return f82117E88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82117E90(uint8_t* base, const lifted::Args& a) {
  return f82117E90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82117EB0(uint8_t* base, const lifted::Args& a) {
  return {f82117EB0(base, a.r[0]), a.f[0]};
}

lifted::Result e82117EB8(uint8_t* base, const lifted::Args& a) {
  return {f82117EB8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82117F78(uint8_t* base, const lifted::Args& a) {
  return {f82117F78(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82117FF0(uint8_t* base, const lifted::Args& a) {
  return f82117FF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e821180B8(uint8_t* base, const lifted::Args& a) {
  return f821180B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82118270(uint8_t* base, const lifted::Args& a) {
  return {f82118270(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82118320(uint8_t* base, const lifted::Args& a) {
  return f82118320(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82118438(uint8_t* base, const lifted::Args& a) {
  return f82118438(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118490(uint8_t* base, const lifted::Args& a) {
  return f82118490(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821185C8(uint8_t* base, const lifted::Args& a) {
  return f821185C8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82118610(uint8_t* base, const lifted::Args& a) {
  return f82118610(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

const Registration kEntries[] = {
    {0x820F0E00, e820F0E00},
    {0x820F1098, e820F1098},
    {0x820F1100, e820F1100},
    {0x820F77A0, e820F77A0},
    {0x820F7D70, e820F7D70},
    {0x820F7EE8, e820F7EE8},
    {0x820F9DD0, e820F9DD0},
    {0x820F9E28, e820F9E28},
    {0x820FA8F8, e820FA8F8},
    {0x820FB9C0, e820FB9C0},
    {0x820FBB68, e820FBB68},
    {0x820FF368, e820FF368},
    {0x820FF4D0, e820FF4D0},
    {0x820FF5A0, e820FF5A0},
    {0x820FF628, e820FF628},
    {0x820FF680, e820FF680},
    {0x820FF790, e820FF790},
    {0x820FF908, e820FF908},
    {0x820FF958, e820FF958},
    {0x820FFA88, e820FFA88},
    {0x820FFB60, e820FFB60},
    {0x82101940, e82101940},
    {0x821022B0, e821022B0},
    {0x82102A28, e82102A28},
    {0x82104630, e82104630},
    {0x82104CE0, e82104CE0},
    {0x82104F00, e82104F00},
    {0x82104F20, e82104F20},
    {0x82105228, e82105228},
    {0x82105688, e82105688},
    {0x82106678, e82106678},
    {0x821066D0, e821066D0},
    {0x82106718, e82106718},
    {0x82106B60, e82106B60},
    {0x82107518, e82107518},
    {0x82107590, e82107590},
    {0x82107C20, e82107C20},
    {0x82107C48, e82107C48},
    {0x82107D80, e82107D80},
    {0x82108A08, e82108A08},
    {0x82108A70, e82108A70},
    {0x82109160, e82109160},
    {0x82109400, e82109400},
    {0x82109A40, e82109A40},
    {0x8210A4D8, e8210A4D8},
    {0x8210AE38, e8210AE38},
    {0x8210AEA0, e8210AEA0},
    {0x8210AFA0, e8210AFA0},
    {0x8210B090, e8210B090},
    {0x8210B0E8, e8210B0E8},
    {0x8210B388, e8210B388},
    {0x8210B650, e8210B650},
    {0x8210B6E8, e8210B6E8},
    {0x8210B908, e8210B908},
    {0x8210BDB8, e8210BDB8},
    {0x8210CDE8, e8210CDE8},
    {0x82110388, e82110388},
    {0x821116E0, e821116E0},
    {0x82113400, e82113400},
    {0x82116BE0, e82116BE0},
    {0x82116D88, e82116D88},
    {0x82116FB0, e82116FB0},
    {0x82117348, e82117348},
    {0x82117400, e82117400},
    {0x82117408, e82117408},
    {0x82117438, e82117438},
    {0x82117550, e82117550},
    {0x82117598, e82117598},
    {0x821175D8, e821175D8},
    {0x82117630, e82117630},
    {0x82117698, e82117698},
    {0x821176C0, e821176C0},
    {0x821176E8, e821176E8},
    {0x82117778, e82117778},
    {0x821177D0, e821177D0},
    {0x82117828, e82117828},
    {0x82117878, e82117878},
    {0x821178C8, e821178C8},
    {0x82117B88, e82117B88},
    {0x82117D00, e82117D00},
    {0x82117D08, e82117D08},
    {0x82117DC0, e82117DC0},
    {0x82117E20, e82117E20},
    {0x82117E68, e82117E68},
    {0x82117E70, e82117E70},
    {0x82117E78, e82117E78},
    {0x82117E80, e82117E80},
    {0x82117E88, e82117E88},
    {0x82117E90, e82117E90},
    {0x82117EB0, e82117EB0},
    {0x82117EB8, e82117EB8},
    {0x82117F78, e82117F78},
    {0x82117FF0, e82117FF0},
    {0x821180B8, e821180B8},
    {0x82118270, e82118270},
    {0x82118320, e82118320},
    {0x82118438, e82118438},
    {0x82118490, e82118490},
    {0x821185C8, e821185C8},
    {0x82118610, e82118610},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_005)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_820F0E00) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F0E00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F0E00);
}

extern "C" REX_FUNC(sub_820F1098) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F1098(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F1098);
}

extern "C" REX_FUNC(sub_820F1100) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F1100(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F1100);
}

extern "C" REX_FUNC(sub_820F77A0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F77A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F77A0);
}

extern "C" REX_FUNC(sub_820F7D70) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F7D70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F7D70);
}

extern "C" REX_FUNC(sub_820F7EE8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F7EE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F7EE8);
}

extern "C" REX_FUNC(sub_820F9DD0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F9DD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F9DD0);
}

extern "C" REX_FUNC(sub_820F9E28) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820F9E28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820F9E28);
}

extern "C" REX_FUNC(sub_820FA8F8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FA8F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FA8F8);
}

extern "C" REX_FUNC(sub_820FB9C0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FB9C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FB9C0);
}

extern "C" REX_FUNC(sub_820FBB68) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FBB68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FBB68);
}

extern "C" REX_FUNC(sub_820FF368) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF368(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF368);
}

extern "C" REX_FUNC(sub_820FF4D0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF4D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF4D0);
}

extern "C" REX_FUNC(sub_820FF5A0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF5A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF5A0);
}

extern "C" REX_FUNC(sub_820FF628) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF628(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF628);
}

extern "C" REX_FUNC(sub_820FF680) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF680(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF680);
}

extern "C" REX_FUNC(sub_820FF790) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF790(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF790);
}

extern "C" REX_FUNC(sub_820FF908) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF908(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF908);
}

extern "C" REX_FUNC(sub_820FF958) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FF958(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FF958);
}

extern "C" REX_FUNC(sub_820FFA88) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FFA88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FFA88);
}

extern "C" REX_FUNC(sub_820FFB60) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_820FFB60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820FFB60);
}

extern "C" REX_FUNC(sub_82101940) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82101940(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82101940);
}

extern "C" REX_FUNC(sub_821022B0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821022B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821022B0);
}

extern "C" REX_FUNC(sub_82102A28) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82102A28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82102A28);
}

extern "C" REX_FUNC(sub_82104630) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82104630(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82104630);
}

extern "C" REX_FUNC(sub_82104CE0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82104CE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82104CE0);
}

extern "C" REX_FUNC(sub_82104F00) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82104F00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82104F00);
}

extern "C" REX_FUNC(sub_82104F20) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82104F20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82104F20);
}

extern "C" REX_FUNC(sub_82105228) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82105228(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82105228);
}

extern "C" REX_FUNC(sub_82105688) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82105688(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82105688);
}

extern "C" REX_FUNC(sub_82106678) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82106678(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82106678);
}

extern "C" REX_FUNC(sub_821066D0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821066D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821066D0);
}

extern "C" REX_FUNC(sub_82106718) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82106718(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82106718);
}

extern "C" REX_FUNC(sub_82106B60) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82106B60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82106B60);
}

extern "C" REX_FUNC(sub_82107518) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82107518(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82107518);
}

extern "C" REX_FUNC(sub_82107590) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82107590(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82107590);
}

extern "C" REX_FUNC(sub_82107C20) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82107C20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82107C20);
}

extern "C" REX_FUNC(sub_82107C48) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82107C48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82107C48);
}

extern "C" REX_FUNC(sub_82107D80) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82107D80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82107D80);
}

extern "C" REX_FUNC(sub_82108A08) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82108A08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82108A08);
}

extern "C" REX_FUNC(sub_82108A70) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82108A70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82108A70);
}

extern "C" REX_FUNC(sub_82109160) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82109160(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82109160);
}

extern "C" REX_FUNC(sub_82109400) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82109400(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82109400);
}

extern "C" REX_FUNC(sub_82109A40) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82109A40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82109A40);
}

extern "C" REX_FUNC(sub_8210A4D8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210A4D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210A4D8);
}

extern "C" REX_FUNC(sub_8210AE38) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210AE38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210AE38);
}

extern "C" REX_FUNC(sub_8210AEA0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210AEA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210AEA0);
}

extern "C" REX_FUNC(sub_8210AFA0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210AFA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210AFA0);
}

extern "C" REX_FUNC(sub_8210B090) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B090(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B090);
}

extern "C" REX_FUNC(sub_8210B0E8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B0E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B0E8);
}

extern "C" REX_FUNC(sub_8210B388) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B388(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B388);
}

extern "C" REX_FUNC(sub_8210B650) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B650(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B650);
}

extern "C" REX_FUNC(sub_8210B6E8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B6E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B6E8);
}

extern "C" REX_FUNC(sub_8210B908) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210B908(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210B908);
}

extern "C" REX_FUNC(sub_8210BDB8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210BDB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210BDB8);
}

extern "C" REX_FUNC(sub_8210CDE8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_8210CDE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8210CDE8);
}

extern "C" REX_FUNC(sub_82110388) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82110388(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82110388);
}

extern "C" REX_FUNC(sub_821116E0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821116E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821116E0);
}

extern "C" REX_FUNC(sub_82113400) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82113400(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82113400);
}

extern "C" REX_FUNC(sub_82116BE0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82116BE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82116BE0);
}

extern "C" REX_FUNC(sub_82116D88) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82116D88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82116D88);
}

extern "C" REX_FUNC(sub_82116FB0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82116FB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82116FB0);
}

extern "C" REX_FUNC(sub_82117348) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117348(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117348);
}

extern "C" REX_FUNC(sub_82117400) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117400(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117400);
}

extern "C" REX_FUNC(sub_82117408) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117408(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117408);
}

extern "C" REX_FUNC(sub_82117438) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117438);
}

extern "C" REX_FUNC(sub_82117550) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117550(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117550);
}

extern "C" REX_FUNC(sub_82117598) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117598(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117598);
}

extern "C" REX_FUNC(sub_821175D8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821175D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821175D8);
}

extern "C" REX_FUNC(sub_82117630) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117630(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117630);
}

extern "C" REX_FUNC(sub_82117698) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117698(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117698);
}

extern "C" REX_FUNC(sub_821176C0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821176C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821176C0);
}

extern "C" REX_FUNC(sub_821176E8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821176E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821176E8);
}

extern "C" REX_FUNC(sub_82117778) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117778(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117778);
}

extern "C" REX_FUNC(sub_821177D0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821177D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821177D0);
}

extern "C" REX_FUNC(sub_82117828) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117828(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117828);
}

extern "C" REX_FUNC(sub_82117878) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117878(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117878);
}

extern "C" REX_FUNC(sub_821178C8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821178C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821178C8);
}

extern "C" REX_FUNC(sub_82117B88) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117B88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117B88);
}

extern "C" REX_FUNC(sub_82117D00) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117D00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117D00);
}

extern "C" REX_FUNC(sub_82117D08) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117D08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117D08);
}

extern "C" REX_FUNC(sub_82117DC0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117DC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117DC0);
}

extern "C" REX_FUNC(sub_82117E20) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E20);
}

extern "C" REX_FUNC(sub_82117E68) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E68);
}

extern "C" REX_FUNC(sub_82117E70) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E70);
}

extern "C" REX_FUNC(sub_82117E78) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E78);
}

extern "C" REX_FUNC(sub_82117E80) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E80);
}

extern "C" REX_FUNC(sub_82117E88) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E88);
}

extern "C" REX_FUNC(sub_82117E90) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117E90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117E90);
}

extern "C" REX_FUNC(sub_82117EB0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117EB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117EB0);
}

extern "C" REX_FUNC(sub_82117EB8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117EB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117EB8);
}

extern "C" REX_FUNC(sub_82117F78) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117F78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117F78);
}

extern "C" REX_FUNC(sub_82117FF0) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82117FF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82117FF0);
}

extern "C" REX_FUNC(sub_821180B8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821180B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821180B8);
}

extern "C" REX_FUNC(sub_82118270) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82118270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82118270);
}

extern "C" REX_FUNC(sub_82118320) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82118320(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82118320);
}

extern "C" REX_FUNC(sub_82118438) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82118438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82118438);
}

extern "C" REX_FUNC(sub_82118490) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82118490(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82118490);
}

extern "C" REX_FUNC(sub_821185C8) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_821185C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821185C8);
}

extern "C" REX_FUNC(sub_82118610) {
  if (!ITC_FLAG(native_lifted_005)) {
    __imp__sub_82118610(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82118610);
}
