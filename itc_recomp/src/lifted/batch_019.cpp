// Lift sweep batch 019 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_019, true, "ITC",
              "Run lift sweep batch 019's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_8257CF10: lifted from generated/ by lift.py.
uint64_t f8257CF10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257CF10 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
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
  // mr r29,r3
  r29 = r3;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r31,0(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r5,r31
  r5 = r31;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // lwz r3,392(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 392);
  // bl 0x8257c160
  // (left out: dead in C++)
  r3 = f8257C160(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8257cf74
  if (!cr0.lt) goto loc_8257CF74;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r10,r10,-2
  r10 = uint64_t(int64_t(int64_t(r10) + -2));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // b 0x8257cf9c
  goto loc_8257CF9C;
loc_8257CF74:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r29,380
  r11 = uint64_t(int64_t(int64_t(r29) + 380));
  // stw r10,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257cf94
  if (cr0.eq) goto loc_8257CF94;
  // stw r30,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r30));
  // b 0x8257cf98
  goto loc_8257CF98;
loc_8257CF94:
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
loc_8257CF98:
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
loc_8257CF9C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_8257CFA8: lifted from generated/ by lift.py.
lifted::Result f8257CFA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257CFA8 (lifted)");
  uint64_t r4 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8257d004
  if (cr6.eq) goto loc_8257D004;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257cfe8
  if (cr0.eq) goto loc_8257CFE8;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,3
  r4 = r4 | 3;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r30,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r30));
loc_8257CFE8:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8257c710
  // (left out: dead in C++)
  { const lifted::Result o = f8257C710(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32787
  r4 = r4 | 32787;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8257D004:
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

// sub_8257D020: lifted from generated/ by lift.py.
lifted::Result f8257D020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8257D020 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r28,-32768
  r28 = uint64_t(int64_t(-2147483648));
  // mr r30,r3
  r30 = r3;
  // ori r28,r28,16389
  r28 = r28 | 16389;
  // bl 0x8257c9a0
  // (left out: dead in C++)
  { const lifted::Result o = f8257C9A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8257d194
  if (cr0.eq) goto loc_8257D194;
  // addi r31,r30,372
  r31 = uint64_t(int64_t(int64_t(r30) + 372));
  // lwz r29,0(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r29,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq 0x8257d190
  if (cr0.eq) goto loc_8257D190;
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // ld r11,16(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 16);
  // ld r9,24(r10)
  r9 = REX_LOAD_U64(uint32_t(r10) + 24);
  // cmpld cr6,r9,r11
  cr6 = lifted::Compare<uint64_t>(r9, r11, so);
  // beq cr6,0x8257d10c
  if (cr6.eq) goto loc_8257D10C;
  // lwz r11,316(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 316);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,316(r30)
  REX_STORE_U32(uint32_t(r30) + 316, uint32_t(r11));
  // lwz r11,352(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 352);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8257d188
  if (cr6.eq) goto loc_8257D188;
  // addi r29,r30,364
  r29 = uint64_t(int64_t(int64_t(r30) + 364));
  // lwz r31,0(r29)
  r31 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x8257d1c8
  if (cr0.eq) goto loc_8257D1C8;
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8257d0a4
  if (!cr6.eq) goto loc_8257D0A4;
  // stw r27,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r27));
loc_8257D0A4:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r4,r30,340
  r4 = uint64_t(int64_t(int64_t(r30) + 340));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // stw r27,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r27));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8257cf10
  // (left out: dead in C++)
  r3 = f8257CF10(base, r1, r13, r3, r4);
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge 0x8257d180
  if (!cr0.lt) goto loc_8257D180;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257d1a8
  if (!cr6.eq) goto loc_8257D1A8;
  // stw r31,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r31));
  // b 0x8257d1a8
  goto loc_8257D1A8;
loc_8257D10C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257d134
  if (cr0.eq) goto loc_8257D134;
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x8257d128
  if (!cr6.eq) goto loc_8257D128;
  // stw r27,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r27));
loc_8257D128:
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
loc_8257D134:
  // addi r3,r30,340
  r3 = uint64_t(int64_t(int64_t(r30) + 340));
  // lwz r5,12(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + 12);
  // lwz r4,8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 8);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // stw r11,352(r30)
  REX_STORE_U32(uint32_t(r30) + 352, uint32_t(r11));
  // bl 0x8257cf10
  // (left out: dead in C++)
  r3 = f8257CF10(base, r1, r13, r3, r4);
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge 0x8257d19c
  if (!cr0.lt) goto loc_8257D19C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r11,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257d1a8
  if (!cr6.eq) goto loc_8257D1A8;
  // stw r29,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r29));
  // b 0x8257d1a8
  goto loc_8257D1A8;
loc_8257D180:
  // stw r27,352(r30)
  REX_STORE_U32(uint32_t(r30) + 352, uint32_t(r27));
  // b 0x8257d1a8
  goto loc_8257D1A8;
loc_8257D188:
  // mr r28,r27
  r28 = r27;
  // b 0x8257d1a8
  goto loc_8257D1A8;
loc_8257D190:
  // stw r27,72(r30)
  REX_STORE_U32(uint32_t(r30) + 72, uint32_t(r27));
loc_8257D194:
  // std r27,56(r30)
  REX_STORE_U64(uint32_t(r30) + 56, r27);
  // b 0x8257d1a8
  goto loc_8257D1A8;
loc_8257D19C:
  // ld r11,56(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 56);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // std r11,56(r30)
  REX_STORE_U64(uint32_t(r30) + 56, r11);
loc_8257D1A8:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x8257d1c8
  if (cr6.lt) goto loc_8257D1C8;
  // ld r10,16(r30)
  r10 = REX_LOAD_U64(uint32_t(r30) + 16);
  // lwz r11,320(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 320);
  // addi r10,r10,20
  r10 = uint64_t(int64_t(int64_t(r10) + 20));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // std r10,16(r30)
  REX_STORE_U64(uint32_t(r30) + 16, r10);
  // stw r11,320(r30)
  REX_STORE_U32(uint32_t(r30) + 320, uint32_t(r11));
loc_8257D1C8:
  // ld r11,56(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 56);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // ble cr6,0x8257d1dc
  if (!cr6.gt) goto loc_8257D1DC;
  // addi r11,r11,-20
  r11 = uint64_t(int64_t(int64_t(r11) + -20));
  // std r11,56(r30)
  REX_STORE_U64(uint32_t(r30) + 56, r11);
loc_8257D1DC:
  // addi r29,r30,380
  r29 = uint64_t(int64_t(int64_t(r30) + 380));
  // b 0x8257d27c
  goto loc_8257D27C;
loc_8257D1E4:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // beq cr6,0x8257d288
  if (cr6.eq) goto loc_8257D288;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257d21c
  if (cr0.eq) goto loc_8257D21C;
  // lwz r10,4(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8257d210
  if (!cr6.eq) goto loc_8257D210;
  // stw r27,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r27));
loc_8257D210:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
loc_8257D21C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r10,r10,-2
  r10 = uint64_t(int64_t(int64_t(r10) + -2));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r5,12(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,364
  r11 = uint64_t(int64_t(int64_t(r30) + 364));
  // stw r27,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r27));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257d274
  if (cr0.eq) goto loc_8257D274;
  // stw r31,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r31));
  // b 0x8257d278
  goto loc_8257D278;
loc_8257D274:
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
loc_8257D278:
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
loc_8257D27C:
  // lwz r31,0(r29)
  r31 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne 0x8257d1e4
  if (!cr0.eq) goto loc_8257D1E4;
loc_8257D288:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257D298: lifted from generated/ by lift.py.
lifted::Result f8257D298(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8257D298 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r26,r4
  r26 = r4;
  // mr r31,r3
  r31 = r3;
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r25,r5
  r25 = r5;
  // mr r28,r24
  r28 = r24;
  // lhz r11,0(r26)
  r11 = REX_LOAD_U16(uint32_t(r26) + 0);
  // lwz r10,72(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 72);
  // rlwinm r9,r11,20,12,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 20) & 0xFFFFF;
  // clrlwi r27,r11,20
  r27 = uint32_t(r11) & 0xFFF;
  // clrlwi r30,r9,28
  r30 = uint32_t(r9) & 0xF;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x8257d330
  if (!cr6.eq) goto loc_8257D330;
  // lwz r11,336(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 336);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8257d310
  if (cr6.eq) goto loc_8257D310;
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822956F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,30504
  r11 = uint64_t(int64_t(int64_t(r11) + 30504));
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // clrldi r10,r3,32
  r10 = r3 & 0xFFFFFFFF;
  // bne cr6,0x8257d308
  if (!cr6.eq) goto loc_8257D308;
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // std r9,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r9);
loc_8257D308:
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
  // b 0x8257d31c
  goto loc_8257D31C;
loc_8257D310:
  // mr r3,r31
  r3 = r31;
  // bl 0x8257c948
  // (left out: dead in C++)
  { const lifted::Result o = f8257C948(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // std r3,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r3);
loc_8257D31C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // std r24,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r24);
  // stb r30,308(r31)
  REX_STORE_U8(uint32_t(r31) + 308, uint8_t(r30));
  // sth r27,32(r31)
  REX_STORE_U16(uint32_t(r31) + 32, uint16_t(r27));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
loc_8257D330:
  // lbz r11,308(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 308);
  // clrlwi r29,r30,24
  r29 = uint32_t(r30) & 0xFF;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // beq cr6,0x8257d34c
  if (cr6.eq) goto loc_8257D34C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257ccd8
  // (left out: dead in C++)
  r3 = f8257CCD8(base, r1, r13, r3);
  // stb r30,308(r31)
  REX_STORE_U8(uint32_t(r31) + 308, uint8_t(r30));
loc_8257D34C:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257ca10
  // (left out: dead in C++)
  r3 = f8257CA10(base, r3, r4, r5);
  // lwz r11,304(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 304);
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,304(r31)
  REX_STORE_U32(uint32_t(r31) + 304, uint32_t(r11));
  // blt cr6,0x8257d39c
  if (cr6.lt) goto loc_8257D39C;
  // beq cr6,0x8257d41c
  if (cr6.eq) goto loc_8257D41C;
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // bge cr6,0x8257d454
  if (!cr6.lt) goto loc_8257D454;
  // lwz r11,360(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 360);
  // lwz r10,356(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 356);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x8257d450
  if (!cr6.lt) goto loc_8257D450;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257cc08
  // (left out: dead in C++)
  f8257CC08(base, r3, r4);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8257ca10
  // (left out: dead in C++)
  r3 = f8257CA10(base, r3, r4, r5);
loc_8257D39C:
  // addi r29,r31,364
  r29 = uint64_t(int64_t(int64_t(r31) + 364));
  // lwz r30,0(r29)
  r30 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x8257d450
  if (cr0.eq) goto loc_8257D450;
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x8257d3bc
  if (!cr6.eq) goto loc_8257D3BC;
  // stw r24,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r24));
loc_8257D3BC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r5,r25
  r5 = r25;
  // ld r10,80(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 80);
  // mr r4,r26
  r4 = r26;
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // stw r24,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r24));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // std r10,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r10);
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // stw r25,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r25));
  // bl 0x8257cb38
  // (left out: dead in C++)
  r3 = f8257CB38(base, r3, r4);
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge 0x8257d454
  if (!cr0.lt) goto loc_8257D454;
  // stw r24,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r24));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257d43c
  if (cr0.eq) goto loc_8257D43C;
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // b 0x8257d440
  goto loc_8257D440;
loc_8257D41C:
  // lbz r11,308(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 308);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x8257d454
  if (!cr6.eq) goto loc_8257D454;
  // lwz r11,312(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 312);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,312(r31)
  REX_STORE_U32(uint32_t(r31) + 312, uint32_t(r11));
  // b 0x8257d454
  goto loc_8257D454;
loc_8257D43C:
  // stw r30,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r30));
loc_8257D440:
  // stw r30,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r30));
  // lwz r11,304(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 304);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,304(r31)
  REX_STORE_U32(uint32_t(r31) + 304, uint32_t(r11));
loc_8257D450:
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_8257D454:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257D460: lifted from generated/ by lift.py.
void f8257D460(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257D460 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // ld r10,40(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 40);
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // addi r8,r3,372
  r8 = uint64_t(int64_t(int64_t(r3) + 372));
  // divdu r10,r10,r9
  r10 = r9 ? r10 / r9 : 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // sth r10,64(r3)
  REX_STORE_U16(uint32_t(r3) + 64, uint16_t(r10));
  // lwz r10,0(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + 0);
  // std r11,16(r3)
  REX_STORE_U64(uint32_t(r3) + 16, r11);
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,72(r3)
  REX_STORE_U32(uint32_t(r3) + 72, uint32_t(r11));
  // std r11,296(r3)
  REX_STORE_U64(uint32_t(r3) + 296, r11);
  // stw r11,304(r3)
  REX_STORE_U32(uint32_t(r3) + 304, uint32_t(r11));
  // stb r11,308(r3)
  REX_STORE_U8(uint32_t(r3) + 308, uint8_t(r11));
  // stw r11,312(r3)
  REX_STORE_U32(uint32_t(r3) + 312, uint32_t(r11));
  // beq 0x8257d4f8
  if (cr0.eq) goto loc_8257D4F8;
  // addi r7,r3,364
  r7 = uint64_t(int64_t(int64_t(r3) + 364));
loc_8257D4A4:
  // lwz r9,0(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8257d4cc
  if (cr0.eq) goto loc_8257D4CC;
  // lwz r6,4(r8)
  r6 = REX_LOAD_U32(uint32_t(r8) + 4);
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // bne cr6,0x8257d4c0
  if (!cr6.eq) goto loc_8257D4C0;
  // stw r11,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r11));
loc_8257D4C0:
  // lwz r6,4(r9)
  r6 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r6,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r6));
  // stw r11,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r11));
loc_8257D4CC:
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r9,4(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 4);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8257d4e4
  if (cr0.eq) goto loc_8257D4E4;
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // b 0x8257d4e8
  goto loc_8257D4E8;
loc_8257D4E4:
  // stw r10,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r10));
loc_8257D4E8:
  // stw r10,4(r7)
  REX_STORE_U32(uint32_t(r7) + 4, uint32_t(r10));
  // lwz r10,0(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x8257d4a4
  if (!cr0.eq) goto loc_8257D4A4;
loc_8257D4F8:
  // addi r10,r3,80
  r10 = uint64_t(int64_t(int64_t(r3) + 80));
  // li r11,25
  r11 = uint64_t(int64_t(25));
loc_8257D500:
  // ld r9,280(r3)
  r9 = REX_LOAD_U64(uint32_t(r3) + 280);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // std r9,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bne 0x8257d500
  if (!cr0.eq) goto loc_8257D500;
  // blr
  return;
}

// sub_8257D518: lifted from generated/ by lift.py.
lifted::Result f8257D518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257D518 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r29,r4
  r29 = r4;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r30,r3
  r30 = r3;
  // ori r4,r4,32787
  r4 = r4 | 32787;
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x8257d550
  if (!cr0.eq) goto loc_8257D550;
loc_8257D544:
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,14
  r30 = r30 | 14;
  // b 0x8257d6b4
  goto loc_8257D6B4;
loc_8257D550:
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // stw r30,392(r31)
  REX_STORE_U32(uint32_t(r31) + 392, uint32_t(r30));
  // li r10,28
  r10 = uint64_t(int64_t(28));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // li r7,10
  r7 = uint64_t(int64_t(10));
  // lfd f0,30008(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 30008)));
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stfd f0,280(r31)
  REX_STORE_U64(uint32_t(r31) + 280, std::bit_cast<uint64_t>(f0));
  // addi r3,r31,328
  r3 = uint64_t(int64_t(int64_t(r31) + 328));
  // sth r9,64(r31)
  REX_STORE_U16(uint32_t(r31) + 64, uint16_t(r9));
  // sth r8,68(r31)
  REX_STORE_U16(uint32_t(r31) + 68, uint16_t(r8));
  // sth r10,66(r31)
  REX_STORE_U16(uint32_t(r31) + 66, uint16_t(r10));
  // lfd f13,30000(r11)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 30000)));
  // li r11,240
  r11 = uint64_t(int64_t(240));
  // stfd f13,288(r31)
  REX_STORE_U64(uint32_t(r31) + 288, std::bit_cast<uint64_t>(f13));
  // stw r7,360(r31)
  REX_STORE_U32(uint32_t(r31) + 360, uint32_t(r7));
  // stw r6,336(r31)
  REX_STORE_U32(uint32_t(r31) + 336, uint32_t(r6));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // std r11,48(r31)
  REX_STORE_U64(uint32_t(r31) + 48, r11);
  // std r11,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r11);
  // bl 0x825b9a70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9A70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8257d5d4
  if (!cr0.eq) goto loc_8257D5D4;
  // bl 0x82295f18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295F18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bgt 0x8257d5c8
  if (cr0.gt) goto loc_8257D5C8;
  // mr r30,r3
  r30 = r3;
  // b 0x8257d6ac
  goto loc_8257D6AC;
loc_8257D5C8:
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // oris r30,r11,32775
  r30 = r11 | 2147942400;
  // b 0x8257d6ac
  goto loc_8257D6AC;
loc_8257D5D4:
  // addi r10,r31,80
  r10 = uint64_t(int64_t(int64_t(r31) + 80));
  // li r11,25
  r11 = uint64_t(int64_t(25));
loc_8257D5DC:
  // lfd f0,280(r31)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r31) + 280)));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stfd f0,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, std::bit_cast<uint64_t>(f0));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bne 0x8257d5dc
  if (!cr0.eq) goto loc_8257D5DC;
  // lwz r11,24(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 24);
  // mr r7,r31
  r7 = r31;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r5,8(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 8);
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // addi r3,r11,2
  r3 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8257c858
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8257C858, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x8257d6b4
  if (cr0.lt) goto loc_8257D6B4;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,3
  r4 = r4 | 3;
  // rlwinm r3,r11,3,0,28
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // beq 0x8257d544
  if (cr0.eq) goto loc_8257D544;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8257d6ac
  if (!cr6.gt) goto loc_8257D6AC;
  // addi r7,r31,364
  r7 = uint64_t(int64_t(int64_t(r31) + 364));
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8257D650:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r6,4(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r5,8(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mullw r11,r5,r8
  r11 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r8))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stwx r11,r10,r6
  REX_STORE_U32(uint32_t(r10) + uint32_t(r6), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r9,4(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 4);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8257d690
  if (cr0.eq) goto loc_8257D690;
  // stw r11,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r11));
  // b 0x8257d694
  goto loc_8257D694;
loc_8257D690:
  // stw r11,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r11));
loc_8257D694:
  // stw r11,4(r7)
  REX_STORE_U32(uint32_t(r7) + 4, uint32_t(r11));
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // cmplw cr6,r8,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r11), so);
  // blt cr6,0x8257d650
  if (cr6.lt) goto loc_8257D650;
loc_8257D6AC:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x8257d6c0
  if (!cr6.lt) goto loc_8257D6C0;
loc_8257D6B4:
  // mr r3,r31
  r3 = r31;
  // bl 0x8257cfa8
  // (left out: dead in C++)
  { const lifted::Result o = f8257CFA8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_8257D6C0:
  // mr r3,r30
  r3 = r30;
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257D6D8: lifted from generated/ by lift.py.
void f8257D6D8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257D6D8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lwz r11,200(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 200);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8257d6ec
  if (!cr0.eq) goto loc_8257D6EC;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,140(r3)
  REX_STORE_U32(uint32_t(r3) + 140, uint32_t(r10));
loc_8257D6EC:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,200(r3)
  REX_STORE_U32(uint32_t(r3) + 200, uint32_t(r11));
  // blr
  return;
}

// sub_8257E8E8: lifted from generated/ by lift.py.
lifted::Result f8257E8E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_8257E8E8 (lifted)");
  uint64_t r4 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r11,r3
  r11 = r3;
  // mr r3,r9
  r3 = r9;
  // stb r7,128(r1)
  REX_STORE_U8(uint32_t(r1) + 128, uint8_t(r7));
  // li r7,3
  r7 = uint64_t(int64_t(3));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stb r9,112(r1)
  REX_STORE_U8(uint32_t(r1) + 112, uint8_t(r9));
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stb r9,88(r1)
  REX_STORE_U8(uint32_t(r1) + 88, uint8_t(r9));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r8,72(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 72);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // stb r7,136(r1)
  REX_STORE_U8(uint32_t(r1) + 136, uint8_t(r7));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // stb r10,120(r1)
  REX_STORE_U8(uint32_t(r1) + 120, uint8_t(r10));
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // stb r10,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r10));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // stw r9,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r9));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // stb r7,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r7));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // stw r7,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r7));
  // bne cr6,0x8257e9a8
  if (!cr6.eq) goto loc_8257E9A8;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r8,r1,116
  r8 = uint64_t(int64_t(int64_t(r1) + 116));
  // addi r9,r11,36
  r9 = uint64_t(int64_t(int64_t(r11) + 36));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // lfs f0,2608(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 2608);
  f0 = double(temp.f32);
loc_8257E97C:
  // lwz r7,0(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmplwi cr6,r7,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 65535, so);
  // bgt cr6,0x8257e98c
  if (cr6.gt) goto loc_8257E98C;
  // stfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
loc_8257E98C:
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r8,r8,8
  r8 = uint64_t(int64_t(int64_t(r8) + 8));
  // bne 0x8257e97c
  if (!cr0.eq) goto loc_8257E97C;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,68(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 68);
  // bl 0x82459f48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459F48, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8257E9A8:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8257E9B8: lifted from generated/ by lift.py.
lifted::Result f8257E9B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8257E9B8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // lwz r5,72(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplw cr6,r30,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r5), so);
  // beq cr6,0x8257eab4
  if (cr6.eq) goto loc_8257EAB4;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stb r29,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r29));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // addi r10,r1,116
  r10 = uint64_t(int64_t(int64_t(r1) + 116));
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r6,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r6));
  // addi r9,r31,36
  r9 = uint64_t(int64_t(int64_t(r31) + 36));
  // lfs f0,2612(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 2612);
  f0 = double(temp.f32);
  // lfs f31,2608(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2608);
  f31 = double(temp.f32);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // mr r11,r6
  r11 = r6;
loc_8257EA2C:
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // stb r6,-4(r10)
  REX_STORE_U8(uint32_t(r10) + -4, uint8_t(r6));
  // cmplwi cr6,r8,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 65535, so);
  // stb r11,-3(r10)
  REX_STORE_U8(uint32_t(r10) + -3, uint8_t(r11));
  // bgt cr6,0x8257ea58
  if (cr6.gt) goto loc_8257EA58;
  // lbz r8,64(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 64);
  // slw r7,r29,r11
  r7 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r29) << (uint8_t(r11) & 0x3F));
  // and. r8,r8,r7
  r8 = r8 & r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8257ea58
  if (!cr0.eq) goto loc_8257EA58;
  // stfs f31,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // b 0x8257ea5c
  goto loc_8257EA5C;
loc_8257EA58:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
loc_8257EA5C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8257ea2c
  if (cr6.lt) goto loc_8257EA2C;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // beq cr6,0x8257ea80
  if (cr6.eq) goto loc_8257EA80;
  // mr r3,r5
  r3 = r5;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459E18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8257EA80:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r30,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r30));
  // bne cr6,0x8257ea94
  if (!cr6.eq) goto loc_8257EA94;
  // lwz r30,76(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 76);
  // b 0x8257eaa4
  goto loc_8257EAA4;
loc_8257EA94:
  // mr r3,r30
  r3 = r30;
  // bl 0x82459db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459DB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,116(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stb r29,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r29));
loc_8257EAA4:
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // stw r30,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r30));
  // bl 0x82459ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459EF0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8257EAB4:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257EAF0: lifted from generated/ by lift.py.
lifted::Result f8257EAF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257EAF0 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,20(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8257eb48
  if (!cr6.gt) goto loc_8257EB48;
  // addi r31,r30,12
  r31 = uint64_t(int64_t(int64_t(r30) + 12));
loc_8257EB18:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r28,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r3), so);
  // beq cr6,0x8257eb54
  if (cr6.eq) goto loc_8257EB54;
  // lwz r11,20(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8257eb18
  if (cr6.lt) goto loc_8257EB18;
loc_8257EB48:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8257EB4C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_8257EB54:
  // addi r11,r29,3
  r11 = uint64_t(int64_t(int64_t(r29) + 3));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r30
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r30));
  // b 0x8257eb4c
  goto loc_8257EB4C;
}

// sub_8257EDE8: lifted from generated/ by lift.py.
lifted::Result f8257EDE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257EDE8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8257eec4
  if (cr6.eq) goto loc_8257EEC4;
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r29,r28
  r29 = r28;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8257ee38
  if (!cr6.gt) goto loc_8257EE38;
  // addi r30,r31,24
  r30 = uint64_t(int64_t(int64_t(r31) + 24));
loc_8257EE18:
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x8257c3e0
  // (left out: dead in C++)
  { const lifted::Result o = f8257C3E0(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r28));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8257ee18
  if (cr6.lt) goto loc_8257EE18;
loc_8257EE38:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // mr r29,r28
  r29 = r28;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8257ee78
  if (!cr6.gt) goto loc_8257EE78;
  // addi r30,r31,12
  r30 = uint64_t(int64_t(int64_t(r31) + 12));
loc_8257EE4C:
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r28));
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8257ee4c
  if (cr6.lt) goto loc_8257EE4C;
loc_8257EE78:
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257ee8c
  if (cr0.eq) goto loc_8257EE8C;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459E18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r28));
loc_8257EE8C:
  // lwz r3,76(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 76);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257eea0
  if (cr0.eq) goto loc_8257EEA0;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459E18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r28));
loc_8257EEA0:
  // lwz r3,72(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257eeb4
  if (cr0.eq) goto loc_8257EEB4;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459E18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r28));
loc_8257EEB4:
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32784
  r4 = r4 | 32784;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8257EEC4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257EF80: lifted from generated/ by lift.py.
lifted::Result f8257EF80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257EF80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r5
  r29 = r5;
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // mr r27,r3
  r27 = r3;
  // ori r28,r28,87
  r28 = r28 | 87;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8257efe8
  if (cr6.eq) goto loc_8257EFE8;
  // mr r31,r4
  r31 = r4;
