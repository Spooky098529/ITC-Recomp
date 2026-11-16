// Lift sweep batch 016 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_016, true, "ITC",
              "Run lift sweep batch 016's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_8245FE88: lifted from generated/ by lift.py.
lifted::Result f8245FE88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8245FE88 (lifted)");
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
  // mr r28,r3
  r28 = r3;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8245FE9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8245fec4
  if (cr6.eq) goto loc_8245FEC4;
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r8), so);
  // beq cr6,0x8245fedc
  if (cr6.eq) goto loc_8245FEDC;
loc_8245FEC4:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8245FECC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r30
  r8 = r30;
  // stw r8,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r8));
  // stb r29,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r29));
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8245FEDC:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r28,24
  r11 = uint64_t(int64_t(int64_t(r28) + 24));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x8245ff18
  if (cr6.eq) goto loc_8245FF18;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r11,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r11));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8245FF18:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8245ff58
  if (cr6.eq) goto loc_8245FF58;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8245ff58
  if (!cr6.eq) goto loc_8245FF58;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8245ff58
  if (!cr6.eq) goto loc_8245FF58;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8245FF50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8245FF58, r31)); r3 = o.r3; f1 = o.f1; }
loc_8245FF58:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_824601F8: lifted from generated/ by lift.py.
void f824601F8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_824601F8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r4,3
  r11 = uint64_t(int64_t(int64_t(r4) + 3));
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // rlwinm r11,r11,0,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFC;
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,16(r3)
  REX_STORE_U32(uint32_t(r3) + 16, uint32_t(r11));
  // blr
  return;
}

// sub_82460210: lifted from generated/ by lift.py.
lifted::Result f82460210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82460210 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r5,r4
  r5 = r4;
  // lwz r4,16(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x8246024c
  if (!cr6.eq) goto loc_8246024C;
loc_82460234:
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
loc_8246024C:
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // stw r5,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r5));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // bl 0x8245ef80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF80, LIFTED_ARGS(0x8246025C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r3));
  // bne cr6,0x82460234
  if (!cr6.eq) goto loc_82460234;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
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

// sub_82460368: lifted from generated/ by lift.py.
lifted::Result f82460368(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82460368 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82460410
  { const lifted::Result o = f82460410(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82460398: lifted from generated/ by lift.py.
lifted::Result f82460398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82460398 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82460470
  { const lifted::Result o = f82460470(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82460410: lifted from generated/ by lift.py.
lifted::Result f82460410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82460410 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x824602f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x824602F8, LIFTED_ARGS(0x82460430, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82460450
  if (cr6.eq) goto loc_82460450;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x82460450, r31)); r3 = o.r3; f1 = o.f1; }
loc_82460450:
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

// sub_82460470: lifted from generated/ by lift.py.
lifted::Result f82460470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82460470 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-22188
  r11 = uint64_t(int64_t(int64_t(r11) + -22188));
  // addi r10,r10,-22164
  r10 = uint64_t(int64_t(int64_t(r10) + -22164));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x824602f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x824602F8, LIFTED_ARGS(0x824604A8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x824604c8
  if (cr6.eq) goto loc_824604C8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x824604C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_824604C8:
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

// sub_82460538: lifted from generated/ by lift.py.
lifted::Result f82460538(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82460538 (lifted)");
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
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // bl 0x8245ef80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF80, LIFTED_ARGS(0x82460560, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x824605c8
  if (cr6.eq) goto loc_824605C8;
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r9,r10,-22856
  r9 = uint64_t(int64_t(int64_t(r10) + -22856));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // addi r8,r10,-22188
  r8 = uint64_t(int64_t(int64_t(r10) + -22188));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // stw r6,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r6));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // addi r7,r10,-22164
  r7 = uint64_t(int64_t(int64_t(r10) + -22164));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // mr r3,r10
  r3 = r10;
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
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
loc_824605C8:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
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

// sub_82460E90: lifted from generated/ by lift.py.
lifted::Result f82460E90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82460E90 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x82460b50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82460B50, LIFTED_ARGS(0x82460EB0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82460ed0
  if (cr6.eq) goto loc_82460ED0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x82460ED0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82460ED0:
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

// sub_824613A0: lifted from generated/ by lift.py.
lifted::Result f824613A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_824613A0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r11,r4
  r11 = r4;
  // lbz r10,60(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 60);
  // mr r4,r5
  r4 = r5;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // mr r5,r6
  r5 = r6;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x824613d8
  if (cr6.lt) goto loc_824613D8;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // ori r3,r3,87
  r3 = r3 | 87;
  // b 0x824613e8
  goto loc_824613E8;
loc_824613D8:
  // lwz r10,36(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 36);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r11,r10
  r11 = r11 + r10;
loc_824613E8:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82461408
  if (cr6.lt) goto loc_82461408;
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r6,r7
  r6 = r7;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82461408, r31)); r3 = o.r3; f1 = o.f1; }
loc_82461408:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82461490: lifted from generated/ by lift.py.
uint64_t f82461490(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82461490 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r10,60(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 60);
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x824614ac
  if (cr6.lt) goto loc_824614AC;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return r3;
loc_824614AC:
  // lwz r10,36(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 36);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r11,r10
  r11 = r11 + r10;
  // lbz r11,5(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 5);
  // stb r11,0(r5)
  REX_STORE_U8(uint32_t(r5) + 0, uint8_t(r11));
  // blr
  return r3;
}

// sub_824614C8: lifted from generated/ by lift.py.
uint64_t f824614C8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_824614C8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lbz r10,60(r3)
  r10 = REX_LOAD_U8(uint32_t(r3) + 60);
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x824614e4
  if (cr6.lt) goto loc_824614E4;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return r3;
loc_824614E4:
  // lwz r10,36(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 36);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stb r5,5(r11)
  REX_STORE_U8(uint32_t(r11) + 5, uint8_t(r5));
  // blr
  return r3;
}

// sub_824615A0: lifted from generated/ by lift.py.
lifted::Result f824615A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_824615A0 (lifted)");
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
  // bl 0x82461240
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82461240, LIFTED_ARGS(0x824615C0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x824615e0
  if (cr6.eq) goto loc_824615E0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x824615E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_824615E0:
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

// sub_82461F10: lifted from generated/ by lift.py.
uint64_t f82461F10(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82461F10 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_82461F20: lifted from generated/ by lift.py.
lifted::Result f82461F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82461F20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // mr r29,r3
  r29 = r3;
  // mr r26,r6
  r26 = r6;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x82462074
  if (cr6.eq) goto loc_82462074;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x82461f64
  if (cr6.eq) goto loc_82461F64;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_82461F64:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825b9a70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9A70(base, LIFTED_ARGS(0x82461F6C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82461F70, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r28,r3
  r28 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x82461f98
  if (cr6.eq) goto loc_82461F98;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x82461fac
  if (cr6.eq) goto loc_82461FAC;
loc_82461F98:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x82461FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_82461FAC:
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // addis r10,r29,5
  r10 = uint64_t(int64_t(int64_t(r29) + 327680));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r10,r10,-17760
  r10 = uint64_t(int64_t(int64_t(r10) + -17760));
  // mr r8,r13
  r8 = r13;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // addis r11,r29,5
  r11 = uint64_t(int64_t(int64_t(r29) + 327680));
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
  // addi r11,r11,-17752
  r11 = uint64_t(int64_t(int64_t(r11) + -17752));
  // ld r7,0(r10)
  r7 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // std r7,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r7);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // std r9,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r9);
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // fdiv f13,f12,f13
  f13 = f12 / f13;
  // lfd f0,88(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f11,f0
  f11 = double(std::bit_cast<int64_t>(f0));
  // lfd f0,-21896(r9)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r9) + -21896)));
  // fmul f0,f11,f0
  f0 = f11 * f0;
  // fdiv f0,f13,f0
  f0 = f13 / f0;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,0(r26)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r26) + 0, temp.u32);
  // std r27,0(r10)
  REX_STORE_U64(uint32_t(r10) + 0, r27);
  // std r27,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r27);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82462138
  if (cr6.eq) goto loc_82462138;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // bne cr6,0x82462138
  if (!cr6.eq) goto loc_82462138;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82462138
  if (!cr6.eq) goto loc_82462138;
  // mr r11,r27
  r11 = r27;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246205C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82462064, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_82462074:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82462078, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r28,r3
  r28 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x824620a0
  if (cr6.eq) goto loc_824620A0;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r9), so);
  // beq cr6,0x824620c0
  if (cr6.eq) goto loc_824620C0;
loc_824620A0:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x824620A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r9,r30
  r9 = r30;
  // mr r30,r28
  r30 = r28;
  // stw r9,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r9));
  // stb r30,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r30));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // b 0x824620c4
  goto loc_824620C4;
loc_824620C0:
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
loc_824620C4:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r8,r13
  r8 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r10,16(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 16);
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // add r10,r10,r29
  r10 = r10 + r29;
  // lfs f13,108(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 108);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f0,10764(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10764);
  f0 = double(temp.f32);
  // fmuls f31,f13,f0
  f31 = double(float(f13 * f0));
  // beq cr6,0x82462128
  if (cr6.eq) goto loc_82462128;
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // bne cr6,0x82462128
  if (!cr6.eq) goto loc_82462128;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82462128
  if (!cr6.eq) goto loc_82462128;
  // mr r11,r27
  r11 = r27;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82462120, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82462128, r31)); r3 = o.r3; f1 = o.f1; }
