// Lift sweep batch 001 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_001, true, "ITC",
              "Run lift sweep batch 001's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82090148: lifted from generated/ by lift.py.
lifted::Result f82090148(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82090148 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2616
  r11 = uint64_t(int64_t(int64_t(r11) + 2616));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82090174
  if (cr0.eq) goto loc_82090174;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82090174, r31)); r3 = o.r3; f1 = o.f1; }
loc_82090174:
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

// sub_82090190: lifted from generated/ by lift.py.
lifted::Result f82090190(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82090190 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2640
  r11 = uint64_t(int64_t(int64_t(r11) + 2640));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820901bc
  if (cr0.eq) goto loc_820901BC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820901BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_820901BC:
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

// sub_820901D8: lifted from generated/ by lift.py.
lifted::Result f820901D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820901D8 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2676
  r11 = uint64_t(int64_t(int64_t(r11) + 2676));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82090204
  if (cr0.eq) goto loc_82090204;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82090204, r31)); r3 = o.r3; f1 = o.f1; }
loc_82090204:
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

// sub_82090220: lifted from generated/ by lift.py.
lifted::Result f82090220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82090220 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2696
  r11 = uint64_t(int64_t(int64_t(r11) + 2696));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x8209024c
  if (cr0.eq) goto loc_8209024C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8209024C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209024C:
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

// sub_82090268: lifted from generated/ by lift.py.
lifted::Result f82090268(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82090268 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82090294
  if (cr0.eq) goto loc_82090294;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82090294, r31)); r3 = o.r3; f1 = o.f1; }
loc_82090294:
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

// sub_82090350: lifted from generated/ by lift.py.
lifted::Result f82090350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090350 (lifted)");
  uint64_t r4 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090380: lifted from generated/ by lift.py.
lifted::Result f82090380(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090380 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // beq 0x820903bc
  if (cr0.eq) goto loc_820903BC;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820903B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_820903BC:
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

// sub_820903D0: lifted from generated/ by lift.py.
lifted::Result f820903D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820903D0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // beq 0x8209040c
  if (cr0.eq) goto loc_8209040C;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82090404, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8209040C:
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

// sub_820905E0: lifted from generated/ by lift.py.
void f820905E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820905E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r30,r30,2804
  r30 = uint64_t(int64_t(int64_t(r30) + 2804));
  // addi r11,r11,2956
  r11 = uint64_t(int64_t(int64_t(r11) + 2956));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // stw r30,40(r3)
  REX_STORE_U32(uint32_t(r3) + 40, uint32_t(r30));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r4,-32256
  r4 = uint64_t(int64_t(-2113929216));
  // lis r31,-32256
  r31 = uint64_t(int64_t(-2113929216));
  // lis r29,-32256
  r29 = uint64_t(int64_t(-2113929216));
  // lis r28,-32256
  r28 = uint64_t(int64_t(-2113929216));
  // lis r27,-32256
  r27 = uint64_t(int64_t(-2113929216));
  // lis r26,-32256
  r26 = uint64_t(int64_t(-2113929216));
  // lis r25,-32256
  r25 = uint64_t(int64_t(-2113929216));
  // lis r24,-32255
  r24 = uint64_t(int64_t(-2113863680));
  // lis r23,-32256
  r23 = uint64_t(int64_t(-2113929216));
  // addi r10,r10,2948
  r10 = uint64_t(int64_t(int64_t(r10) + 2948));
  // addi r9,r9,2944
  r9 = uint64_t(int64_t(int64_t(r9) + 2944));
  // addi r8,r8,2940
  r8 = uint64_t(int64_t(int64_t(r8) + 2940));
  // addi r7,r7,2928
  r7 = uint64_t(int64_t(int64_t(r7) + 2928));
  // addi r6,r6,2892
  r6 = uint64_t(int64_t(int64_t(r6) + 2892));
  // addi r5,r5,2872
  r5 = uint64_t(int64_t(int64_t(r5) + 2872));
  // stw r10,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r10));
  // addi r4,r4,2856
  r4 = uint64_t(int64_t(int64_t(r4) + 2856));
  // stw r9,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r9));
  // addi r31,r31,2812
  r31 = uint64_t(int64_t(int64_t(r31) + 2812));
  // stw r8,16(r3)
  REX_STORE_U32(uint32_t(r3) + 16, uint32_t(r8));
  // addi r29,r29,2796
  r29 = uint64_t(int64_t(int64_t(r29) + 2796));
  // stw r7,20(r3)
  REX_STORE_U32(uint32_t(r3) + 20, uint32_t(r7));
  // addi r28,r28,2788
  r28 = uint64_t(int64_t(int64_t(r28) + 2788));
  // stw r6,24(r3)
  REX_STORE_U32(uint32_t(r3) + 24, uint32_t(r6));
  // addi r27,r27,2772
  r27 = uint64_t(int64_t(int64_t(r27) + 2772));
  // stw r5,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r5));
  // addi r26,r26,2764
  r26 = uint64_t(int64_t(int64_t(r26) + 2764));
  // stw r4,32(r3)
  REX_STORE_U32(uint32_t(r3) + 32, uint32_t(r4));
  // addi r25,r25,2760
  r25 = uint64_t(int64_t(int64_t(r25) + 2760));
  // stw r31,36(r3)
  REX_STORE_U32(uint32_t(r3) + 36, uint32_t(r31));
  // lis r22,-32256
  r22 = uint64_t(int64_t(-2113929216));
  // stw r29,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r29));
  // addi r24,r24,2908
  r24 = uint64_t(int64_t(int64_t(r24) + 2908));
  // stw r28,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r28));
  // lis r21,-32256
  r21 = uint64_t(int64_t(-2113929216));
  // stw r27,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r27));
  // addi r23,r23,2740
  r23 = uint64_t(int64_t(int64_t(r23) + 2740));
  // stw r26,56(r3)
  REX_STORE_U32(uint32_t(r3) + 56, uint32_t(r26));
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // stw r25,60(r3)
  REX_STORE_U32(uint32_t(r3) + 60, uint32_t(r25));
  // addi r22,r22,3400
  r22 = uint64_t(int64_t(int64_t(r22) + 3400));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r24,60(r3)
  REX_STORE_U32(uint32_t(r3) + 60, uint32_t(r24));
  // addi r21,r21,2740
  r21 = uint64_t(int64_t(int64_t(r21) + 2740));
  // addi r30,r30,2740
  r30 = uint64_t(int64_t(int64_t(r30) + 2740));
  // stw r23,56(r3)
  REX_STORE_U32(uint32_t(r3) + 56, uint32_t(r23));
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // stw r22,52(r3)
  REX_STORE_U32(uint32_t(r3) + 52, uint32_t(r22));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // stw r21,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r21));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stw r30,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r30));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // stw r11,40(r3)
  REX_STORE_U32(uint32_t(r3) + 40, uint32_t(r11));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r5,-32255
  r5 = uint64_t(int64_t(-2113863680));
  // lis r4,-32256
  r4 = uint64_t(int64_t(-2113929216));
  // lis r31,-32255
  r31 = uint64_t(int64_t(-2113863680));
  // addi r10,r10,2696
  r10 = uint64_t(int64_t(int64_t(r10) + 2696));
  // addi r9,r9,3400
  r9 = uint64_t(int64_t(int64_t(r9) + 3400));
  // addi r8,r8,2676
  r8 = uint64_t(int64_t(int64_t(r8) + 2676));
  // addi r7,r7,2640
  r7 = uint64_t(int64_t(int64_t(r7) + 2640));
  // addi r6,r6,2748
  r6 = uint64_t(int64_t(int64_t(r6) + 2748));
  // addi r11,r11,2908
  r11 = uint64_t(int64_t(int64_t(r11) + 2908));
  // addi r5,r5,2908
  r5 = uint64_t(int64_t(int64_t(r5) + 2908));
  // addi r4,r4,2740
  r4 = uint64_t(int64_t(int64_t(r4) + 2740));
  // addi r31,r31,2908
  r31 = uint64_t(int64_t(int64_t(r31) + 2908));
  // stw r10,36(r3)
  REX_STORE_U32(uint32_t(r3) + 36, uint32_t(r10));
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // stw r9,32(r3)
  REX_STORE_U32(uint32_t(r3) + 32, uint32_t(r9));
  // stw r8,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r8));
  // addi r30,r30,2616
  r30 = uint64_t(int64_t(int64_t(r30) + 2616));
  // stw r7,24(r3)
  REX_STORE_U32(uint32_t(r3) + 24, uint32_t(r7));
  // stw r6,20(r3)
  REX_STORE_U32(uint32_t(r3) + 20, uint32_t(r6));
  // stw r11,16(r3)
  REX_STORE_U32(uint32_t(r3) + 16, uint32_t(r11));
  // stw r5,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r5));
  // stw r4,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r4));
  // stw r31,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r30,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r30));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return;
}

// sub_82090780: lifted from generated/ by lift.py.
uint64_t f82090780(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090780 (lifted)");
  // lwz r3,80(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 80);
  // blr
  return r3;
}

// sub_82090790: lifted from generated/ by lift.py.
lifted::Result f82090790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090790 (lifted)");
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
  // bl 0x820905e0
  // (left out: dead in C++)
  f820905E0(base, r3);
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820907c0
  if (cr0.eq) goto loc_820907C0;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820907C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820907C0:
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