loc_8257EFAC:
  // mr r3,r27
  r3 = r27;
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8257eaf0
  // (left out: dead in C++)
  { const lifted::Result o = f8257EAF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257efe8
  if (cr0.eq) goto loc_8257EFE8;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt 0x8257efe8
  if (cr0.lt) goto loc_8257EFE8;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // blt cr6,0x8257efac
  if (cr6.lt) goto loc_8257EFAC;
loc_8257EFE8:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257EFF8: lifted from generated/ by lift.py.
lifted::Result f8257EFF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257EFF8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r5
  r29 = r5;
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // mr r27,r3
  r27 = r3;
  // ori r28,r28,87
  r28 = r28 | 87;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8257f060
  if (cr6.eq) goto loc_8257F060;
  // mr r31,r4
  r31 = r4;
loc_8257F024:
  // mr r3,r27
  r3 = r27;
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8257eaf0
  // (left out: dead in C++)
  { const lifted::Result o = f8257EAF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257f060
  if (cr0.eq) goto loc_8257F060;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r28,r3
  r28 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt 0x8257f060
  if (cr0.lt) goto loc_8257F060;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // blt cr6,0x8257f024
  if (cr6.lt) goto loc_8257F024;
loc_8257F060:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257F070: lifted from generated/ by lift.py.
lifted::Result f8257F070(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8257F070 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r26,r4
  r26 = r4;
  // mr r30,r5
  r30 = r5;
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,36
  r3 = uint64_t(int64_t(int64_t(r31) + 36));
  // mr r28,r6
  r28 = r6;
  // mr r29,r7
  r29 = r7;
  // mr r27,r8
  r27 = r8;
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stb r27,64(r31)
  REX_STORE_U8(uint32_t(r31) + 64, uint8_t(r27));
  // bne cr6,0x8257f0d8
  if (!cr6.eq) goto loc_8257F0D8;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8257f0d8
  if (!cr6.eq) goto loc_8257F0D8;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257e9b8
  // (left out: dead in C++)
  { const lifted::Result o = f8257E9B8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x8257f0f4
  if (!cr6.eq) goto loc_8257F0F4;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257e8e8
  // (left out: dead in C++)
  { const lifted::Result o = f8257E8E8(base, r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8257f0f4
  goto loc_8257F0F4;
loc_8257F0D8:
  // mr r6,r29
  r6 = r29;
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257eb68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8257EB68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8257f104
  if (cr0.lt) goto loc_8257F104;
loc_8257F0F4:
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // std r26,0(r31)
  REX_STORE_U64(uint32_t(r31) + 0, r26);
  // stw r10,1308(r11)
  REX_STORE_U32(uint32_t(r11) + 1308, uint32_t(r10));
loc_8257F104:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257F198: lifted from generated/ by lift.py.
lifted::Result f8257F198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8257F198 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0;
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
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // lis r31,-32768
  r31 = uint64_t(int64_t(-2147483648));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // ori r31,r31,16389
  r31 = r31 | 16389;
  // lwz r4,29992(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 29992);
  // bl 0x8257eaf0
  // (left out: dead in C++)
  { const lifted::Result o = f8257EAF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257f1d8
  if (cr0.eq) goto loc_8257F1D8;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x82579590
  // (left out: dead in C++)
  { const lifted::Result o = f82579590(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
loc_8257F1D8:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257F1E8: lifted from generated/ by lift.py.
lifted::Result f8257F1E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_8257F1E8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r4,9
  r11 = uint64_t(int64_t(int64_t(r4) + 9));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r11,r3
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  // cmplw cr6,r5,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r10), so);
  // beq cr6,0x8257f20c
  if (cr6.eq) goto loc_8257F20C;
  // stwx r5,r11,r3
  REX_STORE_U32(uint32_t(r11) + uint32_t(r3), uint32_t(r5));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // stw r10,1308(r11)
  REX_STORE_U32(uint32_t(r11) + 1308, uint32_t(r10));
loc_8257F20C:
  // b 0x8257e8e8
  { const lifted::Result o = f8257E8E8(base, r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8257F970: lifted from generated/ by lift.py.
lifted::Result f8257F970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8257F970 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x8257f994
  if (!cr6.eq) goto loc_8257F994;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257F994:
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257f9a8
  if (!cr6.eq) goto loc_8257F9A8;
  // li r3,6130
  r3 = uint64_t(int64_t(6130));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257F9A8:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x8257f9bc
  if (!cr0.eq) goto loc_8257F9BC;
  // li r3,6125
  r3 = uint64_t(int64_t(6125));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257F9BC:
  // lhz r10,14(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 14);
  // lhz r11,12(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 12);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8257f9d4
  if (cr6.lt) goto loc_8257F9D4;
  // li r3,6145
  r3 = uint64_t(int64_t(6145));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257F9D4:
  // addi r29,r31,18
  r29 = uint64_t(int64_t(int64_t(r31) + 18));
  // lhz r10,0(r29)
  r10 = REX_LOAD_U16(uint32_t(r29) + 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8257f9ec
  if (cr6.lt) goto loc_8257F9EC;
  // li r3,6150
  r3 = uint64_t(int64_t(6150));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257F9EC:
  // lbz r11,16(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 16);
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // ble cr6,0x8257fa00
  if (!cr6.gt) goto loc_8257FA00;
  // li r3,6135
  r3 = uint64_t(int64_t(6135));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257FA00:
  // addi r30,r31,20
  r30 = uint64_t(int64_t(int64_t(r31) + 20));
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // ble cr6,0x8257fa18
  if (!cr6.gt) goto loc_8257FA18;
  // li r3,6140
  r3 = uint64_t(int64_t(6140));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257FA18:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x8257fa80
  if (cr6.eq) goto loc_8257FA80;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi cr6,r11,64206
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64206, so);
  // bne cr6,0x8257fa80
  if (!cr6.eq) goto loc_8257FA80;
  // lbz r11,12(r28)
  r11 = REX_LOAD_U8(uint32_t(r28) + 12);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x8257fa40
  if (cr6.eq) goto loc_8257FA40;
  // li r3,6110
  r3 = uint64_t(int64_t(6110));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257FA40:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,16(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 16);
  // bl 0x825828d0
  // (left out: dead in C++)
  { const lifted::Result o = f825828D0(base, r1, r13, r3, r4, r5, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r4,16(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 16);
  // bl 0x82582158
  // (left out: dead in C++)
  { const lifted::Result o = f82582158(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r6,r30
  r6 = r30;
  // lhz r7,12(r31)
  r7 = REX_LOAD_U16(uint32_t(r31) + 12);
  // mr r5,r29
  r5 = r29;
  // lwz r4,8(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x825820e8
  // (left out: dead in C++)
  r3 = f825820E8(base, r1, r13, r3, r4, r5, r6, r7, r8);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8257fa84
  goto loc_8257FA84;
loc_8257FA80:
  // li r3,6100
  r3 = uint64_t(int64_t(6100));
loc_8257FA84:
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257FA90: lifted from generated/ by lift.py.
lifted::Result f8257FA90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8257FA90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8257f970
  // (left out: dead in C++)
  { const lifted::Result o = f8257F970(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // ld r10,88(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 88);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // addi r11,r11,-32288
  r11 = uint64_t(int64_t(int64_t(r11) + -32288));
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // cmpd cr6,r10,r9
  cr6 = lifted::Compare<int64_t>(int64_t(r10), int64_t(r9), so);
  // bge cr6,0x8257faf0
  if (!cr6.lt) goto loc_8257FAF0;
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
loc_8257FAF0:
  // ld r9,16(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 16);
  // cmpd cr6,r10,r9
  cr6 = lifted::Compare<int64_t>(int64_t(r10), int64_t(r9), so);
  // ble cr6,0x8257fb00
  if (!cr6.gt) goto loc_8257FB00;
  // std r10,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r10);
loc_8257FB00:
  // ld r9,24(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 24);
  // mr r3,r31
  r3 = r31;
  // add r10,r9,r10
  r10 = r9 + r10;
  // std r10,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r10);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
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

// sub_8257FB38: lifted from generated/ by lift.py.
lifted::Result f8257FB38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FB38 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8257fb58
  if (!cr6.eq) goto loc_8257FB58;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x8257fbf0
  goto loc_8257FBF0;
loc_8257FB58:
  // lwz r31,0(r28)
  r31 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x8257fbec
  if (cr0.eq) goto loc_8257FBEC;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r11,64206
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64206, so);
  // bne cr6,0x8257fbec
  if (!cr6.eq) goto loc_8257FBEC;
  // lbz r11,12(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 12);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x8257fb84
  if (cr6.eq) goto loc_8257FB84;
  // li r3,6115
  r3 = uint64_t(int64_t(6115));
  // b 0x8257fbf0
  goto loc_8257FBF0;
loc_8257FB84:
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // bl 0x82582320
  // (left out: dead in C++)
  { const lifted::Result o = f82582320(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257fbb4
  if (cr0.eq) goto loc_8257FBB4;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r29));
loc_8257FBB4:
  // lwz r3,40(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257fbd0
  if (cr0.eq) goto loc_8257FBD0;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r29));
loc_8257FBD0:
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r29,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r29));
  // b 0x8257fbf0
  goto loc_8257FBF0;
loc_8257FBEC:
  // li r3,6100
  r3 = uint64_t(int64_t(6100));
loc_8257FBF0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257FBF8: lifted from generated/ by lift.py.
lifted::Result f8257FBF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FBF8 (lifted)");
  // b 0x8257fb38
  { const lifted::Result o = f8257FB38(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8257FC00: lifted from generated/ by lift.py.
lifted::Result f8257FC00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FC00 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x8257fc24
  if (!cr6.eq) goto loc_8257FC24;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x8257fd04
  goto loc_8257FD04;
loc_8257FC24:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,30560(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30560);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30564);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30568(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30568);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30572(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30572);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30576(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30576);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30580(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30580);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257fd00
  if (cr6.eq) goto loc_8257FD00;
  // lwz r11,30560(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30560);
  // li r4,44
  r4 = uint64_t(int64_t(44));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8257fcb0
  if (!cr0.eq) goto loc_8257FCB0;
loc_8257FCA8:
  // li r3,6000
  r3 = uint64_t(int64_t(6000));
  // b 0x8257fd04
  goto loc_8257FD04;
loc_8257FCB0:
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // lwz r4,24(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 24);
  // ori r9,r9,64206
  r9 = r9 | 64206;
  // li r8,100
  r8 = uint64_t(int64_t(100));
  // li r7,3000
  r7 = uint64_t(int64_t(3000));
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // stb r10,12(r11)
  REX_STORE_U8(uint32_t(r11) + 12, uint8_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // bl 0x825821f0
  // (left out: dead in C++)
  { const lifted::Result o = f825821F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257fcf8
  if (cr0.eq) goto loc_8257FCF8;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257fb38
  // (left out: dead in C++)
  { const lifted::Result o = f8257FB38(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8257fca8
  goto loc_8257FCA8;
loc_8257FCF8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8257fd04
  goto loc_8257FD04;
loc_8257FD00:
  // li r3,6001
  r3 = uint64_t(int64_t(6001));
loc_8257FD04:
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

// sub_8257FD18: lifted from generated/ by lift.py.
lifted::Result f8257FD18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FD18 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32104
  r11 = uint64_t(int64_t(-2103967744));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,-32288
  r11 = uint64_t(int64_t(int64_t(r11) + -32288));
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
  // std r10,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r10);
  // std r10,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r10);
  // std r10,32(r11)
  REX_STORE_U64(uint32_t(r11) + 32, r10);
  // lis r10,15258
  r10 = uint64_t(int64_t(999948288));
  // ori r10,r10,51712
  r10 = r10 | 51712;
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // b 0x8257fc00
  { const lifted::Result o = f8257FC00(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8257FD48: lifted from generated/ by lift.py.
lifted::Result f8257FD48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FD48 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f13 = 0.0;
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
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r3
  r7 = r3;
  // mr r31,r5
  r31 = r5;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x8257fe98
  if (cr6.eq) goto loc_8257FE98;
  // lwz r11,0(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 0);
  // cmplwi cr6,r11,64206
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64206, so);
  // bne cr6,0x8257fe98
  if (!cr6.eq) goto loc_8257FE98;
  // lbz r11,12(r7)
  r11 = REX_LOAD_U8(uint32_t(r7) + 12);
  // cmplwi cr6,r11,11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 11, so);
  // beq cr6,0x8257fd8c
  if (cr6.eq) goto loc_8257FD8C;
  // li r3,6115
  r3 = uint64_t(int64_t(6115));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FD8C:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x8257fd9c
  if (!cr6.eq) goto loc_8257FD9C;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FD9C:
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257fdb0
  if (!cr6.eq) goto loc_8257FDB0;
  // li r3,6130
  r3 = uint64_t(int64_t(6130));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FDB0:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257fdc4
  if (!cr6.eq) goto loc_8257FDC4;
  // li r3,6125
  r3 = uint64_t(int64_t(6125));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FDC4:
  // addi r8,r4,14
  r8 = uint64_t(int64_t(int64_t(r4) + 14));
  // lhz r11,12(r4)
  r11 = REX_LOAD_U16(uint32_t(r4) + 12);
  // lhz r10,0(r8)
  r10 = REX_LOAD_U16(uint32_t(r8) + 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8257fde0
  if (cr6.lt) goto loc_8257FDE0;
  // li r3,6145
  r3 = uint64_t(int64_t(6145));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FDE0:
  // lhz r10,18(r4)
  r10 = REX_LOAD_U16(uint32_t(r4) + 18);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8257fdf4
  if (cr6.lt) goto loc_8257FDF4;
  // li r3,6150
  r3 = uint64_t(int64_t(6150));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FDF4:
  // addi r9,r4,16
  r9 = uint64_t(int64_t(int64_t(r4) + 16));
  // lbz r11,0(r9)
  r11 = REX_LOAD_U8(uint32_t(r9) + 0);
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // ble cr6,0x8257fe0c
  if (!cr6.gt) goto loc_8257FE0C;
  // li r3,6135
  r3 = uint64_t(int64_t(6135));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FE0C:
  // lbz r11,20(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 20);
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // ble cr6,0x8257fe20
  if (!cr6.gt) goto loc_8257FE20;
  // li r3,6140
  r3 = uint64_t(int64_t(6140));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FE20:
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,16(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 16);
  // addi r30,r1,160
  r30 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r4,28(r7)
  REX_STORE_U32(uint32_t(r7) + 28, uint32_t(r4));
  // addi r5,r4,4
  r5 = uint64_t(int64_t(int64_t(r4) + 4));
  // lhz r7,12(r4)
  r7 = REX_LOAD_U16(uint32_t(r4) + 12);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r6,8(r4)
  r6 = REX_LOAD_U32(uint32_t(r4) + 8);
  // lwz r4,0(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r30,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r30));
  // bl 0x82582cc8
  // (left out: dead in C++)
  { const lifted::Result o = f82582CC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8257fe90
  if (cr6.eq) goto loc_8257FE90;
  // lfs f0,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // addi r10,r31,52
  r10 = uint64_t(int64_t(int64_t(r31) + 52));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8257FE68:
  // addi r9,r1,160
  r9 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfsx f0,r11,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  f0 = double(temp.f32);
  // lfsx f13,r11,r8
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  f13 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f0,-48(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + -48, temp.u32);
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // cmpwi cr6,r11,48
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 48, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // blt cr6,0x8257fe68
  if (cr6.lt) goto loc_8257FE68;
loc_8257FE90:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8257fe9c
  goto loc_8257FE9C;
loc_8257FE98:
  // li r3,6100
  r3 = uint64_t(int64_t(6100));
loc_8257FE9C:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
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

// sub_8257FEB8: lifted from generated/ by lift.py.
lifted::Result f8257FEB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8257FEB8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
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
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r5,0(r29)
  r5 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x8257fd48
  // (left out: dead in C++)
  { const lifted::Result o = f8257FD48(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // ld r10,88(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 88);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,30520
  r11 = uint64_t(int64_t(int64_t(r11) + 30520));
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // cmpd cr6,r10,r9
  cr6 = lifted::Compare<int64_t>(int64_t(r10), int64_t(r9), so);
  // bge cr6,0x8257ff18
  if (!cr6.lt) goto loc_8257FF18;
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
loc_8257FF18:
  // ld r9,16(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 16);
  // cmpd cr6,r10,r9
  cr6 = lifted::Compare<int64_t>(int64_t(r10), int64_t(r9), so);
  // ble cr6,0x8257ff28
  if (!cr6.gt) goto loc_8257FF28;
  // std r10,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r10);
loc_8257FF28:
  // ld r9,24(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 24);
  // mr r3,r31
  r3 = r31;
  // add r10,r9,r10
  r10 = r9 + r10;
  // std r10,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r10);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257FF50: lifted from generated/ by lift.py.
uint64_t f8257FF50(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257FF50 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lhz r11,92(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 92);
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // blr
  return r3;
}

// sub_8257FF68: lifted from generated/ by lift.py.
lifted::Result f8257FF68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257FF68 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8257ff84
  if (!cr6.eq) goto loc_8257FF84;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x82580018
  goto loc_82580018;
loc_8257FF84:
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82580014
  if (cr0.eq) goto loc_82580014;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r11,64206
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64206, so);
  // bne cr6,0x82580014
  if (!cr6.eq) goto loc_82580014;
  // lbz r11,12(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 12);
  // cmplwi cr6,r11,11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 11, so);
  // beq cr6,0x8257ffb0
  if (cr6.eq) goto loc_8257FFB0;
  // li r3,6115
  r3 = uint64_t(int64_t(6115));
  // b 0x82580018
  goto loc_82580018;
loc_8257FFB0:
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // bl 0x82583090
  // (left out: dead in C++)
  { const lifted::Result o = f82583090(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257ffe0
  if (cr0.eq) goto loc_8257FFE0;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r29));
loc_8257FFE0:
  // lwz r3,40(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257fffc
  if (cr0.eq) goto loc_8257FFFC;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r29));
loc_8257FFFC:
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82580018
  goto loc_82580018;
loc_82580014:
  // li r3,6100
  r3 = uint64_t(int64_t(6100));
loc_82580018:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82580020: lifted from generated/ by lift.py.
lifted::Result f82580020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82580020 (lifted)");
  // b 0x8257ff68
  { const lifted::Result o = f8257FF68(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82580028: lifted from generated/ by lift.py.
lifted::Result f82580028(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82580028 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x82580050
  if (!cr6.eq) goto loc_82580050;
  // li r3,6170
  r3 = uint64_t(int64_t(6170));
  // b 0x82580154
  goto loc_82580154;
loc_82580050:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // lwz r11,30560(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30560);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30564);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30568(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30568);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30572(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30572);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30576(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30576);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30580(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30580);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82580150
  if (cr6.eq) goto loc_82580150;
  // lwz r11,30560(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30560);
  // li r4,44
  r4 = uint64_t(int64_t(44));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825800dc
  if (!cr0.eq) goto loc_825800DC;
loc_825800D4:
  // li r3,6000
  r3 = uint64_t(int64_t(6000));
  // b 0x82580154
  goto loc_82580154;
loc_825800DC:
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r9,r9,64206
  r9 = r9 | 64206;
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // stb r10,12(r11)
  REX_STORE_U8(uint32_t(r11) + 12, uint8_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // bl 0x82582ec0
  // (left out: dead in C++)
  { const lifted::Result o = f82582EC0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82580110
  if (cr0.eq) goto loc_82580110;
  // mr r3,r31
  r3 = r31;
  // bl 0x8257ff68
  // (left out: dead in C++)
  { const lifted::Result o = f8257FF68(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x825800d4
  goto loc_825800D4;
loc_82580110:
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r31,r11,30520
  r31 = uint64_t(int64_t(int64_t(r11) + 30520));
  // lis r11,15258
  r11 = uint64_t(int64_t(999948288));
  // ori r11,r11,51712
  r11 = r11 | 51712;
  // std r30,0(r31)
  REX_STORE_U64(uint32_t(r31) + 0, r30);
  // std r30,8(r31)
  REX_STORE_U64(uint32_t(r31) + 8, r30);
  // std r30,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r30);
  // std r30,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r30);
  // std r30,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r30);
  // std r11,0(r31)
  REX_STORE_U64(uint32_t(r31) + 0, r11);
  // bl 0x825b9a70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9A70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r11));
  // b 0x82580154
  goto loc_82580154;
loc_82580150:
  // li r3,6001
  r3 = uint64_t(int64_t(6001));
loc_82580154:
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

// sub_82580170: lifted from generated/ by lift.py.
lifted::Result f82580170(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82580170 (lifted)");
  // b 0x82580028
  { const lifted::Result o = f82580028(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82580178: lifted from generated/ by lift.py.
uint64_t f82580178(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82580178 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30560(r11)
  REX_STORE_U32(uint32_t(r11) + 30560, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82580188: lifted from generated/ by lift.py.
uint64_t f82580188(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82580188 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30564(r11)
  REX_STORE_U32(uint32_t(r11) + 30564, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82580198: lifted from generated/ by lift.py.
uint64_t f82580198(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82580198 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30568(r11)
  REX_STORE_U32(uint32_t(r11) + 30568, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825801A8: lifted from generated/ by lift.py.
uint64_t f825801A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825801A8 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30572(r11)
  REX_STORE_U32(uint32_t(r11) + 30572, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825801B8: lifted from generated/ by lift.py.
uint64_t f825801B8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825801B8 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30576(r11)
  REX_STORE_U32(uint32_t(r11) + 30576, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825801C8: lifted from generated/ by lift.py.
uint64_t f825801C8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825801C8 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r3,30580(r11)
  REX_STORE_U32(uint32_t(r11) + 30580, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825820E8: lifted from generated/ by lift.py.
uint64_t f825820E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825820E8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // sth r7,84(r1)
  REX_STORE_U16(uint32_t(r1) + 84, uint16_t(r7));
  // clrlwi r9,r8,16
  r9 = uint32_t(r8) & 0xFFFF;
  // stw r5,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r5));
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // stw r6,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r6));
  // stw r4,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r4));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,30072
  r31 = uint64_t(int64_t(int64_t(r11) + 30072));
  // stw r10,60(r3)
  REX_STORE_U32(uint32_t(r3) + 60, uint32_t(r10));
  // stw r9,64(r3)
  REX_STORE_U32(uint32_t(r3) + 64, uint32_t(r9));
  // mr r30,r31
  r30 = r31;
loc_82582124:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lha r5,0(r30)
  r5 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r30) + 0))));
  // clrlwi r4,r11,24
  r4 = uint32_t(r11) & 0xFF;
  // bl 0x825834f8
  // (left out: dead in C++)
  f825834F8(base, r1, r3, r4, r5);
  // addi r30,r30,2
  r30 = uint64_t(int64_t(int64_t(r30) + 2));
  // addi r11,r31,34
  r11 = uint64_t(int64_t(int64_t(r31) + 34));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x82582124
  if (cr6.lt) goto loc_82582124;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82582158: lifted from generated/ by lift.py.
lifted::Result f82582158(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82582158 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0;
  lifted::Temp temp{};
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
  // mr r29,r5
  r29 = r5;
  // mr r30,r4
  r30 = r4;
  // addi r28,r29,8
  r28 = uint64_t(int64_t(int64_t(r29) + 8));
  // addi r26,r29,4
  r26 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r27,r28,4
  r27 = uint64_t(int64_t(int64_t(r28) + 4));
  // mr r31,r3
  r31 = r3;
  // lfs f1,56(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 56);
  f1 = double(temp.f32);
  // lfs f2,0(r26)
  temp.u32 = REX_LOAD_U32(uint32_t(r26) + 0);
  f2 = double(temp.f32);
  // lfs f3,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f3 = double(temp.f32);
  // bl 0x82583ce8
  // (left out: dead in C++)
  r3 = f82583CE8(base, f1, f2, f3);
  // mr r6,r27
  r6 = r27;
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // mr r5,r26
  r5 = r26;
  // lfs f1,56(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 56);
  f1 = double(temp.f32);
  // bl 0x82583d68
  // (left out: dead in C++)
  { const lifted::Result o = f82583D68(base, r1, r13, r3, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // lfs f1,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f1 = double(temp.f32);
  // bl 0x82583c00
  // (left out: dead in C++)
  { const lifted::Result o = f82583C00(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // stw r3,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r3));
  // bl 0x82583c98
  // (left out: dead in C++)
  f1 = f82583C98(base, r1, r13, r3);
  // stfs f1,0(r28)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lfs f1,16(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 16);
  f1 = double(temp.f32);
  // bl 0x82583ba8
  // (left out: dead in C++)
  r3 = f82583BA8(base, f1);
  // mr r11,r3
  r11 = r3;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // addi r4,r31,12
  r4 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r3,r29,20
  r3 = uint64_t(int64_t(int64_t(r29) + 20));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lfs f1,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f1 = double(temp.f32);
  // bl 0x82583910
  // (left out: dead in C++)
  { const lifted::Result o = f82583910(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // lfs f0,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f0 = double(temp.f32);
  // stfs f0,56(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 56, temp.u32);
  // lfs f0,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // stfs f0,60(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 60, temp.u32);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825821F0: lifted from generated/ by lift.py.
lifted::Result f825821F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825821F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-64(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82582218
  if (!cr6.eq) goto loc_82582218;
loc_82582210:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82582310
  goto loc_82582310;
loc_82582218:
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lis r29,-32107
  r29 = uint64_t(int64_t(-2104164352));
  // li r4,88
  r4 = uint64_t(int64_t(88));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r28,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r28));
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x82582210
  if (cr0.eq) goto loc_82582210;
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // stw r31,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r31));
  // stw r27,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r27));
  // li r4,40
  r4 = uint64_t(int64_t(40));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r26,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r26));
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // beq 0x82582210
  if (cr0.eq) goto loc_82582210;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // sth r28,28(r11)
  REX_STORE_U16(uint32_t(r11) + 28, uint16_t(r28));
  // mr r4,r31
  r4 = r31;
  // sth r28,36(r11)
  REX_STORE_U16(uint32_t(r11) + 36, uint16_t(r28));
  // li r3,371
  r3 = uint64_t(int64_t(371));
  // lfs f0,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // stfs f0,20(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // lfs f31,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f31 = double(temp.f32);
  // stfs f31,24(r11)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // bl 0x82584a00
  // (left out: dead in C++)
  { const lifted::Result o = f82584A00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82582210
  if (!cr0.eq) goto loc_82582210;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // stfs f31,8(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // bl 0x82584988
  // (left out: dead in C++)
  { const lifted::Result o = f82584988(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82582210
  if (!cr0.eq) goto loc_82582210;
  // addi r3,r31,20
  r3 = uint64_t(int64_t(int64_t(r31) + 20));
  // bl 0x825840d0
  // (left out: dead in C++)
  { const lifted::Result o = f825840D0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82582210
  if (!cr0.eq) goto loc_82582210;
  // addi r3,r31,24
  r3 = uint64_t(int64_t(int64_t(r31) + 24));
  // bl 0x82583fd0
  // (left out: dead in C++)
  { const lifted::Result o = f82583FD0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // extsh. r11,r3
  r11 = uint64_t(int64_t(int16_t(r3)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82582210
  if (!cr0.eq) goto loc_82582210;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f31,28(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f31,48(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stw r26,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r26));
  // stfs f31,56(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 56, temp.u32);
  // stw r26,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r26));
  // stw r28,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r28));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lfs f0,10764(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f0 = double(temp.f32);
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // stfs f0,60(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
loc_82582310:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82582320: lifted from generated/ by lift.py.
lifted::Result f82582320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82582320 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82582394
  if (cr0.eq) goto loc_82582394;
  // mr r3,r31
  r3 = r31;
  // bl 0x825876d8
  // (left out: dead in C++)
  { const lifted::Result o = f825876D8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x82584080
  // (left out: dead in C++)
  { const lifted::Result o = f82584080(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,20
  r3 = uint64_t(int64_t(int64_t(r31) + 20));
  // bl 0x82584178
  // (left out: dead in C++)
  { const lifted::Result o = f82584178(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,24
  r3 = uint64_t(int64_t(int64_t(r31) + 24));
  // bl 0x82584080
  // (left out: dead in C++)
  { const lifted::Result o = f82584080(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,72(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82582384
  if (cr0.eq) goto loc_82582384;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
loc_82582384:
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82582394:
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

// sub_825823B0: lifted from generated/ by lift.py.
lifted::Result f825823B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825823B0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // stwu r1,-3344(r1)
  ea = -3344 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,291
  r5 = uint64_t(int64_t(291));
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // bl 0x82584b90
  // (left out: dead in C++)
  { const lifted::Result o = f82584B90(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,30112
  r11 = uint64_t(int64_t(int64_t(r11) + 30112));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r7,145
  r7 = uint64_t(int64_t(145));
  // addi r8,r1,1240
  r8 = uint64_t(int64_t(int64_t(r1) + 1240));
loc_825823F4:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r9)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // lfs f13,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f13 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r8,r8,-4
  r8 = uint64_t(int64_t(int64_t(r8) + -4));
  // bgt 0x825823f4
  if (cr0.gt) goto loc_825823F4;
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // li r5,291
  r5 = uint64_t(int64_t(291));
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // bl 0x82584e48
  // (left out: dead in C++)
  { const lifted::Result o = f82584E48(base, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // addi r7,r1,2288
  r7 = uint64_t(int64_t(int64_t(r1) + 2288));
  // addi r6,r1,1248
  r6 = uint64_t(int64_t(int64_t(r1) + 1248));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,291
  r4 = uint64_t(int64_t(291));
  // lfs f0,30696(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 30696);
  f0 = double(temp.f32);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmuls f0,f1,f0
  f0 = double(float(f1 * f0));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // bl 0x82584cf0
  // (left out: dead in C++)
  { const lifted::Result o = f82584CF0(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,2288
  r4 = uint64_t(int64_t(int64_t(r1) + 2288));
  // addi r3,r1,1248
  r3 = uint64_t(int64_t(int64_t(r1) + 1248));
  // bl 0x82584758
  // (left out: dead in C++)
  { const lifted::Result o = f82584758(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,3344
  r1 = uint64_t(int64_t(int64_t(r1) + 3344));
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

// sub_825828D0: lifted from generated/ by lift.py.
lifted::Result f825828D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825828D0 (lifted)");
  uint64_t r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-4352(r1)
  ea = -4352 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r26,r5
  r26 = r5;
  // addi r28,r26,8
  r28 = uint64_t(int64_t(int64_t(r26) + 8));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r10,40(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r29,0(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r24,72(r31)
  r24 = REX_LOAD_U32(uint32_t(r31) + 72);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8258290c
  if (!cr0.eq) goto loc_8258290C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82582ad0
  goto loc_82582AD0;
loc_8258290C:
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r11,160
  r11 = uint64_t(int64_t(160));
loc_82582914:
  // lha r9,0(r3)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r3) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r3,r3,2
  r3 = uint64_t(int64_t(int64_t(r3) + 2));
  // std r9,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r9);
  // lfd f0,96(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bgt 0x82582914
  if (cr0.gt) goto loc_82582914;
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,160
  r5 = uint64_t(int64_t(160));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x82584a70
  // (left out: dead in C++)
  { const lifted::Result o = f82584A70(base, r1, r13, r3, r4, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // addi r25,r31,44
  r25 = uint64_t(int64_t(int64_t(r31) + 44));
  // mr r3,r29
  r3 = r29;
  // lwz r4,16(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 16);
  // mr r5,r25
  r5 = r25;
  // bl 0x825823b0
  // (left out: dead in C++)
  { const lifted::Result o = f825823B0(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // stfs f1,0(r28)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lwz r27,24(r31)
  r27 = REX_LOAD_U32(uint32_t(r31) + 24);
  // bl 0x825852f0
  // (left out: dead in C++)
  r3 = f825852F0(base, f1);
  // mr r30,r3
  r30 = r3;
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,145
  r5 = uint64_t(int64_t(145));
  // mr r4,r29
  r4 = r29;
  // mr r3,r27
  r3 = r27;
  // mr r6,r30
  r6 = r30;
  // bl 0x82583dc8
  // (left out: dead in C++)
  { const lifted::Result o = f82583DC8(base, r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r5,r30
  r5 = r30;
  // mr r3,r29
  r3 = r29;
  // addi r4,r27,145
  r4 = uint64_t(int64_t(int64_t(r27) + 145));
  // bl 0x82584b90
  // (left out: dead in C++)
  { const lifted::Result o = f82584B90(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lfs f1,8680(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f1 = double(temp.f32);
  // bl 0x82584eb0
  // (left out: dead in C++)
  r3 = f82584EB0(base, r3, r4, r6, f1);
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825850e0
  // (left out: dead in C++)
  { const lifted::Result o = f825850E0(base, r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // addi r7,r1,1168
  r7 = uint64_t(int64_t(int64_t(r1) + 1168));
  // addi r6,r1,2208
  r6 = uint64_t(int64_t(int64_t(r1) + 2208));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82584cf0
  // (left out: dead in C++)
  { const lifted::Result o = f82584CF0(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r9,r27
  r9 = r27;
  // mr r8,r30
  r8 = r30;
  // mr r7,r31
  r7 = r31;
  // mr r6,r26
  r6 = r26;
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // addi r5,r1,1168
  r5 = uint64_t(int64_t(int64_t(r1) + 1168));
  // addi r4,r1,2208
  r4 = uint64_t(int64_t(int64_t(r1) + 2208));
  // mr r3,r29
  r3 = r29;
  // bl 0x82582658
  // (left out: dead in C++)
  { const lifted::Result o = f82582658(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lfs f1,96(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f1 = double(temp.f32);
  // li r5,225
  r5 = uint64_t(int64_t(225));
  // extsh r6,r11
  r6 = uint64_t(int64_t(int16_t(r11)));
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // bl 0x82586758
  // (left out: dead in C++)
  { const lifted::Result o = f82586758(base, r1, r13, r3, r4, r5, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // lwz r3,76(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 76);
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // lfs f0,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f0 = double(temp.f32);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stfs f0,48(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stw r11,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r11));
  // beq 0x82582a5c
  if (cr0.eq) goto loc_82582A5C;
  // lfs f2,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f2 = double(temp.f32);
  // lfs f1,0(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 0);
  f1 = double(temp.f32);
  // bl 0x825865e8
  // (left out: dead in C++)
  { const lifted::Result o = f825865E8(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
loc_82582A5C:
  // lwz r11,68(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82582acc
  if (cr6.eq) goto loc_82582ACC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // lfs f13,20(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 20);
  f13 = double(temp.f32);
  // lfs f10,16(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 16);
  f10 = double(temp.f32);
  // lfs f9,24(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 24);
  f9 = double(temp.f32);
  // lfs f11,12(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 12);
  f11 = double(temp.f32);
  // lfs f12,10256(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10256);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fdivs f0,f12,f0
  f0 = double(float(f12 / f0));
  // lfs f8,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f8 = double(temp.f32);
  // fmuls f11,f11,f8
  f11 = double(float(f11 * f8));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,9596(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9596);
  f13 = double(temp.f32);
  // fmadds f0,f0,f10,f9
  f0 = double(float(std::fma(f0, f10, f9)));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82582aac
  if (!cr6.lt) goto loc_82582AAC;
  // fmr f0,f13
  f0 = f13;
loc_82582AAC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,7132(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7132);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82582ac0
  if (!cr6.gt) goto loc_82582AC0;
  // fmr f0,f13
  f0 = f13;
loc_82582AC0:
  // fdivs f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 / f0));
  // stfs f11,4(r28)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r28) + 4, temp.u32);
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
loc_82582ACC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82582AD0:
  // addi r1,r1,4352
  r1 = uint64_t(int64_t(int64_t(r1) + 4352));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82582AD8: lifted from generated/ by lift.py.
lifted::Result f82582AD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82582AD8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // lfs f27,112(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 112);
  f27 = double(temp.f32);
  // mr r29,r6
  r29 = r6;
  // lfs f28,108(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 108);
  f28 = double(temp.f32);
  // lfs f31,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f31 = double(temp.f32);
  // mr r27,r5
  r27 = r5;
  // lfs f30,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f30 = double(temp.f32);
  // addi r31,r29,8
  r31 = uint64_t(int64_t(int64_t(r29) + 8));
  // lfs f29,48(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 48);
  f29 = double(temp.f32);
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lfs f26,64(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 64);
  f26 = double(temp.f32);
  // rlwinm r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // blt cr6,0x82582b2c
  if (cr6.lt) goto loc_82582B2C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82582B2C:
  // lis r9,-32250
  r9 = uint64_t(int64_t(-2113536000));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r28,r9,30704
  r28 = uint64_t(int64_t(int64_t(r9) + 30704));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfsx f0,r11,r28
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r28));
  f0 = double(temp.f32);
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // bne cr6,0x82582b50
  if (!cr6.eq) goto loc_82582B50;
  // stfs f27,4(r29)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r29) + 4, temp.u32);
  // b 0x82582b54
  goto loc_82582B54;
loc_82582B50:
  // stfs f0,4(r29)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 4, temp.u32);
loc_82582B54:
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // lwa r10,8(r30)
  r10 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 8))));
  // lfs f0,31064(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 31064);
  f0 = double(temp.f32);
  // mr r11,r10
  r11 = r10;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f12,f13
  f12 = double(float(f13));
  // lfs f13,31060(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 31060);
  f13 = double(temp.f32);
  // fnmsubs f1,f12,f0,f13
  f1 = double(float(-std::fma(f12, f0, -f13)));
  // bl 0x825abe30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825ABE30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10256(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10256);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fmadds f13,f13,f31,f29
  f13 = double(float(std::fma(f13, f31, f29)));
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // lfs f13,3124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3124);
  f13 = double(temp.f32);
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82582bb8
  if (!cr6.lt) goto loc_82582BB8;
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
loc_82582BB8:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f0,-32296(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -32296);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x82582bd0
  if (!cr6.gt) goto loc_82582BD0;
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
loc_82582BD0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f0,10816(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10816);
  f0 = double(temp.f32);
  // fmuls f0,f28,f0
  f0 = double(float(f28 * f0));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82582c14
  if (cr6.gt) goto loc_82582C14;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,4208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f0,f28,f0
  f0 = double(float(f28 * f0));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82582c14
  if (cr6.lt) goto loc_82582C14;
  // fadds f13,f13,f28
  f13 = double(float(f13 + f28));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // b 0x82582c70
  goto loc_82582C70;
loc_82582C14:
  // fcmpu cr6,f27,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f27, f12);
  // ble cr6,0x82582c28
  if (!cr6.gt) goto loc_82582C28;
  // fmr f0,f28
  f0 = f28;
  // fmr f12,f27
  f12 = f27;
  // b 0x82582c2c
  goto loc_82582C2C;
loc_82582C28:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_82582C2C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,3396(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3396);
  f11 = double(temp.f32);
  // fcmpu cr6,f0,f11
  cr6 = lifted::CompareFloat(f0, f11);
  // bge cr6,0x82582c70
  if (!cr6.lt) goto loc_82582C70;
  // fmuls f11,f12,f0
  f11 = double(float(f12 * f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // lfs f12,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // fcmpu cr6,f11,f12
  cr6 = lifted::CompareFloat(f11, f12);
  // bge cr6,0x82582c70
  if (!cr6.lt) goto loc_82582C70;
  // fcmpu cr6,f13,f28
  cr6 = lifted::CompareFloat(f13, f28);
  // ble cr6,0x82582c6c
  if (!cr6.gt) goto loc_82582C6C;
  // fmr f0,f13
  f0 = f13;
  // b 0x82582c70
  goto loc_82582C70;
loc_82582C6C:
  // fmr f0,f28
  lifted::ScalarFloatMode();
  f0 = f28;
loc_82582C70:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stfs f0,0(r29)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // cmpwi cr6,r11,64
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 64, so);
  // blt cr6,0x82582c84
  if (cr6.lt) goto loc_82582C84;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82582C84:
  // addi r10,r28,32
  r10 = uint64_t(int64_t(int64_t(r28) + 32));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r7,r27
  r7 = r27;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // addi r4,r29,20
  r4 = uint64_t(int64_t(int64_t(r29) + 20));
  // addi r3,r30,12
  r3 = uint64_t(int64_t(int64_t(r30) + 12));
  // lfsx f0,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f0 = double(temp.f32);
  // stfs f0,16(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 16, temp.u32);
  // lfs f1,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f1 = double(temp.f32);
  // bl 0x825868e8
  // (left out: dead in C++)
  r3 = f825868E8(base, r1, r3, r4, r5, r7, f1);
  // lfs f0,4(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f0 = double(temp.f32);
  // fmuls f0,f0,f26
  f0 = double(float(f0 * f26));
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82582EC0: lifted from generated/ by lift.py.
lifted::Result f82582EC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82582EC0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r28,r3
  r28 = r3;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82582ee0
  if (!cr6.eq) goto loc_82582EE0;
loc_82582ED8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82583088
  goto loc_82583088;
loc_82582EE0:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r29,-32107
  r29 = uint64_t(int64_t(-2104164352));
  // li r4,116
  r4 = uint64_t(int64_t(116));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r30,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r30));
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r31,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r31));
  // li r10,100
  r10 = uint64_t(int64_t(100));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // stw r30,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r30));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // sth r30,56(r31)
  REX_STORE_U16(uint32_t(r31) + 56, uint16_t(r30));
  // li r3,644
  r3 = uint64_t(int64_t(644));
  // sth r30,58(r31)
  REX_STORE_U16(uint32_t(r31) + 58, uint16_t(r30));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,28(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stw r30,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r30));
  // stfs f0,40(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 40, temp.u32);
  // sth r30,94(r31)
  REX_STORE_U16(uint32_t(r31) + 94, uint16_t(r30));
  // stfs f0,44(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 44, temp.u32);
  // sth r10,96(r31)
  REX_STORE_U16(uint32_t(r31) + 96, uint16_t(r10));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // stw r30,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r30));
  // lfs f12,32600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 32600);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,32(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // stfs f0,60(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // stfs f0,64(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
  // stw r30,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r30));
  // stfs f0,68(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 68, temp.u32);
  // stw r30,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r30));
  // lfs f13,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f13,36(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // stfs f13,48(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stfs f0,72(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 72, temp.u32);
  // stfs f13,80(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 80, temp.u32);
  // lfs f11,7820(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7820);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f11,76(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 76, temp.u32);
  // lfs f10,10764(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f10 = double(temp.f32);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stfs f10,108(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 108, temp.u32);
  // sth r11,92(r31)
  REX_STORE_U16(uint32_t(r31) + 92, uint16_t(r11));
  // stw r11,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r11));
  // stw r11,100(r31)
  REX_STORE_U32(uint32_t(r31) + 100, uint32_t(r11));
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // li r3,80
  r3 = uint64_t(int64_t(80));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // li r3,65
  r3 = uint64_t(int64_t(65));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // li r3,65
  r3 = uint64_t(int64_t(65));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // li r3,102
  r3 = uint64_t(int64_t(102));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // li r3,104
  r3 = uint64_t(int64_t(104));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r3));
  // beq 0x82582ed8
  if (cr0.eq) goto loc_82582ED8;
  // mr r3,r31
  r3 = r31;
  // bl 0x82587658
  // (left out: dead in C++)
  { const lifted::Result o = f82587658(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
loc_82583088:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82583090: lifted from generated/ by lift.py.
lifted::Result f82583090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82583090 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82583170
  if (cr0.eq) goto loc_82583170;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825830cc
  if (cr0.eq) goto loc_825830CC;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r29));
loc_825830CC:
  // mr r3,r31
  r3 = r31;
  // bl 0x825876d8
  // (left out: dead in C++)
  { const lifted::Result o = f825876D8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825830f0
  if (cr0.eq) goto loc_825830F0;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r29));
loc_825830F0:
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8258310c
  if (cr0.eq) goto loc_8258310C;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r29));
loc_8258310C:
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82583128
  if (cr0.eq) goto loc_82583128;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r29));
loc_82583128:
  // lwz r3,24(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82583144
  if (cr0.eq) goto loc_82583144;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r29));
loc_82583144:
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82583160
  if (cr0.eq) goto loc_82583160;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
loc_82583160:
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82583170:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825834F8: lifted from generated/ by lift.py.
void f825834F8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825834F8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // extsh r6,r4
  r6 = uint64_t(int64_t(int16_t(r4)));
  // lwz r11,20(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lhz r7,4(r3)
  r7 = REX_LOAD_U16(uint32_t(r3) + 4);
  // lhz r9,0(r10)
  r9 = REX_LOAD_U16(uint32_t(r10) + 0);
  // lbz r4,0(r11)
  r4 = REX_LOAD_U8(uint32_t(r11) + 0);
  // add r9,r9,r8
  r9 = r9 + r8;
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne 0x8258352c
  if (!cr0.eq) goto loc_8258352C;
  // stb r6,0(r9)
  REX_STORE_U8(uint32_t(r9) + 0, uint8_t(r6));
  // b 0x82583544
  goto loc_82583544;
loc_8258352C:
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsh r4,r6
  r4 = uint64_t(int64_t(int16_t(r6)));
  // lbz r31,0(r9)
  r31 = REX_LOAD_U8(uint32_t(r9) + 0);
  // slw r8,r4,r8
  r8 = uint8_t(r8) & 0x20 ? 0 : (uint32_t(r4) << (uint8_t(r8) & 0x3F));
  // or r8,r8,r31
  r8 = r8 | r31;
  // stb r8,0(r9)
  REX_STORE_U8(uint32_t(r9) + 0, uint8_t(r8));
loc_82583544:
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // add r8,r8,r5
  r8 = r8 + r5;
  // clrlwi r8,r8,24
  r8 = uint32_t(r8) & 0xFF;
  // cmplwi cr6,r8,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 8, so);
  // stb r8,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r8));
  // ble cr6,0x825835b8
  if (!cr6.gt) goto loc_825835B8;
  // lhz r8,0(r10)
  r8 = REX_LOAD_U16(uint32_t(r10) + 0);
  // clrlwi r7,r7,16
  r7 = uint32_t(r7) & 0xFFFF;
  // addi r4,r8,1
  r4 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // clrlwi r9,r4,16
  r9 = uint32_t(r4) & 0xFFFF;
  // mr r4,r9
  r4 = r9;
  // cmplw cr6,r4,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r7), so);
  // sth r9,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r9));
  // blt cr6,0x8258358c
  if (cr6.lt) goto loc_8258358C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // sth r9,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r9));
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
loc_8258358C:
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsh r9,r6
  r9 = uint64_t(int64_t(int16_t(r6)));
  // subf r10,r10,r5
  r10 = r5 - r10;
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // sraw r10,r9,r10
  temp.u32 = uint32_t(r10) & 0x3F;
  if (temp.u32 > 0x1F) temp.u32 = 0x1F;
  ca = (int32_t(r9) < 0) & (((int32_t(r9) >> temp.u32) << temp.u32) != int32_t(r9));
  r10 = uint64_t(int64_t(int32_t(r9) >> temp.u32));
  // stb r10,0(r8)
  REX_STORE_U8(uint32_t(r8) + 0, uint8_t(r10));
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r10,r10,248
  r10 = uint64_t(int64_t(int64_t(r10) + 248));
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // b 0x825835f0
  goto loc_825835F0;
loc_825835B8:
  // clrlwi r9,r8,24
  r9 = uint32_t(r8) & 0xFF;
  // cmplwi cr6,r9,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 8, so);
  // bne cr6,0x825835f0
  if (!cr6.eq) goto loc_825835F0;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // clrlwi r8,r7,16
  r8 = uint32_t(r7) & 0xFFFF;
  // stb r9,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r9));
  // lhz r11,0(r10)
  r11 = REX_LOAD_U16(uint32_t(r10) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // mr r7,r11
  r7 = r11;
  // cmplw cr6,r7,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r8), so);
  // sth r11,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r11));
  // blt cr6,0x825835f0
  if (cr6.lt) goto loc_825835F0;
  // sth r9,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r9));
loc_825835F0:
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_825835F8: lifted from generated/ by lift.py.
double f825835F8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825835F8 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // cmpw cr6,r5,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r4), so);
  // blt cr6,0x82583604
  if (cr6.lt) goto loc_82583604;
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82583604:
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lfsx f1,r11,r3
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82583610: lifted from generated/ by lift.py.
uint64_t f82583610(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82583610 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r9,r3
  r9 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lfs f0,-28920(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -28920);
  f0 = double(temp.f32);
  // stfs f0,-16(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -16, temp.u32);
  // blelr cr6
  if (!cr6.gt) return r3;
  // lfs f12,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f12 = double(temp.f32);
  // lfs f11,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f11 = double(temp.f32);
  // lwz r9,-16(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + -16);
loc_8258363C:
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // addi r11,r4,4
  r11 = uint64_t(int64_t(int64_t(r4) + 4));
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // addi r4,r11,4
  r4 = uint64_t(int64_t(int64_t(r11) + 4));
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // fsubs f0,f10,f11
  f0 = double(float(f10 - f11));
  // fmadds f0,f0,f0,f13
  f0 = double(float(std::fma(f0, f0, f13)));
  // stfs f0,-16(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -16, temp.u32);
  // lwz r11,-16(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + -16);
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bge cr6,0x82583674
  if (!cr6.lt) goto loc_82583674;
  // mr r9,r11
  r9 = r11;
  // mr r3,r10
  r3 = r10;
loc_82583674:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r10,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r5), so);
  // blt cr6,0x8258363c
  if (cr6.lt) goto loc_8258363C;
  // blr
  return r3;
}

// sub_82583BA8: lifted from generated/ by lift.py.
uint64_t f82583BA8(uint8_t* base, double f1) {
  ITC_NATIVE_RUNNING("sub_82583BA8 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r11,r11,-28912
  r11 = uint64_t(int64_t(int64_t(r11) + -28912));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r11,4
  r10 = uint64_t(int64_t(int64_t(r11) + 4));
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f1,f0
  f0 = double(float(f1 - f0));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
loc_82583BC8:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f1,f0
  f0 = double(float(f1 - f0));
  // fmuls f0,f0,f0
  f0 = double(float(f0 * f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82583be4
  if (!cr6.lt) goto loc_82583BE4;
  // fmr f13,f0
  f13 = f0;
  // mr r3,r9
  r3 = r9;
loc_82583BE4:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r8,r11,256
  r8 = uint64_t(int64_t(int64_t(r11) + 256));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpw cr6,r10,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r8), so);
  // blt cr6,0x82583bc8
  if (cr6.lt) goto loc_82583BC8;
  // blr
  return r3;
}

// sub_82583C98: lifted from generated/ by lift.py.
double f82583C98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82583C98 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // extsw r10,r3
  r10 = uint64_t(int64_t(int32_t(r3)));
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfs f0,31064(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 31064);
  f0 = double(temp.f32);
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f12,f13
  f12 = double(float(f13));
  // lfs f13,31060(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 31060);
  f13 = double(temp.f32);
  // fnmsubs f1,f12,f0,f13
  f1 = double(float(-std::fma(f12, f0, -f13)));
  // bl 0x825abe30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825ABE30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_82583CE8: lifted from generated/ by lift.py.
uint64_t f82583CE8(uint8_t* base, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_82583CE8 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f13,f3,f3
  lifted::ScalarFloatMode();
  f13 = double(float(f3 * f3));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r11,r11,-28640
  r11 = uint64_t(int64_t(int64_t(r11) + -28640));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r11,4
  r10 = uint64_t(int64_t(int64_t(r11) + 4));
loc_82583D04:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f3,f0
  f0 = double(float(f3 - f0));
  // fmuls f0,f0,f0
  f0 = double(float(f0 * f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82583d20
  if (!cr6.lt) goto loc_82583D20;
  // fmr f13,f0
  f13 = f0;
  // mr r8,r9
  r8 = r9;
loc_82583D20:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r6,r11,32
  r6 = uint64_t(int64_t(int64_t(r11) + 32));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpw cr6,r10,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r6), so);
  // blt cr6,0x82583d04
  if (cr6.lt) goto loc_82583D04;
  // rlwinm r10,r8,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // fsubs f0,f1,f2
  lifted::ScalarFloatMode();
  f0 = double(float(f1 - f2));
  // lfsx f13,r10,r11
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  f13 = double(temp.f32);
  // fsubs f13,f13,f2
  f13 = double(float(f13 - f2));
  // fmuls f0,f0,f0
  f0 = double(float(f0 * f0));
  // fmuls f13,f13,f13
  f13 = double(float(f13 * f13));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x82583d58
  if (!cr6.lt) goto loc_82583D58;
  // li r7,1
  r7 = uint64_t(int64_t(1));
loc_82583D58:
  // rlwinm r11,r7,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 3) & 0xFFFFFFF8;
  // add r3,r11,r8
  r3 = r11 + r8;
  // blr
  return r3;
}

// sub_82583D68: lifted from generated/ by lift.py.
lifted::Result f82583D68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_82583D68 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // fmr f31,f1
  f31 = f1;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // rlwinm r31,r11,0,28,28
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // mr r29,r5
  r29 = r5;
  // addi r3,r10,-28640
  r3 = uint64_t(int64_t(int64_t(r10) + -28640));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // subf r5,r31,r11
  r5 = r11 - r31;
  // mr r30,r6
  r30 = r6;
  // bl 0x825835f8
  // (left out: dead in C++)
  f1 = f825835F8(base, r3, r4, r5);
  // stfs f1,0(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne cr6,0x82583db4
  if (!cr6.eq) goto loc_82583DB4;
  // stfs f31,0(r29)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // b 0x82583db8
  goto loc_82583DB8;
loc_82583DB4:
  // stfs f1,0(r29)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
loc_82583DB8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82583FD0: lifted from generated/ by lift.py.
lifted::Result f82583FD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82583FD0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r30,r3
  r30 = r3;
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,30560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // stw r31,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r31));
  // bne 0x82584014
  if (!cr0.eq) goto loc_82584014;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82584068
  goto loc_82584068;
loc_82584014:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // stw r11,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r11));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // lwz r11,30568(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30568);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30568(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30568);
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82584068:
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

// sub_82584080: lifted from generated/ by lift.py.
lifted::Result f82584080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82584080 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825840b8
  if (cr0.eq) goto loc_825840B8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_825840B8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_825840D0: lifted from generated/ by lift.py.
lifted::Result f825840D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825840D0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825840f0
  if (!cr6.eq) goto loc_825840F0;
loc_825840E8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82584170
  goto loc_82584170;
loc_825840F0:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // lwz r11,30560(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x825840e8
  if (cr0.eq) goto loc_825840E8;
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // lwz r11,30560(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30560);
  // li r3,13
  r3 = uint64_t(int64_t(13));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r3));
  // beq 0x825840e8
  if (cr0.eq) goto loc_825840E8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r5,52
  r5 = uint64_t(int64_t(52));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lfs f0,7132(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7132);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // lfs f13,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // stfs f13,4(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // lwz r11,30568(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30568);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82584170:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584178: lifted from generated/ by lift.py.
lifted::Result f82584178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82584178 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x825841cc
  if (cr0.eq) goto loc_825841CC;
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lis r30,-32107
  r30 = uint64_t(int64_t(-2104164352));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825841bc
  if (cr0.eq) goto loc_825841BC;
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
loc_825841BC:
  // lwz r11,30564(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30564);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825841CC:
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

// sub_825841E8: lifted from generated/ by lift.py.
uint64_t f825841E8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825841E8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addic. r11,r4,-1
  ca = uint32_t(r4) > 0;
  r11 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r9,r11
  r9 = r11;
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // fmr f12,f0
  f12 = f0;
  // blt 0x8258421c
  if (cr0.lt) goto loc_8258421C;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r3
  r10 = r10 + r3;
loc_82584208:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // fadds f12,f13,f12
  f12 = double(float(f13 + f12));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bge 0x82584208
  if (!cr0.lt) goto loc_82584208;
loc_8258421C:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lfs f13,8708(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8708);
  f13 = double(temp.f32);
  // mr r10,r11
  r10 = r11;
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // blt cr6,0x82584258
  if (cr6.lt) goto loc_82584258;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r3
  r11 = r11 + r3;
loc_8258423C:
  // lfs f12,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82584258
  if (!cr6.lt) goto loc_82584258;
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bge 0x8258423c
  if (!cr0.lt) goto loc_8258423C;
loc_82584258:
  // addi r3,r10,2
  r3 = uint64_t(int64_t(int64_t(r10) + 2));
  // cmpwi cr6,r3,76
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 76, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r3,76
  r3 = uint64_t(int64_t(76));
  // blr
  return r3;
}

// sub_82584758: lifted from generated/ by lift.py.
lifted::Result f82584758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82584758 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-768(r1)
  ea = -768 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r5
  r27 = r5;
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r5,92
  r5 = uint64_t(int64_t(92));
  // mr r29,r3
  r29 = r3;
  // mr r28,r4
  r28 = r4;
  // bl 0x82584e78
  // (left out: dead in C++)
  r3 = f82584E78(base, r3, r4, r5, r6);
  // addi r6,r1,100
  r6 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r4,92
  r4 = uint64_t(int64_t(92));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x82584ed8
  // (left out: dead in C++)
  f82584ED8(base, r1, r3, r4, r5, r6);
  // li r4,86
  r4 = uint64_t(int64_t(86));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // bl 0x825841e8
  // (left out: dead in C++)
  r3 = f825841E8(base, r3, r4);
  // mr r30,r3
  r30 = r3;
  // addi r6,r1,120
  r6 = uint64_t(int64_t(int64_t(r1) + 120));
  // lfs f1,100(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f1 = double(temp.f32);
  // addi r5,r1,528
  r5 = uint64_t(int64_t(int64_t(r1) + 528));
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r4,r30
  r4 = r30;
  // bl 0x825845a8
  // (left out: dead in C++)
  { const lifted::Result o = f825845A8(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,100(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // lfs f13,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x825847dc
  if (!cr6.gt) goto loc_825847DC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
loc_825847DC:
  // addic. r11,r30,-1
  ca = uint32_t(r30) > 0;
  r11 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // blt 0x8258481c
  if (cr0.lt) goto loc_8258481C;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,160
  r9 = uint64_t(int64_t(int64_t(r1) + 160));
  // add r10,r10,r9
  r10 = r10 + r9;
loc_825847F4:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fmuls f13,f0,f13
  f13 = double(float(f0 * f13));
  // fsqrts f13,f13
  f13 = double(float(sqrt(f13)));
  // fsqrts f13,f13
  f13 = double(float(sqrt(f13)));
  // fsqrts f13,f13
  f13 = double(float(sqrt(f13)));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bge 0x825847f4
  if (!cr0.lt) goto loc_825847F4;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
loc_8258481C:
  // addi r11,r1,116
  r11 = uint64_t(int64_t(int64_t(r1) + 116));
  // lfs f1,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f1 = double(temp.f32);
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r6,120(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 120);
  // addi r7,r1,128
  r7 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfs f2,4(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 4);
  f2 = double(temp.f32);
  // addi r5,r1,528
  r5 = uint64_t(int64_t(int64_t(r1) + 528));
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82587bc8
  // (left out: dead in C++)
  { const lifted::Result o = f82587BC8(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r30
  r3 = r30;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lfs f1,-4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f1 = double(temp.f32);
  // bl 0x82587918
  // (left out: dead in C++)
  r3 = f82587918(base, r1, r3, f1);
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // mr r30,r3
  r30 = r3;
  // mr. r11,r31
  r11 = r31;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // b 0x825848a4
  goto loc_825848A4;
loc_82584874:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r6,r30
  r6 = r30;
  // mr r5,r28
  r5 = r28;
  // mr r4,r29
  r4 = r29;
  // lfsx f1,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f1 = double(temp.f32);
  // bl 0x82587728
  // (left out: dead in C++)
  { const lifted::Result o = f82587728(base, r1, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,96(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 96);
  // addi r9,r1,104
  r9 = uint64_t(int64_t(int64_t(r1) + 104));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stfsx f1,r10,r9
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r9), temp.u32);
loc_825848A4:
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bge 0x82584874
  if (!cr0.lt) goto loc_82584874;
  // lwz r11,116(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 116);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpw cr6,r11,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r31), so);
  // bge cr6,0x8258491c
  if (!cr6.lt) goto loc_8258491C;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,-28196
  r10 = uint64_t(int64_t(int64_t(r10) + -28196));
  // beq cr6,0x825848ec
  if (cr6.eq) goto loc_825848EC;
  // lfs f13,104(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f13 = double(temp.f32);
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // addi r7,r1,104
  r7 = uint64_t(int64_t(int64_t(r1) + 104));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfsx f13,r8,r7
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r7));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82584948
  if (cr6.lt) goto loc_82584948;
loc_825848EC:
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f0 = double(temp.f32);
  // addi r8,r1,104
  r8 = uint64_t(int64_t(int64_t(r1) + 104));
  // rlwinm r7,r31,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r6,r1,104
  r6 = uint64_t(int64_t(int64_t(r1) + 104));
  // lfsx f13,r9,r8
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r8));
  f13 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfsx f13,r7,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r6));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x82584944
  if (!cr6.lt) goto loc_82584944;
  // mr r9,r11
  r9 = r11;
  // b 0x82584948
  goto loc_82584948;
loc_8258491C:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,104(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f13 = double(temp.f32);
  // addi r10,r1,104
  r10 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r11,r11,-28196
  r11 = uint64_t(int64_t(int64_t(r11) + -28196));
  // lfs f0,8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8);
  f0 = double(temp.f32);
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfsx f13,r11,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82584948
  if (cr6.lt) goto loc_82584948;
loc_82584944:
  // mr r9,r31
  r9 = r31;
loc_82584948:
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfsx f1,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f1 = double(temp.f32);
  // lfsx f0,r11,r9
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  f0 = double(temp.f32);
  // stfs f0,4(r27)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r27) + 4, temp.u32);
  // stfs f1,0(r27)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // addi r1,r1,768
  r1 = uint64_t(int64_t(int64_t(r1) + 768));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584970: lifted from generated/ by lift.py.
void f82584970(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82584970 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,74
  r10 = uint64_t(int64_t(74));
  // addi r11,r11,-28544
  r11 = uint64_t(int64_t(int64_t(r11) + -28544));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // stw r10,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r10));
  // blr
  return;
}

// sub_82584988: lifted from generated/ by lift.py.
lifted::Result f82584988(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82584988 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
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
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r31,r3
  r31 = r3;
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,30560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // bne 0x825849c8
  if (!cr0.eq) goto loc_825849C8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825849e8
  goto loc_825849E8;
loc_825849C8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10764(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,0(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,4(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
loc_825849E8:
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

// sub_82584A00: lifted from generated/ by lift.py.
lifted::Result f82584A00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82584A00 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r4,1492
  r4 = uint64_t(int64_t(1492));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r11,30560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82584a44
  if (!cr0.eq) goto loc_82584A44;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x82584a58
  goto loc_82584A58;
loc_82584A44:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
loc_82584A58:
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

// sub_82584A70: lifted from generated/ by lift.py.
lifted::Result f82584A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82584A70 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // rotlwi r11,r4,1
  r11 = __builtin_rotateleft32(uint32_t(r4), 1);
  // mr r30,r5
  r30 = r5;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r27,r6
  r27 = r6;
  // lwz r31,0(r29)
  r31 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r26,r29,8
  r26 = uint64_t(int64_t(int64_t(r29) + 8));
  // lwz r23,4(r29)
  r23 = REX_LOAD_U32(uint32_t(r29) + 4);
  // divw r10,r4,r31
  r10 = uint32_t((int32_t(r31) && !(int32_t(r4) == INT32_MIN && int32_t(r31) == -1)) ? int32_t(r4) / int32_t(r31) : 0);
  // andc r9,r31,r11
  r9 = r31 & ~r11;
  // mullw r11,r10,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r31))));
  // subf r11,r11,r4
  r11 = r4 - r11;
  // twlgei r9,-1
  if (int32_t(r9) == -1 || uint32_t(r9) > 4294967295u) lifted::Trap(0);
  // add r11,r11,r23
  r11 = r11 + r23;
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // add r11,r11,r31
  r11 = r11 + r31;
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // divw r9,r11,r31
  r9 = uint32_t((int32_t(r31) && !(int32_t(r11) == INT32_MIN && int32_t(r31) == -1)) ? int32_t(r11) / int32_t(r31) : 0);
  // rotlwi r10,r11,1
  r10 = __builtin_rotateleft32(uint32_t(r11), 1);
  // mullw r9,r9,r31
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r31))));
  // subf r11,r9,r11
  r11 = r11 - r9;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // subf r28,r11,r31
  r28 = r31 - r11;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // andc r10,r31,r10
  r10 = r31 & ~r10;
  // cmpw cr6,r28,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r30), so);
  // mr r4,r27
  r4 = r27;
  // twlgei r10,-1
  if (int32_t(r10) == -1 || uint32_t(r10) > 4294967295u) lifted::Trap(0);
  // add r3,r11,r26
  r3 = r11 + r26;
  // blt cr6,0x82584b04
  if (cr6.lt) goto loc_82584B04;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // rlwinm r5,r30,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,30572(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 30572);
  // b 0x82584b30
  goto loc_82584B30;
loc_82584B04:
  // lis r24,-32107
  r24 = uint64_t(int64_t(-2104164352));
  // rlwinm r25,r28,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r25
  r5 = r25;
  // lwz r11,30572(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 30572);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // subf r11,r28,r30
  r11 = r30 - r28;
  // add r4,r25,r27
  r4 = r25 + r27;
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,30572(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 30572);
  // mr r3,r26
  r3 = r26;
loc_82584B30:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // divw r11,r30,r31
  r11 = uint32_t((int32_t(r31) && !(int32_t(r30) == INT32_MIN && int32_t(r31) == -1)) ? int32_t(r30) / int32_t(r31) : 0);
  // rotlwi r10,r30,1
  r10 = __builtin_rotateleft32(uint32_t(r30), 1);
  // mullw r11,r11,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r31))));
  // subf r11,r11,r30
  r11 = r30 - r11;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // add r11,r11,r23
  r11 = r11 + r23;
  // andc r10,r31,r10
  r10 = r31 & ~r10;
  // add r9,r11,r31
  r9 = r11 + r31;
  // twlgei r10,-1
  if (int32_t(r10) == -1 || uint32_t(r10) > 4294967295u) lifted::Trap(0);
  // divw r10,r9,r31
  r10 = uint32_t((int32_t(r31) && !(int32_t(r9) == INT32_MIN && int32_t(r31) == -1)) ? int32_t(r9) / int32_t(r31) : 0);
  // rotlwi r11,r9,1
  r11 = __builtin_rotateleft32(uint32_t(r9), 1);
  // mullw r10,r10,r31
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r31))));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // subf r10,r10,r9
  r10 = r9 - r10;
  // andc r11,r31,r11
  r11 = r31 & ~r11;
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
  // stw r10,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r10));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584B90: lifted from generated/ by lift.py.
lifted::Result f82584B90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82584B90 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // mr r31,r5
  r31 = r5;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r29,r6
  r29 = r6;
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // addi r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 1));
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // srawi r10,r10,1
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x1) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 1));
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // add r10,r10,r4
  r10 = r10 + r4;
  // addi r28,r3,8
  r28 = uint64_t(int64_t(int64_t(r3) + 8));
  // neg r8,r10
  r8 = uint64_t(int64_t(static_cast<int64_t>(-r10)));
  // mr r3,r29
  r3 = r29;
  // rotlwi r10,r8,1
  r10 = __builtin_rotateleft32(uint32_t(r8), 1);
  // divw r7,r8,r11
  r7 = uint32_t((int32_t(r11) && !(int32_t(r8) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r8) / int32_t(r11) : 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // andc r6,r11,r10
  r6 = r11 & ~r10;
  // mullw r10,r7,r11
  r10 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r11))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // twlgei r6,-1
  if (int32_t(r6) == -1 || uint32_t(r6) > 4294967295u) lifted::Trap(0);
  // add r10,r10,r9
  r10 = r10 + r9;
  // add r10,r10,r11
  r10 = r10 + r11;
  // divw r8,r10,r11
  r8 = uint32_t((int32_t(r11) && !(int32_t(r10) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r10) / int32_t(r11) : 0);
  // rotlwi r9,r10,1
  r9 = __builtin_rotateleft32(uint32_t(r10), 1);
  // mullw r8,r8,r11
  r8 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r11))));
  // subf r10,r8,r10
  r10 = r10 - r8;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // subf r30,r10,r11
  r30 = r11 - r10;
  // andc r9,r11,r9
  r9 = r11 & ~r9;
  // rlwinm r11,r10,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // cmpw cr6,r30,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r31), so);
  // twlgei r9,-1
  if (int32_t(r9) == -1 || uint32_t(r9) > 4294967295u) lifted::Trap(0);
  // add r4,r11,r28
  r4 = r11 + r28;
  // blt cr6,0x82584c30
  if (cr6.lt) goto loc_82584C30;
  // lis r9,-32107
  r9 = uint64_t(int64_t(-2104164352));
  // rlwinm r5,r31,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,30572(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 30572);
  // b 0x82584c5c
  goto loc_82584C5C;
loc_82584C30:
  // lis r26,-32107
  r26 = uint64_t(int64_t(-2104164352));
  // rlwinm r27,r30,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r27
  r5 = r27;
  // lwz r11,30572(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 30572);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // subf r11,r30,r31
  r11 = r31 - r30;
  // mr r4,r28
  r4 = r28;
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,30572(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 30572);
  // add r3,r27,r29
  r3 = r27 + r29;
loc_82584C5C:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584C70: lifted from generated/ by lift.py.
lifted::Result f82584C70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82584C70 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  uint8_t ca = 0;
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
  // srawi r11,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 1));
  // mr r31,r3
  r31 = r3;
  // addze r29,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r29 = uint64_t(int64_t(temp.s64));
  // mr r30,r4
  r30 = r4;
  // mr r5,r29
  r5 = r29;
  // bl 0x825881e0
  // (left out: dead in C++)
  f1 = f825881E0(base, r1, r3, r4, r5, f1);
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82587f98
  // (left out: dead in C++)
  r3 = f82587F98(base, r3, r4, r5);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584CB0: lifted from generated/ by lift.py.
lifted::Result f82584CB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82584CB0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  uint8_t ca = 0;
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
  // srawi r11,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 1));
  // mr r31,r3
  r31 = r3;
  // addze r29,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r29 = uint64_t(int64_t(temp.s64));
  // mr r30,r4
  r30 = r4;
  // mr r5,r29
  r5 = r29;
  // bl 0x825880b8
  // (left out: dead in C++)
  f825880B8(base, r1, r3, r4, r5);
  // mr r5,r29
  r5 = r29;
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x825881e0
  // (left out: dead in C++)
  f1 = f825881E0(base, r1, r3, r4, r5, f1);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584CF0: lifted from generated/ by lift.py.
lifted::Result f82584CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82584CF0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // mr r9,r3
  r9 = r3;
  // srawi r3,r4,1
  ca = (int32_t(r4) < 0) & ((uint32_t(r4) & 0x1) != 0);
  r3 = uint64_t(int64_t(int32_t(r4) >> 1));
  // srawi. r11,r4,2
  ca = (int32_t(r4) < 0) & ((uint32_t(r4) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r4) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r30,r6
  r30 = r6;
  // subfic r6,r11,256
  ca = uint32_t(r11) <= 256;
  r6 = static_cast<uint64_t>(256) - r11;
  // mr r29,r7
  r29 = r7;
  // slw r5,r10,r5
  r5 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r5) & 0x3F));
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // add r8,r10,r30
  r8 = r10 + r30;
  // add r10,r10,r29
  r10 = r10 + r29;
  // ble 0x82584d58
  if (!cr0.gt) goto loc_82584D58;
  // mr r7,r11
  r7 = r11;
loc_82584D30:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bne 0x82584d30
  if (!cr0.eq) goto loc_82584D30;
loc_82584D58:
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // mr r10,r30
  r10 = r30;
  // subf r8,r11,r4
  r8 = r4 - r11;
  // mr r11,r29
  r11 = r29;
  // srawi. r7,r8,1
  ca = (int32_t(r8) < 0) & ((uint32_t(r8) & 0x1) != 0);
  r7 = uint64_t(int64_t(int32_t(r8) >> 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ble 0x82584da0
  if (!cr0.gt) goto loc_82584DA0;
  // mr r8,r7
  r8 = r7;
loc_82584D78:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82584d78
  if (!cr0.eq) goto loc_82584D78;
loc_82584DA0:
  // rlwinm r7,r3,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // cmpw cr6,r4,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r4), int32_t(r7), so);
  // beq cr6,0x82584dcc
  if (cr6.eq) goto loc_82584DCC;
  // lfs f13,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f0,2612(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2612);
  f0 = double(temp.f32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
loc_82584DCC:
  // cmpw cr6,r8,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r6), so);
  // bge cr6,0x82584e14
  if (!cr6.lt) goto loc_82584E14;
  // subf r9,r8,r6
  r9 = r6 - r8;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm. r8,r9,30,2,31
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0x3FFFFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x82584df8
  if (cr0.eq) goto loc_82584DF8;
  // mtctr r8
  ctr = r8;
loc_82584DEC:
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x82584dec
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82584DEC;
loc_82584DF8:
  // rlwinm. r11,r9,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 30) & 0x3FFFFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // beq 0x82584e14
  if (cr0.eq) goto loc_82584E14;
  // mtctr r11
  ctr = r11;
loc_82584E08:
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x82584e08
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82584E08;
loc_82584E14:
  // srawi r11,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 1));
  // mr r4,r29
  r4 = r29;
  // addze r31,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r31 = uint64_t(int64_t(temp.s64));
  // mr r3,r30
  r3 = r30;
  // mr r5,r31
  r5 = r31;
  // bl 0x825881e0
  // (left out: dead in C++)
  f1 = f825881E0(base, r1, r3, r4, r5, f1);
  // mr r5,r31
  r5 = r31;
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x82587f98
  // (left out: dead in C++)
  r3 = f82587F98(base, r3, r4, r5);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82584E48: lifted from generated/ by lift.py.
lifted::Result f82584E48(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82584E48 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blelr cr6
  if (!cr6.gt) return {r3, f1};
loc_82584E58:
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lfs f13,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // fmadds f1,f0,f13,f1
  f1 = double(float(std::fma(f0, f13, f1)));
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // bgt 0x82584e58
  if (cr0.gt) goto loc_82584E58;
  // blr
  return {r3, f1};
}

// sub_82584E78: lifted from generated/ by lift.py.
uint64_t f82584E78(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82584E78 (lifted)");
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // blelr cr6
  if (!cr6.gt) return r3;
loc_82584E80:
  // lfs f13,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // fmuls f13,f13,f13
  f13 = double(float(f13 * f13));
  // lfs f0,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // fmadds f0,f0,f0,f13
  f0 = double(float(std::fma(f0, f0, f13)));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // bgt 0x82584e80
  if (cr0.gt) goto loc_82584E80;
  // blr
  return r3;
}

// sub_82584EB0: lifted from generated/ by lift.py.
uint64_t f82584EB0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_82584EB0 (lifted)");
  double f0 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // blelr cr6
  if (!cr6.gt) return r3;
loc_82584EB8:
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // addic. r4,r4,-1
  ca = uint32_t(r4) > 0;
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // fmuls f0,f0,f1
  f0 = double(float(f0 * f1));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // bgt 0x82584eb8
  if (cr0.gt) goto loc_82584EB8;
  // blr
  return r3;
}

// sub_82584ED8: lifted from generated/ by lift.py.
void f82584ED8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82584ED8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // mr r30,r11
  r30 = r11;
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge 0x82585000
  if (!cr0.lt) goto loc_82585000;
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // addi r7,r3,12
  r7 = uint64_t(int64_t(int64_t(r3) + 12));
loc_82584F04:
  // addi r9,r8,3
  r9 = uint64_t(int64_t(int64_t(r8) + 3));
  // cmpw cr6,r9,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r4), so);
  // bgt cr6,0x82584fb4
  if (cr6.gt) goto loc_82584FB4;
  // lwz r9,-8(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + -8);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82584f24
  if (!cr6.gt) goto loc_82584F24;
  // mr r11,r9
  r11 = r9;
  // mr r31,r10
  r31 = r10;
loc_82584F24:
  // cmpw cr6,r9,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r30), so);
  // bge cr6,0x82584f34
  if (!cr6.lt) goto loc_82584F34;
  // mr r30,r9
  r30 = r9;
  // mr r29,r10
  r29 = r10;
loc_82584F34:
  // lwz r9,-4(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + -4);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82584f48
  if (!cr6.gt) goto loc_82584F48;
  // mr r11,r9
  r11 = r9;
  // mr r31,r8
  r31 = r8;
loc_82584F48:
  // cmpw cr6,r9,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r30), so);
  // bge cr6,0x82584f58
  if (!cr6.lt) goto loc_82584F58;
  // mr r30,r9
  r30 = r9;
  // mr r29,r8
  r29 = r8;
loc_82584F58:
  // lwz r9,0(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 0);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82584f6c
  if (!cr6.gt) goto loc_82584F6C;
  // mr r11,r9
  r11 = r9;
  // addi r31,r8,1
  r31 = uint64_t(int64_t(int64_t(r8) + 1));
loc_82584F6C:
  // cmpw cr6,r9,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r30), so);
  // bge cr6,0x82584f7c
  if (!cr6.lt) goto loc_82584F7C;
  // mr r30,r9
  r30 = r9;
  // addi r29,r8,1
  r29 = uint64_t(int64_t(int64_t(r8) + 1));
loc_82584F7C:
  // lwz r9,4(r7)
  r9 = REX_LOAD_U32(uint32_t(r7) + 4);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82584f90
  if (!cr6.gt) goto loc_82584F90;
  // mr r11,r9
  r11 = r9;
  // addi r31,r8,2
  r31 = uint64_t(int64_t(int64_t(r8) + 2));
loc_82584F90:
  // cmpw cr6,r9,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r30), so);
  // bge cr6,0x82584fa0
  if (!cr6.lt) goto loc_82584FA0;
  // mr r30,r9
  r30 = r9;
  // addi r29,r8,2
  r29 = uint64_t(int64_t(int64_t(r8) + 2));
loc_82584FA0:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r7,r7,16
  r7 = uint64_t(int64_t(int64_t(r7) + 16));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82584f04
  if (cr6.lt) goto loc_82584F04;
loc_82584FB4:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x82585000
  if (!cr6.lt) goto loc_82585000;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r8,r9,r3
  r8 = r9 + r3;
loc_82584FC4:
  // cmpw cr6,r10,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r4), so);
  // bge cr6,0x82585000
  if (!cr6.lt) goto loc_82585000;
  // lwz r9,0(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82584fe0
  if (!cr6.gt) goto loc_82584FE0;
  // mr r11,r9
  r11 = r9;
  // mr r31,r10
  r31 = r10;
loc_82584FE0:
  // cmpw cr6,r9,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r30), so);
  // bge cr6,0x82584ff0
  if (!cr6.lt) goto loc_82584FF0;
  // mr r30,r9
  r30 = r9;
  // mr r29,r10
  r29 = r10;
loc_82584FF0:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82584fc4
  if (cr6.lt) goto loc_82584FC4;
loc_82585000:
  // addi r7,r10,4
  r7 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpw cr6,r7,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r4), so);
  // bgt cr6,0x8258507c
  if (cr6.gt) goto loc_8258507C;
  // addi r9,r10,2
  r9 = uint64_t(int64_t(int64_t(r10) + 2));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r8,r9,r3
  r8 = r9 + r3;
loc_82585018:
  // lwz r9,-8(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + -8);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x8258502c
  if (!cr6.gt) goto loc_8258502C;
  // mr r11,r9
  r11 = r9;
  // mr r31,r10
  r31 = r10;
loc_8258502C:
  // lwz r9,-4(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + -4);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82585040
  if (!cr6.gt) goto loc_82585040;
  // mr r11,r9
  r11 = r9;
  // addi r31,r10,1
  r31 = uint64_t(int64_t(int64_t(r10) + 1));
loc_82585040:
  // lwz r9,0(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82585054
  if (!cr6.gt) goto loc_82585054;
  // mr r11,r9
  r11 = r9;
  // addi r31,r10,2
  r31 = uint64_t(int64_t(int64_t(r10) + 2));
loc_82585054:
  // lwz r9,4(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 4);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x82585068
  if (!cr6.gt) goto loc_82585068;
  // mr r11,r9
  r11 = r9;
  // addi r31,r10,3
  r31 = uint64_t(int64_t(int64_t(r10) + 3));
loc_82585068:
  // mr r10,r7
  r10 = r7;
  // addi r8,r8,16
  r8 = uint64_t(int64_t(int64_t(r8) + 16));
  // addi r7,r10,4
  r7 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpw cr6,r7,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r4), so);
  // ble cr6,0x82585018
  if (!cr6.gt) goto loc_82585018;
loc_8258507C:
  // cmpw cr6,r10,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r4), so);
  // bge cr6,0x825850b0
  if (!cr6.lt) goto loc_825850B0;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r8,r9,r3
  r8 = r9 + r3;
loc_8258508C:
  // lwz r9,0(r8)
  r9 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // ble cr6,0x825850a0
  if (!cr6.gt) goto loc_825850A0;
  // mr r11,r9
  r11 = r9;
  // mr r31,r10
  r31 = r10;
loc_825850A0:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // cmpw cr6,r10,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r4), so);
  // blt cr6,0x8258508c
  if (cr6.lt) goto loc_8258508C;
loc_825850B0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r11));
  // bge cr6,0x825850c4
  if (!cr6.lt) goto loc_825850C4;
  // stw r30,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r30));
  // blt cr6,0x825850cc
  if (cr6.lt) goto loc_825850CC;
loc_825850C4:
  // mr r11,r31
  r11 = r31;
  // b 0x825850d0
  goto loc_825850D0;
loc_825850CC:
  // mr r11,r29
  r11 = r29;
loc_825850D0:
  // lfs f0,-48(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -48);
  f0 = double(temp.f32);
  // stw r11,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r11));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // b 0x825a6edc
  // (left out: dead in C++)
  return;
}

// sub_825850E0: lifted from generated/ by lift.py.
lifted::Result f825850E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825850E0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-40
  r12 = uint64_t(int64_t(int64_t(r1) + -40));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r31,r3
  r31 = r3;
  // extsw r9,r29
  r9 = uint64_t(int64_t(int32_t(r29)));
  // addi r11,r29,-1
  r11 = uint64_t(int64_t(int64_t(r29) + -1));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // extsw r8,r11
  r8 = uint64_t(int64_t(int32_t(r11)));
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // std r9,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r9);
  // srawi r28,r29,1
  ca = (int32_t(r29) < 0) & ((uint32_t(r29) & 0x1) != 0);
  r28 = uint64_t(int64_t(int32_t(r29) >> 1));
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,30576(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 30576);
  // std r8,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r8);
  // addi r30,r11,-4
  r30 = uint64_t(int64_t(int64_t(r11) + -4));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f29,8680(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f29 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f0,f0
  f0 = double(float(f0));
  // frsp f12,f13
  f12 = double(float(f13));
  // lfs f13,8104(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8104);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fdivs f0,f29,f0
  f0 = double(float(f29 / f0));
  // fdivs f1,f13,f12
  f1 = double(float(f13 / f12));
  // lfs f13,11404(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11404);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f28,f0,f13
  f28 = double(float(f0 * f13));
  // lfs f13,-28028(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -28028);
  f13 = double(temp.f32);
  // fmuls f31,f0,f13
  f31 = double(float(f0 * f13));
  // fmr f30,f28
  f30 = f28;
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fsubs f0,f31,f28
  lifted::ScalarFloatMode();
  f0 = double(float(f31 - f28));
  // frsp f13,f1
  f13 = double(float(f1));
  // lfs f12,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r10,r30,-4
  r10 = uint64_t(int64_t(int64_t(r30) + -4));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // cmpwi cr6,r28,4
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 4, so);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // lfs f12,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f12 = double(temp.f32);
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // fmuls f0,f13,f28
  f0 = double(float(f13 * f28));
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // fmuls f13,f13,f29
  f13 = double(float(f13 * f29));
  // fsubs f12,f31,f0
  f12 = double(float(f31 - f0));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // ble cr6,0x82585268
  if (!cr6.gt) goto loc_82585268;
  // li r8,4
  r8 = uint64_t(int64_t(4));
loc_825851E0:
  // fmsubs f12,f0,f13,f30
  lifted::ScalarFloatMode();
  f12 = double(float(std::fma(f0, f13, -f30)));
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // addi r8,r8,3
  r8 = uint64_t(int64_t(int64_t(r8) + 3));
  // addi r9,r9,3
  r9 = uint64_t(int64_t(int64_t(r9) + 3));
  // cmpw cr6,r8,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r28), so);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmsubs f30,f12,f13,f0
  f30 = double(float(std::fma(f12, f13, -f0)));
  // fmuls f0,f10,f11
  f0 = double(float(f10 * f11));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f11,f31,f30
  f11 = double(float(f31 - f30));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // fmsubs f0,f13,f30,f12
  f0 = double(float(std::fma(f13, f30, -f12)));
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fmuls f12,f12,f11
  f12 = double(float(f12 * f11));
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f12,f12,f11
  f12 = double(float(f12 * f11));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f12,f31,f0
  f12 = double(float(f31 - f0));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // blt cr6,0x825851e0
  if (cr6.lt) goto loc_825851E0;
loc_82585268:
  // cmpw cr6,r9,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r28), so);
  // bge cr6,0x825852c4
  if (!cr6.lt) goto loc_825852C4;
  // fmsubs f11,f0,f13,f30
  lifted::ScalarFloatMode();
  f11 = double(float(std::fma(f0, f13, -f30)));
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpw cr6,r9,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r28), so);
  // fsubs f12,f31,f11
  f12 = double(float(f31 - f11));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f10,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f10 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f12,f10,f12
  f12 = double(float(f10 * f12));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // bge cr6,0x825852c4
  if (!cr6.lt) goto loc_825852C4;
  // fmsubs f0,f11,f13,f0
  f0 = double(float(std::fma(f11, f13, -f0)));
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fsubs f0,f31,f0
  f0 = double(float(f31 - f0));
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f13,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,-4(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
loc_825852C4:
  // clrlwi. r10,r29,31
  r10 = uint32_t(r29) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825852dc
  if (cr0.eq) goto loc_825852DC;
  // fadds f0,f31,f28
  lifted::ScalarFloatMode();
  f0 = double(float(f31 + f28));
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
loc_825852DC:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-40
  r12 = uint64_t(int64_t(int64_t(r1) + -40));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825852F0: lifted from generated/ by lift.py.
uint64_t f825852F0(uint8_t* base, double f1) {
  ITC_NATIVE_RUNNING("sub_825852F0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32692(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -32692);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x82585308
  if (!cr6.gt) goto loc_82585308;
  // li r3,261
  r3 = uint64_t(int64_t(261));
  // blr
  return r3;
loc_82585308:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32148(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -32148);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x82585320
  if (!cr6.gt) goto loc_82585320;
  // li r3,241
  r3 = uint64_t(int64_t(241));
  // blr
  return r3;
loc_82585320:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7820(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7820);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x82585338
  if (!cr6.gt) goto loc_82585338;
  // li r3,221
  r3 = uint64_t(int64_t(221));
  // blr
  return r3;
loc_82585338:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,26548(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 26548);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x82585350
  if (!cr6.gt) goto loc_82585350;
  // li r3,201
  r3 = uint64_t(int64_t(201));
  // blr
  return r3;
loc_82585350:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,26544(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 26544);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x82585368
  if (!cr6.gt) goto loc_82585368;
  // li r3,181
  r3 = uint64_t(int64_t(181));
  // blr
  return r3;
loc_82585368:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-23268(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -23268);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x82585390
  if (cr6.gt) goto loc_82585390;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-24268(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -24268);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x82585390
  if (cr6.gt) goto loc_82585390;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8112(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8112);
  f0 = double(temp.f32);
loc_82585390:
  // li r3,161
  r3 = uint64_t(int64_t(161));
  // blr
  return r3;
}

// sub_82585398: lifted from generated/ by lift.py.
double f82585398(uint8_t* base, uint64_t r1, double f1) {
  ITC_NATIVE_RUNNING("sub_82585398 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint8_t ca = 0;
  lifted::Temp temp{};
  // stfs f1,-16(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + -16, temp.u32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f13 = double(temp.f32);
  // lwz r11,-16(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + -16);
  // clrlwi r10,r11,9
  r10 = uint32_t(r11) & 0x7FFFFF;
  // srawi r11,r11,23
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x7FFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 23));
  // oris r10,r10,16256
  r10 = r10 | 1065353216;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // addi r11,r11,-127
  r11 = uint64_t(int64_t(int64_t(r11) + -127));
  // stw r10,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r10));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // lfs f11,-16(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f11 = double(temp.f32);
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // fmsubs f0,f11,f0,f12
  f0 = double(float(std::fma(f11, f0, -f12)));
  // lfd f12,-16(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmadds f1,f0,f13,f12
  f1 = double(float(std::fma(f0, f13, f12)));
  // blr
  return f1;
}

// sub_825853F8: lifted from generated/ by lift.py.
void f825853F8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825853F8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fmuls f11,f0,f1
  f11 = double(float(f0 * f1));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // lfs f6,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f6 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f3,-28008(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -28008);
  f3 = double(temp.f32);
  // fmr f13,f6
  f13 = f6;
  // lfs f5,10428(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10428);
  f5 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f0,f11,f5
  f0 = double(float(f11 + f5));
  // lfs f12,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f4,8152(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8152);
  f4 = double(temp.f32);
  // b 0x82585440
  goto loc_82585440;
loc_82585438:
  // fmuls f0,f0,f4
  lifted::ScalarFloatMode();
  f0 = double(float(f0 * f4));
  // fmuls f13,f13,f3
  f13 = double(float(f13 * f3));
loc_82585440:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x82585438
  if (cr6.lt) goto loc_82585438;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addic. r11,r4,-1
  ca = uint32_t(r4) > 0;
  r11 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lfs f9,-28012(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -28012);
  f9 = double(temp.f32);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lfs f8,-28016(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -28016);
  f8 = double(temp.f32);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // fnmsubs f10,f0,f9,f8
  f10 = double(float(-std::fma(f0, f9, -f8)));
  // lfs f7,-28020(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -28020);
  f7 = double(temp.f32);
  // addi r10,r6,4
  r10 = uint64_t(int64_t(int64_t(r6) + 4));
  // fmadds f0,f10,f0,f7
  f0 = double(float(std::fma(f10, f0, f7)));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // blelr
  if (!cr0.gt) return;
loc_8258547C:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // fmuls f10,f0,f1
  f10 = double(float(f0 * f1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fcmpu cr6,f10,f11
  cr6 = lifted::CompareFloat(f10, f11);
  // beq cr6,0x825854c0
  if (cr6.eq) goto loc_825854C0;
  // fadds f0,f10,f5
  f0 = double(float(f10 + f5));
  // fmr f13,f6
  f13 = f6;
  // b 0x825854a8
  goto loc_825854A8;
loc_825854A0:
  // fmuls f0,f0,f4
  lifted::ScalarFloatMode();
  f0 = double(float(f0 * f4));
  // fmuls f13,f13,f3
  f13 = double(float(f13 * f3));
loc_825854A8:
  // fcmpu cr6,f0,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x825854a0
  if (cr6.lt) goto loc_825854A0;
  // fnmsubs f11,f0,f9,f8
  f11 = double(float(-std::fma(f0, f9, -f8)));
  // fmadds f0,f11,f0,f7
  f0 = double(float(std::fma(f11, f0, f7)));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // b 0x825854c4
  goto loc_825854C4;
loc_825854C0:
  // lfs f0,-4(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f0 = double(temp.f32);
loc_825854C4:
  // stfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fmr f11,f10
  f11 = f10;
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bgt cr6,0x8258547c
  if (cr6.gt) goto loc_8258547C;
  // blr
  return;
}

// sub_825854E0: lifted from generated/ by lift.py.
double f825854E0(uint8_t* base, double f1, double f2, double f3, double f4, double f5) {
  ITC_NATIVE_RUNNING("sub_825854E0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8748(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825854fc
  if (cr6.gt) goto loc_825854FC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f2,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f2 = double(temp.f32);
loc_825854FC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,11540(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11540);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // lfs f12,-28004(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -28004);
  f12 = double(temp.f32);
  // blt cr6,0x82585548
  if (cr6.lt) goto loc_82585548;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // lfs f13,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f4,f13
  f13 = double(float(f4 * f13));
  // fmuls f11,f13,f0
  f11 = double(float(f13 * f0));
  // lfs f13,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bge cr6,0x8258553c
  if (!cr6.lt) goto loc_8258553C;
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
loc_8258553C:
  // fcmpu cr6,f0,f2
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f2);
  // ble cr6,0x82585548
  if (!cr6.gt) goto loc_82585548;
  // fmr f2,f0
  f2 = f0;
loc_82585548:
  // fcmpu cr6,f3,f2
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f3, f2);
  // ble cr6,0x825855a4
  if (!cr6.gt) goto loc_825855A4;
  // fcmpu cr6,f3,f12
  cr6 = lifted::CompareFloat(f3, f12);
  // blt cr6,0x825855a4
  if (cr6.lt) goto loc_825855A4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10816(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10816);
  f0 = double(temp.f32);
  // fmuls f0,f5,f0
  f0 = double(float(f5 * f0));
  // fcmpu cr6,f4,f0
  cr6 = lifted::CompareFloat(f4, f0);
  // bge cr6,0x82585598
  if (!cr6.lt) goto loc_82585598;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,4208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f0,f5,f0
  f0 = double(float(f5 * f0));
  // fcmpu cr6,f4,f0
  cr6 = lifted::CompareFloat(f4, f0);
  // ble cr6,0x82585598
  if (!cr6.gt) goto loc_82585598;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,10604(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10604);
  f13 = double(temp.f32);
  // fmuls f13,f3,f13
  f13 = double(float(f3 * f13));
  // fmadds f2,f2,f0,f13
  f2 = double(float(std::fma(f2, f0, f13)));
loc_82585598:
  // fcmpu cr6,f2,f12
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f2, f12);
  // bge cr6,0x825855a4
  if (!cr6.lt) goto loc_825855A4;
  // fadds f2,f2,f12
  f2 = double(float(f2 + f12));
loc_825855A4:
  // fmr f1,f2
  lifted::ScalarFloatMode();
  f1 = f2;
  // blr
  return f1;
}

// sub_825858A0: lifted from generated/ by lift.py.
double f825858A0(uint8_t* base, double f1, double f2, double f3, double f4) {
  ITC_NATIVE_RUNNING("sub_825858A0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f13,f4
  lifted::ScalarFloatMode();
  f13 = f4;
  // lfs f0,10816(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10816);
  f0 = double(temp.f32);
  // fmuls f0,f1,f0
  f0 = double(float(f1 * f0));
  // fcmpu cr6,f2,f0
  cr6 = lifted::CompareFloat(f2, f0);
  // bgt cr6,0x825858e0
  if (cr6.gt) goto loc_825858E0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,4208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // fmuls f0,f1,f0
  f0 = double(float(f1 * f0));
  // fcmpu cr6,f2,f0
  cr6 = lifted::CompareFloat(f2, f0);
  // blt cr6,0x825858e0
  if (cr6.lt) goto loc_825858E0;
  // fadds f13,f1,f2
  f13 = double(float(f1 + f2));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // b 0x8258593c
  goto loc_8258593C;
loc_825858E0:
  // fcmpu cr6,f3,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f3, f13);
  // ble cr6,0x825858f4
  if (!cr6.gt) goto loc_825858F4;
  // fmr f0,f1
  f0 = f1;
  // fmr f13,f3
  f13 = f3;
  // b 0x825858f8
  goto loc_825858F8;
loc_825858F4:
  // fmr f0,f2
  lifted::ScalarFloatMode();
  f0 = f2;
loc_825858F8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,3396(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3396);
  f12 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x8258593c
  if (!cr6.lt) goto loc_8258593C;
  // fmuls f12,f13,f0
  f12 = double(float(f13 * f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // lfs f13,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f12,f13
  cr6 = lifted::CompareFloat(f12, f13);
  // bge cr6,0x8258593c
  if (!cr6.lt) goto loc_8258593C;
  // fcmpu cr6,f2,f1
  cr6 = lifted::CompareFloat(f2, f1);
  // ble cr6,0x82585938
  if (!cr6.gt) goto loc_82585938;
  // fmr f0,f2
  f0 = f2;
  // b 0x8258593c
  goto loc_8258593C;
loc_82585938:
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
loc_8258593C:
  // fmr f1,f0
  lifted::ScalarFloatMode();
  f1 = f0;
  // blr
  return f1;
}

// sub_82585D30: lifted from generated/ by lift.py.
double f82585D30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82585D30 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x82585d70
  if (cr6.eq) goto loc_82585D70;
loc_82585D50:
  // lfs f1,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f1 = double(temp.f32);
  // bl 0x825a80a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A80A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82585d50
  if (!cr0.eq) goto loc_82585D50;
loc_82585D70:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return f1;
}

// sub_82585D78: lifted from generated/ by lift.py.
uint64_t f82585D78(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_82585D78 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // lfs f9,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f9 = double(temp.f32);
  // fcmpu cr6,f13,f9
  cr6 = lifted::CompareFloat(f13, f9);
  // bne cr6,0x82585d9c
  if (!cr6.eq) goto loc_82585D9C;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,-27980(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -27980);
  f13 = double(temp.f32);
loc_82585D9C:
  // clrlwi. r11,r4,31
  r11 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82585db0
  if (cr0.eq) goto loc_82585DB0;
  // addi r11,r1,-112
  r11 = uint64_t(int64_t(int64_t(r1) + -112));
  // mr r29,r5
  r29 = r5;
  // b 0x82585db8
  goto loc_82585DB8;
loc_82585DB0:
  // mr r11,r5
  r11 = r5;
  // addi r29,r1,-112
  r29 = uint64_t(int64_t(int64_t(r1) + -112));
loc_82585DB8:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // lfs f10,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f10 = double(temp.f32);
  // stfs f10,0(r29)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // ble cr6,0x82585eb0
  if (!cr6.gt) goto loc_82585EB0;
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r3,4
  r30 = uint64_t(int64_t(int64_t(r3) + 4));
  // lfs f11,-27984(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -27984);
  f11 = double(temp.f32);
  // lfs f12,-27988(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -27988);
  f12 = double(temp.f32);
loc_82585DEC:
  // fmr f0,f9
  lifted::ScalarFloatMode();
  f0 = f9;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // blt cr6,0x82585e20
  if (cr6.lt) goto loc_82585E20;
  // mr r8,r11
  r8 = r11;
  // mr r5,r30
  r5 = r30;
  // addi r10,r9,1
  r10 = uint64_t(int64_t(int64_t(r9) + 1));
loc_82585E04:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f7,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f7 = double(temp.f32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // fmadds f0,f8,f7,f0
  f0 = double(float(std::fma(f8, f7, f0)));
  // addi r5,r5,-4
  r5 = uint64_t(int64_t(int64_t(r5) + -4));
  // bne 0x82585e04
  if (!cr0.eq) goto loc_82585E04;
loc_82585E20:
  // fdivs f0,f0,f13
  lifted::ScalarFloatMode();
  f0 = double(float(f0 / f13));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // ble cr6,0x82585e30
  if (!cr6.gt) goto loc_82585E30;
  // fmr f0,f12
  f0 = f12;
loc_82585E30:
  // fcmpu cr6,f0,f11
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f11);
  // bge cr6,0x82585e3c
  if (!cr6.lt) goto loc_82585E3C;
  // fmr f0,f11
  f0 = f11;
loc_82585E3C:
  // mr r10,r11
  r10 = r11;
  // stfs f0,0(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // mr r11,r29
  r11 = r29;
  // fneg f8,f0
  f8 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // add r8,r31,r11
  r8 = r31 + r11;
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // mr r29,r10
  r29 = r10;
  // stfs f8,4(r8)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // blt cr6,0x82585e94
  if (cr6.lt) goto loc_82585E94;
  // add r5,r31,r10
  r5 = r31 + r10;
  // subf r3,r11,r10
  r3 = r10 - r11;
  // addi r8,r11,4
  r8 = uint64_t(int64_t(int64_t(r11) + 4));
  // mr r10,r9
  r10 = r9;
loc_82585E74:
  // lfs f8,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f8 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfsx f7,r3,r8
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r8));
  f7 = double(temp.f32);
  // addi r5,r5,-4
  r5 = uint64_t(int64_t(int64_t(r5) + -4));
  // fnmsubs f8,f8,f0,f7
  f8 = double(float(-std::fma(f8, f0, -f7)));
  // stfs f8,0(r8)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bne 0x82585e74
  if (!cr0.eq) goto loc_82585E74;
loc_82585E94:
  // fnmsubs f0,f0,f0,f10
  lifted::ScalarFloatMode();
  f0 = double(float(-std::fma(f0, f0, -f10)));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpw cr6,r9,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r4), so);
  // fmuls f13,f0,f13
  f13 = double(float(f0 * f13));
  // blt cr6,0x82585dec
  if (cr6.lt) goto loc_82585DEC;
loc_82585EB0:
  // stfs f13,0(r7)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82586218: lifted from generated/ by lift.py.
void f82586218(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82586218 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // addi r11,r4,1
  r11 = uint64_t(int64_t(int64_t(r4) + 1));
  // addi r5,r5,-1
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpw cr6,r11,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r5), so);
  // bge cr6,0x82586280
  if (!cr6.lt) goto loc_82586280;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r3
  r10 = r10 + r3;
loc_82586234:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // lfs f13,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x82586270
  if (cr6.lt) goto loc_82586270;
  // lfs f13,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82586270
  if (!cr6.gt) goto loc_82586270;
  // mr r4,r11
  r4 = r11;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // stw r4,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r4));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // beq cr6,0x82586280
  if (cr6.eq) goto loc_82586280;
loc_82586270:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpw cr6,r11,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r5), so);
  // blt cr6,0x82586234
  if (cr6.lt) goto loc_82586234;
loc_82586280:
  // stw r9,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r9));
  // blr
  return;
}

// sub_82586288: lifted from generated/ by lift.py.
double f82586288(uint8_t* base, uint64_t r6, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_82586288 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // fadds f13,f1,f3
  lifted::ScalarFloatMode();
  f13 = double(float(f1 + f3));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fsubs f12,f3,f1
  f12 = double(float(f3 - f1));
  // lfs f0,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmsubs f13,f13,f0,f2
  f13 = double(float(std::fma(f13, f0, -f2)));
  // lfs f1,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // fcmpu cr6,f13,f1
  cr6 = lifted::CompareFloat(f13, f1);
  // beq cr6,0x825862c4
  if (cr6.eq) goto loc_825862C4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f12 = double(temp.f32);
  // fmuls f12,f13,f12
  f12 = double(float(f13 * f12));
  // fdivs f12,f0,f12
  f12 = double(float(f0 / f12));
  // fneg f1,f12
  f1 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) ^ 0x8000000000000000));
loc_825862C4:
  // fmadds f0,f1,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(std::fma(f1, f13, f0)));
  // fmadds f0,f0,f1,f2
  f0 = double(float(std::fma(f0, f1, f2)));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // blr
  return f1;
}

// sub_825862D8: lifted from generated/ by lift.py.
uint64_t f825862D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825862D8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-544(r1)
  ea = -544 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r5
  r30 = r5;
  // mr r31,r6
  r31 = r6;
  // mr r6,r30
  r6 = r30;
  // li r5,256
  r5 = uint64_t(int64_t(256));
  // mr r28,r7
  r28 = r7;
  // mr r27,r8
  r27 = r8;
  // mr r29,r9
  r29 = r9;
  // bl 0x82584e78
  // (left out: dead in C++)
  r3 = f82584E78(base, r3, r4, r5, r6);
  // mr r8,r27
  r8 = r27;
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r6,r29
  r6 = r29;
  // li r5,256
  r5 = uint64_t(int64_t(256));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x82586218
  // (left out: dead in C++)
  f82586218(base, r3, r4, r5, r6, r7, r8);
  // lwz r4,0(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmpwi r4,0
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // ble 0x825863d0
  if (!cr0.gt) goto loc_825863D0;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // subf r7,r31,r10
  r7 = r10 - r31;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // rotlwi r8,r4,0
  r8 = __builtin_rotateleft32(uint32_t(r4), 0);
  // lfs f8,8680(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 8680);
  f8 = double(temp.f32);
  // mr r11,r31
  r11 = r31;
  // subf r5,r31,r28
  r5 = r28 - r31;
  // lfs f9,2612(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2612);
  f9 = double(temp.f32);
  // lfs f10,7140(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7140);
  f10 = double(temp.f32);
loc_82586358:
  // lwzx r9,r7,r11
  r9 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r11));
  // rlwinm r10,r9,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r30
  r10 = r10 + r30;
  // lfs f0,4(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f0 = double(temp.f32);
  // lfs f13,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f13 = double(temp.f32);
  // fadds f12,f13,f0
  f12 = double(float(f13 + f0));
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // fsubs f13,f0,f13
  f13 = double(float(f0 - f13));
  // fmsubs f0,f12,f10,f11
  f0 = double(float(std::fma(f12, f10, -f11)));
  // fmuls f12,f13,f10
  f12 = double(float(f13 * f10));
  // fcmpu cr6,f0,f9
  cr6 = lifted::CompareFloat(f0, f9);
  // beq cr6,0x82586398
  if (cr6.eq) goto loc_82586398;
  // fmuls f13,f0,f8
  f13 = double(float(f0 * f8));
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // b 0x8258639c
  goto loc_8258639C;
loc_82586398:
  // fmr f13,f9
  lifted::ScalarFloatMode();
  f13 = f9;
loc_8258639C:
  // extsw r10,r9
  r10 = uint64_t(int64_t(int32_t(r9)));
  // fmadds f0,f13,f0,f12
  lifted::ScalarFloatMode();
  f0 = double(float(std::fma(f13, f0, f12)));
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // fmadds f0,f0,f13,f11
  f0 = double(float(std::fma(f0, f13, f11)));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfsx f0,r5,r11
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + uint32_t(r11), temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82586358
  if (!cr0.eq) goto loc_82586358;
loc_825863D0:
  // cmpwi cr6,r4,3
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 3, so);
  // bgt cr6,0x8258641c
  if (cr6.gt) goto loc_8258641C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r9,r30,28
  r9 = uint64_t(int64_t(int64_t(r30) + 28));
  // subf r8,r31,r28
  r8 = r28 - r31;
  // li r10,36
  r10 = uint64_t(int64_t(36));
  // lfs f13,13744(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 13744);
  f13 = double(temp.f32);
  // mr r11,r31
  r11 = r31;
  // fmr f0,f13
  f0 = f13;
loc_825863F4:
  // lfs f12,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f12 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfsx f0,r11,r8
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), temp.u32);
  // addi r9,r9,28
  r9 = uint64_t(int64_t(int64_t(r9) + 28));
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x825863f4
  if (!cr0.eq) goto loc_825863F4;
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_8258641C:
  // addi r1,r1,544
  r1 = uint64_t(int64_t(int64_t(r1) + 544));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_82586428: lifted from generated/ by lift.py.
uint64_t f82586428(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82586428 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // bltlr cr6
  if (cr6.lt) return r3;
  // addi r11,r5,1
  r11 = uint64_t(int64_t(int64_t(r5) + 1));
loc_8258643C:
  // lfs f13,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r4)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // fmuls f0,f0,f1
  f0 = double(float(f0 * f1));
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // bne 0x8258643c
  if (!cr0.eq) goto loc_8258643C;
  // blr
  return r3;
}

// sub_82586460: lifted from generated/ by lift.py.
uint64_t f82586460(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82586460 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpw cr6,r5,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r10), so);
  // mr r31,r10
  r31 = r10;
  // bgt cr6,0x8258647c
  if (cr6.gt) goto loc_8258647C;
  // mr r31,r5
  r31 = r5;
loc_8258647C:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ble cr6,0x825864b4
  if (!cr6.gt) goto loc_825864B4;
  // mr r6,r7
  r6 = r7;
  // subf r30,r7,r3
  r30 = r3 - r7;
  // subf r29,r7,r4
  r29 = r4 - r7;
  // subf r28,r7,r8
  r28 = r8 - r7;
  // mr r11,r31
  r11 = r31;
loc_82586498:
  // lfsx f0,r30,r6
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r6));
  f0 = double(temp.f32);
  // addic. r31,r31,-1
  ca = uint32_t(r31) > 0;
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // lfsx f0,r29,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r6));
  f0 = double(temp.f32);
  // stfsx f0,r28,r6
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + uint32_t(r6), temp.u32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // bne 0x82586498
  if (!cr0.eq) goto loc_82586498;
loc_825864B4:
  // subf r6,r11,r10
  r6 = r10 - r11;
  // addi r30,r5,-1
  r30 = uint64_t(int64_t(int64_t(r5) + -1));
  // cmpw cr6,r30,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r6), so);
  // ble cr6,0x825864c8
  if (!cr6.gt) goto loc_825864C8;
  // mr r30,r6
  r30 = r6;
loc_825864C8:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // ble cr6,0x82586520
  if (!cr6.gt) goto loc_82586520;
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r29,r3
  r29 = r3;
  // add r31,r6,r8
  r31 = r6 + r8;
  // addi r6,r4,4
  r6 = uint64_t(int64_t(int64_t(r4) + 4));
  // subf r28,r4,r3
  r28 = r3 - r4;
  // subf r27,r8,r7
  r27 = r7 - r8;
  // add r11,r30,r11
  r11 = r30 + r11;
loc_825864EC:
  // lfs f13,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // lfsx f0,r28,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r6));
  f0 = double(temp.f32);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfsx f0,r27,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r27) + uint32_t(r31), temp.u32);
  // lfs f0,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f0 = double(temp.f32);
  // lfs f13,-4(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + -4);
  f13 = double(temp.f32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x825864ec
  if (!cr0.eq) goto loc_825864EC;
loc_82586520:
  // subf r10,r11,r10
  r10 = r10 - r11;
  // addi r5,r5,-2
  r5 = uint64_t(int64_t(int64_t(r5) + -2));
  // cmpw cr6,r5,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r10), so);
  // ble cr6,0x82586534
  if (!cr6.gt) goto loc_82586534;
  // mr r5,r10
  r5 = r10;
loc_82586534:
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // ble cr6,0x82586588
  if (!cr6.gt) goto loc_82586588;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r31,r8,r7
  r31 = r7 - r8;
  // add r6,r10,r8
  r6 = r10 + r8;
  // addi r10,r4,8
  r10 = uint64_t(int64_t(int64_t(r4) + 8));
  // subf r4,r4,r3
  r4 = r3 - r4;
  // add r11,r5,r11
  r11 = r5 + r11;
loc_82586554:
  // lfs f13,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lfsx f0,r10,r4
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r4));
  f0 = double(temp.f32);
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfsx f0,r6,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + uint32_t(r31), temp.u32);
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // lfs f13,-8(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f13 = double(temp.f32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // bne 0x82586554
  if (!cr0.eq) goto loc_82586554;
loc_82586588:
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // blt cr6,0x825865e4
  if (cr6.lt) goto loc_825865E4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x825865e0
  if (!cr6.gt) goto loc_825865E0;
  // lis r4,-32256
  r4 = uint64_t(int64_t(-2113929216));
  // mr r10,r8
  r10 = r8;
  // subf r6,r7,r8
  r6 = r8 - r7;
  // subf r8,r8,r7
  r8 = r7 - r8;
  // lfs f13,8708(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8708);
  f13 = double(temp.f32);
loc_825865B4:
  // lfs f0,0(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x825865d4
  if (!cr6.gt) goto loc_825865D4;
  // stfsx f0,r10,r8
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r8), temp.u32);
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // lfsx f0,r7,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r6));
  f0 = double(temp.f32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
loc_825865D4:
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne 0x825865b4
  if (!cr0.eq) goto loc_825865B4;
loc_825865E0:
  // stw r5,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r5));
loc_825865E4:
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_82586758: lifted from generated/ by lift.py.
lifted::Result f82586758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82586758 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-88(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -88, std::bit_cast<uint64_t>(f29));
  // stfd f30,-80(r1)
  REX_STORE_U64(uint32_t(r1) + -80, std::bit_cast<uint64_t>(f30));
  // stfd f31,-72(r1)
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-1232(r1)
  ea = -1232 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r4
  r26 = r4;
  // fmr f29,f1
  f29 = f1;
  // mr r30,r3
  r30 = r3;
  // addi r25,r26,8
  r25 = uint64_t(int64_t(int64_t(r26) + 8));
  // mr r27,r5
  r27 = r5;
  // mr r29,r8
  r29 = r8;
  // lfs f1,52(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 52);
  f1 = double(temp.f32);
  // lfs f2,0(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 0);
  f2 = double(temp.f32);
  // lfs f3,48(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 48);
  f3 = double(temp.f32);
  // lfs f4,4(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 4);
  f4 = double(temp.f32);
  // bl 0x825858a0
  // (left out: dead in C++)
  f1 = f825858A0(base, f1, f2, f3, f4);
  // fmr f30,f1
  lifted::ScalarFloatMode();
  f30 = f1;
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x825867b4
  if (!cr6.eq) goto loc_825867B4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // fmuls f30,f30,f0
  f30 = double(float(f30 * f0));
loc_825867B4:
  // lwz r28,0(r30)
  r28 = REX_LOAD_U32(uint32_t(r30) + 0);
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // bl 0x825852f0
  // (left out: dead in C++)
  r3 = f825852F0(base, f1);
  // mr r31,r3
  r31 = r3;
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r27
  r4 = r27;
  // mr r3,r28
  r3 = r28;
  // mr r5,r31
  r5 = r31;
  // bl 0x82584b90
  // (left out: dead in C++)
  { const lifted::Result o = f82584B90(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825850e0
  // (left out: dead in C++)
  { const lifted::Result o = f825850E0(base, r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f31,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f31 = double(temp.f32);
  // fmr f2,f31
  f2 = f31;
  // bl 0x82585948
  // (left out: dead in C++)
  { const lifted::Result o = f82585948(base, r1, r13, r3, r4, r6, f1, f2); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f1,0(r26)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r26) + 0, temp.u32);
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // lfs f10,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f10 = double(temp.f32);
  // bne cr6,0x82586820
  if (!cr6.eq) goto loc_82586820;
  // fmuls f0,f1,f10
  f0 = double(float(f1 * f10));
  // stfs f0,0(r26)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r26) + 0, temp.u32);
loc_82586820:
  // lfs f12,28(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 28);
  f12 = double(temp.f32);
  // fmr f9,f29
  f9 = f29;
  // lfs f0,4(r25)
  temp.u32 = REX_LOAD_U32(uint32_t(r25) + 4);
  f0 = double(temp.f32);
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // lfs f13,48(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 48);
  f13 = double(temp.f32);
  // bge cr6,0x8258683c
  if (!cr6.lt) goto loc_8258683C;
  // stfs f31,28(r30)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r30) + 28, temp.u32);
loc_8258683C:
  // fcmpu cr6,f29,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f29, f31);
  // bge cr6,0x82586848
  if (!cr6.lt) goto loc_82586848;
  // fmr f9,f31
  f9 = f31;
loc_82586848:
  // fsubs f12,f0,f13
  lifted::ScalarFloatMode();
  f12 = double(float(f0 - f13));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,11340(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11340);
  f11 = double(temp.f32);
  // fabs f12,f12
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // fcmpu cr6,f12,f11
  cr6 = lifted::CompareFloat(f12, f11);
  // bge cr6,0x8258686c
  if (!cr6.lt) goto loc_8258686C;
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fmuls f0,f0,f10
  f0 = double(float(f0 * f10));
  // b 0x825868a4
  goto loc_825868A4;
loc_8258686C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // lfs f11,11344(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11344);
  f11 = double(temp.f32);
  // fcmpu cr6,f12,f11
  cr6 = lifted::CompareFloat(f12, f11);
  // bge cr6,0x82586890
  if (!cr6.lt) goto loc_82586890;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x82586898
  if (cr6.gt) goto loc_82586898;
loc_82586888:
  // fdivs f0,f0,f9
  lifted::ScalarFloatMode();
  f0 = double(float(f0 / f9));
  // b 0x825868a0
  goto loc_825868A0;
loc_82586890:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x82586888
  if (cr6.gt) goto loc_82586888;
loc_82586898:
  // lfs f0,28(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 28);
  f0 = double(temp.f32);
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
loc_825868A0:
  // fmuls f0,f0,f12
  lifted::ScalarFloatMode();
  f0 = double(float(f0 * f12));
loc_825868A4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x825868b8
  if (!cr6.lt) goto loc_825868B8;
  // fmr f0,f13
  f0 = f13;
loc_825868B8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x825868cc
  if (!cr6.gt) goto loc_825868CC;
  // fmr f0,f13
  f0 = f13;
loc_825868CC:
  // stfs f9,28(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r30) + 28, temp.u32);
  // stfs f0,4(r26)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r26) + 4, temp.u32);
  // addi r1,r1,1232
  r1 = uint64_t(int64_t(int64_t(r1) + 1232));
  // lfd f29,-88(r1)
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -88)));
  // lfd f30,-80(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -80)));
  // lfd f31,-72(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825868E8: lifted from generated/ by lift.py.
uint64_t f825868E8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825868E8 (lifted)");
  uint64_t r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r9,-32154
  r9 = uint64_t(int64_t(-2107244544));
  // mr r10,r3
  r10 = r3;
  // addi r25,r7,320
  r25 = uint64_t(int64_t(int64_t(r7) + 320));
  // addi r24,r7,368
  r24 = uint64_t(int64_t(int64_t(r7) + 368));
  // lfs f0,4208(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // addi r28,r9,-9816
  r28 = uint64_t(int64_t(int64_t(r9) + -9816));
  // addi r11,r11,-27968
  r11 = uint64_t(int64_t(int64_t(r11) + -27968));
  // ble cr6,0x8258692c
  if (!cr6.gt) goto loc_8258692C;
  // addi r9,r11,1644
  r9 = uint64_t(int64_t(int64_t(r11) + 1644));
  // mr r22,r11
  r22 = r11;
  // mr r8,r28
  r8 = r28;
  // b 0x82586938
  goto loc_82586938;
loc_8258692C:
  // addi r22,r11,48
  r22 = uint64_t(int64_t(int64_t(r11) + 48));
  // addi r8,r28,24
  r8 = uint64_t(int64_t(int64_t(r28) + 24));
  // addi r9,r11,2744
  r9 = uint64_t(int64_t(int64_t(r11) + 2744));
loc_82586938:
  // stw r9,-112(r1)
  REX_STORE_U32(uint32_t(r1) + -112, uint32_t(r9));
  // addi r9,r28,48
  r9 = uint64_t(int64_t(int64_t(r28) + 48));
  // addi r3,r1,-112
  r3 = uint64_t(int64_t(int64_t(r1) + -112));
  // addi r6,r11,4324
  r6 = uint64_t(int64_t(int64_t(r11) + 4324));
  // addi r7,r11,3600
  r7 = uint64_t(int64_t(int64_t(r11) + 3600));
  // addi r11,r11,4984
  r11 = uint64_t(int64_t(int64_t(r11) + 4984));
  // addi r30,r9,-16
  r30 = uint64_t(int64_t(int64_t(r9) + -16));
  // subf r23,r3,r8
  r23 = r8 - r3;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stw r6,-104(r1)
  REX_STORE_U32(uint32_t(r1) + -104, uint32_t(r6));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // stw r7,-108(r1)
  REX_STORE_U32(uint32_t(r1) + -108, uint32_t(r7));
  // stw r11,-100(r1)
  REX_STORE_U32(uint32_t(r1) + -100, uint32_t(r11));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lfs f9,2612(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2612);
  f9 = double(temp.f32);
  // mr r11,r25
  r11 = r25;
  // lfs f10,2608(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f10 = double(temp.f32);
  // addi r6,r1,-112
  r6 = uint64_t(int64_t(int64_t(r1) + -112));
loc_82586988:
  // cmpwi cr6,r26,4
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 4, so);
  // bge cr6,0x82586a5c
  if (!cr6.lt) goto loc_82586A5C;
  // lwz r9,0(r6)
  r9 = REX_LOAD_U32(uint32_t(r6) + 0);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r7,r8,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,4(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 4);
  // add r8,r8,r7
  r8 = r8 + r7;
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f0,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f0 = double(temp.f32);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lwz r8,24(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 24);
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // ble 0x825869e8
  if (!cr0.gt) goto loc_825869E8;
  // lwz r31,0(r10)
  r31 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r7,28(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 28);
loc_825869CC:
  // lwz r21,0(r7)
  r21 = REX_LOAD_U32(uint32_t(r7) + 0);
  // cmpw cr6,r21,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r31), so);
  // beq cr6,0x82586a54
  if (cr6.eq) goto loc_82586A54;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // cmpw cr6,r3,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r8), so);
  // blt cr6,0x825869cc
  if (cr6.lt) goto loc_825869CC;
loc_825869E8:
  // lwz r7,16(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 16);
loc_825869EC:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lfs f8,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f8 = double(temp.f32);
  // lwz r9,8(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 8);
  // lfs f7,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f7 = double(temp.f32);
  // rlwinm r8,r8,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,4(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 4);
  // add r9,r9,r8
  r9 = r9 + r8;
  // rlwinm r8,r3,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 3) & 0xFFFFFFF8;
  // add r8,r8,r7
  r8 = r8 + r7;
  // lfs f13,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f13 = double(temp.f32);
  // fmuls f6,f13,f13
  f6 = double(float(f13 * f13));
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f11,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f11 = double(temp.f32);
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // lfs f12,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f12 = double(temp.f32);
  // fmadds f6,f0,f0,f6
  f6 = double(float(std::fma(f0, f0, f6)));
  // fmsubs f13,f0,f12,f13
  f13 = double(float(std::fma(f0, f12, -f13)));
  // fdivs f0,f10,f6
  f0 = double(float(f10 / f6));
  // fmadds f13,f13,f0,f8
  f13 = double(float(std::fma(f13, f0, f8)));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f13,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f13 = double(temp.f32);
  // fmuls f13,f13,f12
  f13 = double(float(f13 * f12));
  // lfs f12,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f12 = double(temp.f32);
  // fmadds f13,f12,f11,f13
  f13 = double(float(std::fma(f12, f11, f13)));
  // fmadds f0,f13,f0,f7
  f0 = double(float(std::fma(f13, f0, f7)));
  // b 0x82586a7c
  goto loc_82586A7C;
loc_82586A54:
  // lwz r7,20(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 20);
  // b 0x825869ec
  goto loc_825869EC;
loc_82586A5C:
  // lwz r8,0(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,4(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 4);
  // add r9,r9,r8
  r9 = r9 + r8;
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f0,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f0 = double(temp.f32);
loc_82586A7C:
  // lwzx r9,r23,r6
  r9 = REX_LOAD_U32(uint32_t(r23) + uint32_t(r6));
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // mr r31,r24
  r31 = r24;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // subf r8,r25,r9
  r8 = r9 - r25;
loc_82586A90:
  // fmr f0,f9
  lifted::ScalarFloatMode();
  f0 = f9;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // ble cr6,0x82586abc
  if (!cr6.gt) goto loc_82586ABC;
  // mr r9,r25
  r9 = r25;
  // mr r7,r27
  r7 = r27;
loc_82586AA4:
  // lfsx f13,r8,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r9));
  f13 = double(temp.f32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // lfs f12,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f12 = double(temp.f32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fmadds f0,f13,f12,f0
  f0 = double(float(std::fma(f13, f12, f0)));
  // bne 0x82586aa4
  if (!cr0.eq) goto loc_82586AA4;
loc_82586ABC:
  // stfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // addic. r3,r3,-1
  ca = uint32_t(r3) > 0;
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // add r8,r29,r8
  r8 = r29 + r8;
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82586a90
  if (!cr0.eq) goto loc_82586A90;
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r7,r28,48
  r7 = uint64_t(int64_t(int64_t(r28) + 48));
  // lfs f13,0(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 0);
  f13 = double(temp.f32);
  // addi r9,r27,1
  r9 = uint64_t(int64_t(int64_t(r27) + 1));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lfs f0,4(r24)
  temp.u32 = REX_LOAD_U32(uint32_t(r24) + 4);
  f0 = double(temp.f32);
  // addi r8,r29,4
  r8 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r7,r7,8
  r7 = uint64_t(int64_t(int64_t(r7) + 8));
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // addi r27,r9,1
  r27 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r29,r8,4
  r29 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpw cr6,r30,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r7), so);
  // blt cr6,0x82586988
  if (cr6.lt) goto loc_82586988;
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // ble cr6,0x82586b60
  if (!cr6.gt) goto loc_82586B60;
  // mr r11,r25
  r11 = r25;
  // subf r9,r25,r22
  r9 = r22 - r25;
  // subf r8,r25,r4
  r8 = r4 - r25;
  // mr r10,r5
  r10 = r5;
loc_82586B44:
  // lfsx f0,r11,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  f0 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfsx f0,r11,r8
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82586b44
  if (!cr0.eq) goto loc_82586B44;
loc_82586B60:
  // b 0x825a6ebc
  // (left out: dead in C++)
  return r3;
}

// sub_82586B68: lifted from generated/ by lift.py.
lifted::Result f82586B68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82586B68 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lhz r10,92(r3)
  r10 = REX_LOAD_U16(uint32_t(r3) + 92);
  // mr r11,r4
  r11 = r4;
  // mr r31,r6
  r31 = r6;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82586b9c
  if (!cr6.eq) goto loc_82586B9C;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f5,7140(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7140);
  f5 = double(temp.f32);
  // b 0x82586ba8
  goto loc_82586BA8;
loc_82586B9C:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // lfs f5,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f5 = double(temp.f32);
loc_82586BA8:
  // lwz r8,24(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 24);
  // addi r7,r3,104
  r7 = uint64_t(int64_t(int64_t(r3) + 104));
  // lwz r30,20(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 20);
  // mr r4,r5
  r4 = r5;
  // lfs f4,32(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f4 = double(temp.f32);
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lfs f3,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f3 = double(temp.f32);
  // lwz r9,12(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 12);
  // lwz r6,8(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f2,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f2 = double(temp.f32);
  // lwz r5,4(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lfs f1,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f1 = double(temp.f32);
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // stw r7,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r7));
  // stw r8,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r8));
  // stw r30,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r30));
  // bl 0x82587238
  // (left out: dead in C++)
  { const lifted::Result o = f82587238(base, r1, r13, r3, r4, r5, r6, r9, r10, f1, f2, f3, f4, f5); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r11,r11,80
  r11 = uint64_t(int64_t(int64_t(r11) + 80));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
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

// sub_82587088: lifted from generated/ by lift.py.
void f82587088(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82587088 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r30,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r30);
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r30,12(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 12);
  // lwz r8,0(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lhz r6,4(r3)
  r6 = REX_LOAD_U16(uint32_t(r3) + 4);
  // lhz r9,0(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 0);
  // add r9,r9,r8
  r9 = r9 + r8;
  // slw r10,r10,r5
  r10 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r5) & 0x3F));
  // addi r31,r10,-1
  r31 = uint64_t(int64_t(int64_t(r10) + -1));
  // lbz r10,0(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 0);
  // mr r8,r10
  r8 = r10;
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // add r7,r8,r5
  r7 = r8 + r5;
  // rlwinm. r5,r7,0,0,28
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFF8;
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // beq 0x82587130
  if (cr0.eq) goto loc_82587130;
  // lbz r5,0(r9)
  r5 = REX_LOAD_U8(uint32_t(r9) + 0);
  // clrlwi r6,r6,16
  r6 = uint32_t(r6) & 0xFFFF;
  // subfic r8,r8,8
  ca = uint32_t(r8) <= 8;
  r8 = static_cast<uint64_t>(8) - r8;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // srw r10,r5,r10
  r10 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r5) >> (uint8_t(r10) & 0x3F));
  // stb r10,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r10));
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // mr r5,r10
  r5 = r10;
  // cmplw cr6,r5,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r6), so);
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // blt cr6,0x8258710c
  if (cr6.lt) goto loc_8258710C;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
loc_8258710C:
  // lbz r11,0(r9)
  r11 = REX_LOAD_U8(uint32_t(r9) + 0);
  // sraw r9,r31,r8
  temp.u32 = uint32_t(r8) & 0x3F;
  if (temp.u32 > 0x1F) temp.u32 = 0x1F;
  ca = (int32_t(r31) < 0) & (((int32_t(r31) >> temp.u32) << temp.u32) != int32_t(r31));
  r9 = uint64_t(int64_t(int32_t(r31) >> temp.u32));
  // lbz r10,0(r4)
  r10 = REX_LOAD_U8(uint32_t(r4) + 0);
  // addi r7,r7,-8
  r7 = uint64_t(int64_t(int64_t(r7) + -8));
  // and r11,r9,r11
  r11 = r9 & r11;
  // slw r11,r11,r8
  r11 = uint8_t(r8) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r8) & 0x3F));
  // or r11,r11,r10
  r11 = r11 | r10;
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // b 0x82587174
  goto loc_82587174;
loc_82587130:
  // lbz r9,0(r9)
  r9 = REX_LOAD_U8(uint32_t(r9) + 0);
  // clrlwi r8,r31,24
  r8 = uint32_t(r31) & 0xFF;
  // cmpwi cr6,r7,8
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 8, so);
  // srw r10,r9,r10
  r10 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r9) >> (uint8_t(r10) & 0x3F));
  // and r10,r10,r8
  r10 = r10 & r8;
  // stb r10,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r10));
  // bne cr6,0x82587174
  if (!cr6.eq) goto loc_82587174;
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // clrlwi r9,r6,16
  r9 = uint32_t(r6) & 0xFFFF;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // mr r8,r10
  r8 = r10;
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // blt cr6,0x82587174
  if (cr6.lt) goto loc_82587174;
  // sth r7,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r7));
loc_82587174:
  // stb r7,0(r30)
  REX_STORE_U8(uint32_t(r30) + 0, uint8_t(r7));
  // ld r30,-16(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -16);
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_82587658: lifted from generated/ by lift.py.
lifted::Result f82587658(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82587658 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq cr6,0x825876bc
  if (cr6.eq) goto loc_825876BC;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // lwz r11,30560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30560);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x825876a8
  if (!cr0.eq) goto loc_825876A8;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x825876b8
  goto loc_825876B8;
loc_825876A8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // lfs f0,8692(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8692);
  f0 = double(temp.f32);
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
loc_825876B8:
  // mr r3,r30
  r3 = r30;
loc_825876BC:
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

// sub_825876D8: lifted from generated/ by lift.py.
lifted::Result f825876D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825876D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82587710
  if (cr0.eq) goto loc_82587710;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r11,30564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 30564);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82587710:
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

// sub_82587960: lifted from generated/ by lift.py.
uint64_t f82587960(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82587960 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lfs f31,7136(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7136);
  f31 = double(temp.f32);
  // mr r28,r3
  r28 = r3;
  // lfs f30,-19940(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + -19940);
  f30 = double(temp.f32);
  // mr r30,r4
  r30 = r4;
  // lfs f28,-19944(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -19944);
  f28 = double(temp.f32);
  // mr r26,r5
  r26 = r5;
  // lfs f29,-19948(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -19948);
  f29 = double(temp.f32);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r11,73
  r11 = uint64_t(int64_t(73));
  // addi r29,r10,-28544
  r29 = uint64_t(int64_t(int64_t(r10) + -28544));
loc_825879B4:
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r11,1
  r4 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r3,r28
  r3 = r28;
  // bl 0x82584ed8
  // (left out: dead in C++)
  f82584ED8(base, r1, r3, r4, r5, r6);
  // fmuls f13,f31,f29
  lifted::ScalarFloatMode();
  f13 = double(float(f31 * f29));
  // lfs f0,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x82587a6c
  if (cr6.lt) goto loc_82587A6C;
  // fmuls f13,f31,f28
  f13 = double(float(f31 * f28));
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x825879f0
  if (cr6.lt) goto loc_825879F0;
  // mr r27,r31
  r27 = r31;
loc_825879F0:
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // fmr f31,f0
  lifted::ScalarFloatMode();
  f31 = f0;
  // lfs f12,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f12 = double(temp.f32);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // add r10,r10,r29
  r10 = r10 + r29;
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // lfs f13,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fmuls f0,f13,f30
  f0 = double(float(f13 * f30));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // blt cr6,0x82587a6c
  if (cr6.lt) goto loc_82587A6C;
  // b 0x82587a28
  goto loc_82587A28;
loc_82587A1C:
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
loc_82587A28:
  // fcmpu cr6,f13,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82587a1c
  if (cr6.gt) goto loc_82587A1C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82587a6c
  if (cr6.lt) goto loc_82587A6C;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r28
  r10 = r10 + r28;
loc_82587A40:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82587a5c
  if (!cr6.lt) goto loc_82587A5C;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bge 0x82587a40
  if (!cr0.lt) goto loc_82587A40;
loc_82587A5C:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82587a6c
  if (cr6.lt) goto loc_82587A6C;
  // cmpwi cr6,r31,5
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 5, so);
  // blt cr6,0x825879b4
  if (cr6.lt) goto loc_825879B4;
loc_82587A6C:
  // mr r3,r31
  r3 = r31;
  // stw r27,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r27));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82587A88: lifted from generated/ by lift.py.
uint64_t f82587A88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82587A88 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // mr r28,r5
  r28 = r5;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r29,r4
  r29 = r4;
  // addi r8,r11,-28544
  r8 = uint64_t(int64_t(int64_t(r11) + -28544));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lwz r30,0(r28)
  r30 = REX_LOAD_U32(uint32_t(r28) + 0);
  // mr r27,r6
  r27 = r6;
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r29
  r11 = r11 + r29;
  // lfs f0,3396(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 3396);
  f0 = double(temp.f32);
  // lwz r26,0(r27)
  r26 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r31,-4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + -4);
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r10,r11,r8
  r10 = r11 + r8;
  // lfs f13,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82587bb8
  if (cr6.gt) goto loc_82587BB8;
  // lfsx f12,r11,r3
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lfs f0,8712(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8712);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // lfs f0,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
loc_82587AF8:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lfs f11,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // blt cr6,0x82587af8
  if (cr6.lt) goto loc_82587AF8;
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r10,r11,r3
  r10 = r11 + r3;
  // lfs f0,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82587b30
  if (!cr6.lt) goto loc_82587B30;
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82587b30
  if (!cr6.lt) goto loc_82587B30;
  // li r9,1
  r9 = uint64_t(int64_t(1));
loc_82587B30:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // add r11,r11,r8
  r11 = r11 + r8;
  // lfs f0,8676(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8676);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
loc_82587B40:
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82587b40
  if (cr6.lt) goto loc_82587B40;
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r3,r11,r3
  r3 = r11 + r3;
  // lfs f0,-4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + -4);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82587b78
  if (!cr6.lt) goto loc_82587B78;
  // lfs f0,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82587b78
  if (!cr6.lt) goto loc_82587B78;
  // li r9,1
  r9 = uint64_t(int64_t(1));
loc_82587B78:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bne cr6,0x82587bb8
  if (!cr6.eq) goto loc_82587BB8;
  // cmpwi cr6,r30,1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 1, so);
  // bne cr6,0x82587ba8
  if (!cr6.eq) goto loc_82587BA8;
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // subfic r4,r31,74
  ca = uint32_t(r31) <= 74;
  r4 = static_cast<uint64_t>(74) - r31;
  // bl 0x82584ed8
  // (left out: dead in C++)
  f82584ED8(base, r1, r3, r4, r5, r6);
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // b 0x82587bb8
  goto loc_82587BB8;
loc_82587BA8:
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpw cr6,r26,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r30), so);
  // bne cr6,0x82587bb8
  if (!cr6.eq) goto loc_82587BB8;
  // li r26,0
  r26 = uint64_t(int64_t(0));
loc_82587BB8:
  // stw r26,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r26));
  // stw r30,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r30));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82587BC8: lifted from generated/ by lift.py.
lifted::Result f82587BC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_82587BC8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e54
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-144
  r12 = uint64_t(int64_t(int64_t(r1) + -144));
  // bl 0x825a7910
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-624(r1)
  ea = -624 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r17,r4
  r17 = r4;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // mr r18,r3
  r18 = r3;
  // fmr f27,f2
  f27 = f2;
  // mr r16,r5
  r16 = r5;
  // mr r5,r17
  r5 = r17;
  // mr r4,r18
  r4 = r18;
  // mr r15,r6
  r15 = r6;
  // mr r29,r7
  r29 = r7;
  // mr r26,r8
  r26 = r8;
  // bl 0x82584e48
  // (left out: dead in C++)
  { const lifted::Result o = f82584E48(base, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lfs f0,11016(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11016);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f13,f1,f0
  f13 = double(float(f1 + f0));
  // lfs f0,8152(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r19,r11,-28544
  r19 = uint64_t(int64_t(int64_t(r11) + -28544));
  // fdivs f26,f0,f13
  f26 = double(float(f0 / f13));
loc_82587C2C:
  // mr r7,r17
  r7 = r17;
  // lfsx f1,r31,r19
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r19));
  f1 = double(temp.f32);
  // mr r6,r18
  r6 = r18;
  // fmr f2,f26
  f2 = f26;
  // mr r5,r15
  r5 = r15;
  // mr r4,r16
  r4 = r16;
  // bl 0x82589158
  // (left out: dead in C++)
  { const lifted::Result o = f82589158(base, r1, r3, r4, r6, r7, f1, f2); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfsx f1,r31,r11
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r31) + uint32_t(r11), temp.u32);
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
  // cmpwi cr6,r31,296
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 296, so);
  // blt cr6,0x82587c2c
  if (cr6.lt) goto loc_82587C2C;
  // addi r11,r1,132
  r11 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r10,37
  r10 = uint64_t(int64_t(37));
loc_82587C64:
  // lfs f0,-4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f0 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne 0x82587c64
  if (!cr0.eq) goto loc_82587C64;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10588(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10588);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f0,f31,f0
  f0 = double(float(f31 * f0));
  // lfs f13,-19952(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -19952);
  f13 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // fmuls f12,f31,f13
  f12 = double(float(f31 * f13));
  // lfs f13,0(r19)
  temp.u32 = REX_LOAD_U32(uint32_t(r19) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x82587cb8
  if (!cr6.lt) goto loc_82587CB8;
  // mr r10,r19
  r10 = r19;
loc_82587CA4:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82587ca4
  if (cr6.lt) goto loc_82587CA4;
loc_82587CB8:
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r21,r11
  r21 = r11;
  // lfsx f0,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  f0 = double(temp.f32);
loc_82587CC8:
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfsx f13,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82587ce0
  if (!cr6.lt) goto loc_82587CE0;
  // fmr f0,f13
  f0 = f13;
  // mr r21,r11
  r21 = r11;
loc_82587CE0:
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,296
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 296, so);
  // bge cr6,0x82587cfc
  if (!cr6.lt) goto loc_82587CFC;
  // lfsx f13,r10,r19
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r19));
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // ble cr6,0x82587cc8
  if (!cr6.gt) goto loc_82587CC8;