loc_82462128:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8748(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f0 = double(temp.f32);
  // fmuls f0,f31,f0
  f0 = double(float(f31 * f0));
  // stfs f0,0(r26)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r26) + 0, temp.u32);
loc_82462138:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82465398: lifted from generated/ by lift.py.
lifted::Result f82465398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82465398 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r31,r3,-192
  r31 = uint64_t(int64_t(int64_t(r3) + -192));
  // addi r10,r11,-21900
  r10 = uint64_t(int64_t(int64_t(r11) + -21900));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r9,r11,-22424
  r9 = uint64_t(int64_t(int64_t(r11) + -22424));
  // clrlwi r11,r4,31
  r11 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // addi r11,r31,52
  r11 = uint64_t(int64_t(int64_t(r31) + 52));
  // lwz r8,-52(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + -52);
  // lwz r8,4(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 4);
  // add r11,r8,r11
  r11 = r8 + r11;
  // stw r10,-52(r11)
  REX_STORE_U32(uint32_t(r11) + -52, uint32_t(r10));
  // stw r9,192(r31)
  REX_STORE_U32(uint32_t(r31) + 192, uint32_t(r9));
  // beq cr6,0x824653f4
  if (cr6.eq) goto loc_824653F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x824653F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_824653F4:
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

// sub_82465418: lifted from generated/ by lift.py.
uint64_t f82465418(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82465418 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_82465590: lifted from generated/ by lift.py.
lifted::Result f82465590(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82465590 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r31,r3,-52
  r31 = uint64_t(int64_t(int64_t(r3) + -52));
  // addi r10,r11,-21900
  r10 = uint64_t(int64_t(int64_t(r11) + -21900));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r9,r11,-22424
  r9 = uint64_t(int64_t(int64_t(r11) + -22424));
  // clrlwi r11,r4,31
  r11 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // addi r11,r31,52
  r11 = uint64_t(int64_t(int64_t(r31) + 52));
  // lwz r8,-52(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + -52);
  // lwz r8,4(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 4);
  // add r8,r8,r11
  r8 = r8 + r11;
  // stw r10,-52(r8)
  REX_STORE_U32(uint32_t(r8) + -52, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // beq cr6,0x824655ec
  if (cr6.eq) goto loc_824655EC;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x824655EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_824655EC:
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

// sub_82465848: lifted from generated/ by lift.py.
lifted::Result f82465848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82465848 (lifted)");
  uint64_t r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x824658c8
  { const lifted::Result o = lifted::entry::e824658C8(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82465850: lifted from generated/ by lift.py.
uint64_t f82465850(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82465850 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // sth r10,2(r4)
  REX_STORE_U16(uint32_t(r4) + 2, uint16_t(r10));
  // blr
  return r3;
}

// sub_82465868: lifted from generated/ by lift.py.
void f82465868(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82465868 (lifted)");
  // stw r3,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r3));
  // blr
  return;
}

// sub_824658B8: lifted from generated/ by lift.py.
uint64_t f824658B8(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_824658B8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // li r11,20
  r11 = uint64_t(int64_t(20));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_82465970: lifted from generated/ by lift.py.
lifted::Result f82465970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82465970 (lifted)");
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
  // mr r28,r4
  r28 = r4;
  // mr r29,r3
  r29 = r3;
  // mr r27,r5
  r27 = r5;
  // li r4,20
  r4 = uint64_t(int64_t(20));
  // mr r3,r28
  r3 = r28;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824659A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82465a14
  if (cr6.eq) goto loc_82465A14;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // addi r31,r30,4
  r31 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r11,r11,-21928
  r11 = uint64_t(int64_t(int64_t(r11) + -21928));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // addi r10,r10,-21780
  r10 = uint64_t(int64_t(int64_t(r10) + -21780));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r9,r9,-21808
  r9 = uint64_t(int64_t(int64_t(r9) + -21808));
  // stw r8,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r8));
  // mr r5,r28
  r5 = r28;
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // mr r4,r29
  r4 = r29;
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824659FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x82465a24
  if (cr6.lt) goto loc_82465A24;
  // stw r30,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r30));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_82465A14:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_82465A24:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82465A38, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82465A48: lifted from generated/ by lift.py.
lifted::Result f82465A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82465A48 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // mr r31,r6
  r31 = r6;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82465aa4
  if (!cr6.eq) goto loc_82465AA4;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x82465aa4
  if (!cr6.eq) goto loc_82465AA4;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r6,r31,4
  r6 = uint64_t(int64_t(int64_t(r31) + 4));
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r5,0(r6)
  r5 = REX_LOAD_U32(uint32_t(r6) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82465A90, r31)); r3 = o.r3; f1 = o.f1; }
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
loc_82465AA4:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82465af0
  if (!cr6.eq) goto loc_82465AF0;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // bne cr6,0x82465af0
  if (!cr6.eq) goto loc_82465AF0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82465ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82465b34
  if (cr6.lt) goto loc_82465B34;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
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
loc_82465AF0:
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x82465b2c
  if (!cr6.eq) goto loc_82465B2C;
  // clrlwi r11,r5,24
  r11 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x82465b2c
  if (!cr6.eq) goto loc_82465B2C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82465B18, r31)); r3 = o.r3; f1 = o.f1; }
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
loc_82465B2C:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
loc_82465B34:
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

// sub_82465B48: lifted from generated/ by lift.py.
lifted::Result f82465B48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82465B48 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82465b74
  if (!cr6.eq) goto loc_82465B74;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x82465b74
  if (!cr6.eq) goto loc_82465B74;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r4,0(r6)
  r4 = REX_LOAD_U32(uint32_t(r6) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82465B74:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82465ba0
  if (!cr6.eq) goto loc_82465BA0;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // bne cr6,0x82465ba0
  if (!cr6.eq) goto loc_82465BA0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r10,0(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 0);
  // clrlwi r4,r10,24
  r4 = uint32_t(r10) & 0xFF;
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82465BA0:
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x82465bc8
  if (!cr6.eq) goto loc_82465BC8;
  // clrlwi r11,r5,24
  r11 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x82465bc8
  if (!cr6.eq) goto loc_82465BC8;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r4,0(r6)
  r4 = REX_LOAD_U32(uint32_t(r6) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82465BC8:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return {r3, f1};
}

// sub_82465BF8: lifted from generated/ by lift.py.
uint64_t f82465BF8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82465BF8 (lifted)");
  uint64_t r11 = 0;
  // lbz r11,26(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 26);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // blr
  return r3;
}

// sub_82465C18: lifted from generated/ by lift.py.
lifted::Result f82465C18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82465C18 (lifted)");
  uint64_t r5 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82465CF8: lifted from generated/ by lift.py.
lifted::Result f82465CF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82465CF8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // mr r27,r5
  r27 = r5;
  // mr r25,r6
  r25 = r6;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82465D18, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r26,r3
  r26 = r3;
  // addi r29,r11,20240
  r29 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r28,r13
  r28 = r13;
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82465d40
  if (cr6.eq) goto loc_82465D40;
  // lwz r10,8(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 8);
  // cmplw cr6,r28,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r10), so);
  // beq cr6,0x82465d54
  if (cr6.eq) goto loc_82465D54;
loc_82465D40:
  // mr r3,r29
  r3 = r29;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x82465D48, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,8(r29)
  REX_STORE_U32(uint32_t(r29) + 8, uint32_t(r28));
  // stb r26,12(r29)
  REX_STORE_U8(uint32_t(r29) + 12, uint8_t(r26));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
loc_82465D54:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // stw r11,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r11));
  // lbz r11,16(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 16);
  // mr r10,r11
  r10 = r11;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // beq cr6,0x82465df4
  if (cr6.eq) goto loc_82465DF4;
  // addi r10,r30,8
  r10 = uint64_t(int64_t(int64_t(r30) + 8));
  // stb r11,0(r30)
  REX_STORE_U8(uint32_t(r30) + 0, uint8_t(r11));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r10));
  // lbz r11,16(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82465df8
  if (cr6.eq) goto loc_82465DF8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82465D98:
  // lwz r8,20(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // lwz r7,4(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lbzx r8,r11,r8
  r8 = REX_LOAD_U8(uint32_t(r11) + uint32_t(r8));
  // stbx r8,r10,r7
  REX_STORE_U8(uint32_t(r10) + uint32_t(r7), uint8_t(r8));
  // lwz r7,20(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r8,4(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + 4);
  // add r7,r11,r7
  r7 = r11 + r7;
  // add r8,r10,r8
  r8 = r10 + r8;
  // lbz r7,1(r7)
  r7 = REX_LOAD_U8(uint32_t(r7) + 1);
  // stb r7,1(r8)
  REX_STORE_U8(uint32_t(r8) + 1, uint8_t(r7));
  // lwz r7,20(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r8,4(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + 4);
  // add r7,r11,r7
  r7 = r11 + r7;
  // add r8,r10,r8
  r8 = r10 + r8;
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // lfs f0,8(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8);
  f0 = double(temp.f32);
  // stfs f0,4(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // lbz r8,16(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 16);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x82465d98
  if (cr6.lt) goto loc_82465D98;
  // b 0x82465df8
  goto loc_82465DF8;
loc_82465DF4:
  // stw r10,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r10));
loc_82465DF8:
  // lwz r9,4(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x82465e40
  if (cr6.eq) goto loc_82465E40;
  // lwz r11,8(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x82465e40
  if (!cr6.eq) goto loc_82465E40;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r11));
  // bne cr6,0x82465e40
  if (!cr6.eq) goto loc_82465E40;
  // lbz r31,12(r29)
  r31 = REX_LOAD_U8(uint32_t(r29) + 12);
  // mr r3,r29
  r3 = r29;
  // stb r11,12(r29)
  REX_STORE_U8(uint32_t(r29) + 12, uint8_t(r11));
  // stw r11,8(r29)
  REX_STORE_U32(uint32_t(r29) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82465E38, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82465E40, r31)); r3 = o.r3; f1 = o.f1; }
loc_82465E40:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82466058: lifted from generated/ by lift.py.
lifted::Result f82466058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82466058 (lifted)");
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
  // mr r28,r3
  r28 = r3;
  // mr r31,r4
  r31 = r4;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82466070, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r27,r3
  r27 = r3;
  // addi r30,r11,20240
  r30 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r29,r13
  r29 = r13;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82466098
  if (cr6.eq) goto loc_82466098;
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x824660b0
  if (cr6.eq) goto loc_824660B0;
loc_82466098:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x824660A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r29
  r10 = r29;
  // stw r10,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r10));
  // stb r27,12(r30)
  REX_STORE_U8(uint32_t(r30) + 12, uint8_t(r27));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
loc_824660B0:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // lbz r9,0(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 0);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82466138
  if (cr6.eq) goto loc_82466138;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_824660D4:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lbz r10,16(r28)
  r10 = REX_LOAD_U8(uint32_t(r28) + 16);
  // add r11,r9,r11
  r11 = r9 + r11;
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // bge cr6,0x82466118
  if (!cr6.lt) goto loc_82466118;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r8,20(r28)
  r8 = REX_LOAD_U32(uint32_t(r28) + 20);
  // add r11,r9,r11
  r11 = r9 + r11;
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rotlwi r7,r10,1
  r7 = __builtin_rotateleft32(uint32_t(r10), 1);
  // add r10,r10,r7
  r10 = r10 + r7;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r8
  r10 = r10 + r8;
  // lfs f13,8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8);
  f13 = double(temp.f32);
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // b 0x8246611c
  goto loc_8246611C;
loc_82466118:
  // stfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
loc_8246611C:
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // addi r9,r9,8
  r9 = uint64_t(int64_t(int64_t(r9) + 8));
  // cmplw cr6,r6,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r11), so);
  // blt cr6,0x824660d4
  if (cr6.lt) goto loc_824660D4;
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
loc_82466138:
  // mr r9,r13
  r9 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82466178
  if (cr6.eq) goto loc_82466178;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bne cr6,0x82466178
  if (!cr6.eq) goto loc_82466178;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // bne cr6,0x82466178
  if (!cr6.eq) goto loc_82466178;
  // lbz r31,12(r30)
  r31 = REX_LOAD_U8(uint32_t(r30) + 12);
  // mr r3,r30
  r3 = r30;
  // stb r11,12(r30)
  REX_STORE_U8(uint32_t(r30) + 12, uint8_t(r11));
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82466170, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82466178, r31)); r3 = o.r3; f1 = o.f1; }
loc_82466178:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82466410: lifted from generated/ by lift.py.
uint64_t f82466410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82466410 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82465418
  r3 = f82465418(base, r3, r4);
  return r3;
}

// sub_82466990: lifted from generated/ by lift.py.
lifted::Result f82466990(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82466990 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r4
  r11 = r4;
  // mr r4,r6
  r4 = r6;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x824669fc
  if (!cr6.eq) goto loc_824669FC;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x824669fc
  if (!cr6.eq) goto loc_824669FC;
  // lwz r11,4(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplwi cr6,r11,56
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 56, so);
  // bne cr6,0x824669d8
  if (!cr6.eq) goto loc_824669D8;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r4,0(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_824669D8:
  // cmplwi cr6,r11,60
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 60, so);
  // bne cr6,0x82466a74
  if (!cr6.eq) goto loc_82466A74;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r4,0(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 0);
  // addi r5,r4,56
  r5 = uint64_t(int64_t(int64_t(r4) + 56));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_824669FC:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82466a24
  if (!cr6.eq) goto loc_82466A24;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82466a24
  if (!cr6.eq) goto loc_82466A24;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466A24:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82466a4c
  if (!cr6.eq) goto loc_82466A4C;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82466a4c
  if (!cr6.eq) goto loc_82466A4C;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466A4C:
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x82466a74
  if (!cr6.eq) goto loc_82466A74;
  // clrlwi r11,r5,24
  r11 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82466a74
  if (!cr6.eq) goto loc_82466A74;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,76(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466A74:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return {r3, f1};
}

