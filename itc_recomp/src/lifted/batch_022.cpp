// Lift sweep batch 022 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_022, true, "ITC",
              "Run lift sweep batch 022's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_825C0248: lifted from generated/ by lift.py.
lifted::Result f825C0248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0248 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r28,r3
  r28 = r3;
  // lwz r11,180(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 180);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c0354
  if (!cr6.eq) goto loc_825C0354;
  // addi r27,r28,144
  r27 = uint64_t(int64_t(int64_t(r28) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0270, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,180(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 180);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c034c
  if (!cr6.eq) goto loc_825C034C;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825C0280, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r29,r28,28
  r29 = uint64_t(int64_t(int64_t(r28) + 28));
  // lwz r11,-12812(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -12812);
  // add r11,r3,r11
  r11 = r3 + r11;
  // stw r11,24(r28)
  REX_STORE_U32(uint32_t(r28) + 24, uint32_t(r11));
  // lwz r30,32(r28)
  r30 = REX_LOAD_U32(uint32_t(r28) + 32);
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x825c02e0
  if (cr6.eq) goto loc_825C02E0;
loc_825C02A0:
  // lwz r31,4(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825c02c0
  if (cr6.eq) goto loc_825C02C0;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r3,r31,-12
  r3 = uint64_t(int64_t(int64_t(r31) + -12));
  // bne cr6,0x825c02bc
  if (!cr6.eq) goto loc_825C02BC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C02BC:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x825C02C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C02C0:
  // addi r3,r30,-12
  r3 = uint64_t(int64_t(int64_t(r30) + -12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C02D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r31
  r30 = r31;
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825c02a0
  if (!cr6.eq) goto loc_825C02A0;
loc_825C02E0:
  // lwz r11,60(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 60);
  // addi r30,r28,56
  r30 = uint64_t(int64_t(int64_t(r28) + 56));
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825c0314
  if (cr6.eq) goto loc_825C0314;
loc_825C02F0:
  // addi r3,r11,-8
  r3 = uint64_t(int64_t(int64_t(r11) + -8));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0308, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c02f0
  if (!cr6.eq) goto loc_825C02F0;
loc_825C0314:
  // lwz r11,184(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 184);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c034c
  if (cr6.eq) goto loc_825C034C;
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0328, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,12(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 12);
  // stw r11,184(r28)
  REX_STORE_U32(uint32_t(r28) + 184, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0344, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C034C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C034C:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0354, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C0354:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0458: lifted from generated/ by lift.py.
lifted::Result f825C0458(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0458 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,124(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 124);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0468
  if (cr6.eq) goto loc_825C0468;
  // b 0x825c4138
  { const lifted::Result o = f825C4138(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825C0468:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return {r3, f1};
}

// sub_825C0470: lifted from generated/ by lift.py.
lifted::Result f825C0470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0470 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,124(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 124);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0480
  if (cr6.eq) goto loc_825C0480;
  // b 0x825c4580
  { const lifted::Result o = f825C4580(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825C0480:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return {r3, f1};
}

// sub_825C0488: lifted from generated/ by lift.py.
lifted::Result f825C0488(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C0488 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c04b0
  if (cr6.eq) goto loc_825C04B0;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
loc_825C04B0:
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r5,8(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r4,12(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C05C8: lifted from generated/ by lift.py.
lifted::Result f825C05C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C05C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
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
  // addi r25,r31,144
  r25 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r27,r5
  r27 = r5;
  // mr r3,r25
  r3 = r25;
  // mr r26,r6
  r26 = r6;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C05F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,132(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 132);
  // addi r28,r31,128
  r28 = uint64_t(int64_t(int64_t(r31) + 128));
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // beq cr6,0x825c0688
  if (cr6.eq) goto loc_825C0688;
loc_825C0600:
  // lwz r29,4(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addi r31,r11,-12
  r31 = uint64_t(int64_t(int64_t(r11) + -12));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c0630
  if (cr6.eq) goto loc_825C0630;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,104(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 104);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0628, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c067c
  if (cr6.eq) goto loc_825C067C;
loc_825C0630:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825c0644
  if (cr6.eq) goto loc_825C0644;
  // lwz r11,296(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // bne cr6,0x825c067c
  if (!cr6.eq) goto loc_825C067C;
loc_825C0644:
  // clrlwi r11,r26,16
  r11 = uint32_t(r26) & 0xFFFF;
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c065c
  if (cr6.eq) goto loc_825C065C;
  // lhz r10,304(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x825c067c
  if (!cr6.eq) goto loc_825C067C;
loc_825C065C:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c067c
  if (cr6.eq) goto loc_825C067C;
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C067C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C067C:
  // mr r11,r29
  r11 = r29;
  // cmplw cr6,r29,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r28), so);
  // bne cr6,0x825c0600
  if (!cr6.eq) goto loc_825C0600;
loc_825C0688:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0690, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0698: lifted from generated/ by lift.py.
lifted::Result f825C0698(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0698 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r31,124(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 124);
  // mr r30,r4
  r30 = r4;
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C06C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r10,11(r3)
  r10 = REX_LOAD_U16(uint32_t(r3) + 11);
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x825c06e8
  if (!cr6.lt) goto loc_825C06E8;
  // lwz r10,124(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 124);
  // mulli r11,r11,252
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(252))));
  // add r3,r11,r10
  r3 = r11 + r10;
  // b 0x825c06ec
  goto loc_825C06EC;
loc_825C06E8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C06EC:
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

// sub_825C0730: lifted from generated/ by lift.py.
lifted::Result f825C0730(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0730 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,124(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 124);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0740
  if (cr6.eq) goto loc_825C0740;
  // b 0x825c39f8
  { const lifted::Result o = f825C39F8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825C0740:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // blr
  return {r3, f1};
}

// sub_825C07F0: lifted from generated/ by lift.py.
lifted::Result f825C07F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C07F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0814, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r30
  r3 = r30;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addi r31,r10,1
  r31 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C082C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C0848: lifted from generated/ by lift.py.
lifted::Result f825C0848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0848 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C086C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // bne 0x825c0898
  if (!cr0.eq) goto loc_825C0898;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0890, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x825c089c
  goto loc_825C089C;
loc_825C0898:
  // mr r31,r11
  r31 = r11;
loc_825C089C:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C08A4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C08C0: lifted from generated/ by lift.py.
lifted::Result f825C08C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C08C0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r31,32(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 32);
  // addi r30,r3,28
  r30 = uint64_t(int64_t(int64_t(r3) + 28));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825c094c
  if (cr6.eq) goto loc_825C094C;
loc_825C08E8:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r3,r31,-12
  r3 = uint64_t(int64_t(int64_t(r31) + -12));
  // bne cr6,0x825c08f8
  if (!cr6.eq) goto loc_825C08F8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C08F8:
  // lwz r11,296(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 296);
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x825c0940
  if (!cr6.eq) goto loc_825C0940;
  // lhz r11,304(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 304);
  // clrlwi r10,r28,16
  r10 = uint32_t(r28) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c0940
  if (!cr6.eq) goto loc_825C0940;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825c5758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5758, LIFTED_ARGS(0x825C091C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0940
  if (cr6.eq) goto loc_825C0940;
  // extsw r11,r27
  r11 = uint64_t(int64_t(int32_t(r27)));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // bl 0x825c1f88
  // (left out: dead in C++)
  { const lifted::Result o = f825C1F88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_825C0940:
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c08e8
  if (!cr6.eq) goto loc_825C08E8;
loc_825C094C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0958: lifted from generated/ by lift.py.
lifted::Result f825C0958(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0958 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825c0994
  if (cr6.eq) goto loc_825C0994;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32777
  r4 = r4 | 32777;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C0990, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C0994:
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

// sub_825C0B48: lifted from generated/ by lift.py.
lifted::Result f825C0B48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0B48 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x825c1d90
  { const lifted::Result o = f825C1D90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C0B50: lifted from generated/ by lift.py.
lifted::Result f825C0B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0B50 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x825c0848
  { const lifted::Result o = f825C0848(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C0B58: lifted from generated/ by lift.py.
lifted::Result f825C0B58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0B58 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x825c07f0
  { const lifted::Result o = f825C07F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C0B60: lifted from generated/ by lift.py.
lifted::Result f825C0B60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0B60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // addi r23,r31,144
  r23 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r27,r5
  r27 = r5;
  // mr r3,r23
  r3 = r23;
  // mr r29,r6
  r29 = r6;
  // mr r26,r7
  r26 = r7;
  // mr r24,r8
  r24 = r8;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0B90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r30,r3
  r30 = r3;
  // clrlwi r25,r29,31
  r25 = uint32_t(r29) & 0x1;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c0bfc
  if (cr6.lt) goto loc_825C0BFC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,268
  r3 = uint64_t(int64_t(268));
  // ori r4,r4,32793
  r4 = r4 | 32793;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C0BB8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0c10
  if (cr6.eq) goto loc_825C0C10;
  // mr r4,r31
  r4 = r31;
  // bl 0x825c79b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C79B8, LIFTED_ARGS(0x825C0BC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c0c18
  if (!cr6.eq) goto loc_825C0C18;
loc_825C0BD4:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,14
  r30 = r30 | 14;
loc_825C0BDC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825c0bfc
  if (cr6.eq) goto loc_825C0BFC;
  // addi r3,r29,12
  r3 = uint64_t(int64_t(int64_t(r29) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0BFC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C0BFC:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0C04, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0C10:
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // b 0x825c0bd4
  goto loc_825C0BD4;
loc_825C0C18:
  // mr r7,r26
  r7 = r26;
  // mr r6,r25
  r6 = r25;
  // mr r5,r27
  r5 = r27;
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x825c7e40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7E40, LIFTED_ARGS(0x825C0C30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c0bdc
  if (cr6.lt) goto loc_825C0BDC;
  // addi r11,r31,44
  r11 = uint64_t(int64_t(int64_t(r31) + 44));
  // addi r10,r29,12
  r10 = uint64_t(int64_t(int64_t(r29) + 12));
  // mr r3,r23
  r3 = r23;
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r29,0(r24)
  REX_STORE_U32(uint32_t(r24) + 0, uint32_t(r29));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0C68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0C78: lifted from generated/ by lift.py.
lifted::Result f825C0C78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0C78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // addi r23,r31,144
  r23 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r27,r5
  r27 = r5;
  // mr r3,r23
  r3 = r23;
  // mr r29,r6
  r29 = r6;
  // mr r26,r7
  r26 = r7;
  // mr r24,r8
  r24 = r8;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0CA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r30,r3
  r30 = r3;
  // clrlwi r25,r29,31
  r25 = uint32_t(r29) & 0x1;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c0d14
  if (cr6.lt) goto loc_825C0D14;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,292
  r3 = uint64_t(int64_t(292));
  // ori r4,r4,32796
  r4 = r4 | 32796;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C0CD0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0d28
  if (cr6.eq) goto loc_825C0D28;
  // mr r4,r31
  r4 = r31;
  // bl 0x825c8310
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C8310, LIFTED_ARGS(0x825C0CE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c0d30
  if (!cr6.eq) goto loc_825C0D30;
loc_825C0CEC:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,14
  r30 = r30 | 14;
loc_825C0CF4:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825c0d14
  if (cr6.eq) goto loc_825C0D14;
  // addi r3,r29,8
  r3 = uint64_t(int64_t(int64_t(r29) + 8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0D14, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C0D14:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0D1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0D28:
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // b 0x825c0cec
  goto loc_825C0CEC;
loc_825C0D30:
  // mr r7,r26
  r7 = r26;
  // mr r6,r25
  r6 = r25;
  // mr r5,r27
  r5 = r27;
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x825c83d8
  // (left out: dead in C++)
  { const lifted::Result o = f825C83D8(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c0cf4
  if (cr6.lt) goto loc_825C0CF4;
  // addi r11,r31,56
  r11 = uint64_t(int64_t(int64_t(r31) + 56));
  // addi r10,r29,8
  r10 = uint64_t(int64_t(int64_t(r29) + 8));
  // mr r3,r23
  r3 = r23;
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r29,0(r24)
  REX_STORE_U32(uint32_t(r24) + 0, uint32_t(r29));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0D80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0D90: lifted from generated/ by lift.py.
lifted::Result f825C0D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0D90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // addi r26,r30,144
  r26 = uint64_t(int64_t(int64_t(r30) + 144));
  // mr r27,r5
  r27 = r5;
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0DB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c0e1c
  if (cr6.lt) goto loc_825C0E1C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,340
  r3 = uint64_t(int64_t(340));
  // ori r4,r4,32797
  r4 = r4 | 32797;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C0DD8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0e30
  if (cr6.eq) goto loc_825C0E30;
  // mr r4,r30
  r4 = r30;
  // bl 0x825c8698
  // (left out: dead in C++)
  r3 = f825C8698(base, r1, r13, r3, r4);
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825c0e38
  if (!cr6.eq) goto loc_825C0E38;
loc_825C0DF4:
  // lis r29,-32761
  r29 = uint64_t(int64_t(-2147024896));
  // ori r29,r29,14
  r29 = r29 | 14;
loc_825C0DFC:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c0e1c
  if (cr6.eq) goto loc_825C0E1C;
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0E1C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C0E1C:
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0E24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0E30:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x825c0df4
  goto loc_825C0DF4;
loc_825C0E38:
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c8808
  // (left out: dead in C++)
  { const lifted::Result o = f825C8808(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c0dfc
  if (cr6.lt) goto loc_825C0DFC;
  // addi r11,r30,56
  r11 = uint64_t(int64_t(int64_t(r30) + 56));
  // addi r10,r31,8
  r10 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r3,r26
  r3 = r26;
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r31,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r31));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0E7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0E88: lifted from generated/ by lift.py.
lifted::Result f825C0E88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0E88 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // addi r28,r31,144
  r28 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0EA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c0f5c
  if (cr6.lt) goto loc_825C0F5C;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c0ee4
  if (!cr6.eq) goto loc_825C0EE4;
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // mr r3,r28
  r3 = r28;
  // ori r30,r30,4
  r30 = r30 | 4;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0EE4:
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c8c98
  // (left out: dead in C++)
  { const lifted::Result o = f825C8C98(base, r1, r13, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825c0f14
  if (!cr6.eq) goto loc_825C0F14;
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // mr r3,r28
  r3 = r28;
  // ori r30,r30,87
  r30 = r30 | 87;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0F08, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0F14:
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,36
  r3 = uint64_t(int64_t(36));
  // ori r4,r4,32799
  r4 = r4 | 32799;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C0F24, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0f70
  if (cr6.eq) goto loc_825C0F70;
  // mr r4,r29
  r4 = r29;
  // bl 0x825c8e68
  // (left out: dead in C++)
  r3 = f825C8E68(base, r1, r13, r3, r4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0f70
  if (cr6.eq) goto loc_825C0F70;
  // addi r11,r31,100
  r11 = uint64_t(int64_t(int64_t(r31) + 100));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r3,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r3));
  // stw r3,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r3));
loc_825C0F5C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0F64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0F70:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // mr r3,r28
  r3 = r28;
  // ori r30,r30,14
  r30 = r30 | 14;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0F80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C0F90: lifted from generated/ by lift.py.
lifted::Result f825C0F90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C0F90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // addi r27,r31,144
  r27 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C0FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c1078
  if (cr6.lt) goto loc_825C1078;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c0fec
  if (!cr6.eq) goto loc_825C0FEC;
  // lis r28,-30009
  r28 = uint64_t(int64_t(-1966669824));
  // mr r3,r27
  r3 = r27;
  // ori r28,r28,4
  r28 = r28 | 4;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0FE0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0FEC:
  // lwz r3,104(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 104);
  // addi r29,r31,100
  r29 = uint64_t(int64_t(int64_t(r31) + 100));
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // beq cr6,0x825c1078
  if (cr6.eq) goto loc_825C1078;
loc_825C0FFC:
  // lwz r31,4(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c103c
  if (cr6.eq) goto loc_825C103C;
  // addi r11,r3,16
  r11 = uint64_t(int64_t(int64_t(r3) + 16));
  // mr r10,r30
  r10 = r30;
  // addi r8,r11,20
  r8 = uint64_t(int64_t(int64_t(r11) + 20));
loc_825C1014:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = REX_LOAD_U8(uint32_t(r10) + 0);
  // subf. r9,r7,r9
  r9 = r9 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x825c1034
  if (!cr0.eq) goto loc_825C1034;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // bne cr6,0x825c1014
  if (!cr6.eq) goto loc_825C1014;
loc_825C1034:
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x825c106c
  if (!cr6.eq) goto loc_825C106C;
loc_825C103C:
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C106C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C106C:
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825c0ffc
  if (!cr6.eq) goto loc_825C0FFC;
loc_825C1078:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1080, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1090: lifted from generated/ by lift.py.
lifted::Result f825C1090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C1090 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // addi r25,r31,144
  r25 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r26,r5
  r26 = r5;
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C10B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r29,124(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 124);
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c10e0
  if (!cr6.eq) goto loc_825C10E0;
  // lis r24,-30009
  r24 = uint64_t(int64_t(-1966669824));
  // mr r3,r25
  r3 = r25;
  // ori r24,r24,9
  r24 = r24 | 9;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C10D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C10E0:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C10F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 9);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1180
  if (cr6.eq) goto loc_825C1180;
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // addi r27,r31,28
  r27 = uint64_t(int64_t(int64_t(r31) + 28));
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x825c1188
  if (cr6.eq) goto loc_825C1188;
loc_825C1118:
  // addi r31,r11,-12
  r31 = uint64_t(int64_t(int64_t(r11) + -12));
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1160
  if (cr6.eq) goto loc_825C1160;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mr r5,r28
  r5 = r28;
  // mr r3,r29
  r3 = r29;
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C1140, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c1160
  if (cr6.eq) goto loc_825C1160;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1160, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1160:
  // mr r11,r30
  r11 = r30;
  // cmplw cr6,r30,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r27), so);
  // bne cr6,0x825c1118
  if (!cr6.eq) goto loc_825C1118;
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1174, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C1180:
  // lis r24,-30009
  r24 = uint64_t(int64_t(-1966669824));
  // ori r24,r24,11
  r24 = r24 | 11;
loc_825C1188:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1190, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C11A0: lifted from generated/ by lift.py.
lifted::Result f825C11A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C11A0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // addi r28,r31,144
  r28 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C11C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,124(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 124);
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c11dc
  if (cr6.eq) goto loc_825C11DC;
  // mr r4,r29
  r4 = r29;
  // bl 0x825c39f8
  // (left out: dead in C++)
  { const lifted::Result o = f825C39F8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825C11DC:
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c1224
  if (cr6.eq) goto loc_825C1224;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c1220
  if (cr6.eq) goto loc_825C1220;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // not r10,r11
  r10 = ~r11;
  // clrlwi r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825c1220
  if (!cr6.eq) goto loc_825C1220;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x825c1224
  if (!cr6.eq) goto loc_825C1224;
loc_825C1220:
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
loc_825C1224:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C122C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1238: lifted from generated/ by lift.py.
lifted::Result f825C1238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1238 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-48(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // mr r30,r4
  r30 = r4;
  // addi r28,r31,144
  r28 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1260, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c12d8
  if (cr6.lt) goto loc_825C12D8;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c12d0
  if (cr6.eq) goto loc_825C12D0;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825c12d0
  if (cr6.eq) goto loc_825C12D0;
  // rlwinm r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // beq cr6,0x825c12d0
  if (cr6.eq) goto loc_825C12D0;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c12d0
  if (!cr6.eq) goto loc_825C12D0;
  // lfs f0,20(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // beq cr6,0x825c12d8
  if (cr6.eq) goto loc_825C12D8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825c1f88
  // (left out: dead in C++)
  { const lifted::Result o = f825C1F88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825c12d8
  goto loc_825C12D8;
loc_825C12D0:
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // ori r29,r29,10
  r29 = r29 | 10;
loc_825C12D8:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C12E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C12F0: lifted from generated/ by lift.py.
lifted::Result f825C12F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C12F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // addi r29,r31,144
  r29 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r28,r5
  r28 = r5;
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1314, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c1368
  if (cr6.eq) goto loc_825C1368;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825c1368
  if (cr6.eq) goto loc_825C1368;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c1368
  if (!cr6.eq) goto loc_825C1368;
  // lfs f0,20(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f0 = double(temp.f32);
  // mr r3,r29
  r3 = r29;
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C135C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C1368:
  // lis r31,-30009
  r31 = uint64_t(int64_t(-1966669824));
  // mr r3,r29
  r3 = r29;
  // ori r31,r31,10
  r31 = r31 | 10;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1378, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1548: lifted from generated/ by lift.py.
void f825C1548(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C1548 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r3,76
  r11 = uint64_t(int64_t(int64_t(r3) + 76));
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

// sub_825C1568: lifted from generated/ by lift.py.
lifted::Result f825C1568(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1568 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r30,r3,76
  r30 = uint64_t(int64_t(int64_t(r3) + 76));
  // lwz r3,80(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 80);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825c15cc
  if (cr6.eq) goto loc_825C15CC;
loc_825C158C:
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r31,4(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C15C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c158c
  if (!cr6.eq) goto loc_825C158C;
loc_825C15CC:
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

// sub_825C15E8: lifted from generated/ by lift.py.
lifted::Result f825C15E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C15E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r25,r4
  r25 = r4;
  // mr r30,r5
  r30 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r6,r30
  r6 = r30;
  // mr r5,r25
  r5 = r25;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r28,r3
  r28 = r3;
  // mr r24,r7
  r24 = r7;
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // bl 0x825c05c8
  // (left out: dead in C++)
  { const lifted::Result o = f825C05C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r26,r28,144
  r26 = uint64_t(int64_t(int64_t(r28) + 144));
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1628, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 32);
  // addi r23,r28,28
  r23 = uint64_t(int64_t(int64_t(r28) + 28));
loc_825C1630:
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // beq cr6,0x825c16b4
  if (cr6.eq) goto loc_825C16B4;
  // clrlwi r10,r30,16
  r10 = uint32_t(r30) & 0xFFFF;
  // lwz r29,4(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addi r31,r11,-12
  r31 = uint64_t(int64_t(int64_t(r11) + -12));
  // cmplwi cr6,r10,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 65535, so);
  // beq cr6,0x825c1658
  if (cr6.eq) goto loc_825C1658;
  // lhz r11,304(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c16a8
  if (!cr6.eq) goto loc_825C16A8;
loc_825C1658:
  // lwz r11,296(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplw cr6,r11,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r25), so);
  // bne cr6,0x825c16a8
  if (!cr6.eq) goto loc_825C16A8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1678, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c16a8
  if (!cr6.eq) goto loc_825C16A8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cntlzw r10,r27
  r10 = uint32_t(r27) == 0 ? 32 : __builtin_clz(uint32_t(r27));
  // mr r3,r31
  r3 = r31;
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // xori r4,r10,1
  r4 = r10 ^ 1;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C16A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r22,r3
  r22 = r3;
loc_825C16A8:
  // mr r11,r29
  r11 = r29;
  // cmpwi cr6,r22,0
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 0, so);
  // bge cr6,0x825c1630
  if (!cr6.lt) goto loc_825C1630;
loc_825C16B4:
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C16BC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r22,0
  cr6 = lifted::Compare<int32_t>(int32_t(r22), 0, so);
  // blt cr6,0x825c1728
  if (cr6.lt) goto loc_825C1728;
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // beq cr6,0x825c1728
  if (cr6.eq) goto loc_825C1728;
loc_825C16CC:
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // mr r3,r28
  r3 = r28;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C16E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 32);
  // cmplw cr6,r11,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r23), so);
  // beq cr6,0x825c1728
  if (cr6.eq) goto loc_825C1728;
loc_825C16EC:
  // addi r3,r11,-12
  r3 = uint64_t(int64_t(int64_t(r11) + -12));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,296(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 296);
  // cmplw cr6,r11,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r25), so);
  // bne cr6,0x825c171c
  if (!cr6.eq) goto loc_825C171C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1710, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c16cc
  if (cr6.eq) goto loc_825C16CC;
loc_825C171C:
  // mr r11,r31
  r11 = r31;
  // cmplw cr6,r31,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r23), so);
  // bne cr6,0x825c16ec
  if (!cr6.eq) goto loc_825C16EC;
loc_825C1728:
  // mr r3,r22
  r3 = r22;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1738: lifted from generated/ by lift.py.
void f825C1738(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C1738 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // addi r11,r4,12
  r11 = uint64_t(int64_t(int64_t(r4) + 12));
  // bne cr6,0x825c1748
  if (!cr6.eq) goto loc_825C1748;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825C1748:
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r10,r3,128
  r10 = uint64_t(int64_t(int64_t(r3) + 128));
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r8));
  // stw r11,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r11));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r9,8(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // lwz r9,8(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r11,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r11));
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
  // blr
  return;
}

// sub_825C1788: lifted from generated/ by lift.py.
lifted::Result f825C1788(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1788 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,132(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 132);
  // addi r27,r3,128
  r27 = uint64_t(int64_t(int64_t(r3) + 128));
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x825c17f8
  if (cr6.eq) goto loc_825C17F8;
loc_825C17AC:
  // addi r31,r11,-12
  r31 = uint64_t(int64_t(int64_t(r11) + -12));
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mr r5,r28
  r5 = r28;
  // mr r3,r29
  r3 = r29;
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C17CC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c17ec
  if (cr6.eq) goto loc_825C17EC;
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C17EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C17EC:
  // mr r11,r30
  r11 = r30;
  // cmplw cr6,r30,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r27), so);
  // bne cr6,0x825c17ac
  if (!cr6.eq) goto loc_825C17AC;
loc_825C17F8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1800: lifted from generated/ by lift.py.
lifted::Result f825C1800(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1800 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c1984
  if (cr6.lt) goto loc_825C1984;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1858
  if (cr6.eq) goto loc_825C1858;
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1838, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1850, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1858, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1858:
  // lwz r3,116(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 116);
  // addi r29,r31,112
  r29 = uint64_t(int64_t(int64_t(r31) + 112));
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // beq cr6,0x825c18a8
  if (cr6.eq) goto loc_825C18A8;
loc_825C1868:
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r30,4(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C189C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825c1868
  if (!cr6.eq) goto loc_825C1868;
loc_825C18A8:
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // addi r29,r31,44
  r29 = uint64_t(int64_t(int64_t(r31) + 44));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // beq cr6,0x825c18dc
  if (cr6.eq) goto loc_825C18DC;
loc_825C18B8:
  // addi r3,r11,-12
  r3 = uint64_t(int64_t(int64_t(r11) + -12));
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C18D0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r30
  r11 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825c18b8
  if (!cr6.eq) goto loc_825C18B8;
loc_825C18DC:
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // addi r29,r31,56
  r29 = uint64_t(int64_t(int64_t(r31) + 56));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // beq cr6,0x825c1910
  if (cr6.eq) goto loc_825C1910;
loc_825C18EC:
  // addi r3,r11,-8
  r3 = uint64_t(int64_t(int64_t(r11) + -8));
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1904, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r30
  r11 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825c18ec
  if (!cr6.eq) goto loc_825C18EC;
loc_825C1910:
  // addi r30,r31,144
  r30 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C191C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825c1568
  // (left out: dead in C++)
  { const lifted::Result o = f825C1568(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C192C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,124(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 124);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c194c
  if (cr6.eq) goto loc_825C194C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C194C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C194C:
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r29,124(r31)
  REX_STORE_U32(uint32_t(r31) + 124, uint32_t(r29));
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C195C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stw r11,180(r31)
  REX_STORE_U32(uint32_t(r31) + 180, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1978, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // stw r29,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r29));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1984, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1984:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1990: lifted from generated/ by lift.py.
lifted::Result f825C1990(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C1990 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // mr r27,r3
  r27 = r3;
  // addi r25,r27,144
  r25 = uint64_t(int64_t(int64_t(r27) + 144));
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C19AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r24,r3
  r24 = r3;
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // blt cr6,0x825c1bac
  if (cr6.lt) goto loc_825C1BAC;
  // mr r3,r27
  r3 = r27;
  // bl 0x825c1568
  // (left out: dead in C++)
  { const lifted::Result o = f825C1568(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,116(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 116);
  // addi r30,r27,112
  r30 = uint64_t(int64_t(int64_t(r27) + 112));
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // beq cr6,0x825c1a18
  if (cr6.eq) goto loc_825C1A18;
loc_825C19D8:
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r31,4(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1A0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c19d8
  if (!cr6.eq) goto loc_825C19D8;
loc_825C1A18:
  // lwz r30,32(r27)
  r30 = REX_LOAD_U32(uint32_t(r27) + 32);
  // addi r29,r27,28
  r29 = uint64_t(int64_t(int64_t(r27) + 28));
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // beq cr6,0x825c1a68
  if (cr6.eq) goto loc_825C1A68;
loc_825C1A28:
  // lwz r31,4(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825c1a48
  if (cr6.eq) goto loc_825C1A48;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r3,r31,-12
  r3 = uint64_t(int64_t(int64_t(r31) + -12));
  // bne cr6,0x825c1a44
  if (!cr6.eq) goto loc_825C1A44;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C1A44:
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x825C1A48, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1A48:
  // addi r3,r30,-12
  r3 = uint64_t(int64_t(int64_t(r30) + -12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,76(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r31
  r30 = r31;
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825c1a28
  if (!cr6.eq) goto loc_825C1A28;
loc_825C1A68:
  // lwz r11,60(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 60);
  // addi r30,r27,56
  r30 = uint64_t(int64_t(int64_t(r27) + 56));
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x825c1a9c
  if (cr6.eq) goto loc_825C1A9C;
loc_825C1A78:
  // addi r3,r11,-8
  r3 = uint64_t(int64_t(int64_t(r11) + -8));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1A90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c1a78
  if (!cr6.eq) goto loc_825C1A78;
loc_825C1A9C:
  // lwz r31,132(r27)
  r31 = REX_LOAD_U32(uint32_t(r27) + 132);
  // addi r26,r27,128
  r26 = uint64_t(int64_t(int64_t(r27) + 128));
  // cmplw cr6,r31,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r26), so);
  // beq cr6,0x825c1b70
  if (cr6.eq) goto loc_825C1B70;
loc_825C1AAC:
  // lwz r3,124(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 124);
  // lwz r28,4(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c1b64
  if (cr6.eq) goto loc_825C1B64;
  // addi r30,r31,-12
  r30 = uint64_t(int64_t(int64_t(r31) + -12));
  // mr r4,r30
  r4 = r30;
  // bl 0x825c4940
  // (left out: dead in C++)
  { const lifted::Result o = f825C4940(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c1b64
  if (cr6.eq) goto loc_825C1B64;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // addi r11,r30,12
  r11 = uint64_t(int64_t(int64_t(r30) + 12));
  // stw r31,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r31));
  // bne cr6,0x825c1b00
  if (!cr6.eq) goto loc_825C1B00;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825C1B00:
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // mr r3,r30
  r3 = r30;
  // lwz r10,8(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r10,8(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 8);
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,8(r29)
  REX_STORE_U32(uint32_t(r29) + 8, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825c1b64
  if (!cr6.lt) goto loc_825C1B64;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,136(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 136);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1B48, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c1b64
  if (cr6.eq) goto loc_825C1B64;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1B64, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1B64:
  // mr r31,r28
  r31 = r28;
  // cmplw cr6,r28,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r26), so);
  // bne cr6,0x825c1aac
  if (!cr6.eq) goto loc_825C1AAC;
loc_825C1B70:
  // lwz r11,184(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 184);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c1bac
  if (cr6.eq) goto loc_825C1BAC;
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1B84, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,12(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 12);
  // stw r11,184(r27)
  REX_STORE_U32(uint32_t(r27) + 184, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1BA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1BAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C1BAC:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1BB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1BC0: lifted from generated/ by lift.py.
lifted::Result f825C1BC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C1BC0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // mr r31,r3
  r31 = r3;
  // mr r27,r4
  r27 = r4;
  // addi r24,r31,144
  r24 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r30,r5
  r30 = r5;
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C1BE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r28,r3
  r28 = r3;
  // lwz r29,124(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 124);
  // rlwinm r25,r30,31,31,31
  r25 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 31) & 0x1;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c1ce8
  if (cr6.lt) goto loc_825C1CE8;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c1c24
  if (!cr6.eq) goto loc_825C1C24;
  // lis r28,-30009
  r28 = uint64_t(int64_t(-1966669824));
  // mr r3,r24
  r3 = r24;
  // ori r28,r28,9
  r28 = r28 | 9;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1C18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C1C24:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r4,r27
  r4 = r27;
  // mr r3,r29
  r3 = r29;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 9);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1ce0
  if (cr6.eq) goto loc_825C1CE0;
  // mr r5,r27
  r5 = r27;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c1788
  // (left out: dead in C++)
  { const lifted::Result o = f825C1788(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // addi r26,r31,28
  r26 = uint64_t(int64_t(int64_t(r31) + 28));
loc_825C1C64:
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x825c1ce8
  if (cr6.eq) goto loc_825C1CE8;
  // addi r31,r11,-12
  r31 = uint64_t(int64_t(int64_t(r11) + -12));
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1cc0
  if (cr6.eq) goto loc_825C1CC0;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mr r5,r27
  r5 = r27;
  // mr r3,r29
  r3 = r29;
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C1C94, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c1cc0
  if (cr6.eq) goto loc_825C1CC0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cntlzw r10,r25
  r10 = uint32_t(r25) == 0 ? 32 : __builtin_clz(uint32_t(r25));
  // mr r3,r31
  r3 = r31;
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // xori r4,r10,1
  r4 = r10 ^ 1;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C1CBC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
loc_825C1CC0:
  // mr r11,r30
  r11 = r30;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge cr6,0x825c1c64
  if (!cr6.lt) goto loc_825C1C64;
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C1CE0:
  // lis r28,-30009
  r28 = uint64_t(int64_t(-1966669824));
  // ori r28,r28,11
  r28 = r28 | 11;
loc_825C1CE8:
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C1CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C1D00: lifted from generated/ by lift.py.
lifted::Result f825C1D00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1D00 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,21344
  r11 = uint64_t(int64_t(int64_t(r11) + 21344));
  // addi r10,r10,21320
  r10 = uint64_t(int64_t(int64_t(r10) + 21320));
  // lwz r9,68(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // beq cr6,0x825c1d3c
  if (cr6.eq) goto loc_825C1D3C;
  // bl 0x825c1800
  // (left out: dead in C++)
  { const lifted::Result o = f825C1800(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C1D3C:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // addi r10,r11,-22424
  r10 = uint64_t(int64_t(int64_t(r11) + -22424));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r9,r9,-22856
  r9 = uint64_t(int64_t(int64_t(r9) + -22856));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r10,140(r31)
  REX_STORE_U32(uint32_t(r31) + 140, uint32_t(r10));
  // stw r11,128(r31)
  REX_STORE_U32(uint32_t(r31) + 128, uint32_t(r11));
  // stw r11,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r11));
  // stw r11,100(r31)
  REX_STORE_U32(uint32_t(r31) + 100, uint32_t(r11));
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
  // stw r11,56(r31)
  REX_STORE_U32(uint32_t(r31) + 56, uint32_t(r11));
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r11));
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
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

// sub_825C1D90: lifted from generated/ by lift.py.
lifted::Result f825C1D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1D90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c1d00
  // (left out: dead in C++)
  { const lifted::Result o = f825C1D00(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c1dc8
  if (cr6.eq) goto loc_825C1DC8;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825C1DC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C1DC8:
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

// sub_825C1E08: lifted from generated/ by lift.py.
lifted::Result f825C1E08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1E08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,97
  r3 = uint64_t(int64_t(int64_t(r1) + 97));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825C1E2C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f0 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // lhz r10,244(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 244);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // rlwinm r9,r10,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // stb r10,116(r1)
  REX_STORE_U8(uint32_t(r1) + 116, uint8_t(r10));
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // stb r9,115(r1)
  REX_STORE_U8(uint32_t(r1) + 115, uint8_t(r9));
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // rlwinm r9,r10,8,24,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF;
  // stb r10,120(r1)
  REX_STORE_U8(uint32_t(r1) + 120, uint8_t(r10));
  // stb r9,117(r1)
  REX_STORE_U8(uint32_t(r1) + 117, uint8_t(r9));
  // rldicl r9,r10,56,8
  r9 = __builtin_rotateleft64(r10, 56) & 0xFFFFFFFFFFFFFF;
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // sth r9,118(r1)
  REX_STORE_U16(uint32_t(r1) + 118, uint16_t(r9));
  // rlwinm r9,r10,24,24,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFF;
  // stb r10,106(r1)
  REX_STORE_U8(uint32_t(r1) + 106, uint8_t(r10));
  // stb r9,105(r1)
  REX_STORE_U8(uint32_t(r1) + 105, uint8_t(r9));
  // bne cr6,0x825c1e8c
  if (!cr6.eq) goto loc_825C1E8C;
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stb r11,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r11));
  // b 0x825c1ef0
  goto loc_825C1EF0;
loc_825C1E8C:
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // stb r10,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // srawi r9,r10,24
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r10) >> 24));
  // stb r10,124(r1)
  REX_STORE_U8(uint32_t(r1) + 124, uint8_t(r10));
  // stb r9,121(r1)
  REX_STORE_U8(uint32_t(r1) + 121, uint8_t(r9));
  // rldicl r9,r10,56,8
  r9 = __builtin_rotateleft64(r10, 56) & 0xFFFFFFFFFFFFFF;
  // lhz r10,304(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 304);
  // sth r9,122(r1)
  REX_STORE_U16(uint32_t(r1) + 122, uint16_t(r9));
  // rlwinm r9,r10,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // stb r10,106(r1)
  REX_STORE_U8(uint32_t(r1) + 106, uint8_t(r10));
  // mr r10,r11
  r10 = r11;
  // lwz r11,296(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 296);
  // stb r9,105(r1)
  REX_STORE_U8(uint32_t(r1) + 105, uint8_t(r9));
  // srawi r9,r10,24
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r10) >> 24));
  // stb r10,114(r1)
  REX_STORE_U8(uint32_t(r1) + 114, uint8_t(r10));
  // stb r11,110(r1)
  REX_STORE_U8(uint32_t(r1) + 110, uint8_t(r11));
  // stb r9,111(r1)
  REX_STORE_U8(uint32_t(r1) + 111, uint8_t(r9));
  // rldicl r9,r10,56,8
  r9 = __builtin_rotateleft64(r10, 56) & 0xFFFFFFFFFFFFFF;
  // srawi r10,r11,24
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r11) >> 24));
  // sth r9,112(r1)
  REX_STORE_U16(uint32_t(r1) + 112, uint16_t(r9));
  // stb r10,107(r1)
  REX_STORE_U8(uint32_t(r1) + 107, uint8_t(r10));
  // rldicl r10,r11,56,8
  r10 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,108(r1)
  REX_STORE_U16(uint32_t(r1) + 108, uint16_t(r10));
loc_825C1EF0:
  // lwz r11,120(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 120);
  // lbz r10,124(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 124);
  // rldicr r11,r11,8,63
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFFFF;
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // ld r4,96(r1)
  r4 = REX_LOAD_U64(uint32_t(r1) + 96);
  // or r11,r10,r11
  r11 = r10 | r11;
  // ld r5,104(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 104);
  // ld r6,112(r1)
  r6 = REX_LOAD_U64(uint32_t(r1) + 112);
  // rldicr r7,r11,24,39
  r7 = __builtin_rotateleft64(r11, 24) & 0xFFFFFFFFFF000000;
  // bl 0x825c1388
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1388, LIFTED_ARGS(0x825C1F18, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C1F88: lifted from generated/ by lift.py.
lifted::Result f825C1F88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C1F88 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // bne cr6,0x825c1fbc
  if (!cr6.eq) goto loc_825C1FBC;
  // lwz r11,16(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 16);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x825c2048
  if (cr6.eq) goto loc_825C2048;
loc_825C1FBC:
  // lwz r11,16(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 16);
  // lwz r10,5(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 5);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x825c1fdc
  if (!cr6.lt) goto loc_825C1FDC;
  // stfs f0,20(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // b 0x825c1ffc
  goto loc_825C1FFC;
loc_825C1FDC:
  // lwz r11,9(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 9);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x825c1ff8
  if (!cr6.gt) goto loc_825C1FF8;
  // stfs f0,20(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // b 0x825c1ffc
  goto loc_825C1FFC;
loc_825C1FF8:
  // stfs f1,20(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
loc_825C1FFC:
  // lwz r31,228(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 228);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c2040
  if (cr6.eq) goto loc_825C2040;
loc_825C2008:
  // mr r11,r31
  r11 = r31;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c2020
  if (cr6.eq) goto loc_825C2020;
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // b 0x825c2024
  goto loc_825C2024;
loc_825C2020:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C2024:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lfs f1,20(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 20);
  f1 = double(temp.f32);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2038, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825c2008
  if (!cr6.eq) goto loc_825C2008;
loc_825C2040:
  // mr r3,r30
  r3 = r30;
  // bl 0x825c1e08
  // (left out: dead in C++)
  { const lifted::Result o = f825C1E08(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_825C2048:
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

// sub_825C2308: lifted from generated/ by lift.py.
lifted::Result f825C2308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C2308 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2330, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2348, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,39(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 39);
  // add r3,r11,r30
  r3 = r11 + r30;
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

// sub_825C2368: lifted from generated/ by lift.py.
lifted::Result f825C2368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C2368 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C238C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C23A4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // lwz r10,43(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 43);
  // rlwinm r9,r11,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r3,r11,r29
  r3 = r11 + r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C25B0: lifted from generated/ by lift.py.
lifted::Result f825C25B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C25B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C25D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r29,r3
  r29 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C25EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r10,15(r3)
  r10 = REX_LOAD_U16(uint32_t(r3) + 15);
  // lis r11,-21846
  r11 = uint64_t(int64_t(-1431699456));
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // ori r11,r11,43691
  r11 = r11 | 43691;
  // subf r10,r10,r30
  r10 = r30 - r10;
  // subf r10,r29,r10
  r10 = r10 - r29;
  // mulhwu r11,r10,r11
  r11 = (uint64_t(uint32_t(r10)) * uint64_t(uint32_t(r11))) >> 32;
  // rlwinm r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C2650: lifted from generated/ by lift.py.
lifted::Result f825C2650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2650 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r10,r10,21416
  r10 = uint64_t(int64_t(int64_t(r10) + 21416));
  // lwz r11,128(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 128);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // beq cr6,0x825c26ac
  if (cr6.eq) goto loc_825C26AC;
  // addi r3,r11,-4
  r3 = uint64_t(int64_t(int64_t(r11) + -4));
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825c26a8
  if (cr6.eq) goto loc_825C26A8;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C26A4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825c26ac
  goto loc_825C26AC;
loc_825C26A8:
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825C26AC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C26AC:
  // lwz r11,144(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 144);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c26c4
  if (cr6.eq) goto loc_825C26C4;
  // lwz r4,136(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 136);
  // lwz r3,132(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 132);
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C26C4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C26C4:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c37a8
  // (left out: dead in C++)
  { const lifted::Result o = f825C37A8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825C2B50: lifted from generated/ by lift.py.
lifted::Result f825C2B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2B50 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c2650
  // (left out: dead in C++)
  { const lifted::Result o = f825C2650(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2b90
  if (cr6.eq) goto loc_825C2B90;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32800
  r4 = r4 | 32800;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C2B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C2B90:
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

// sub_825C2BA8: lifted from generated/ by lift.py.
lifted::Result f825C2BA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2BA8 (lifted)");
  // b 0x825c0248
  { const lifted::Result o = f825C0248(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C2BB0: lifted from generated/ by lift.py.
lifted::Result f825C2BB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C2BB0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,97
  r3 = uint64_t(int64_t(int64_t(r1) + 97));
  // stb r11,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825C2BE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stb r11,97(r1)
  REX_STORE_U8(uint32_t(r1) + 97, uint8_t(r11));
  // bl 0x8245a760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A760, LIFTED_ARGS(0x825C2BF4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c2c44
  if (cr6.lt) goto loc_825C2C44;
  // lis r11,-32164
  r11 = uint64_t(int64_t(-2107899904));
  // stw r30,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r30));
  // addi r6,r31,8
  r6 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r5,r30
  r5 = r30;
  // addi r4,r11,11176
  r4 = uint64_t(int64_t(int64_t(r11) + 11176));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8245a5e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A5E8, LIFTED_ARGS(0x825C2C18, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c2c44
  if (cr6.lt) goto loc_825C2C44;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8245a568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A568, LIFTED_ARGS(0x825C2C28, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c2c44
  if (cr6.lt) goto loc_825C2C44;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x8245a3b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A3B8, LIFTED_ARGS(0x825C2C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,89(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 89);
  // sth r11,444(r31)
  REX_STORE_U16(uint32_t(r31) + 444, uint16_t(r11));
loc_825C2C44:
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

// sub_825C2D90: lifted from generated/ by lift.py.
lifted::Result f825C2D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C2D90 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // lwz r3,8(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 8);
  // bl 0x8245a668
  // (left out: dead in C++)
  { const lifted::Result o = f8245A668(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c2ef8
  if (cr6.lt) goto loc_825C2EF8;
  // lwz r11,224(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 224);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c2e40
  if (!cr6.eq) goto loc_825C2E40;
  // addi r31,r30,12
  r31 = uint64_t(int64_t(int64_t(r30) + 12));
loc_825C2DCC:
  // lwz r11,204(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2e2c
  if (cr6.eq) goto loc_825C2E2C;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r10,204(r31)
  REX_STORE_U32(uint32_t(r31) + 204, uint32_t(r10));
  // beq cr6,0x825c2df4
  if (cr6.eq) goto loc_825C2DF4;
  // stw r29,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r29));
  // b 0x825c2df8
  goto loc_825C2DF8;
loc_825C2DF4:
  // stw r29,208(r31)
  REX_STORE_U32(uint32_t(r31) + 208, uint32_t(r29));
loc_825C2DF8:
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,212(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 212);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,212(r31)
  REX_STORE_U32(uint32_t(r31) + 212, uint32_t(r11));
  // beq cr6,0x825c2e2c
  if (cr6.eq) goto loc_825C2E2C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2E2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2E2C:
  // lwz r11,224(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 224);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c2dcc
  if (cr6.eq) goto loc_825C2DCC;
loc_825C2E40:
  // lwz r11,440(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 440);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c2ecc
  if (!cr6.eq) goto loc_825C2ECC;
  // addi r31,r30,228
  r31 = uint64_t(int64_t(int64_t(r30) + 228));
loc_825C2E58:
  // lwz r11,204(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2eb8
  if (cr6.eq) goto loc_825C2EB8;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r10,204(r31)
  REX_STORE_U32(uint32_t(r31) + 204, uint32_t(r10));
  // beq cr6,0x825c2e80
  if (cr6.eq) goto loc_825C2E80;
  // stw r29,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r29));
  // b 0x825c2e84
  goto loc_825C2E84;
loc_825C2E80:
  // stw r29,208(r31)
  REX_STORE_U32(uint32_t(r31) + 208, uint32_t(r29));
loc_825C2E84:
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,212(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 212);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,212(r31)
  REX_STORE_U32(uint32_t(r31) + 212, uint32_t(r11));
  // beq cr6,0x825c2eb8
  if (cr6.eq) goto loc_825C2EB8;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2EB8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2EB8:
  // lwz r11,440(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 440);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c2e58
  if (cr6.eq) goto loc_825C2E58;
loc_825C2ECC:
  // bl 0x8245a4e8
  // (left out: dead in C++)
  { const lifted::Result o = f8245A4E8(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c2ef4
  if (cr6.lt) goto loc_825C2EF4;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2EF4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2EF4:
  // mr r3,r31
  r3 = r31;
loc_825C2EF8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C2F00: lifted from generated/ by lift.py.
lifted::Result f825C2F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2F00 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,216(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 216);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2f8c
  if (cr6.eq) goto loc_825C2F8C;
loc_825C2F1C:
  // mr r9,r11
  r9 = r11;
  // mr r10,r11
  r10 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2f38
  if (cr6.eq) goto loc_825C2F38;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r31,0(r10)
  r31 = REX_LOAD_U32(uint32_t(r10) + 0);
  // b 0x825c2f3c
  goto loc_825C2F3C;
loc_825C2F38:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825C2F3C:
  // cmplw cr6,r31,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r4), so);
  // beq cr6,0x825c2f60
  if (cr6.eq) goto loc_825C2F60;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c2f1c
  if (!cr6.eq) goto loc_825C2F1C;
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
loc_825C2F60:
  // mr r4,r9
  r4 = r9;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // bl 0x825c2c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2C60, LIFTED_ARGS(0x825C2F6C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c2f8c
  if (cr6.eq) goto loc_825C2F8C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C2F8C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2F8C:
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

// sub_825C2FA0: lifted from generated/ by lift.py.
lifted::Result f825C2FA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2FA0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,432(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 432);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c302c
  if (cr6.eq) goto loc_825C302C;
loc_825C2FBC:
  // mr r9,r11
  r9 = r11;
  // mr r10,r11
  r10 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2fd8
  if (cr6.eq) goto loc_825C2FD8;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r31,0(r10)
  r31 = REX_LOAD_U32(uint32_t(r10) + 0);
  // b 0x825c2fdc
  goto loc_825C2FDC;
loc_825C2FD8:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825C2FDC:
  // cmplw cr6,r31,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r4), so);
  // beq cr6,0x825c3000
  if (cr6.eq) goto loc_825C3000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c2fbc
  if (!cr6.eq) goto loc_825C2FBC;
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
loc_825C3000:
  // mr r4,r9
  r4 = r9;
  // addi r3,r3,228
  r3 = uint64_t(int64_t(int64_t(r3) + 228));
  // bl 0x825c2c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2C60, LIFTED_ARGS(0x825C300C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c302c
  if (cr6.eq) goto loc_825C302C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C302C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C302C:
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

// sub_825C3040: lifted from generated/ by lift.py.
uint64_t f825C3040(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C3040 (lifted)");
  // lhz r3,444(r3)
  r3 = REX_LOAD_U16(uint32_t(r3) + 444);
  // blr
  return r3;
}

// sub_825C3048: lifted from generated/ by lift.py.
lifted::Result f825C3048(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C3048 (lifted)");
  uint64_t r3 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x8245a6e8
  { const lifted::Result o = lifted::Call(base, 0x8245A6E8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C3050: lifted from generated/ by lift.py.
lifted::Result f825C3050(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C3050 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,21488
  r11 = uint64_t(int64_t(int64_t(r11) + 21488));
  // addi r3,r31,228
  r3 = uint64_t(int64_t(int64_t(r31) + 228));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c30a8
  if (cr6.eq) goto loc_825C30A8;
loc_825C3084:
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,204(r3)
  REX_STORE_U32(uint32_t(r3) + 204, uint32_t(r10));
  // bne cr6,0x825c3084
  if (!cr6.eq) goto loc_825C3084;
loc_825C30A8:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r30,212(r3)
  REX_STORE_U32(uint32_t(r3) + 212, uint32_t(r30));
  // stw r30,208(r3)
  REX_STORE_U32(uint32_t(r3) + 208, uint32_t(r30));
  // bl 0x825c1f30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1F30, LIFTED_ARGS(0x825C30B8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c30ec
  if (cr6.eq) goto loc_825C30EC;
loc_825C30C8:
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,204(r3)
  REX_STORE_U32(uint32_t(r3) + 204, uint32_t(r10));
  // bne cr6,0x825c30c8
  if (!cr6.eq) goto loc_825C30C8;
loc_825C30EC:
  // stw r30,212(r3)
  REX_STORE_U32(uint32_t(r3) + 212, uint32_t(r30));
  // stw r30,208(r3)
  REX_STORE_U32(uint32_t(r3) + 208, uint32_t(r30));
  // bl 0x825c1f30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1F30, LIFTED_ARGS(0x825C30F8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C3110: lifted from generated/ by lift.py.
lifted::Result f825C3110(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C3110 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r29,r4
  r29 = r4;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // mr r30,r3
  r30 = r3;
  // ori r4,r4,32809
  r4 = r4 | 32809;
  // li r3,448
  r3 = uint64_t(int64_t(448));
  // mr r27,r5
  r27 = r5;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C3138, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c3194
  if (cr6.eq) goto loc_825C3194;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r11,21488
  r11 = uint64_t(int64_t(int64_t(r11) + 21488));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825c2060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2060, LIFTED_ARGS(0x825C3160, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,228
  r3 = uint64_t(int64_t(int64_t(r31) + 228));
  // bl 0x825c2060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2060, LIFTED_ARGS(0x825C3168, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // sth r28,444(r31)
  REX_STORE_U16(uint32_t(r31) + 444, uint16_t(r28));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c2bb0
  // (left out: dead in C++)
  { const lifted::Result o = f825C2BB0(base, r1, r13, r3, r4, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c31ac
  if (cr6.lt) goto loc_825C31AC;
  // stw r31,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r31));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C3194:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // stw r28,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r28));
  // ori r30,r30,14
  r30 = r30 | 14;
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C31AC:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C31C4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r28));
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C3398: lifted from generated/ by lift.py.
lifted::Result f825C3398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825C3398 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // lis r9,-32105
  r9 = uint64_t(int64_t(-2104033280));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // stb r11,88(r1)
  REX_STORE_U8(uint32_t(r1) + 88, uint8_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,5268(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 5268);
  // clrlwi r8,r10,31
  r8 = uint32_t(r10) & 0x1;
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x825c33e4
  if (!cr6.eq) goto loc_825C33E4;
  // ori r10,r10,1
  r10 = r10 | 1;
  // stw r10,5268(r9)
  REX_STORE_U32(uint32_t(r9) + 5268, uint32_t(r10));
  // lis r9,-32105
  r9 = uint64_t(int64_t(-2104033280));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r10,5264(r9)
  REX_STORE_U32(uint32_t(r9) + 5264, uint32_t(r10));
loc_825C33E4:
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r9,21520
  r9 = uint64_t(int64_t(int64_t(r9) + 21520));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // std r11,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r11);
  // std r11,8(r10)
  REX_STORE_U64(uint32_t(r10) + 8, r11);
  // std r11,16(r10)
  REX_STORE_U64(uint32_t(r10) + 16, r11);
  // stw r11,24(r10)
  REX_STORE_U32(uint32_t(r10) + 24, uint32_t(r11));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stb r11,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r9,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r9));
  // stb r10,97(r1)
  REX_STORE_U8(uint32_t(r1) + 97, uint8_t(r10));
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // stb r11,104(r1)
  REX_STORE_U8(uint32_t(r1) + 104, uint8_t(r11));
  // ori r10,r10,48000
  r10 = r10 | 48000;
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // bl 0x825ca4b8
  // (left out: dead in C++)
  { const lifted::Result o = f825CA4B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c3454
  if (cr6.lt) goto loc_825C3454;
  // addi r3,r30,228
  r3 = uint64_t(int64_t(int64_t(r30) + 228));
  // lwz r4,80(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825c60c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C60C0, LIFTED_ARGS(0x825C344C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_825C3454:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C3460: lifted from generated/ by lift.py.
lifted::Result f825C3460(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C3460 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r4
  r3 = r4;
  // mr r4,r5
  r4 = r5;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r6
  r31 = r6;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x825c3110
  // (left out: dead in C++)
  { const lifted::Result o = f825C3110(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c349c
  if (cr6.lt) goto loc_825C349C;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_825C349C:
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

// sub_825C34B0: lifted from generated/ by lift.py.
lifted::Result f825C34B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C34B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c3050
  // (left out: dead in C++)
  { const lifted::Result o = f825C3050(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c34f0
  if (cr6.eq) goto loc_825C34F0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32809
  r4 = r4 | 32809;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C34EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C34F0:
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

// sub_825C37A8: lifted from generated/ by lift.py.
lifted::Result f825C37A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C37A8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,22552
  r11 = uint64_t(int64_t(int64_t(r11) + 22552));
  // lwz r4,8(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825c37f8
  if (cr6.eq) goto loc_825C37F8;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C37F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
loc_825C37F8:
  // lwz r3,104(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 104);
  // lis r11,8324
  r11 = uint64_t(int64_t(545521664));
  // ori r30,r11,1
  r30 = r11 | 1;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c3814
  if (cr6.eq) goto loc_825C3814;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C3814, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C3814:
  // lwz r3,120(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 120);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c3828
  if (cr6.eq) goto loc_825C3828;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C3828, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C3828:
  // lwz r3,112(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 112);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c383c
  if (cr6.eq) goto loc_825C383C;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C383C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C383C:
  // lwz r3,108(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 108);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c3850
  if (cr6.eq) goto loc_825C3850;
  // mr r4,r30
  r4 = r30;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C3850, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C3850:
  // lwz r11,124(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 124);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c388c
  if (cr6.eq) goto loc_825C388C;
  // addi r3,r11,-4
  r3 = uint64_t(int64_t(int64_t(r11) + -4));
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825c3888
  if (cr6.eq) goto loc_825C3888;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r3,r11
  r3 = r11;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3884, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825c388c
  goto loc_825C388C;
loc_825C3888:
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825C388C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C388C:
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

// sub_825C38A8: lifted from generated/ by lift.py.
lifted::Result f825C38A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C38A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C38CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C38E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,8(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 8);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c39e4
  if (cr6.eq) goto loc_825C39E4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3908, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3920, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r4,r29
  r4 = r29;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lhz r5,13(r11)
  r5 = REX_LOAD_U16(uint32_t(r11) + 13);
  // bl 0x825bfc50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFC50, LIFTED_ARGS(0x825C3934, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,1,15,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x1FFFE;
  // lhzx r4,r11,r30
  r4 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r30));
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // beq cr6,0x825c39e4
  if (cr6.eq) goto loc_825C39E4;
loc_825C3944:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3958, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3974, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // mr r11,r29
  r11 = r29;
loc_825C397C:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq cr6,0x825c39a0
  if (cr6.eq) goto loc_825C39A0;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c397c
  if (cr6.eq) goto loc_825C397C;
loc_825C39A0:
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c39c8
  if (cr6.eq) goto loc_825C39C8;
  // lhz r4,4(r30)
  r4 = REX_LOAD_U16(uint32_t(r30) + 4);
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // bne cr6,0x825c3944
  if (!cr6.eq) goto loc_825C3944;
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C39BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C39C8:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C39E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
loc_825C39E4:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C39EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C39F8: lifted from generated/ by lift.py.
lifted::Result f825C39F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C39F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C3A1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3A34, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,8(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 8);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c3b54
  if (cr6.eq) goto loc_825C3B54;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3A58, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,11(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 11);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c3b54
  if (cr6.eq) goto loc_825C3B54;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3A78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3A90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r4,r29
  r4 = r29;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lhz r5,15(r11)
  r5 = REX_LOAD_U16(uint32_t(r11) + 15);
  // bl 0x825bfc50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFC50, LIFTED_ARGS(0x825C3AA4, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,1,15,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x1FFFE;
  // lhzx r4,r11,r30
  r4 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r30));
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // beq cr6,0x825c3b54
  if (cr6.eq) goto loc_825C3B54;
loc_825C3AB4:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3AC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3AE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // mr r11,r29
  r11 = r29;
loc_825C3AEC:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq cr6,0x825c3b10
  if (cr6.eq) goto loc_825C3B10;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c3aec
  if (cr6.eq) goto loc_825C3AEC;
loc_825C3B10:
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x825c3b38
  if (cr6.eq) goto loc_825C3B38;
  // lhz r4,4(r30)
  r4 = REX_LOAD_U16(uint32_t(r30) + 4);
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // bne cr6,0x825c3ab4
  if (!cr6.eq) goto loc_825C3AB4;
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C3B2C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C3B38:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3B50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
loc_825C3B54:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C3B5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C40E0: lifted from generated/ by lift.py.
lifted::Result f825C40E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C40E0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c37a8
  // (left out: dead in C++)
  { const lifted::Result o = f825C37A8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4120
  if (cr6.eq) goto loc_825C4120;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32874
  r4 = r4 | 32874;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C411C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C4120:
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

// sub_825C4138: lifted from generated/ by lift.py.
lifted::Result f825C4138(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4138 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r27,r4
  r27 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r24,r11,144
  r24 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C415C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,28(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 28);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lhz r4,1(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 1);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C417C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r9,28(r27)
  r9 = REX_LOAD_U32(uint32_t(r27) + 28);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lhz r4,1(r9)
  r4 = REX_LOAD_U16(uint32_t(r9) + 1);
  // lhz r23,3(r11)
  r23 = REX_LOAD_U16(uint32_t(r11) + 3);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C41A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r9,28(r27)
  r9 = REX_LOAD_U32(uint32_t(r27) + 28);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lhz r4,1(r9)
  r4 = REX_LOAD_U16(uint32_t(r9) + 1);
  // lhz r22,1(r11)
  r22 = REX_LOAD_U16(uint32_t(r11) + 1);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C41C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 28);
  // lhz r28,1(r11)
  r28 = REX_LOAD_U16(uint32_t(r11) + 1);
  // lbz r11,5(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 5);
  // mr r30,r28
  r30 = r28;
  // clrlwi r25,r11,30
  r25 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r30,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 65535, so);
  // beq cr6,0x825c456c
  if (cr6.eq) goto loc_825C456C;
loc_825C41E0:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C41F8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825c44b0
  if (cr6.eq) goto loc_825C44B0;
  // lwz r11,104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 104);
  // rlwinm r9,r30,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwzx r30,r9,r11
  r30 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4228, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x825c44b0
  if (cr6.lt) goto loc_825C44B0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C424C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 7, so);
  // bgt cr6,0x825c456c
  if (cr6.gt) goto loc_825C456C;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,17004
  r12 = uint64_t(int64_t(int64_t(r12) + 17004));
  // rlwinm r0,r3,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r3)) {
  case 0:
  goto loc_825C450C;
  case 1:
  goto loc_825C44EC;
  case 2:
  goto loc_825C428C;
  case 3:
  goto loc_825C4324;
  case 4:
  goto loc_825C43E8;
  case 5:
  goto loc_825C456C;
  case 6:
  goto loc_825C456C;
  case 7:
  goto loc_825C456C;
  default:
  std::abort();
  }
loc_825C428C:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c44b0
  if (cr6.eq) goto loc_825C44B0;
loc_825C42A0:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c42b0
  if (!cr6.eq) goto loc_825C42B0;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C42B0:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c430c
  if (cr6.eq) goto loc_825C430C;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C42D0, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c430c
  if (!cr6.eq) goto loc_825C430C;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c430c
  if (cr6.eq) goto loc_825C430C;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x825c430c
  if (cr6.eq) goto loc_825C430C;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mr r5,r28
  r5 = r28;
  // mr r3,r31
  r3 = r31;
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C4304, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825c4520
  if (!cr6.eq) goto loc_825C4520;
loc_825C430C:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c42a0
  if (!cr6.eq) goto loc_825C42A0;
  // b 0x825c44b0
  goto loc_825C44B0;
loc_825C4324:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c44a8
  if (cr6.eq) goto loc_825C44A8;
loc_825C4338:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c4348
  if (!cr6.eq) goto loc_825C4348;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C4348:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c43d0
  if (cr6.eq) goto loc_825C43D0;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4368, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c43d0
  if (!cr6.eq) goto loc_825C43D0;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c43d0
  if (cr6.eq) goto loc_825C43D0;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x825c43d0
  if (cr6.eq) goto loc_825C43D0;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mr r5,r28
  r5 = r28;
  // mr r3,r31
  r3 = r31;
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C439C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c43d0
  if (cr6.eq) goto loc_825C43D0;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4520
  if (cr6.eq) goto loc_825C4520;
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x825c43cc
  if (cr6.eq) goto loc_825C43CC;
  // lwz r10,52(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 52);
  // lfs f0,88(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 88);
  f0 = double(temp.f32);
  // lfs f13,88(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 88);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x825c43d0
  if (!cr6.lt) goto loc_825C43D0;
loc_825C43CC:
  // mr r26,r30
  r26 = r30;
loc_825C43D0:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c4338
  if (!cr6.eq) goto loc_825C4338;
  // b 0x825c44a8
  goto loc_825C44A8;
loc_825C43E8:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c44a8
  if (cr6.eq) goto loc_825C44A8;
loc_825C43FC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c440c
  if (!cr6.eq) goto loc_825C440C;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C440C:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c4494
  if (cr6.eq) goto loc_825C4494;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C442C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c4494
  if (!cr6.eq) goto loc_825C4494;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4494
  if (cr6.eq) goto loc_825C4494;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x825c4494
  if (cr6.eq) goto loc_825C4494;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mr r5,r28
  r5 = r28;
  // mr r3,r31
  r3 = r31;
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c3548
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C3548, LIFTED_ARGS(0x825C4460, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c4494
  if (cr6.eq) goto loc_825C4494;
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x825c4490
  if (cr6.eq) goto loc_825C4490;
  // lwz r11,52(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 52);
  // lwz r10,52(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 52);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r10,28(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 28);
  // lbz r11,6(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 6);
  // lbz r10,6(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 6);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x825c4494
  if (!cr6.gt) goto loc_825C4494;
loc_825C4490:
  // mr r26,r30
  r26 = r30;
loc_825C4494:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c43fc
  if (!cr6.eq) goto loc_825C43FC;
loc_825C44A8:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // bne cr6,0x825c4528
  if (!cr6.eq) goto loc_825C4528;
loc_825C44B0:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C44C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // clrlwi r30,r28,16
  r30 = uint32_t(r28) & 0xFFFF;
  // cmplwi cr6,r30,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 65535, so);
  // bne cr6,0x825c41e0
  if (!cr6.eq) goto loc_825C41E0;
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C44E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C44EC:
  // lwz r4,48(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 48);
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825c1738
  // (left out: dead in C++)
  f825C1738(base, r3, r4);
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4500, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C450C:
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4514, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C4520:
  // addi r3,r30,24
  r3 = uint64_t(int64_t(int64_t(r30) + 24));
  // b 0x825c452c
  goto loc_825C452C;
loc_825C4528:
  // addi r3,r26,24
  r3 = uint64_t(int64_t(int64_t(r26) + 24));
loc_825C452C:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r6,r25
  r6 = r25;
  // mr r5,r23
  r5 = r23;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4548, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 48);
  // mr r6,r25
  r6 = r25;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // mr r5,r22
  r5 = r22;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C456C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C456C:
  // mr r3,r24
  r3 = r24;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4574, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4580: lifted from generated/ by lift.py.
lifted::Result f825C4580(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4580 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r27,r3
  r27 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // addi r26,r11,144
  r26 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C45A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,108(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 108);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C45BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,108(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 108);
  // lhz r25,3(r11)
  r25 = REX_LOAD_U16(uint32_t(r11) + 3);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C45D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lbz r11,5(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 5);
  // lwz r10,108(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 108);
  // clrlwi r24,r11,30
  r24 = uint32_t(r11) & 0x3;
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C45F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825c4928
  if (cr6.eq) goto loc_825C4928;
  // lwz r10,296(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 296);
  // mr r3,r31
  r3 = r31;
  // lhz r9,304(r31)
  r9 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // rotlwi r9,r9,2
  r9 = __builtin_rotateleft32(uint32_t(r9), 2);
  // lwz r10,248(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 248);
  // lwz r11,108(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 108);
  // lwzx r30,r9,r10
  r30 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C462C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x825c4928
  if (cr6.lt) goto loc_825C4928;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,108(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 108);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C464C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,5(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 5);
  // rlwinm r11,r11,29,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x7;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bgt cr6,0x825c4928
  if (cr6.gt) goto loc_825C4928;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,18036
  r12 = uint64_t(int64_t(int64_t(r12) + 18036));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825C46B4;
  case 1:
  goto loc_825C4694;
  case 2:
  goto loc_825C46C8;
  case 3:
  goto loc_825C4770;
  case 4:
  goto loc_825C482C;
  case 5:
  goto loc_825C4928;
  case 6:
  goto loc_825C4928;
  case 7:
  goto loc_825C4928;
  default:
  std::abort();
  }
loc_825C4694:
  // mr r4,r31
  r4 = r31;
  // lwz r3,4(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 4);
  // bl 0x825c1738
  // (left out: dead in C++)
  f825C1738(base, r3, r4);
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C46A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C46B4:
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C46BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C46C8:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c4928
  if (cr6.eq) goto loc_825C4928;
loc_825C46DC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c46ec
  if (!cr6.eq) goto loc_825C46EC;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C46EC:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c4740
  if (cr6.eq) goto loc_825C4740;
  // lhz r11,304(r30)
  r11 = REX_LOAD_U16(uint32_t(r30) + 304);
  // lhz r10,304(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c4740
  if (!cr6.eq) goto loc_825C4740;
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // beq cr6,0x825c4740
  if (cr6.eq) goto loc_825C4740;
  // lwz r11,296(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 296);
  // lwz r10,296(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c4740
  if (!cr6.eq) goto loc_825C4740;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4734, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4768
  if (cr6.eq) goto loc_825C4768;
loc_825C4740:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c46dc
  if (!cr6.eq) goto loc_825C46DC;
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C475C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C4768:
  // addi r3,r30,24
  r3 = uint64_t(int64_t(int64_t(r30) + 24));
  // b 0x825c48f8
  goto loc_825C48F8;
loc_825C4770:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c4928
  if (cr6.eq) goto loc_825C4928;
loc_825C4784:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c4794
  if (!cr6.eq) goto loc_825C4794;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C4794:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c4814
  if (cr6.eq) goto loc_825C4814;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C47B4, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c4814
  if (!cr6.eq) goto loc_825C4814;
  // lhz r11,304(r30)
  r11 = REX_LOAD_U16(uint32_t(r30) + 304);
  // lhz r10,304(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c4814
  if (!cr6.eq) goto loc_825C4814;
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // beq cr6,0x825c4814
  if (cr6.eq) goto loc_825C4814;
  // lwz r11,296(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 296);
  // lwz r10,296(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c4814
  if (!cr6.eq) goto loc_825C4814;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4814
  if (cr6.eq) goto loc_825C4814;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825c4810
  if (cr6.eq) goto loc_825C4810;
  // lwz r10,52(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 52);
  // lfs f0,88(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 88);
  f0 = double(temp.f32);
  // lfs f13,88(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 88);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x825c4814
  if (!cr6.lt) goto loc_825C4814;
loc_825C4810:
  // mr r28,r30
  r28 = r30;
loc_825C4814:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c4784
  if (!cr6.eq) goto loc_825C4784;
  // b 0x825c48ec
  goto loc_825C48EC;
loc_825C482C:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,32(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 32);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c4928
  if (cr6.eq) goto loc_825C4928;
loc_825C4840:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r30,r29,-12
  r30 = uint64_t(int64_t(int64_t(r29) + -12));
  // bne cr6,0x825c4850
  if (!cr6.eq) goto loc_825C4850;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825C4850:
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825c48d8
  if (cr6.eq) goto loc_825C48D8;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4870, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c48d8
  if (!cr6.eq) goto loc_825C48D8;
  // lhz r11,304(r30)
  r11 = REX_LOAD_U16(uint32_t(r30) + 304);
  // lhz r10,304(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c48d8
  if (!cr6.eq) goto loc_825C48D8;
  // cmplw cr6,r30,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r31), so);
  // beq cr6,0x825c48d8
  if (cr6.eq) goto loc_825C48D8;
  // lwz r11,296(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 296);
  // lwz r10,296(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825c48d8
  if (!cr6.eq) goto loc_825C48D8;
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c48d8
  if (cr6.eq) goto loc_825C48D8;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825c48d4
  if (cr6.eq) goto loc_825C48D4;
  // lwz r10,52(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 52);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r10,28(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 28);
  // lbz r11,6(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 6);
  // lbz r10,6(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 6);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x825c48d8
  if (!cr6.gt) goto loc_825C48D8;
loc_825C48D4:
  // mr r28,r30
  r28 = r30;
loc_825C48D8:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825c4840
  if (!cr6.eq) goto loc_825C4840;
loc_825C48EC:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825c4928
  if (cr6.eq) goto loc_825C4928;
  // addi r3,r28,24
  r3 = uint64_t(int64_t(int64_t(r28) + 24));
loc_825C48F8:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r6,r24
  r6 = r24;
  // mr r5,r25
  r5 = r25;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4914, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,128(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 128);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4928, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C4928:
  // mr r3,r26
  r3 = r26;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4930, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4940: lifted from generated/ by lift.py.
lifted::Result f825C4940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4940 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,112(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 112);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4968, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c49b0
  if (cr6.eq) goto loc_825C49B0;
  // lwz r11,296(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 296);
  // mr r3,r30
  r3 = r30;
  // lhz r10,304(r30)
  r10 = REX_LOAD_U16(uint32_t(r30) + 304);
  // lwz r9,0(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 0);
  // rotlwi r10,r10,2
  r10 = __builtin_rotateleft32(uint32_t(r10), 2);
  // lwz r11,248(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 248);
  // lwz r9,108(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 108);
  // lwzx r29,r10,r11
  r29 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4998, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x825c49b0
  if (cr6.lt) goto loc_825C49B0;
loc_825C49A4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C49B0:
  // lwz r11,52(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4a4c
  if (cr6.eq) goto loc_825C4A4C;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lhz r30,1(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 1);
  // mr r29,r30
  r29 = r30;
  // cmplwi cr6,r29,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 65535, so);
  // beq cr6,0x825c4a4c
  if (cr6.eq) goto loc_825C4A4C;
loc_825C49D0:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C49E8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825c4a24
  if (cr6.eq) goto loc_825C4A24;
  // lwz r11,104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 104);
  // rlwinm r9,r29,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwzx r29,r9,r11
  r29 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4A18, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // bge cr6,0x825c49a4
  if (!cr6.lt) goto loc_825C49A4;
loc_825C4A24:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4A3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // clrlwi r29,r30,16
  r29 = uint32_t(r30) & 0xFFFF;
  // cmplwi cr6,r29,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 65535, so);
  // bne cr6,0x825c49d0
  if (!cr6.eq) goto loc_825C49D0;
loc_825C4A4C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4A58: lifted from generated/ by lift.py.
lifted::Result f825C4A58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C4A58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,24
  r3 = uint64_t(int64_t(int64_t(r3) + 24));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C4A78: lifted from generated/ by lift.py.
void f825C4A78(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C4A78 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // blt cr6,0x825c4a88
  if (cr6.lt) goto loc_825C4A88;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // beq cr6,0x825c4a8c
  if (cr6.eq) goto loc_825C4A8C;
loc_825C4A88:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825C4A8C:
  // stw r11,60(r3)
  REX_STORE_U32(uint32_t(r3) + 60, uint32_t(r11));
  // blr
  return;
}

// sub_825C4A98: lifted from generated/ by lift.py.
lifted::Result f825C4A98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825C4A98 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r30,296(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 296);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c4b30
  if (cr6.eq) goto loc_825C4B30;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,81
  r3 = uint64_t(int64_t(int64_t(r1) + 81));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825C4AC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r10,304(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r11,104(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 104);
  // rlwinm r9,r10,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // stb r29,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r29));
  // rldicr r11,r11,8,63
  r11 = __builtin_rotateleft64(r11, 8) & 0xFFFFFFFFFFFFFFFF;
  // stb r30,94(r1)
  REX_STORE_U8(uint32_t(r1) + 94, uint8_t(r30));
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // stb r10,90(r1)
  REX_STORE_U8(uint32_t(r1) + 90, uint8_t(r10));
  // mr r10,r31
  r10 = r31;
  // ld r4,80(r1)
  r4 = REX_LOAD_U64(uint32_t(r1) + 80);
  // stb r9,89(r1)
  REX_STORE_U8(uint32_t(r1) + 89, uint8_t(r9));
  // srawi r9,r10,24
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r10) >> 24));
  // stb r10,98(r1)
  REX_STORE_U8(uint32_t(r1) + 98, uint8_t(r10));
  // stb r9,95(r1)
  REX_STORE_U8(uint32_t(r1) + 95, uint8_t(r9));
  // rldicl r9,r10,56,8
  r9 = __builtin_rotateleft64(r10, 56) & 0xFFFFFFFFFFFFFF;
  // lbz r10,108(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 108);
  // or r11,r10,r11
  r11 = r10 | r11;
  // srawi r10,r30,24
  ca = (int32_t(r30) < 0) & ((uint32_t(r30) & 0xFFFFFF) != 0);
  r10 = uint64_t(int64_t(int32_t(r30) >> 24));
  // rldicr r7,r11,24,39
  r7 = __builtin_rotateleft64(r11, 24) & 0xFFFFFFFFFF000000;
  // sth r9,96(r1)
  REX_STORE_U16(uint32_t(r1) + 96, uint16_t(r9));
  // ld r6,96(r1)
  r6 = REX_LOAD_U64(uint32_t(r1) + 96);
  // stb r10,91(r1)
  REX_STORE_U8(uint32_t(r1) + 91, uint8_t(r10));
  // rldicl r10,r30,56,8
  r10 = __builtin_rotateleft64(r30, 56) & 0xFFFFFFFFFFFFFF;
  // sth r10,92(r1)
  REX_STORE_U16(uint32_t(r1) + 92, uint16_t(r10));
  // ld r5,88(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 88);
  // bl 0x825c1388
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1388, LIFTED_ARGS(0x825C4B30, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C4B30:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4B38: lifted from generated/ by lift.py.
lifted::Result f825C4B38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4B38 (lifted)");
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
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r30,r11,144
  r30 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C4B58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c4b84
  if (cr6.lt) goto loc_825C4B84;
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4B84, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C4B84:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4B98: lifted from generated/ by lift.py.
lifted::Result f825C4B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4B98 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r30,r11,144
  r30 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C4BC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,29,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x6;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c4bec
  if (!cr6.eq) goto loc_825C4BEC;
  // lis r31,-30009
  r31 = uint64_t(int64_t(-1966669824));
  // ori r31,r31,6
  r31 = r31 | 6;
  // b 0x825c4bf8
  goto loc_825C4BF8;
loc_825C4BEC:
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // bl 0x825cbb28
  // (left out: dead in C++)
  { const lifted::Result o = f825CBB28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_825C4BF8:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4C00, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825C4C20: lifted from generated/ by lift.py.
lifted::Result f825C4C20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4C20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C4C44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4C5C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c4ce0
  if (!cr6.eq) goto loc_825C4CE0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4C7C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x10;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4ca0
  if (cr6.eq) goto loc_825C4CA0;
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x825c4ce0
  if (cr6.eq) goto loc_825C4CE0;
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4ce0
  if (cr6.eq) goto loc_825C4CE0;
loc_825C4CA0:
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // blt cr6,0x825c4cb8
  if (cr6.lt) goto loc_825C4CB8;
  // bne cr6,0x825c4cb8
  if (!cr6.eq) goto loc_825C4CB8;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // b 0x825c4cbc
  goto loc_825C4CBC;
loc_825C4CB8:
  // stw r29,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r29));
loc_825C4CBC:
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c4ce0
  if (cr6.eq) goto loc_825C4CE0;
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r4,r11,27,31,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x825cbc58
  // (left out: dead in C++)
  { const lifted::Result o = f825CBC58(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825C4CE0:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4CE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4CF8: lifted from generated/ by lift.py.
lifted::Result f825C4CF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4CF8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C4D1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4d78
  if (cr6.eq) goto loc_825C4D78;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4D40, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4d68
  if (cr6.eq) goto loc_825C4D68;
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // mr r3,r28
  r3 = r28;
  // ori r30,r30,6
  r30 = r30 | 6;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C4D68:
  // mr r4,r29
  r4 = r29;
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // bl 0x825ca6f8
  // (left out: dead in C++)
  { const lifted::Result o = f825CA6F8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825C4D78:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4D80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4E30: lifted from generated/ by lift.py.
lifted::Result f825C4E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4E30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r30,r11,144
  r30 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r30
  r3 = r30;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C4E54, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4E68, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C4E78, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C4E88: lifted from generated/ by lift.py.
lifted::Result f825C4E88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4E88 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4EAC, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,27,28
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x18;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4ed8
  if (cr6.eq) goto loc_825C4ED8;
  // addi r3,r31,24
  r3 = uint64_t(int64_t(int64_t(r31) + 24));
  // bl 0x825cc1d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC1D0, LIFTED_ARGS(0x825C4EC0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825c4ed8
  if (cr6.eq) goto loc_825C4ED8;
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c4ed8
  if (cr6.eq) goto loc_825C4ED8;
  // bl 0x825ca860
  // (left out: dead in C++)
  { const lifted::Result o = f825CA860(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C4ED8:
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c4ee8
  if (cr6.eq) goto loc_825C4EE8;
  // bl 0x825cbd10
  // (left out: dead in C++)
  { const lifted::Result o = f825CBD10(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C4EE8:
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

// sub_825C4F00: lifted from generated/ by lift.py.
lifted::Result f825C4F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C4F00 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c4f24
  if (cr6.eq) goto loc_825C4F24;
  // bl 0x825cb040
  // (left out: dead in C++)
  { const lifted::Result o = f825CB040(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C4F24:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4F38, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x30;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4f64
  if (cr6.eq) goto loc_825C4F64;
  // lwz r11,320(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 320);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c4f64
  if (cr6.eq) goto loc_825C4F64;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lhz r4,304(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r3,296(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 296);
  // stw r11,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r11));
  // bl 0x825c7540
  // (left out: dead in C++)
  { const lifted::Result o = f825C7540(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_825C4F64:
  // lwz r11,336(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 336);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c4fa8
  if (cr6.eq) goto loc_825C4FA8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4F84, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c4fa8
  if (cr6.eq) goto loc_825C4FA8;
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C4FA8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C4FA8:
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

// sub_825C4FD0: lifted from generated/ by lift.py.
lifted::Result f825C4FD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C4FD0 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,52(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c4fe0
  if (cr6.eq) goto loc_825C4FE0;
  // b 0x825ca808
  { const lifted::Result o = f825CA808(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825C4FE0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return {r3, f1};
}

// sub_825C4FE8: lifted from generated/ by lift.py.
lifted::Result f825C4FE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C4FE8 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r27,r6
  r27 = r6;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C5014, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,308(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 308);
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // rotlwi r5,r11,3
  r5 = __builtin_rotateleft32(uint32_t(r11), 3);
  // addi r29,r5,8
  r29 = uint64_t(int64_t(int64_t(r5) + 8));
  // beq cr6,0x825c5040
  if (cr6.eq) goto loc_825C5040;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c5040
  if (cr6.eq) goto loc_825C5040;
  // stb r11,0(r30)
  REX_STORE_U8(uint32_t(r30) + 0, uint8_t(r11));
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r4,312(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 312);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x825C5040, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5040:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825c504c
  if (cr6.eq) goto loc_825C504C;
  // stw r29,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r29));
loc_825C504C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5054, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5060: lifted from generated/ by lift.py.
lifted::Result f825C5060(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5060 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C5084, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // rotlwi r28,r11,3
  r28 = __builtin_rotateleft32(uint32_t(r11), 3);
  // ori r4,r4,1
  r4 = r4 | 1;
  // mr r3,r28
  r3 = r28;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C50A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c50c8
  if (!cr6.eq) goto loc_825C50C8;
  // lis r26,-32761
  r26 = uint64_t(int64_t(-2147024896));
  // mr r3,r27
  r3 = r27;
  // ori r26,r26,14
  r26 = r26 | 14;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C50BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825C50C8:
  // mr r5,r28
  r5 = r28;
  // lwz r4,4(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x825C50D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c50f0
  if (cr6.eq) goto loc_825C50F0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C50F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C50F0:
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // addi r4,r31,340
  r4 = uint64_t(int64_t(int64_t(r31) + 340));
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // stw r29,344(r31)
  REX_STORE_U32(uint32_t(r31) + 344, uint32_t(r29));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // bl 0x825ca900
  // (left out: dead in C++)
  { const lifted::Result o = f825CA900(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // stw r11,348(r31)
  REX_STORE_U32(uint32_t(r31) + 348, uint32_t(r11));
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5118, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5128: lifted from generated/ by lift.py.
lifted::Result f825C5128(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5128 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C514C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5210
  if (cr6.eq) goto loc_825C5210;
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // ori r4,r11,87
  r4 = r11 | 87;
loc_825C5168:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // rlwinm r6,r5,3,0,28
  r6 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 3) & 0xFFFFFFF8;
  // lbz r10,340(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 340);
  // add r11,r11,r6
  r11 = r11 + r6;
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x825c51f0
  if (!cr6.lt) goto loc_825C51F0;
  // lwz r7,4(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lbz r11,0(r7)
  r11 = REX_LOAD_U8(uint32_t(r7) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c51f4
  if (cr6.eq) goto loc_825C51F4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_825C5198:
  // lwz r10,344(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 344);
  // rlwinm r11,r8,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r9,4(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 4);
  // add r10,r10,r6
  r10 = r10 + r6;
  // add r9,r9,r11
  r9 = r9 + r11;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lbz r27,0(r9)
  r27 = REX_LOAD_U8(uint32_t(r9) + 0);
  // lbz r3,0(r10)
  r3 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmplw cr6,r27,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r3), so);
  // bge cr6,0x825c51d4
  if (!cr6.lt) goto loc_825C51D4;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lfs f0,4(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f0 = double(temp.f32);
  // add r11,r10,r11
  r11 = r10 + r11;
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // b 0x825c51d8
  goto loc_825C51D8;
loc_825C51D4:
  // mr r29,r4
  r29 = r4;
loc_825C51D8:
  // addi r11,r8,1
  r11 = uint64_t(int64_t(int64_t(r8) + 1));
  // lbz r10,0(r7)
  r10 = REX_LOAD_U8(uint32_t(r7) + 0);
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x825c5198
  if (cr6.lt) goto loc_825C5198;
  // b 0x825c51f4
  goto loc_825C51F4;
loc_825C51F0:
  // mr r29,r4
  r29 = r4;
loc_825C51F4:
  // addi r11,r5,1
  r11 = uint64_t(int64_t(int64_t(r5) + 1));
  // lbz r10,0(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 0);
  // clrlwi r5,r11,24
  r5 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r5,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r10), so);
  // blt cr6,0x825c5168
  if (cr6.lt) goto loc_825C5168;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c5224
  if (cr6.lt) goto loc_825C5224;
loc_825C5210:
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5224
  if (cr6.eq) goto loc_825C5224;
  // mr r4,r30
  r4 = r30;
  // bl 0x825ca940
  // (left out: dead in C++)
  { const lifted::Result o = f825CA940(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C5224:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C522C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5238: lifted from generated/ by lift.py.
lifted::Result f825C5238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5238 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
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
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r27,r11,144
  r27 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C525C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c5338
  if (cr6.lt) goto loc_825C5338;
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // rotlwi r28,r11,3
  r28 = __builtin_rotateleft32(uint32_t(r11), 3);
  // ori r4,r4,1
  r4 = r4 | 1;
  // mr r3,r28
  r3 = r28;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C5288, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825c52b0
  if (!cr6.eq) goto loc_825C52B0;
  // lis r29,-32761
  r29 = uint64_t(int64_t(-2147024896));
  // mr r3,r27
  r3 = r27;
  // ori r29,r29,14
  r29 = r29 | 14;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C52A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C52B0:
  // mr r5,r28
  r5 = r28;
  // lwz r4,4(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x825C52C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,312(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 312);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c52d8
  if (cr6.eq) goto loc_825C52D8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C52D8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C52D8:
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // stw r29,312(r31)
  REX_STORE_U32(uint32_t(r31) + 312, uint32_t(r29));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // beq cr6,0x825c52f8
  if (cr6.eq) goto loc_825C52F8;
  // bl 0x825ca980
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CA980, LIFTED_ARGS(0x825C52F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C52F8:
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r30,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r30));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r31
  r3 = r31;
  // lwz r10,52(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 52);
  // stb r11,88(r1)
  REX_STORE_U8(uint32_t(r1) + 88, uint8_t(r11));
  // stw r9,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r9));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // stw r11,316(r31)
  REX_STORE_U32(uint32_t(r31) + 316, uint32_t(r11));
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5330, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // stw r28,348(r31)
  REX_STORE_U32(uint32_t(r31) + 348, uint32_t(r28));
loc_825C5338:
  // mr r3,r27
  r3 = r27;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5340, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5350: lifted from generated/ by lift.py.
lifted::Result f825C5350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5350 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,56(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C5374, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c53e4
  if (cr6.eq) goto loc_825C53E4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_825C538C:
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r8,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lbz r9,308(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 308);
  // add r10,r10,r11
  r10 = r10 + r11;
  // lbz r7,0(r10)
  r7 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmplw cr6,r7,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r9), so);
  // bge cr6,0x825c53cc
  if (!cr6.lt) goto loc_825C53CC;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r9,312(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 312);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rotlwi r10,r10,3
  r10 = __builtin_rotateleft32(uint32_t(r10), 3);
  // add r10,r10,r9
  r10 = r10 + r9;
  // lfs f13,4(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f13 = double(temp.f32);
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // b 0x825c53d0
  goto loc_825C53D0;
loc_825C53CC:
  // stfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
loc_825C53D0:
  // addi r11,r8,1
  r11 = uint64_t(int64_t(int64_t(r8) + 1));
  // lbz r10,0(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 0);
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x825c538c
  if (cr6.lt) goto loc_825C538C;
loc_825C53E4:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C53EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C53F8: lifted from generated/ by lift.py.
lifted::Result f825C53F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C53F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,56(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C541C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5478
  if (cr6.eq) goto loc_825C5478;
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_825C542C:
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // rlwinm r11,r8,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lbz r9,308(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 308);
  // lbzx r10,r10,r11
  r10 = REX_LOAD_U8(uint32_t(r10) + uint32_t(r11));
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x825c5464
  if (!cr6.lt) goto loc_825C5464;
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,312(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 312);
  // add r11,r9,r11
  r11 = r9 + r11;
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // rotlwi r11,r9,3
  r11 = __builtin_rotateleft32(uint32_t(r9), 3);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
loc_825C5464:
  // addi r11,r8,1
  r11 = uint64_t(int64_t(int64_t(r8) + 1));
  // lbz r10,0(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 0);
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x825c542c
  if (cr6.lt) goto loc_825C542C;
loc_825C5478:
  // lwz r3,52(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c548c
  if (cr6.eq) goto loc_825C548C;
  // mr r4,r31
  r4 = r31;
  // bl 0x825ca9c0
  // (left out: dead in C++)
  { const lifted::Result o = f825CA9C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C548C:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5494, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C54A0: lifted from generated/ by lift.py.
lifted::Result f825C54A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C54A0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
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
  // mr r26,r6
  r26 = r6;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r25,r11,144
  r25 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C54CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r27,r3
  r27 = r3;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // blt cr6,0x825c55e0
  if (cr6.lt) goto loc_825C55E0;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c54f0
  if (cr6.eq) goto loc_825C54F0;
  // cmplwi cr6,r30,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 8, so);
  // ble cr6,0x825c550c
  if (!cr6.gt) goto loc_825C550C;
loc_825C54F0:
  // lis r27,-32768
  r27 = uint64_t(int64_t(-2147483648));
  // mr r3,r25
  r3 = r25;
  // ori r27,r27,16389
  r27 = r27 | 16389;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5500, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C550C:
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5524, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x825c554c
  if (cr6.eq) goto loc_825C554C;
loc_825C5530:
  // lis r27,-32761
  r27 = uint64_t(int64_t(-2147024896));
  // mr r3,r25
  r3 = r25;
  // ori r27,r27,87
  r27 = r27 | 87;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5540, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C554C:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x825c5530
  if (cr6.eq) goto loc_825C5530;
  // lhz r11,68(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 68);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x825c55a4
  if (cr6.eq) goto loc_825C55A4;
  // mullw r11,r30,r29
  r11 = uint64_t(int64_t(int64_t(int32_t(r30)) * int64_t(int32_t(r29))));
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // rlwinm r3,r11,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C5574, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825c55f4
  if (cr6.eq) goto loc_825C55F4;
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c559c
  if (cr6.eq) goto loc_825C559C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,1
  r4 = r4 | 1;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C559C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C559C:
  // stw r28,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r28));
  // sth r30,68(r31)
  REX_STORE_U16(uint32_t(r31) + 68, uint16_t(r30));
loc_825C55A4:
  // mullw r11,r30,r29
  r11 = uint64_t(int64_t(int64_t(int32_t(r30)) * int64_t(int32_t(r29))));
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r26
  r4 = r26;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x825C55B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // beq cr6,0x825c55e0
  if (cr6.eq) goto loc_825C55E0;
  // mr r6,r26
  r6 = r26;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x825caa00
  // (left out: dead in C++)
  { const lifted::Result o = f825CAA00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
loc_825C55E0:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C55E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C55F4:
  // lis r27,-32761
  r27 = uint64_t(int64_t(-2147024896));
  // mr r3,r25
  r3 = r25;
  // ori r27,r27,14
  r27 = r27 | 14;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5604, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5610: lifted from generated/ by lift.py.
lifted::Result f825C5610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C5610 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,52(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825ca860
  { const lifted::Result o = f825CA860(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C5628: lifted from generated/ by lift.py.
lifted::Result f825C5628(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C5628 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,320(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 320);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c565c
  if (cr6.eq) goto loc_825C565C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lhz r4,304(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r3,296(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 296);
  // stw r11,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r11));
  // bl 0x825c7540
  // (left out: dead in C++)
  { const lifted::Result o = f825C7540(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_825C565C:
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = f825C4A98(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825C5680: lifted from generated/ by lift.py.
lifted::Result f825C5680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825C5680 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lhz r4,304(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 304);
  // cmplwi cr6,r4,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 65535, so);
  // beq cr6,0x825c56bc
  if (cr6.eq) goto loc_825C56BC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,296(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 296);
  // stw r11,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r11));
  // bl 0x825c7510
  // (left out: dead in C++)
  { const lifted::Result o = f825C7510(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = f825C4A98(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_825C56BC:
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

// sub_825C56D0: lifted from generated/ by lift.py.
lifted::Result f825C56D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C56D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r29,r11,144
  r29 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C56F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825c0730
  // (left out: dead in C++)
  { const lifted::Result o = f825C0730(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c5744
  if (cr6.eq) goto loc_825C5744;
  // mr r4,r30
  r4 = r30;
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825c0748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0748, LIFTED_ARGS(0x825C571C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5740
  if (cr6.eq) goto loc_825C5740;
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c5740
  if (cr6.eq) goto loc_825C5740;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c5744
  if (!cr6.eq) goto loc_825C5744;
loc_825C5740:
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
loc_825C5744:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C574C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5790: lifted from generated/ by lift.py.
lifted::Result f825C5790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5790 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,56(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 56);
  // addi r23,r11,144
  r23 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C57B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // mr r24,r26
  r24 = r26;
  // stb r26,2(r31)
  REX_STORE_U8(uint32_t(r31) + 2, uint8_t(r26));
  // sth r11,0(r31)
  REX_STORE_U16(uint32_t(r31) + 0, uint16_t(r11));
  // stb r26,3(r31)
  REX_STORE_U8(uint32_t(r31) + 3, uint8_t(r26));
  // sth r11,4(r31)
  REX_STORE_U16(uint32_t(r31) + 4, uint16_t(r11));
  // stw r26,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r26));
  // stb r26,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r26));
  // stb r26,13(r31)
  REX_STORE_U8(uint32_t(r31) + 13, uint8_t(r26));
  // lwz r11,52(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 52);
  // stw r26,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r26));
  // stw r26,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r26));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r26,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r26));
  // beq cr6,0x825c5948
  if (cr6.eq) goto loc_825C5948;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lhz r11,1(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 1);
  // sth r11,0(r31)
  REX_STORE_U16(uint32_t(r31) + 0, uint16_t(r11));
  // lwz r11,52(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 52);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lbz r11,6(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 6);
  // stb r11,3(r31)
  REX_STORE_U8(uint32_t(r31) + 3, uint8_t(r11));
  // lwz r10,52(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 52);
  // mr r11,r10
  r11 = r10;
  // addi r11,r11,36
  r11 = uint64_t(int64_t(int64_t(r11) + 36));
  // lwz r28,40(r10)
  r28 = REX_LOAD_U32(uint32_t(r10) + 40);
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // beq cr6,0x825c5944
  if (cr6.eq) goto loc_825C5944;
loc_825C5830:
  // lwz r11,24(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 24);
  // lwz r25,4(r28)
  r25 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5924
  if (cr6.eq) goto loc_825C5924;
  // lwz r30,36(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 36);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c58e8
  if (cr6.eq) goto loc_825C58E8;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5860, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5878, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825c58e8
  if (cr6.eq) goto loc_825C58E8;
  // clrlwi r11,r24,24
  r11 = uint32_t(r24) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c58b8
  if (!cr6.eq) goto loc_825C58B8;
  // lhz r11,52(r29)
  r11 = REX_LOAD_U16(uint32_t(r29) + 52);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // sth r11,4(r31)
  REX_STORE_U16(uint32_t(r31) + 4, uint16_t(r11));
  // beq cr6,0x825c58e8
  if (cr6.eq) goto loc_825C58E8;
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // lhz r4,52(r29)
  r4 = REX_LOAD_U16(uint32_t(r29) + 52);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825cc380
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC380, LIFTED_ARGS(0x825C58B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
loc_825C58B8:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c58e8
  if (cr6.eq) goto loc_825C58E8;
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lhz r4,52(r29)
  r4 = REX_LOAD_U16(uint32_t(r29) + 52);
  // mr r3,r30
  r3 = r30;
  // bl 0x825cc320
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC320, LIFTED_ARGS(0x825C58D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lbz r10,12(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 12);
  // rlwinm r11,r11,30,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x7;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x825c58e8
  if (!cr6.gt) goto loc_825C58E8;
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
loc_825C58E8:
  // lwz r11,24(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 24);
  // mr r3,r26
  r3 = r26;
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c5910
  if (cr6.eq) goto loc_825C5910;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5910, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5910:
  // lbz r11,13(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 13);
  // clrlwi r10,r3,24
  r10 = uint32_t(r3) & 0xFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x825c5924
  if (!cr6.gt) goto loc_825C5924;
  // stb r3,13(r31)
  REX_STORE_U8(uint32_t(r31) + 13, uint8_t(r3));
loc_825C5924:
  // lwz r11,52(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 52);
  // clrlwi r10,r24,24
  r10 = uint32_t(r24) & 0xFF;
  // mr r28,r25
  r28 = r25;
  // addi r11,r11,36
  r11 = uint64_t(int64_t(int64_t(r11) + 36));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r25,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), uint32_t(r11), so);
  // clrlwi r24,r10,24
  r24 = uint32_t(r10) & 0xFF;
  // bne cr6,0x825c5830
  if (!cr6.eq) goto loc_825C5830;
loc_825C5944:
  // stb r24,2(r31)
  REX_STORE_U8(uint32_t(r31) + 2, uint8_t(r24));
loc_825C5948:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C5950, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5A70: lifted from generated/ by lift.py.
lifted::Result f825C5A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5A70 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r3
  r28 = r3;
  // mr r25,r5
  r25 = r5;
  // lwz r3,296(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 296);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5A98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // not r11,r11
  r11 = ~r11;
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5b14
  if (cr6.eq) goto loc_825C5B14;
  // ori r4,r4,32790
  r4 = r4 | 32790;
  // li r3,140
  r3 = uint64_t(int64_t(140));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C5AC0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5adc
  if (cr6.eq) goto loc_825C5ADC;
  // mr r6,r31
  r6 = r31;
  // lwz r4,56(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 56);
  // mr r5,r28
  r5 = r28;
  // bl 0x825ccc40
  // (left out: dead in C++)
  { const lifted::Result o = f825CCC40(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // b 0x825c5b38
  goto loc_825C5B38;
loc_825C5ADC:
  // li r27,0
  r27 = uint64_t(int64_t(0));
loc_825C5AE0:
  // lis r26,-32761
  r26 = uint64_t(int64_t(-2147024896));
  // ori r26,r26,14
  r26 = r26 | 14;
loc_825C5AE8:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825c5b08
  if (cr6.eq) goto loc_825C5B08;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5B08, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5B08:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C5B14:
  // ori r4,r4,32789
  r4 = r4 | 32789;
  // li r3,128
  r3 = uint64_t(int64_t(128));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C5B20, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5adc
  if (cr6.eq) goto loc_825C5ADC;
  // mr r6,r31
  r6 = r31;
  // lwz r4,56(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 56);
  // mr r5,r28
  r5 = r28;
  // bl 0x825cc810
  // (left out: dead in C++)
  { const lifted::Result o = f825CC810(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
loc_825C5B38:
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825c5ae0
  if (cr6.eq) goto loc_825C5AE0;
  // lwz r11,56(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 56);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lwz r31,124(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 124);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c5c18
  if (cr6.eq) goto loc_825C5C18;
  // lwz r11,28(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 28);
  // lhz r30,1(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 1);
  // mr r29,r30
  r29 = r30;
  // cmplwi cr6,r29,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 65535, so);
  // beq cr6,0x825c5c18
  if (cr6.eq) goto loc_825C5C18;
loc_825C5B6C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5B84, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825c5be8
  if (!cr6.eq) goto loc_825C5BE8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5BA4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825c5be8
  if (cr6.eq) goto loc_825C5BE8;
  // lwz r11,104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 104);
  // rlwinm r9,r29,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwzx r29,r9,r11
  r29 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x825c5be8
  if (cr6.lt) goto loc_825C5BE8;
  // lis r26,-30009
  r26 = uint64_t(int64_t(-1966669824));
  // ori r26,r26,8
  r26 = r26 | 8;
loc_825C5BE8:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5C00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // clrlwi r29,r30,16
  r29 = uint32_t(r30) & 0xFFFF;
  // cmplwi cr6,r29,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 65535, so);
  // bne cr6,0x825c5b6c
  if (!cr6.eq) goto loc_825C5B6C;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // blt cr6,0x825c5ae8
  if (cr6.lt) goto loc_825C5AE8;
loc_825C5C18:
  // addic. r4,r28,308
  ca = uint32_t(r28) > 4294966987;
  r4 = uint64_t(int64_t(int64_t(r28) + 308));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq 0x825c5c28
  if (cr0.eq) goto loc_825C5C28;
  // mr r3,r27
  r3 = r27;
  // bl 0x825ca980
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CA980, LIFTED_ARGS(0x825C5C28, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5C28:
  // lwz r11,72(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 72);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c5c68
  if (cr6.eq) goto loc_825C5C68;
  // lwz r11,56(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 56);
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5C4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r27
  r3 = r27;
  // lwz r6,64(r28)
  r6 = REX_LOAD_U32(uint32_t(r28) + 64);
  // clrlwi r5,r11,16
  r5 = uint32_t(r11) & 0xFFFF;
  // lhz r4,68(r28)
  r4 = REX_LOAD_U16(uint32_t(r28) + 68);
  // bl 0x825caa00
  // (left out: dead in C++)
  { const lifted::Result o = f825CAA00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
loc_825C5C68:
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // blt cr6,0x825c5ae8
  if (cr6.lt) goto loc_825C5AE8;
  // stw r27,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r27));
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5C80: lifted from generated/ by lift.py.
lifted::Result f825C5C80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5C80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r25,r5
  r25 = r5;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r29,r3
  r29 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r27,r6
  r27 = r6;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // mr r26,r7
  r26 = r7;
  // bl 0x825c5a70
  // (left out: dead in C++)
  { const lifted::Result o = f825C5A70(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r30,80(r1)
  r30 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c5cdc
  if (cr6.lt) goto loc_825C5CDC;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_825C5CDC:
  // lis r11,-30009
  r11 = uint64_t(int64_t(-1966669824));
  // ori r11,r11,25
  r11 = r11 | 25;
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // bne cr6,0x825c5d50
  if (!cr6.eq) goto loc_825C5D50;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // beq cr6,0x825c5d74
  if (cr6.eq) goto loc_825C5D74;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c5d14
  if (cr6.eq) goto loc_825C5D14;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5D14, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5D14:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x825c5a70
  // (left out: dead in C++)
  { const lifted::Result o = f825C5A70(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r30,80(r1)
  r30 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c5d74
  if (cr6.lt) goto loc_825C5D74;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5D4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_825C5D50:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c5d74
  if (cr6.lt) goto loc_825C5D74;
  // lbz r11,52(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 52);
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,16
  r11 = r11 | 16;
  // stb r11,52(r30)
  REX_STORE_U8(uint32_t(r30) + 52, uint8_t(r11));
  // stw r30,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r30));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C5D74:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x825c5d94
  if (cr6.eq) goto loc_825C5D94;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5D94, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5D94:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5DA0: lifted from generated/ by lift.py.
lifted::Result f825C5DA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C5DA0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r31,r6
  r31 = r6;
  // mr r28,r9
  r28 = r9;
  // lwz r3,296(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 296);
  // bl 0x825c6678
  // (left out: dead in C++)
  { const lifted::Result o = f825C6678(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5e0c
  if (cr6.eq) goto loc_825C5E0C;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // clrlwi r11,r31,16
  r11 = uint32_t(r31) & 0xFFFF;
  // rlwinm r6,r10,27,8,31
  r6 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0xFFFFFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5e0c
  if (cr6.eq) goto loc_825C5E0C;
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // cmplwi cr6,r10,18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 18, so);
  // bne cr6,0x825c5e0c
  if (!cr6.eq) goto loc_825C5E0C;
  // lwz r10,11(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 11);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lhz r10,80(r1)
  r10 = REX_LOAD_U16(uint32_t(r1) + 80);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x825c5e0c
  if (!cr6.gt) goto loc_825C5E0C;
  // lhz r10,82(r1)
  r10 = REX_LOAD_U16(uint32_t(r1) + 82);
  // mullw r6,r10,r11
  r6 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
loc_825C5E0C:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r5,r28
  r5 = r28;
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = f825C5C80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C5F88: lifted from generated/ by lift.py.
lifted::Result f825C5F88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C5F88 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x825c6560
  { const lifted::Result o = f825C6560(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C5F90: lifted from generated/ by lift.py.
lifted::Result f825C5F90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C5F90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c5ff4
  if (cr6.eq) goto loc_825C5FF4;
  // addic. r11,r31,-8
  ca = uint32_t(r31) > 7;
  r11 = uint64_t(int64_t(int64_t(r31) + -8));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r4,r31
  r4 = r31;
  // bne 0x825c5fc0
  if (!cr0.eq) goto loc_825C5FC0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_825C5FC0:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5FD0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,-8
  r11 = uint64_t(int64_t(int64_t(r31) + -8));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c5ff4
  if (cr6.eq) goto loc_825C5FF4;
  // addi r3,r11,12
  r3 = uint64_t(int64_t(int64_t(r11) + 12));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C5FF4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C5FF4:
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

// sub_825C6008: lifted from generated/ by lift.py.
lifted::Result f825C6008(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C6008 (lifted)");
  uint64_t r4 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-24
  r3 = uint64_t(int64_t(int64_t(r3) + -24));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C6020: lifted from generated/ by lift.py.
lifted::Result f825C6020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825C6020 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // stw r4,336(r3)
  REX_STORE_U32(uint32_t(r3) + 336, uint32_t(r4));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C6038: lifted from generated/ by lift.py.
uint64_t f825C6038(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C6038 (lifted)");
  // lwz r3,336(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 336);
  // blr
  return r3;
}

// sub_825C6080: lifted from generated/ by lift.py.
lifted::Result f825C6080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6080 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x825cc0e0
  // (left out: dead in C++)
  f825CC0E0(base, r3, r4, r5, r6);
  // lwz r3,28(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c60a8
  if (cr6.eq) goto loc_825C60A8;
  // bl 0x825ca860
  // (left out: dead in C++)
  { const lifted::Result o = f825CA860(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825C60A8:
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

// sub_825C6148: lifted from generated/ by lift.py.
lifted::Result f825C6148(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6148 (lifted)");
  uint64_t r4 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,22648
  r11 = uint64_t(int64_t(int64_t(r11) + 22648));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r28,r31,8
  r28 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r26,r31,12
  r26 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r24,r31,24
  r24 = uint64_t(int64_t(int64_t(r31) + 24));
  // addi r10,r10,22636
  r10 = uint64_t(int64_t(int64_t(r10) + 22636));
  // addi r9,r9,24748
  r9 = uint64_t(int64_t(int64_t(r9) + 24748));
  // addi r8,r8,22632
  r8 = uint64_t(int64_t(int64_t(r8) + 22632));
  // addi r7,r7,24732
  r7 = uint64_t(int64_t(int64_t(r7) + 24732));
  // lis r11,8324
  r11 = uint64_t(int64_t(545521664));
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r9,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r9));
  // ori r29,r11,1
  r29 = r11 | 1;
  // stw r8,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r8));
  // stw r7,0(r24)
  REX_STORE_U32(uint32_t(r24) + 0, uint32_t(r7));
  // beq cr6,0x825c61c0
  if (cr6.eq) goto loc_825C61C0;
  // mr r4,r29
  r4 = r29;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C61C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C61C0:
  // lwz r3,312(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 312);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c61d4
  if (cr6.eq) goto loc_825C61D4;
  // mr r4,r29
  r4 = r29;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C61D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C61D4:
  // lwz r3,344(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 344);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c61e8
  if (cr6.eq) goto loc_825C61E8;
  // mr r4,r29
  r4 = r29;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C61E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C61E8:
  // lwz r11,320(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 320);
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c6208
  if (cr6.eq) goto loc_825C6208;
  // lhz r4,304(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r3,296(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 296);
  // stw r25,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r25));
  // bl 0x825c7540
  // (left out: dead in C++)
  { const lifted::Result o = f825C7540(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_825C6208:
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4d90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C4D90, LIFTED_ARGS(0x825C6210, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,288(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 288);
  // rlwinm r27,r3,0,27,28
  r27 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x18;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825c626c
  if (!cr6.eq) goto loc_825C626C;
  // addi r29,r31,76
  r29 = uint64_t(int64_t(int64_t(r31) + 76));
loc_825C622C:
  // lwz r11,204(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c6240
  if (cr6.eq) goto loc_825C6240;
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // b 0x825c6244
  goto loc_825C6244;
loc_825C6240:
  // mr r3,r25
  r3 = r25;
loc_825C6244:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r4,r30
  r4 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6258, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,288(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 288);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c622c
  if (cr6.eq) goto loc_825C622C;
loc_825C626C:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // stw r25,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r25));
  // beq cr6,0x825c62a0
  if (cr6.eq) goto loc_825C62A0;
  // lwz r11,320(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 320);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825c6294
  if (cr6.eq) goto loc_825C6294;
  // lhz r4,304(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 304);
  // lwz r3,296(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 296);
  // stw r25,320(r31)
  REX_STORE_U32(uint32_t(r31) + 320, uint32_t(r25));
  // bl 0x825c7540
  // (left out: dead in C++)
  { const lifted::Result o = f825C7540(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_825C6294:
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = f825C4A98(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_825C62A0:
  // lwz r11,4(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 4);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x825c62d8
  if (cr6.eq) goto loc_825C62D8;
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // beq cr6,0x825c62d8
  if (cr6.eq) goto loc_825C62D8;
  // lwz r10,4(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 4);
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lwz r10,4(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r26,4(r26)
  REX_STORE_U32(uint32_t(r26) + 4, uint32_t(r26));
  // stw r26,8(r26)
  REX_STORE_U32(uint32_t(r26) + 8, uint32_t(r26));
loc_825C62D8:
  // lwz r11,296(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 296);
  // mr r4,r28
  r4 = r28;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C62F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,300(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 300);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c6314
  if (cr6.eq) goto loc_825C6314;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6314, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C6314:
  // lwz r3,328(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 328);
  // addi r29,r31,324
  r29 = uint64_t(int64_t(int64_t(r31) + 324));
  // cmplw cr6,r3,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r29), so);
  // beq cr6,0x825c6364
  if (cr6.eq) goto loc_825C6364;
loc_825C6324:
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r30,4(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r3,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r3));
  // stw r3,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6358, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // bne cr6,0x825c6324
  if (!cr6.eq) goto loc_825C6324;
loc_825C6364:
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c4a98
  // (left out: dead in C++)
  { const lifted::Result o = f825C4A98(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,76
  r3 = uint64_t(int64_t(int64_t(r31) + 76));
  // addi r31,r11,21316
  r31 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c63b0
  if (cr6.eq) goto loc_825C63B0;
loc_825C638C:
  // lwz r11,204(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 204);
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r10,204(r3)
  REX_STORE_U32(uint32_t(r3) + 204, uint32_t(r10));
  // bne cr6,0x825c638c
  if (!cr6.eq) goto loc_825C638C;
loc_825C63B0:
  // stw r25,212(r3)
  REX_STORE_U32(uint32_t(r3) + 212, uint32_t(r25));
  // stw r25,208(r3)
  REX_STORE_U32(uint32_t(r3) + 208, uint32_t(r25));
  // bl 0x825c1f30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1F30, LIFTED_ARGS(0x825C63BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r24
  r3 = r24;
  // bl 0x825cc0d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC0D0, LIFTED_ARGS(0x825C63C4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r31));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C63D0: lifted from generated/ by lift.py.
lifted::Result f825C63D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C63D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-48(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // mr r29,r4
  r29 = r4;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C63FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // blt cr6,0x825c6480
  if (cr6.lt) goto loc_825C6480;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825C642C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c6480
  if (cr6.lt) goto loc_825C6480;
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825c6478
  if (cr6.eq) goto loc_825C6478;
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x825c6478
  if (cr6.eq) goto loc_825C6478;
  // lfs f0,20(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // beq cr6,0x825c6480
  if (cr6.eq) goto loc_825C6480;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825c1f88
  // (left out: dead in C++)
  { const lifted::Result o = f825C1F88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825c6480
  goto loc_825C6480;
loc_825C6478:
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,10
  r30 = r30 | 10;
loc_825C6480:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C6488, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C6498: lifted from generated/ by lift.py.
lifted::Result f825C6498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C6498 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r27,r5
  r27 = r5;
  // lwz r11,56(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 56);
  // addi r28,r11,144
  r28 = uint64_t(int64_t(int64_t(r11) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C64C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,56(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 56);
  // bl 0x825bfea0
  // (left out: dead in C++)
  r3 = f825BFEA0(base, r1, r13, r3);
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c654c
  if (cr6.lt) goto loc_825C654C;
  // clrlwi r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825c654c
  if (cr6.eq) goto loc_825C654C;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x825c654c
  if (cr6.eq) goto loc_825C654C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825C6504, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825c654c
  if (cr6.lt) goto loc_825C654C;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r10,16(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lbz r10,0(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 0);
  // clrlwi r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c6544
  if (cr6.eq) goto loc_825C6544;
  // mr r3,r28
  r3 = r28;
  // lfs f0,20(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // stfs f0,0(r27)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C6538, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C6544:
  // lis r30,-30009
  r30 = uint64_t(int64_t(-1966669824));
  // ori r30,r30,10
  r30 = r30 | 10;
loc_825C654C:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C6554, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C6560: lifted from generated/ by lift.py.
lifted::Result f825C6560(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C6560 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // bl 0x825c6148
  // (left out: dead in C++)
  { const lifted::Result o = f825C6148(base, r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c65a0
  if (cr6.eq) goto loc_825C65A0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32858
  r4 = r4 | 32858;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C659C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825C65A0:
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

// sub_825C65B8: lifted from generated/ by lift.py.
uint64_t f825C65B8(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C65B8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,0(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 0);
  // rlwinm r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825c65dc
  if (!cr6.eq) goto loc_825C65DC;
  // rlwinm r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825c65dc
  if (!cr6.eq) goto loc_825C65DC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_825C65DC:
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c65f0
  if (cr6.eq) goto loc_825C65F0;
  // lhz r3,10(r4)
  r3 = REX_LOAD_U16(uint32_t(r4) + 10);
  // blr
  return r3;
loc_825C65F0:
  // lhz r3,12(r4)
  r3 = REX_LOAD_U16(uint32_t(r4) + 12);
  // blr
  return r3;
}

// sub_825C65F8: lifted from generated/ by lift.py.
uint64_t f825C65F8(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C65F8 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r11,0(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 0);
  // rlwinm r10,r11,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c6670
  if (cr6.eq) goto loc_825C6670;
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // rlwinm r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // beq cr6,0x825c6644
  if (cr6.eq) goto loc_825C6644;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825c6630
  if (!cr6.eq) goto loc_825C6630;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // beq cr6,0x825c6634
  if (cr6.eq) goto loc_825C6634;
loc_825C6630:
  // lhz r11,10(r4)
  r11 = REX_LOAD_U16(uint32_t(r4) + 10);
loc_825C6634:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // add r11,r11,r4
  r11 = r11 + r4;
  // lhz r3,10(r11)
  r3 = REX_LOAD_U16(uint32_t(r11) + 10);
  // blr
  return r3;
loc_825C6644:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825c665c
  if (!cr6.eq) goto loc_825C665C;
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // beq cr6,0x825c6660
  if (cr6.eq) goto loc_825C6660;
loc_825C665C:
  // lhz r11,12(r4)
  r11 = REX_LOAD_U16(uint32_t(r4) + 12);
loc_825C6660:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // add r11,r11,r4
  r11 = r11 + r4;
  // lhz r3,12(r11)
  r3 = REX_LOAD_U16(uint32_t(r11) + 12);
  // blr
  return r3;
loc_825C6670:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825C6678: lifted from generated/ by lift.py.
lifted::Result f825C6678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C6678 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-1184(r1)
  ea = -1184 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r27,r7
  r27 = r7;
  // mr r25,r8
  r25 = r8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C66AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x825c67dc
  if (!cr6.eq) goto loc_825C67DC;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C66D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,1024
  r5 = uint64_t(int64_t(1024));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825C66EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,1(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 1);
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r26
  r30 = r26;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // stw r26,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r26));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // lwz r10,24(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 24);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C671C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c67dc
  if (cr6.eq) goto loc_825C67DC;
  // addi r29,r1,96
  r29 = uint64_t(int64_t(int64_t(r1) + 96));
loc_825C6728:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // beq cr6,0x825c6740
  if (cr6.eq) goto loc_825C6740;
  // cmplwi cr6,r11,18
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 18, so);
  // bne cr6,0x825c6770
  if (!cr6.eq) goto loc_825C6770;
loc_825C6740:
  // lwz r11,7(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 7);
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // blt cr6,0x825c6770
  if (cr6.lt) goto loc_825C6770;
  // cmplw cr6,r11,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r25), so);
  // bgt cr6,0x825c6770
  if (cr6.gt) goto loc_825C6770;
  // stw r3,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r3));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne cr6,0x825c6790
  if (!cr6.eq) goto loc_825C6790;
  // cmpwi cr6,r30,256
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 256, so);
  // beq cr6,0x825c6790
  if (cr6.eq) goto loc_825C6790;
loc_825C6770:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C6788, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x825c6728
  if (!cr6.eq) goto loc_825C6728;
loc_825C6790:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // ble cr6,0x825c67dc
  if (!cr6.gt) goto loc_825C67DC;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x825c67b8
  if (cr6.eq) goto loc_825C67B8;
  // mr r3,r26
  r3 = r26;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // addi r1,r1,1184
  r1 = uint64_t(int64_t(int64_t(r1) + 1184));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C67B8:
  // addi r5,r30,-1
  r5 = uint64_t(int64_t(int64_t(r30) + -1));
  // lwz r3,24(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 24);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825bf6b8
  // (left out: dead in C++)
  r3 = f825BF6B8(base, r1, r13, r3, r4, r5);
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // addi r1,r1,1184
  r1 = uint64_t(int64_t(int64_t(r1) + 1184));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C67DC:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,1184
  r1 = uint64_t(int64_t(int64_t(r1) + 1184));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C6858: lifted from generated/ by lift.py.
lifted::Result f825C6858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825C6858 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x825c5e58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5E58, LIFTED_ARGS(0x825C6870, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,22936
  r11 = uint64_t(int64_t(int64_t(r11) + 22936));
  // addi r10,r10,22636
  r10 = uint64_t(int64_t(int64_t(r10) + 22636));
  // addi r9,r9,24748
  r9 = uint64_t(int64_t(int64_t(r9) + 24748));
  // addi r8,r8,22932
  r8 = uint64_t(int64_t(int64_t(r8) + 22932));
  // addi r7,r7,24732
  r7 = uint64_t(int64_t(int64_t(r7) + 24732));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r8));
  // stw r7,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r7));
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

// sub_825C68C8: lifted from generated/ by lift.py.
lifted::Result f825C68C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C68C8 (lifted)");
  uint64_t r4 = 0;
  // mr r4,r3
  r4 = r3;
  // lwz r3,56(r4)
  r3 = REX_LOAD_U32(uint32_t(r4) + 56);
  // b 0x825c0470
  { const lifted::Result o = f825C0470(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

namespace {

lifted::Result e825C0248(uint8_t* base, const lifted::Args& a) {
  return f825C0248(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C0458(uint8_t* base, const lifted::Args& a) {
  return f825C0458(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C0470(uint8_t* base, const lifted::Args& a) {
  return f825C0470(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C0488(uint8_t* base, const lifted::Args& a) {
  return f825C0488(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e825C05C8(uint8_t* base, const lifted::Args& a) {
  return f825C05C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0698(uint8_t* base, const lifted::Args& a) {
  return f825C0698(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0730(uint8_t* base, const lifted::Args& a) {
  return f825C0730(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C07F0(uint8_t* base, const lifted::Args& a) {
  return f825C07F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0848(uint8_t* base, const lifted::Args& a) {
  return f825C0848(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C08C0(uint8_t* base, const lifted::Args& a) {
  return f825C08C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C0958(uint8_t* base, const lifted::Args& a) {
  return f825C0958(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C0B48(uint8_t* base, const lifted::Args& a) {
  return f825C0B48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C0B50(uint8_t* base, const lifted::Args& a) {
  return f825C0B50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0B58(uint8_t* base, const lifted::Args& a) {
  return f825C0B58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0B60(uint8_t* base, const lifted::Args& a) {
  return f825C0B60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0C78(uint8_t* base, const lifted::Args& a) {
  return f825C0C78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0D90(uint8_t* base, const lifted::Args& a) {
  return f825C0D90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0E88(uint8_t* base, const lifted::Args& a) {
  return f825C0E88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C0F90(uint8_t* base, const lifted::Args& a) {
  return f825C0F90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1090(uint8_t* base, const lifted::Args& a) {
  return f825C1090(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C11A0(uint8_t* base, const lifted::Args& a) {
  return f825C11A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1238(uint8_t* base, const lifted::Args& a) {
  return f825C1238(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C12F0(uint8_t* base, const lifted::Args& a) {
  return f825C12F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1548(uint8_t* base, const lifted::Args& a) {
  f825C1548(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825C1568(uint8_t* base, const lifted::Args& a) {
  return f825C1568(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C15E8(uint8_t* base, const lifted::Args& a) {
  return f825C15E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1738(uint8_t* base, const lifted::Args& a) {
  f825C1738(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825C1788(uint8_t* base, const lifted::Args& a) {
  return f825C1788(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C1800(uint8_t* base, const lifted::Args& a) {
  return f825C1800(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C1990(uint8_t* base, const lifted::Args& a) {
  return f825C1990(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1BC0(uint8_t* base, const lifted::Args& a) {
  return f825C1BC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C1D00(uint8_t* base, const lifted::Args& a) {
  return f825C1D00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C1D90(uint8_t* base, const lifted::Args& a) {
  return f825C1D90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C1E08(uint8_t* base, const lifted::Args& a) {
  return f825C1E08(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825C1F88(uint8_t* base, const lifted::Args& a) {
  return f825C1F88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C2308(uint8_t* base, const lifted::Args& a) {
  return f825C2308(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C2368(uint8_t* base, const lifted::Args& a) {
  return f825C2368(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C25B0(uint8_t* base, const lifted::Args& a) {
  return f825C25B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C2650(uint8_t* base, const lifted::Args& a) {
  return f825C2650(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C2B50(uint8_t* base, const lifted::Args& a) {
  return f825C2B50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C2BA8(uint8_t* base, const lifted::Args& a) {
  return f825C2BA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C2BB0(uint8_t* base, const lifted::Args& a) {
  return f825C2BB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C2D90(uint8_t* base, const lifted::Args& a) {
  return f825C2D90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C2F00(uint8_t* base, const lifted::Args& a) {
  return f825C2F00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C2FA0(uint8_t* base, const lifted::Args& a) {
  return f825C2FA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C3040(uint8_t* base, const lifted::Args& a) {
  return {f825C3040(base, a.r[0]), a.f[0]};
}

lifted::Result e825C3048(uint8_t* base, const lifted::Args& a) {
  return f825C3048(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C3050(uint8_t* base, const lifted::Args& a) {
  return f825C3050(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C3110(uint8_t* base, const lifted::Args& a) {
  return f825C3110(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C3398(uint8_t* base, const lifted::Args& a) {
  return f825C3398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825C3460(uint8_t* base, const lifted::Args& a) {
  return f825C3460(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C34B0(uint8_t* base, const lifted::Args& a) {
  return f825C34B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C37A8(uint8_t* base, const lifted::Args& a) {
  return f825C37A8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C38A8(uint8_t* base, const lifted::Args& a) {
  return f825C38A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C39F8(uint8_t* base, const lifted::Args& a) {
  return f825C39F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C40E0(uint8_t* base, const lifted::Args& a) {
  return f825C40E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C4138(uint8_t* base, const lifted::Args& a) {
  return f825C4138(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4580(uint8_t* base, const lifted::Args& a) {
  return f825C4580(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4940(uint8_t* base, const lifted::Args& a) {
  return f825C4940(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4A58(uint8_t* base, const lifted::Args& a) {
  return f825C4A58(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C4A78(uint8_t* base, const lifted::Args& a) {
  f825C4A78(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825C4A98(uint8_t* base, const lifted::Args& a) {
  return f825C4A98(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e825C4B38(uint8_t* base, const lifted::Args& a) {
  return f825C4B38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4B98(uint8_t* base, const lifted::Args& a) {
  return f825C4B98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4C20(uint8_t* base, const lifted::Args& a) {
  return f825C4C20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4CF8(uint8_t* base, const lifted::Args& a) {
  return f825C4CF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4E30(uint8_t* base, const lifted::Args& a) {
  return f825C4E30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4E88(uint8_t* base, const lifted::Args& a) {
  return f825C4E88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C4F00(uint8_t* base, const lifted::Args& a) {
  return f825C4F00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C4FD0(uint8_t* base, const lifted::Args& a) {
  return f825C4FD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C4FE8(uint8_t* base, const lifted::Args& a) {
  return f825C4FE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5060(uint8_t* base, const lifted::Args& a) {
  return f825C5060(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5128(uint8_t* base, const lifted::Args& a) {
  return f825C5128(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5238(uint8_t* base, const lifted::Args& a) {
  return f825C5238(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5350(uint8_t* base, const lifted::Args& a) {
  return f825C5350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C53F8(uint8_t* base, const lifted::Args& a) {
  return f825C53F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C54A0(uint8_t* base, const lifted::Args& a) {
  return f825C54A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5610(uint8_t* base, const lifted::Args& a) {
  return f825C5610(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C5628(uint8_t* base, const lifted::Args& a) {
  return f825C5628(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C5680(uint8_t* base, const lifted::Args& a) {
  return f825C5680(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825C56D0(uint8_t* base, const lifted::Args& a) {
  return f825C56D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5790(uint8_t* base, const lifted::Args& a) {
  return f825C5790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5A70(uint8_t* base, const lifted::Args& a) {
  return f825C5A70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5C80(uint8_t* base, const lifted::Args& a) {
  return f825C5C80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5DA0(uint8_t* base, const lifted::Args& a) {
  return f825C5DA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C5F88(uint8_t* base, const lifted::Args& a) {
  return f825C5F88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C5F90(uint8_t* base, const lifted::Args& a) {
  return f825C5F90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C6008(uint8_t* base, const lifted::Args& a) {
  return f825C6008(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C6020(uint8_t* base, const lifted::Args& a) {
  return f825C6020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825C6038(uint8_t* base, const lifted::Args& a) {
  return {f825C6038(base, a.r[0]), a.f[0]};
}

lifted::Result e825C6080(uint8_t* base, const lifted::Args& a) {
  return f825C6080(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C6148(uint8_t* base, const lifted::Args& a) {
  return f825C6148(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825C63D0(uint8_t* base, const lifted::Args& a) {
  return f825C63D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C6498(uint8_t* base, const lifted::Args& a) {
  return f825C6498(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C6560(uint8_t* base, const lifted::Args& a) {
  return f825C6560(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C65B8(uint8_t* base, const lifted::Args& a) {
  return {f825C65B8(base, a.r[1]), a.f[0]};
}

lifted::Result e825C65F8(uint8_t* base, const lifted::Args& a) {
  return {f825C65F8(base, a.r[1]), a.f[0]};
}

lifted::Result e825C6678(uint8_t* base, const lifted::Args& a) {
  return f825C6678(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C6858(uint8_t* base, const lifted::Args& a) {
  return f825C6858(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e825C68C8(uint8_t* base, const lifted::Args& a) {
  return f825C68C8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

const Registration kEntries[] = {
    {0x825C0248, e825C0248},
    {0x825C0458, e825C0458},
    {0x825C0470, e825C0470},
    {0x825C0488, e825C0488},
    {0x825C05C8, e825C05C8},
    {0x825C0698, e825C0698},
    {0x825C0730, e825C0730},
    {0x825C07F0, e825C07F0},
    {0x825C0848, e825C0848},
    {0x825C08C0, e825C08C0},
    {0x825C0958, e825C0958},
    {0x825C0B48, e825C0B48},
    {0x825C0B50, e825C0B50},
    {0x825C0B58, e825C0B58},
    {0x825C0B60, e825C0B60},
    {0x825C0C78, e825C0C78},
    {0x825C0D90, e825C0D90},
    {0x825C0E88, e825C0E88},
    {0x825C0F90, e825C0F90},
    {0x825C1090, e825C1090},
    {0x825C11A0, e825C11A0},
    {0x825C1238, e825C1238},
    {0x825C12F0, e825C12F0},
    {0x825C1548, e825C1548},
    {0x825C1568, e825C1568},
    {0x825C15E8, e825C15E8},
    {0x825C1738, e825C1738},
    {0x825C1788, e825C1788},
    {0x825C1800, e825C1800},
    {0x825C1990, e825C1990},
    {0x825C1BC0, e825C1BC0},
    {0x825C1D00, e825C1D00},
    {0x825C1D90, e825C1D90},
    {0x825C1E08, e825C1E08},
    {0x825C1F88, e825C1F88},
    {0x825C2308, e825C2308},
    {0x825C2368, e825C2368},
    {0x825C25B0, e825C25B0},
    {0x825C2650, e825C2650},
    {0x825C2B50, e825C2B50},
    {0x825C2BA8, e825C2BA8},
    {0x825C2BB0, e825C2BB0},
    {0x825C2D90, e825C2D90},
    {0x825C2F00, e825C2F00},
    {0x825C2FA0, e825C2FA0},
    {0x825C3040, e825C3040},
    {0x825C3048, e825C3048},
    {0x825C3050, e825C3050},
    {0x825C3110, e825C3110},
    {0x825C3398, e825C3398},
    {0x825C3460, e825C3460},
    {0x825C34B0, e825C34B0},
    {0x825C37A8, e825C37A8},
    {0x825C38A8, e825C38A8},
    {0x825C39F8, e825C39F8},
    {0x825C40E0, e825C40E0},
    {0x825C4138, e825C4138},
    {0x825C4580, e825C4580},
    {0x825C4940, e825C4940},
    {0x825C4A58, e825C4A58},
    {0x825C4A78, e825C4A78},
    {0x825C4A98, e825C4A98},
    {0x825C4B38, e825C4B38},
    {0x825C4B98, e825C4B98},
    {0x825C4C20, e825C4C20},
    {0x825C4CF8, e825C4CF8},
    {0x825C4E30, e825C4E30},
    {0x825C4E88, e825C4E88},
    {0x825C4F00, e825C4F00},
    {0x825C4FD0, e825C4FD0},
    {0x825C4FE8, e825C4FE8},
    {0x825C5060, e825C5060},
    {0x825C5128, e825C5128},
    {0x825C5238, e825C5238},
    {0x825C5350, e825C5350},
    {0x825C53F8, e825C53F8},
    {0x825C54A0, e825C54A0},
    {0x825C5610, e825C5610},
    {0x825C5628, e825C5628},
    {0x825C5680, e825C5680},
    {0x825C56D0, e825C56D0},
    {0x825C5790, e825C5790},
    {0x825C5A70, e825C5A70},
    {0x825C5C80, e825C5C80},
    {0x825C5DA0, e825C5DA0},
    {0x825C5F88, e825C5F88},
    {0x825C5F90, e825C5F90},
    {0x825C6008, e825C6008},
    {0x825C6020, e825C6020},
    {0x825C6038, e825C6038},
    {0x825C6080, e825C6080},
    {0x825C6148, e825C6148},
    {0x825C63D0, e825C63D0},
    {0x825C6498, e825C6498},
    {0x825C6560, e825C6560},
    {0x825C65B8, e825C65B8},
    {0x825C65F8, e825C65F8},
    {0x825C6678, e825C6678},
    {0x825C6858, e825C6858},
    {0x825C68C8, e825C68C8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_022)), true);

}  // namespace

}  // namespace lifted