loc_82587CFC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fcmpu cr6,f27,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f27, f0);
  // lfs f28,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f28 = double(temp.f32);
  // ble cr6,0x82587d14
  if (!cr6.gt) goto loc_82587D14;
  // fadds f0,f0,f27
  f0 = double(float(f0 + f27));
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
loc_82587D14:
  // rlwinm r31,r21,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r21) | (r21 << 32), 2) & 0xFFFFFFFC;
  // addi r30,r1,128
  r30 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfsx f31,r31,r30
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r30));
  f31 = double(temp.f32);
  // stfsx f0,r31,r30
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + uint32_t(r30), temp.u32);
  // bl 0x82587960
  // (left out: dead in C++)
  r3 = f82587960(base, r1, r13, r3, r4, r5);
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x82587a88
  // (left out: dead in C++)
  r3 = f82587A88(base, r1, r13, r3, r4, r5, r6);
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r7,88(r1)
  r7 = REX_LOAD_U32(uint32_t(r1) + 88);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r23,r11
  r23 = r11;
  // mr r20,r7
  r20 = r7;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82587d98
  if (!cr6.gt) goto loc_82587D98;
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
loc_82587D70:
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmpw cr6,r21,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r8), so);
  // bne cr6,0x82587d84
  if (!cr6.eq) goto loc_82587D84;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // cmpw cr6,r21,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r8), so);