// sub_82466A80: lifted from generated/ by lift.py.
lifted::Result f82466A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82466A80 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x82466ae4
  if (!cr6.eq) goto loc_82466AE4;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x82466ae4
  if (!cr6.eq) goto loc_82466AE4;
  // lwz r11,4(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 4);
  // cmplwi cr6,r11,56
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 56, so);
  // bne cr6,0x82466ac0
  if (!cr6.eq) goto loc_82466AC0;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r4,0(r6)
  r4 = REX_LOAD_U32(uint32_t(r6) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466AC0:
  // cmplwi cr6,r11,60
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 60, so);
  // bne cr6,0x82466b68
  if (!cr6.eq) goto loc_82466B68;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r4,0(r6)
  r4 = REX_LOAD_U32(uint32_t(r6) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lbz r5,56(r4)
  r5 = REX_LOAD_U8(uint32_t(r4) + 56);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466AE4:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82466b10
  if (!cr6.eq) goto loc_82466B10;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82466b10
  if (!cr6.eq) goto loc_82466B10;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lfs f1,0(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466B10:
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82466b3c
  if (!cr6.eq) goto loc_82466B3C;
  // clrlwi r10,r5,24
  r10 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x82466b3c
  if (!cr6.eq) goto loc_82466B3C;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lfs f1,0(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466B3C:
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x82466b68
  if (!cr6.eq) goto loc_82466B68;
  // clrlwi r11,r5,24
  r11 = uint32_t(r5) & 0xFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82466b68
  if (!cr6.eq) goto loc_82466B68;
  // addi r3,r3,12
  r3 = uint64_t(int64_t(int64_t(r3) + 12));
  // lfs f1,0(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82466B68:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return {r3, f1};
}

// sub_82466C40: lifted from generated/ by lift.py.
lifted::Result f82466C40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82466C40 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x82467450
  { const lifted::Result o = f82467450(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82466C48: lifted from generated/ by lift.py.
uint64_t f82466C48(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82466C48 (lifted)");
  uint64_t r11 = 0;
  // lbz r11,156(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 156);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // blr
  return r3;
}

// sub_82466C58: lifted from generated/ by lift.py.
lifted::Result f82466C58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82466C58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x824699e0
  { const lifted::Result o = lifted::entry::e824699E0(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82466C60: lifted from generated/ by lift.py.
lifted::Result f82466C60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82466C60 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82466C88, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82466c9c
  if (cr6.lt) goto loc_82466C9C;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82466C9C:
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

// sub_82466CB0: lifted from generated/ by lift.py.
lifted::Result f82466CB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82466CB0 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82466CD8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82466ce8
  if (cr6.lt) goto loc_82466CE8;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r31,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r31));
loc_82466CE8:
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

// sub_82466D00: lifted from generated/ by lift.py.
lifted::Result f82466D00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82466D00 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82466D28, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x82466d3c
  if (cr6.lt) goto loc_82466D3C;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82466D3C:
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

// sub_82466D50: lifted from generated/ by lift.py.
uint64_t f82466D50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82466D50 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r3,r4
  r3 = r4;
  // mr r30,r5
  r30 = r5;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82466d84
  if (cr6.eq) goto loc_82466D84;
  // addi r4,r31,36
  r4 = uint64_t(int64_t(int64_t(r31) + 36));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x82466D84, r31)); r3 = o.r3; f1 = o.f1; }
loc_82466D84:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // beq cr6,0x82466d98
  if (cr6.eq) goto loc_82466D98;
  // lbz r11,157(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 157);
  // stb r11,0(r30)
  REX_STORE_U8(uint32_t(r30) + 0, uint8_t(r11));
loc_82466D98:
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

// sub_82466DB0: lifted from generated/ by lift.py.
uint64_t f82466DB0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82466DB0 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,96(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 96);
  f0 = double(temp.f32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // blr
  return r3;
}

// sub_82466DC0: lifted from generated/ by lift.py.
uint64_t f82466DC0(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82466DC0 (lifted)");
  uint64_t r11 = 0;
  lifted::Temp temp{};
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f1,96(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r11) + 96, temp.u32);
  // blr
  return r3;
}

// sub_82466DD0: lifted from generated/ by lift.py.
lifted::Result f82466DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82466DD0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lfs f1,100(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 100);
  f1 = double(temp.f32);
  // mr r31,r4
  r31 = r4;
  // bl 0x825a9c00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9C00(base, LIFTED_ARGS(0x82466DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lfs f0,-21512(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -21512);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
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

// sub_82466E18: lifted from generated/ by lift.py.
lifted::Result f82466E18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82466E18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f2,f1
  lifted::ScalarFloatMode();
  f2 = f1;
  // mr r31,r3
  r31 = r3;
  // lfd f1,22232(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 22232)));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8ED0(base, LIFTED_ARGS(0x82466E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f0,100(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 100, temp.u32);
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

// sub_82466E60: lifted from generated/ by lift.py.
uint64_t f82466E60(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82466E60 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,100(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 100);
  f0 = double(temp.f32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // blr
  return r3;
}

// sub_82466E70: lifted from generated/ by lift.py.
uint64_t f82466E70(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82466E70 (lifted)");
  uint64_t r11 = 0;
  lifted::Temp temp{};
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f1,100(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r11) + 100, temp.u32);
  // blr
  return r3;
}

// sub_82466F40: lifted from generated/ by lift.py.
lifted::Result f82466F40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82466F40 (lifted)");
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
  // mr r28,r3
  r28 = r3;
  // mr r27,r4
  r27 = r4;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82466F58, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r29,r3
  r29 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82466f80
  if (cr6.eq) goto loc_82466F80;
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r8), so);
  // beq cr6,0x82466fa0
  if (cr6.eq) goto loc_82466FA0;
loc_82466F80:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x82466F88, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r30
  r8 = r30;
  // mr r30,r29
  r30 = r29;
  // stw r8,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r8));
  // stb r30,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r30));
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // b 0x82466fa4
  goto loc_82466FA4;
loc_82466FA0:
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
loc_82466FA4:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r28,4
  r11 = uint64_t(int64_t(int64_t(r28) + 4));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beq cr6,0x82466fcc
  if (cr6.eq) goto loc_82466FCC;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r11,r11,r9
  r11 = r9 - r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82467018
  if (!cr6.eq) goto loc_82467018;
loc_82466FCC:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82467008
  if (cr6.eq) goto loc_82467008;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x82467008
  if (!cr6.eq) goto loc_82467008;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82467008
  if (!cr6.eq) goto loc_82467008;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82467000, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82467008, r31)); r3 = o.r3; f1 = o.f1; }
loc_82467008:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_82467018:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82467030
  if (cr6.eq) goto loc_82467030;
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_82467030:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8246706c
  if (cr6.eq) goto loc_8246706C;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8246706c
  if (!cr6.eq) goto loc_8246706C;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8246706c
  if (!cr6.eq) goto loc_8246706C;
  // mr r3,r31
  r3 = r31;
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82467064, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246706C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246706C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82467140: lifted from generated/ by lift.py.
lifted::Result f82467140(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82467140 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82467154, r31)); r3 = o.r3; f1 = o.f1; }
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
  // beq cr6,0x8246717c
  if (cr6.eq) goto loc_8246717C;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x82467190
  if (cr6.eq) goto loc_82467190;
loc_8246717C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x82467184, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_82467190:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lbz r10,156(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 156);
  // clrlwi r9,r10,31
  r9 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x824671b4
  if (!cr6.eq) goto loc_824671B4;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // b 0x824671c8
  goto loc_824671C8;
loc_824671B4:
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x824671e0
  if (cr6.eq) goto loc_824671E0;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,6
  r4 = uint64_t(int64_t(6));
loc_824671C8:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824671DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_824671E0:
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82467224
  if (cr6.eq) goto loc_82467224;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82467224
  if (!cr6.eq) goto loc_82467224;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82467224
  if (!cr6.eq) goto loc_82467224;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246721C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82467224, r31)); r3 = o.r3; f1 = o.f1; }
loc_82467224:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82467230: lifted from generated/ by lift.py.
lifted::Result f82467230(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82467230 (lifted)");
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
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82467248, r31)); r3 = o.r3; f1 = o.f1; }
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
  // beq cr6,0x82467270
  if (cr6.eq) goto loc_82467270;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x82467284
  if (cr6.eq) goto loc_82467284;
loc_82467270:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x82467278, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_82467284:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lbz r10,156(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 156);
  // clrlwi r9,r10,31
  r9 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8246730c
  if (cr6.eq) goto loc_8246730C;
  // clrlwi r9,r27,31
  r9 = uint32_t(r27) & 0x1;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x824672ec
  if (!cr6.eq) goto loc_824672EC;
  // andi. r9,r10,18
  r9 = r10 & 18;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x824672ec
  if (!cr6.eq) goto loc_824672EC;
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8246730c
  if (!cr6.eq) goto loc_8246730C;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824672DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lhz r11,-18096(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + -18096);
  // sth r11,158(r30)
  REX_STORE_U16(uint32_t(r30) + 158, uint16_t(r11));
  // b 0x82467308
  goto loc_82467308;
loc_824672EC:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82467308, r31)); r3 = o.r3; f1 = o.f1; }
loc_82467308:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246730C:
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82467350
  if (cr6.eq) goto loc_82467350;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82467350
  if (!cr6.eq) goto loc_82467350;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82467350
  if (!cr6.eq) goto loc_82467350;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82467348, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82467350, r31)); r3 = o.r3; f1 = o.f1; }
loc_82467350:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82467360: lifted from generated/ by lift.py.
lifted::Result f82467360(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82467360 (lifted)");
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
  // mr r29,r3
  r29 = r3;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x82467374, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r28,r3
  r28 = r3;
  // addi r31,r11,20240
  r31 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8246739c
  if (cr6.eq) goto loc_8246739C;
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r8), so);
  // beq cr6,0x824673b4
  if (cr6.eq) goto loc_824673B4;
loc_8246739C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x824673A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r30
  r8 = r30;
  // stw r8,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r8));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_824673B4:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r29,4
  r11 = uint64_t(int64_t(int64_t(r29) + 4));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beq cr6,0x82467400
  if (cr6.eq) goto loc_82467400;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r11,r11,r9
  r11 = r9 - r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82467400
  if (cr6.eq) goto loc_82467400;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r29
  r3 = r29;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x824673F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_82467400:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x82467440
  if (cr6.eq) goto loc_82467440;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x82467440
  if (!cr6.eq) goto loc_82467440;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x82467440
  if (!cr6.eq) goto loc_82467440;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x82467438, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x82467440, r31)); r3 = o.r3; f1 = o.f1; }
loc_82467440:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82467450: lifted from generated/ by lift.py.
lifted::Result f82467450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82467450 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r11,-21552
  r11 = uint64_t(int64_t(int64_t(r11) + -21552));
  // addi r10,r10,-21648
  r10 = uint64_t(int64_t(int64_t(r10) + -21648));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r10));
  // bl 0x82469960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82469960, LIFTED_ARGS(0x8246748C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // clrlwi r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  // addi r11,r11,-22856
  r11 = uint64_t(int64_t(int64_t(r11) + -22856));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x824674b8
  if (cr6.eq) goto loc_824674B8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x824674B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_824674B8:
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

// sub_82469A88: lifted from generated/ by lift.py.
lifted::Result f82469A88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82469A88 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-18088
  r11 = uint64_t(int64_t(int64_t(r11) + -18088));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8246e0e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8246E0E8, LIFTED_ARGS(0x82469AB8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82469ad8
  if (cr6.eq) goto loc_82469AD8;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lis r5,24962
  r5 = uint64_t(int64_t(1635909632));
  // addi r3,r11,20256
  r3 = uint64_t(int64_t(int64_t(r11) + 20256));
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ef90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245EF90, LIFTED_ARGS(0x82469AD8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82469AD8:
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

// sub_8246A698: lifted from generated/ by lift.py.
uint64_t f8246A698(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8246A698 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // rlwinm r11,r5,11,0,20
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 11) & 0xFFFFF800;
  // add r6,r3,r4
  r6 = r3 + r4;
  // add r10,r11,r3
  r10 = r11 + r3;
  // rlwinm r8,r5,14,0,17
  r8 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 14) & 0xFFFFC000;
  // mr r5,r10
  r5 = r10;
  // mr r4,r8
  r4 = r8;
  // lwz r7,0(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 0);
  // clrlwi r11,r7,24
  r11 = uint32_t(r7) & 0xFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r9,r11,11,0,20
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 11) & 0xFFFFF800;
  // add r11,r9,r10
  r11 = r9 + r10;
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // bge cr6,0x8246a6fc
  if (!cr6.lt) goto loc_8246A6FC;
loc_8246A6CC:
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r4,r8
  r4 = r8;
  // add r8,r9,r8
  r8 = r9 + r8;
  // clrlwi r9,r7,24
  r9 = uint32_t(r7) & 0xFF;
  // mr r5,r10
  r5 = r10;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // mr r10,r11
  r10 = r11;
  // rlwinm r9,r9,11,0,20
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 11) & 0xFFFFF800;
  // add r11,r11,r9
  r11 = r11 + r9;
  // cmplw cr6,r11,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r6), so);
  // blt cr6,0x8246a6cc
  if (cr6.lt) goto loc_8246A6CC;
loc_8246A6FC:
  // rlwinm r11,r7,0,0,5
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFC000000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246a710
  if (!cr6.eq) goto loc_8246A710;
  // mr r10,r5
  r10 = r5;
  // mr r8,r4
  r8 = r4;