// sub_820907E0: lifted from generated/ by lift.py.
lifted::Result f820907E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820907E0 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820907E8: lifted from generated/ by lift.py.
lifted::Result f820907E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820907E8 (lifted)");
  // addi r3,r3,-40
  r3 = uint64_t(int64_t(int64_t(r3) + -40));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820907F0: lifted from generated/ by lift.py.
uint64_t f820907F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820907F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82090778
  { const lifted::Result o = lifted::Call(base, 0x82090778, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820907F8: lifted from generated/ by lift.py.
lifted::Result f820907F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820907F8 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090800: lifted from generated/ by lift.py.
lifted::Result f82090800(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090800 (lifted)");
  // addi r3,r3,-44
  r3 = uint64_t(int64_t(int64_t(r3) + -44));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090808: lifted from generated/ by lift.py.
uint64_t f82090808(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090808 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82090778
  { const lifted::Result o = lifted::Call(base, 0x82090778, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82090810: lifted from generated/ by lift.py.
lifted::Result f82090810(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090810 (lifted)");
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090818: lifted from generated/ by lift.py.
lifted::Result f82090818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090818 (lifted)");
  // addi r3,r3,-48
  r3 = uint64_t(int64_t(int64_t(r3) + -48));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090820: lifted from generated/ by lift.py.
uint64_t f82090820(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090820 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x8248f030
  { const lifted::Result o = lifted::Call(base, 0x8248F030, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82090848: lifted from generated/ by lift.py.
lifted::Result f82090848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090848 (lifted)");
  // addi r3,r3,-20
  r3 = uint64_t(int64_t(int64_t(r3) + -20));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090850: lifted from generated/ by lift.py.
lifted::Result f82090850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090850 (lifted)");
  // addi r3,r3,-52
  r3 = uint64_t(int64_t(int64_t(r3) + -52));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090858: lifted from generated/ by lift.py.
uint64_t f82090858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090858 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x82117770
  { const lifted::Result o = lifted::Call(base, 0x82117770, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82090870: lifted from generated/ by lift.py.
lifted::Result f82090870(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090870 (lifted)");
  // addi r3,r3,-24
  r3 = uint64_t(int64_t(int64_t(r3) + -24));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090878: lifted from generated/ by lift.py.
lifted::Result f82090878(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090878 (lifted)");
  // addi r3,r3,-56
  r3 = uint64_t(int64_t(int64_t(r3) + -56));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090880: lifted from generated/ by lift.py.
uint64_t f82090880(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090880 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-20
  r3 = uint64_t(int64_t(int64_t(r3) + -20));
  // b 0x8248f030
  { const lifted::Result o = lifted::Call(base, 0x8248F030, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82090888: lifted from generated/ by lift.py.
uint64_t f82090888(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82090888 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82090770
  { const lifted::Result o = lifted::Call(base, 0x82090770, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82090898: lifted from generated/ by lift.py.
lifted::Result f82090898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090898 (lifted)");
  // addi r3,r3,-28
  r3 = uint64_t(int64_t(int64_t(r3) + -28));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820908A0: lifted from generated/ by lift.py.
lifted::Result f820908A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820908A0 (lifted)");
  // addi r3,r3,-60
  r3 = uint64_t(int64_t(int64_t(r3) + -60));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820908A8: lifted from generated/ by lift.py.
uint64_t f820908A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820908A8 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82090780
  r3 = f82090780(base, r3);
  return r3;
}

// sub_820908B0: lifted from generated/ by lift.py.
uint64_t f820908B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820908B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-24
  r3 = uint64_t(int64_t(int64_t(r3) + -24));
  // b 0x8248f030
  { const lifted::Result o = lifted::Call(base, 0x8248F030, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820908B8: lifted from generated/ by lift.py.
uint64_t f820908B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820908B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82090770
  { const lifted::Result o = lifted::Call(base, 0x82090770, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820908C0: lifted from generated/ by lift.py.
uint64_t f820908C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820908C0 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-24
  r3 = uint64_t(int64_t(int64_t(r3) + -24));
  // b 0x824936e8
  r3 = f824936E8(base, r3);
  return r3;
}

// sub_820908C8: lifted from generated/ by lift.py.
uint64_t f820908C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_820908C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x8248f030
  { const lifted::Result o = lifted::Call(base, 0x8248F030, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820908D0: lifted from generated/ by lift.py.
lifted::Result f820908D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820908D0 (lifted)");
  // addi r3,r3,-32
  r3 = uint64_t(int64_t(int64_t(r3) + -32));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820908F0: lifted from generated/ by lift.py.
lifted::Result f820908F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820908F0 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820908F8: lifted from generated/ by lift.py.
lifted::Result f820908F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820908F8 (lifted)");
  // addi r3,r3,-36
  r3 = uint64_t(int64_t(int64_t(r3) + -36));
  // b 0x82090790
  { const lifted::Result o = f82090790(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090900: lifted from generated/ by lift.py.
uint64_t f82090900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82090900 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x82090780
  r3 = f82090780(base, r3);
  return r3;
}

// sub_82090910: lifted from generated/ by lift.py.
lifted::Result f82090910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090910 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // addi r3,r31,120
  r3 = uint64_t(int64_t(int64_t(r31) + 120));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,112
  r3 = uint64_t(int64_t(int64_t(r31) + 112));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,116
  r3 = uint64_t(int64_t(int64_t(r31) + 116));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,100
  r3 = uint64_t(int64_t(int64_t(r31) + 100));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,96
  r3 = uint64_t(int64_t(int64_t(r31) + 96));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,104
  r3 = uint64_t(int64_t(int64_t(r31) + 104));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,88
  r3 = uint64_t(int64_t(int64_t(r31) + 88));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,92
  r3 = uint64_t(int64_t(int64_t(r31) + 92));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,80
  r3 = uint64_t(int64_t(int64_t(r31) + 80));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,76
  r3 = uint64_t(int64_t(int64_t(r31) + 76));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,72
  r3 = uint64_t(int64_t(int64_t(r31) + 72));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,68
  r3 = uint64_t(int64_t(int64_t(r31) + 68));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,108
  r3 = uint64_t(int64_t(int64_t(r31) + 108));
  // bl 0x820a87a0
  // (left out: dead in C++)
  { const lifted::Result o = f820A87A0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,84
  r3 = uint64_t(int64_t(int64_t(r31) + 84));
  // bl 0x820903d0
  // (left out: dead in C++)
  { const lifted::Result o = f820903D0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_82090A08: lifted from generated/ by lift.py.
lifted::Result f82090A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82090A08 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-352(r1)
  ea = -352 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // mr r31,r3
  r31 = r3;
  // li r29,2
  r29 = uint64_t(int64_t(2));
  // mr r30,r4
  r30 = r4;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // addi r11,r31,60
  r11 = uint64_t(int64_t(int64_t(r31) + 60));
  // li r26,3
  r26 = uint64_t(int64_t(3));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // stw r29,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r29));
  // stw r29,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r29));
  // stw r30,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r30));
  // stw r29,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r29));
  // stw r26,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r26));
  // stw r29,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r29));
  // stw r30,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r30));
  // stw r11,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r11));
  // stw r25,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r25));
  // stw r25,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r25));
  // bl 0x82099a08
  // (left out: dead in C++)
  { const lifted::Result o = f82099A08(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r30,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r30));
  // addi r10,r31,56
  r10 = uint64_t(int64_t(int64_t(r31) + 56));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // stw r11,128(r31)
  REX_STORE_U32(uint32_t(r31) + 128, uint32_t(r11));
  // stw r10,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r10));
  // stw r27,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r27));
  // stw r27,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r27));
  // bl 0x820a63c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A63C8, LIFTED_ARGS(0x82090A90, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // stw r30,224(r1)
  REX_STORE_U32(uint32_t(r1) + 224, uint32_t(r30));
  // mr r11,r3
  r11 = r3;
  // stw r27,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r27));
  // addi r28,r31,12
  r28 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r3,r1,224
  r3 = uint64_t(int64_t(int64_t(r1) + 224));
  // stw r10,232(r1)
  REX_STORE_U32(uint32_t(r1) + 232, uint32_t(r10));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r11,124(r31)
  REX_STORE_U32(uint32_t(r31) + 124, uint32_t(r11));
  // stw r28,228(r1)
  REX_STORE_U32(uint32_t(r1) + 228, uint32_t(r28));
  // stw r10,236(r1)
  REX_STORE_U32(uint32_t(r1) + 236, uint32_t(r10));
  // addi r10,r1,136
  r10 = uint64_t(int64_t(int64_t(r1) + 136));
  // stw r10,240(r1)
  REX_STORE_U32(uint32_t(r1) + 240, uint32_t(r10));
  // bl 0x82095620
  // (left out: dead in C++)
  { const lifted::Result o = f82095620(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // stw r30,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r30));
  // mr r11,r3
  r11 = r3;
  // stw r28,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r28));
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // stw r26,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r26));
  // stw r29,212(r1)
  REX_STORE_U32(uint32_t(r1) + 212, uint32_t(r29));
  // stw r10,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r10));
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r10,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r10));
  // bl 0x82095620
  // (left out: dead in C++)
  { const lifted::Result o = f82095620(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r30,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r30));
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // bl 0x820925a0
  // (left out: dead in C++)
  { const lifted::Result o = f820925A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r30,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r30));
  // addi r10,r31,40
  r10 = uint64_t(int64_t(int64_t(r31) + 40));
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82090B30, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82090b48
  if (cr0.eq) goto loc_82090B48;
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82092710
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092710, LIFTED_ARGS(0x82090B40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x82090b4c
  goto loc_82090B4C;
loc_82090B48:
  // mr r11,r25
  r11 = r25;
loc_82090B4C:
  // addi r10,r31,8
  r10 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r11));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // bl 0x82098c70
  // (left out: dead in C++)
  { const lifted::Result o = f82098C70(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r30,272(r1)
  REX_STORE_U32(uint32_t(r1) + 272, uint32_t(r30));
  // addi r10,r31,16
  r10 = uint64_t(int64_t(int64_t(r31) + 16));
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
  // stw r10,276(r1)
  REX_STORE_U32(uint32_t(r1) + 276, uint32_t(r10));
  // bl 0x820993e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820993E8, LIFTED_ARGS(0x82090B80, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,68(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 68);
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,104
  r3 = uint64_t(int64_t(int64_t(r1) + 104));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // addi r10,r31,20
  r10 = uint64_t(int64_t(int64_t(r31) + 20));
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // bl 0x82091160
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091160, LIFTED_ARGS(0x82090BA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // stw r30,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r30));
  // addi r10,r31,52
  r10 = uint64_t(int64_t(int64_t(r31) + 52));
  // li r3,44
  r3 = uint64_t(int64_t(44));
  // stw r11,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r11));
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82090BBC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82090bd4
  if (cr0.eq) goto loc_82090BD4;
  // addi r4,r1,120
  r4 = uint64_t(int64_t(int64_t(r1) + 120));
  // bl 0x82098630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82098630, LIFTED_ARGS(0x82090BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x82090bd8
  goto loc_82090BD8;
loc_82090BD4:
  // mr r11,r25
  r11 = r25;
loc_82090BD8:
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82090BE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82090bfc
  if (cr0.eq) goto loc_82090BFC;
  // addi r4,r31,44
  r4 = uint64_t(int64_t(int64_t(r31) + 44));
  // bl 0x820ac8f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AC8F8, LIFTED_ARGS(0x82090BF4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x82090c00
  goto loc_82090C00;
loc_82090BFC:
  // mr r11,r25
  r11 = r25;
loc_82090C00:
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // stw r11,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r11));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x82090C0C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82090c24
  if (cr0.eq) goto loc_82090C24;
  // addi r4,r31,48
  r4 = uint64_t(int64_t(int64_t(r31) + 48));
  // bl 0x820ac848
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AC848, LIFTED_ARGS(0x82090C1C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x82090c28
  goto loc_82090C28;
loc_82090C24:
  // mr r11,r25
  r11 = r25;
loc_82090C28:
  // addi r10,r31,36
  r10 = uint64_t(int64_t(int64_t(r31) + 36));
  // stw r11,116(r31)
  REX_STORE_U32(uint32_t(r31) + 116, uint32_t(r11));
  // lwz r11,112(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 112);
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r30,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r30));
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // stw r11,-18160(r10)
  REX_STORE_U32(uint32_t(r10) + -18160, uint32_t(r11));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // lwz r11,116(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 116);
  // stw r11,-18156(r10)
  REX_STORE_U32(uint32_t(r10) + -18156, uint32_t(r11));
  // bl 0x82094ab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82094AB8, LIFTED_ARGS(0x82090C58, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32247
  r9 = uint64_t(int64_t(-2113339392));
  // stw r25,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r25));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stw r31,260(r1)
  REX_STORE_U32(uint32_t(r1) + 260, uint32_t(r31));
  // addi r9,r9,712
  r9 = uint64_t(int64_t(int64_t(r9) + 712));
  // addi r10,r10,-31940
  r10 = uint64_t(int64_t(int64_t(r10) + -31940));
  // mr r11,r3
  r11 = r3;
  // addi r5,r1,256
  r5 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // mr r3,r30
  r3 = r30;
  // stw r10,256(r1)
  REX_STORE_U32(uint32_t(r1) + 256, uint32_t(r10));
  // ld r10,80(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 80);
  // stw r11,120(r31)
  REX_STORE_U32(uint32_t(r31) + 120, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // std r10,264(r1)
  REX_STORE_U64(uint32_t(r1) + 264, r10);
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82090CAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,352
  r1 = uint64_t(int64_t(int64_t(r1) + 352));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82090D60: lifted from generated/ by lift.py.
lifted::Result f82090D60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82090D60 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82090d68
  { const lifted::Result o = f82090D68(base, r1, r13, r3, r4, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82090D68: lifted from generated/ by lift.py.
lifted::Result f82090D68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82090D68 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2984
  r11 = uint64_t(int64_t(int64_t(r11) + 2984));
  // addi r10,r10,2740
  r10 = uint64_t(int64_t(int64_t(r10) + 2740));
  // addi r9,r9,3768
  r9 = uint64_t(int64_t(int64_t(r9) + 3768));
  // clrlwi. r8,r4,31
  r8 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
  // beq 0x82090dac
  if (cr0.eq) goto loc_82090DAC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82090DAC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82090DAC:
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

// sub_82090F88: lifted from generated/ by lift.py.
lifted::Result f82090F88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82090F88 (lifted)");
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
  // std r5,32(r1)
  REX_STORE_U64(uint32_t(r1) + 32, r5);
  // std r6,40(r1)
  REX_STORE_U64(uint32_t(r1) + 40, r6);
  // std r7,48(r1)
  REX_STORE_U64(uint32_t(r1) + 48, r7);
  // std r8,56(r1)
  REX_STORE_U64(uint32_t(r1) + 56, r8);
  // std r9,64(r1)
  REX_STORE_U64(uint32_t(r1) + 64, r9);
  // std r10,72(r1)
  REX_STORE_U64(uint32_t(r1) + 72, r10);
  // stwu r1,-2176(r1)
  ea = -2176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82091048
  if (cr0.eq) goto loc_82091048;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82090FDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82090FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r31,r3
  r31 = r3;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091008, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091018, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r10,r1,2208
  r10 = uint64_t(int64_t(int64_t(r1) + 2208));
  // mr r30,r3
  r30 = r3;
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r5,80(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825a71b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71B0, LIFTED_ARGS(0x82091038, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8258c3a8
  // (left out: dead in C++)
  { const lifted::Result o = f8258C3A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82091048:
  // addi r1,r1,2176
  r1 = uint64_t(int64_t(int64_t(r1) + 2176));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82091070: lifted from generated/ by lift.py.
lifted::Result f82091070(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091070 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3016
  r11 = uint64_t(int64_t(int64_t(r11) + 3016));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8258c060
  // (left out: dead in C++)
  { const lifted::Result o = f8258C060(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32153
  r10 = uint64_t(int64_t(-2107179008));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-12160(r10)
  REX_STORE_U32(uint32_t(r10) + -12160, uint32_t(r11));
  // lwz r3,48(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820910b8
  if (cr0.eq) goto loc_820910B8;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x820910B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
loc_820910B8:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r11,r31,8
  r11 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r10,r10,2748
  r10 = uint64_t(int64_t(int64_t(r10) + 2748));
  // addi r9,r9,2984
  r9 = uint64_t(int64_t(int64_t(r9) + 2984));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // addi r8,r8,2740
  r8 = uint64_t(int64_t(int64_t(r8) + 2740));
  // addi r7,r7,3768
  r7 = uint64_t(int64_t(int64_t(r7) + 3768));
  // addi r6,r6,3400
  r6 = uint64_t(int64_t(int64_t(r6) + 3400));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
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

// sub_82091110: lifted from generated/ by lift.py.
lifted::Result f82091110(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091110 (lifted)");
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
  // bl 0x82091070
  // (left out: dead in C++)
  { const lifted::Result o = f82091070(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82091140
  if (cr0.eq) goto loc_82091140;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82091140, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091140:
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

// sub_820911F8: lifted from generated/ by lift.py.
uint64_t f820911F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820911F8 (lifted)");
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // blr
  return r3;
}

// sub_82091200: lifted from generated/ by lift.py.
lifted::Result f82091200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82091200 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // mr r31,r3
  r31 = r3;
  // lwz r10,4104(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4104);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82091230
  if (cr6.eq) goto loc_82091230;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091230, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091230:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091244, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a69d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A69D8, LIFTED_ARGS(0x8209124C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82091260: lifted from generated/ by lift.py.
lifted::Result f82091260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82091260 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825a69d8
  { const lifted::Result o = lifted::Call(base, 0x825A69D8, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82091268: lifted from generated/ by lift.py.
lifted::Result f82091268(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82091268 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3264
  r11 = uint64_t(int64_t(int64_t(r11) + 3264));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82091294
  if (cr0.eq) goto loc_82091294;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82091294, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091294:
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

// sub_820915F8: lifted from generated/ by lift.py.
lifted::Result f820915F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820915F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3288
  r11 = uint64_t(int64_t(int64_t(r11) + 3288));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS(0x82091628, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3264
  r11 = uint64_t(int64_t(int64_t(r11) + 3264));
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

// sub_82091648: lifted from generated/ by lift.py.
uint64_t f82091648(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82091648 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // addi r11,r3,8
  r11 = uint64_t(int64_t(int64_t(r3) + 8));
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // blt cr6,0x82091660
  if (cr6.lt) goto loc_82091660;
  // lwz r3,4(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 4);
  // blr
  return r3;
loc_82091660:
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // blr
  return r3;
}

// sub_82091668: lifted from generated/ by lift.py.
lifted::Result f82091668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091668 (lifted)");
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
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82091698
  if (cr0.eq) goto loc_82091698;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82091698, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091698:
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

// sub_820916B8: lifted from generated/ by lift.py.
lifted::Result f820916B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820916B8 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x820915f8
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820916C8: lifted from generated/ by lift.py.
lifted::Result f820916C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820916C8 (lifted)");
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
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82091704
  if (cr0.eq) goto loc_82091704;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82091704, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091704:
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

// sub_82091910: lifted from generated/ by lift.py.
lifted::Result f82091910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091910 (lifted)");
  uint64_t r5 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS(0x82091938, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82091720
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091720, LIFTED_ARGS(0x8209194C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82091968: lifted from generated/ by lift.py.
lifted::Result f82091968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82091968 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r11,r11,3316
  r11 = uint64_t(int64_t(int64_t(r11) + 3316));
  // addi r10,r10,3288
  r10 = uint64_t(int64_t(int64_t(r10) + 3288));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // bl 0x82091910
  // (left out: dead in C++)
  { const lifted::Result o = f82091910(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_820919B8: lifted from generated/ by lift.py.
lifted::Result f820919B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820919B8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // addi r11,r11,3316
  r11 = uint64_t(int64_t(int64_t(r11) + 3316));
  // addi r10,r10,3288
  r10 = uint64_t(int64_t(int64_t(r10) + 3288));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // bl 0x82091910
  // (left out: dead in C++)
  { const lifted::Result o = f82091910(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
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

// sub_82091A10: lifted from generated/ by lift.py.
lifted::Result f82091A10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82091A10 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3400
  r11 = uint64_t(int64_t(int64_t(r11) + 3400));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82091a3c
  if (cr0.eq) goto loc_82091A3C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82091A3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091A3C:
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

// sub_82091A58: lifted from generated/ by lift.py.
lifted::Result f82091A58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091A58 (lifted)");
  // addi r3,r3,388
  r3 = uint64_t(int64_t(int64_t(r3) + 388));
  // b 0x8258dd78
  { const lifted::Result o = f8258DD78(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82091B98: lifted from generated/ by lift.py.
lifted::Result f82091B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82091B98 (lifted)");
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
  // std r4,120(r1)
  REX_STORE_U64(uint32_t(r1) + 120, r4);
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82091BC0: lifted from generated/ by lift.py.
lifted::Result f82091BC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82091BC0 (lifted)");
  uint64_t r4 = 0, r6 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // addi r3,r3,64
  r3 = uint64_t(int64_t(int64_t(r3) + 64));
  // mr r31,r5
  r31 = r5;
  // bl 0x8258d998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D998, LIFTED_ARGS(0x82091BDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8229f1b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8229F1B0, LIFTED_ARGS(0x82091BE4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82091D28: lifted from generated/ by lift.py.
lifted::Result f82091D28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82091D28 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32247
  r10 = uint64_t(int64_t(-2113339392));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,7064
  r10 = uint64_t(int64_t(int64_t(r10) + 7064));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r31,r11,28
  r31 = uint64_t(int64_t(int64_t(r11) + 28));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r7,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r7));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r10,80(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // std r10,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r10);
  // mr r10,r11
  r10 = r11;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ld r6,0(r9)
  r6 = REX_LOAD_U64(uint32_t(r9) + 0);
  // ld r7,8(r9)
  r7 = REX_LOAD_U64(uint32_t(r9) + 8);
  // stw r31,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r31));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // ld r5,0(r8)
  r5 = REX_LOAD_U64(uint32_t(r8) + 0);
  // ld r4,0(r4)
  r4 = REX_LOAD_U64(uint32_t(r4) + 0);
  // bl 0x82092bf0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092BF0, LIFTED_ARGS(0x82091DA0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82099cf8
  // (left out: dead in C++)
  { const lifted::Result o = f82099CF8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82091DC0: lifted from generated/ by lift.py.
lifted::Result f82091DC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82091DC0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82091e44
  if (!cr6.eq) goto loc_82091E44;
  // bl 0x82091d28
  // (left out: dead in C++)
  { const lifted::Result o = f82091D28(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091E04, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091E24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82091E44, r31)); r3 = o.r3; f1 = o.f1; }
loc_82091E44:
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

// sub_820921A8: lifted from generated/ by lift.py.
lifted::Result f820921A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820921A8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3452
  r11 = uint64_t(int64_t(int64_t(r11) + 3452));
  // addi r10,r10,3436
  r10 = uint64_t(int64_t(int64_t(r10) + 3436));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r30,r31,388
  r30 = uint64_t(int64_t(int64_t(r31) + 388));
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // stb r28,396(r31)
  REX_STORE_U8(uint32_t(r31) + 396, uint8_t(r28));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD78(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dde0
  // (left out: dead in C++)
  { const lifted::Result o = f8258DDE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,64
  r29 = uint64_t(int64_t(int64_t(r31) + 64));
  // mr r3,r29
  r3 = r29;
  // bl 0x8258dba8
  // (left out: dead in C++)
  { const lifted::Result o = f8258DBA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,392
  r3 = uint64_t(int64_t(int64_t(r31) + 392));
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x8209220C); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82092214); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x8258d490
  // (left out: dead in C++)
  { const lifted::Result o = f8258D490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,40
  r30 = uint64_t(int64_t(int64_t(r31) + 40));
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82092230
  if (cr0.eq) goto loc_82092230;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82092230, r31)); r3 = o.r3; f1 = o.f1; }
loc_82092230:
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r28,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r28));
  // stw r28,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r28));
  // stw r28,12(r30)
  REX_STORE_U32(uint32_t(r30) + 12, uint32_t(r28));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,3400
  r11 = uint64_t(int64_t(int64_t(r11) + 3400));
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // bl 0x8258ddd0
  // (left out: dead in C++)
  f8258DDD0(base, r1, r13, r3);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82092260: lifted from generated/ by lift.py.
lifted::Result f82092260(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82092260 (lifted)");
  // addi r3,r3,-20
  r3 = uint64_t(int64_t(int64_t(r3) + -20));
  // b 0x82092268
  { const lifted::Result o = f82092268(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82092268: lifted from generated/ by lift.py.
lifted::Result f82092268(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82092268 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x820921a8
  // (left out: dead in C++)
  { const lifted::Result o = f820921A8(base, r1, r13, r3, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82092298
  if (cr0.eq) goto loc_82092298;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82092298, r31)); r3 = o.r3; f1 = o.f1; }
loc_82092298:
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

// sub_82092318: lifted from generated/ by lift.py.
lifted::Result f82092318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82092318 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,20
  r3 = uint64_t(int64_t(int64_t(r3) + 20));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209A930, LIFTED_ARGS(0x82092344, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82092358: lifted from generated/ by lift.py.
lifted::Result f82092358(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82092358 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // bl 0x8258de88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258DE88, LIFTED_ARGS(0x82092370, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r28,r31,20
  r28 = uint64_t(int64_t(int64_t(r31) + 20));
  // addi r11,r11,3400
  r11 = uint64_t(int64_t(int64_t(r11) + 3400));
  // addi r10,r10,3452
  r10 = uint64_t(int64_t(int64_t(r10) + 3452));
  // addi r9,r9,3436
  r9 = uint64_t(int64_t(int64_t(r9) + 3436));
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r9,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r9));
  // bl 0x82099fb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099FB0, LIFTED_ARGS(0x820923A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r27,r31,64
  r27 = uint64_t(int64_t(int64_t(r31) + 64));
  // mr r3,r27
  r3 = r27;
  // stw r30,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r30));
  // stw r30,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r30));
  // stw r30,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r30));
  // bl 0x8258db18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258DB18, LIFTED_ARGS(0x820923BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,388
  r3 = uint64_t(int64_t(int64_t(r31) + 388));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = f8258DCC8(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,392
  r3 = uint64_t(int64_t(int64_t(r31) + 392));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = f8258DCC8(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // stb r24,396(r31)
  REX_STORE_U8(uint32_t(r31) + 396, uint8_t(r24));
  // bl 0x8258d568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D568, LIFTED_ARGS(0x820923F0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8229da10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8229DA10, LIFTED_ARGS(0x820923F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // addi r11,r11,6744
  r11 = uint64_t(int64_t(int64_t(r11) + 6744));
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r27,r10,-31940
  r27 = uint64_t(int64_t(int64_t(r10) + -31940));
  // stw r31,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r31));
  // li r25,2
  r25 = uint64_t(int64_t(2));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // stw r27,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r27));
  // stw r25,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r25));
  // std r11,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r11);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092440, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,8408
  r11 = uint64_t(int64_t(int64_t(r11) + 8408));
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r26,r10,2748
  r26 = uint64_t(int64_t(int64_t(r10) + 2748));
  // stw r27,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r27));
  // stw r31,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r31));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // stw r26,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r26));
  // std r11,120(r1)
  REX_STORE_U64(uint32_t(r1) + 120, r11);
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092490, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32247
  r10 = uint64_t(int64_t(-2113339392));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r26,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r26));
  // addi r10,r10,7616
  r10 = uint64_t(int64_t(int64_t(r10) + 7616));
  // addi r11,r11,-31940
  r11 = uint64_t(int64_t(int64_t(r11) + -31940));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r31,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r31));
  // std r11,136(r1)
  REX_STORE_U64(uint32_t(r1) + 136, r11);
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820924E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32247
  r10 = uint64_t(int64_t(-2113339392));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // addi r10,r10,7104
  r10 = uint64_t(int64_t(int64_t(r10) + 7104));
  // stw r31,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r31));
  // addi r11,r11,2748
  r11 = uint64_t(int64_t(int64_t(r11) + 2748));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r29,r9,-31940
  r29 = uint64_t(int64_t(int64_t(r9) + -31940));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // mr r3,r28
  r3 = r28;
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // stw r29,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r29));
  // std r11,152(r1)
  REX_STORE_U64(uint32_t(r1) + 152, r11);
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820922b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820922B8, LIFTED_ARGS(0x8209252C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32247
  r11 = uint64_t(int64_t(-2113339392));
  // stw r30,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r30));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stw r29,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r29));
  // addi r11,r11,6752
  r11 = uint64_t(int64_t(int64_t(r11) + 6752));
  // stw r31,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r31));
  // addi r27,r10,2748
  r27 = uint64_t(int64_t(int64_t(r10) + 2748));
  // addi r5,r1,160
  r5 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // stw r27,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r27));
  // std r11,168(r1)
  REX_STORE_U64(uint32_t(r1) + 168, r11);
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820922b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820922B8, LIFTED_ARGS(0x82092570, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r27,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r27));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r25,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r25));
  // mr r4,r31
  r4 = r31;
  // stw r24,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258df18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258DF18, LIFTED_ARGS(0x82092590, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820925A0: lifted from generated/ by lift.py.
lifted::Result f820925A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820925A0 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS(0x820925BC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820925d0
  if (cr0.eq) goto loc_820925D0;
  // mr r4,r31
  r4 = r31;
  // bl 0x82092358
  // (left out: dead in C++)
  { const lifted::Result o = f82092358(base, r1, r13, r3, r4, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // b 0x820925d4
  goto loc_820925D4;
loc_820925D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820925D4:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r3,r3,20
  r3 = uint64_t(int64_t(int64_t(r3) + 20));
  // bne cr6,0x820925e4
  if (!cr6.eq) goto loc_820925E4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820925E4:
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

// sub_820925F8: lifted from generated/ by lift.py.
lifted::Result f820925F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820925F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092618, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,64
  r11 = uint64_t(int64_t(64));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209263C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8258e1d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E1D8, LIFTED_ARGS(0x82092640, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82092650: lifted from generated/ by lift.py.
lifted::Result f82092650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82092650 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x820926b0
  if (!cr6.eq) goto loc_820926B0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092690, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x820926B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_820926B0:
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

// sub_820926C8: lifted from generated/ by lift.py.
lifted::Result f820926C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820926C8 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3460
  r11 = uint64_t(int64_t(int64_t(r11) + 3460));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820926f4
  if (cr0.eq) goto loc_820926F4;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820926F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820926F4:
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

// sub_820927E8: lifted from generated/ by lift.py.
lifted::Result f820927E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820927E8 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3488
  r11 = uint64_t(int64_t(int64_t(r11) + 3488));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x82092814
  if (cr0.eq) goto loc_82092814;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82092814, r31)); r3 = o.r3; f1 = o.f1; }
loc_82092814:
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

// sub_82092830: lifted from generated/ by lift.py.
lifted::Result f82092830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82092830 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3536
  r11 = uint64_t(int64_t(int64_t(r11) + 3536));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x8209285c
  if (cr0.eq) goto loc_8209285C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8209285C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209285C:
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

// sub_82092A70: lifted from generated/ by lift.py.
lifted::Result f82092A70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82092A70 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // beq 0x82092aac
  if (cr0.eq) goto loc_82092AAC;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,192(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 192);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092AA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82092AAC:
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

// sub_82092AC0: lifted from generated/ by lift.py.
lifted::Result f82092AC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82092AC0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // beq 0x82092afc
  if (cr0.eq) goto loc_82092AFC;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092AF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_82092AFC:
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

// sub_82092DE0: lifted from generated/ by lift.py.
lifted::Result f82092DE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82092DE0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r5
  r30 = r5;
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82092e0c
  if (!cr6.eq) goto loc_82092E0C;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x82092d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092D50, LIFTED_ARGS(0x82092E0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82092E0C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82092e64
  if (!cr6.eq) goto loc_82092E64;
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r29
  r3 = r29;
  // bl 0x82092d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092D50, LIFTED_ARGS(0x82092E24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092E44, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82092E64, r31)); r3 = o.r3; f1 = o.f1; }
loc_82092E64:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82093000: lifted from generated/ by lift.py.
lifted::Result f82093000(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82093000 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // std r5,256(r1)
  REX_STORE_U64(uint32_t(r1) + 256, r5);
  // mr r25,r3
  r25 = r3;
  // lwz r31,260(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 260);
  // mr r27,r4
  r27 = r4;
  // lbz r11,21(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 21);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8209306c
  if (cr6.eq) goto loc_8209306C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,3324
  r4 = uint64_t(int64_t(int64_t(r11) + 3324));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825931D0, LIFTED_ARGS(0x82093038, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820919b8
  // (left out: dead in C++)
  { const lifted::Result o = f820919B8(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82091200
  // (left out: dead in C++)
  { const lifted::Result o = f82091200(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS(0x8209306C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8209306C:
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // mr r26,r31
  r26 = r31;
  // bl 0x82099b18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099B18, LIFTED_ARGS(0x82093078, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lbz r10,21(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 21);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82093090
  if (cr6.eq) goto loc_82093090;
  // lwz r28,8(r26)
  r28 = REX_LOAD_U32(uint32_t(r26) + 8);
  // b 0x820930b8
  goto loc_820930B8;
loc_82093090:
  // lwz r10,8(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lbz r10,21(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 21);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x820930a8
  if (cr6.eq) goto loc_820930A8;
  // mr r28,r11
  r28 = r11;
  // b 0x820930b8
  goto loc_820930B8;
loc_820930A8:
  // lwz r11,260(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 260);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // lwz r28,8(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 8);
  // bne cr6,0x82093194
  if (!cr6.eq) goto loc_82093194;
loc_820930B8:
  // lbz r11,21(r28)
  r11 = REX_LOAD_U8(uint32_t(r28) + 21);
  // lwz r31,4(r26)
  r31 = REX_LOAD_U32(uint32_t(r26) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x820930cc
  if (!cr6.eq) goto loc_820930CC;
  // stw r31,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r31));
loc_820930CC:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r10,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r26), so);
  // bne cr6,0x820930e4
  if (!cr6.eq) goto loc_820930E4;
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // b 0x820930fc
  goto loc_820930FC;
loc_820930E4:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x820930f8
  if (!cr6.eq) goto loc_820930F8;
  // stw r28,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r28));
  // b 0x820930fc
  goto loc_820930FC;
loc_820930F8:
  // stw r28,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r28));
loc_820930FC:
  // lwz r9,4(r27)
  r9 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x82093144
  if (!cr6.eq) goto loc_82093144;
  // lbz r11,21(r28)
  r11 = REX_LOAD_U8(uint32_t(r28) + 21);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82093120
  if (cr6.eq) goto loc_82093120;
  // mr r11,r31
  r11 = r31;
  // b 0x82093140
  goto loc_82093140;
loc_82093120:
  // lwz r10,0(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 0);
  // mr r11,r28
  r11 = r28;
  // b 0x82093134
  goto loc_82093134;
loc_8209312C:
  // mr r11,r10
  r11 = r10;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
loc_82093134:
  // lbz r8,21(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 21);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8209312c
  if (cr6.eq) goto loc_8209312C;
loc_82093140:
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
loc_82093144:
  // lwz r8,4(r27)
  r8 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r11,8(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + 8);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x8209322c
  if (!cr6.eq) goto loc_8209322C;
  // lbz r11,21(r28)
  r11 = REX_LOAD_U8(uint32_t(r28) + 21);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82093168
  if (cr6.eq) goto loc_82093168;
  // mr r11,r31
  r11 = r31;
  // b 0x8209318c
  goto loc_8209318C;
loc_82093168:
  // addi r10,r28,8
  r10 = uint64_t(int64_t(int64_t(r28) + 8));
  // mr r11,r28
  r11 = r28;
  // b 0x8209317c
  goto loc_8209317C;
loc_82093174:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r10,r11,8
  r10 = uint64_t(int64_t(int64_t(r11) + 8));
loc_8209317C:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lbz r9,21(r9)
  r9 = REX_LOAD_U8(uint32_t(r9) + 21);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x82093174
  if (cr6.eq) goto loc_82093174;
loc_8209318C:
  // stw r11,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r11));
  // b 0x8209322c
  goto loc_8209322C;
loc_82093194:
  // lwz r10,0(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 0);
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r10,0(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 0);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r10,8(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x820931b8
  if (!cr6.eq) goto loc_820931B8;
  // mr r31,r11
  r31 = r11;
  // b 0x820931e0
  goto loc_820931E0;
loc_820931B8:
  // lbz r10,21(r28)
  r10 = REX_LOAD_U8(uint32_t(r28) + 21);
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820931cc
  if (!cr6.eq) goto loc_820931CC;
  // stw r31,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r31));
loc_820931CC:
  // stw r28,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r28));
  // lwz r10,8(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r10,8(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 8);
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
loc_820931E0:
  // lwz r10,4(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 4);
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // cmplw cr6,r9,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r26), so);
  // bne cr6,0x820931f8
  if (!cr6.eq) goto loc_820931F8;
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // b 0x82093214
  goto loc_82093214;
loc_820931F8:
  // lwz r10,4(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 4);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplw cr6,r9,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r26), so);
  // bne cr6,0x82093210
  if (!cr6.eq) goto loc_82093210;
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // b 0x82093214
  goto loc_82093214;
loc_82093210:
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_82093214:
  // lwz r10,4(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lbz r9,20(r26)
  r9 = REX_LOAD_U8(uint32_t(r26) + 20);
  // lbz r10,20(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 20);
  // stb r9,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r9));
  // stb r10,20(r26)
  REX_STORE_U8(uint32_t(r26) + 20, uint8_t(r10));
loc_8209322C:
  // lbz r11,20(r26)
  r11 = REX_LOAD_U8(uint32_t(r26) + 20);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x820933cc
  if (!cr6.eq) goto loc_820933CC;
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // beq cr6,0x820933c8
  if (cr6.eq) goto loc_820933C8;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82093250:
  // lbz r11,20(r28)
  r11 = REX_LOAD_U8(uint32_t(r28) + 20);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x820933c8
  if (!cr6.eq) goto loc_820933C8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bne cr6,0x8209330c
  if (!cr6.eq) goto loc_8209330C;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lbz r10,20(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 20);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82093290
  if (!cr6.eq) goto loc_82093290;
  // mr r4,r31
  r4 = r31;
  // stb r30,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r30));
  // mr r3,r27
  r3 = r27;
  // stb r29,20(r31)
  REX_STORE_U8(uint32_t(r31) + 20, uint8_t(r29));
  // bl 0x82092b10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092B10, LIFTED_ARGS(0x8209328C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
loc_82093290:
  // lbz r10,21(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 21);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82093360
  if (!cr6.eq) goto loc_82093360;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x820932bc
  if (!cr6.eq) goto loc_820932BC;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x8209335c
  if (cr6.eq) goto loc_8209335C;
loc_820932BC:
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x820932e8
  if (!cr6.eq) goto loc_820932E8;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r4,r11
  r4 = r11;
  // mr r3,r27
  r3 = r27;
  // stb r30,20(r10)
  REX_STORE_U8(uint32_t(r10) + 20, uint8_t(r30));
  // stb r29,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r29));
  // bl 0x82092a00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092A00, LIFTED_ARGS(0x820932E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
loc_820932E8:
  // lbz r10,20(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 20);
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // stb r10,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r10));
  // stb r30,20(r31)
  REX_STORE_U8(uint32_t(r31) + 20, uint8_t(r30));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stb r30,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r30));
  // bl 0x82092b10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092B10, LIFTED_ARGS(0x82093308, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x820933c8
  goto loc_820933C8;
loc_8209330C:
  // lbz r10,20(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 20);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82093330
  if (!cr6.eq) goto loc_82093330;
  // mr r4,r31
  r4 = r31;
  // stb r30,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r30));
  // mr r3,r27
  r3 = r27;
  // stb r29,20(r31)
  REX_STORE_U8(uint32_t(r31) + 20, uint8_t(r29));
  // bl 0x82092a00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092A00, LIFTED_ARGS(0x8209332C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
loc_82093330:
  // lbz r10,21(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 21);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82093360
  if (!cr6.eq) goto loc_82093360;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8209337c
  if (!cr6.eq) goto loc_8209337C;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8209337c
  if (!cr6.eq) goto loc_8209337C;
loc_8209335C:
  // stb r29,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r29));
loc_82093360:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // mr r28,r31
  r28 = r31;
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bne cr6,0x82093250
  if (!cr6.eq) goto loc_82093250;
  // b 0x820933c8
  goto loc_820933C8;
loc_8209337C:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r10,20(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 20);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x820933a8
  if (!cr6.eq) goto loc_820933A8;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mr r4,r11
  r4 = r11;
  // mr r3,r27
  r3 = r27;
  // stb r30,20(r10)
  REX_STORE_U8(uint32_t(r10) + 20, uint8_t(r30));
  // stb r29,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r29));
  // bl 0x82092b10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092B10, LIFTED_ARGS(0x820933A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
loc_820933A8:
  // lbz r10,20(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 20);
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // stb r10,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r10));
  // stb r30,20(r31)
  REX_STORE_U8(uint32_t(r31) + 20, uint8_t(r30));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stb r30,20(r11)
  REX_STORE_U8(uint32_t(r11) + 20, uint8_t(r30));
  // bl 0x82092a00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092A00, LIFTED_ARGS(0x820933C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_820933C8:
  // stb r30,20(r28)
  REX_STORE_U8(uint32_t(r28) + 20, uint8_t(r30));
loc_820933CC:
  // mr r3,r26
  r3 = r26;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820933D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 8);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820933e8
  if (cr0.eq) goto loc_820933E8;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,8(r27)
  REX_STORE_U32(uint32_t(r27) + 8, uint32_t(r11));
loc_820933E8:
  // ld r11,256(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 256);
  // mr r3,r25
  r3 = r25;
  // std r11,0(r25)
  REX_STORE_U64(uint32_t(r25) + 0, r11);
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82093638: lifted from generated/ by lift.py.
lifted::Result f82093638(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82093638 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // std r5,176(r1)
  REX_STORE_U64(uint32_t(r1) + 176, r5);
  // lwz r9,176(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 176);
  // mr r30,r3
  r30 = r3;
  // std r6,184(r1)
  REX_STORE_U64(uint32_t(r1) + 184, r6);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // beq cr6,0x82093670
  if (cr6.eq) goto loc_82093670;
  // cmplw cr6,r9,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r31), so);
  // beq cr6,0x82093674
  if (cr6.eq) goto loc_82093674;
loc_82093670:
  // twi 31,r0,22
  lifted::Trap(22);
loc_82093674:
  // lwz r8,180(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 180);
  // lwz r28,188(r1)
  r28 = REX_LOAD_U32(uint32_t(r1) + 188);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // lwz r29,184(r1)
  r29 = REX_LOAD_U32(uint32_t(r1) + 184);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820936d0
  if (cr0.eq) goto loc_820936D0;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x820936a0
  if (cr6.eq) goto loc_820936A0;
  // cmplw cr6,r29,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r31), so);
  // beq cr6,0x820936a4
  if (cr6.eq) goto loc_820936A4;
loc_820936A0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820936A4:
  // subf r11,r28,r10
  r11 = r10 - r28;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820936d0
  if (cr0.eq) goto loc_820936D0;
  // mr r3,r31
  r3 = r31;
  // bl 0x82099cf8
  // (left out: dead in C++)
  { const lifted::Result o = f82099CF8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r31,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r31));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // b 0x82093730
  goto loc_82093730;
loc_820936D0:
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x820936e0
  if (cr6.eq) goto loc_820936E0;
  // cmplw cr6,r9,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r29), so);
  // beq cr6,0x820936e4
  if (cr6.eq) goto loc_820936E4;
loc_820936E0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820936E4:
  // lwz r11,180(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 180);
  // subf r11,r11,r28
  r11 = r28 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8209372c
  if (cr0.eq) goto loc_8209372C;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // std r5,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r5);
  // bl 0x82099b18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099B18, LIFTED_ARGS(0x8209370C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // bl 0x82093000
  // (left out: dead in C++)
  { const lifted::Result o = f82093000(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // ld r5,176(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 176);
  // lwz r9,176(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 176);
  // b 0x820936d0
  goto loc_820936D0;
loc_8209372C:
  // std r5,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r5);
loc_82093730:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82093850: lifted from generated/ by lift.py.
lifted::Result f82093850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82093850 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r1,88
  r9 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r31,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r31));
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ld r6,0(r10)
  r6 = REX_LOAD_U64(uint32_t(r10) + 0);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // ld r5,0(r9)
  r5 = REX_LOAD_U64(uint32_t(r9) + 0);
  // bl 0x82093638
  // (left out: dead in C++)
  { const lifted::Result o = f82093638(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820938A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
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

// sub_820941F0: lifted from generated/ by lift.py.
lifted::Result f820941F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820941F0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,3744
  r11 = uint64_t(int64_t(int64_t(r11) + 3744));
  // addi r10,r10,3732
  r10 = uint64_t(int64_t(int64_t(r10) + 3732));
  // addi r9,r9,3712
  r9 = uint64_t(int64_t(int64_t(r9) + 3712));
  // addi r8,r8,3696
  r8 = uint64_t(int64_t(int64_t(r8) + 3696));
  // addi r7,r7,3640
  r7 = uint64_t(int64_t(int64_t(r7) + 3640));
  // addi r6,r6,3592
  r6 = uint64_t(int64_t(int64_t(r6) + 3592));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // stw r9,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r8,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r8));
  // stw r7,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r7));
  // stw r6,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r6));
  // bl 0x82106718
  // (left out: dead in C++)
  { const lifted::Result o = f82106718(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,124
  r3 = uint64_t(int64_t(int64_t(r31) + 124));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,120
  r3 = uint64_t(int64_t(int64_t(r31) + 120));
  // bl 0x82092a70
  // (left out: dead in C++)
  { const lifted::Result o = f82092A70(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x82092ac0
  // (left out: dead in C++)
  { const lifted::Result o = f82092AC0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,116
  r3 = uint64_t(int64_t(int64_t(r31) + 116));
  // bl 0x82090380
  // (left out: dead in C++)
  { const lifted::Result o = f82090380(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,96
  r28 = uint64_t(int64_t(int64_t(r31) + 96));
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82094280
  if (cr0.eq) goto loc_82094280;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82094280, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094280:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r31,76
  r29 = uint64_t(int64_t(int64_t(r31) + 76));
  // stw r30,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r30));
  // stw r30,8(r28)
  REX_STORE_U32(uint32_t(r28) + 8, uint32_t(r30));
  // stw r30,12(r28)
  REX_STORE_U32(uint32_t(r28) + 12, uint32_t(r30));
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820942a4
  if (cr0.eq) goto loc_820942A4;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x820942A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_820942A4:
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // stw r30,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r30));
  // stw r30,8(r29)
  REX_STORE_U32(uint32_t(r29) + 8, uint32_t(r30));
  // stw r30,12(r29)
  REX_STORE_U32(uint32_t(r29) + 12, uint32_t(r30));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,40
  r3 = uint64_t(int64_t(int64_t(r31) + 40));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x82093850
  // (left out: dead in C++)
  { const lifted::Result o = f82093850(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3488
  r11 = uint64_t(int64_t(int64_t(r11) + 3488));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // addi r10,r10,3536
  r10 = uint64_t(int64_t(int64_t(r10) + 3536));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r9,r9,3400
  r9 = uint64_t(int64_t(int64_t(r9) + 3400));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // addi r8,r8,2676
  r8 = uint64_t(int64_t(int64_t(r8) + 2676));
  // addi r7,r7,2748
  r7 = uint64_t(int64_t(int64_t(r7) + 2748));
  // stw r10,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r10));
  // addi r6,r6,2748
  r6 = uint64_t(int64_t(int64_t(r6) + 2748));
  // stw r9,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r9));
  // stw r8,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r8));
  // stw r7,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r7));
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82094380: lifted from generated/ by lift.py.
lifted::Result f82094380(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82094380 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,16(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82094398: lifted from generated/ by lift.py.
lifted::Result f82094398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82094398 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820943E0: lifted from generated/ by lift.py.
uint64_t f820943E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820943E0 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_820943F8: lifted from generated/ by lift.py.
uint64_t f820943F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820943F8 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // addi r3,r3,20
  r3 = uint64_t(int64_t(int64_t(r3) + 20));
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82094410: lifted from generated/ by lift.py.
uint64_t f82094410(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82094410 (lifted)");
  // lwz r3,104(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 104);
  // blr
  return r3;
}

// sub_82094418: lifted from generated/ by lift.py.
uint64_t f82094418(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82094418 (lifted)");
  uint64_t r3 = 0;
  // li r3,9
  r3 = uint64_t(int64_t(9));
  // blr
  return r3;
}

// sub_82094420: lifted from generated/ by lift.py.
lifted::Result f82094420(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82094420 (lifted)");
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
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // mr r31,r4
  r31 = r4;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82094448, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r4,r31
  r4 = r31;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8209445C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82094470: lifted from generated/ by lift.py.
uint64_t f82094470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82094470 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x820943f8
  r3 = f820943F8(base, r3);
  return r3;
}

// sub_82094478: lifted from generated/ by lift.py.
lifted::Result f82094478(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82094478 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82094380
  { const lifted::Result o = f82094380(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82094488: lifted from generated/ by lift.py.
lifted::Result f82094488(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82094488 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x82094380
  { const lifted::Result o = f82094380(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82094490: lifted from generated/ by lift.py.
lifted::Result f82094490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82094490 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x82094368
  { const lifted::Result o = lifted::Call(base, 0x82094368, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820944A0: lifted from generated/ by lift.py.
lifted::Result f820944A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820944A0 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x820947e0
  { const lifted::Result o = f820947E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820944B0: lifted from generated/ by lift.py.
lifted::Result f820944B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820944B0 (lifted)");
  // addi r3,r3,-8
  r3 = uint64_t(int64_t(int64_t(r3) + -8));
  // b 0x820947e0
  { const lifted::Result o = f820947E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820944B8: lifted from generated/ by lift.py.
lifted::Result f820944B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820944B8 (lifted)");
  // addi r3,r3,-12
  r3 = uint64_t(int64_t(int64_t(r3) + -12));
  // b 0x820947e0
  { const lifted::Result o = f820947E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820944C0: lifted from generated/ by lift.py.
lifted::Result f820944C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820944C0 (lifted)");
  // addi r3,r3,-16
  r3 = uint64_t(int64_t(int64_t(r3) + -16));
  // b 0x820947e0
  { const lifted::Result o = f820947E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820944D0: lifted from generated/ by lift.py.
lifted::Result f820944D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820944D0 (lifted)");
  // addi r3,r3,-20
  r3 = uint64_t(int64_t(int64_t(r3) + -20));
  // b 0x820947e0
  { const lifted::Result o = f820947E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820947E0: lifted from generated/ by lift.py.
lifted::Result f820947E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820947E0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x820941f0
  // (left out: dead in C++)
  { const lifted::Result o = f820941F0(base, r1, r13, r3, r4, r5, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82094810
  if (cr0.eq) goto loc_82094810;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82094810, r31)); r3 = o.r3; f1 = o.f1; }
loc_82094810:
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

// sub_82094890: lifted from generated/ by lift.py.
lifted::Result f82094890(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82094890 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,76
  r3 = uint64_t(int64_t(int64_t(r3) + 76));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209A930, LIFTED_ARGS(0x820948BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82094930: lifted from generated/ by lift.py.
lifted::Result f82094930(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82094930 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r3,76
  r3 = uint64_t(int64_t(int64_t(r3) + 76));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8209a930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209A930, LIFTED_ARGS(0x8209495C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

namespace {

lifted::Result e82090148(uint8_t* base, const lifted::Args& a) {
  return f82090148(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82090190(uint8_t* base, const lifted::Args& a) {
  return f82090190(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820901D8(uint8_t* base, const lifted::Args& a) {
  return f820901D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82090220(uint8_t* base, const lifted::Args& a) {
  return f82090220(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82090268(uint8_t* base, const lifted::Args& a) {
  return f82090268(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82090350(uint8_t* base, const lifted::Args& a) {
  return f82090350(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82090380(uint8_t* base, const lifted::Args& a) {
  return f82090380(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820903D0(uint8_t* base, const lifted::Args& a) {
  return f820903D0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820905E0(uint8_t* base, const lifted::Args& a) {
  f820905E0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82090780(uint8_t* base, const lifted::Args& a) {
  return {f82090780(base, a.r[0]), a.f[0]};
}

lifted::Result e82090790(uint8_t* base, const lifted::Args& a) {
  return f82090790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820907E0(uint8_t* base, const lifted::Args& a) {
  return f820907E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820907E8(uint8_t* base, const lifted::Args& a) {
  return f820907E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820907F0(uint8_t* base, const lifted::Args& a) {
  return {f820907F0(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e820907F8(uint8_t* base, const lifted::Args& a) {
  return f820907F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090800(uint8_t* base, const lifted::Args& a) {
  return f82090800(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090808(uint8_t* base, const lifted::Args& a) {
  return {f82090808(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82090810(uint8_t* base, const lifted::Args& a) {
  return f82090810(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090818(uint8_t* base, const lifted::Args& a) {
  return f82090818(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090820(uint8_t* base, const lifted::Args& a) {
  return {f82090820(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82090848(uint8_t* base, const lifted::Args& a) {
  return f82090848(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090850(uint8_t* base, const lifted::Args& a) {
  return f82090850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090858(uint8_t* base, const lifted::Args& a) {
  return {f82090858(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82090870(uint8_t* base, const lifted::Args& a) {
  return f82090870(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090878(uint8_t* base, const lifted::Args& a) {
  return f82090878(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090880(uint8_t* base, const lifted::Args& a) {
  return {f82090880(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82090888(uint8_t* base, const lifted::Args& a) {
  return {f82090888(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82090898(uint8_t* base, const lifted::Args& a) {
  return f82090898(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820908A0(uint8_t* base, const lifted::Args& a) {
  return f820908A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820908A8(uint8_t* base, const lifted::Args& a) {
  return {f820908A8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820908B0(uint8_t* base, const lifted::Args& a) {
  return {f820908B0(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e820908B8(uint8_t* base, const lifted::Args& a) {
  return {f820908B8(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e820908C0(uint8_t* base, const lifted::Args& a) {
  return {f820908C0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e820908C8(uint8_t* base, const lifted::Args& a) {
  return {f820908C8(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e820908D0(uint8_t* base, const lifted::Args& a) {
  return f820908D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820908F0(uint8_t* base, const lifted::Args& a) {
  return f820908F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820908F8(uint8_t* base, const lifted::Args& a) {
  return f820908F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090900(uint8_t* base, const lifted::Args& a) {
  return {f82090900(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82090910(uint8_t* base, const lifted::Args& a) {
  return f82090910(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82090A08(uint8_t* base, const lifted::Args& a) {
  return f82090A08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82090D60(uint8_t* base, const lifted::Args& a) {
  return f82090D60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82090D68(uint8_t* base, const lifted::Args& a) {
  return f82090D68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82090F88(uint8_t* base, const lifted::Args& a) {
  return f82090F88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091070(uint8_t* base, const lifted::Args& a) {
  return f82091070(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091110(uint8_t* base, const lifted::Args& a) {
  return f82091110(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820911F8(uint8_t* base, const lifted::Args& a) {
  return {f820911F8(base, a.r[0]), a.f[0]};
}

lifted::Result e82091200(uint8_t* base, const lifted::Args& a) {
  return f82091200(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82091260(uint8_t* base, const lifted::Args& a) {
  return f82091260(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82091268(uint8_t* base, const lifted::Args& a) {
  return f82091268(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820915F8(uint8_t* base, const lifted::Args& a) {
  return f820915F8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091648(uint8_t* base, const lifted::Args& a) {
  return {f82091648(base, a.r[0]), a.f[0]};
}

lifted::Result e82091668(uint8_t* base, const lifted::Args& a) {
  return f82091668(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820916B8(uint8_t* base, const lifted::Args& a) {
  return f820916B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820916C8(uint8_t* base, const lifted::Args& a) {
  return f820916C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091910(uint8_t* base, const lifted::Args& a) {
  return f82091910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091968(uint8_t* base, const lifted::Args& a) {
  return f82091968(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820919B8(uint8_t* base, const lifted::Args& a) {
  return f820919B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82091A10(uint8_t* base, const lifted::Args& a) {
  return f82091A10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82091A58(uint8_t* base, const lifted::Args& a) {
  return f82091A58(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091B98(uint8_t* base, const lifted::Args& a) {
  return f82091B98(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82091BC0(uint8_t* base, const lifted::Args& a) {
  return f82091BC0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[4], a.r[5], a.r[6], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82091D28(uint8_t* base, const lifted::Args& a) {
  return f82091D28(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82091DC0(uint8_t* base, const lifted::Args& a) {
  return f82091DC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820921A8(uint8_t* base, const lifted::Args& a) {
  return f820921A8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82092260(uint8_t* base, const lifted::Args& a) {
  return f82092260(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82092268(uint8_t* base, const lifted::Args& a) {
  return f82092268(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82092318(uint8_t* base, const lifted::Args& a) {
  return f82092318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82092358(uint8_t* base, const lifted::Args& a) {
  return f82092358(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820925A0(uint8_t* base, const lifted::Args& a) {
  return f820925A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820925F8(uint8_t* base, const lifted::Args& a) {
  return f820925F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82092650(uint8_t* base, const lifted::Args& a) {
  return f82092650(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820926C8(uint8_t* base, const lifted::Args& a) {
  return f820926C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820927E8(uint8_t* base, const lifted::Args& a) {
  return f820927E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82092830(uint8_t* base, const lifted::Args& a) {
  return f82092830(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82092A70(uint8_t* base, const lifted::Args& a) {
  return f82092A70(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82092AC0(uint8_t* base, const lifted::Args& a) {
  return f82092AC0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82092DE0(uint8_t* base, const lifted::Args& a) {
  return f82092DE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82093000(uint8_t* base, const lifted::Args& a) {
  return f82093000(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82093638(uint8_t* base, const lifted::Args& a) {
  return f82093638(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82093850(uint8_t* base, const lifted::Args& a) {
  return f82093850(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820941F0(uint8_t* base, const lifted::Args& a) {
  return f820941F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82094380(uint8_t* base, const lifted::Args& a) {
  return f82094380(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82094398(uint8_t* base, const lifted::Args& a) {
  return f82094398(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820943E0(uint8_t* base, const lifted::Args& a) {
  return {f820943E0(base, a.r[0]), a.f[0]};
}

lifted::Result e820943F8(uint8_t* base, const lifted::Args& a) {
  return {f820943F8(base, a.r[0]), a.f[0]};
}

lifted::Result e82094410(uint8_t* base, const lifted::Args& a) {
  return {f82094410(base, a.r[0]), a.f[0]};
}

lifted::Result e82094418(uint8_t* base, const lifted::Args& a) {
  return {f82094418(base), a.f[0]};
}

lifted::Result e82094420(uint8_t* base, const lifted::Args& a) {
  return f82094420(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82094470(uint8_t* base, const lifted::Args& a) {
  return {f82094470(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82094478(uint8_t* base, const lifted::Args& a) {
  return f82094478(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82094488(uint8_t* base, const lifted::Args& a) {
  return f82094488(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82094490(uint8_t* base, const lifted::Args& a) {
  return f82094490(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e820944A0(uint8_t* base, const lifted::Args& a) {
  return f820944A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820944B0(uint8_t* base, const lifted::Args& a) {
  return f820944B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820944B8(uint8_t* base, const lifted::Args& a) {
  return f820944B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820944C0(uint8_t* base, const lifted::Args& a) {
  return f820944C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820944D0(uint8_t* base, const lifted::Args& a) {
  return f820944D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820947E0(uint8_t* base, const lifted::Args& a) {
  return f820947E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82094890(uint8_t* base, const lifted::Args& a) {
  return f82094890(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82094930(uint8_t* base, const lifted::Args& a) {
  return f82094930(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

const Registration kEntries[] = {
    {0x82090148, e82090148},
    {0x82090190, e82090190},
    {0x820901D8, e820901D8},
    {0x82090220, e82090220},
    {0x82090268, e82090268},
    {0x82090350, e82090350},
    {0x82090380, e82090380},
    {0x820903D0, e820903D0},
    {0x820905E0, e820905E0},
    {0x82090780, e82090780},
    {0x82090790, e82090790},
    {0x820907E0, e820907E0},
    {0x820907E8, e820907E8},
    {0x820907F0, e820907F0},
    {0x820907F8, e820907F8},
    {0x82090800, e82090800},
    {0x82090808, e82090808},
    {0x82090810, e82090810},
    {0x82090818, e82090818},
    {0x82090820, e82090820},
    {0x82090848, e82090848},
    {0x82090850, e82090850},
    {0x82090858, e82090858},
    {0x82090870, e82090870},
    {0x82090878, e82090878},
    {0x82090880, e82090880},
    {0x82090888, e82090888},
    {0x82090898, e82090898},
    {0x820908A0, e820908A0},
    {0x820908A8, e820908A8},
    {0x820908B0, e820908B0},
    {0x820908B8, e820908B8},
    {0x820908C0, e820908C0},
    {0x820908C8, e820908C8},
    {0x820908D0, e820908D0},
    {0x820908F0, e820908F0},
    {0x820908F8, e820908F8},
    {0x82090900, e82090900},
    {0x82090910, e82090910},
    {0x82090A08, e82090A08},
    {0x82090D60, e82090D60},
    {0x82090D68, e82090D68},
    {0x82090F88, e82090F88},
    {0x82091070, e82091070},
    {0x82091110, e82091110},
    {0x820911F8, e820911F8},
    {0x82091200, e82091200},
    {0x82091260, e82091260},
    {0x82091268, e82091268},
    {0x820915F8, e820915F8},
    {0x82091648, e82091648},
    {0x82091668, e82091668},
    {0x820916B8, e820916B8},
    {0x820916C8, e820916C8},
    {0x82091910, e82091910},
    {0x82091968, e82091968},
    {0x820919B8, e820919B8},
    {0x82091A10, e82091A10},
    {0x82091A58, e82091A58},
    {0x82091B98, e82091B98},
    {0x82091BC0, e82091BC0},
    {0x82091D28, e82091D28},
    {0x82091DC0, e82091DC0},
    {0x820921A8, e820921A8},
    {0x82092260, e82092260},
    {0x82092268, e82092268},
    {0x82092318, e82092318},
    {0x82092358, e82092358},
    {0x820925A0, e820925A0},
    {0x820925F8, e820925F8},
    {0x82092650, e82092650},
    {0x820926C8, e820926C8},
    {0x820927E8, e820927E8},
    {0x82092830, e82092830},
    {0x82092A70, e82092A70},
    {0x82092AC0, e82092AC0},
    {0x82092DE0, e82092DE0},
    {0x82093000, e82093000},
    {0x82093638, e82093638},
    {0x82093850, e82093850},
    {0x820941F0, e820941F0},
    {0x82094380, e82094380},
    {0x82094398, e82094398},
    {0x820943E0, e820943E0},
    {0x820943F8, e820943F8},
    {0x82094410, e82094410},
    {0x82094418, e82094418},
    {0x82094420, e82094420},
    {0x82094470, e82094470},
    {0x82094478, e82094478},
    {0x82094488, e82094488},
    {0x82094490, e82094490},
    {0x820944A0, e820944A0},
    {0x820944B0, e820944B0},
    {0x820944B8, e820944B8},
    {0x820944C0, e820944C0},
    {0x820944D0, e820944D0},
    {0x820947E0, e820947E0},
    {0x82094890, e82094890},
    {0x82094930, e82094930},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_001)), true);

}  // namespace

}  // namespace lifted