loc_82587D84:
  // bgt cr6,0x82587d98
  if (cr6.gt) goto loc_82587D98;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // blt cr6,0x82587d70
  if (cr6.lt) goto loc_82587D70;
loc_82587D98:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // bne cr6,0x82587e50
  if (!cr6.eq) goto loc_82587E50;
  // rlwinm r9,r7,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r31,r30
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r30));
  f0 = double(temp.f32);
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r6,r1,128
  r6 = uint64_t(int64_t(int64_t(r1) + 128));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // lwzx r9,r9,r8
  r9 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r8));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lfsx f12,r9,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r6));
  f12 = double(temp.f32);
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lfs f13,-19944(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -19944);
  f13 = double(temp.f32);
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // bne cr6,0x82587df4
  if (!cr6.eq) goto loc_82587DF4;
  // lwz r9,96(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 96);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lfsx f11,r9,r8
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r8));
  f11 = double(temp.f32);
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // lfs f12,-6152(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -6152);
  f12 = double(temp.f32);
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82587e04
  if (!cr6.lt) goto loc_82587E04;
loc_82587DF4:
  // cmpw cr6,r10,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r7), so);
  // ble cr6,0x82587e50
  if (!cr6.gt) goto loc_82587E50;
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x82587e50
  if (cr6.lt) goto loc_82587E50;