loc_8246A710:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r11,6,26,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0x3F;
  // rlwinm r11,r11,21,17,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 21) & 0x7FFF;
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // addi r11,r11,32
  r11 = uint64_t(int64_t(int64_t(r11) + 32));
  // ble cr6,0x8246a750
  if (!cr6.gt) goto loc_8246A750;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
loc_8246A72C:
  // rlwinm r7,r11,29,3,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // clrlwi r6,r11,29
  r6 = uint32_t(r11) & 0x7;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lwzx r7,r7,r10
  r7 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r10));
  // slw r7,r7,r6
  r7 = uint8_t(r6) & 0x20 ? 0 : (uint32_t(r7) << (uint8_t(r6) & 0x3F));
  // rlwinm r7,r7,15,17,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 15) & 0x7FFF;
  // add r11,r7,r11
  r11 = r7 + r11;
  // bne cr6,0x8246a72c
  if (!cr6.eq) goto loc_8246A72C;
loc_8246A750:
  // add r3,r11,r8
  r3 = r11 + r8;
  // blr
  return r3;
}

// sub_8246A758: lifted from generated/ by lift.py.
lifted::Result f8246A758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8246A758 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32251
  r10 = uint64_t(int64_t(-2113601536));
  // lis r9,-32251
  r9 = uint64_t(int64_t(-2113601536));
  // addi r30,r31,4
  r30 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r11,r11,-17856
  r11 = uint64_t(int64_t(int64_t(r11) + -17856));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // addi r10,r10,-17896
  r10 = uint64_t(int64_t(int64_t(r10) + -17896));
  // addi r9,r9,-17992
  r9 = uint64_t(int64_t(int64_t(r9) + -17992));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // stw r9,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r9));
  // beq cr6,0x8246a7b0
  if (cr6.eq) goto loc_8246A7B0;
  // bl 0x8245c880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245C880(base, LIFTED_ARGS(0x8246A7A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,184(r31)
  REX_STORE_U32(uint32_t(r31) + 184, uint32_t(r11));
loc_8246A7B0:
  // mr r3,r30
  r3 = r30;
  // bl 0x82466ee8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82466EE8(base, LIFTED_ARGS(0x8246A7B8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8246A7D0: lifted from generated/ by lift.py.
lifted::Result f8246A7D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8246A7D0 (lifted)");
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
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r5
  r30 = r5;
  // lbz r11,266(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246a804
  if (!cr6.eq) goto loc_8246A804;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16385
  r3 = r3 | 16385;
  // b 0x8246a848
  goto loc_8246A848;
loc_8246A804:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,266(r31)
  REX_STORE_U8(uint32_t(r31) + 266, uint8_t(r11));
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // ble cr6,0x8246a838
  if (!cr6.gt) goto loc_8246A838;
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x8246a838
  if (cr6.eq) goto loc_8246A838;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,88
  r5 = uint64_t(int64_t(88));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x8246A82C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
loc_8246A838:
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // ori r5,r11,48
  r5 = r11 | 48;
  // bl 0x82469ef8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82469EF8, LIFTED_ARGS(0x8246A848, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246A848:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
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

