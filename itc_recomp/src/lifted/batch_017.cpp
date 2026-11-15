// Lift sweep batch 017 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_017, true, "ITC",
              "Run lift sweep batch 017's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address.
#define LIFTED_ARGS(lr) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr) }

namespace lifted {

// sub_8246D310: lifted from generated/ by lift.py.
lifted::Result f8246D310(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D310 (lifted)");
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246d0c8
  { const lifted::Result o = lifted::Call(base, 0x8246D0C8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D318: lifted from generated/ by lift.py.
lifted::Result f8246D318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D318 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246cec0
  { const lifted::Result o = f8246CEC0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D328: lifted from generated/ by lift.py.
lifted::Result f8246D328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246D328 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a998
  { const lifted::Result o = f8246A998(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D330: lifted from generated/ by lift.py.
uint64_t f8246D330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D330 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82460370
  { const lifted::Result o = lifted::Call(base, 0x82460370, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_8246D338: lifted from generated/ by lift.py.
lifted::Result f8246D338(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D338 (lifted)");
  uint64_t r10 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246cfa8
  { const lifted::Result o = lifted::Call(base, 0x8246CFA8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D340: lifted from generated/ by lift.py.
lifted::Result f8246D340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246D340 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a9f0
  { const lifted::Result o = f8246A9F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D348: lifted from generated/ by lift.py.
lifted::Result f8246D348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246D348 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246D360)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r28,r3
  r28 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r29,r13
  r29 = r13;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246d388
  if (cr6.eq) goto loc_8246D388;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x8246d39c
  if (cr6.eq) goto loc_8246D39C;
loc_8246D388:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246D390)); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246D39C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r10,r13
  r10 = r13;
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lhz r9,108(r30)
  r9 = REX_LOAD_U16(uint32_t(r30) + 108);
  // divwu r11,r11,r9
  r11 = uint32_t(uint32_t(r9) ? uint32_t(r11) / uint32_t(r9) : 0);
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // stw r11,120(r30)
  REX_STORE_U32(uint32_t(r30) + 120, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246d400
  if (cr6.eq) goto loc_8246D400;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246d400
  if (!cr6.eq) goto loc_8246D400;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8246d400
  if (!cr6.eq) goto loc_8246D400;
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246D3F8)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246D400)); r3 = o.r3; f1 = o.f1; }
loc_8246D400:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246D738: lifted from generated/ by lift.py.
lifted::Result f8246D738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D738 (lifted)");
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246d630
  { const lifted::Result o = lifted::Call(base, 0x8246D630, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D740: lifted from generated/ by lift.py.
lifted::Result f8246D740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D740 (lifted)");
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246d500
  { const lifted::Result o = lifted::Call(base, 0x8246D500, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D748: lifted from generated/ by lift.py.
lifted::Result f8246D748(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D748 (lifted)");
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246d410
  { const lifted::Result o = lifted::Call(base, 0x8246D410, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_824784D0: lifted from generated/ by lift.py.
void f824784D0(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824784D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r7,13(r3)
  r7 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r9
  r8 = r9 - r11;
  // lwz r5,24(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r7,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r11))));
  // lwz r4,0(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r9,r10,r5
  r9 = r5 - r10;
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r5,r5,r4
  r5 = r5 + r4;
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // add r6,r11,r6
  r6 = r11 + r6;
  // mr r4,r8
  r4 = r8;
  // blt cr6,0x82478520
  if (cr6.lt) goto loc_82478520;
  // mr r4,r9
  r4 = r9;
loc_82478520:
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r31,r7,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
loc_82478544:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi cr6,r7,4
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 4, so);
  // blt cr6,0x824785ac
  if (cr6.lt) goto loc_824785AC;
  // addi r11,r7,-4
  r11 = uint64_t(int64_t(int64_t(r7) + -4));
  // addi r10,r6,2048
  r10 = uint64_t(int64_t(int64_t(r6) + 2048));
  // rlwinm r9,r11,30,2,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // addi r11,r5,8
  r11 = uint64_t(int64_t(int64_t(r5) + 8));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r8,r9,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
loc_82478568:
  // lfs f12,-8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -8);
  f12 = double(temp.f32);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,-2048(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + -2048, temp.u32);
  // lfs f12,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f12 = double(temp.f32);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,-1024(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + -1024, temp.u32);
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // lfs f12,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,1024(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // addi r10,r10,4096
  r10 = uint64_t(int64_t(int64_t(r10) + 4096));
  // bne cr6,0x82478568
  if (!cr6.eq) goto loc_82478568;
loc_824785AC:
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // bge cr6,0x824785e8
  if (!cr6.lt) goto loc_824785E8;
  // rlwinm r10,r8,10,0,21
  r10 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 10) & 0xFFFFFC00;
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // add r9,r10,r6
  r9 = r10 + r6;
  // add r10,r11,r5
  r10 = r11 + r5;
  // subf r11,r8,r7
  r11 = r7 - r8;
loc_824785C8:
  // lfs f12,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r9)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r9,r9,1024
  r9 = uint64_t(int64_t(int64_t(r9) + 1024));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x824785c8
  if (!cr6.eq) goto loc_824785C8;
loc_824785E8:
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // add r5,r31,r5
  r5 = r31 + r5;
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x82478544
  if (!cr6.eq) goto loc_82478544;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r10,r10,2
  r10 = __builtin_rotateleft32(uint32_t(r10), 2);
  // subf r9,r11,r5
  r9 = r5 - r11;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r10,r9,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x8247862c
  if (cr6.lt) goto loc_8247862C;
  // mr r9,r11
  r9 = r11;
loc_8247862C:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r6
  r10 = r6 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247864c
  if (!cr6.lt) goto loc_8247864C;
  // mr r11,r10
  r11 = r10;
loc_8247864C:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_82478660: lifted from generated/ by lift.py.
void f82478660(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478660 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r7,r10,r8
  r7 = r10 + r8;
  // blt cr6,0x824786a8
  if (cr6.lt) goto loc_824786A8;
  // mr r9,r6
  r9 = r6;
loc_824786A8:
  // rlwinm r8,r9,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r8,r8,127
  r8 = uint64_t(int64_t(int64_t(r8) + 127));
  // rlwinm r8,r8,25,7,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x824786d4
  if (cr6.eq) goto loc_824786D4;
loc_824786C0:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // blt cr6,0x824786c0
  if (cr6.lt) goto loc_824786C0;
loc_824786D4:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
loc_824786F4:
  // lfs f12,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r7)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x824786f4
  if (!cr6.eq) goto loc_824786F4;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r8,r10,2
  r8 = __builtin_rotateleft32(uint32_t(r10), 2);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r9,r11
  r11 = r11 - r9;
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // divwu r11,r11,r8
  r11 = uint32_t(uint32_t(r8) ? uint32_t(r11) / uint32_t(r8) : 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r9,r11
  r9 = r11;
  // blt cr6,0x82478744
  if (cr6.lt) goto loc_82478744;
  // mr r9,r10
  r9 = r10;
loc_82478744:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r7
  r10 = r7 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82478764
  if (!cr6.lt) goto loc_82478764;
  // mr r11,r10
  r11 = r10;
loc_82478764:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82478770: lifted from generated/ by lift.py.
void f82478770(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478770 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r8,r10,r8
  r8 = r10 + r8;
  // blt cr6,0x824787b8
  if (cr6.lt) goto loc_824787B8;
  // mr r9,r6
  r9 = r6;
loc_824787B8:
  // rlwinm r7,r9,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x824787e4
  if (cr6.eq) goto loc_824787E4;
loc_824787D0:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x824787d0
  if (cr6.lt) goto loc_824787D0;
loc_824787E4:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
loc_82478804:
  // lfs f12,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,1024(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 1024, temp.u32);
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bne cr6,0x82478804
  if (!cr6.eq) goto loc_82478804;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r10,2
  r7 = __builtin_rotateleft32(uint32_t(r10), 2);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r9,r11
  r11 = r11 - r9;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r9,r11
  r9 = r11;
  // blt cr6,0x82478860
  if (cr6.lt) goto loc_82478860;
  // mr r9,r10
  r9 = r10;
loc_82478860:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r8
  r10 = r8 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82478880
  if (!cr6.lt) goto loc_82478880;
  // mr r11,r10
  r11 = r10;
loc_82478880:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82478890: lifted from generated/ by lift.py.
void f82478890(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478890 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r8
  r10 = r10 + r8;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // mr r8,r9
  r8 = r9;
  // blt cr6,0x824788dc
  if (cr6.lt) goto loc_824788DC;
  // mr r8,r6
  r8 = r6;
loc_824788DC:
  // rlwinm r7,r8,4,0,27
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 4) & 0xFFFFFFF0;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82478908
  if (cr6.eq) goto loc_82478908;
loc_824788F4:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // blt cr6,0x824788f4
  if (cr6.lt) goto loc_824788F4;
loc_82478908:
  // extsw r9,r6
  r9 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
loc_82478928:
  // lfs f12,12(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 12);
  f12 = double(temp.f32);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,3072(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 3072, temp.u32);
  // lfs f12,8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8);
  f12 = double(temp.f32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,2048(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 2048, temp.u32);
  // lfs f12,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,1024(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne cr6,0x82478928
  if (!cr6.eq) goto loc_82478928;
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r9,2
  r7 = __builtin_rotateleft32(uint32_t(r9), 2);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bge cr6,0x82478998
  if (!cr6.lt) goto loc_82478998;
  // mr r9,r11
  r9 = r11;
loc_82478998:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x824789b8
  if (!cr6.lt) goto loc_824789B8;
  // mr r11,r10
  r11 = r10;
loc_824789B8:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_824789C8: lifted from generated/ by lift.py.
void f824789C8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824789C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r10,r10,r9
  r10 = r10 + r9;
  // blt cr6,0x82478a10
  if (cr6.lt) goto loc_82478A10;
  // mr r8,r6
  r8 = r6;
loc_82478A10:
  // rlwinm r7,r8,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // add r7,r8,r7
  r7 = r8 + r7;
  // rlwinm r7,r7,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 3) & 0xFFFFFFF8;
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82478a44
  if (cr6.eq) goto loc_82478A44;
loc_82478A30:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // blt cr6,0x82478a30
  if (cr6.lt) goto loc_82478A30;
loc_82478A44:
  // extsw r9,r6
  r9 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f13,f12
  f13 = double(float(f13 / f12));
loc_82478A64:
  // lfs f12,20(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f12 = double(temp.f32);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,5120(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 5120, temp.u32);
  // lfs f12,16(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 16);
  f12 = double(temp.f32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4096(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 4096, temp.u32);
  // lfs f12,12(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 12);
  f12 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,3072(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 3072, temp.u32);
  // lfs f12,8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8);
  f12 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,2048(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 2048, temp.u32);
  // lfs f12,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,1024(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // bne cr6,0x82478a64
  if (!cr6.eq) goto loc_82478A64;
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r9,2
  r7 = __builtin_rotateleft32(uint32_t(r9), 2);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bge cr6,0x82478aec
  if (!cr6.lt) goto loc_82478AEC;
  // mr r9,r11
  r9 = r11;
loc_82478AEC:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82478b0c
  if (!cr6.lt) goto loc_82478B0C;
  // mr r11,r10
  r11 = r10;
loc_82478B0C:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82478B18: lifted from generated/ by lift.py.
void f82478B18(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478B18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // std r30,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r7,13(r3)
  r7 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r9
  r8 = r9 - r11;
  // lwz r5,24(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r7,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r11))));
  // lwz r4,0(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r9,r10,r5
  r9 = r5 - r10;
  // rlwinm r5,r11,1,0,30
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r5,r5,r4
  r5 = r5 + r4;
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // add r6,r11,r6
  r6 = r11 + r6;
  // mr r4,r8
  r4 = r8;
  // blt cr6,0x82478b6c
  if (cr6.lt) goto loc_82478B6C;
  // mr r4,r9
  r4 = r9;
loc_82478B6C:
  // extsw r11,r9
  r11 = uint64_t(int64_t(int32_t(r9)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r31,r7,1,0,30
  r31 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 1) & 0xFFFFFFFE;
  // std r11,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, r11);
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f12,-64(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -11936);
  f13 = double(temp.f32);
loc_82478B98:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmpwi cr6,r7,4
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 4, so);
  // blt cr6,0x82478c60
  if (cr6.lt) goto loc_82478C60;
  // addi r11,r7,-4
  r11 = uint64_t(int64_t(int64_t(r7) + -4));
  // addi r10,r6,2048
  r10 = uint64_t(int64_t(int64_t(r6) + 2048));
  // rlwinm r9,r11,30,2,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // addi r11,r5,4
  r11 = uint64_t(int64_t(int64_t(r5) + 4));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r8,r9,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
loc_82478BBC:
  // lhz r30,-4(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + -4);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // extsh r30,r30
  r30 = uint64_t(int64_t(int16_t(r30)));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // std r30,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, r30);
  // lfd f11,-64(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,-2048(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + -2048, temp.u32);
  // lhz r30,-2(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + -2);
  // extsh r30,r30
  r30 = uint64_t(int64_t(int16_t(r30)));
  // std r30,-56(r1)
  REX_STORE_U64(uint32_t(r1) + -56, r30);
  // lfd f11,-56(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,-1024(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + -1024, temp.u32);
  // lhz r30,0(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 0);
  // extsh r30,r30
  r30 = uint64_t(int64_t(int16_t(r30)));
  // std r30,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r30);
  // lfd f11,-48(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // lhz r30,2(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 2);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // extsh r30,r30
  r30 = uint64_t(int64_t(int16_t(r30)));
  // std r30,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, r30);
  // lfd f11,-40(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // addi r10,r10,4096
  r10 = uint64_t(int64_t(int64_t(r10) + 4096));
  // bne cr6,0x82478bbc
  if (!cr6.eq) goto loc_82478BBC;
loc_82478C60:
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // bge cr6,0x82478cb4
  if (!cr6.lt) goto loc_82478CB4;
  // rlwinm r10,r8,10,0,21
  r10 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 10) & 0xFFFFFC00;
  // rlwinm r11,r8,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r9,r10,r6
  r9 = r10 + r6;
  // add r10,r11,r5
  r10 = r11 + r5;
  // subf r11,r8,r7
  r11 = r7 - r8;
loc_82478C7C:
  // lhz r8,0(r10)
  r8 = REX_LOAD_U16(uint32_t(r10) + 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // std r8,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r8);
  // lfd f11,-32(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,1024
  r9 = uint64_t(int64_t(int64_t(r9) + 1024));
  // bne cr6,0x82478c7c
  if (!cr6.eq) goto loc_82478C7C;
loc_82478CB4:
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // fadds f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 + f0));
  // add r5,r31,r5
  r5 = r31 + r5;
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x82478b98
  if (!cr6.eq) goto loc_82478B98;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // subf r9,r11,r5
  r9 = r5 - r11;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r10,r9,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82478cf8
  if (cr6.lt) goto loc_82478CF8;
  // mr r9,r11
  r9 = r11;
loc_82478CF8:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r6
  r10 = r6 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82478d18
  if (!cr6.lt) goto loc_82478D18;
  // mr r11,r10
  r11 = r10;
loc_82478D18:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // ld r30,-16(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_82478D30: lifted from generated/ by lift.py.
void f82478D30(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478D30 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r7,r10,r8
  r7 = r10 + r8;
  // blt cr6,0x82478d78
  if (cr6.lt) goto loc_82478D78;
  // mr r9,r6
  r9 = r6;
loc_82478D78:
  // rlwinm r8,r9,1,0,30
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r8,r8,127
  r8 = uint64_t(int64_t(int64_t(r8) + 127));
  // rlwinm r8,r8,25,7,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x82478da4
  if (cr6.eq) goto loc_82478DA4;
loc_82478D90:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // blt cr6,0x82478d90
  if (cr6.lt) goto loc_82478D90;
loc_82478DA4:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -11936);
  f13 = double(temp.f32);
loc_82478DCC:
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f11,-16(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82478dcc
  if (!cr6.eq) goto loc_82478DCC;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r8,r10,1
  r8 = __builtin_rotateleft32(uint32_t(r10), 1);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r9,r11
  r11 = r11 - r9;
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // divwu r11,r11,r8
  r11 = uint32_t(uint32_t(r8) ? uint32_t(r11) / uint32_t(r8) : 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r9,r11
  r9 = r11;
  // blt cr6,0x82478e34
  if (cr6.lt) goto loc_82478E34;
  // mr r9,r10
  r9 = r10;
loc_82478E34:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r7
  r10 = r7 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82478e54
  if (!cr6.lt) goto loc_82478E54;
  // mr r11,r10
  r11 = r10;
loc_82478E54:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82478FB8: lifted from generated/ by lift.py.
void f82478FB8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82478FB8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r9,r10,r9
  r9 = r10 + r9;
  // blt cr6,0x82479000
  if (cr6.lt) goto loc_82479000;
  // mr r8,r6
  r8 = r6;
loc_82479000:
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x8247902c
  if (cr6.eq) goto loc_8247902C;
loc_82479018:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x82479018
  if (cr6.lt) goto loc_82479018;
loc_8247902C:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // lfd f12,-32(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -11936);
  f13 = double(temp.f32);
loc_82479054:
  // lhz r10,6(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 6);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lfd f11,-32(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,3072(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 3072, temp.u32);
  // lhz r10,4(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 4);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r10);
  // lfd f11,-24(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,2048(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 2048, temp.u32);
  // lhz r10,2(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 2);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 1024, temp.u32);
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r10);
  // lfd f11,-8(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82479054
  if (!cr6.eq) goto loc_82479054;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r10,1
  r7 = __builtin_rotateleft32(uint32_t(r10), 1);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r8,r11
  r8 = r11;
  // blt cr6,0x82479128
  if (cr6.lt) goto loc_82479128;
  // mr r8,r10
  r8 = r10;
loc_82479128:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r8,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r8));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479148
  if (!cr6.lt) goto loc_82479148;
  // mr r11,r10
  r11 = r10;
loc_82479148:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479158: lifted from generated/ by lift.py.
void f82479158(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479158 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r8
  r10 = r10 + r8;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // mr r8,r9
  r8 = r9;
  // blt cr6,0x824791a4
  if (cr6.lt) goto loc_824791A4;
  // mr r8,r6
  r8 = r6;
loc_824791A4:
  // rlwinm r7,r8,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // add r7,r8,r7
  r7 = r8 + r7;
  // rlwinm r7,r7,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x824791d8
  if (cr6.eq) goto loc_824791D8;
loc_824791C4:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // blt cr6,0x824791c4
  if (cr6.lt) goto loc_824791C4;
loc_824791D8:
  // extsw r9,r6
  r9 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // lfd f12,-48(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -11936);
  f13 = double(temp.f32);
loc_82479200:
  // lhz r9,10(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 10);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lfd f11,-48(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,5120(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 5120, temp.u32);
  // lhz r9,8(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 8);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, r9);
  // lfd f11,-40(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,4096(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 4096, temp.u32);
  // lhz r9,6(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 6);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r9);
  // lfd f11,-32(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,3072(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 3072, temp.u32);
  // lhz r9,4(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 4);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r9);
  // lfd f11,-24(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,2048(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 2048, temp.u32);
  // lhz r9,2(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 2);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // lhz r9,0(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r9);
  // lfd f11,-8(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82479200
  if (!cr6.eq) goto loc_82479200;
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r9,1
  r7 = __builtin_rotateleft32(uint32_t(r9), 1);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bge cr6,0x82479318
  if (!cr6.lt) goto loc_82479318;
  // mr r9,r11
  r9 = r11;
loc_82479318:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479338
  if (!cr6.lt) goto loc_82479338;
  // mr r11,r10
  r11 = r10;
loc_82479338:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479348: lifted from generated/ by lift.py.
void f82479348(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479348 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lbz r7,13(r3)
  r7 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // mullw r6,r7,r11
  r6 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r11))));
  // lwz r8,24(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r4,0(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r5,20(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r8,r10,r8
  r8 = r8 - r10;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r6,r6,r4
  r6 = r6 + r4;
  // cmpw cr6,r9,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r8), so);
  // add r5,r11,r5
  r5 = r11 + r5;
  // mr r4,r9
  r4 = r9;
  // blt cr6,0x82479394
  if (cr6.lt) goto loc_82479394;
  // mr r4,r8
  r4 = r8;
loc_82479394:
  // extsw r11,r8
  r11 = uint64_t(int64_t(int32_t(r8)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // std r11,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r11);
  // lis r11,-32252
  r11 = uint64_t(int64_t(-2113667072));
  // lfd f12,-48(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,-18448(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -18448);
  f13 = double(temp.f32);
  // lfs f12,-32728(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -32728);
  f12 = double(temp.f32);
loc_824793C4:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpwi cr6,r7,4
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 4, so);
  // blt cr6,0x8247948c
  if (cr6.lt) goto loc_8247948C;
  // addi r10,r7,-4
  r10 = uint64_t(int64_t(int64_t(r7) + -4));
  // addi r11,r6,1
  r11 = uint64_t(int64_t(int64_t(r6) + 1));
  // rlwinm r9,r10,30,2,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // addi r10,r5,2048
  r10 = uint64_t(int64_t(int64_t(r5) + 2048));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r8,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
loc_824793E8:
  // lbz r31,-1(r11)
  r31 = REX_LOAD_U8(uint32_t(r11) + -1);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // std r31,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r31);
  // lfd f10,-48(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,-2048(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + -2048, temp.u32);
  // lbz r31,0(r11)
  r31 = REX_LOAD_U8(uint32_t(r11) + 0);
  // std r31,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, r31);
  // lfd f10,-40(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,-1024(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + -1024, temp.u32);
  // lbz r31,1(r11)
  r31 = REX_LOAD_U8(uint32_t(r11) + 1);
  // std r31,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r31);
  // lfd f10,-32(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // lbz r31,2(r11)
  r31 = REX_LOAD_U8(uint32_t(r11) + 2);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // std r31,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r31);
  // lfd f10,-24(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,1024(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // addi r10,r10,4096
  r10 = uint64_t(int64_t(int64_t(r10) + 4096));
  // bne cr6,0x824793e8
  if (!cr6.eq) goto loc_824793E8;
loc_8247948C:
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // bge cr6,0x824794d0
  if (!cr6.lt) goto loc_824794D0;
  // rlwinm r11,r9,10,0,21
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 10) & 0xFFFFFC00;
  // add r11,r11,r5
  r11 = r11 + r5;
loc_8247949C:
  // lbzx r10,r9,r6
  r10 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r6));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f10,-16(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,1024
  r11 = uint64_t(int64_t(int64_t(r11) + 1024));
  // blt cr6,0x8247949c
  if (cr6.lt) goto loc_8247949C;
loc_824794D0:
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // fadds f0,f11,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f11 + f0));
  // add r6,r6,r7
  r6 = r6 + r7;
  // addi r5,r5,4
  r5 = uint64_t(int64_t(int64_t(r5) + 4));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x824793c4
  if (!cr6.eq) goto loc_824793C4;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r11,r6
  r9 = r6 - r11;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r10,r9,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82479510
  if (cr6.lt) goto loc_82479510;
  // mr r9,r11
  r9 = r11;
loc_82479510:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r5
  r10 = r5 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479530
  if (!cr6.lt) goto loc_82479530;
  // mr r11,r10
  r11 = r10;
loc_82479530:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_82479540: lifted from generated/ by lift.py.
void f82479540(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479540 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r7,r10,r8
  r7 = r10 + r8;
  // blt cr6,0x82479584
  if (cr6.lt) goto loc_82479584;
  // mr r9,r6
  r9 = r6;
loc_82479584:
  // addi r8,r9,127
  r8 = uint64_t(int64_t(int64_t(r9) + 127));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // rlwinm r8,r8,25,7,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x824795ac
  if (cr6.eq) goto loc_824795AC;
loc_82479598:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // blt cr6,0x82479598
  if (cr6.lt) goto loc_82479598;
loc_824795AC:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,-18448(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -18448);
  f13 = double(temp.f32);
  // lfs f12,-32728(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -32728);
  f12 = double(temp.f32);
loc_824795DC:
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f10,-16(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r7)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // bne cr6,0x824795dc
  if (!cr6.eq) goto loc_824795DC;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r10,r10,r11
  r10 = r11 - r10;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // divwu r10,r10,r9
  r10 = uint32_t(uint32_t(r9) ? uint32_t(r10) / uint32_t(r9) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82479640
  if (cr6.lt) goto loc_82479640;
  // mr r9,r11
  r9 = r11;
loc_82479640:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r7
  r10 = r7 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479660
  if (!cr6.lt) goto loc_82479660;
  // mr r11,r10
  r11 = r10;
loc_82479660:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479670: lifted from generated/ by lift.py.
void f82479670(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479670 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // subf r6,r10,r6
  r6 = r6 - r10;
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // add r8,r10,r8
  r8 = r10 + r8;
  // blt cr6,0x824796b4
  if (cr6.lt) goto loc_824796B4;
  // mr r9,r6
  r9 = r6;
loc_824796B4:
  // rlwinm r7,r9,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x824796e0
  if (cr6.eq) goto loc_824796E0;
loc_824796CC:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x824796cc
  if (cr6.lt) goto loc_824796CC;
loc_824796E0:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,-18448(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -18448);
  f13 = double(temp.f32);
  // lfs f12,-32728(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + -32728);
  f12 = double(temp.f32);
loc_82479710:
  // lbz r10,1(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 1);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f10,-16(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,1024(r8)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r8) + 1024, temp.u32);
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // std r10,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r10);
  // lfd f10,-8(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r8)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // bne cr6,0x82479710
  if (!cr6.eq) goto loc_82479710;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r10,r10,r11
  r10 = r11 - r10;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // divwu r10,r10,r9
  r10 = uint32_t(uint32_t(r9) ? uint32_t(r10) / uint32_t(r9) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82479798
  if (cr6.lt) goto loc_82479798;
  // mr r9,r11
  r9 = r11;
loc_82479798:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r8
  r10 = r8 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x824797b8
  if (!cr6.lt) goto loc_824797B8;
  // mr r11,r10
  r11 = r10;
loc_824797B8:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_824797C8: lifted from generated/ by lift.py.
void f824797C8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824797C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // subf r6,r10,r6
  r6 = r6 - r10;
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // add r9,r10,r9
  r9 = r10 + r9;
  // blt cr6,0x8247980c
  if (cr6.lt) goto loc_8247980C;
  // mr r8,r6
  r8 = r6;
loc_8247980C:
  // rlwinm r7,r8,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82479838
  if (cr6.eq) goto loc_82479838;
loc_82479824:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x82479824
  if (cr6.lt) goto loc_82479824;
loc_82479838:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // lfd f12,-32(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,-18448(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -18448);
  f13 = double(temp.f32);
  // lfs f12,-32728(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + -32728);
  f12 = double(temp.f32);
loc_82479868:
  // lbz r10,3(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 3);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lfd f10,-32(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,3072(r9)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r9) + 3072, temp.u32);
  // lbz r10,2(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 2);
  // std r10,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r10);
  // lfd f10,-24(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,2048(r9)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r9) + 2048, temp.u32);
  // lbz r10,1(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 1);
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f10,-16(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,1024(r9)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r9) + 1024, temp.u32);
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // std r10,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r10);
  // lfd f10,-8(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r9)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // bne cr6,0x82479868
  if (!cr6.eq) goto loc_82479868;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r8,13(r3)
  r8 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r10,r10,r11
  r10 = r11 - r10;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // divwu r10,r10,r8
  r10 = uint32_t(uint32_t(r8) ? uint32_t(r10) / uint32_t(r8) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r8,r10
  r8 = r10;
  // blt cr6,0x82479938
  if (cr6.lt) goto loc_82479938;
  // mr r8,r11
  r8 = r11;
loc_82479938:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r8,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r8));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479958
  if (!cr6.lt) goto loc_82479958;
  // mr r11,r10
  r11 = r10;
loc_82479958:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479968: lifted from generated/ by lift.py.
void f82479968(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479968 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // add r10,r10,r8
  r10 = r10 + r8;
  // add r11,r11,r7
  r11 = r11 + r7;
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // mr r8,r9
  r8 = r9;
  // blt cr6,0x824799b0
  if (cr6.lt) goto loc_824799B0;
  // mr r8,r6
  r8 = r6;
loc_824799B0:
  // rlwinm r7,r8,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // add r7,r8,r7
  r7 = r8 + r7;
  // rlwinm r7,r7,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 1) & 0xFFFFFFFE;
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x824799e4
  if (cr6.eq) goto loc_824799E4;
loc_824799D0:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // blt cr6,0x824799d0
  if (cr6.lt) goto loc_824799D0;
loc_824799E4:
  // extsw r9,r6
  r9 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r7,-32255
  r7 = uint64_t(int64_t(-2113863680));
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lis r9,-32252
  r9 = uint64_t(int64_t(-2113667072));
  // lfd f12,-48(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,-18448(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -18448);
  f13 = double(temp.f32);
  // lfs f12,-32728(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + -32728);
  f12 = double(temp.f32);
loc_82479A14:
  // lbz r9,5(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 5);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lfd f10,-48(r1)
  lifted::ScalarFloatMode();
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,5120(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 5120, temp.u32);
  // lbz r9,4(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 4);
  // std r9,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, r9);
  // lfd f10,-40(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,4096(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 4096, temp.u32);
  // lbz r9,3(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 3);
  // std r9,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r9);
  // lfd f10,-32(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,3072(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 3072, temp.u32);
  // lbz r9,2(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 2);
  // std r9,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r9);
  // lfd f10,-24(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,2048(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 2048, temp.u32);
  // lbz r9,1(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 1);
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f10,-16(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,1024(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 6));
  // std r9,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r9);
  // lfd f10,-8(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f10,f10
  f10 = double(float(f10));
  // fsubs f10,f10,f13
  f10 = double(float(f10 - f13));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,0(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // bne cr6,0x82479a14
  if (!cr6.eq) goto loc_82479A14;
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r8,13(r3)
  r8 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r9,r11
  r9 = r11 - r9;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // divwu r9,r9,r8
  r9 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // blt cr6,0x82479b28
  if (cr6.lt) goto loc_82479B28;
  // mr r9,r11
  r9 = r11;
loc_82479B28:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479b48
  if (!cr6.lt) goto loc_82479B48;
  // mr r11,r10
  r11 = r10;
loc_82479B48:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479B58: lifted from generated/ by lift.py.
void f82479B58(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479B58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // std r30,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // mullw r8,r5,r11
  r8 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r4,24(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // subf r11,r10,r4
  r11 = r4 - r10;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r9
  r4 = r9;
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // add r6,r10,r6
  r6 = r10 + r6;
  // rlwinm r10,r8,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r7,r10,r7
  r7 = r10 + r7;
  // blt cr6,0x82479bac
  if (cr6.lt) goto loc_82479BAC;
  // mr r4,r11
  r4 = r11;
loc_82479BAC:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r31,r5,1,0,30
  r31 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 1) & 0xFFFFFFFE;
  // std r11,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r11);
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f12,-32(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -11936);
  f13 = double(temp.f32);
loc_82479BD8:
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // beq cr6,0x82479c2c
  if (cr6.eq) goto loc_82479C2C;
  // mr r8,r6
  r8 = r6;
  // mr r9,r7
  r9 = r7;
  // mr r10,r5
  r10 = r5;
loc_82479BEC:
  // lhz r30,0(r9)
  r30 = REX_LOAD_U16(uint32_t(r9) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // addi r9,r9,2
  r9 = uint64_t(int64_t(int64_t(r9) + 2));
  // rlwinm r11,r30,24,24,31
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 24) & 0xFF;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // rlwimi r11,r30,8,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 8) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // std r11,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r11);
  // lfd f11,-32(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,1024
  r8 = uint64_t(int64_t(int64_t(r8) + 1024));
  // bne cr6,0x82479bec
  if (!cr6.eq) goto loc_82479BEC;
loc_82479C2C:
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // fadds f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 + f0));
  // add r7,r31,r7
  r7 = r31 + r7;
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x82479bd8
  if (!cr6.eq) goto loc_82479BD8;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // subf r9,r11,r7
  r9 = r7 - r11;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r10,r9,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82479c70
  if (cr6.lt) goto loc_82479C70;
  // mr r9,r11
  r9 = r11;
loc_82479C70:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r6
  r10 = r6 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479c90
  if (!cr6.lt) goto loc_82479C90;
  // mr r11,r10
  r11 = r10;
loc_82479C90:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // ld r30,-16(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_82479CA8: lifted from generated/ by lift.py.
void f82479CA8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479CA8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r4,13(r3)
  r4 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lwz r7,24(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r4,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r11))));
  // lwz r6,0(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r5,r10,r7
  r5 = r7 - r10;
  // rlwinm r7,r11,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r9,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r5), so);
  // add r10,r7,r6
  r10 = r7 + r6;
  // add r8,r11,r8
  r8 = r11 + r8;
  // blt cr6,0x82479cf0
  if (cr6.lt) goto loc_82479CF0;
  // mr r9,r5
  r9 = r5;
loc_82479CF0:
  // rlwinm r7,r9,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82479d1c
  if (cr6.eq) goto loc_82479D1C;
loc_82479D08:
  // rlwinm r6,r11,7,0,24
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0xFFFFFF80;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // blt cr6,0x82479d08
  if (cr6.lt) goto loc_82479D08;
loc_82479D1C:
  // extsw r11,r5
  r11 = uint64_t(int64_t(int32_t(r5)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -11936);
  f13 = double(temp.f32);
loc_82479D44:
  // lhz r7,0(r10)
  r7 = REX_LOAD_U16(uint32_t(r10) + 0);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // rlwinm r11,r7,24,24,31
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwimi r11,r7,8,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lfd f11,-16(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82479d44
  if (!cr6.eq) goto loc_82479D44;
  // lbz r11,13(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r11,1
  r7 = __builtin_rotateleft32(uint32_t(r11), 1);
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r10,r9,r10
  r10 = r10 - r9;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r10,r10,r7
  r10 = uint32_t(uint32_t(r7) ? uint32_t(r10) / uint32_t(r7) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x82479db4
  if (cr6.lt) goto loc_82479DB4;
  // mr r9,r11
  r9 = r11;
loc_82479DB4:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r8
  r10 = r8 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479dd4
  if (!cr6.lt) goto loc_82479DD4;
  // mr r11,r10
  r11 = r10;
loc_82479DD4:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479DE0: lifted from generated/ by lift.py.
void f82479DE0(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479DE0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r4,13(r3)
  r4 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lwz r7,24(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r4,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r11))));
  // lwz r6,0(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r5,r10,r7
  r5 = r7 - r10;
  // rlwinm r7,r11,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r8,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r5), so);
  // add r10,r7,r6
  r10 = r7 + r6;
  // add r9,r11,r9
  r9 = r11 + r9;
  // blt cr6,0x82479e28
  if (cr6.lt) goto loc_82479E28;
  // mr r8,r5
  r8 = r5;
loc_82479E28:
  // rlwinm r7,r8,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82479e54
  if (cr6.eq) goto loc_82479E54;
loc_82479E40:
  // rlwinm r6,r11,7,0,24
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0xFFFFFF80;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // blt cr6,0x82479e40
  if (cr6.lt) goto loc_82479E40;
loc_82479E54:
  // extsw r11,r5
  r11 = uint64_t(int64_t(int32_t(r5)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -11936);
  f13 = double(temp.f32);
loc_82479E7C:
  // lhz r7,2(r10)
  r7 = REX_LOAD_U16(uint32_t(r10) + 2);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // rlwinm r11,r7,24,24,31
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // rlwimi r11,r7,8,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lfd f11,-16(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 1024, temp.u32);
  // lhz r7,0(r10)
  r7 = REX_LOAD_U16(uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // rlwinm r11,r7,24,24,31
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r11,r7,8,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // std r11,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r11);
  // lfd f11,-8(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82479e7c
  if (!cr6.eq) goto loc_82479E7C;
  // lbz r11,13(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r11,1
  r7 = __builtin_rotateleft32(uint32_t(r11), 1);
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r10,r10,r7
  r10 = uint32_t(uint32_t(r7) ? uint32_t(r10) / uint32_t(r7) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r8,r10
  r8 = r10;
  // blt cr6,0x82479f18
  if (cr6.lt) goto loc_82479F18;
  // mr r8,r11
  r8 = r11;
loc_82479F18:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r8,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r8));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82479f38
  if (!cr6.lt) goto loc_82479F38;
  // mr r11,r10
  r11 = r10;
loc_82479F38:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_82479F48: lifted from generated/ by lift.py.
void f82479F48(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82479F48 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r9,r10,r9
  r9 = r10 + r9;
  // blt cr6,0x82479f90
  if (cr6.lt) goto loc_82479F90;
  // mr r8,r6
  r8 = r6;
loc_82479F90:
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x82479fbc
  if (cr6.eq) goto loc_82479FBC;
loc_82479FA8:
  // rlwinm r5,r10,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x82479fa8
  if (cr6.lt) goto loc_82479FA8;
loc_82479FBC:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // lfd f12,-32(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -11936);
  f13 = double(temp.f32);
loc_82479FE4:
  // lhz r7,6(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 6);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // rlwinm r10,r7,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // rlwimi r10,r7,8,16,23
  r10 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r10 & 0xFFFFFFFFFFFF00FF);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r10);
  // lfd f11,-32(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,3072(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 3072, temp.u32);
  // lhz r7,4(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 4);
  // rlwinm r10,r7,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r10,r7,8,16,23
  r10 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r10 & 0xFFFFFFFFFFFF00FF);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r10);
  // lfd f11,-24(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,2048(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 2048, temp.u32);
  // lhz r7,2(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 2);
  // rlwinm r10,r7,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r10,r7,8,16,23
  r10 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r10 & 0xFFFFFFFFFFFF00FF);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 1024, temp.u32);
  // lhz r7,0(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwinm r10,r7,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r10,r7,8,16,23
  r10 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r10 & 0xFFFFFFFFFFFF00FF);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // std r10,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r10);
  // lfd f11,-8(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x82479fe4
  if (!cr6.eq) goto loc_82479FE4;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r10,1
  r7 = __builtin_rotateleft32(uint32_t(r10), 1);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // mr r8,r11
  r8 = r11;
  // blt cr6,0x8247a0d8
  if (cr6.lt) goto loc_8247A0D8;
  // mr r8,r10
  r8 = r10;
loc_8247A0D8:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r8,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r8));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247a0f8
  if (!cr6.lt) goto loc_8247A0F8;
  // mr r11,r10
  r11 = r10;
loc_8247A0F8:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8247A108: lifted from generated/ by lift.py.
void f8247A108(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247A108 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r10,28(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 28);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lwz r6,24(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mullw r11,r5,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r11))));
  // lwz r7,0(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r9,20(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20);
  // subf r6,r10,r6
  r6 = r6 - r10;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r10,r10,r9
  r10 = r10 + r9;
  // blt cr6,0x8247a150
  if (cr6.lt) goto loc_8247A150;
  // mr r8,r6
  r8 = r6;
loc_8247A150:
  // rlwinm r7,r8,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // add r7,r8,r7
  r7 = r8 + r7;
  // rlwinm r7,r7,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x8247a184
  if (cr6.eq) goto loc_8247A184;
loc_8247A170:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // blt cr6,0x8247a170
  if (cr6.lt) goto loc_8247A170;
loc_8247A184:
  // extsw r9,r6
  r9 = uint64_t(int64_t(int32_t(r6)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // lfd f12,-48(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f12,f13,f12
  f12 = double(float(f13 / f12));
  // lfs f13,-11936(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -11936);
  f13 = double(temp.f32);
loc_8247A1AC:
  // lhz r7,10(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 10);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, r9);
  // lfd f11,-48(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,5120(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 5120, temp.u32);
  // lhz r7,8(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 8);
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, r9);
  // lfd f11,-40(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,4096(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 4096, temp.u32);
  // lhz r7,6(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 6);
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, r9);
  // lfd f11,-32(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,3072(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 3072, temp.u32);
  // lhz r7,4(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 4);
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r9);
  // lfd f11,-24(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,2048(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 2048, temp.u32);
  // lhz r7,2(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 2);
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,1024(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // lhz r7,0(r11)
  r7 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // rlwinm r9,r7,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 24) & 0xFF;
  // rlwimi r9,r7,8,16,23
  r9 = (__builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 8) & 0xFF00) | (r9 & 0xFFFFFFFFFFFF00FF);
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // std r9,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r9);
  // lfd f11,-8(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // bne cr6,0x8247a1ac
  if (!cr6.eq) goto loc_8247A1AC;
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r7,r9,1
  r7 = __builtin_rotateleft32(uint32_t(r9), 1);
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // twllei r7,0
  if (int32_t(r7) == 0 || uint32_t(r7) < 0u) lifted::Trap(0);
  // divwu r11,r11,r7
  r11 = uint32_t(uint32_t(r7) ? uint32_t(r11) / uint32_t(r7) : 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bge cr6,0x8247a2f4
  if (!cr6.lt) goto loc_8247A2F4;
  // mr r9,r11
  r9 = r11;
loc_8247A2F4:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247a314
  if (!cr6.lt) goto loc_8247A314;
  // mr r11,r10
  r11 = r10;
loc_8247A314:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8247A320: lifted from generated/ by lift.py.
void f8247A320(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247A320 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,24(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 24);
  // addi r29,r3,52
  r29 = uint64_t(int64_t(int64_t(r3) + 52));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r7,4(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r5,r11,r9
  r5 = r9 - r11;
  // lbz r4,13(r3)
  r4 = REX_LOAD_U8(uint32_t(r3) + 13);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r9,r10,r7
  r9 = r7 - r10;
  // lwz r6,0(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 0);
  // srawi r7,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r7 = uint64_t(int64_t(int32_t(r5) >> 1));
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // mullw r5,r4,r10
  r5 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // addze r10,r7
  temp.s64 = int64_t(r7) + ca;
  ca = temp.u32 < uint32_t(r7);
  r10 = uint64_t(int64_t(temp.s64));
  // rlwinm r7,r5,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // add r30,r11,r8
  r30 = r11 + r8;
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // add r28,r7,r6
  r28 = r7 + r6;
  // mr r26,r9
  r26 = r9;
  // blt cr6,0x8247a380
  if (cr6.lt) goto loc_8247A380;
  // mr r26,r10
  r26 = r10;
loc_8247A380:
  // extsw r11,r10
  r11 = uint64_t(int64_t(int32_t(r10)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r4,4
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 4, so);
  // std r11,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f12,-64(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f9,f13,f12
  f9 = double(float(f13 / f12));
  // lfs f13,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f13 = double(temp.f32);
  // fmuls f8,f9,f13
  f8 = double(float(f9 * f13));
  // blt cr6,0x8247a3dc
  if (cr6.lt) goto loc_8247A3DC;
  // addi r11,r4,-4
  r11 = uint64_t(int64_t(int64_t(r4) + -4));
  // addi r9,r29,8
  r9 = uint64_t(int64_t(int64_t(r29) + 8));
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_8247A3CC:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8247a3cc
  if (!cr6.eq) goto loc_8247A3CC;
loc_8247A3DC:
  // cmplw cr6,r10,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r4), so);
  // bge cr6,0x8247a400
  if (!cr6.lt) goto loc_8247A400;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf r11,r10,r4
  r11 = r4 - r10;
  // add r10,r9,r29
  r10 = r9 + r29;
loc_8247A3F0:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8247a3f0
  if (!cr6.eq) goto loc_8247A3F0;
loc_8247A400:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // rlwinm r27,r4,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // subf r31,r29,r28
  r31 = r28 - r29;
  // lfs f12,7140(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f12 = double(temp.f32);
loc_8247A410:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmpwi cr6,r4,4
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 4, so);
  // blt cr6,0x8247a4ec
  if (cr6.lt) goto loc_8247A4EC;
  // addi r11,r4,-4
  r11 = uint64_t(int64_t(int64_t(r4) + -4));
  // fadds f13,f9,f0
  lifted::ScalarFloatMode();
  f13 = double(float(f9 + f0));
  // addi r9,r28,12
  r9 = uint64_t(int64_t(int64_t(r28) + 12));
  // rlwinm r10,r11,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // addi r11,r30,1024
  r11 = uint64_t(int64_t(int64_t(r30) + 1024));
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r10,r29,4
  r10 = uint64_t(int64_t(int64_t(r29) + 4));
  // rlwinm r5,r8,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
loc_8247A43C:
  // lfs f11,-12(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -12);
  f11 = double(temp.f32);
  // addi r7,r10,4
  r7 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f10,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f10 = double(temp.f32);
  // fmuls f7,f13,f11
  f7 = double(float(f13 * f11));
  // stfs f11,-4(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // fadds f11,f11,f10
  f11 = double(float(f11 + f10));
  // addi r6,r10,8
  r6 = uint64_t(int64_t(int64_t(r10) + 8));
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,-1024(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -1024, temp.u32);
  // stfs f7,-1020(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + -1020, temp.u32);
  // lfsx f11,r10,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  f11 = double(temp.f32);
  // lfs f10,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f11,4(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lfs f11,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f11 = double(temp.f32);
  // lfs f10,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,1024(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 1024, temp.u32);
  // stfs f11,1028(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 1028, temp.u32);
  // lfs f11,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f11 = double(temp.f32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // lfs f10,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,2048(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 2048, temp.u32);
  // stfs f11,2052(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 2052, temp.u32);
  // addi r11,r11,4096
  r11 = uint64_t(int64_t(int64_t(r11) + 4096));
  // bne cr6,0x8247a43c
  if (!cr6.eq) goto loc_8247A43C;
loc_8247A4EC:
  // cmplw cr6,r5,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r4), so);
  // bge cr6,0x8247a544
  if (!cr6.lt) goto loc_8247A544;
  // rlwinm r10,r5,10,0,21
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 10) & 0xFFFFFC00;
  // fadds f10,f9,f0
  lifted::ScalarFloatMode();
  f10 = double(float(f9 + f0));
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r30
  r10 = r10 + r30;
  // add r11,r11,r29
  r11 = r11 + r29;
  // subf r9,r5,r4
  r9 = r4 - r5;
loc_8247A50C:
  // lfsx f13,r31,r11
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  f13 = double(temp.f32);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // fadds f11,f13,f11
  f11 = double(float(f13 + f11));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fmuls f13,f10,f13
  f13 = double(float(f10 * f13));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // stfs f13,4(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // addi r10,r10,1024
  r10 = uint64_t(int64_t(int64_t(r10) + 1024));
  // bne cr6,0x8247a50c
  if (!cr6.eq) goto loc_8247A50C;
loc_8247A544:
  // addi r26,r26,-1
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  // fadds f0,f8,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f8 + f0));
  // add r28,r27,r28
  r28 = r27 + r28;
  // add r31,r27,r31
  r31 = r27 + r31;
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bne cr6,0x8247a410
  if (!cr6.eq) goto loc_8247A410;
  // lbz r10,13(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 13);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rotlwi r10,r10,2
  r10 = __builtin_rotateleft32(uint32_t(r10), 2);
  // subf r9,r11,r28
  r9 = r28 - r11;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r10,r9,r10
  r10 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x8247a58c
  if (cr6.lt) goto loc_8247A58C;
  // mr r9,r11
  r9 = r11;
loc_8247A58C:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r30
  r10 = r30 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247a5ac
  if (!cr6.lt) goto loc_8247A5AC;
  // mr r11,r10
  r11 = r10;
loc_8247A5AC:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r4,4
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 4, so);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blt cr6,0x8247a5e4
  if (cr6.lt) goto loc_8247A5E4;
  // addi r11,r4,-4
  r11 = uint64_t(int64_t(int64_t(r4) + -4));
  // addi r9,r29,8
  r9 = uint64_t(int64_t(int64_t(r29) + 8));
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_8247A5D4:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8247a5d4
  if (!cr6.eq) goto loc_8247A5D4;
loc_8247A5E4:
  // cmplw cr6,r10,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r4), so);
  // bge cr6,0x8247a608
  if (!cr6.lt) goto loc_8247A608;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // subf r11,r10,r4
  r11 = r4 - r10;
  // add r10,r9,r29
  r10 = r9 + r29;
loc_8247A5F8:
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8247a5f8
  if (!cr6.eq) goto loc_8247A5F8;
loc_8247A608:
  // b 0x825a6ed0
  // (left out: dead in C++)
  return;
}

// sub_8247A610: lifted from generated/ by lift.py.
void f8247A610(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247A610 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,24(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 24);
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r4,r11,r9
  r4 = r9 - r11;
  // lbz r6,13(r3)
  r6 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r10,r8
  r9 = r8 - r10;
  // lwz r5,0(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 0);
  // srawi r8,r4,1
  ca = (int32_t(r4) < 0) & ((uint32_t(r4) & 0x1) != 0);
  r8 = uint64_t(int64_t(int32_t(r4) >> 1));
  // lwz r7,20(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 20);
  // mullw r6,r6,r10
  r6 = uint64_t(int64_t(int64_t(int32_t(r6)) * int64_t(int32_t(r10))));
  // addze r10,r8
  temp.s64 = int64_t(r8) + ca;
  ca = temp.u32 < uint32_t(r8);
  r10 = uint64_t(int64_t(temp.s64));
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r6,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // add r8,r8,r7
  r8 = r8 + r7;
  // add r11,r6,r5
  r11 = r6 + r5;
  // blt cr6,0x8247a660
  if (cr6.lt) goto loc_8247A660;
  // mr r9,r10
  r9 = r10;
loc_8247A660:
  // extsw r6,r10
  r6 = uint64_t(int64_t(int32_t(r10)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r7,r9,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r7,r7,127
  r7 = uint64_t(int64_t(int64_t(r7) + 127));
  // std r6,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r6);
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // rlwinm r7,r7,25,7,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f11,f13,f12
  f11 = double(float(f13 / f12));
  // lfs f13,8680(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 8680);
  f13 = double(temp.f32);
  // fmuls f10,f11,f13
  f10 = double(float(f11 * f13));
  // beq cr6,0x8247a6b8
  if (cr6.eq) goto loc_8247A6B8;
loc_8247A6A4:
  // rlwinm r6,r10,7,0,24
  r6 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r7), so);
  // blt cr6,0x8247a6a4
  if (cr6.lt) goto loc_8247A6A4;
loc_8247A6B8:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f9,7140(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7140);
  f9 = double(temp.f32);
loc_8247A6C0:
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fadds f8,f11,f0
  f8 = double(float(f11 + f0));
  // lfs f12,52(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 52);
  f12 = double(temp.f32);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // fadds f12,f13,f12
  f12 = double(float(f13 + f12));
  // stfs f13,52(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // fmuls f13,f8,f13
  f13 = double(float(f8 * f13));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fadds f0,f10,f0
  f0 = double(float(f10 + f0));
  // fmuls f12,f12,f9
  f12 = double(float(f12 * f9));
  // stfs f12,0(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f13,4(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // addi r8,r8,8
  r8 = uint64_t(int64_t(int64_t(r8) + 8));
  // bne cr6,0x8247a6c0
  if (!cr6.eq) goto loc_8247A6C0;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r9,13(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r10,r10,r11
  r10 = r11 - r10;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // rotlwi r9,r9,2
  r9 = __builtin_rotateleft32(uint32_t(r9), 2);
  // divwu r10,r10,r9
  r10 = uint32_t(uint32_t(r9) ? uint32_t(r10) / uint32_t(r9) : 0);
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r9,r10
  r9 = r10;
  // blt cr6,0x8247a72c
  if (cr6.lt) goto loc_8247A72C;
  // mr r9,r11
  r9 = r11;
loc_8247A72C:
  // lwz r10,20(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r10,r8
  r10 = r8 - r10;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247a74c
  if (!cr6.lt) goto loc_8247A74C;
  // mr r11,r10
  r11 = r10;
loc_8247A74C:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8247A758: lifted from generated/ by lift.py.
void f8247A758(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247A758 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r9,24(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 24);
  // addi r7,r3,52
  r7 = uint64_t(int64_t(int64_t(r3) + 52));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r8,4(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r9,r11,r9
  r9 = r9 - r11;
  // lbz r5,13(r3)
  r5 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r8,r10,r8
  r8 = r8 - r10;
  // lwz r4,0(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mullw r10,r5,r10
  r10 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r10))));
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // srawi r9,r9,1
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x1) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 1));
  // rlwinm r5,r10,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addze r9,r9
  temp.s64 = int64_t(r9) + ca;
  ca = temp.u32 < uint32_t(r9);
  r9 = uint64_t(int64_t(temp.s64));
  // add r11,r5,r4
  r11 = r5 + r4;
  // add r10,r10,r6
  r10 = r10 + r6;
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // blt cr6,0x8247a7ac
  if (cr6.lt) goto loc_8247A7AC;
  // mr r8,r9
  r8 = r9;
loc_8247A7AC:
  // extsw r5,r9
  r5 = uint64_t(int64_t(int32_t(r9)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r6,r8,3,0,28
  r6 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r6,r6,127
  r6 = uint64_t(int64_t(int64_t(r6) + 127));
  // std r5,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r5);
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // rlwinm r6,r6,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f9,f13,f12
  f9 = double(float(f13 / f12));
  // lfs f13,8680(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8680);
  f13 = double(temp.f32);
  // fmuls f8,f9,f13
  f8 = double(float(f9 * f13));
  // beq cr6,0x8247a804
  if (cr6.eq) goto loc_8247A804;
loc_8247A7F0:
  // rlwinm r5,r9,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // blt cr6,0x8247a7f0
  if (cr6.lt) goto loc_8247A7F0;
loc_8247A804:
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f11,7140(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 7140);
  f11 = double(temp.f32);
loc_8247A80C:
  // lfs f13,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // fadds f12,f9,f0
  f12 = double(float(f9 + f0));
  // lfs f10,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f10 = double(temp.f32);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // fadds f10,f13,f10
  f10 = double(float(f13 + f10));
  // stfs f13,4(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f11
  f10 = double(float(f10 * f11));
  // stfs f10,1024(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 1024, temp.u32);
  // stfs f13,1028(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 1028, temp.u32);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // lfs f10,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f10 = double(temp.f32);
  // fadds f10,f13,f10
  f10 = double(float(f13 + f10));
  // stfs f13,0(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // fmuls f12,f10,f0
  f12 = double(float(f10 * f0));
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // fmuls f12,f12,f11
  f12 = double(float(f12 * f11));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // stfs f13,4(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bne cr6,0x8247a80c
  if (!cr6.eq) goto loc_8247A80C;
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r8,13(r3)
  r8 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r9,r11
  r9 = r11 - r9;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // rotlwi r8,r8,2
  r8 = __builtin_rotateleft32(uint32_t(r8), 2);
  // divwu r9,r9,r8
  r9 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // blt cr6,0x8247a898
  if (cr6.lt) goto loc_8247A898;
  // mr r9,r11
  r9 = r11;
loc_8247A898:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r10,r8,r10
  r10 = r10 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x8247a8b8
  if (!cr6.lt) goto loc_8247A8B8;
  // mr r11,r10
  r11 = r10;
loc_8247A8B8:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8247A8C8: lifted from generated/ by lift.py.
void f8247A8C8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247A8C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,24(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 24);
  // addi r9,r3,52
  r9 = uint64_t(int64_t(int64_t(r3) + 52));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r7,4(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r8,r11,r8
  r8 = r8 - r11;
  // lbz r4,13(r3)
  r4 = REX_LOAD_U8(uint32_t(r3) + 13);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r7,r10,r7
  r7 = r7 - r10;
  // lwz r5,0(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mullw r10,r4,r10
  r10 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // srawi r8,r8,1
  ca = (int32_t(r8) < 0) & ((uint32_t(r8) & 0x1) != 0);
  r8 = uint64_t(int64_t(int32_t(r8) >> 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addze r8,r8
  temp.s64 = int64_t(r8) + ca;
  ca = temp.u32 < uint32_t(r8);
  r8 = uint64_t(int64_t(temp.s64));
  // add r11,r11,r6
  r11 = r11 + r6;
  // add r10,r10,r5
  r10 = r10 + r5;
  // cmpw cr6,r7,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r8), so);
  // blt cr6,0x8247a91c
  if (cr6.lt) goto loc_8247A91C;
  // mr r7,r8
  r7 = r8;
loc_8247A91C:
  // extsw r5,r8
  r5 = uint64_t(int64_t(int32_t(r8)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // rlwinm r6,r7,4,0,27
  r6 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 4) & 0xFFFFFFF0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r6,r6,127
  r6 = uint64_t(int64_t(int64_t(r6) + 127));
  // std r5,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r5);
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // rlwinm r6,r6,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f9,f13,f12
  f9 = double(float(f13 / f12));
  // lfs f13,8680(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8680);
  f13 = double(temp.f32);
  // fmuls f8,f9,f13
  f8 = double(float(f9 * f13));
  // beq cr6,0x8247a974
  if (cr6.eq) goto loc_8247A974;
loc_8247A960:
  // rlwinm r5,r8,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 7) & 0xFFFFFF80;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // cmplw cr6,r8,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r6), so);
  // blt cr6,0x8247a960
  if (cr6.lt) goto loc_8247A960;
loc_8247A974:
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f12,7140(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 7140);
  f12 = double(temp.f32);
loc_8247A97C:
  // lfs f11,12(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 12);
  f11 = double(temp.f32);
  // fadds f13,f9,f0
  f13 = double(float(f9 + f0));
  // lfs f10,12(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 12);
  f10 = double(temp.f32);
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,12(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 12, temp.u32);
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,3072(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 3072, temp.u32);
  // stfs f11,3076(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 3076, temp.u32);
  // lfs f11,8(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8);
  f11 = double(temp.f32);
  // lfs f10,8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,8(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 8, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,2048(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 2048, temp.u32);
  // stfs f11,2052(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 2052, temp.u32);
  // lfs f11,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f11 = double(temp.f32);
  // lfs f10,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,4(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,1024(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 1024, temp.u32);
  // stfs f11,1028(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 1028, temp.u32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // lfs f10,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f10 = double(temp.f32);
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,0(r9)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fmuls f11,f10,f0
  f11 = double(float(f10 * f0));
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne cr6,0x8247a97c
  if (!cr6.eq) goto loc_8247A97C;
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r8,13(r3)
  r8 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r9,r10
  r9 = r10 - r9;
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // rotlwi r8,r8,2
  r8 = __builtin_rotateleft32(uint32_t(r8), 2);
  // divwu r9,r9,r8
  r9 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // blt cr6,0x8247aa50
  if (cr6.lt) goto loc_8247AA50;
  // mr r9,r10
  r9 = r10;
loc_8247AA50:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r10,24(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8247aa70
  if (cr6.lt) goto loc_8247AA70;
  // mr r11,r10
  r11 = r10;
loc_8247AA70:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8247AA80: lifted from generated/ by lift.py.
void f8247AA80(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8247AA80 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f0 = double(temp.f32);
  // lwz r8,24(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 24);
  // addi r10,r3,52
  r10 = uint64_t(int64_t(int64_t(r3) + 52));
  // lwz r9,8(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r7,4(r3)
  r7 = REX_LOAD_U32(uint32_t(r3) + 4);
  // subf r31,r11,r8
  r31 = r8 - r11;
  // lbz r4,13(r3)
  r4 = REX_LOAD_U8(uint32_t(r3) + 13);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r8,r9,r7
  r8 = r7 - r9;
  // lwz r5,0(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mullw r9,r4,r9
  r9 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r9))));
  // lwz r6,20(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 20);
  // srawi r7,r31,1
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x1) != 0);
  r7 = uint64_t(int64_t(int32_t(r31) >> 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // addze r7,r7
  temp.s64 = int64_t(r7) + ca;
  ca = temp.u32 < uint32_t(r7);
  r7 = uint64_t(int64_t(temp.s64));
  // add r11,r11,r6
  r11 = r11 + r6;
  // add r9,r9,r5
  r9 = r9 + r5;
  // cmpw cr6,r8,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r7), so);
  // blt cr6,0x8247aad8
  if (cr6.lt) goto loc_8247AAD8;
  // mr r8,r7
  r8 = r7;
loc_8247AAD8:
  // extsw r6,r7
  r6 = uint64_t(int64_t(int32_t(r7)));
  // lfs f13,40(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f13 = double(temp.f32);
  // fsubs f13,f13,f0
  f13 = double(float(f13 - f0));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // std r6,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r6);
  // rlwinm r6,r8,1,0,30
  r6 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r6,r8,r6
  r6 = r8 + r6;
  // rlwinm r6,r6,3,0,28
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 3) & 0xFFFFFFF8;
  // addi r6,r6,127
  r6 = uint64_t(int64_t(int64_t(r6) + 127));
  // rlwinm r6,r6,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x1FFFFFF;
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f9,f13,f12
  f9 = double(float(f13 / f12));
  // lfs f13,8680(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8680);
  f13 = double(temp.f32);
  // fmuls f8,f9,f13
  f8 = double(float(f9 * f13));
  // beq cr6,0x8247ab38
  if (cr6.eq) goto loc_8247AB38;
loc_8247AB24:
  // rlwinm r5,r7,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 7) & 0xFFFFFF80;
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // cmplw cr6,r7,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r6), so);
  // blt cr6,0x8247ab24
  if (cr6.lt) goto loc_8247AB24;
loc_8247AB38:
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lfs f12,7140(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 7140);
  f12 = double(temp.f32);
loc_8247AB40:
  // lfs f11,20(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 20);
  f11 = double(temp.f32);
  // fadds f13,f9,f0
  f13 = double(float(f9 + f0));
  // lfs f10,20(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 20);
  f10 = double(temp.f32);
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,20(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 20, temp.u32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,5120(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 5120, temp.u32);
  // stfs f11,5124(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 5124, temp.u32);
  // lfs f11,16(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 16);
  f11 = double(temp.f32);
  // lfs f10,16(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,16(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 16, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,4096(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 4096, temp.u32);
  // stfs f11,4100(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 4100, temp.u32);
  // lfs f11,12(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 12);
  f11 = double(temp.f32);
  // lfs f10,12(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 12);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,12(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 12, temp.u32);
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,3072(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 3072, temp.u32);
  // stfs f11,3076(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 3076, temp.u32);
  // lfs f11,8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8);
  f11 = double(temp.f32);
  // lfs f10,8(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,8(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 8, temp.u32);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,2048(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 2048, temp.u32);
  // stfs f11,2052(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 2052, temp.u32);
  // lfs f11,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f11 = double(temp.f32);
  // lfs f10,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f10 = double(temp.f32);
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,4(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,1024(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 1024, temp.u32);
  // stfs f11,1028(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 1028, temp.u32);
  // lfs f11,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f11 = double(temp.f32);
  // addi r9,r9,24
  r9 = uint64_t(int64_t(int64_t(r9) + 24));
  // lfs f10,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f10 = double(temp.f32);
  // fmuls f13,f11,f13
  f13 = double(float(f11 * f13));
  // fadds f10,f11,f10
  f10 = double(float(f11 + f10));
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fmuls f11,f10,f0
  f11 = double(float(f10 * f0));
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne cr6,0x8247ab40
  if (!cr6.eq) goto loc_8247AB40;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r8,13(r3)
  r8 = REX_LOAD_U8(uint32_t(r3) + 13);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // rotlwi r8,r8,2
  r8 = __builtin_rotateleft32(uint32_t(r8), 2);
  // divwu r9,r9,r8
  r9 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // twllei r8,0
  if (int32_t(r8) == 0 || uint32_t(r8) < 0u) lifted::Trap(0);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // blt cr6,0x8247ac5c
  if (cr6.lt) goto loc_8247AC5C;
  // mr r9,r10
  r9 = r10;
loc_8247AC5C:
  // lwz r8,20(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r10,24(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 24);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // stw r9,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r9));
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8247ac7c
  if (cr6.lt) goto loc_8247AC7C;
  // mr r11,r10
  r11 = r10;
loc_8247AC7C:
  // stfs f0,36(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_8247DE28: lifted from generated/ by lift.py.
lifted::Result f8247DE28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247DE28 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,30
  r11 = uint32_t(r6) & 0x3;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247dee8
  if (!cr6.eq) goto loc_8247DEE8;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247dee8
  if (!cr6.gt) goto loc_8247DEE8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247dee8
  if (cr6.lt) goto loc_8247DEE8;
  // bl 0x8246f300
  // (left out: dead in C++)
  { const lifted::Result o = f8246F300(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247DEE8:
  // mr r3,r7
  r3 = r7;
  // bl 0x824728b0
  // (left out: dead in C++)
  f824728B0(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247DF00: lifted from generated/ by lift.py.
lifted::Result f8247DF00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247DF00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,30
  r11 = uint32_t(r6) & 0x3;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247dfc0
  if (!cr6.eq) goto loc_8247DFC0;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247dfc0
  if (!cr6.gt) goto loc_8247DFC0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247dfc0
  if (cr6.lt) goto loc_8247DFC0;
  // bl 0x8246f540
  // (left out: dead in C++)
  { const lifted::Result o = f8246F540(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247DFC0:
  // mr r3,r7
  r3 = r7;
  // bl 0x82472bb8
  // (left out: dead in C++)
  f82472BB8(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247DFD8: lifted from generated/ by lift.py.
lifted::Result f8247DFD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247DFD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,30
  r11 = uint32_t(r6) & 0x3;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e098
  if (!cr6.eq) goto loc_8247E098;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e098
  if (!cr6.gt) goto loc_8247E098;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e098
  if (cr6.lt) goto loc_8247E098;
  // bl 0x8246f7c0
  // (left out: dead in C++)
  { const lifted::Result o = f8246F7C0(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247E098:
  // mr r3,r7
  r3 = r7;
  // bl 0x82472f30
  // (left out: dead in C++)
  f82472F30(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247E0B0: lifted from generated/ by lift.py.
lifted::Result f8247E0B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E0B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,30
  r11 = uint32_t(r6) & 0x3;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e170
  if (!cr6.eq) goto loc_8247E170;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e170
  if (!cr6.gt) goto loc_8247E170;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e170
  if (cr6.lt) goto loc_8247E170;
  // bl 0x8246fac0
  // (left out: dead in C++)
  { const lifted::Result o = f8246FAC0(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247E170:
  // mr r3,r7
  r3 = r7;
  // bl 0x824733c0
  // (left out: dead in C++)
  f824733C0(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247E338: lifted from generated/ by lift.py.
lifted::Result f8247E338(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E338 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e3f8
  if (!cr6.eq) goto loc_8247E3F8;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e3f8
  if (!cr6.gt) goto loc_8247E3F8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e3f8
  if (cr6.lt) goto loc_8247E3F8;
  // bl 0x8246e7f8
  // (left out: dead in C++)
  { const lifted::Result o = f8246E7F8(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247E3F8:
  // mr r3,r7
  r3 = r7;
  // bl 0x82474578
  // (left out: dead in C++)
  f82474578(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247E410: lifted from generated/ by lift.py.
lifted::Result f8247E410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E410 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e4d0
  if (!cr6.eq) goto loc_8247E4D0;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e4d0
  if (!cr6.gt) goto loc_8247E4D0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e4d0
  if (cr6.lt) goto loc_8247E4D0;
  // bl 0x8246ec08
  // (left out: dead in C++)
  { const lifted::Result o = f8246EC08(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247E4D0:
  // mr r3,r7
  r3 = r7;
  // bl 0x82474ae8
  // (left out: dead in C++)
  f82474AE8(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247E4E8: lifted from generated/ by lift.py.
lifted::Result f8247E4E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E4E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r31,4(r7)
  r31 = REX_LOAD_U32(uint32_t(r7) + 4);
  // add r5,r5,r9
  r5 = r5 + r9;
  // add r3,r11,r8
  r3 = r11 + r8;
  // or r11,r6,r5
  r11 = r6 | r5;
  // subf r4,r10,r31
  r4 = r31 - r10;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e5a8
  if (!cr6.eq) goto loc_8247E5A8;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e5a8
  if (!cr6.gt) goto loc_8247E5A8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e5a8
  if (cr6.lt) goto loc_8247E5A8;
  // bl 0x8246fe48
  // (left out: dead in C++)
  { const lifted::Result o = f8246FE48(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
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
loc_8247E5A8:
  // mr r3,r7
  r3 = r7;
  // bl 0x82475688
  // (left out: dead in C++)
  f82475688(base, r1, r3);
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

// sub_8247E5C8: lifted from generated/ by lift.py.
lifted::Result f8247E5C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E5C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r31,4(r7)
  r31 = REX_LOAD_U32(uint32_t(r7) + 4);
  // add r5,r5,r9
  r5 = r5 + r9;
  // add r3,r11,r8
  r3 = r11 + r8;
  // or r11,r6,r5
  r11 = r6 | r5;
  // subf r4,r10,r31
  r4 = r31 - r10;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e688
  if (!cr6.eq) goto loc_8247E688;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e688
  if (!cr6.gt) goto loc_8247E688;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e688
  if (cr6.lt) goto loc_8247E688;
  // bl 0x824701e0
  // (left out: dead in C++)
  { const lifted::Result o = f824701E0(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
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
loc_8247E688:
  // mr r3,r7
  r3 = r7;
  // bl 0x824759d8
  // (left out: dead in C++)
  f824759D8(base, r1, r3);
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

// sub_8247E6A8: lifted from generated/ by lift.py.
lifted::Result f8247E6A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E6A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r31,4(r7)
  r31 = REX_LOAD_U32(uint32_t(r7) + 4);
  // add r5,r5,r9
  r5 = r5 + r9;
  // add r3,r11,r8
  r3 = r11 + r8;
  // or r11,r6,r5
  r11 = r6 | r5;
  // subf r4,r10,r31
  r4 = r31 - r10;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e768
  if (!cr6.eq) goto loc_8247E768;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e768
  if (!cr6.gt) goto loc_8247E768;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e768
  if (cr6.lt) goto loc_8247E768;
  // bl 0x82470668
  // (left out: dead in C++)
  { const lifted::Result o = f82470668(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
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
loc_8247E768:
  // mr r3,r7
  r3 = r7;
  // bl 0x82475e20
  // (left out: dead in C++)
  f82475E20(base, r1, r3);
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

// sub_8247E788: lifted from generated/ by lift.py.
lifted::Result f8247E788(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E788 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r31,4(r7)
  r31 = REX_LOAD_U32(uint32_t(r7) + 4);
  // add r5,r5,r9
  r5 = r5 + r9;
  // add r3,r11,r8
  r3 = r11 + r8;
  // or r11,r6,r5
  r11 = r6 | r5;
  // subf r4,r10,r31
  r4 = r31 - r10;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e848
  if (!cr6.eq) goto loc_8247E848;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e848
  if (!cr6.gt) goto loc_8247E848;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e848
  if (cr6.lt) goto loc_8247E848;
  // bl 0x82470ca0
  // (left out: dead in C++)
  { const lifted::Result o = f82470CA0(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
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
loc_8247E848:
  // mr r3,r7
  r3 = r7;
  // bl 0x82476418
  // (left out: dead in C++)
  f82476418(base, r1, r3);
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

// sub_8247E868: lifted from generated/ by lift.py.
lifted::Result f8247E868(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E868 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247e928
  if (!cr6.eq) goto loc_8247E928;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247e928
  if (!cr6.gt) goto loc_8247E928;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247e928
  if (cr6.lt) goto loc_8247E928;
  // bl 0x82471510
  // (left out: dead in C++)
  { const lifted::Result o = f82471510(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247E928:
  // mr r3,r7
  r3 = r7;
  // bl 0x82476ff0
  // (left out: dead in C++)
  f82476FF0(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247E940: lifted from generated/ by lift.py.
lifted::Result f8247E940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247E940 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247ea00
  if (!cr6.eq) goto loc_8247EA00;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247ea00
  if (!cr6.gt) goto loc_8247EA00;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247ea00
  if (cr6.lt) goto loc_8247EA00;
  // bl 0x824717f8
  // (left out: dead in C++)
  { const lifted::Result o = f824717F8(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247EA00:
  // mr r3,r7
  r3 = r7;
  // bl 0x82477348
  // (left out: dead in C++)
  f82477348(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247EA18: lifted from generated/ by lift.py.
lifted::Result f8247EA18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247EA18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247ead8
  if (!cr6.eq) goto loc_8247EAD8;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247ead8
  if (!cr6.gt) goto loc_8247EAD8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247ead8
  if (cr6.lt) goto loc_8247EAD8;
  // bl 0x82471b40
  // (left out: dead in C++)
  { const lifted::Result o = f82471B40(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247EAD8:
  // mr r3,r7
  r3 = r7;
  // bl 0x82477760
  // (left out: dead in C++)
  f82477760(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8247EAF0: lifted from generated/ by lift.py.
lifted::Result f8247EAF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8247EAF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // lwz r11,28(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 28);
  // lfs f13,48(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 48);
  f13 = double(temp.f32);
  // lwz r6,24(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 24);
  // lwz r10,8(r7)
  r10 = REX_LOAD_U32(uint32_t(r7) + 8);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lbz r4,13(r7)
  r4 = REX_LOAD_U8(uint32_t(r7) + 13);
  // subf r6,r11,r6
  r6 = r6 - r11;
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // mullw r11,r4,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r4)) * int64_t(int32_t(r10))));
  // lwz r8,0(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 0);
  // lwz r9,20(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 20);
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // subf r4,r10,r3
  r4 = r3 - r10;
  // add r3,r11,r8
  r3 = r11 + r8;
  // add r5,r5,r9
  r5 = r5 + r9;
  // clrlwi r11,r6,29
  r11 = uint32_t(r6) & 0x7;
  // or r11,r11,r5
  r11 = r11 | r5;
  // clrlwi r11,r11,28
  r11 = uint32_t(r11) & 0xF;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8247ebb0
  if (!cr6.eq) goto loc_8247EBB0;
  // extsw r11,r6
  r11 = uint64_t(int64_t(int32_t(r6)));
  // lfs f0,44(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 44);
  f0 = double(temp.f32);
  // addi r10,r4,-1
  r10 = uint64_t(int64_t(int64_t(r4) + -1));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // ble cr6,0x8247ebb0
  if (!cr6.gt) goto loc_8247EBB0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x8247ebb0
  if (cr6.lt) goto loc_8247EBB0;
  // bl 0x82471f68
  // (left out: dead in C++)
  { const lifted::Result o = f82471F68(base, r1, r13, r3, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_8247EBB0:
  // mr r3,r7
  r3 = r7;
  // bl 0x82477d20
  // (left out: dead in C++)
  f82477D20(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82481B88: lifted from generated/ by lift.py.
lifted::Result f82481B88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82481B88 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82481B98: lifted from generated/ by lift.py.
lifted::Result f82481B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11) {
  ITC_NATIVE_RUNNING("sub_82481B98 (lifted)");
  uint64_t r11 = 0, r12 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e60
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r22,r3
  r22 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r3,296
  r3 = uint64_t(int64_t(296));
  // mr r28,r4
  r28 = r4;
  // mr r25,r5
  r25 = r5;
  // mr r24,r6
  r24 = r6;
  // mr r18,r7
  r18 = r7;
  // mr r30,r31
  r30 = r31;
  // bl 0x824875a8
  // (left out: dead in C++)
  { const lifted::Result o = f824875A8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481BC8); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481bdc
  if (cr6.eq) goto loc_82481BDC;
  // bl 0x824875b8
  // (left out: dead in C++)
  { const lifted::Result o = f824875B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r19,r3
  r19 = r3;
  // b 0x82481be0
  goto loc_82481BE0;
loc_82481BDC:
  // mr r19,r31
  r19 = r31;
loc_82481BE0:
  // li r3,344
  r3 = uint64_t(int64_t(344));
  // bl 0x82486810
  // (left out: dead in C++)
  { const lifted::Result o = f82486810(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481BE8); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481bfc
  if (cr6.eq) goto loc_82481BFC;
  // bl 0x82486f90
  // (left out: dead in C++)
  { const lifted::Result o = f82486F90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // b 0x82481c00
  goto loc_82481C00;
loc_82481BFC:
  // mr r23,r31
  r23 = r31;
loc_82481C00:
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // stw r31,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r31));
  // mr r27,r31
  r27 = r31;
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // mr r26,r31
  r26 = r31;
  // mr r21,r31
  r21 = r31;
  // mr r20,r31
  r20 = r31;
  // ori r29,r11,14
  r29 = r11 | 14;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x82481c30
  if (cr6.eq) goto loc_82481C30;
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // bne cr6,0x82481c34
  if (!cr6.eq) goto loc_82481C34;
loc_82481C30:
  // mr r30,r29
  r30 = r29;
loc_82481C34:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82481c98
  if (cr6.eq) goto loc_82481C98;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481c98
  if (cr6.lt) goto loc_82481C98;
  // li r3,680
  r3 = uint64_t(int64_t(680));
  // bl 0x82485460
  // (left out: dead in C++)
  { const lifted::Result o = f82485460(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481C4C); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481c60
  if (cr6.eq) goto loc_82481C60;
  // bl 0x82486710
  // (left out: dead in C++)
  { const lifted::Result o = f82486710(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // b 0x82481c64
  goto loc_82481C64;
loc_82481C60:
  // mr r27,r31
  r27 = r31;
loc_82481C64:
  // li r3,1488
  r3 = uint64_t(int64_t(1488));
  // bl 0x82483460
  // (left out: dead in C++)
  { const lifted::Result o = f82483460(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481C6C); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481c80
  if (cr6.eq) goto loc_82481C80;
  // bl 0x82483700
  // (left out: dead in C++)
  { const lifted::Result o = f82483700(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r21,r3
  r21 = r3;
  // b 0x82481c84
  goto loc_82481C84;
loc_82481C80:
  // mr r21,r31
  r21 = r31;
loc_82481C84:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82481c94
  if (cr6.eq) goto loc_82481C94;
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // bne cr6,0x82481c98
  if (!cr6.eq) goto loc_82481C98;
loc_82481C94:
  // mr r30,r29
  r30 = r29;
loc_82481C98:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82481cfc
  if (cr6.eq) goto loc_82481CFC;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481cfc
  if (cr6.lt) goto loc_82481CFC;
  // li r3,552
  r3 = uint64_t(int64_t(552));
  // bl 0x824847d0
  // (left out: dead in C++)
  { const lifted::Result o = f824847D0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481CB0); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481cc4
  if (cr6.eq) goto loc_82481CC4;
  // bl 0x82485350
  // (left out: dead in C++)
  { const lifted::Result o = f82485350(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // b 0x82481cc8
  goto loc_82481CC8;
loc_82481CC4:
  // mr r26,r31
  r26 = r31;
loc_82481CC8:
  // li r3,376
  r3 = uint64_t(int64_t(376));
  // bl 0x82483dc0
  // (left out: dead in C++)
  { const lifted::Result o = f82483DC0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82481CD0); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82481ce4
  if (cr6.eq) goto loc_82481CE4;
  // bl 0x82483f30
  // (left out: dead in C++)
  { const lifted::Result o = f82483F30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r20,r3
  r20 = r3;
  // b 0x82481ce8
  goto loc_82481CE8;
loc_82481CE4:
  // mr r20,r31
  r20 = r31;
loc_82481CE8:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x82481cf8
  if (cr6.eq) goto loc_82481CF8;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // bne cr6,0x82481cfc
  if (!cr6.eq) goto loc_82481CFC;
loc_82481CF8:
  // mr r30,r29
  r30 = r29;
loc_82481CFC:
  // li r29,8
  r29 = uint64_t(int64_t(8));
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82481d5c
  if (cr6.eq) goto loc_82481D5C;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481d5c
  if (cr6.lt) goto loc_82481D5C;
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // subf r10,r25,r24
  r10 = r24 - r25;
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // mr r3,r27
  r3 = r27;
  // std r31,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r31);
  // std r31,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r31);
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // lwz r11,24(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 24);
  // stw r29,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r29));
  // stw r11,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r11));
  // rlwinm r11,r10,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481D58)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481D5C:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82481db8
  if (cr6.eq) goto loc_82481DB8;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481db8
  if (cr6.lt) goto loc_82481DB8;
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // subf r10,r25,r24
  r10 = r24 - r25;
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // mr r3,r26
  r3 = r26;
  // std r31,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r31);
  // std r31,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r31);
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // lwz r11,28(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 28);
  // stw r29,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r29));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // rlwinm r11,r10,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // stw r11,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r11));
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481DB4)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481DB8:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82481df8
  if (cr6.eq) goto loc_82481DF8;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481df8
  if (cr6.lt) goto loc_82481DF8;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r10,4(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r21
  r3 = r21;
  // std r31,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r31);
  // clrlwi r11,r10,31
  r11 = uint32_t(r10) & 0x1;
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // bl 0x82483770
  // (left out: dead in C++)
  { const lifted::Result o = f82483770(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481DF8:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82481e18
  if (cr6.eq) goto loc_82481E18;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481ec0
  if (cr6.lt) goto loc_82481EC0;
  // mr r4,r22
  r4 = r22;
  // mr r3,r20
  r3 = r20;
  // bl 0x82483130
  // (left out: dead in C++)
  { const lifted::Result o = f82483130(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481E18:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481ec0
  if (cr6.lt) goto loc_82481EC0;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r7,r20
  r7 = r20;
  // mr r6,r26
  r6 = r26;
  // mr r5,r21
  r5 = r21;
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mr r4,r27
  r4 = r27;
  // mr r3,r23
  r3 = r23;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481E4C)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481ec0
  if (cr6.lt) goto loc_82481EC0;
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r10,4(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // mr r3,r19
  r3 = r19;
  // std r31,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r31);
  // std r31,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r31);
  // std r31,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r31);
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,8(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 8);
  // stw r22,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r22));
  // stw r23,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r23));
  // stw r11,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r11));
  // lwz r11,12(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 12);
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // rlwinm r11,r10,31,31,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0x1;
  // stw r11,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r11));
  // lwz r11,16(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 16);
  // stw r11,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r11));
  // lwz r11,20(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 20);
  // stw r11,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r11));
  // lwz r11,0(r19)
  r11 = REX_LOAD_U32(uint32_t(r19) + 0);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481EBC)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481EC0:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82481eec
  if (cr6.eq) goto loc_82481EEC;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481eec
  if (cr6.lt) goto loc_82481EEC;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // mr r4,r25
  r4 = r25;
  // mr r3,r27
  r3 = r27;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481EE8)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481EEC:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82481f18
  if (cr6.eq) goto loc_82481F18;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481f18
  if (cr6.lt) goto loc_82481F18;
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // mr r4,r24
  r4 = r24;
  // mr r3,r26
  r3 = r26;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481F14)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481F18:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x82481f44
  if (cr6.eq) goto loc_82481F44;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481f44
  if (cr6.lt) goto loc_82481F44;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r27
  r3 = r27;
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481F40)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481F44:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x82481f70
  if (cr6.eq) goto loc_82481F70;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481f9c
  if (cr6.lt) goto loc_82481F9C;
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r26
  r3 = r26;
  // lwz r11,92(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 92);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481F6C)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481F70:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82481f9c
  if (cr6.lt) goto loc_82481F9C;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r5,80(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r3,r23
  r3 = r23;
  // lwz r4,84(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481F98)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82481F9C:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82481fb8
  if (cr6.eq) goto loc_82481FB8;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // mr r3,r27
  r3 = r27;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481FB8)); r3 = o.r3; f1 = o.f1; }
loc_82481FB8:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x82481fd4
  if (cr6.eq) goto loc_82481FD4;
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // mr r3,r26
  r3 = r26;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481FD4)); r3 = o.r3; f1 = o.f1; }
loc_82481FD4:
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // beq cr6,0x82481ff0
  if (cr6.eq) goto loc_82481FF0;
  // lwz r11,0(r21)
  r11 = REX_LOAD_U32(uint32_t(r21) + 0);
  // mr r3,r21
  r3 = r21;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82481FF0)); r3 = o.r3; f1 = o.f1; }
loc_82481FF0:
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x8248200c
  if (cr6.eq) goto loc_8248200C;
  // lwz r11,0(r20)
  r11 = REX_LOAD_U32(uint32_t(r20) + 0);
  // mr r3,r20
  r3 = r20;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8248200C)); r3 = o.r3; f1 = o.f1; }
loc_8248200C:
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x82482028
  if (cr6.eq) goto loc_82482028;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // mr r3,r23
  r3 = r23;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482028)); r3 = o.r3; f1 = o.f1; }
loc_82482028:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82482040
  if (cr6.lt) goto loc_82482040;
  // stw r19,0(r18)
  REX_STORE_U32(uint32_t(r18) + 0, uint32_t(r19));
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // b 0x825a6eb0
  // (left out: dead in C++)
  return {r3, f1};
loc_82482040:
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // beq cr6,0x8248205c
  if (cr6.eq) goto loc_8248205C;
  // lwz r11,0(r19)
  r11 = REX_LOAD_U32(uint32_t(r19) + 0);
  // mr r3,r19
  r3 = r19;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8248205C)); r3 = o.r3; f1 = o.f1; }
loc_8248205C:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // b 0x825a6eb0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82482068: lifted from generated/ by lift.py.
lifted::Result f82482068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11) {
  ITC_NATIVE_RUNNING("sub_82482068 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // mr r20,r5
  r20 = r5;
  // mr r31,r4
  r31 = r4;
  // mr r22,r3
  r22 = r3;
  // mr r25,r21
  r25 = r21;
  // mr r23,r21
  r23 = r21;
  // stw r21,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r21));
  // stw r21,0(r20)
  REX_STORE_U32(uint32_t(r20) + 0, uint32_t(r21));
  // mr r24,r21
  r24 = r21;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bgt cr6,0x8248247c
  if (cr6.gt) goto loc_8248247C;
  // lis r12,-32184
  r12 = uint64_t(int64_t(-2109210624));
  // addi r12,r12,8384
  r12 = uint64_t(int64_t(int64_t(r12) + 8384));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_8248247C;
  case 1:
  goto loc_824820D0;
  case 2:
  goto loc_824822D0;
  case 3:
  goto loc_82482370;
  default:
  std::abort();
  }
loc_824820D0:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x8248210c
  if (cr6.eq) goto loc_8248210C;
  // li r3,136
  r3 = uint64_t(int64_t(136));
  // bl 0x82487be8
  // (left out: dead in C++)
  { const lifted::Result o = f82487BE8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x824820E4); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // bl 0x82488f50
  // (left out: dead in C++)
  { const lifted::Result o = f82488F50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // bne cr6,0x8248210c
  if (!cr6.eq) goto loc_8248210C;
loc_824820FC:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
loc_8248210C:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482140
  if (cr6.eq) goto loc_82482140;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82482184
  if (cr6.eq) goto loc_82482184;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // mr r24,r23
  r24 = r23;
  // mr r3,r23
  r3 = r23;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482140)); r3 = o.r3; f1 = o.f1; }
loc_82482140:
  // lwz r28,56(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 56);
  // lwz r29,60(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lwz r27,64(r31)
  r27 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82482164
  if (!cr6.eq) goto loc_82482164;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lis r28,2
  r28 = uint64_t(int64_t(131072));
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
loc_82482164:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x824821d4
  if (!cr6.eq) goto loc_824821D4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x824821c0
  if (cr6.eq) goto loc_824821C0;
  // li r29,64
  r29 = uint64_t(int64_t(64));
  // b 0x824821d4
  goto loc_824821D4;
loc_82482184:
  // li r3,136
  r3 = uint64_t(int64_t(136));
  // bl 0x82487be8
  // (left out: dead in C++)
  { const lifted::Result o = f82487BE8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x8248218C); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x824821b0
  if (cr6.eq) goto loc_824821B0;
  // bl 0x82488f50
  // (left out: dead in C++)
  { const lifted::Result o = f82488F50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x82482140
  if (!cr6.eq) goto loc_82482140;
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_824821B0:
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // mr r24,r21
  r24 = r21;
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_824821C0:
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r11,r11,0,27,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x18;
  // addi r29,r11,8
  r29 = uint64_t(int64_t(int64_t(r11) + 8));
loc_824821D4:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // bne cr6,0x824821f4
  if (!cr6.eq) goto loc_824821F4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
  // addi r27,r11,1
  r27 = uint64_t(int64_t(int64_t(r11) + 1));
loc_824821F4:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // li r30,4
  r30 = uint64_t(int64_t(4));
  // li r26,12
  r26 = uint64_t(int64_t(12));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482284
  if (cr6.eq) goto loc_82482284;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r9,r21
  r9 = r21;
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_82482218:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x82482218
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82482218;
  // lwz r10,32(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // stw r28,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r28));
  // stw r29,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r29));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ld r10,40(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 40);
  // stw r27,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r27));
  // std r10,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r10);
  // ld r10,48(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 48);
  // std r10,120(r1)
  REX_STORE_U64(uint32_t(r1) + 120, r10);
  // beq cr6,0x82482260
  if (cr6.eq) goto loc_82482260;
  // stw r26,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r26));
loc_82482260:
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r23
  r3 = r23;
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482278)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x824826b8
  if (cr6.lt) goto loc_824826B8;
loc_82482284:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // cmplw cr6,r23,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r24), so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r9,r21
  r9 = r21;
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_824822A8:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x824822a8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_824822A8;
  // lwz r10,32(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // ld r10,40(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 40);
  // std r10,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r10);
  // ld r10,48(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 48);
  // std r10,120(r1)
  REX_STORE_U64(uint32_t(r1) + 120, r10);
  // b 0x82482650
  goto loc_82482650;
loc_824822D0:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482314
  if (cr6.eq) goto loc_82482314;
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // bl 0x824878b0
  // (left out: dead in C++)
  { const lifted::Result o = f824878B0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x824822E4); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // bl 0x82487b28
  // (left out: dead in C++)
  { const lifted::Result o = f82487B28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r4,32(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 32);
  // bl 0x824878d0
  // (left out: dead in C++)
  { const lifted::Result o = f824878D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x824826b8
  if (cr6.lt) goto loc_824826B8;
loc_82482314:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // bl 0x824878b0
  // (left out: dead in C++)
  { const lifted::Result o = f824878B0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82482328); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8248234c
  if (cr6.eq) goto loc_8248234C;
  // bl 0x82487b28
  // (left out: dead in C++)
  { const lifted::Result o = f82487B28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x8248235c
  if (!cr6.eq) goto loc_8248235C;
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_8248234C:
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // mr r24,r21
  r24 = r21;
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_8248235C:
  // mr r3,r24
  r3 = r24;
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r4,32(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 32);
  // bl 0x824878d0
  // (left out: dead in C++)
  { const lifted::Result o = f824878D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x8248268c
  goto loc_8248268C;
loc_82482370:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x824823c0
  if (cr6.eq) goto loc_824823C0;
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // bl 0x82487660
  // (left out: dead in C++)
  { const lifted::Result o = f82487660(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82482384); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // bl 0x824877e0
  // (left out: dead in C++)
  { const lifted::Result o = f824877E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // lwz r5,40(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r4,32(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824823B4)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x824826b8
  if (cr6.lt) goto loc_824826B8;
loc_824823C0:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82482418
  if (cr6.eq) goto loc_82482418;
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r10,36(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82482418
  if (!cr6.eq) goto loc_82482418;
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r10,44(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 44);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82482418
  if (!cr6.eq) goto loc_82482418;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // mr r24,r23
  r24 = r23;
  // mr r3,r23
  r3 = r23;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482414)); r3 = o.r3; f1 = o.f1; }
  // b 0x82482690
  goto loc_82482690;
loc_82482418:
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // bl 0x82487660
  // (left out: dead in C++)
  { const lifted::Result o = f82487660(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82482420); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82482444
  if (cr6.eq) goto loc_82482444;
  // bl 0x824877e0
  // (left out: dead in C++)
  { const lifted::Result o = f824877E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x82482454
  if (!cr6.eq) goto loc_82482454;
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_82482444:
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // mr r24,r21
  r24 = r21;
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_82482454:
  // cmplw cr6,r23,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r24), so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // lwz r11,0(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 0);
  // mr r3,r24
  r3 = r24;
  // lwz r5,44(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r4,36(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482478)); r3 = o.r3; f1 = o.f1; }
  // b 0x8248268c
  goto loc_8248268C;
loc_8248247C:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x824824a8
  if (cr6.eq) goto loc_824824A8;
  // li r3,136
  r3 = uint64_t(int64_t(136));
  // bl 0x82487be8
  // (left out: dead in C++)
  { const lifted::Result o = f82487BE8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82482490); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
  // bl 0x82488f50
  // (left out: dead in C++)
  { const lifted::Result o = f82488F50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x824820fc
  if (cr6.eq) goto loc_824820FC;
loc_824824A8:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x824824dc
  if (cr6.eq) goto loc_824824DC;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82482520
  if (cr6.eq) goto loc_82482520;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // mr r24,r23
  r24 = r23;
  // mr r3,r23
  r3 = r23;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824824DC)); r3 = o.r3; f1 = o.f1; }
loc_824824DC:
  // lwz r28,36(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r29,40(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r27,44(r31)
  r27 = REX_LOAD_U32(uint32_t(r31) + 44);
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82482500
  if (!cr6.eq) goto loc_82482500;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lis r28,2
  r28 = uint64_t(int64_t(131072));
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
loc_82482500:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x82482570
  if (!cr6.eq) goto loc_82482570;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8248255c
  if (cr6.eq) goto loc_8248255C;
  // li r29,64
  r29 = uint64_t(int64_t(64));
  // b 0x82482570
  goto loc_82482570;
loc_82482520:
  // li r3,136
  r3 = uint64_t(int64_t(136));
  // bl 0x82487be8
  // (left out: dead in C++)
  { const lifted::Result o = f82487BE8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, 0x82482528); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8248254c
  if (cr6.eq) goto loc_8248254C;
  // bl 0x82488f50
  // (left out: dead in C++)
  { const lifted::Result o = f82488F50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x824824dc
  if (!cr6.eq) goto loc_824824DC;
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_8248254C:
  // lis r25,-32761
  r25 = uint64_t(int64_t(-2147024896));
  // mr r24,r21
  r24 = r21;
  // ori r25,r25,14
  r25 = r25 | 14;
  // b 0x824826b8
  goto loc_824826B8;
loc_8248255C:
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r11,r11,0,27,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x18;
  // addi r29,r11,8
  r29 = uint64_t(int64_t(int64_t(r11) + 8));
loc_82482570:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // bne cr6,0x82482590
  if (!cr6.eq) goto loc_82482590;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // subfic r11,r11,0
  ca = uint32_t(r11) <= 0;
  r11 = static_cast<uint64_t>(0) - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
  // addi r27,r11,1
  r27 = uint64_t(int64_t(int64_t(r11) + 1));
loc_82482590:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r26,9
  r26 = uint64_t(int64_t(9));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482614
  if (cr6.eq) goto loc_82482614;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r9,r21
  r9 = r21;
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_824825B4:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x824825b4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_824825B4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,32(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 32);
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // stw r30,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r30));
  // stw r28,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r28));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r29,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r29));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // stw r27,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r27));
  // beq cr6,0x824825f0
  if (cr6.eq) goto loc_824825F0;
  // stw r26,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r26));
loc_824825F0:
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r3,r23
  r3 = r23;
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482608)); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x824826b8
  if (cr6.lt) goto loc_824826B8;
loc_82482614:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // cmplw cr6,r23,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), uint32_t(r24), so);
  // beq cr6,0x82482690
  if (cr6.eq) goto loc_82482690;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r9,r21
  r9 = r21;
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // mtctr r10
  ctr = r10;
loc_82482638:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x82482638
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82482638;
  // lwz r10,32(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r30,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r30));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
loc_82482650:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r27,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r27));
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // stw r29,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r29));
  // stw r28,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r28));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // beq cr6,0x82482674
  if (cr6.eq) goto loc_82482674;
  // stw r26,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r26));
loc_82482674:
  // lwz r11,0(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 0);
  // mr r3,r24
  r3 = r24;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8248268C)); r3 = o.r3; f1 = o.f1; }
loc_8248268C:
  // mr r25,r3
  r25 = r3;
loc_82482690:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x824826b8
  if (cr6.lt) goto loc_824826B8;
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r6,r24
  r6 = r24;
  // mr r5,r23
  r5 = r23;
  // mr r4,r31
  r4 = r31;
  // mr r3,r22
  r3 = r22;
  // bl 0x82481b98
  // (left out: dead in C++)
  { const lifted::Result o = f82481B98(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); r3 = o.r3; f1 = o.f1; }
  // lwz r21,80(r1)
  r21 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r25,r3
  r25 = r3;
loc_824826B8:
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x824826d4
  if (cr6.eq) goto loc_824826D4;
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // mr r3,r23
  r3 = r23;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824826D4)); r3 = o.r3; f1 = o.f1; }
loc_824826D4:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x824826f0
  if (cr6.eq) goto loc_824826F0;
  // lwz r11,0(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 0);
  // mr r3,r24
  r3 = r24;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824826F0)); r3 = o.r3; f1 = o.f1; }
loc_824826F0:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // blt cr6,0x82482708
  if (cr6.lt) goto loc_82482708;
  // stw r21,0(r20)
  REX_STORE_U32(uint32_t(r20) + 0, uint32_t(r21));
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
loc_82482708:
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // beq cr6,0x82482724
  if (cr6.eq) goto loc_82482724;
  // lwz r11,0(r21)
  r11 = REX_LOAD_U32(uint32_t(r21) + 0);
  // mr r3,r21
  r3 = r21;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82482724)); r3 = o.r3; f1 = o.f1; }
loc_82482724:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82482730: lifted from generated/ by lift.py.
lifted::Result f82482730(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11) {
  ITC_NATIVE_RUNNING("sub_82482730 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r8,r4
  r8 = r4;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // mtctr r10
  ctr = r10;
loc_82482750:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x82482750
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82482750;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r8,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82482068
  // (left out: dead in C++)
  { const lifted::Result o = f82482068(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82482780: lifted from generated/ by lift.py.
uint64_t f82482780(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82482780 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
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
  // bl 0x824892a8
  // (left out: dead in C++)
  f824892A8(base, r3);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,120
  r5 = uint64_t(int64_t(120));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,144
  r3 = uint64_t(int64_t(int64_t(r31) + 144));
  // lfs f13,-18996(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -18996);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,-24904(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -24904);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,132(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 132, temp.u32);
  // stfs f13,136(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 136, temp.u32);
  // lfs f12,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f12 = double(temp.f32);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stfs f12,140(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 140, temp.u32);
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
  // stw r10,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r10));
  // stw r11,116(r31)
  REX_STORE_U32(uint32_t(r31) + 116, uint32_t(r11));
  // stw r11,120(r31)
  REX_STORE_U32(uint32_t(r31) + 120, uint32_t(r11));
  // stw r11,124(r31)
  REX_STORE_U32(uint32_t(r31) + 124, uint32_t(r11));
  // stw r11,128(r31)
  REX_STORE_U32(uint32_t(r31) + 128, uint32_t(r11));
  // stw r11,264(r31)
  REX_STORE_U32(uint32_t(r31) + 264, uint32_t(r11));
  // stw r11,268(r31)
  REX_STORE_U32(uint32_t(r31) + 268, uint32_t(r11));
  // stw r11,272(r31)
  REX_STORE_U32(uint32_t(r31) + 272, uint32_t(r11));
  // stw r11,276(r31)
  REX_STORE_U32(uint32_t(r31) + 276, uint32_t(r11));
  // stw r11,280(r31)
  REX_STORE_U32(uint32_t(r31) + 280, uint32_t(r11));
  // stw r11,284(r31)
  REX_STORE_U32(uint32_t(r31) + 284, uint32_t(r11));
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
  // stw r11,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r11));
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
  // stw r11,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r11));
  // stw r11,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r11));
  // stw r11,100(r31)
  REX_STORE_U32(uint32_t(r31) + 100, uint32_t(r11));
  // stw r11,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r11));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x82482838)); r3 = o.r3; f1 = o.f1; }
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

// sub_82482968: lifted from generated/ by lift.py.
lifted::Result f82482968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82482968 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82482780
  // (left out: dead in C++)
  r3 = f82482780(base, r1, r13, r3);
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

// sub_82483130: lifted from generated/ by lift.py.
lifted::Result f82483130(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11) {
  ITC_NATIVE_RUNNING("sub_82483130 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82483154)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x82483180
  if (cr6.lt) goto loc_82483180;
  // mr r3,r29
  r3 = r29;
  // bl 0x8229d7e0
  // (left out: dead in C++)
  r3 = f8229D7E0(base, r3);
  // mr r3,r31
  r3 = r31;
  // stw r29,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r29));
  // bl 0x82482b18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82482B18, LIFTED_ARGS(0x82483174)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x82483194
  if (!cr6.lt) goto loc_82483194;
loc_82483180:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82483194)); r3 = o.r3; f1 = o.f1; }
loc_82483194:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82483238: lifted from generated/ by lift.py.
lifted::Result f82483238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82483238 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Temp temp{};
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
  // bl 0x82489300
  // (left out: dead in C++)
  { const lifted::Result o = f82489300(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-11512
  r11 = uint64_t(int64_t(int64_t(r11) + -11512));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x824892d0
  // (left out: dead in C++)
  f824892D0(base, r3);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lfs f0,3396(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 3396);
  f0 = double(temp.f32);
  // stfs f0,72(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 72, temp.u32);
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
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

// sub_82483388: lifted from generated/ by lift.py.
lifted::Result f82483388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82483388 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Temp temp{};
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
  // bl 0x82489300
  // (left out: dead in C++)
  { const lifted::Result o = f82489300(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-11448
  r11 = uint64_t(int64_t(int64_t(r11) + -11448));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x824892d0
  // (left out: dead in C++)
  f824892D0(base, r3);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // stfs f0,80(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 80, temp.u32);
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
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

// sub_82483460: lifted from generated/ by lift.py.
lifted::Result f82483460(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82483460 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32808
  r4 = r4 | 32808;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82483700: lifted from generated/ by lift.py.
lifted::Result f82483700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82483700 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-11384
  r11 = uint64_t(int64_t(int64_t(r11) + -11384));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x824892a8
  // (left out: dead in C++)
  f824892A8(base, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r5,1408
  r5 = uint64_t(int64_t(1408));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,60
  r3 = uint64_t(int64_t(int64_t(r31) + 60));
  // stw r11,1472(r31)
  REX_STORE_U32(uint32_t(r31) + 1472, uint32_t(r11));
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,1468(r31)
  REX_STORE_U32(uint32_t(r31) + 1468, uint32_t(r11));
  // stw r11,1476(r31)
  REX_STORE_U32(uint32_t(r31) + 1476, uint32_t(r11));
  // stw r11,1480(r31)
  REX_STORE_U32(uint32_t(r31) + 1480, uint32_t(r11));
  // stw r11,1484(r31)
  REX_STORE_U32(uint32_t(r31) + 1484, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x82483758)); r3 = o.r3; f1 = o.f1; }
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

// sub_82483770: lifted from generated/ by lift.py.
lifted::Result f82483770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82483770 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82483794)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82483894
  if (cr6.lt) goto loc_82483894;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8245a760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A760, LIFTED_ARGS(0x824837A8)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82483894
  if (cr6.lt) goto loc_82483894;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r6,1484(r30)
  REX_STORE_U32(uint32_t(r30) + 1484, uint32_t(r6));
  // lwz r7,8(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 8);
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // bne cr6,0x824837cc
  if (!cr6.eq) goto loc_824837CC;
  // li r7,16
  r7 = uint64_t(int64_t(16));
loc_824837CC:
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r7,1468(r30)
  REX_STORE_U32(uint32_t(r30) + 1468, uint32_t(r7));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // mtctr r10
  ctr = r10;
loc_824837E0:
  // std r9,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r9);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bdnz 0x824837e0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_824837E0;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lis r8,-32184
  r8 = uint64_t(int64_t(-2109210624));
  // lwz r11,12(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 12);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r10,4(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 4);
  // stb r7,139(r1)
  REX_STORE_U8(uint32_t(r1) + 139, uint8_t(r7));
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // stb r6,144(r1)
  REX_STORE_U8(uint32_t(r1) + 144, uint8_t(r6));
  // stw r11,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r11));
  // addi r11,r8,14016
  r11 = uint64_t(int64_t(int64_t(r8) + 14016));
  // rlwinm r5,r10,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // clrlwi r5,r5,24
  r5 = uint32_t(r5) & 0xFF;
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // xori r11,r5,1
  r11 = r5 ^ 1;
  // stb r11,136(r1)
  REX_STORE_U8(uint32_t(r1) + 136, uint8_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // mtctr r11
  ctr = r11;
loc_82483834:
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x82483834
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82483834;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stb r6,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r6));
  // addi r4,r30,56
  r4 = uint64_t(int64_t(int64_t(r30) + 56));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stb r11,84(r1)
  REX_STORE_U8(uint32_t(r1) + 84, uint8_t(r11));
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,44100
  r11 = r11 | 44100;
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x8245a768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A768, LIFTED_ARGS(0x82483864)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82483894
  if (cr6.lt) goto loc_82483894;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82483888)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x824838a8
  if (!cr6.lt) goto loc_824838A8;
loc_82483894:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824838A8)); r3 = o.r3; f1 = o.f1; }
loc_824838A8:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82483DC0: lifted from generated/ by lift.py.
lifted::Result f82483DC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82483DC0 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32810
  r4 = r4 | 32810;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82483DD0: lifted from generated/ by lift.py.
uint64_t f82483DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82483DD0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0;
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
  // bl 0x82482780
  // (left out: dead in C++)
  r3 = f82482780(base, r1, r13, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r31,288
  r10 = uint64_t(int64_t(int64_t(r31) + 288));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r11,368(r31)
  REX_STORE_U32(uint32_t(r31) + 368, uint32_t(r11));
  // stw r11,324(r31)
  REX_STORE_U32(uint32_t(r31) + 324, uint32_t(r11));
  // stw r11,328(r31)
  REX_STORE_U32(uint32_t(r31) + 328, uint32_t(r11));
  // stw r11,372(r31)
  REX_STORE_U32(uint32_t(r31) + 372, uint32_t(r11));
  // mtctr r9
  ctr = r9;
loc_82483E08:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82483e08
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82483E08;
  // stw r11,332(r31)
  REX_STORE_U32(uint32_t(r31) + 332, uint32_t(r11));
  // stw r11,336(r31)
  REX_STORE_U32(uint32_t(r31) + 336, uint32_t(r11));
  // stw r11,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r11));
  // stw r11,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r11));
  // stw r11,348(r31)
  REX_STORE_U32(uint32_t(r31) + 348, uint32_t(r11));
  // stw r11,352(r31)
  REX_STORE_U32(uint32_t(r31) + 352, uint32_t(r11));
  // stw r11,356(r31)
  REX_STORE_U32(uint32_t(r31) + 356, uint32_t(r11));
  // stw r11,360(r31)
  REX_STORE_U32(uint32_t(r31) + 360, uint32_t(r11));
  // stw r11,364(r31)
  REX_STORE_U32(uint32_t(r31) + 364, uint32_t(r11));
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

// sub_82483F30: lifted from generated/ by lift.py.
lifted::Result f82483F30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82483F30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82482968
  // (left out: dead in C++)
  { const lifted::Result o = f82482968(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-10120
  r11 = uint64_t(int64_t(int64_t(r11) + -10120));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82483dd0
  // (left out: dead in C++)
  r3 = f82483DD0(base, r1, r13, r3);
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

// sub_824847D0: lifted from generated/ by lift.py.
lifted::Result f824847D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_824847D0 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32797
  r4 = r4 | 32797;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82485350: lifted from generated/ by lift.py.
lifted::Result f82485350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82485350 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,56
  r3 = uint64_t(int64_t(int64_t(r31) + 56));
  // bl 0x82489300
  // (left out: dead in C++)
  { const lifted::Result o = f82489300(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,136
  r3 = uint64_t(int64_t(int64_t(r31) + 136));
  // bl 0x82483238
  // (left out: dead in C++)
  { const lifted::Result o = f82483238(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,212
  r3 = uint64_t(int64_t(int64_t(r31) + 212));
  // bl 0x8248ca80
  // (left out: dead in C++)
  { const lifted::Result o = f8248CA80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-10008
  r11 = uint64_t(int64_t(int64_t(r11) + -10008));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x824893b8
  // (left out: dead in C++)
  { const lifted::Result o = f824893B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r10,508(r31)
  REX_STORE_U32(uint32_t(r31) + 508, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,520(r31)
  REX_STORE_U32(uint32_t(r31) + 520, uint32_t(r11));
  // stw r11,524(r31)
  REX_STORE_U32(uint32_t(r31) + 524, uint32_t(r11));
  // std r11,528(r31)
  REX_STORE_U64(uint32_t(r31) + 528, r11);
  // stw r11,536(r31)
  REX_STORE_U32(uint32_t(r31) + 536, uint32_t(r11));
  // stw r11,540(r31)
  REX_STORE_U32(uint32_t(r31) + 540, uint32_t(r11));
  // stw r10,544(r31)
  REX_STORE_U32(uint32_t(r31) + 544, uint32_t(r10));
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

// sub_82485460: lifted from generated/ by lift.py.
lifted::Result f82485460(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82485460 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32791
  r4 = r4 | 32791;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82486710: lifted from generated/ by lift.py.
lifted::Result f82486710(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82486710 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,56
  r3 = uint64_t(int64_t(int64_t(r31) + 56));
  // bl 0x82489300
  // (left out: dead in C++)
  { const lifted::Result o = f82489300(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // bl 0x82483388
  // (left out: dead in C++)
  { const lifted::Result o = f82483388(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,208
  r3 = uint64_t(int64_t(int64_t(r31) + 208));
  // bl 0x8248ed78
  // (left out: dead in C++)
  { const lifted::Result o = f8248ED78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-9696
  r11 = uint64_t(int64_t(int64_t(r11) + -9696));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8248cbd0
  // (left out: dead in C++)
  { const lifted::Result o = f8248CBD0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,672(r31)
  REX_STORE_U32(uint32_t(r31) + 672, uint32_t(r11));
  // stw r10,660(r31)
  REX_STORE_U32(uint32_t(r31) + 660, uint32_t(r10));
  // stw r11,676(r31)
  REX_STORE_U32(uint32_t(r31) + 676, uint32_t(r11));
  // stw r11,664(r31)
  REX_STORE_U32(uint32_t(r31) + 664, uint32_t(r11));
  // stw r11,668(r31)
  REX_STORE_U32(uint32_t(r31) + 668, uint32_t(r11));
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

// sub_82486810: lifted from generated/ by lift.py.
lifted::Result f82486810(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82486810 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32817
  r4 = r4 | 32817;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82486F90: lifted from generated/ by lift.py.
lifted::Result f82486F90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82486F90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r3,r31,68
  r3 = uint64_t(int64_t(int64_t(r31) + 68));
  // addi r11,r11,-9504
  r11 = uint64_t(int64_t(int64_t(r11) + -9504));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8248f218
  // (left out: dead in C++)
  { const lifted::Result o = f8248F218(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,140
  r3 = uint64_t(int64_t(int64_t(r31) + 140));
  // bl 0x8248f218
  // (left out: dead in C++)
  { const lifted::Result o = f8248F218(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,212
  r3 = uint64_t(int64_t(int64_t(r31) + 212));
  // bl 0x8248f218
  // (left out: dead in C++)
  { const lifted::Result o = f8248F218(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // std r11,288(r31)
  REX_STORE_U64(uint32_t(r31) + 288, r11);
  // stfs f0,336(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 336, temp.u32);
  // std r11,304(r31)
  REX_STORE_U64(uint32_t(r31) + 304, r11);
  // std r11,296(r31)
  REX_STORE_U64(uint32_t(r31) + 296, r11);
  // stw r11,312(r31)
  REX_STORE_U32(uint32_t(r31) + 312, uint32_t(r11));
  // stw r11,316(r31)
  REX_STORE_U32(uint32_t(r31) + 316, uint32_t(r11));
  // lfs f13,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f13 = double(temp.f32);
  // stw r11,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r11));
  // stfs f13,340(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 340, temp.u32);
  // stw r11,324(r31)
  REX_STORE_U32(uint32_t(r31) + 324, uint32_t(r11));
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,328(r31)
  REX_STORE_U32(uint32_t(r31) + 328, uint32_t(r11));
  // stw r11,332(r31)
  REX_STORE_U32(uint32_t(r31) + 332, uint32_t(r11));
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

// sub_824875A8: lifted from generated/ by lift.py.
lifted::Result f824875A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_824875A8 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32819
  r4 = r4 | 32819;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_824875B8: lifted from generated/ by lift.py.
lifted::Result f824875B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_824875B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82490960
  // (left out: dead in C++)
  { const lifted::Result o = f82490960(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-9376
  r11 = uint64_t(int64_t(int64_t(r11) + -9376));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
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

// sub_82487660: lifted from generated/ by lift.py.
lifted::Result f82487660(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82487660 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32780
  r4 = r4 | 32780;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_824877E0: lifted from generated/ by lift.py.
lifted::Result f824877E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_824877E0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r11,-9080
  r10 = uint64_t(int64_t(int64_t(r11) + -9080));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r9,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r9));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // std r11,48(r31)
  REX_STORE_U64(uint32_t(r31) + 48, r11);
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
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

// sub_824878B0: lifted from generated/ by lift.py.
lifted::Result f824878B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_824878B0 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32781
  r4 = r4 | 32781;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_824878D0: lifted from generated/ by lift.py.
lifted::Result f824878D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_824878D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824878F8)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82487908
  if (cr6.lt) goto loc_82487908;
  // stw r30,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r30));
  // stw r29,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r29));
loc_82487908:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82487B28: lifted from generated/ by lift.py.
lifted::Result f82487B28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82487B28 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r10,r11,-8976
  r10 = uint64_t(int64_t(int64_t(r11) + -8976));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
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

// sub_82487BE8: lifted from generated/ by lift.py.
lifted::Result f82487BE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82487BE8 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,8332
  r4 = uint64_t(int64_t(546045952));
  // ori r4,r4,32778
  r4 = r4 | 32778;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82488F50: lifted from generated/ by lift.py.
lifted::Result f82488F50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82488F50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r10,r11,-8880
  r10 = uint64_t(int64_t(int64_t(r11) + -8880));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r9,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r9));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // std r11,56(r31)
  REX_STORE_U64(uint32_t(r31) + 56, r11);
  // stw r8,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r8));
  // std r11,64(r31)
  REX_STORE_U64(uint32_t(r31) + 64, r11);
  // std r11,72(r31)
  REX_STORE_U64(uint32_t(r31) + 72, r11);
  // std r11,80(r31)
  REX_STORE_U64(uint32_t(r31) + 80, r11);
  // std r11,88(r31)
  REX_STORE_U64(uint32_t(r31) + 88, r11);
  // std r11,96(r31)
  REX_STORE_U64(uint32_t(r31) + 96, r11);
  // stw r11,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r11));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // stw r11,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r11));
  // stw r11,116(r31)
  REX_STORE_U32(uint32_t(r31) + 116, uint32_t(r11));
  // stw r11,120(r31)
  REX_STORE_U32(uint32_t(r31) + 120, uint32_t(r11));
  // stw r11,124(r31)
  REX_STORE_U32(uint32_t(r31) + 124, uint32_t(r11));
  // stw r11,128(r31)
  REX_STORE_U32(uint32_t(r31) + 128, uint32_t(r11));
  // stw r11,132(r31)
  REX_STORE_U32(uint32_t(r31) + 132, uint32_t(r11));
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

// sub_82489048: lifted from generated/ by lift.py.
lifted::Result f82489048(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82489048 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stw r31,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r31));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // stw r31,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r31));
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS(0x82489074)); r3 = o.r3; f1 = o.f1; }
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

// sub_824892A8: lifted from generated/ by lift.py.
void f824892A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824892A8 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // stw r11,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r11));
  // stw r11,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r11));
  // blr
  return;
}

// sub_824892D0: lifted from generated/ by lift.py.
void f824892D0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824892D0 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r11));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // stw r11,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r11));
  // blr
  return;
}

// sub_82489300: lifted from generated/ by lift.py.
lifted::Result f82489300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82489300 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r10,r11,-8744
  r10 = uint64_t(int64_t(int64_t(r11) + -8744));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
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

// sub_824893B8: lifted from generated/ by lift.py.
lifted::Result f824893B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_824893B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // bl 0x8248cb58
  // (left out: dead in C++)
  f8248CB58(base, r3);
  // addi r3,r31,136
  r3 = uint64_t(int64_t(int64_t(r31) + 136));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824893E4)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,504(r31)
  REX_STORE_U32(uint32_t(r31) + 504, uint32_t(r11));
  // stw r10,500(r31)
  REX_STORE_U32(uint32_t(r31) + 500, uint32_t(r10));
  // stw r11,508(r31)
  REX_STORE_U32(uint32_t(r31) + 508, uint32_t(r11));
  // stw r10,512(r31)
  REX_STORE_U32(uint32_t(r31) + 512, uint32_t(r10));
  // std r11,128(r31)
  REX_STORE_U64(uint32_t(r31) + 128, r11);
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

// sub_8248CA80: lifted from generated/ by lift.py.
lifted::Result f8248CA80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8248CA80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x82496148
  // (left out: dead in C++)
  { const lifted::Result o = f82496148(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // addi r11,r11,-7864
  r11 = uint64_t(int64_t(int64_t(r11) + -7864));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,204
  r3 = uint64_t(int64_t(int64_t(r31) + 204));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248CAB4)); r3 = o.r3; f1 = o.f1; }
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248CAC4)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,284(r31)
  REX_STORE_U32(uint32_t(r31) + 284, uint32_t(r11));
  // bl 0x824960b8
  // (left out: dead in C++)
  r3 = f824960B8(base, r1, r13, r3, 0x8248CAD4);
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

// sub_8248CB58: lifted from generated/ by lift.py.
void f8248CB58(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8248CB58 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r11));
  // stw r11,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r11));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // stw r11,112(r3)
  REX_STORE_U32(uint32_t(r3) + 112, uint32_t(r11));
  // stw r11,116(r3)
  REX_STORE_U32(uint32_t(r3) + 116, uint32_t(r11));
  // stw r11,120(r3)
  REX_STORE_U32(uint32_t(r3) + 120, uint32_t(r11));
  // blr
  return;
}

// sub_8248CBD0: lifted from generated/ by lift.py.
lifted::Result f8248CBD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8248CBD0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // bl 0x8248cb58
  // (left out: dead in C++)
  f8248CB58(base, r3);
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8248CBFC)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,660(r31)
  REX_STORE_U32(uint32_t(r31) + 660, uint32_t(r11));
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

// sub_8248EC58: lifted from generated/ by lift.py.
uint64_t f8248EC58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8248EC58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248EC7C)); r3 = o.r3; f1 = o.f1; }
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,284
  r3 = uint64_t(int64_t(int64_t(r31) + 284));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248EC8C)); r3 = o.r3; f1 = o.f1; }
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,364
  r3 = uint64_t(int64_t(int64_t(r31) + 364));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248EC9C)); r3 = o.r3; f1 = o.f1; }
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,204
  r3 = uint64_t(int64_t(int64_t(r31) + 204));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x8248ECAC)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,444(r31)
  REX_STORE_U32(uint32_t(r31) + 444, uint32_t(r11));
  // stw r11,448(r31)
  REX_STORE_U32(uint32_t(r31) + 448, uint32_t(r11));
  // bl 0x824960b8
  // (left out: dead in C++)
  r3 = f824960B8(base, r1, r13, r3, 0x8248ECC0);
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

// sub_8248ED78: lifted from generated/ by lift.py.
lifted::Result f8248ED78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8248ED78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82496148
  // (left out: dead in C++)
  { const lifted::Result o = f82496148(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-7784
  r11 = uint64_t(int64_t(int64_t(r11) + -7784));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8248ec58
  // (left out: dead in C++)
  r3 = f8248EC58(base, r1, r13, r3);
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

// sub_8248F218: lifted from generated/ by lift.py.
lifted::Result f8248F218(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8248F218 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r10,r11,-7680
  r10 = uint64_t(int64_t(int64_t(r11) + -7680));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
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

// sub_8248F550: lifted from generated/ by lift.py.
void f8248F550(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8248F550 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,280(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 280, temp.u32);
  // stw r10,212(r3)
  REX_STORE_U32(uint32_t(r3) + 212, uint32_t(r10));
  // stw r10,216(r3)
  REX_STORE_U32(uint32_t(r3) + 216, uint32_t(r10));
  // stw r10,240(r3)
  REX_STORE_U32(uint32_t(r3) + 240, uint32_t(r10));
  // stw r11,244(r3)
  REX_STORE_U32(uint32_t(r3) + 244, uint32_t(r11));
  // stw r11,248(r3)
  REX_STORE_U32(uint32_t(r3) + 248, uint32_t(r11));
  // stw r11,252(r3)
  REX_STORE_U32(uint32_t(r3) + 252, uint32_t(r11));
  // stw r11,256(r3)
  REX_STORE_U32(uint32_t(r3) + 256, uint32_t(r11));
  // stw r11,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r11));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // stw r11,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r11));
  // stw r11,56(r3)
  REX_STORE_U32(uint32_t(r3) + 56, uint32_t(r11));
  // stw r11,60(r3)
  REX_STORE_U32(uint32_t(r3) + 60, uint32_t(r11));
  // stw r11,64(r3)
  REX_STORE_U32(uint32_t(r3) + 64, uint32_t(r11));
  // stw r11,68(r3)
  REX_STORE_U32(uint32_t(r3) + 68, uint32_t(r11));
  // stw r11,72(r3)
  REX_STORE_U32(uint32_t(r3) + 72, uint32_t(r11));
  // stw r11,76(r3)
  REX_STORE_U32(uint32_t(r3) + 76, uint32_t(r11));
  // stw r11,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r11));
  // stw r11,220(r3)
  REX_STORE_U32(uint32_t(r3) + 220, uint32_t(r11));
  // stw r11,224(r3)
  REX_STORE_U32(uint32_t(r3) + 224, uint32_t(r11));
  // stw r11,228(r3)
  REX_STORE_U32(uint32_t(r3) + 228, uint32_t(r11));
  // stw r11,232(r3)
  REX_STORE_U32(uint32_t(r3) + 232, uint32_t(r11));
  // stw r11,236(r3)
  REX_STORE_U32(uint32_t(r3) + 236, uint32_t(r11));
  // stw r11,260(r3)
  REX_STORE_U32(uint32_t(r3) + 260, uint32_t(r11));
  // stw r11,264(r3)
  REX_STORE_U32(uint32_t(r3) + 264, uint32_t(r11));
  // stw r11,268(r3)
  REX_STORE_U32(uint32_t(r3) + 268, uint32_t(r11));
  // stw r11,272(r3)
  REX_STORE_U32(uint32_t(r3) + 272, uint32_t(r11));
  // stw r11,276(r3)
  REX_STORE_U32(uint32_t(r3) + 276, uint32_t(r11));
  // std r11,288(r3)
  REX_STORE_U64(uint32_t(r3) + 288, r11);
  // stw r11,84(r3)
  REX_STORE_U32(uint32_t(r3) + 84, uint32_t(r11));
  // stw r11,88(r3)
  REX_STORE_U32(uint32_t(r3) + 88, uint32_t(r11));
  // stw r11,92(r3)
  REX_STORE_U32(uint32_t(r3) + 92, uint32_t(r11));
  // stw r11,96(r3)
  REX_STORE_U32(uint32_t(r3) + 96, uint32_t(r11));
  // stw r11,100(r3)
  REX_STORE_U32(uint32_t(r3) + 100, uint32_t(r11));
  // stw r11,104(r3)
  REX_STORE_U32(uint32_t(r3) + 104, uint32_t(r11));
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // stw r11,112(r3)
  REX_STORE_U32(uint32_t(r3) + 112, uint32_t(r11));
  // stw r11,116(r3)
  REX_STORE_U32(uint32_t(r3) + 116, uint32_t(r11));
  // stw r11,120(r3)
  REX_STORE_U32(uint32_t(r3) + 120, uint32_t(r11));
  // stw r11,124(r3)
  REX_STORE_U32(uint32_t(r3) + 124, uint32_t(r11));
  // stw r11,128(r3)
  REX_STORE_U32(uint32_t(r3) + 128, uint32_t(r11));
  // stw r11,132(r3)
  REX_STORE_U32(uint32_t(r3) + 132, uint32_t(r11));
  // stw r11,136(r3)
  REX_STORE_U32(uint32_t(r3) + 136, uint32_t(r11));
  // stw r11,140(r3)
  REX_STORE_U32(uint32_t(r3) + 140, uint32_t(r11));
  // stw r11,144(r3)
  REX_STORE_U32(uint32_t(r3) + 144, uint32_t(r11));
  // stw r11,148(r3)
  REX_STORE_U32(uint32_t(r3) + 148, uint32_t(r11));
  // stw r11,152(r3)
  REX_STORE_U32(uint32_t(r3) + 152, uint32_t(r11));
  // stw r11,156(r3)
  REX_STORE_U32(uint32_t(r3) + 156, uint32_t(r11));
  // stw r11,160(r3)
  REX_STORE_U32(uint32_t(r3) + 160, uint32_t(r11));
  // stw r11,164(r3)
  REX_STORE_U32(uint32_t(r3) + 164, uint32_t(r11));
  // stw r11,168(r3)
  REX_STORE_U32(uint32_t(r3) + 168, uint32_t(r11));
  // stw r11,172(r3)
  REX_STORE_U32(uint32_t(r3) + 172, uint32_t(r11));
  // stw r11,176(r3)
  REX_STORE_U32(uint32_t(r3) + 176, uint32_t(r11));
  // stw r11,180(r3)
  REX_STORE_U32(uint32_t(r3) + 180, uint32_t(r11));
  // stw r11,184(r3)
  REX_STORE_U32(uint32_t(r3) + 184, uint32_t(r11));
  // stw r11,188(r3)
  REX_STORE_U32(uint32_t(r3) + 188, uint32_t(r11));
  // stw r11,192(r3)
  REX_STORE_U32(uint32_t(r3) + 192, uint32_t(r11));
  // stw r11,196(r3)
  REX_STORE_U32(uint32_t(r3) + 196, uint32_t(r11));
  // stw r11,200(r3)
  REX_STORE_U32(uint32_t(r3) + 200, uint32_t(r11));
  // stw r11,204(r3)
  REX_STORE_U32(uint32_t(r3) + 204, uint32_t(r11));
  // stw r11,208(r3)
  REX_STORE_U32(uint32_t(r3) + 208, uint32_t(r11));
  // blr
  return;
}

// sub_82490960: lifted from generated/ by lift.py.
lifted::Result f82490960(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82490960 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-7536
  r11 = uint64_t(int64_t(int64_t(r11) + -7536));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8248f550
  // (left out: dead in C++)
  f8248F550(base, r3);
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

// sub_824936E8: lifted from generated/ by lift.py.
uint64_t f824936E8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_824936E8 (lifted)");
  // lwz r3,48(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 48);
  // blr
  return r3;
}

// sub_824960B8: lifted from generated/ by lift.py.
uint64_t f824960B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_824960B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r3,44
  r3 = uint64_t(int64_t(int64_t(r3) + 44));
  // b 0x825a71d0
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82496148: lifted from generated/ by lift.py.
lifted::Result f82496148(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82496148 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // bl 0x82489048
  // (left out: dead in C++)
  { const lifted::Result o = f82489048(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r5,80
  r5 = uint64_t(int64_t(80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,44
  r3 = uint64_t(int64_t(int64_t(r31) + 44));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x82496170)); r3 = o.r3; f1 = o.f1; }
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

// sub_82575B68: lifted from generated/ by lift.py.
lifted::Result f82575B68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82575B68 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r31,r4
  r31 = r4;
  // addi r11,r11,30336
  r11 = uint64_t(int64_t(int64_t(r11) + 30336));
  // mr r9,r3
  r9 = r3;
  // lwz r4,44(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 44);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne 0x82575ba0
  if (!cr0.eq) goto loc_82575BA0;
  // lis r3,-32747
  r3 = uint64_t(int64_t(-2146107392));
  // ori r3,r3,5
  r3 = r3 | 5;
  // b 0x82575bdc
  goto loc_82575BDC;
loc_82575BA0:
  // lis r10,-32169
  r10 = uint64_t(int64_t(-2108227584));
  // lhz r5,40(r11)
  r5 = REX_LOAD_U16(uint32_t(r11) + 40);
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // sth r9,80(r1)
  REX_STORE_U16(uint32_t(r1) + 80, uint16_t(r9));
  // addi r7,r10,23288
  r7 = uint64_t(int64_t(int64_t(r10) + 23288));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ac970
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825AC970, LIFTED_ARGS(0x82575BBC)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82575bd0
  if (!cr0.eq) goto loc_82575BD0;
  // lis r3,-32747
  r3 = uint64_t(int64_t(-2146107392));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x82575bdc
  goto loc_82575BDC;
loc_82575BD0:
  // lhz r11,2(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 2);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82575BDC:
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

// sub_82575BF0: lifted from generated/ by lift.py.
uint64_t f82575BF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82575BF0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // addi r3,r10,30336
  r3 = uint64_t(int64_t(int64_t(r10) + 30336));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r11
  r4 = r11;
  // mr r26,r6
  r26 = r6;
  // mr r28,r7
  r28 = r7;
  // mr r31,r8
  r31 = r8;
  // mr r27,r9
  r27 = r9;
  // bl 0x825774d0
  // (left out: dead in C++)
  r3 = f825774D0(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82575c84
  if (cr0.lt) goto loc_82575C84;
  // rlwinm. r11,r30,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82575c40
  if (!cr0.eq) goto loc_82575C40;
  // lwz r29,92(r1)
  r29 = REX_LOAD_U32(uint32_t(r1) + 92);
loc_82575C40:
  // mulli r11,r29,120
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(120))));
  // li r10,100
  r10 = uint64_t(int64_t(100));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // divwu r11,r11,r10
  r11 = uint32_t(uint32_t(r10) ? uint32_t(r11) / uint32_t(r10) : 0);
  // addi r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 3));
  // rlwinm r11,r11,0,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFC;
  // beq cr6,0x82575c60
  if (cr6.eq) goto loc_82575C60;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82575C60:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82575c70
  if (cr6.eq) goto loc_82575C70;
  // li r10,4096
  r10 = uint64_t(int64_t(4096));
  // stw r10,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r10));
loc_82575C70:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82575c84
  if (cr6.eq) goto loc_82575C84;
  // add r11,r11,r26
  r11 = r11 + r26;
  // addi r11,r11,11884
  r11 = uint64_t(int64_t(int64_t(r11) + 11884));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_82575C84:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82575C90: lifted from generated/ by lift.py.
lifted::Result f82575C90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82575C90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r4
  r27 = r4;
  // lis r4,5
  r4 = uint64_t(int64_t(327680));
  // addi r30,r6,7712
  r30 = uint64_t(int64_t(int64_t(r6) + 7712));
  // mr r28,r3
  r28 = r3;
  // mr r26,r5
  r26 = r5;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // ori r4,r4,32772
  r4 = r4 | 32772;
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // mr r31,r7
  r31 = r7;
  // mr r25,r8
  r25 = r8;
  // mr r24,r9
  r24 = r9;
  // mr r23,r10
  r23 = r10;
  // addi r22,r30,4172
  r22 = uint64_t(int64_t(int64_t(r30) + 4172));
  // bl 0x825dbd44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgInProcessCall, LIFTED_ARGS(0x82575CD8)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // blt 0x82575ce8
  if (cr0.lt) goto loc_82575CE8;
  // lwz r10,144(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 144);
loc_82575CE8:
  // lwz r11,324(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 324);
  // li r8,4096
  r8 = uint64_t(int64_t(4096));
  // mr r29,r31
  r29 = r31;
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // mr r9,r30
  r9 = r30;
  // stw r23,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r23));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stw r24,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r24));
  // mr r7,r26
  r7 = r26;
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
  // addi r11,r31,76
  r11 = uint64_t(int64_t(int64_t(r31) + 76));
  // subf r31,r22,r25
  r31 = r25 - r22;
  // stw r8,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r8));
  // add r30,r30,r11
  r30 = r30 + r11;
  // mr r8,r11
  r8 = r11;
  // add r11,r31,r30
  r11 = r31 + r30;
  // addi r5,r10,30336
  r5 = uint64_t(int64_t(int64_t(r10) + 30336));
  // lwz r10,348(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 348);
  // mr r6,r27
  r6 = r27;
  // stw r31,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r31));
  // mr r4,r28
  r4 = r28;
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // mr r3,r29
  r3 = r29;
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x825776a8
  // (left out: dead in C++)
  { const lifted::Result o = f825776A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82575d6c
  if (cr0.lt) goto loc_82575D6C;
  // lwz r11,332(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 332);
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // lwz r11,340(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 340);
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // lwz r11,356(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 356);
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
loc_82575D6C:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82575D78: lifted from generated/ by lift.py.
lifted::Result f82575D78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82575D78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r29,r5
  r29 = r5;
  // mr r28,r7
  r28 = r7;
  // mr r27,r8
  r27 = r8;
  // mr r26,r9
  r26 = r9;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // not r11,r6
  r11 = ~r6;
  // mr r25,r10
  r25 = r10;
  // cmplwi cr6,r4,1088
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1088, so);
  // rlwinm r24,r11,30,31,31
  r24 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x1;
  // bge cr6,0x82575de4
  if (!cr6.lt) goto loc_82575DE4;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82575dc4
  if (cr6.eq) goto loc_82575DC4;
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,122
  r31 = r31 | 122;
  // b 0x82575e40
  goto loc_82575E40;
loc_82575DC4:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,1088
  r4 = uint64_t(int64_t(1088));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbd64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamAlloc, LIFTED_ARGS(0x82575DD4)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82575e30
  if (cr0.lt) goto loc_82575E30;
  // lwz r30,80(r1)
  r30 = REX_LOAD_U32(uint32_t(r1) + 80);
  // b 0x82575de8
  goto loc_82575DE8;
loc_82575DE4:
  // mr r30,r3
  r30 = r3;
loc_82575DE8:
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r9,0(r25)
  r9 = REX_LOAD_U32(uint32_t(r25) + 0);
  // mr r10,r24
  r10 = r24;
  // addi r4,r11,30336
  r4 = uint64_t(int64_t(int64_t(r11) + 30336));
  // mr r8,r26
  r8 = r26;
  // mr r7,r27
  r7 = r27;
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x82577788
  // (left out: dead in C++)
  r3 = f82577788(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82575e30
  if (cr0.lt) goto loc_82575E30;
  // mr r3,r30
  r3 = r30;
  // bl 0x825780a0
  // (left out: dead in C++)
  { const lifted::Result o = f825780A0(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82575e30
  if (cr0.lt) goto loc_82575E30;
  // lwz r11,20(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20);
  // stw r11,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r11));
loc_82575E30:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82575e40
  if (cr6.eq) goto loc_82575E40;
  // bl 0x825dbd54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamFree, LIFTED_ARGS(0x82575E40)); r3 = o.r3; f1 = o.f1; }
loc_82575E40:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82575E50: lifted from generated/ by lift.py.
lifted::Result f82575E50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82575E50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r30,r3
  r30 = r3;
  // mr r25,r5
  r25 = r5;
  // mr r24,r6
  r24 = r6;
  // mr r28,r7
  r28 = r7;
  // mr r27,r8
  r27 = r8;
  // mr r26,r9
  r26 = r9;
  // mr r22,r10
  r22 = r10;
  // cmplwi cr6,r29,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 4, so);
  // bge cr6,0x82575fcc
  if (!cr6.lt) goto loc_82575FCC;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82575fcc
  if (cr6.eq) goto loc_82575FCC;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82575fcc
  if (cr6.eq) goto loc_82575FCC;
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x82575fcc
  if (cr6.eq) goto loc_82575FCC;
  // li r12,-5952
  r12 = uint64_t(int64_t(-5952));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,0(r22)
  REX_STORE_U32(uint32_t(r22) + 0, uint32_t(r11));
  // and. r10,r30,r12
  r10 = r30 & r12;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82575fcc
  if (!cr0.eq) goto loc_82575FCC;
  // rlwinm. r10,r30,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82575ec0
  if (cr0.eq) goto loc_82575EC0;
  // rlwinm. r11,r30,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82575fcc
  if (cr0.eq) goto loc_82575FCC;
loc_82575EC0:
  // rlwinm. r11,r30,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82575ed0
  if (cr0.eq) goto loc_82575ED0;
  // rlwinm. r11,r30,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82575fcc
  if (cr0.eq) goto loc_82575FCC;
loc_82575ED0:
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82575ee4
  if (cr0.eq) goto loc_82575EE4;
  // andi. r11,r30,44
  r11 = r30 & 44;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82575fcc
  if (cr0.eq) goto loc_82575FCC;
loc_82575EE4:
  // rlwinm. r11,r30,0,21,23
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x700;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82575f14
  if (cr0.eq) goto loc_82575F14;
  // andi. r9,r30,10
  r9 = r30 & 10;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x82575fcc
  if (cr0.eq) goto loc_82575FCC;
  // rlwinm. r9,r30,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82575f14
  if (!cr0.eq) goto loc_82575F14;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82575f14
  if (cr6.eq) goto loc_82575F14;
  // rlwinm r10,r30,0,21,21
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x400;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82575fcc
  if (!cr6.eq) goto loc_82575FCC;
loc_82575F14:
  // mr r3,r22
  r3 = r22;
  // bl 0x825dbd84
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionCreateHandle, LIFTED_ARGS(0x82575F1C)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82575fd0
  if (!cr0.eq) goto loc_82575FD0;
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // lwz r3,0(r22)
  r3 = REX_LOAD_U32(uint32_t(r22) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r23,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r23));
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82575F38)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82575fd0
  if (!cr0.eq) goto loc_82575FD0;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,28
  r7 = uint64_t(int64_t(28));
  // stw r30,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r30));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r29,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r29));
  // mr r5,r26
  r5 = r26;
  // stw r24,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r24));
  // ori r4,r4,16
  r4 = r4 | 16;
  // stw r25,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r25));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r27,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r27));
  // stw r28,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r28));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82575F7C)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82575f8c
  if (!cr0.lt) goto loc_82575F8C;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x82575fbc
  goto loc_82575FBC;
loc_82575F8C:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // bne cr6,0x82575fa8
  if (!cr6.eq) goto loc_82575FA8;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82575F98)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x82575fac
  goto loc_82575FAC;
loc_82575FA8:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_82575FAC:
  // cmplwi cr6,r31,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 997, so);
  // beq cr6,0x82575fd0
  if (cr6.eq) goto loc_82575FD0;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82575fd0
  if (cr6.eq) goto loc_82575FD0;
loc_82575FBC:
  // lwz r3,0(r22)
  r3 = REX_LOAD_U32(uint32_t(r22) + 0);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82575FC4)); r3 = o.r3; f1 = o.f1; }
  // stw r23,0(r22)
  REX_STORE_U32(uint32_t(r22) + 0, uint32_t(r23));
  // b 0x82575fd0
  goto loc_82575FD0;
loc_82575FCC:
  // li r31,87
  r31 = uint64_t(int64_t(87));
loc_82575FD0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576080: lifted from generated/ by lift.py.
lifted::Result f82576080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82576080 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x825760A0)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576110
  if (!cr0.eq) goto loc_82576110;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r5,r30
  r5 = r30;
  // ori r4,r4,17
  r4 = r4 | 17;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // std r11,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r11);
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x825760D8)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x825760e8
  if (!cr0.lt) goto loc_825760E8;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x82576108
  goto loc_82576108;
loc_825760E8:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82576104
  if (!cr6.eq) goto loc_82576104;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825760F4)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x82576108
  goto loc_82576108;
loc_82576104:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_82576108:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576110)); r3 = o.r3; f1 = o.f1; }
loc_82576110:
  // mr r3,r31
  r3 = r31;
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

namespace {

lifted::Result e8246D310(uint8_t* base, const lifted::Args& a) {
  return f8246D310(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246D318(uint8_t* base, const lifted::Args& a) {
  return f8246D318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8246D328(uint8_t* base, const lifted::Args& a) {
  return f8246D328(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246D330(uint8_t* base, const lifted::Args& a) {
  return {f8246D330(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e8246D338(uint8_t* base, const lifted::Args& a) {
  return f8246D338(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e8246D340(uint8_t* base, const lifted::Args& a) {
  return f8246D340(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246D348(uint8_t* base, const lifted::Args& a) {
  return f8246D348(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246D738(uint8_t* base, const lifted::Args& a) {
  return f8246D738(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8246D740(uint8_t* base, const lifted::Args& a) {
  return f8246D740(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8246D748(uint8_t* base, const lifted::Args& a) {
  return f8246D748(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e824784D0(uint8_t* base, const lifted::Args& a) {
  f824784D0(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478660(uint8_t* base, const lifted::Args& a) {
  f82478660(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478770(uint8_t* base, const lifted::Args& a) {
  f82478770(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478890(uint8_t* base, const lifted::Args& a) {
  f82478890(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e824789C8(uint8_t* base, const lifted::Args& a) {
  f824789C8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478B18(uint8_t* base, const lifted::Args& a) {
  f82478B18(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478D30(uint8_t* base, const lifted::Args& a) {
  f82478D30(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82478FB8(uint8_t* base, const lifted::Args& a) {
  f82478FB8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479158(uint8_t* base, const lifted::Args& a) {
  f82479158(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479348(uint8_t* base, const lifted::Args& a) {
  f82479348(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479540(uint8_t* base, const lifted::Args& a) {
  f82479540(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479670(uint8_t* base, const lifted::Args& a) {
  f82479670(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e824797C8(uint8_t* base, const lifted::Args& a) {
  f824797C8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479968(uint8_t* base, const lifted::Args& a) {
  f82479968(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479B58(uint8_t* base, const lifted::Args& a) {
  f82479B58(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479CA8(uint8_t* base, const lifted::Args& a) {
  f82479CA8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479DE0(uint8_t* base, const lifted::Args& a) {
  f82479DE0(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82479F48(uint8_t* base, const lifted::Args& a) {
  f82479F48(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247A108(uint8_t* base, const lifted::Args& a) {
  f8247A108(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247A320(uint8_t* base, const lifted::Args& a) {
  f8247A320(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247A610(uint8_t* base, const lifted::Args& a) {
  f8247A610(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247A758(uint8_t* base, const lifted::Args& a) {
  f8247A758(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247A8C8(uint8_t* base, const lifted::Args& a) {
  f8247A8C8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247AA80(uint8_t* base, const lifted::Args& a) {
  f8247AA80(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8247DE28(uint8_t* base, const lifted::Args& a) {
  return f8247DE28(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247DF00(uint8_t* base, const lifted::Args& a) {
  return f8247DF00(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247DFD8(uint8_t* base, const lifted::Args& a) {
  return f8247DFD8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E0B0(uint8_t* base, const lifted::Args& a) {
  return f8247E0B0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E338(uint8_t* base, const lifted::Args& a) {
  return f8247E338(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E410(uint8_t* base, const lifted::Args& a) {
  return f8247E410(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E4E8(uint8_t* base, const lifted::Args& a) {
  return f8247E4E8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E5C8(uint8_t* base, const lifted::Args& a) {
  return f8247E5C8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E6A8(uint8_t* base, const lifted::Args& a) {
  return f8247E6A8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E788(uint8_t* base, const lifted::Args& a) {
  return f8247E788(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E868(uint8_t* base, const lifted::Args& a) {
  return f8247E868(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247E940(uint8_t* base, const lifted::Args& a) {
  return f8247E940(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247EA18(uint8_t* base, const lifted::Args& a) {
  return f8247EA18(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8247EAF0(uint8_t* base, const lifted::Args& a) {
  return f8247EAF0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82481B88(uint8_t* base, const lifted::Args& a) {
  return f82481B88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82481B98(uint8_t* base, const lifted::Args& a) {
  return f82481B98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10]);
}

lifted::Result e82482068(uint8_t* base, const lifted::Args& a) {
  return f82482068(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10]);
}

lifted::Result e82482730(uint8_t* base, const lifted::Args& a) {
  return f82482730(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10]);
}

lifted::Result e82482780(uint8_t* base, const lifted::Args& a) {
  return {f82482780(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82482968(uint8_t* base, const lifted::Args& a) {
  return f82482968(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82483130(uint8_t* base, const lifted::Args& a) {
  return f82483130(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10]);
}

lifted::Result e82483238(uint8_t* base, const lifted::Args& a) {
  return f82483238(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82483388(uint8_t* base, const lifted::Args& a) {
  return f82483388(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82483460(uint8_t* base, const lifted::Args& a) {
  return f82483460(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82483700(uint8_t* base, const lifted::Args& a) {
  return f82483700(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82483770(uint8_t* base, const lifted::Args& a) {
  return f82483770(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82483DC0(uint8_t* base, const lifted::Args& a) {
  return f82483DC0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82483DD0(uint8_t* base, const lifted::Args& a) {
  return {f82483DD0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82483F30(uint8_t* base, const lifted::Args& a) {
  return f82483F30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824847D0(uint8_t* base, const lifted::Args& a) {
  return f824847D0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82485350(uint8_t* base, const lifted::Args& a) {
  return f82485350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82485460(uint8_t* base, const lifted::Args& a) {
  return f82485460(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82486710(uint8_t* base, const lifted::Args& a) {
  return f82486710(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82486810(uint8_t* base, const lifted::Args& a) {
  return f82486810(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82486F90(uint8_t* base, const lifted::Args& a) {
  return f82486F90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824875A8(uint8_t* base, const lifted::Args& a) {
  return f824875A8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e824875B8(uint8_t* base, const lifted::Args& a) {
  return f824875B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82487660(uint8_t* base, const lifted::Args& a) {
  return f82487660(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e824877E0(uint8_t* base, const lifted::Args& a) {
  return f824877E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824878B0(uint8_t* base, const lifted::Args& a) {
  return f824878B0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e824878D0(uint8_t* base, const lifted::Args& a) {
  return f824878D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82487B28(uint8_t* base, const lifted::Args& a) {
  return f82487B28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82487BE8(uint8_t* base, const lifted::Args& a) {
  return f82487BE8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82488F50(uint8_t* base, const lifted::Args& a) {
  return f82488F50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82489048(uint8_t* base, const lifted::Args& a) {
  return f82489048(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824892A8(uint8_t* base, const lifted::Args& a) {
  f824892A8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e824892D0(uint8_t* base, const lifted::Args& a) {
  f824892D0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82489300(uint8_t* base, const lifted::Args& a) {
  return f82489300(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824893B8(uint8_t* base, const lifted::Args& a) {
  return f824893B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8248CA80(uint8_t* base, const lifted::Args& a) {
  return f8248CA80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8248CB58(uint8_t* base, const lifted::Args& a) {
  f8248CB58(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8248CBD0(uint8_t* base, const lifted::Args& a) {
  return f8248CBD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8248EC58(uint8_t* base, const lifted::Args& a) {
  return {f8248EC58(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8248ED78(uint8_t* base, const lifted::Args& a) {
  return f8248ED78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8248F218(uint8_t* base, const lifted::Args& a) {
  return f8248F218(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8248F550(uint8_t* base, const lifted::Args& a) {
  f8248F550(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82490960(uint8_t* base, const lifted::Args& a) {
  return f82490960(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824936E8(uint8_t* base, const lifted::Args& a) {
  return {f824936E8(base, a.r[0]), a.f[0]};
}

lifted::Result e824960B8(uint8_t* base, const lifted::Args& a) {
  return {f824960B8(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82496148(uint8_t* base, const lifted::Args& a) {
  return f82496148(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82575B68(uint8_t* base, const lifted::Args& a) {
  return f82575B68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[5], a.f[0]);
}

lifted::Result e82575BF0(uint8_t* base, const lifted::Args& a) {
  return {f82575BF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]), a.f[0]};
}

lifted::Result e82575C90(uint8_t* base, const lifted::Args& a) {
  return f82575C90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82575D78(uint8_t* base, const lifted::Args& a) {
  return f82575D78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82575E50(uint8_t* base, const lifted::Args& a) {
  return f82575E50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82576080(uint8_t* base, const lifted::Args& a) {
  return f82576080(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

const Registration kEntries[] = {
    {0x8246D310, e8246D310},
    {0x8246D318, e8246D318},
    {0x8246D328, e8246D328},
    {0x8246D330, e8246D330},
    {0x8246D338, e8246D338},
    {0x8246D340, e8246D340},
    {0x8246D348, e8246D348},
    {0x8246D738, e8246D738},
    {0x8246D740, e8246D740},
    {0x8246D748, e8246D748},
    {0x824784D0, e824784D0},
    {0x82478660, e82478660},
    {0x82478770, e82478770},
    {0x82478890, e82478890},
    {0x824789C8, e824789C8},
    {0x82478B18, e82478B18},
    {0x82478D30, e82478D30},
    {0x82478FB8, e82478FB8},
    {0x82479158, e82479158},
    {0x82479348, e82479348},
    {0x82479540, e82479540},
    {0x82479670, e82479670},
    {0x824797C8, e824797C8},
    {0x82479968, e82479968},
    {0x82479B58, e82479B58},
    {0x82479CA8, e82479CA8},
    {0x82479DE0, e82479DE0},
    {0x82479F48, e82479F48},
    {0x8247A108, e8247A108},
    {0x8247A320, e8247A320},
    {0x8247A610, e8247A610},
    {0x8247A758, e8247A758},
    {0x8247A8C8, e8247A8C8},
    {0x8247AA80, e8247AA80},
    {0x8247DE28, e8247DE28},
    {0x8247DF00, e8247DF00},
    {0x8247DFD8, e8247DFD8},
    {0x8247E0B0, e8247E0B0},
    {0x8247E338, e8247E338},
    {0x8247E410, e8247E410},
    {0x8247E4E8, e8247E4E8},
    {0x8247E5C8, e8247E5C8},
    {0x8247E6A8, e8247E6A8},
    {0x8247E788, e8247E788},
    {0x8247E868, e8247E868},
    {0x8247E940, e8247E940},
    {0x8247EA18, e8247EA18},
    {0x8247EAF0, e8247EAF0},
    {0x82481B88, e82481B88},
    {0x82481B98, e82481B98},
    {0x82482068, e82482068},
    {0x82482730, e82482730},
    {0x82482780, e82482780},
    {0x82482968, e82482968},
    {0x82483130, e82483130},
    {0x82483238, e82483238},
    {0x82483388, e82483388},
    {0x82483460, e82483460},
    {0x82483700, e82483700},
    {0x82483770, e82483770},
    {0x82483DC0, e82483DC0},
    {0x82483DD0, e82483DD0},
    {0x82483F30, e82483F30},
    {0x824847D0, e824847D0},
    {0x82485350, e82485350},
    {0x82485460, e82485460},
    {0x82486710, e82486710},
    {0x82486810, e82486810},
    {0x82486F90, e82486F90},
    {0x824875A8, e824875A8},
    {0x824875B8, e824875B8},
    {0x82487660, e82487660},
    {0x824877E0, e824877E0},
    {0x824878B0, e824878B0},
    {0x824878D0, e824878D0},
    {0x82487B28, e82487B28},
    {0x82487BE8, e82487BE8},
    {0x82488F50, e82488F50},
    {0x82489048, e82489048},
    {0x824892A8, e824892A8},
    {0x824892D0, e824892D0},
    {0x82489300, e82489300},
    {0x824893B8, e824893B8},
    {0x8248CA80, e8248CA80},
    {0x8248CB58, e8248CB58},
    {0x8248CBD0, e8248CBD0},
    {0x8248EC58, e8248EC58},
    {0x8248ED78, e8248ED78},
    {0x8248F218, e8248F218},
    {0x8248F550, e8248F550},
    {0x82490960, e82490960},
    {0x824936E8, e824936E8},
    {0x824960B8, e824960B8},
    {0x82496148, e82496148},
    {0x82575B68, e82575B68},
    {0x82575BF0, e82575BF0},
    {0x82575C90, e82575C90},
    {0x82575D78, e82575D78},
    {0x82575E50, e82575E50},
    {0x82576080, e82576080},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_017)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_8246D310) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D310(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D310);
}

extern "C" REX_FUNC(sub_8246D318) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D318(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D318);
}

extern "C" REX_FUNC(sub_8246D328) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D328(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D328);
}

extern "C" REX_FUNC(sub_8246D330) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D330(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D330);
}

extern "C" REX_FUNC(sub_8246D338) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D338(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D338);
}

extern "C" REX_FUNC(sub_8246D340) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D340(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D340);
}

extern "C" REX_FUNC(sub_8246D348) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D348(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D348);
}

extern "C" REX_FUNC(sub_8246D738) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D738(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D738);
}

extern "C" REX_FUNC(sub_8246D740) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D740(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D740);
}

extern "C" REX_FUNC(sub_8246D748) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8246D748(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246D748);
}

extern "C" REX_FUNC(sub_824784D0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824784D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824784D0);
}

extern "C" REX_FUNC(sub_82478660) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478660(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478660);
}

extern "C" REX_FUNC(sub_82478770) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478770(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478770);
}

extern "C" REX_FUNC(sub_82478890) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478890(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478890);
}

extern "C" REX_FUNC(sub_824789C8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824789C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824789C8);
}

extern "C" REX_FUNC(sub_82478B18) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478B18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478B18);
}

extern "C" REX_FUNC(sub_82478D30) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478D30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478D30);
}

extern "C" REX_FUNC(sub_82478FB8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82478FB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82478FB8);
}

extern "C" REX_FUNC(sub_82479158) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479158(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479158);
}

extern "C" REX_FUNC(sub_82479348) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479348(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479348);
}

extern "C" REX_FUNC(sub_82479540) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479540(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479540);
}

extern "C" REX_FUNC(sub_82479670) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479670(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479670);
}

extern "C" REX_FUNC(sub_824797C8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824797C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824797C8);
}

extern "C" REX_FUNC(sub_82479968) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479968(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479968);
}

extern "C" REX_FUNC(sub_82479B58) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479B58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479B58);
}

extern "C" REX_FUNC(sub_82479CA8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479CA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479CA8);
}

extern "C" REX_FUNC(sub_82479DE0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479DE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479DE0);
}

extern "C" REX_FUNC(sub_82479F48) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82479F48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82479F48);
}

extern "C" REX_FUNC(sub_8247A108) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247A108(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247A108);
}

extern "C" REX_FUNC(sub_8247A320) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247A320(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247A320);
}

extern "C" REX_FUNC(sub_8247A610) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247A610(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247A610);
}

extern "C" REX_FUNC(sub_8247A758) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247A758(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247A758);
}

extern "C" REX_FUNC(sub_8247A8C8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247A8C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247A8C8);
}

extern "C" REX_FUNC(sub_8247AA80) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247AA80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247AA80);
}

extern "C" REX_FUNC(sub_8247DE28) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247DE28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247DE28);
}

extern "C" REX_FUNC(sub_8247DF00) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247DF00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247DF00);
}

extern "C" REX_FUNC(sub_8247DFD8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247DFD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247DFD8);
}

extern "C" REX_FUNC(sub_8247E0B0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E0B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E0B0);
}

extern "C" REX_FUNC(sub_8247E338) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E338(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E338);
}

extern "C" REX_FUNC(sub_8247E410) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E410(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E410);
}

extern "C" REX_FUNC(sub_8247E4E8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E4E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E4E8);
}

extern "C" REX_FUNC(sub_8247E5C8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E5C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E5C8);
}

extern "C" REX_FUNC(sub_8247E6A8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E6A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E6A8);
}

extern "C" REX_FUNC(sub_8247E788) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E788(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E788);
}

extern "C" REX_FUNC(sub_8247E868) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E868(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E868);
}

extern "C" REX_FUNC(sub_8247E940) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247E940(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247E940);
}

extern "C" REX_FUNC(sub_8247EA18) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247EA18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247EA18);
}

extern "C" REX_FUNC(sub_8247EAF0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8247EAF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8247EAF0);
}

extern "C" REX_FUNC(sub_82481B88) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82481B88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82481B88);
}

extern "C" REX_FUNC(sub_82481B98) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82481B98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82481B98);
}

extern "C" REX_FUNC(sub_82482068) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82482068(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82482068);
}

extern "C" REX_FUNC(sub_82482730) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82482730(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82482730);
}

extern "C" REX_FUNC(sub_82482780) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82482780(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82482780);
}

extern "C" REX_FUNC(sub_82482968) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82482968(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82482968);
}

extern "C" REX_FUNC(sub_82483130) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483130(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483130);
}

extern "C" REX_FUNC(sub_82483238) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483238);
}

extern "C" REX_FUNC(sub_82483388) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483388(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483388);
}

extern "C" REX_FUNC(sub_82483460) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483460);
}

extern "C" REX_FUNC(sub_82483700) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483700(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483700);
}

extern "C" REX_FUNC(sub_82483770) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483770(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483770);
}

extern "C" REX_FUNC(sub_82483DC0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483DC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483DC0);
}

extern "C" REX_FUNC(sub_82483DD0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483DD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483DD0);
}

extern "C" REX_FUNC(sub_82483F30) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82483F30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82483F30);
}

extern "C" REX_FUNC(sub_824847D0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824847D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824847D0);
}

extern "C" REX_FUNC(sub_82485350) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82485350(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82485350);
}

extern "C" REX_FUNC(sub_82485460) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82485460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82485460);
}

extern "C" REX_FUNC(sub_82486710) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82486710(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82486710);
}

extern "C" REX_FUNC(sub_82486810) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82486810(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82486810);
}

extern "C" REX_FUNC(sub_82486F90) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82486F90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82486F90);
}

extern "C" REX_FUNC(sub_824875A8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824875A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824875A8);
}

extern "C" REX_FUNC(sub_824875B8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824875B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824875B8);
}

extern "C" REX_FUNC(sub_82487660) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82487660(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82487660);
}

extern "C" REX_FUNC(sub_824877E0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824877E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824877E0);
}

extern "C" REX_FUNC(sub_824878B0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824878B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824878B0);
}

extern "C" REX_FUNC(sub_824878D0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824878D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824878D0);
}

extern "C" REX_FUNC(sub_82487B28) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82487B28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82487B28);
}

extern "C" REX_FUNC(sub_82487BE8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82487BE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82487BE8);
}

extern "C" REX_FUNC(sub_82488F50) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82488F50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82488F50);
}

extern "C" REX_FUNC(sub_82489048) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82489048(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82489048);
}

extern "C" REX_FUNC(sub_824892A8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824892A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824892A8);
}

extern "C" REX_FUNC(sub_824892D0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824892D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824892D0);
}

extern "C" REX_FUNC(sub_82489300) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82489300(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82489300);
}

extern "C" REX_FUNC(sub_824893B8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824893B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824893B8);
}

extern "C" REX_FUNC(sub_8248CA80) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248CA80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248CA80);
}

extern "C" REX_FUNC(sub_8248CB58) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248CB58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248CB58);
}

extern "C" REX_FUNC(sub_8248CBD0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248CBD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248CBD0);
}

extern "C" REX_FUNC(sub_8248EC58) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248EC58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248EC58);
}

extern "C" REX_FUNC(sub_8248ED78) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248ED78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248ED78);
}

extern "C" REX_FUNC(sub_8248F218) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248F218(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248F218);
}

extern "C" REX_FUNC(sub_8248F550) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_8248F550(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8248F550);
}

extern "C" REX_FUNC(sub_82490960) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82490960(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82490960);
}

extern "C" REX_FUNC(sub_824936E8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824936E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824936E8);
}

extern "C" REX_FUNC(sub_824960B8) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_824960B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e824960B8);
}

extern "C" REX_FUNC(sub_82496148) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82496148(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82496148);
}

extern "C" REX_FUNC(sub_82575B68) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82575B68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82575B68);
}

extern "C" REX_FUNC(sub_82575BF0) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82575BF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82575BF0);
}

extern "C" REX_FUNC(sub_82575C90) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82575C90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82575C90);
}

extern "C" REX_FUNC(sub_82575D78) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82575D78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82575D78);
}

extern "C" REX_FUNC(sub_82575E50) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82575E50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82575E50);
}

extern "C" REX_FUNC(sub_82576080) {
  if (!ITC_FLAG(native_lifted_017)) {
    __imp__sub_82576080(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82576080);
}