loc_82587E04:
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // ble cr6,0x82587e34
  if (!cr6.gt) goto loc_82587E34;
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
  // subf r9,r10,r11
  r9 = r11 - r10;
  // add r8,r8,r7
  r8 = r8 + r7;
loc_82587E1C:
  // addi r7,r8,-4
  r7 = uint64_t(int64_t(int64_t(r8) + -4));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lwz r6,0(r7)
  r6 = REX_LOAD_U32(uint32_t(r7) + 0);
  // stw r6,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r6));
  // mr r8,r7
  r8 = r7;
  // bne 0x82587e1c
  if (!cr0.eq) goto loc_82587E1C;
loc_82587E34:
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stwx r21,r9,r8
  REX_STORE_U32(uint32_t(r9) + uint32_t(r8), uint32_t(r21));
  // blt cr6,0x82587e4c
  if (cr6.lt) goto loc_82587E4C;
  // mr r20,r10
  r20 = r10;
loc_82587E4C:
  // addi r23,r11,1
  r23 = uint64_t(int64_t(int64_t(r11) + 1));
loc_82587E50:
  // stfsx f31,r31,r30
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + uint32_t(r30), temp.u32);
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // addi r22,r23,-1
  r22 = uint64_t(int64_t(int64_t(r23) + -1));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // ble cr6,0x82587f80
  if (!cr6.gt) goto loc_82587F80;
  // addi r27,r1,96
  r27 = uint64_t(int64_t(int64_t(r1) + 96));
  // subf r24,r26,r29
  r24 = r29 - r26;