// sub_8246A900: lifted from generated/ by lift.py.
lifted::Result f8246A900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246A900 (lifted)");
  uint64_t r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246aad0
  { const lifted::Result o = lifted::Call(base, 0x8246AAD0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246A908: lifted from generated/ by lift.py.
lifted::Result f8246A908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246A908 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a860
  { const lifted::Result o = lifted::entry::e8246A860(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246A910: lifted from generated/ by lift.py.
lifted::Result f8246A910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8246A910 (lifted)");
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a7d0
  { const lifted::Result o = f8246A7D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246A930: lifted from generated/ by lift.py.
uint64_t f8246A930(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246A930 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,112(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 112);
  f0 = double(temp.f32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // blr
  return r3;
}

// sub_8246A940: lifted from generated/ by lift.py.
lifted::Result f8246A940(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246A940 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lfs f1,116(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f1 = double(temp.f32);
  // mr r31,r4
  r31 = r4;
  // bl 0x825a9c00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9C00(base, LIFTED_ARGS(0x8246A95C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lfs f0,-21512(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -21512);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
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

// sub_8246A988: lifted from generated/ by lift.py.
uint64_t f8246A988(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246A988 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,116(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f0 = double(temp.f32);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // blr
  return r3;
}

// sub_8246A998: lifted from generated/ by lift.py.
lifted::Result f8246A998(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246A998 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246A9C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x8246a9d8
  if (cr6.lt) goto loc_8246A9D8;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8246A9D8:
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

// sub_8246A9F0: lifted from generated/ by lift.py.
lifted::Result f8246A9F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246A9F0 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246AA1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x8246aa30
  if (cr6.lt) goto loc_8246AA30;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8246AA30:
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

// sub_8246AA48: lifted from generated/ by lift.py.
uint64_t f8246AA48(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246AA48 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,168(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 168);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_8246AA58: lifted from generated/ by lift.py.
uint64_t f8246AA58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8246AA58 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r11,r4
  r11 = r4;
  // mr r30,r5
  r30 = r5;
  // addi r31,r3,16
  r31 = uint64_t(int64_t(int64_t(r3) + 16));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246aa90
  if (cr6.eq) goto loc_8246AA90;
  // addi r4,r31,36
  r4 = uint64_t(int64_t(int64_t(r31) + 36));
  // li r5,56
  r5 = uint64_t(int64_t(56));
  // mr r3,r11
  r3 = r11;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x8246AA90, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246AA90:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // beq cr6,0x8246aaa4
  if (cr6.eq) goto loc_8246AAA4;
  // lbz r11,157(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 157);
  // stb r11,0(r30)
  REX_STORE_U8(uint32_t(r30) + 0, uint8_t(r11));
loc_8246AAA4:
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

// sub_8246AAC0: lifted from generated/ by lift.py.
lifted::Result f8246AAC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246AAC0 (lifted)");
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // b 0x82466990
  { const lifted::Result o = f82466990(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246AAC8: lifted from generated/ by lift.py.
lifted::Result f8246AAC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246AAC8 (lifted)");
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // b 0x82466a80
  { const lifted::Result o = f82466A80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246AB00: lifted from generated/ by lift.py.
lifted::Result f8246AB00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8246AB00 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x82469e58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82469E58, LIFTED_ARGS(0x8246AB1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x8246ab30
  if (cr6.lt) goto loc_8246AB30;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // sth r11,108(r31)
  REX_STORE_U16(uint32_t(r31) + 108, uint16_t(r11));
  // sth r11,110(r31)
  REX_STORE_U16(uint32_t(r31) + 110, uint16_t(r11));
loc_8246AB30:
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

// sub_8246B0C8: lifted from generated/ by lift.py.
lifted::Result f8246B0C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246B0C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r29,r3
  r29 = r3;
  // addi r31,r29,268
  r31 = uint64_t(int64_t(int64_t(r29) + 268));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246B0E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b100
  if (cr6.eq) goto loc_8246B100;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246b110
  if (cr6.eq) goto loc_8246B110;
loc_8246B100:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246B108, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
loc_8246B110:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245c948
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245C948(base, LIFTED_ARGS(0x8246B124, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246b170
  if (cr6.eq) goto loc_8246B170;
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d208
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D208(base, LIFTED_ARGS(0x8246B138, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D288(base, LIFTED_ARGS(0x8246B140, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,56(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b170
  if (cr6.eq) goto loc_8246B170;
  // mr r30,r28
  r30 = r28;
loc_8246B150:
  // mr r4,r30
  r4 = r30;
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245ce98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CE98(base, LIFTED_ARGS(0x8246B15C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lbz r10,56(r29)
  r10 = REX_LOAD_U8(uint32_t(r29) + 56);
  // clrlwi r30,r11,24
  r30 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // blt cr6,0x8246b150
  if (cr6.lt) goto loc_8246B150;
loc_8246B170:
  // stb r28,266(r29)
  REX_STORE_U8(uint32_t(r29) + 266, uint8_t(r28));
  // mr r10,r13
  r10 = r13;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b1b8
  if (cr6.eq) goto loc_8246B1B8;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246b1b8
  if (!cr6.eq) goto loc_8246B1B8;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne 0x8246b1b8
  if (!cr0.eq) goto loc_8246B1B8;
  // mr r3,r31
  r3 = r31;
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // stw r28,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r28));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246B1B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246B1B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B1B8:
  // addi r3,r29,16
  r3 = uint64_t(int64_t(int64_t(r29) + 16));
  // bl 0x824699e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e824699E0(base, LIFTED_ARGS(0x8246B1C0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246B1C8: lifted from generated/ by lift.py.
lifted::Result f8246B1C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246B1C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // mr r27,r4
  r27 = r4;
  // mr r26,r25
  r26 = r25;
  // addi r30,r29,268
  r30 = uint64_t(int64_t(int64_t(r29) + 268));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246B1EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r31,r13
  r31 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b20c
  if (cr6.eq) goto loc_8246B20C;
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // beq cr6,0x8246b21c
  if (cr6.eq) goto loc_8246B21C;
loc_8246B20C:
  // mr r3,r30
  r3 = r30;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246B214, r31)); r3 = o.r3; f1 = o.f1; }
  // stb r28,12(r30)
  REX_STORE_U8(uint32_t(r30) + 12, uint8_t(r28));
  // stw r31,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r31));
loc_8246B21C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d350
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D350(base, LIFTED_ARGS(0x8246B230, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x8246b258
  if (!cr6.eq) goto loc_8246B258;
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d208
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D208(base, LIFTED_ARGS(0x8246B244, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // blt cr6,0x8246b2c8
  if (cr6.lt) goto loc_8246B2C8;
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D288(base, LIFTED_ARGS(0x8246B258, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B258:
  // lbz r11,56(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b2b4
  if (cr6.eq) goto loc_8246B2B4;
  // mr r31,r25
  r31 = r25;
loc_8246B268:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // mr r4,r31
  r4 = r31;
  // bl 0x8245d070
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D070(base, LIFTED_ARGS(0x8246B278, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // blt cr6,0x8246b2b4
  if (cr6.lt) goto loc_8246B2B4;
  // lwz r9,88(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 88);
  // rlwinm r11,r31,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 3) & 0xFFFFFFF8;
  // lwz r8,84(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 84);
  // addi r10,r31,1
  r10 = uint64_t(int64_t(int64_t(r31) + 1));
  // add r11,r11,r27
  r11 = r11 + r27;
  // rlwinm r9,r9,7,0,24
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFF80;
  // clrlwi r31,r10,24
  r31 = uint32_t(r10) & 0xFF;
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // sth r9,4(r11)
  REX_STORE_U16(uint32_t(r11) + 4, uint16_t(r9));
  // lbz r11,56(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 56);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8246b268
  if (cr6.lt) goto loc_8246B268;
loc_8246B2B4:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x8246b2c8
  if (!cr6.eq) goto loc_8246B2C8;
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d4d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D4D0(base, LIFTED_ARGS(0x8246B2C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
loc_8246B2C8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b30c
  if (cr6.eq) goto loc_8246B30C;
  // lwz r9,8(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246b30c
  if (!cr6.eq) goto loc_8246B30C;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // bne 0x8246b30c
  if (!cr0.eq) goto loc_8246B30C;
  // lbz r31,12(r30)
  r31 = REX_LOAD_U8(uint32_t(r30) + 12);
  // mr r3,r30
  r3 = r30;
  // stw r25,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r25));
  // stb r25,12(r30)
  REX_STORE_U8(uint32_t(r30) + 12, uint8_t(r25));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246B304, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246B30C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B30C:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246B318: lifted from generated/ by lift.py.
lifted::Result f8246B318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8246B318 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r29,r3
  r29 = r3;
  // mr r27,r4
  r27 = r4;
  // lbz r11,266(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246b348
  if (!cr6.eq) goto loc_8246B348;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16385
  r3 = r3 | 16385;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_8246B348:
  // addi r31,r29,268
  r31 = uint64_t(int64_t(int64_t(r29) + 268));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246B350, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b370
  if (cr6.eq) goto loc_8246B370;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246b380
  if (cr6.eq) goto loc_8246B380;
loc_8246B370:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246B378, r31)); r3 = o.r3; f1 = o.f1; }
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_8246B380:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r3,r29,120
  r3 = uint64_t(int64_t(int64_t(r29) + 120));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r4,r27
  r4 = r27;
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x8246B39C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b3e4
  if (cr6.eq) goto loc_8246B3E4;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246b3e4
  if (!cr6.eq) goto loc_8246B3E4;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne 0x8246b3e4
  if (!cr0.eq) goto loc_8246B3E4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // mr r3,r31
  r3 = r31;
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stb r11,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246B3DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246B3E4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B3E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246B3F0: lifted from generated/ by lift.py.
lifted::Result f8246B3F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246B3F0 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a940
  { const lifted::Result o = f8246A940(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B3F8: lifted from generated/ by lift.py.
lifted::Result f8246B3F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8246B3F8 (lifted)");
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246ab00
  { const lifted::Result o = f8246AB00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B400: lifted from generated/ by lift.py.
lifted::Result f8246B400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246B400 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x8246aac8
  { const lifted::Result o = f8246AAC8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B408: lifted from generated/ by lift.py.
lifted::Result f8246B408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246B408 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246b0c8
  { const lifted::Result o = f8246B0C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B410: lifted from generated/ by lift.py.
uint64_t f8246B410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246B410 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a988
  r3 = f8246A988(base, r3, r4);
  return r3;
}

// sub_8246B418: lifted from generated/ by lift.py.
uint64_t f8246B418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246B418 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a920
  { const lifted::Result o = lifted::entry::e8246A920(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_8246B420: lifted from generated/ by lift.py.
lifted::Result f8246B420(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246B420 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246ab48
  { const lifted::Result o = lifted::entry::e8246AB48(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B428: lifted from generated/ by lift.py.
uint64_t f8246B428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8246B428 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246aa58
  r3 = f8246AA58(base, r1, r13, r3, r4, r5);
  return r3;
}

// sub_8246B430: lifted from generated/ by lift.py.
lifted::Result f8246B430(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246B430 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x8246aac0
  { const lifted::Result o = f8246AAC0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B438: lifted from generated/ by lift.py.
lifted::Result f8246B438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246B438 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246ac40
  { const lifted::Result o = lifted::entry::e8246AC40(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B440: lifted from generated/ by lift.py.
uint64_t f8246B440(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246B440 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246a930
  r3 = f8246A930(base, r3, r4);
  return r3;
}

// sub_8246B448: lifted from generated/ by lift.py.
lifted::Result f8246B448(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8246B448 (lifted)");
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246ad98
  { const lifted::Result o = f8246AD98(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246B450: lifted from generated/ by lift.py.
lifted::Result f8246B450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246B450 (lifted)");
  double f1 = 0.0;
  // b 0x8246b0c8
  { const lifted::Result o = f8246B0C8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246BE20: lifted from generated/ by lift.py.
lifted::Result f8246BE20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246BE20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lbz r11,252(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 252);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bee0
  if (cr6.eq) goto loc_8246BEE0;
  // addi r31,r29,268
  r31 = uint64_t(int64_t(int64_t(r29) + 268));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246BE4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246be6c
  if (cr6.eq) goto loc_8246BE6C;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246be7c
  if (cr6.eq) goto loc_8246BE7C;
loc_8246BE6C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246BE74, r31)); r3 = o.r3; f1 = o.f1; }
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_8246BE7C:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245d138
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D138(base, LIFTED_ARGS(0x8246BE90, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246bed8
  if (cr6.eq) goto loc_8246BED8;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246bed8
  if (!cr6.eq) goto loc_8246BED8;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne 0x8246bed8
  if (!cr0.eq) goto loc_8246BED8;
  // mr r3,r31
  r3 = r31;
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // stw r27,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r27));
  // stb r27,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r27));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246BED0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246BED8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246BED8:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x8246bfc8
  if (cr6.lt) goto loc_8246BFC8;
loc_8246BEE0:
  // addi r30,r29,16
  r30 = uint64_t(int64_t(int64_t(r29) + 16));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246BEE8, r31)); r3 = o.r3; f1 = o.f1; }
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
  // beq cr6,0x8246bf10
  if (cr6.eq) goto loc_8246BF10;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x8246bf24
  if (cr6.eq) goto loc_8246BF24;
loc_8246BF10:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246BF18, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246BF24:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lbz r10,156(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 156);
  // clrlwi r9,r10,31
  r9 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x8246bf48
  if (!cr6.eq) goto loc_8246BF48;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // b 0x8246bf5c
  goto loc_8246BF5C;
loc_8246BF48:
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246bf74
  if (cr6.eq) goto loc_8246BF74;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,6
  r4 = uint64_t(int64_t(6));
loc_8246BF5C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246BF70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246BF74:
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246bfbc
  if (cr6.eq) goto loc_8246BFBC;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246bfbc
  if (!cr6.eq) goto loc_8246BFBC;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8246bfbc
  if (!cr6.eq) goto loc_8246BFBC;
  // mr r11,r27
  r11 = r27;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246BFB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246BFBC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246BFBC:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_8246BFC8:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246BFD8: lifted from generated/ by lift.py.
lifted::Result f8246BFD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246BFD8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // clrlwi r27,r4,31
  r27 = uint32_t(r4) & 0x1;
  // mr r29,r3
  r29 = r3;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8246c098
  if (cr6.eq) goto loc_8246C098;
  // lbz r11,252(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 252);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c098
  if (cr6.eq) goto loc_8246C098;
  // addi r31,r29,268
  r31 = uint64_t(int64_t(int64_t(r29) + 268));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246C010, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r28,r3
  r28 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c030
  if (cr6.eq) goto loc_8246C030;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246c040
  if (cr6.eq) goto loc_8246C040;
loc_8246C030:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246C038, r31)); r3 = o.r3; f1 = o.f1; }
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_8246C040:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lwz r3,184(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 184);
  // bl 0x8245c8d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245C8D0(base, LIFTED_ARGS(0x8246C054, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c098
  if (cr6.eq) goto loc_8246C098;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246c098
  if (!cr6.eq) goto loc_8246C098;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne 0x8246c098
  if (!cr0.eq) goto loc_8246C098;
  // mr r3,r31
  r3 = r31;
  // lbz r30,12(r31)
  r30 = REX_LOAD_U8(uint32_t(r31) + 12);
  // stw r26,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r26));
  // stb r26,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r26));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246C090, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246C098, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C098:
  // addi r30,r29,16
  r30 = uint64_t(int64_t(int64_t(r29) + 16));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246C0A0, r31)); r3 = o.r3; f1 = o.f1; }
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
  // beq cr6,0x8246c0c8
  if (cr6.eq) goto loc_8246C0C8;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // beq cr6,0x8246c0dc
  if (cr6.eq) goto loc_8246C0DC;
loc_8246C0C8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246C0D0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r29));
  // stb r28,12(r31)
  REX_STORE_U8(uint32_t(r31) + 12, uint8_t(r28));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246C0DC:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // lbz r10,156(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 156);
  // clrlwi r9,r10,31
  r9 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8246c160
  if (cr6.eq) goto loc_8246C160;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // bne cr6,0x8246c140
  if (!cr6.eq) goto loc_8246C140;
  // andi. r9,r10,18
  r9 = r10 & 18;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x8246c140
  if (!cr6.eq) goto loc_8246C140;
  // rlwinm r10,r10,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8246c160
  if (!cr6.eq) goto loc_8246C160;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246C130, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lhz r11,-18096(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + -18096);
  // sth r11,158(r30)
  REX_STORE_U16(uint32_t(r30) + 158, uint16_t(r11));
  // b 0x8246c15c
  goto loc_8246C15C;
loc_8246C140:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r30
  r3 = r30;
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246C15C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C15C:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8246C160:
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c1a8
  if (cr6.eq) goto loc_8246C1A8;
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246c1a8
  if (!cr6.eq) goto loc_8246C1A8;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x8246c1a8
  if (!cr6.eq) goto loc_8246C1A8;
  // mr r11,r26
  r11 = r26;
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
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246C1A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246C1A8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C1A8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246C1B8: lifted from generated/ by lift.py.
lifted::Result f8246C1B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246C1B8 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246bfd8
  { const lifted::Result o = f8246BFD8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246C1C0: lifted from generated/ by lift.py.
lifted::Result f8246C1C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246C1C0 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246b450
  { const lifted::Result o = f8246B450(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246C1C8: lifted from generated/ by lift.py.
lifted::Result f8246C1C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246C1C8 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246be20
  { const lifted::Result o = f8246BE20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246C320: lifted from generated/ by lift.py.
lifted::Result f8246C320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246C320 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-96(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -96, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // mr r28,r5
  r28 = r5;
  // addi r24,r31,268
  r24 = uint64_t(int64_t(int64_t(r31) + 268));
  // mr r26,r23
  r26 = r23;
  // mr r22,r23
  r22 = r23;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246C34C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c36c
  if (cr6.eq) goto loc_8246C36C;
  // lwz r11,8(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246c37c
  if (cr6.eq) goto loc_8246C37C;
loc_8246C36C:
  // mr r3,r24
  r3 = r24;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246C374, r31)); r3 = o.r3; f1 = o.f1; }
  // stb r29,12(r24)
  REX_STORE_U8(uint32_t(r24) + 12, uint8_t(r29));
  // stw r30,8(r24)
  REX_STORE_U32(uint32_t(r24) + 8, uint32_t(r30));
loc_8246C37C:
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r11));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245c948
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245C948(base, LIFTED_ARGS(0x8246C390, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246cd58
  if (cr6.eq) goto loc_8246CD58;
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245d208
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D208(base, LIFTED_ARGS(0x8246C3A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x8246c400
  if (!cr6.lt) goto loc_8246C400;
loc_8246C3AC:
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c3f0
  if (cr6.eq) goto loc_8246C3F0;
  // lwz r9,8(r24)
  r9 = REX_LOAD_U32(uint32_t(r24) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246c3f0
  if (!cr6.eq) goto loc_8246C3F0;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r11));
  // bne 0x8246c3f0
  if (!cr0.eq) goto loc_8246C3F0;
  // stw r23,8(r24)
  REX_STORE_U32(uint32_t(r24) + 8, uint32_t(r23));
  // mr r3,r24
  r3 = r24;
  // lbz r31,12(r24)
  r31 = REX_LOAD_U8(uint32_t(r24) + 12);
  // stb r23,12(r24)
  REX_STORE_U8(uint32_t(r24) + 12, uint8_t(r23));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246C3E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246C3F0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C3F0:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f31,-96(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_8246C400:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246C404, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r29,r3
  r29 = r3;
  // addi r25,r11,20240
  r25 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r30,r13
  r30 = r13;
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c42c
  if (cr6.eq) goto loc_8246C42C;
  // lwz r10,8(r25)
  r10 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8246c440
  if (cr6.eq) goto loc_8246C440;
loc_8246C42C:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246C434, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r30));
  // stb r29,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r29));
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246C440:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r28
  r3 = r28;
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bl 0x8245f718
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245F718(base, LIFTED_ARGS(0x8246C454, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x8246c4bc
  if (cr6.lt) goto loc_8246C4BC;
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // mr r10,r23
  r10 = r23;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c49c
  if (cr6.eq) goto loc_8246C49C;
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // mr r11,r23
  r11 = r23;
loc_8246C478:
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r7,r11,8
  r7 = uint64_t(int64_t(int64_t(r11) + 8));
  // mr r11,r9
  r11 = r9;
  // rlwinm r9,r7,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 3) & 0xFFFFFFF8;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // lbzx r9,r9,r31
  r9 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r31));
  // add r10,r9,r10
  r10 = r9 + r10;
  // blt cr6,0x8246c478
  if (cr6.lt) goto loc_8246C478;
loc_8246C49C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stb r10,89(r1)
  REX_STORE_U8(uint32_t(r1) + 89, uint8_t(r10));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r28
  r3 = r28;
  // bl 0x8245fa10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245FA10(base, LIFTED_ARGS(0x8246C4B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bge cr6,0x8246c77c
  if (!cr6.lt) goto loc_8246C77C;
loc_8246C4BC:
  // lwz r9,4(r25)
  r9 = REX_LOAD_U32(uint32_t(r25) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8246c508
  if (cr6.eq) goto loc_8246C508;
  // lwz r11,8(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8246c508
  if (!cr6.eq) goto loc_8246C508;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bne cr6,0x8246c508
  if (!cr6.eq) goto loc_8246C508;
  // mr r11,r23
  r11 = r23;
  // lbz r30,12(r25)
  r30 = REX_LOAD_U8(uint32_t(r25) + 12);
  // mr r3,r25
  r3 = r25;
  // stb r11,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r11));
  // stw r11,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246C500, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246C508, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C508:
  // mr r30,r29
  r30 = r29;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x8246c3ac
  if (!cr6.eq) goto loc_8246C3AC;
loc_8246C514:
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245d288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D288(base, LIFTED_ARGS(0x8246C51C, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,266(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246c530
  if (!cr6.eq) goto loc_8246C530;
  // mr r3,r31
  r3 = r31;
  // bl 0x8246bb98
  // (left out: dead in C++)
  { const lifted::Result o = f8246BB98(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8246C530:
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c6b8
  if (cr6.eq) goto loc_8246C6B8;
  // mr r27,r23
  r27 = r23;
loc_8246C540:
  // mr r4,r27
  r4 = r27;
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245cb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CB38(base, LIFTED_ARGS(0x8246C54C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r29,r23
  r29 = r23;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x8246c5f0
  if (cr6.eq) goto loc_8246C5F0;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // mr r4,r27
  r4 = r27;
  // bl 0x8245cd68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CD68(base, LIFTED_ARGS(0x8246C56C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8246c5f0
  if (cr6.eq) goto loc_8246C5F0;
  // mulli r30,r27,88
  r30 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(88))));
loc_8246C578:
  // lwz r11,188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 188);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // beq cr6,0x8246c598
  if (cr6.eq) goto loc_8246C598;
  // mr r3,r23
  r3 = r23;
loc_8246C598:
  // stw r3,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r3));
  // lwz r11,188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 188);
  // add r3,r30,r11
  r3 = r30 + r11;
  // bl 0x8246f140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246F140(base, LIFTED_ARGS(0x8246C5A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // mr r4,r27
  r4 = r27;
  // add r29,r5,r29
  r29 = r5 + r29;
  // bl 0x8245cbb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CBB0(base, LIFTED_ARGS(0x8246C5C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 188);
  // add r11,r30,r11
  r11 = r30 + r11;
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // subf. r11,r11,r10
  r11 = r10 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8246c5f0
  if (cr0.eq) goto loc_8246C5F0;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // mr r4,r27
  r4 = r27;
  // bl 0x8245cd68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CD68(base, LIFTED_ARGS(0x8246C5E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8246c578
  if (!cr6.eq) goto loc_8246C578;
loc_8246C5F0:
  // addi r11,r27,127
  r11 = uint64_t(int64_t(int64_t(r27) + 127));
  // subf r10,r29,r28
  r10 = r28 - r29;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lhzx r11,r11,r31
  r11 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r31));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bgt cr6,0x8246c60c
  if (cr6.gt) goto loc_8246C60C;
  // li r22,1
  r22 = uint64_t(int64_t(1));
loc_8246C60C:
  // mr r4,r28
  r4 = r28;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x8245f640
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245F640(base, LIFTED_ARGS(0x8246C618, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x8245f640
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245F640(base, LIFTED_ARGS(0x8246C624, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245d050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D050(base, LIFTED_ARGS(0x8246C630, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mulli r11,r27,88
  r11 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(88))));
  // stw r3,168(r31)
  REX_STORE_U32(uint32_t(r31) + 168, uint32_t(r3));
  // add r30,r11,r10
  r30 = r11 + r10;
  // lwz r11,28(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28);
  // lwz r10,24(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 24);
  // lbz r29,13(r30)
  r29 = REX_LOAD_U8(uint32_t(r30) + 13);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246c690
  if (cr6.eq) goto loc_8246C690;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8246c68c
  if (cr6.eq) goto loc_8246C68C;
  // rlwinm r26,r10,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r28,r11,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
loc_8246C668:
  // lwz r11,20(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20);
  // mr r5,r26
  r5 = r26;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r28,r11
  r3 = r28 + r11;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x8246C67C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,-1
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  // addi r28,r28,1024
  r28 = uint64_t(int64_t(int64_t(r28) + 1024));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x8246c668
  if (!cr6.eq) goto loc_8246C668;
loc_8246C68C:
  // li r26,1
  r26 = uint64_t(int64_t(1));
loc_8246C690:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // bne cr6,0x8246c6a4
  if (!cr6.eq) goto loc_8246C6A4;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8245f640
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245F640(base, LIFTED_ARGS(0x8246C6A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C6A4:
  // addi r11,r27,1
  r11 = uint64_t(int64_t(int64_t(r27) + 1));
  // lbz r10,56(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 56);
  // clrlwi r27,r11,24
  r27 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r27,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r10), so);
  // blt cr6,0x8246c540
  if (cr6.lt) goto loc_8246C540;
loc_8246C6B8:
  // mr r3,r31
  r3 = r31;
  // bl 0x8246b480
  // (left out: dead in C++)
  { const lifted::Result o = f8246B480(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246C6C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246c6e8
  if (cr6.eq) goto loc_8246C6E8;
  // lwz r10,8(r25)
  r10 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8246c6fc
  if (cr6.eq) goto loc_8246C6FC;
loc_8246C6E8:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246C6F0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r30));
  // stb r29,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r29));
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246C6FC:
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r7,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r7));
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c740
  if (cr6.eq) goto loc_8246C740;
  // mr r11,r23
  r11 = r23;
loc_8246C714:
  // lwz r9,188(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mulli r10,r11,88
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(88))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r8,24
  r11 = uint32_t(r8) & 0xFF;
  // lfs f0,40(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 40);
  f0 = double(temp.f32);
  // stfs f0,36(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 36, temp.u32);
  // lbz r10,56(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8246c714
  if (cr6.lt) goto loc_8246C714;
  // lwz r7,4(r25)
  r7 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246C740:
  // clrlwi r11,r26,24
  r11 = uint32_t(r26) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246cae0
  if (cr6.eq) goto loc_8246CAE0;
  // lbz r11,172(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 172);
  // rlwinm r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8246cab0
  if (!cr6.eq) goto loc_8246CAB0;
  // mr r10,r23
  r10 = r23;
  // mr r8,r23
  r8 = r23;
  // addi r9,r31,20
  r9 = uint64_t(int64_t(int64_t(r31) + 20));
loc_8246C768:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246ca34
  if (cr6.eq) goto loc_8246CA34;
  // lwz r11,8(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 8);
  // add r11,r11,r10
  r11 = r11 + r10;
  // b 0x8246ca38
  goto loc_8246CA38;
loc_8246C77C:
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c7dc
  if (cr6.eq) goto loc_8246C7DC;
  // lwz r8,96(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 96);
  // mr r11,r23
  r11 = r23;
  // li r6,256
  r6 = uint64_t(int64_t(256));
loc_8246C794:
  // lwz r7,188(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 188);
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // mulli r10,r11,88
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(88))));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // add r10,r10,r7
  r10 = r10 + r7;
  // rlwinm r7,r11,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // clrlwi r11,r9,24
  r11 = uint32_t(r9) & 0xFF;
  // stw r8,20(r10)
  REX_STORE_U32(uint32_t(r10) + 20, uint32_t(r8));
  // stw r6,24(r10)
  REX_STORE_U32(uint32_t(r10) + 24, uint32_t(r6));
  // stw r23,28(r10)
  REX_STORE_U32(uint32_t(r10) + 28, uint32_t(r23));
  // lbzx r10,r7,r31
  r10 = REX_LOAD_U8(uint32_t(r7) + uint32_t(r31));
  // lwz r8,96(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 96);
  // rotlwi r10,r10,10
  r10 = __builtin_rotateleft32(uint32_t(r10), 10);
  // lbz r9,56(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 56);
  // add r8,r10,r8
  r8 = r10 + r8;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // stw r8,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r8));
  // blt cr6,0x8246c794
  if (cr6.lt) goto loc_8246C794;
loc_8246C7DC:
  // lbz r11,172(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 172);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246c96c
  if (cr6.eq) goto loc_8246C96C;
  // rlwinm r10,r11,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8246c96c
  if (!cr6.eq) goto loc_8246C96C;
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // beq cr6,0x8246c89c
  if (cr6.eq) goto loc_8246C89C;
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c880
  if (cr6.eq) goto loc_8246C880;
  // mr r8,r23
  r8 = r23;
loc_8246C81C:
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mulli r11,r8,88
  r11 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(88))));
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f0,112(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 112);
  f0 = double(temp.f32);
  // mr r7,r23
  r7 = r23;
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // stfs f0,40(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 40, temp.u32);
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f0,40(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 40);
  f0 = double(temp.f32);
  // stfs f0,36(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 36, temp.u32);
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r10,r11,52
  r10 = uint64_t(int64_t(int64_t(r11) + 52));
  // mtctr r9
  ctr = r9;
loc_8246C858:
  // stw r7,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r7));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x8246c858
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8246C858;
  // addi r10,r8,1
  r10 = uint64_t(int64_t(int64_t(r8) + 1));
  // stfs f31,48(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // mr r11,r10
  r11 = r10;
  // lbz r10,56(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 56);
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x8246c81c
  if (cr6.lt) goto loc_8246C81C;
loc_8246C880:
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246C89C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246C89C:
  // lbz r11,172(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 172);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c91c
  if (cr6.eq) goto loc_8246C91C;
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // addi r11,r31,16
  r11 = uint64_t(int64_t(int64_t(r31) + 16));
  // lfs f13,40(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 40);
  f13 = double(temp.f32);
  // lhz r10,158(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 158);
  // addis r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 65536));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // sth r10,158(r11)
  REX_STORE_U16(uint32_t(r11) + 158, uint16_t(r10));
  // beq cr6,0x8246c8e4
  if (cr6.eq) goto loc_8246C8E4;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfs f0,-18092(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -18092);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // b 0x8246c8e8
  goto loc_8246C8E8;
loc_8246C8E4:
  // fmr f0,f31
  lifted::ScalarFloatMode();
  f0 = f31;
loc_8246C8E8:
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c91c
  if (cr6.eq) goto loc_8246C91C;
  // mr r11,r23
  r11 = r23;
loc_8246C8F8:
  // lwz r9,188(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mulli r10,r11,88
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(88))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r8,24
  r11 = uint32_t(r8) & 0xFF;
  // stfs f0,40(r10)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 40, temp.u32);
  // lbz r10,56(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8246c8f8
  if (cr6.lt) goto loc_8246C8F8;
loc_8246C91C:
  // lwz r9,4(r25)
  r9 = REX_LOAD_U32(uint32_t(r25) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8246c514
  if (cr6.eq) goto loc_8246C514;
  // lwz r11,8(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8246c514
  if (!cr6.eq) goto loc_8246C514;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bne cr6,0x8246c514
  if (!cr6.eq) goto loc_8246C514;
  // mr r11,r23
  r11 = r23;
  // lbz r30,12(r25)
  r30 = REX_LOAD_U8(uint32_t(r25) + 12);
  // mr r3,r25
  r3 = r25;
  // stb r11,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r11));
  // stw r11,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246C960, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246C968, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8246c514
  goto loc_8246C514;
loc_8246C96C:
  // lbz r11,56(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c9e0
  if (cr6.eq) goto loc_8246C9E0;
  // mr r26,r23
  r26 = r23;
loc_8246C97C:
  // lwz r10,188(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mulli r11,r26,88
  r11 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(88))));
  // add r28,r11,r10
  r28 = r11 + r10;
  // lwz r11,24(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 24);
  // lbz r30,13(r28)
  r30 = REX_LOAD_U8(uint32_t(r28) + 13);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246c9cc
  if (cr6.eq) goto loc_8246C9CC;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8246c9cc
  if (cr6.eq) goto loc_8246C9CC;
  // rlwinm r27,r11,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r29,r23
  r29 = r23;
loc_8246C9A8:
  // lwz r11,20(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 20);
  // mr r5,r27
  r5 = r27;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r29,r11
  r3 = r29 + r11;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x8246C9BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r29,r29,1024
  r29 = uint64_t(int64_t(int64_t(r29) + 1024));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8246c9a8
  if (!cr6.eq) goto loc_8246C9A8;
loc_8246C9CC:
  // addi r11,r26,1
  r11 = uint64_t(int64_t(int64_t(r26) + 1));
  // lbz r10,56(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 56);
  // clrlwi r26,r11,24
  r26 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r26,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r10), so);
  // blt cr6,0x8246c97c
  if (cr6.lt) goto loc_8246C97C;
loc_8246C9E0:
  // lwz r9,4(r25)
  r9 = REX_LOAD_U32(uint32_t(r25) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8246ca2c
  if (cr6.eq) goto loc_8246CA2C;
  // lwz r11,8(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8246ca2c
  if (!cr6.eq) goto loc_8246CA2C;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bne cr6,0x8246ca2c
  if (!cr6.eq) goto loc_8246CA2C;
  // mr r11,r23
  r11 = r23;
  // lbz r31,12(r25)
  r31 = REX_LOAD_U8(uint32_t(r25) + 12);
  // mr r3,r25
  r3 = r25;
  // stb r11,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r11));
  // stw r11,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246CA24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246CA2C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CA2C:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x8246c3ac
  goto loc_8246C3AC;
loc_8246CA34:
  // mr r11,r9
  r11 = r9;
loc_8246CA38:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x8246ca50
  if (cr6.eq) goto loc_8246CA50;
  // lwz r10,8(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // b 0x8246ca54
  goto loc_8246CA54;
loc_8246CA50:
  // mr r11,r23
  r11 = r23;
loc_8246CA54:
  // mr r10,r11
  r10 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246ca84
  if (cr6.eq) goto loc_8246CA84;
  // lbz r11,116(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 116);
  // clrlwi r8,r8,24
  r8 = uint32_t(r8) & 0xFF;
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // or r11,r11,r8
  r11 = r11 | r8;
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // b 0x8246c768
  goto loc_8246C768;
loc_8246CA84:
  // clrlwi r11,r8,24
  r11 = uint32_t(r8) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246cae0
  if (!cr6.eq) goto loc_8246CAE0;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246CAAC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8246cadc
  goto loc_8246CADC;
loc_8246CAB0:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x8246cae0
  if (!cr6.eq) goto loc_8246CAE0;
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246cae0
  if (!cr6.eq) goto loc_8246CAE0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246CADC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CADC:
  // lwz r7,4(r25)
  r7 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246CAE0:
  // lbz r11,172(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 172);
  // rlwinm r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246cb24
  if (cr6.eq) goto loc_8246CB24;
  // lhz r10,174(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 174);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246cb08
  if (cr6.eq) goto loc_8246CB08;
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246cb24
  if (cr6.eq) goto loc_8246CB24;
loc_8246CB08:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246CB20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r7,4(r25)
  r7 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246CB24:
  // lbz r11,172(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 172);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246cb40
  if (cr6.eq) goto loc_8246CB40;
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246cce0
  if (cr6.eq) goto loc_8246CCE0;
loc_8246CB40:
  // addi r27,r31,20
  r27 = uint64_t(int64_t(int64_t(r31) + 20));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // beq cr6,0x8246cb5c
  if (cr6.eq) goto loc_8246CB5C;
  // lwz r10,8(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // b 0x8246cb60
  goto loc_8246CB60;
loc_8246CB5C:
  // mr r11,r23
  r11 = r23;
loc_8246CB60:
  // mr r28,r11
  r28 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246cce0
  if (cr6.eq) goto loc_8246CCE0;
loc_8246CB6C:
  // mr r29,r28
  r29 = r28;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x8246cb84
  if (cr6.eq) goto loc_8246CB84;
  // lwz r11,8(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 8);
  // add r11,r11,r28
  r11 = r11 + r28;
  // b 0x8246cb88
  goto loc_8246CB88;
loc_8246CB84:
  // mr r11,r27
  r11 = r27;
loc_8246CB88:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // beq cr6,0x8246cba0
  if (cr6.eq) goto loc_8246CBA0;
  // lwz r10,8(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 8);
  // subf r28,r10,r11
  r28 = r11 - r10;
  // b 0x8246cba4
  goto loc_8246CBA4;
loc_8246CBA0:
  // mr r28,r23
  r28 = r23;
loc_8246CBA4:
  // lbz r11,116(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 116);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x8246ccd8
  if (!cr6.eq) goto loc_8246CCD8;
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq cr6,0x8246cbf8
  if (cr6.eq) goto loc_8246CBF8;
  // lwz r10,8(r25)
  r10 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8246cbf8
  if (!cr6.eq) goto loc_8246CBF8;
  // addi r11,r7,-1
  r11 = uint64_t(int64_t(int64_t(r7) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bne cr6,0x8246cbf8
  if (!cr6.eq) goto loc_8246CBF8;
  // mr r11,r23
  r11 = r23;
  // lbz r30,12(r25)
  r30 = REX_LOAD_U8(uint32_t(r25) + 12);
  // mr r3,r25
  r3 = r25;
  // stb r11,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r11));
  // stw r11,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246CBF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246CBF8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CBF8:
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246cc3c
  if (cr6.eq) goto loc_8246CC3C;
  // lwz r9,8(r24)
  r9 = REX_LOAD_U32(uint32_t(r24) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246cc3c
  if (!cr6.eq) goto loc_8246CC3C;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r11));
  // bne 0x8246cc3c
  if (!cr0.eq) goto loc_8246CC3C;
  // mr r3,r24
  r3 = r24;
  // lbz r30,12(r24)
  r30 = REX_LOAD_U8(uint32_t(r24) + 12);
  // stw r23,8(r24)
  REX_STORE_U32(uint32_t(r24) + 8, uint32_t(r23));
  // stb r23,12(r24)
  REX_STORE_U8(uint32_t(r24) + 12, uint8_t(r23));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246CC34, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246CC3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CC3C:
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,92(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 92);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246CC58, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246CC5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246cc7c
  if (cr6.eq) goto loc_8246CC7C;
  // lwz r11,8(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246cc8c
  if (cr6.eq) goto loc_8246CC8C;
loc_8246CC7C:
  // mr r3,r24
  r3 = r24;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246CC84, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r24)
  REX_STORE_U32(uint32_t(r24) + 8, uint32_t(r30));
  // stb r29,12(r24)
  REX_STORE_U8(uint32_t(r24) + 12, uint8_t(r29));
loc_8246CC8C:
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r11));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeRaiseIrqlToDpcLevel, LIFTED_ARGS(0x8246CC9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246ccbc
  if (cr6.eq) goto loc_8246CCBC;
  // lwz r10,8(r25)
  r10 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // beq cr6,0x8246ccd0
  if (cr6.eq) goto loc_8246CCD0;
loc_8246CCBC:
  // mr r3,r25
  r3 = r25;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeAcquireSpinLockAtRaisedIrql, LIFTED_ARGS(0x8246CCC4, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r30));
  // stb r29,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r29));
  // lwz r11,4(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 4);
loc_8246CCD0:
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r7,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r7));
loc_8246CCD8:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8246cb6c
  if (!cr6.eq) goto loc_8246CB6C;
loc_8246CCE0:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq cr6,0x8246cd28
  if (cr6.eq) goto loc_8246CD28;
  // lwz r10,8(r25)
  r10 = REX_LOAD_U32(uint32_t(r25) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8246cd28
  if (!cr6.eq) goto loc_8246CD28;
  // addi r11,r7,-1
  r11 = uint64_t(int64_t(int64_t(r7) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // bne cr6,0x8246cd28
  if (!cr6.eq) goto loc_8246CD28;
  // mr r11,r23
  r11 = r23;
  // lbz r30,12(r25)
  r30 = REX_LOAD_U8(uint32_t(r25) + 12);
  // mr r3,r25
  r3 = r25;
  // stb r11,12(r25)
  REX_STORE_U8(uint32_t(r25) + 12, uint8_t(r11));
  // stw r11,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246CD20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246CD28, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CD28:
  // clrlwi r11,r22,24
  r11 = uint32_t(r22) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246cd58
  if (cr6.eq) goto loc_8246CD58;
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245c948
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245C948(base, LIFTED_ARGS(0x8246CD3C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246cd58
  if (cr6.eq) goto loc_8246CD58;
  // lwz r3,184(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 184);
  // bl 0x8245d4d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D4D0(base, LIFTED_ARGS(0x8246CD4C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x8246c3ac
  if (cr6.lt) goto loc_8246C3AC;
loc_8246CD58:
  // lwz r11,4(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 4);
  // mr r10,r13
  r10 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246cd9c
  if (cr6.eq) goto loc_8246CD9C;
  // lwz r9,8(r24)
  r9 = REX_LOAD_U32(uint32_t(r24) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246cd9c
  if (!cr6.eq) goto loc_8246CD9C;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r24)
  REX_STORE_U32(uint32_t(r24) + 4, uint32_t(r11));
  // bne 0x8246cd9c
  if (!cr0.eq) goto loc_8246CD9C;
  // lbz r31,12(r24)
  r31 = REX_LOAD_U8(uint32_t(r24) + 12);
  // mr r3,r24
  r3 = r24;
  // stw r23,8(r24)
  REX_STORE_U32(uint32_t(r24) + 8, uint32_t(r23));
  // stb r23,12(r24)
  REX_STORE_U8(uint32_t(r24) + 12, uint8_t(r23));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeReleaseSpinLockFromRaisedIrql, LIFTED_ARGS(0x8246CD94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KfLowerIrql, LIFTED_ARGS(0x8246CD9C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246CD9C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f31,-96(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246CE60: lifted from generated/ by lift.py.
uint64_t f8246CE60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246CE60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246cf50
  { const lifted::Result o = lifted::Call(base, 0x8246CF50, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_8246CEC0: lifted from generated/ by lift.py.
lifted::Result f8246CEC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246CEC0 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // b 0x8246a1c0
  { const lifted::Result o = lifted::Call(base, 0x8246A1C0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246CED0: lifted from generated/ by lift.py.
uint64_t f8246CED0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246CED0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lhz r10,108(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 108);
  // lwz r11,120(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 120);
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_8246CEF0: lifted from generated/ by lift.py.
lifted::Result f8246CEF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246CEF0 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // mr r31,r4
  r31 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246CF1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x8246cf2c
  if (cr6.lt) goto loc_8246CF2C;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r31,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r31));
loc_8246CF2C:
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

// sub_8246CF40: lifted from generated/ by lift.py.
void f8246CF40(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8246CF40 (lifted)");
  uint64_t r11 = 0;
  // addi r11,r3,-4
  r11 = uint64_t(int64_t(int64_t(r3) + -4));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_8246D2E0: lifted from generated/ by lift.py.
uint64_t f8246D2E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D2E0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82466e80
  { const lifted::Result o = lifted::entry::e82466E80(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_8246D2E8: lifted from generated/ by lift.py.
lifted::Result f8246D2E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D2E8 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246ceb8
  { const lifted::Result o = lifted::entry::e8246CEB8(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D2F0: lifted from generated/ by lift.py.
lifted::Result f8246D2F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D2F0 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246d008
  { const lifted::Result o = lifted::entry::e8246D008(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D2F8: lifted from generated/ by lift.py.
lifted::Result f8246D2F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D2F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246cec8
  { const lifted::Result o = lifted::entry::e8246CEC8(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D300: lifted from generated/ by lift.py.
lifted::Result f8246D300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246D300 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x8246cef0
  { const lifted::Result o = f8246CEF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8246D308: lifted from generated/ by lift.py.
lifted::Result f8246D308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8246D308 (lifted)");
  uint64_t r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x8246ce70
  { const lifted::Result o = lifted::entry::e8246CE70(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

lifted::Result e8245FE88(uint8_t* base, const lifted::Args& a) {
  return f8245FE88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824601F8(uint8_t* base, const lifted::Args& a) {
  f824601F8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82460210(uint8_t* base, const lifted::Args& a) {
  return f82460210(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82460368(uint8_t* base, const lifted::Args& a) {
  return f82460368(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82460398(uint8_t* base, const lifted::Args& a) {
  return f82460398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82460410(uint8_t* base, const lifted::Args& a) {
  return f82460410(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82460470(uint8_t* base, const lifted::Args& a) {
  return f82460470(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82460538(uint8_t* base, const lifted::Args& a) {
  return f82460538(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82460E90(uint8_t* base, const lifted::Args& a) {
  return f82460E90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e824613A0(uint8_t* base, const lifted::Args& a) {
  return f824613A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82461490(uint8_t* base, const lifted::Args& a) {
  return {f82461490(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e824614C8(uint8_t* base, const lifted::Args& a) {
  return {f824614C8(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e824615A0(uint8_t* base, const lifted::Args& a) {
  return f824615A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82461F10(uint8_t* base, const lifted::Args& a) {
  return {f82461F10(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82461F20(uint8_t* base, const lifted::Args& a) {
  return f82461F20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82465398(uint8_t* base, const lifted::Args& a) {
  return f82465398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82465418(uint8_t* base, const lifted::Args& a) {
  return {f82465418(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82465590(uint8_t* base, const lifted::Args& a) {
  return f82465590(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82465848(uint8_t* base, const lifted::Args& a) {
  return f82465848(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e82465850(uint8_t* base, const lifted::Args& a) {
  return {f82465850(base, a.r[1]), a.f[0]};
}

lifted::Result e82465868(uint8_t* base, const lifted::Args& a) {
  f82465868(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e824658B8(uint8_t* base, const lifted::Args& a) {
  return {f824658B8(base, a.r[1]), a.f[0]};
}

lifted::Result e82465970(uint8_t* base, const lifted::Args& a) {
  return f82465970(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82465A48(uint8_t* base, const lifted::Args& a) {
  return f82465A48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82465B48(uint8_t* base, const lifted::Args& a) {
  return f82465B48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82465BF8(uint8_t* base, const lifted::Args& a) {
  return {f82465BF8(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82465C18(uint8_t* base, const lifted::Args& a) {
  return f82465C18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82465CF8(uint8_t* base, const lifted::Args& a) {
  return f82465CF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82466058(uint8_t* base, const lifted::Args& a) {
  return f82466058(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82466410(uint8_t* base, const lifted::Args& a) {
  return {f82466410(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82466990(uint8_t* base, const lifted::Args& a) {
  return f82466990(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82466A80(uint8_t* base, const lifted::Args& a) {
  return f82466A80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82466C40(uint8_t* base, const lifted::Args& a) {
  return f82466C40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82466C48(uint8_t* base, const lifted::Args& a) {
  return {f82466C48(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82466C58(uint8_t* base, const lifted::Args& a) {
  return f82466C58(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82466C60(uint8_t* base, const lifted::Args& a) {
  return f82466C60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82466CB0(uint8_t* base, const lifted::Args& a) {
  return f82466CB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82466D00(uint8_t* base, const lifted::Args& a) {
  return f82466D00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82466D50(uint8_t* base, const lifted::Args& a) {
  return {f82466D50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82466DB0(uint8_t* base, const lifted::Args& a) {
  return {f82466DB0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82466DC0(uint8_t* base, const lifted::Args& a) {
  return {f82466DC0(base, a.r[0], a.f[0]), a.f[0]};
}

lifted::Result e82466DD0(uint8_t* base, const lifted::Args& a) {
  return f82466DD0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e82466E18(uint8_t* base, const lifted::Args& a) {
  return f82466E18(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82466E60(uint8_t* base, const lifted::Args& a) {
  return {f82466E60(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82466E70(uint8_t* base, const lifted::Args& a) {
  return {f82466E70(base, a.r[0], a.f[0]), a.f[0]};
}

lifted::Result e82466F40(uint8_t* base, const lifted::Args& a) {
  return f82466F40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82467140(uint8_t* base, const lifted::Args& a) {
  return f82467140(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82467230(uint8_t* base, const lifted::Args& a) {
  return f82467230(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82467360(uint8_t* base, const lifted::Args& a) {
  return f82467360(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82467450(uint8_t* base, const lifted::Args& a) {
  return f82467450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82469A88(uint8_t* base, const lifted::Args& a) {
  return f82469A88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246A698(uint8_t* base, const lifted::Args& a) {
  return {f8246A698(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8246A758(uint8_t* base, const lifted::Args& a) {
  return f8246A758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8246A7D0(uint8_t* base, const lifted::Args& a) {
  return f8246A7D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246A900(uint8_t* base, const lifted::Args& a) {
  return f8246A900(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.lr);
}

lifted::Result e8246A908(uint8_t* base, const lifted::Args& a) {
  return f8246A908(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246A910(uint8_t* base, const lifted::Args& a) {
  return f8246A910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246A930(uint8_t* base, const lifted::Args& a) {
  return {f8246A930(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246A940(uint8_t* base, const lifted::Args& a) {
  return f8246A940(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e8246A988(uint8_t* base, const lifted::Args& a) {
  return {f8246A988(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246A998(uint8_t* base, const lifted::Args& a) {
  return f8246A998(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246A9F0(uint8_t* base, const lifted::Args& a) {
  return f8246A9F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246AA48(uint8_t* base, const lifted::Args& a) {
  return {f8246AA48(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246AA58(uint8_t* base, const lifted::Args& a) {
  return {f8246AA58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8246AAC0(uint8_t* base, const lifted::Args& a) {
  return f8246AAC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246AAC8(uint8_t* base, const lifted::Args& a) {
  return f8246AAC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246AB00(uint8_t* base, const lifted::Args& a) {
  return f8246AB00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8246B0C8(uint8_t* base, const lifted::Args& a) {
  return f8246B0C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246B1C8(uint8_t* base, const lifted::Args& a) {
  return f8246B1C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246B318(uint8_t* base, const lifted::Args& a) {
  return f8246B318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246B3F0(uint8_t* base, const lifted::Args& a) {
  return f8246B3F0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e8246B3F8(uint8_t* base, const lifted::Args& a) {
  return f8246B3F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8246B400(uint8_t* base, const lifted::Args& a) {
  return f8246B400(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246B408(uint8_t* base, const lifted::Args& a) {
  return f8246B408(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246B410(uint8_t* base, const lifted::Args& a) {
  return {f8246B410(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246B418(uint8_t* base, const lifted::Args& a) {
  return {f8246B418(base, a.r1, a.r13, a.r[0], a.r[1], a.lr), a.f[0]};
}

lifted::Result e8246B420(uint8_t* base, const lifted::Args& a) {
  return f8246B420(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246B428(uint8_t* base, const lifted::Args& a) {
  return {f8246B428(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8246B430(uint8_t* base, const lifted::Args& a) {
  return f8246B430(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246B438(uint8_t* base, const lifted::Args& a) {
  return f8246B438(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246B440(uint8_t* base, const lifted::Args& a) {
  return {f8246B440(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246B448(uint8_t* base, const lifted::Args& a) {
  return f8246B448(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246B450(uint8_t* base, const lifted::Args& a) {
  return f8246B450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246BE20(uint8_t* base, const lifted::Args& a) {
  return f8246BE20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246BFD8(uint8_t* base, const lifted::Args& a) {
  return f8246BFD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246C1B8(uint8_t* base, const lifted::Args& a) {
  return f8246C1B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246C1C0(uint8_t* base, const lifted::Args& a) {
  return f8246C1C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246C1C8(uint8_t* base, const lifted::Args& a) {
  return f8246C1C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246C320(uint8_t* base, const lifted::Args& a) {
  return f8246C320(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8246CE60(uint8_t* base, const lifted::Args& a) {
  return {f8246CE60(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e8246CEC0(uint8_t* base, const lifted::Args& a) {
  return f8246CEC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8246CED0(uint8_t* base, const lifted::Args& a) {
  return {f8246CED0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8246CEF0(uint8_t* base, const lifted::Args& a) {
  return f8246CEF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246CF40(uint8_t* base, const lifted::Args& a) {
  f8246CF40(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8246D2E0(uint8_t* base, const lifted::Args& a) {
  return {f8246D2E0(base, a.r1, a.r13, a.r[0], a.r[1], a.lr), a.f[0]};
}

lifted::Result e8246D2E8(uint8_t* base, const lifted::Args& a) {
  return f8246D2E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8246D2F0(uint8_t* base, const lifted::Args& a) {
  return f8246D2F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246D2F8(uint8_t* base, const lifted::Args& a) {
  return f8246D2F8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8246D300(uint8_t* base, const lifted::Args& a) {
  return f8246D300(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8246D308(uint8_t* base, const lifted::Args& a) {
  return f8246D308(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

namespace {

const Registration kEntries[] = {
    {0x8245FE88, e8245FE88},
    {0x824601F8, e824601F8},
    {0x82460210, e82460210},
    {0x82460368, e82460368},
    {0x82460398, e82460398},
    {0x82460410, e82460410},
    {0x82460470, e82460470},
    {0x82460538, e82460538},
    {0x82460E90, e82460E90},
    {0x824613A0, e824613A0},
    {0x82461490, e82461490},
    {0x824614C8, e824614C8},
    {0x824615A0, e824615A0},
    {0x82461F10, e82461F10},
    {0x82461F20, e82461F20},
    {0x82465398, e82465398},
    {0x82465418, e82465418},
    {0x82465590, e82465590},
    {0x82465848, e82465848},
    {0x82465850, e82465850},
    {0x82465868, e82465868},
    {0x824658B8, e824658B8},
    {0x82465970, e82465970},
    {0x82465A48, e82465A48},
    {0x82465B48, e82465B48},
    {0x82465BF8, e82465BF8},
    {0x82465C18, e82465C18},
    {0x82465CF8, e82465CF8},
    {0x82466058, e82466058},
    {0x82466410, e82466410},
    {0x82466990, e82466990},
    {0x82466A80, e82466A80},
    {0x82466C40, e82466C40},
    {0x82466C48, e82466C48},
    {0x82466C58, e82466C58},
    {0x82466C60, e82466C60},
    {0x82466CB0, e82466CB0},
    {0x82466D00, e82466D00},
    {0x82466D50, e82466D50},
    {0x82466DB0, e82466DB0},
    {0x82466DC0, e82466DC0},
    {0x82466DD0, e82466DD0},
    {0x82466E18, e82466E18},
    {0x82466E60, e82466E60},
    {0x82466E70, e82466E70},
    {0x82466F40, e82466F40},
    {0x82467140, e82467140},
    {0x82467230, e82467230},
    {0x82467360, e82467360},
    {0x82467450, e82467450},
    {0x82469A88, e82469A88},
    {0x8246A698, e8246A698},
    {0x8246A758, e8246A758},
    {0x8246A7D0, e8246A7D0},
    {0x8246A900, e8246A900},
    {0x8246A908, e8246A908},
    {0x8246A910, e8246A910},
    {0x8246A930, e8246A930},
    {0x8246A940, e8246A940},
    {0x8246A988, e8246A988},
    {0x8246A998, e8246A998},
    {0x8246A9F0, e8246A9F0},
    {0x8246AA48, e8246AA48},
    {0x8246AA58, e8246AA58},
    {0x8246AAC0, e8246AAC0},
    {0x8246AAC8, e8246AAC8},
    {0x8246AB00, e8246AB00},
    {0x8246B0C8, e8246B0C8},
    {0x8246B1C8, e8246B1C8},
    {0x8246B318, e8246B318},
    {0x8246B3F0, e8246B3F0},
    {0x8246B3F8, e8246B3F8},
    {0x8246B400, e8246B400},
    {0x8246B408, e8246B408},
    {0x8246B410, e8246B410},
    {0x8246B418, e8246B418},
    {0x8246B420, e8246B420},
    {0x8246B428, e8246B428},
    {0x8246B430, e8246B430},
    {0x8246B438, e8246B438},
    {0x8246B440, e8246B440},
    {0x8246B448, e8246B448},
    {0x8246B450, e8246B450},
    {0x8246BE20, e8246BE20},
    {0x8246BFD8, e8246BFD8},
    {0x8246C1B8, e8246C1B8},
    {0x8246C1C0, e8246C1C0},
    {0x8246C1C8, e8246C1C8},
    {0x8246C320, e8246C320},
    {0x8246CE60, e8246CE60},
    {0x8246CEC0, e8246CEC0},
    {0x8246CED0, e8246CED0},
    {0x8246CEF0, e8246CEF0},
    {0x8246CF40, e8246CF40},
    {0x8246D2E0, e8246D2E0},
    {0x8246D2E8, e8246D2E8},
    {0x8246D2F0, e8246D2F0},
    {0x8246D2F8, e8246D2F8},
    {0x8246D300, e8246D300},
    {0x8246D308, e8246D308},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_016)), true);

}  // namespace

}  // namespace lifted