loc_82587E70:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x82587e88
  if (cr6.eq) goto loc_82587E88;
  // cmpw cr6,r28,r22
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r22), so);
  // beq cr6,0x82587e88
  if (cr6.eq) goto loc_82587E88;
  // cmpw cr6,r28,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r20), so);
  // bne cr6,0x82587f70
  if (!cr6.eq) goto loc_82587F70;
loc_82587E88:
  // cmpw cr6,r28,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r20), so);
  // bne cr6,0x82587e98
  if (!cr6.eq) goto loc_82587E98;
  // lwz r11,708(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 708);
  // stw r25,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r25));
loc_82587E98:
  // lwz r30,0(r27)
  r30 = REX_LOAD_U32(uint32_t(r27) + 0);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // rlwinm r10,r30,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // cmpwi r30,0
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // add r9,r10,r19
  r9 = r10 + r19;
  // lfsx f31,r10,r8
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r8));
  f31 = double(temp.f32);
  // lfs f29,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f29 = double(temp.f32);
  // ble 0x82587ed0
  if (!cr0.gt) goto loc_82587ED0;
  // lfs f0,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f0 = double(temp.f32);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // fadds f0,f0,f29
  f0 = double(float(f0 + f29));
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
loc_82587ED0:
  // addi r10,r30,1
  r10 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r10,74
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 74, so);
  // bge cr6,0x82587efc
  if (!cr6.lt) goto loc_82587EFC;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lfsx f0,r10,r19
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r19));
  f0 = double(temp.f32);
  // fadds f0,f0,f29
  f0 = double(float(f0 + f29));
  // fmuls f0,f0,f28
  f0 = double(float(f0 * f28));
  // stfsx f0,r9,r8
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + uint32_t(r8), temp.u32);
loc_82587EFC:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82587f48
  if (!cr6.gt) goto loc_82587F48;
  // addi r31,r1,80
  r31 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r29,r11
  r29 = r11;
loc_82587F0C:
  // lfs f30,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f30 = double(temp.f32);
  // mr r7,r17
  r7 = r17;
  // mr r6,r18
  r6 = r18;
  // fmr f2,f26
  f2 = f26;
  // mr r5,r15
  r5 = r15;
  // fmr f1,f30
  f1 = f30;
  // mr r4,r16
  r4 = r16;
  // bl 0x82589158
  // (left out: dead in C++)
  { const lifted::Result o = f82589158(base, r1, r3, r4, r6, r7, f1, f2); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // ble cr6,0x82587f3c
  if (!cr6.gt) goto loc_82587F3C;
  // fmr f29,f30
  f29 = f30;
  // fmr f31,f1
  f31 = f1;
loc_82587F3C:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x82587f0c
  if (!cr0.eq) goto loc_82587F0C;
loc_82587F48:
  // stfsx f29,r24,r26
  lifted::ScalarFloatMode();
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r24) + uint32_t(r26), temp.u32);
  // cmpw cr6,r30,r21
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r21), so);
  // bne cr6,0x82587f64
  if (!cr6.eq) goto loc_82587F64;
  // fcmpu cr6,f27,f31
  cr6 = lifted::CompareFloat(f27, f31);
  // ble cr6,0x82587f64
  if (!cr6.gt) goto loc_82587F64;
  // fadds f0,f31,f27
  f0 = double(float(f31 + f27));
  // fmuls f31,f0,f28
  f31 = double(float(f0 * f28));
loc_82587F64:
  // stfs f31,0(r26)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r26) + 0, temp.u32);
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // addi r26,r26,4
  r26 = uint64_t(int64_t(int64_t(r26) + 4));
loc_82587F70:
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // cmpw cr6,r28,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r23), so);
  // blt cr6,0x82587e70
  if (cr6.lt) goto loc_82587E70;
loc_82587F80:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,624
  r1 = uint64_t(int64_t(int64_t(r1) + 624));
  // addi r12,r1,-144
  r12 = uint64_t(int64_t(int64_t(r1) + -144));
  // bl 0x825a795c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ea4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82587F98: lifted from generated/ by lift.py.
uint64_t f82587F98(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82587F98 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lfs f13,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // fadds f11,f13,f0
  f11 = double(float(f13 + f0));
  // stfs f11,0(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // addi r8,r4,4
  r8 = uint64_t(int64_t(int64_t(r4) + 4));
  // lfs f12,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f12 = double(temp.f32);
  // add r10,r11,r3
  r10 = r11 + r3;
  // add r11,r11,r4
  r11 = r11 + r4;
  // stfs f12,0(r4)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // cmpwi cr6,r5,256
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 256, so);
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bne cr6,0x82587ff0
  if (!cr6.eq) goto loc_82587FF0;
  // addi r6,r7,-19936
  r6 = uint64_t(int64_t(int64_t(r7) + -19936));
  // b 0x82587ff8
  goto loc_82587FF8;
loc_82587FF0:
  // addi r7,r7,-19936
  r7 = uint64_t(int64_t(int64_t(r7) + -19936));
  // addi r6,r7,1016
  r6 = uint64_t(int64_t(int64_t(r7) + 1016));
loc_82587FF8:
  // srawi r7,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r7 = uint64_t(int64_t(int32_t(r5) >> 1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cmpwi cr6,r7,1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 1, so);
  // ble cr6,0x825880a4
  if (!cr6.gt) goto loc_825880A4;
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // addi r3,r7,1
  r3 = uint64_t(int64_t(int64_t(r7) + 1));
  // lfs f0,7140(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 7140);
  f0 = double(temp.f32);
loc_82588018:
  // lfs f12,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f12 = double(temp.f32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // fadds f6,f10,f12
  f6 = double(float(f10 + f12));
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // fsubs f10,f12,f10
  f10 = double(float(f12 - f10));
  // fsubs f7,f13,f11
  f7 = double(float(f13 - f11));
  // lfs f9,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f9 = double(temp.f32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // fadds f5,f11,f13
  f5 = double(float(f11 + f13));
  // lfs f8,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f8 = double(temp.f32);
  // addi r6,r6,4
  r6 = uint64_t(int64_t(int64_t(r6) + 4));
  // fmuls f12,f6,f0
  f12 = double(float(f6 * f0));
  // fmuls f11,f10,f0
  f11 = double(float(f10 * f0));
  // fmuls f13,f7,f0
  f13 = double(float(f7 * f0));
  // fmuls f10,f5,f0
  f10 = double(float(f5 * f0));
  // fmuls f7,f12,f8
  f7 = double(float(f12 * f8));
  // fmuls f8,f13,f8
  f8 = double(float(f13 * f8));
  // fmsubs f13,f13,f9,f7
  f13 = double(float(std::fma(f13, f9, -f7)));
  // fmadds f12,f12,f9,f8
  f12 = double(float(std::fma(f12, f9, f8)));
  // fadds f9,f13,f11
  f9 = double(float(f13 + f11));
  // fsubs f13,f11,f13
  f13 = double(float(f11 - f13));
  // stfs f13,0(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fadds f13,f12,f10
  f13 = double(float(f12 + f10));
  // stfs f13,0(r9)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fsubs f13,f10,f12
  f13 = double(float(f10 - f12));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // fneg f13,f9
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f9) ^ 0x8000000000000000));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bne 0x82588018
  if (!cr0.eq) goto loc_82588018;
loc_825880A4:
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r11,r4
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r4));
  f0 = double(temp.f32);
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfsx f0,r11,r4
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r4), temp.u32);
  // blr
  return r3;
}

// sub_825880B8: lifted from generated/ by lift.py.
void f825880B8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825880B8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f0 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // std r31,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r31);
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // addi r9,r3,4
  r9 = uint64_t(int64_t(int64_t(r3) + 4));
  // add r10,r11,r3
  r10 = r11 + r3;
  // add r11,r11,r4
  r11 = r11 + r4;
  // addi r8,r4,4
  r8 = uint64_t(int64_t(int64_t(r4) + 4));
  // addi r7,r11,-4
  r7 = uint64_t(int64_t(int64_t(r11) + -4));
  // cmpwi cr6,r5,256
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 256, so);
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // fadds f10,f12,f0
  f10 = double(float(f12 + f0));
  // fadds f9,f11,f13
  f9 = double(float(f11 + f13));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // fsubs f13,f13,f11
  f13 = double(float(f13 - f11));
  // fsubs f12,f10,f9
  f12 = double(float(f10 - f9));
  // stfs f12,0(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // bne cr6,0x8258811c
  if (!cr6.eq) goto loc_8258811C;
  // addi r31,r11,-19936
  r31 = uint64_t(int64_t(int64_t(r11) + -19936));
  // b 0x82588124
  goto loc_82588124;
loc_8258811C:
  // addi r11,r11,-19936
  r11 = uint64_t(int64_t(int64_t(r11) + -19936));
  // addi r31,r11,1016
  r31 = uint64_t(int64_t(int64_t(r11) + 1016));
loc_82588124:
  // srawi r6,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r6 = uint64_t(int64_t(int32_t(r5) >> 1));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // rlwinm r11,r6,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // cmpwi cr6,r6,1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 1, so);
  // lfs f0,8680(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8680);
  f0 = double(temp.f32);
  // lfsx f13,r11,r3
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  f13 = double(temp.f32);
  // lfsx f12,r11,r4
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r4));
  f12 = double(temp.f32);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // stfsx f13,r11,r3
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r3), temp.u32);
  // stfsx f0,r11,r4
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r4), temp.u32);
  // ble cr6,0x825881d4
  if (!cr6.gt) goto loc_825881D4;
  // addi r11,r6,-1
  r11 = uint64_t(int64_t(int64_t(r6) + -1));
loc_82588158:
  // lfs f11,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // addi r6,r31,4
  r6 = uint64_t(int64_t(int64_t(r31) + 4));
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fsubs f8,f13,f11
  f8 = double(float(f13 - f11));
  // lfs f10,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f10 = double(temp.f32);
  // lfs f12,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f12 = double(temp.f32);
  // fadds f13,f11,f13
  f13 = double(float(f11 + f13));
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // fadds f7,f10,f12
  f7 = double(float(f10 + f12));
  // lfs f9,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f9 = double(temp.f32);
  // fsubs f12,f12,f10
  f12 = double(float(f12 - f10));
  // addi r31,r6,4
  r31 = uint64_t(int64_t(int64_t(r6) + 4));
  // fmuls f11,f0,f8
  f11 = double(float(f0 * f8));
  // fmuls f10,f9,f8
  f10 = double(float(f9 * f8));
  // fmadds f11,f9,f7,f11
  f11 = double(float(std::fma(f9, f7, f11)));
  // fmsubs f0,f0,f7,f10
  f0 = double(float(std::fma(f0, f7, -f10)));
  // fsubs f10,f12,f11
  f10 = double(float(f12 - f11));
  // fadds f12,f11,f12
  f12 = double(float(f11 + f12));
  // stfs f12,0(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fsubs f12,f13,f0
  f12 = double(float(f13 - f0));
  // stfs f12,0(r9)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // fneg f0,f10
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f10) ^ 0x8000000000000000));
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r7,r7,-4
  r7 = uint64_t(int64_t(int64_t(r7) + -4));
  // bne 0x82588158
  if (!cr0.eq) goto loc_82588158;
loc_825881D4:
  // ld r31,-8(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -8);
  // blr
  return;
}

// sub_825881E0: lifted from generated/ by lift.py.
double f825881E0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_825881E0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-144(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -144, std::bit_cast<uint64_t>(f30));
  // stfd f31,-136(r1)
  REX_STORE_U64(uint32_t(r1) + -136, std::bit_cast<uint64_t>(f31));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // addi r11,r11,-18672
  r11 = uint64_t(int64_t(int64_t(r11) + -18672));
  // cmpwi cr6,r5,256
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 256, so);
  // addi r10,r11,1536
  r10 = uint64_t(int64_t(int64_t(r11) + 1536));
  // beq cr6,0x8258820c
  if (cr6.eq) goto loc_8258820C;
  // addi r10,r11,1920
  r10 = uint64_t(int64_t(int64_t(r11) + 1920));
loc_8258820C:
  // srawi r11,r5,4
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0xF) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 4));
  // mulli r17,r11,96
  r17 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(96))));
loc_82588214:
  // rlwinm r11,r25,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 4) & 0xFFFFFFF0;
  // srawi r24,r5,2
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x3) != 0);
  r24 = uint64_t(int64_t(int32_t(r5) >> 2));
  // addi r11,r11,-12
  r11 = uint64_t(int64_t(int64_t(r11) + -12));
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // mullw r11,r11,r24
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r24))));
  // add r9,r11,r3
  r9 = r11 + r3;
  // add r11,r11,r4
  r11 = r11 + r4;
  // addi r26,r9,-16
  r26 = uint64_t(int64_t(int64_t(r9) + -16));
  // addi r31,r11,-16
  r31 = uint64_t(int64_t(int64_t(r11) + -16));
  // ble cr6,0x82588390
  if (!cr6.gt) goto loc_82588390;
  // addi r18,r24,-4
  r18 = uint64_t(int64_t(int64_t(r24) + -4));
  // mulli r21,r24,12
  r21 = uint64_t(int64_t(static_cast<int64_t>(r24 * static_cast<uint64_t>(12))));
  // mr r19,r25
  r19 = r25;
loc_82588248:
  // subf r10,r17,r10
  r10 = r10 - r17;
  // cmpwi cr6,r18,0
  cr6 = lifted::Compare<int32_t>(int32_t(r18), 0, so);
  // blt cr6,0x82588380
  if (cr6.lt) goto loc_82588380;
  // addi r11,r18,4
  r11 = uint64_t(int64_t(int64_t(r18) + 4));
  // rlwinm r23,r24,3,0,28
  r23 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 3) & 0xFFFFFFF8;
  // rlwinm r22,r24,2,0,29
  r22 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r20,r11,30,2,31
  r20 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
loc_82588264:
  // mr r9,r31
  r9 = r31;
  // add r7,r21,r31
  r7 = r21 + r31;
  // add r8,r23,r31
  r8 = r23 + r31;
  // add r11,r22,r31
  r11 = r22 + r31;
  // subf r6,r31,r26
  r6 = r26 - r31;
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_8258827C:
  // add r27,r6,r7
  r27 = r6 + r7;
  // lfs f11,0(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // add r29,r6,r9
  r29 = r6 + r9;
  // lfs f12,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f12 = double(temp.f32);
  // fadds f30,f11,f13
  f30 = double(float(f11 + f13));
  // lfs f0,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // fsubs f13,f13,f11
  f13 = double(float(f13 - f11));
  // lfsx f3,r6,r11
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + uint32_t(r11));
  f3 = double(temp.f32);
  // fadds f1,f12,f0
  f1 = double(float(f12 + f0));
  // lfs f31,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f31 = double(temp.f32);
  // add r28,r6,r8
  r28 = r6 + r8;
  // fadds f11,f31,f3
  f11 = double(float(f31 + f3));
  // lfs f4,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f4 = double(temp.f32);
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // lfs f10,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f10 = double(temp.f32);
  // fsubs f3,f3,f31
  f3 = double(float(f3 - f31));
  // lfs f8,32(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 32);
  f8 = double(temp.f32);
  // lfs f9,16(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 16);
  f9 = double(temp.f32);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // lfs f2,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f2 = double(temp.f32);
  // fadds f12,f2,f4
  f12 = double(float(f2 + f4));
  // lfs f7,48(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 48);
  f7 = double(temp.f32);
  // fsubs f4,f4,f2
  f4 = double(float(f4 - f2));
  // lfs f6,64(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 64);
  f6 = double(temp.f32);
  // lfs f5,80(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 80);
  f5 = double(temp.f32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fsubs f2,f1,f30
  f2 = double(float(f1 - f30));
  // fadds f1,f30,f1
  f1 = double(float(f30 + f1));
  // stfs f1,0(r9)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // fsubs f30,f0,f3
  f30 = double(float(f0 - f3));
  // fadds f0,f0,f3
  f0 = double(float(f0 + f3));
  // fsubs f1,f12,f11
  f1 = double(float(f12 - f11));
  // fadds f31,f13,f4
  f31 = double(float(f13 + f4));
  // fadds f12,f11,f12
  f12 = double(float(f11 + f12));
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // fsubs f13,f4,f13
  f13 = double(float(f4 - f13));
  // fmuls f4,f2,f10
  f4 = double(float(f2 * f10));
  // fmuls f3,f2,f9
  f3 = double(float(f2 * f9));
  // fmuls f12,f30,f8
  f12 = double(float(f30 * f8));
  // fmuls f11,f30,f7
  f11 = double(float(f30 * f7));
  // fmuls f2,f0,f6
  f2 = double(float(f0 * f6));
  // fmuls f0,f0,f5
  f0 = double(float(f0 * f5));
  // fmadds f9,f1,f9,f4
  f9 = double(float(std::fma(f1, f9, f4)));
  // stfs f9,0(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fmsubs f10,f1,f10,f3
  f10 = double(float(std::fma(f1, f10, -f3)));
  // stfsx f10,r6,r11
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r6) + uint32_t(r11), temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmadds f12,f31,f7,f12
  f12 = double(float(std::fma(f31, f7, f12)));
  // stfs f12,0(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fmsubs f12,f31,f8,f11
  f12 = double(float(std::fma(f31, f8, -f11)));
  // stfs f12,0(r28)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // fmadds f12,f13,f5,f2
  f12 = double(float(std::fma(f13, f5, f2)));
  // stfs f12,0(r7)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fmsubs f0,f13,f6,f0
  f0 = double(float(std::fma(f13, f6, -f0)));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // stfs f0,0(r27)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne 0x8258827c
  if (!cr0.eq) goto loc_8258827C;
  // addic. r20,r20,-1
  ca = uint32_t(r20) > 0;
  r20 = uint64_t(int64_t(int64_t(r20) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // addi r26,r26,-16
  r26 = uint64_t(int64_t(int64_t(r26) + -16));
  // addi r31,r31,-16
  r31 = uint64_t(int64_t(int64_t(r31) + -16));
  // addi r10,r10,80
  r10 = uint64_t(int64_t(int64_t(r10) + 80));
  // bne 0x82588264
  if (!cr0.eq) goto loc_82588264;
loc_82588380:
  // addic. r19,r19,-1
  ca = uint32_t(r19) > 0;
  r19 = uint64_t(int64_t(int64_t(r19) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // subf r26,r21,r26
  r26 = r26 - r21;
  // subf r31,r21,r31
  r31 = r31 - r21;
  // bne 0x82588248
  if (!cr0.eq) goto loc_82588248;
loc_82588390:
  // srawi r11,r5,2
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 2));
  // rlwinm r25,r25,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 2) & 0xFFFFFFFC;
  // addze r5,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r5 = uint64_t(int64_t(temp.s64));
  // srawi r11,r5,4
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0xF) != 0);
  r11 = uint64_t(int64_t(int32_t(r5) >> 4));
  // cmpwi cr6,r5,4
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 4, so);
  // mulli r17,r11,96
  r17 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(96))));
  // add r10,r17,r10
  r10 = r17 + r10;
  // bgt cr6,0x82588214
  if (cr6.gt) goto loc_82588214;
  // rlwinm r11,r25,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 4) & 0xFFFFFFF0;
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // add r11,r11,r3
  r11 = r11 + r3;
  // addi r10,r11,-16
  r10 = uint64_t(int64_t(int64_t(r11) + -16));
  // ble cr6,0x8258847c
  if (!cr6.gt) goto loc_8258847C;
  // subf r11,r3,r10
  r11 = r10 - r3;
  // subf r30,r4,r3
  r30 = r3 - r4;
  // add r11,r11,r4
  r11 = r11 + r4;
  // mr r9,r25
  r9 = r25;
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
loc_825883D8:
  // addi r8,r10,8
  r8 = uint64_t(int64_t(int64_t(r10) + 8));
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // addi r7,r10,12
  r7 = uint64_t(int64_t(int64_t(r10) + 12));
  // lfsx f12,r30,r11
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r11));
  f12 = double(temp.f32);
  // addi r6,r11,-4
  r6 = uint64_t(int64_t(int64_t(r11) + -4));
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r31,r11,8
  r31 = uint64_t(int64_t(int64_t(r11) + 8));
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f10,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f10 = double(temp.f32);
  // fadds f6,f11,f0
  f6 = double(float(f11 + f0));
  // fadds f5,f10,f12
  f5 = double(float(f10 + f12));
  // lfs f9,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f9 = double(temp.f32);
  // lfs f8,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f8 = double(temp.f32);
  // fsubs f0,f0,f11
  f0 = double(float(f0 - f11));
  // lfs f7,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f7 = double(temp.f32);
  // fadds f11,f8,f9
  f11 = double(float(f8 + f9));
  // fadds f4,f7,f13
  f4 = double(float(f7 + f13));
  // fsubs f13,f13,f7
  f13 = double(float(f13 - f7));
  // fsubs f12,f12,f10
  f12 = double(float(f12 - f10));
  // fsubs f9,f9,f8
  f9 = double(float(f9 - f8));
  // fadds f10,f5,f6
  f10 = double(float(f5 + f6));
  // stfs f10,0(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f10,f6,f5
  f10 = double(float(f6 - f5));
  // stfsx f10,r30,r11
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r30) + uint32_t(r11), temp.u32);
  // addi r10,r10,-16
  r10 = uint64_t(int64_t(int64_t(r10) + -16));
  // fsubs f10,f11,f4
  f10 = double(float(f11 - f4));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fadds f11,f4,f11
  f11 = double(float(f4 + f11));
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // fadds f11,f13,f0
  f11 = double(float(f13 + f0));
  // addi r11,r11,-16
  r11 = uint64_t(int64_t(int64_t(r11) + -16));
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fsubs f10,f9,f12
  f10 = double(float(f9 - f12));
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fadds f13,f9,f12
  f13 = double(float(f9 + f12));
  // stfs f10,0(r5)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // bne 0x825883d8
  if (!cr0.eq) goto loc_825883D8;
loc_8258847C:
  // rlwinm r6,r25,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 2) & 0xFFFFFFFC;
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // addi r5,r6,-1
  r5 = uint64_t(int64_t(int64_t(r6) + -1));
  // srawi r9,r6,1
  ca = (int32_t(r6) < 0) & ((uint32_t(r6) & 0x1) != 0);
  r9 = uint64_t(int64_t(int32_t(r6) >> 1));
  // cmpwi cr6,r5,1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 1, so);
  // ble cr6,0x825884ec
  if (!cr6.gt) goto loc_825884EC;
  // addi r10,r4,4
  r10 = uint64_t(int64_t(int64_t(r4) + 4));
  // subf r8,r4,r3
  r8 = r3 - r4;
loc_8258849C:
  // cmpw cr6,r7,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r9), so);
  // ble cr6,0x825884c8
  if (!cr6.gt) goto loc_825884C8;
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lfsx f13,r10,r8
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r8));
  f13 = double(temp.f32);
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // lfsx f12,r11,r3
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  f12 = double(temp.f32);
  // lfsx f11,r11,r4
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r4));
  f11 = double(temp.f32);
  // stfsx f12,r10,r8
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r8), temp.u32);
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // stfsx f13,r11,r3
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r3), temp.u32);
  // stfsx f0,r11,r4
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r4), temp.u32);
loc_825884C8:
  // mr r11,r6
  r11 = r6;
loc_825884CC:
  // srawi r11,r11,1
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 1));
  // xor r9,r11,r9
  r9 = r11 ^ r9;
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // blt cr6,0x825884cc
  if (cr6.lt) goto loc_825884CC;
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpw cr6,r7,r5
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r5), so);
  // blt cr6,0x8258849c
  if (cr6.lt) goto loc_8258849C;
loc_825884EC:
  // lfd f30,-144(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -144)));
  // lfd f31,-136(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -136)));
  // b 0x825a6eac
  // (left out: dead in C++)
  return f1;
}

// sub_82588750: lifted from generated/ by lift.py.
void f82588750(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82588750 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r4,1020
  r11 = uint64_t(int64_t(int64_t(r4) + 1020));
  // ori r9,r9,41600
  r9 = r9 | 41600;
  // li r10,255
  r10 = uint64_t(int64_t(255));
loc_82588760:
  // addi r9,r9,-162
  r9 = uint64_t(int64_t(int64_t(r9) + -162));
  // rlwinm r8,r9,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFFFFFF;
  // addi r8,r8,65
  r8 = uint64_t(int64_t(int64_t(r8) + 65));
  // mullw r8,r8,r10
  r8 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r10))));
  // addi r8,r8,209
  r8 = uint64_t(int64_t(int64_t(r8) + 209));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r8,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 24) & 0xFFFFFF;
  // cmplwi cr6,r10,86
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 86, so);
  // addi r8,r8,29
  r8 = uint64_t(int64_t(int64_t(r8) + 29));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r8,r3
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r3));
  f0 = double(temp.f32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bgt cr6,0x82588760
  if (cr6.gt) goto loc_82588760;
  // li r10,17789
  r10 = uint64_t(int64_t(17789));
  // li r9,87
  r9 = uint64_t(int64_t(87));
loc_825887A0:
  // addi r10,r10,-203
  r10 = uint64_t(int64_t(int64_t(r10) + -203));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r8,r10,26,6,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0x3FFFFFC;
  // lfsx f0,r8,r3
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r3));
  f0 = double(temp.f32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bne 0x825887a0
  if (!cr0.eq) goto loc_825887A0;
  // blr
  return;
}

// sub_82588908: lifted from generated/ by lift.py.
void f82588908(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82588908 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // rlwinm r11,r3,29,27,31
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 29) & 0x1F;
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f13,-16628(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -16628);
  f13 = double(temp.f32);
  // lfs f12,2608(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2608);
  f12 = double(temp.f32);
  // mr. r9,r11
  r9 = r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r11,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f0 = double(temp.f32);
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // fmadds f0,f0,f13,f12
  f0 = double(float(std::fma(f0, f13, f12)));
  // stfsx f0,r11,r10
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), temp.u32);
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lfsx f0,r10,r11
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  f0 = double(temp.f32);
  // ble 0x82588970
  if (!cr0.gt) goto loc_82588970;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r8,r9
  r8 = r9;
loc_8258894C:
  // lfsx f11,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f11 = double(temp.f32);
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // stfsx f11,r11,r10
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), temp.u32);
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lfsx f11,r11,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f11 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // bne 0x8258894c
  if (!cr0.eq) goto loc_8258894C;
loc_82588970:
  // addi r11,r9,1
  r11 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpwi cr6,r11,32
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 32, so);
  // bgt cr6,0x825889a8
  if (cr6.gt) goto loc_825889A8;
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_82588984:
  // lfsx f11,r11,r10
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f11 = double(temp.f32);
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // stfsx f11,r11,r10
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), temp.u32);
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lfsx f11,r11,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f11 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // cmpwi cr6,r11,128
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 128, so);
  // ble cr6,0x82588984
  if (!cr6.gt) goto loc_82588984;
loc_825889A8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // fdivs f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 / f0));
  // lfs f13,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f13,4(r4)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
loc_825889C0:
  // extsw r9,r11
  r9 = uint64_t(int64_t(int32_t(r11)));
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // lfs f12,4(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f12 = double(temp.f32);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,32
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 32, so);
  // std r9,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r9);
  // lfd f11,-16(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f13,f11,f13
  f13 = double(float(f11 * f13));
  // fmadds f13,f13,f0,f12
  f13 = double(float(std::fma(f13, f0, f12)));
  // stfs f13,4(r4)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // ble cr6,0x825889c0
  if (!cr6.gt) goto loc_825889C0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,3124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3124);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,4(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // sth r11,8(r4)
  REX_STORE_U16(uint32_t(r4) + 8, uint16_t(r11));
  // blr
  return;
}

// sub_82588A18: lifted from generated/ by lift.py.
void f82588A18(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82588A18 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lha r11,8(r3)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r3) + 8))));
  // cmpwi cr6,r11,84
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 84, so);
  // bgt cr6,0x82588a58
  if (cr6.gt) goto loc_82588A58;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lfs f0,-16624(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -16624);
  f0 = double(temp.f32);
loc_82588A30:
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsh r11,r8
  r11 = uint64_t(int64_t(int16_t(r8)));
  // lfsx f13,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  f13 = double(temp.f32);
  // cmpwi cr6,r11,32
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 32, so);
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfsx f13,r10,r9
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r9), temp.u32);
  // ble cr6,0x82588a30
  if (!cr6.gt) goto loc_82588A30;
  // b 0x82588a60
  goto loc_82588A60;
loc_82588A58:
  // li r11,84
  r11 = uint64_t(int64_t(84));
  // sth r11,8(r3)
  REX_STORE_U16(uint32_t(r3) + 8, uint16_t(r11));
loc_82588A60:
  // lhz r11,8(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 8);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // sth r11,8(r3)
  REX_STORE_U16(uint32_t(r3) + 8, uint16_t(r11));
  // blr
  return;
}

// sub_82588A70: lifted from generated/ by lift.py.
void f82588A70(uint8_t* base, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82588A70 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // lfs f13,-16596(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16596);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82588aa4
  if (!cr6.gt) goto loc_82588AA4;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,-16600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16600);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f12,-16604(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16604);
  f12 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fnmsubs f12,f0,f13,f12
  f12 = double(float(-std::fma(f0, f13, -f12)));
  // lfs f13,-16608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16608);
  f13 = double(temp.f32);
  // b 0x82588ac0
  goto loc_82588AC0;
loc_82588AA4:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,-16612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16612);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f12,-16616(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16616);
  f12 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fnmsubs f12,f0,f13,f12
  f12 = double(float(-std::fma(f0, f13, -f12)));
  // lfs f13,-16620(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16620);
  f13 = double(temp.f32);
loc_82588AC0:
  // fmadds f0,f12,f0,f13
  lifted::ScalarFloatMode();
  f0 = double(float(std::fma(f12, f0, f13)));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stfs f0,4(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x82588ad8
  if (cr6.gt) goto loc_82588AD8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82588AD8:
  // stb r11,19(r4)
  REX_STORE_U8(uint32_t(r4) + 19, uint8_t(r11));
  // blr
  return;
}

// sub_82588AE0: lifted from generated/ by lift.py.
void f82588AE0(uint8_t* base, uint64_t r1, uint64_t r5, uint64_t r6, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_82588AE0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lbz r11,19(r6)
  r11 = REX_LOAD_U8(uint32_t(r6) + 19);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82588b54
  if (cr6.eq) goto loc_82588B54;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,-16588(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16588);
  f0 = double(temp.f32);
  // extsh r11,r5
  r11 = uint64_t(int64_t(int16_t(r5)));
  // fsubs f0,f2,f0
  f0 = double(float(f2 - f0));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfd f13,-16(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f12,f13
  f12 = double(float(f13));
  // lfs f13,-16592(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16592);
  f13 = double(temp.f32);
  // fsubs f13,f12,f13
  f13 = double(float(f12 - f13));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82588b24
  if (!cr6.lt) goto loc_82588B24;
  // fmr f0,f13
  f0 = f13;
loc_82588B24:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x82588b54
  if (cr6.lt) goto loc_82588B54;
  // lhz r11,14(r6)
  r11 = REX_LOAD_U16(uint32_t(r6) + 14);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // sth r11,14(r6)
  REX_STORE_U16(uint32_t(r6) + 14, uint16_t(r11));
  // ble cr6,0x82588b4c
  if (!cr6.gt) goto loc_82588B4C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x82588b58
  goto loc_82588B58;
loc_82588B4C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x82588b5c
  goto loc_82588B5C;
loc_82588B54:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82588B58:
  // sth r11,14(r6)
  REX_STORE_U16(uint32_t(r6) + 14, uint16_t(r11));
loc_82588B5C:
  // stb r11,20(r6)
  REX_STORE_U8(uint32_t(r6) + 20, uint8_t(r11));
  // blr
  return;
}

// sub_82588B68: lifted from generated/ by lift.py.
void f82588B68(uint8_t* base, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82588B68 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f13 = double(temp.f32);
  // lfs f0,-16596(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16596);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x82588ba4
  if (!cr6.lt) goto loc_82588BA4;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,-16564(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16564);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f12,-16568(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16568);
  f12 = double(temp.f32);
  // fadds f12,f13,f12
  f12 = double(float(f13 + f12));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x82588bcc
  if (!cr6.lt) goto loc_82588BCC;
  // fmr f0,f12
  f0 = f12;
  // b 0x82588bcc
  goto loc_82588BCC;
loc_82588BA4:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,-16572(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16572);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x82588bc0
  if (!cr6.lt) goto loc_82588BC0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,-16576(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16576);
  f0 = double(temp.f32);
  // b 0x82588bc8
  goto loc_82588BC8;
loc_82588BC0:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,-16580(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16580);
  f0 = double(temp.f32);
loc_82588BC8:
  // fmuls f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 * f0));
loc_82588BCC:
  // fcmpu cr6,f1,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f13);
  // bge cr6,0x82588cc0
  if (!cr6.lt) goto loc_82588CC0;
  // lbz r11,18(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 18);
  // extsb. r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f9,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f9 = double(temp.f32);
  // bgt 0x82588c10
  if (cr0.gt) goto loc_82588C10;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f12,f1,f9
  f12 = double(float(f1 + f9));
  // lfs f11,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f11,f13,f11
  f11 = double(float(f13 * f11));
  // lfs f10,10796(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10796);
  f10 = double(temp.f32);
  // fmadds f8,f12,f10,f11
  f8 = double(float(std::fma(f12, f10, f11)));
  // fcmpu cr6,f8,f0
  cr6 = lifted::CompareFloat(f8, f0);
  // bgt cr6,0x82588c10
  if (cr6.gt) goto loc_82588C10;
  // fmadds f0,f12,f10,f11
  f0 = double(float(std::fma(f12, f10, f11)));
loc_82588C10:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,10712(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10712);
  f12 = double(temp.f32);
  // fmuls f12,f13,f12
  f12 = double(float(f13 * f12));
  // fcmpu cr6,f1,f12
  cr6 = lifted::CompareFloat(f1, f12);
  // bgt cr6,0x82588c7c
  if (cr6.gt) goto loc_82588C7C;
  // fcmpu cr6,f1,f9
  cr6 = lifted::CompareFloat(f1, f9);
  // bge cr6,0x82588c58
  if (!cr6.lt) goto loc_82588C58;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f12,f1,f9
  f12 = double(float(f1 + f9));
  // lfs f11,10756(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10756);
  f11 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // lfs f11,11344(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11344);
  f11 = double(temp.f32);
  // fmadds f10,f12,f11,f13
  f10 = double(float(std::fma(f12, f11, f13)));
  // fcmpu cr6,f10,f0
  cr6 = lifted::CompareFloat(f10, f0);
  // bgt cr6,0x82588cf0
  if (cr6.gt) goto loc_82588CF0;
  // fmadds f0,f12,f11,f13
  f0 = double(float(std::fma(f12, f11, f13)));
  // b 0x82588cf0
  goto loc_82588CF0;
loc_82588C58:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,10772(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10772);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,10768(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10768);
  f11 = double(temp.f32);
loc_82588C68:
  // fmuls f13,f13,f11
  lifted::ScalarFloatMode();
  f13 = double(float(f13 * f11));
  // fmadds f13,f1,f12,f13
  f13 = double(float(std::fma(f1, f12, f13)));
loc_82588C70:
  // fcmpu cr6,f13,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82588cf0
  if (cr6.gt) goto loc_82588CF0;
  // b 0x82588cec
  goto loc_82588CEC;
loc_82588C7C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,8748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f12 = double(temp.f32);
  // fmuls f12,f13,f12
  f12 = double(float(f13 * f12));
  // fcmpu cr6,f1,f12
  cr6 = lifted::CompareFloat(f1, f12);
  // bgt cr6,0x82588ca4
  if (cr6.gt) goto loc_82588CA4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,10812(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,10604(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10604);
  f11 = double(temp.f32);
  // b 0x82588c68
  goto loc_82588C68;
loc_82588CA4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,8712(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8712);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,4208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4208);
  f11 = double(temp.f32);
  // fmuls f11,f1,f11
  f11 = double(float(f1 * f11));
  // fmadds f13,f13,f12,f11
  f13 = double(float(std::fma(f13, f12, f11)));
  // b 0x82588c70
  goto loc_82588C70;
loc_82588CC0:
  // lbz r11,19(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 19);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82588cf0
  if (!cr6.eq) goto loc_82588CF0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,10588(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10588);
  f12 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f11,10600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10600);
  f11 = double(temp.f32);
  // fmuls f11,f1,f11
  f11 = double(float(f1 * f11));
  // fmadds f13,f13,f12,f11
  f13 = double(float(std::fma(f13, f12, f11)));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82588cf0
  if (cr6.lt) goto loc_82588CF0;
loc_82588CEC:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_82588CF0:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f13,-16584(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -16584);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82588d04
  if (!cr6.gt) goto loc_82588D04;
  // fmr f0,f13
  f0 = f13;
loc_82588D04:
  // lbz r11,18(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 18);
  // extsb. r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bgt 0x82588d18
  if (cr0.gt) goto loc_82588D18;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,18(r4)
  REX_STORE_U8(uint32_t(r4) + 18, uint8_t(r11));
loc_82588D18:
  // stfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // blr
  return;
}

namespace {

lifted::Result e8257CF10(uint8_t* base, const lifted::Args& a) {
  return {f8257CF10(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257CFA8(uint8_t* base, const lifted::Args& a) {
  return f8257CFA8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257D020(uint8_t* base, const lifted::Args& a) {
  return f8257D020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8257D298(uint8_t* base, const lifted::Args& a) {
  return f8257D298(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8257D460(uint8_t* base, const lifted::Args& a) {
  f8257D460(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8257D518(uint8_t* base, const lifted::Args& a) {
  return f8257D518(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257D6D8(uint8_t* base, const lifted::Args& a) {
  f8257D6D8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8257E8E8(uint8_t* base, const lifted::Args& a) {
  return f8257E8E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e8257E9B8(uint8_t* base, const lifted::Args& a) {
  return f8257E9B8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8257EAF0(uint8_t* base, const lifted::Args& a) {
  return f8257EAF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257EDE8(uint8_t* base, const lifted::Args& a) {
  return f8257EDE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257EF80(uint8_t* base, const lifted::Args& a) {
  return f8257EF80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257EFF8(uint8_t* base, const lifted::Args& a) {
  return f8257EFF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257F070(uint8_t* base, const lifted::Args& a) {
  return f8257F070(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8257F198(uint8_t* base, const lifted::Args& a) {
  return f8257F198(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8257F1E8(uint8_t* base, const lifted::Args& a) {
  return f8257F1E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e8257F970(uint8_t* base, const lifted::Args& a) {
  return f8257F970(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8257FA90(uint8_t* base, const lifted::Args& a) {
  return f8257FA90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8257FB38(uint8_t* base, const lifted::Args& a) {
  return f8257FB38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257FBF8(uint8_t* base, const lifted::Args& a) {
  return f8257FBF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257FC00(uint8_t* base, const lifted::Args& a) {
  return f8257FC00(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8257FD18(uint8_t* base, const lifted::Args& a) {
  return f8257FD18(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8257FD48(uint8_t* base, const lifted::Args& a) {
  return f8257FD48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8257FEB8(uint8_t* base, const lifted::Args& a) {
  return f8257FEB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8257FF50(uint8_t* base, const lifted::Args& a) {
  return {f8257FF50(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257FF68(uint8_t* base, const lifted::Args& a) {
  return f8257FF68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82580020(uint8_t* base, const lifted::Args& a) {
  return f82580020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82580028(uint8_t* base, const lifted::Args& a) {
  return f82580028(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82580170(uint8_t* base, const lifted::Args& a) {
  return f82580170(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82580178(uint8_t* base, const lifted::Args& a) {
  return {f82580178(base, a.r[0]), a.f[0]};
}

lifted::Result e82580188(uint8_t* base, const lifted::Args& a) {
  return {f82580188(base, a.r[0]), a.f[0]};
}

lifted::Result e82580198(uint8_t* base, const lifted::Args& a) {
  return {f82580198(base, a.r[0]), a.f[0]};
}

lifted::Result e825801A8(uint8_t* base, const lifted::Args& a) {
  return {f825801A8(base, a.r[0]), a.f[0]};
}

lifted::Result e825801B8(uint8_t* base, const lifted::Args& a) {
  return {f825801B8(base, a.r[0]), a.f[0]};
}

lifted::Result e825801C8(uint8_t* base, const lifted::Args& a) {
  return {f825801C8(base, a.r[0]), a.f[0]};
}

lifted::Result e825820E8(uint8_t* base, const lifted::Args& a) {
  return {f825820E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e82582158(uint8_t* base, const lifted::Args& a) {
  return f82582158(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e825821F0(uint8_t* base, const lifted::Args& a) {
  return f825821F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82582320(uint8_t* base, const lifted::Args& a) {
  return f82582320(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825823B0(uint8_t* base, const lifted::Args& a) {
  return f825823B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e825828D0(uint8_t* base, const lifted::Args& a) {
  return f825828D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82582AD8(uint8_t* base, const lifted::Args& a) {
  return f82582AD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e82582EC0(uint8_t* base, const lifted::Args& a) {
  return f82582EC0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82583090(uint8_t* base, const lifted::Args& a) {
  return f82583090(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825834F8(uint8_t* base, const lifted::Args& a) {
  f825834F8(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825835F8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825835F8(base, a.r[0], a.r[1], a.r[2])};
}

lifted::Result e82583610(uint8_t* base, const lifted::Args& a) {
  return {f82583610(base, a.r1, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82583BA8(uint8_t* base, const lifted::Args& a) {
  return {f82583BA8(base, a.f[0]), a.f[0]};
}

lifted::Result e82583C98(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82583C98(base, a.r1, a.r13, a.r[0])};
}

lifted::Result e82583CE8(uint8_t* base, const lifted::Args& a) {
  return {f82583CE8(base, a.f[0], a.f[1], a.f[2]), a.f[0]};
}

lifted::Result e82583D68(uint8_t* base, const lifted::Args& a) {
  return f82583D68(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e82583FD0(uint8_t* base, const lifted::Args& a) {
  return f82583FD0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82584080(uint8_t* base, const lifted::Args& a) {
  return f82584080(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825840D0(uint8_t* base, const lifted::Args& a) {
  return f825840D0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82584178(uint8_t* base, const lifted::Args& a) {
  return f82584178(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825841E8(uint8_t* base, const lifted::Args& a) {
  return {f825841E8(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82584758(uint8_t* base, const lifted::Args& a) {
  return f82584758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e82584970(uint8_t* base, const lifted::Args& a) {
  f82584970(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82584988(uint8_t* base, const lifted::Args& a) {
  return f82584988(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82584A00(uint8_t* base, const lifted::Args& a) {
  return f82584A00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82584A70(uint8_t* base, const lifted::Args& a) {
  return f82584A70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e82584B90(uint8_t* base, const lifted::Args& a) {
  return f82584B90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e82584C70(uint8_t* base, const lifted::Args& a) {
  return f82584C70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82584CB0(uint8_t* base, const lifted::Args& a) {
  return f82584CB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82584CF0(uint8_t* base, const lifted::Args& a) {
  return f82584CF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82584E48(uint8_t* base, const lifted::Args& a) {
  return f82584E48(base, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e82584E78(uint8_t* base, const lifted::Args& a) {
  return {f82584E78(base, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e82584EB0(uint8_t* base, const lifted::Args& a) {
  return {f82584EB0(base, a.r[0], a.r[1], a.r[3], a.f[0]), a.f[0]};
}

lifted::Result e82584ED8(uint8_t* base, const lifted::Args& a) {
  f82584ED8(base, a.r1, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825850E0(uint8_t* base, const lifted::Args& a) {
  return f825850E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e825852F0(uint8_t* base, const lifted::Args& a) {
  return {f825852F0(base, a.f[0]), a.f[0]};
}

lifted::Result e82585398(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82585398(base, a.r1, a.f[0])};
}

lifted::Result e825853F8(uint8_t* base, const lifted::Args& a) {
  f825853F8(base, a.r[0], a.r[1], a.r[3], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825854E0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825854E0(base, a.f[0], a.f[1], a.f[2], a.f[3], a.f[4])};
}

lifted::Result e825858A0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825858A0(base, a.f[0], a.f[1], a.f[2], a.f[3])};
}

lifted::Result e82585D30(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82585D30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0])};
}

lifted::Result e82585D78(uint8_t* base, const lifted::Args& a) {
  return {f82585D78(base, a.r1, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e82586218(uint8_t* base, const lifted::Args& a) {
  f82586218(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82586288(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82586288(base, a.r[3], a.f[0], a.f[1], a.f[2])};
}

lifted::Result e825862D8(uint8_t* base, const lifted::Args& a) {
  return {f825862D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]), a.f[0]};
}

lifted::Result e82586428(uint8_t* base, const lifted::Args& a) {
  return {f82586428(base, a.r[0], a.r[1], a.r[2], a.f[0]), a.f[0]};
}

lifted::Result e82586460(uint8_t* base, const lifted::Args& a) {
  return {f82586460(base, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e82586758(uint8_t* base, const lifted::Args& a) {
  return f82586758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825868E8(uint8_t* base, const lifted::Args& a) {
  return {f825868E8(base, a.r1, a.r[0], a.r[1], a.r[2], a.r[4], a.f[0]), a.f[0]};
}

lifted::Result e82586B68(uint8_t* base, const lifted::Args& a) {
  return f82586B68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e82587088(uint8_t* base, const lifted::Args& a) {
  f82587088(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82587658(uint8_t* base, const lifted::Args& a) {
  return f82587658(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825876D8(uint8_t* base, const lifted::Args& a) {
  return f825876D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82587960(uint8_t* base, const lifted::Args& a) {
  return {f82587960(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82587A88(uint8_t* base, const lifted::Args& a) {
  return {f82587A88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e82587BC8(uint8_t* base, const lifted::Args& a) {
  return f82587BC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1]);
}

lifted::Result e82587F98(uint8_t* base, const lifted::Args& a) {
  return {f82587F98(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825880B8(uint8_t* base, const lifted::Args& a) {
  f825880B8(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825881E0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825881E0(base, a.r1, a.r[0], a.r[1], a.r[2], a.f[0])};
}

lifted::Result e82588750(uint8_t* base, const lifted::Args& a) {
  f82588750(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82588908(uint8_t* base, const lifted::Args& a) {
  f82588908(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82588A18(uint8_t* base, const lifted::Args& a) {
  f82588A18(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82588A70(uint8_t* base, const lifted::Args& a) {
  f82588A70(base, a.r[1], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82588AE0(uint8_t* base, const lifted::Args& a) {
  f82588AE0(base, a.r1, a.r[2], a.r[3], a.f[0], a.f[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82588B68(uint8_t* base, const lifted::Args& a) {
  f82588B68(base, a.r[1], a.f[0]);
  return {a.r[0], a.f[0]};
}

const Registration kEntries[] = {
    {0x8257CF10, e8257CF10},
    {0x8257CFA8, e8257CFA8},
    {0x8257D020, e8257D020},
    {0x8257D298, e8257D298},
    {0x8257D460, e8257D460},
    {0x8257D518, e8257D518},
    {0x8257D6D8, e8257D6D8},
    {0x8257E8E8, e8257E8E8},
    {0x8257E9B8, e8257E9B8},
    {0x8257EAF0, e8257EAF0},
    {0x8257EDE8, e8257EDE8},
    {0x8257EF80, e8257EF80},
    {0x8257EFF8, e8257EFF8},
    {0x8257F070, e8257F070},
    {0x8257F198, e8257F198},
    {0x8257F1E8, e8257F1E8},
    {0x8257F970, e8257F970},
    {0x8257FA90, e8257FA90},
    {0x8257FB38, e8257FB38},
    {0x8257FBF8, e8257FBF8},
    {0x8257FC00, e8257FC00},
    {0x8257FD18, e8257FD18},
    {0x8257FD48, e8257FD48},
    {0x8257FEB8, e8257FEB8},
    {0x8257FF50, e8257FF50},
    {0x8257FF68, e8257FF68},
    {0x82580020, e82580020},
    {0x82580028, e82580028},
    {0x82580170, e82580170},
    {0x82580178, e82580178},
    {0x82580188, e82580188},
    {0x82580198, e82580198},
    {0x825801A8, e825801A8},
    {0x825801B8, e825801B8},
    {0x825801C8, e825801C8},
    {0x825820E8, e825820E8},
    {0x82582158, e82582158},
    {0x825821F0, e825821F0},
    {0x82582320, e82582320},
    {0x825823B0, e825823B0},
    {0x825828D0, e825828D0},
    {0x82582AD8, e82582AD8},
    {0x82582EC0, e82582EC0},
    {0x82583090, e82583090},
    {0x825834F8, e825834F8},
    {0x825835F8, e825835F8},
    {0x82583610, e82583610},
    {0x82583BA8, e82583BA8},
    {0x82583C98, e82583C98},
    {0x82583CE8, e82583CE8},
    {0x82583D68, e82583D68},
    {0x82583FD0, e82583FD0},
    {0x82584080, e82584080},
    {0x825840D0, e825840D0},
    {0x82584178, e82584178},
    {0x825841E8, e825841E8},
    {0x82584758, e82584758},
    {0x82584970, e82584970},
    {0x82584988, e82584988},
    {0x82584A00, e82584A00},
    {0x82584A70, e82584A70},
    {0x82584B90, e82584B90},
    {0x82584C70, e82584C70},
    {0x82584CB0, e82584CB0},
    {0x82584CF0, e82584CF0},
    {0x82584E48, e82584E48},
    {0x82584E78, e82584E78},
    {0x82584EB0, e82584EB0},
    {0x82584ED8, e82584ED8},
    {0x825850E0, e825850E0},
    {0x825852F0, e825852F0},
    {0x82585398, e82585398},
    {0x825853F8, e825853F8},
    {0x825854E0, e825854E0},
    {0x825858A0, e825858A0},
    {0x82585D30, e82585D30},
    {0x82585D78, e82585D78},
    {0x82586218, e82586218},
    {0x82586288, e82586288},
    {0x825862D8, e825862D8},
    {0x82586428, e82586428},
    {0x82586460, e82586460},
    {0x82586758, e82586758},
    {0x825868E8, e825868E8},
    {0x82586B68, e82586B68},
    {0x82587088, e82587088},
    {0x82587658, e82587658},
    {0x825876D8, e825876D8},
    {0x82587960, e82587960},
    {0x82587A88, e82587A88},
    {0x82587BC8, e82587BC8},
    {0x82587F98, e82587F98},
    {0x825880B8, e825880B8},
    {0x825881E0, e825881E0},
    {0x82588750, e82588750},
    {0x82588908, e82588908},
    {0x82588A18, e82588A18},
    {0x82588A70, e82588A70},
    {0x82588AE0, e82588AE0},
    {0x82588B68, e82588B68},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_019)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_8257CF10) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257CF10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CF10);
}

extern "C" REX_FUNC(sub_8257CFA8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257CFA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CFA8);
}

extern "C" REX_FUNC(sub_8257D020) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257D020(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D020);
}

extern "C" REX_FUNC(sub_8257D298) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257D298(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D298);
}

extern "C" REX_FUNC(sub_8257D460) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257D460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D460);
}

extern "C" REX_FUNC(sub_8257D518) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257D518(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D518);
}

extern "C" REX_FUNC(sub_8257D6D8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257D6D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257D6D8);
}

extern "C" REX_FUNC(sub_8257E8E8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257E8E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257E8E8);
}

extern "C" REX_FUNC(sub_8257E9B8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257E9B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257E9B8);
}

extern "C" REX_FUNC(sub_8257EAF0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257EAF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257EAF0);
}

extern "C" REX_FUNC(sub_8257EDE8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257EDE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257EDE8);
}

extern "C" REX_FUNC(sub_8257EF80) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257EF80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257EF80);
}

extern "C" REX_FUNC(sub_8257EFF8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257EFF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257EFF8);
}

extern "C" REX_FUNC(sub_8257F070) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257F070(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257F070);
}

extern "C" REX_FUNC(sub_8257F198) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257F198(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257F198);
}

extern "C" REX_FUNC(sub_8257F1E8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257F1E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257F1E8);
}

extern "C" REX_FUNC(sub_8257F970) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257F970(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257F970);
}

extern "C" REX_FUNC(sub_8257FA90) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FA90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FA90);
}

extern "C" REX_FUNC(sub_8257FB38) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FB38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FB38);
}

extern "C" REX_FUNC(sub_8257FBF8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FBF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FBF8);
}

extern "C" REX_FUNC(sub_8257FC00) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FC00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FC00);
}

extern "C" REX_FUNC(sub_8257FD18) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FD18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FD18);
}

extern "C" REX_FUNC(sub_8257FD48) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FD48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FD48);
}

extern "C" REX_FUNC(sub_8257FEB8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FEB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FEB8);
}

extern "C" REX_FUNC(sub_8257FF50) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FF50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FF50);
}

extern "C" REX_FUNC(sub_8257FF68) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_8257FF68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257FF68);
}

extern "C" REX_FUNC(sub_82580020) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580020(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580020);
}

extern "C" REX_FUNC(sub_82580028) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580028(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580028);
}

extern "C" REX_FUNC(sub_82580170) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580170(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580170);
}

extern "C" REX_FUNC(sub_82580178) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580178);
}

extern "C" REX_FUNC(sub_82580188) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580188(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580188);
}

extern "C" REX_FUNC(sub_82580198) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82580198(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82580198);
}

extern "C" REX_FUNC(sub_825801A8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825801A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825801A8);
}

extern "C" REX_FUNC(sub_825801B8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825801B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825801B8);
}

extern "C" REX_FUNC(sub_825801C8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825801C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825801C8);
}

extern "C" REX_FUNC(sub_825820E8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825820E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825820E8);
}

extern "C" REX_FUNC(sub_82582158) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82582158(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82582158);
}

extern "C" REX_FUNC(sub_825821F0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825821F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825821F0);
}

extern "C" REX_FUNC(sub_82582320) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82582320(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82582320);
}

extern "C" REX_FUNC(sub_825823B0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825823B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825823B0);
}

extern "C" REX_FUNC(sub_825828D0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825828D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825828D0);
}

extern "C" REX_FUNC(sub_82582AD8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82582AD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82582AD8);
}

extern "C" REX_FUNC(sub_82582EC0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82582EC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82582EC0);
}

extern "C" REX_FUNC(sub_82583090) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583090(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583090);
}

extern "C" REX_FUNC(sub_825834F8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825834F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825834F8);
}

extern "C" REX_FUNC(sub_825835F8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825835F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825835F8);
}

extern "C" REX_FUNC(sub_82583610) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583610(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583610);
}

extern "C" REX_FUNC(sub_82583BA8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583BA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583BA8);
}

extern "C" REX_FUNC(sub_82583C98) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583C98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583C98);
}

extern "C" REX_FUNC(sub_82583CE8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583CE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583CE8);
}

extern "C" REX_FUNC(sub_82583D68) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583D68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583D68);
}

extern "C" REX_FUNC(sub_82583FD0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82583FD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82583FD0);
}

extern "C" REX_FUNC(sub_82584080) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584080(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584080);
}

extern "C" REX_FUNC(sub_825840D0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825840D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825840D0);
}

extern "C" REX_FUNC(sub_82584178) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584178);
}

extern "C" REX_FUNC(sub_825841E8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825841E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825841E8);
}

extern "C" REX_FUNC(sub_82584758) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584758(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584758);
}

extern "C" REX_FUNC(sub_82584970) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584970(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584970);
}

extern "C" REX_FUNC(sub_82584988) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584988(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584988);
}

extern "C" REX_FUNC(sub_82584A00) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584A00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584A00);
}

extern "C" REX_FUNC(sub_82584A70) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584A70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584A70);
}

extern "C" REX_FUNC(sub_82584B90) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584B90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584B90);
}

extern "C" REX_FUNC(sub_82584C70) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584C70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584C70);
}

extern "C" REX_FUNC(sub_82584CB0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584CB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584CB0);
}

extern "C" REX_FUNC(sub_82584CF0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584CF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584CF0);
}

extern "C" REX_FUNC(sub_82584E48) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584E48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584E48);
}

extern "C" REX_FUNC(sub_82584E78) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584E78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584E78);
}

extern "C" REX_FUNC(sub_82584EB0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584EB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584EB0);
}

extern "C" REX_FUNC(sub_82584ED8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82584ED8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82584ED8);
}

extern "C" REX_FUNC(sub_825850E0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825850E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825850E0);
}

extern "C" REX_FUNC(sub_825852F0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825852F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825852F0);
}

extern "C" REX_FUNC(sub_82585398) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82585398(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82585398);
}

extern "C" REX_FUNC(sub_825853F8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825853F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825853F8);
}

extern "C" REX_FUNC(sub_825854E0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825854E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825854E0);
}

extern "C" REX_FUNC(sub_825858A0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825858A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825858A0);
}

extern "C" REX_FUNC(sub_82585D30) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82585D30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82585D30);
}

extern "C" REX_FUNC(sub_82585D78) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82585D78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82585D78);
}

extern "C" REX_FUNC(sub_82586218) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586218(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586218);
}

extern "C" REX_FUNC(sub_82586288) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586288(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586288);
}

extern "C" REX_FUNC(sub_825862D8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825862D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825862D8);
}

extern "C" REX_FUNC(sub_82586428) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586428(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586428);
}

extern "C" REX_FUNC(sub_82586460) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586460);
}

extern "C" REX_FUNC(sub_82586758) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586758(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586758);
}

extern "C" REX_FUNC(sub_825868E8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825868E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825868E8);
}

extern "C" REX_FUNC(sub_82586B68) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82586B68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82586B68);
}

extern "C" REX_FUNC(sub_82587088) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587088(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587088);
}

extern "C" REX_FUNC(sub_82587658) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587658(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587658);
}

extern "C" REX_FUNC(sub_825876D8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825876D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825876D8);
}

extern "C" REX_FUNC(sub_82587960) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587960(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587960);
}

extern "C" REX_FUNC(sub_82587A88) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587A88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587A88);
}

extern "C" REX_FUNC(sub_82587BC8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587BC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587BC8);
}

extern "C" REX_FUNC(sub_82587F98) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82587F98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82587F98);
}

extern "C" REX_FUNC(sub_825880B8) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825880B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825880B8);
}

extern "C" REX_FUNC(sub_825881E0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_825881E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825881E0);
}

extern "C" REX_FUNC(sub_82588750) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588750(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588750);
}

extern "C" REX_FUNC(sub_82588908) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588908(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588908);
}

extern "C" REX_FUNC(sub_82588A18) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588A18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588A18);
}

extern "C" REX_FUNC(sub_82588A70) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588A70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588A70);
}

extern "C" REX_FUNC(sub_82588AE0) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588AE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588AE0);
}

extern "C" REX_FUNC(sub_82588B68) {
  if (!ITC_FLAG(native_lifted_019)) {
    __imp__sub_82588B68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588B68);
}
