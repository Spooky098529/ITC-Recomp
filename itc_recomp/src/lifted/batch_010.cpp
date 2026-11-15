// Lift sweep batch 010 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_010, true, "ITC",
              "Run lift sweep batch 010's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_821AE4C0: lifted from generated/ by lift.py.
uint64_t f821AE4C0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE4C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,76(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 76);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE508:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae508
  if (cr6.lt) goto loc_821AE508;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE540: lifted from generated/ by lift.py.
lifted::Result f821AE540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE540 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,84(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 84);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE588:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae588
  if (cr6.lt) goto loc_821AE588;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE5C0: lifted from generated/ by lift.py.
uint64_t f821AE5C0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE5C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,84(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 84);
  // lwz r11,88(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 88);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE608:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae608
  if (cr6.lt) goto loc_821AE608;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE640: lifted from generated/ by lift.py.
uint64_t f821AE640(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE640 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,596(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ae69c
  if (!cr6.eq) goto loc_821AE69C;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A5B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AE69C:
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821ae6bc
  if (cr6.eq) goto loc_821AE6BC;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A5B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821AE6BC:
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

// sub_821AE6D8: lifted from generated/ by lift.py.
lifted::Result f821AE6D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE6D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,198
  r31 = uint64_t(int64_t(198));
  // li r30,2
  r30 = uint64_t(int64_t(2));
loc_821AE708:
  // addi r5,r31,-191
  r5 = uint64_t(int64_t(int64_t(r31) + -191));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x821ae708
  if (!cr0.eq) goto loc_821AE708;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE748: lifted from generated/ by lift.py.
lifted::Result f821AE748(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE748 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,92(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 92);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821AE7A8: lifted from generated/ by lift.py.
uint64_t f821AE7A8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE7A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,92(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 92);
  // bl 0x821ad148
  // (left out: dead in C++)
  r3 = f821AD148(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821AE808: lifted from generated/ by lift.py.
lifted::Result f821AE808(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE808 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,112(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 112);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821AE868: lifted from generated/ by lift.py.
uint64_t f821AE868(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE868 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,112(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 112);
  // bl 0x821ad148
  // (left out: dead in C++)
  r3 = f821AD148(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821AE8C8: lifted from generated/ by lift.py.
lifted::Result f821AE8C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE8C8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,108(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 108);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821AE928: lifted from generated/ by lift.py.
uint64_t f821AE928(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE928 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,108(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 108);
  // bl 0x821ad148
  // (left out: dead in C++)
  r3 = f821AD148(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821AE988: lifted from generated/ by lift.py.
lifted::Result f821AE988(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE988 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,100(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 100);
  // lwz r11,104(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 104);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE9D0:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,-10
  r5 = uint64_t(int64_t(-10));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae9d0
  if (cr6.lt) goto loc_821AE9D0;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AEA08: lifted from generated/ by lift.py.
uint64_t f821AEA08(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AEA08 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,100(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 100);
  // lwz r11,104(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 104);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AEA50:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,-10
  r5 = uint64_t(int64_t(-10));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821aea50
  if (cr6.lt) goto loc_821AEA50;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AEA88: lifted from generated/ by lift.py.
lifted::Result f821AEA88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AEA88 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,100
  r6 = uint64_t(int64_t(100));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,96(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 96);
  // bl 0x821ad200
  // (left out: dead in C++)
  { const lifted::Result o = f821AD200(base, r1, r13, r3, r4, r5, r6, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821AEAE8: lifted from generated/ by lift.py.
uint64_t f821AEAE8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AEAE8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,25
  r4 = uint64_t(int64_t(25));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r6,100
  r6 = uint64_t(int64_t(100));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r4,96(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 96);
  // bl 0x821ad148
  // (left out: dead in C++)
  r3 = f821AD148(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821AEB48: lifted from generated/ by lift.py.
lifted::Result f821AEB48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AEB48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,24
  r31 = uint64_t(int64_t(24));
  // li r28,5
  r28 = uint64_t(int64_t(5));
loc_821AEB7C:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // addi r5,r31,-5
  r5 = uint64_t(int64_t(int64_t(r31) + -5));
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r3,r29
  r3 = r29;
  // beq cr6,0x821aebd0
  if (cr6.eq) goto loc_821AEBD0;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r6,528
  r6 = uint64_t(int64_t(528));
  // clrlwi r9,r11,22
  r9 = uint32_t(r11) & 0x3FF;
  // rlwinm r8,r11,22,26,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x3F;
  // rlwinm r7,r11,16,17,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0x7FFF;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r5,r31
  r5 = r31;
  // addi r6,r11,1646
  r6 = uint64_t(int64_t(int64_t(r11) + 1646));
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // addi r6,r11,1718
  r6 = uint64_t(int64_t(int64_t(r11) + 1718));
  // b 0x821aebf0
  goto loc_821AEBF0;
loc_821AEBD0:
  // li r6,531
  r6 = uint64_t(int64_t(531));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,533
  r6 = uint64_t(int64_t(533));
  // mr r5,r31
  r5 = r31;
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,533
  r6 = uint64_t(int64_t(533));
loc_821AEBF0:
  // addi r5,r31,5
  r5 = uint64_t(int64_t(int64_t(r31) + 5));
  // li r4,19
  r4 = uint64_t(int64_t(19));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x821aeb7c
  if (!cr0.eq) goto loc_821AEB7C;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AEC18: lifted from generated/ by lift.py.
uint64_t f821AEC18(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AEC18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,18
  r4 = uint64_t(int64_t(18));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // mr r24,r3
  r24 = r3;
  // mr r30,r22
  r30 = r22;
  // li r21,1
  r21 = uint64_t(int64_t(1));
loc_821AEC9C:
  // addi r31,r30,6
  r31 = uint64_t(int64_t(int64_t(r30) + 6));
  // mr r3,r27
  r3 = r27;
  // mr r4,r31
  r4 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r4,r31
  r4 = r31;
  // mr r3,r26
  r3 = r26;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r4,r31
  r4 = r31;
  // mr r3,r25
  r3 = r25;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
  // mr r4,r31
  r4 = r31;
  // mr r3,r24
  r3 = r24;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // bne cr6,0x821aecf0
  if (!cr6.eq) goto loc_821AECF0;
  // stw r21,80(r29)
  REX_STORE_U32(uint32_t(r29) + 80, uint32_t(r21));
  // stw r21,80(r28)
  REX_STORE_U32(uint32_t(r28) + 80, uint32_t(r21));
  // b 0x821aecf8
  goto loc_821AECF8;
loc_821AECF0:
  // stw r22,80(r29)
  REX_STORE_U32(uint32_t(r29) + 80, uint32_t(r22));
  // stw r22,80(r28)
  REX_STORE_U32(uint32_t(r28) + 80, uint32_t(r22));
loc_821AECF8:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r22,80(r23)
  REX_STORE_U32(uint32_t(r23) + 80, uint32_t(r22));
  // stw r22,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r22));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // blt cr6,0x821aec9c
  if (cr6.lt) goto loc_821AEC9C;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return r3;
}

// sub_821AED18: lifted from generated/ by lift.py.
lifted::Result f821AED18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821AED18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-4064(r1)
  ea = -4064 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r26,8
  r26 = uint64_t(int64_t(8));
  // li r24,4
  r24 = uint64_t(int64_t(4));
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // blt cr6,0x821aed60
  if (cr6.lt) goto loc_821AED60;
  // beq cr6,0x821aed5c
  if (cr6.eq) goto loc_821AED5C;
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // blt cr6,0x821aed50
  if (cr6.lt) goto loc_821AED50;
  // bne cr6,0x821aed64
  if (!cr6.eq) goto loc_821AED64;
  // li r24,3
  r24 = uint64_t(int64_t(3));
  // li r26,10
  r26 = uint64_t(int64_t(10));
  // b 0x821aed64
  goto loc_821AED64;
loc_821AED50:
  // li r24,3
  r24 = uint64_t(int64_t(3));
  // li r26,9
  r26 = uint64_t(int64_t(9));
  // b 0x821aed64
  goto loc_821AED64;
loc_821AED5C:
  // li r24,3
  r24 = uint64_t(int64_t(3));
loc_821AED60:
  // li r26,8
  r26 = uint64_t(int64_t(8));
loc_821AED64:
  // bl 0x821021a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821021A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // ori r28,r11,38432
  r28 = r11 | 38432;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101900, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // ori r27,r11,15092
  r27 = r11 | 15092;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r26
  r3 = r26;
  // stwx r25,r11,r10
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), uint32_t(r25));
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // stwx r9,r11,r27
  REX_STORE_U32(uint32_t(r11) + uint32_t(r27), uint32_t(r9));
  // bl 0x82101d38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101D38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x820ff918
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF918, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r29,6120
  r31 = uint64_t(int64_t(int64_t(r29) + 6120));
  // addis r29,r29,2
  r29 = uint64_t(int64_t(int64_t(r29) + 131072));
  // addi r29,r29,-26336
  r29 = uint64_t(int64_t(int64_t(r29) + -26336));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lha r10,204(r31)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 204))));
  // b 0x821aedf4
  goto loc_821AEDF4;
loc_821AEDEC:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_821AEDF4:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r31
  r10 = r10 + r31;
  // bne 0x821aedec
  if (!cr0.eq) goto loc_821AEDEC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 2))));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,572(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 572);
  // cntlzw r9,r11
  r9 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // mulli r11,r10,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // rlwinm r10,r9,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // stw r10,2176(r11)
  REX_STORE_U32(uint32_t(r11) + 2176, uint32_t(r10));
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // stw r11,3972(r1)
  REX_STORE_U32(uint32_t(r1) + 3972, uint32_t(r11));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lha r10,204(r31)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 204))));
  // b 0x821aee44
  goto loc_821AEE44;
loc_821AEE3C:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_821AEE44:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r31
  r10 = r10 + r31;
  // bne 0x821aee3c
  if (!cr0.eq) goto loc_821AEE3C;
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 2))));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lha r10,204(r31)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r31) + 204))));
  // b 0x821aee8c
  goto loc_821AEE8C;
loc_821AEE84:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_821AEE8C:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r31
  r10 = r10 + r31;
  // bne 0x821aee84
  if (!cr0.eq) goto loc_821AEE84;
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 2))));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,2104(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2104);
  f1 = double(temp.f32);
  // bl 0x821017c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821017C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // cmpwi cr6,r24,4
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 4, so);
  // beq cr6,0x821aef00
  if (cr6.eq) goto loc_821AEF00;
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // mr r4,r26
  r4 = r26;
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x82101e90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101E90, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFAB8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stwx r10,r11,r27
  REX_STORE_U32(uint32_t(r11) + uint32_t(r27), uint32_t(r10));
  // b 0x821aef04
  goto loc_821AEF04;
loc_821AEF00:
  // stwx r25,r11,r27
  REX_STORE_U32(uint32_t(r11) + uint32_t(r27), uint32_t(r25));
loc_821AEF04:
  // mr r3,r24
  r3 = r24;
  // bl 0x82101828
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101828, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x820ff968
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF968, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,8
  r10 = r10 | 8;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r3,r11,r28
  r3 = r11 + r28;
  // bl 0x820ffab0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFAB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82102048
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82102048, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,4064
  r1 = uint64_t(int64_t(int64_t(r1) + 4064));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AEF48: lifted from generated/ by lift.py.
lifted::Result f821AEF48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_821AEF48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-6016(r1)
  ea = -6016 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r30,r3
  r30 = r3;
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821af08c
  if (cr6.eq) goto loc_821AF08C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r10,r11,39200
  r10 = r11 | 39200;
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // lwzx r10,r3,r10
  r10 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r10));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 204))));
  // b 0x821aefa4
  goto loc_821AEFA4;
loc_821AEF9C:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821AEFA4:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821aef9c
  if (!cr0.eq) goto loc_821AEF9C;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // ld r10,160(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 160);
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // ld r10,168(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 168);
  // std r10,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r10);
  // ld r10,200(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 200);
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // blt cr6,0x821af014
  if (cr6.lt) goto loc_821AF014;
  // beq cr6,0x821af008
  if (cr6.eq) goto loc_821AF008;
  // cmplwi cr6,r30,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 3, so);
  // bge cr6,0x821af01c
  if (!cr6.lt) goto loc_821AF01C;
  // ld r11,84(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 84);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // b 0x821af01c
  goto loc_821AF01C;
loc_821AF008:
  // ld r11,52(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 52);
  // std r11,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r11);
  // b 0x821af01c
  goto loc_821AF01C;
loc_821AF014:
  // ld r11,44(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 44);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
loc_821AF01C:
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // stb r11,192(r1)
  REX_STORE_U8(uint32_t(r1) + 192, uint8_t(r11));
  // lwz r11,92(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 92);
  // stb r11,193(r1)
  REX_STORE_U8(uint32_t(r1) + 193, uint8_t(r11));
  // lwz r11,96(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 96);
  // stb r11,190(r1)
  REX_STORE_U8(uint32_t(r1) + 190, uint8_t(r11));
  // lwz r11,100(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 100);
  // stb r11,191(r1)
  REX_STORE_U8(uint32_t(r1) + 191, uint8_t(r11));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stb r11,200(r1)
  REX_STORE_U8(uint32_t(r1) + 200, uint8_t(r11));
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // stb r11,201(r1)
  REX_STORE_U8(uint32_t(r1) + 201, uint8_t(r11));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,5968(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 5968);
  // addi r3,r1,2080
  r3 = uint64_t(int64_t(int64_t(r1) + 2080));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,5968(r1)
  REX_STORE_U32(uint32_t(r1) + 5968, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r4,r1,2080
  r4 = uint64_t(int64_t(int64_t(r1) + 2080));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82178c40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82178C40, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821AF08C:
  // addi r1,r1,6016
  r1 = uint64_t(int64_t(int64_t(r1) + 6016));
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

// sub_821AF0A8: lifted from generated/ by lift.py.
lifted::Result f821AF0A8(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821AF0A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // ld r12,-8192(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -8192);
  // stwu r1,-9936(r1)
  ea = -9936 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r10,r11,39200
  r10 = r11 | 39200;
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // lwzx r9,r3,r10
  r9 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r10));
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lha r10,204(r11)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 204))));
  // b 0x821af0e8
  goto loc_821AF0E8;
loc_821AF0E0:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 0))));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
loc_821AF0E8:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // bne 0x821af0e0
  if (!cr0.eq) goto loc_821AF0E0;
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 2))));
  // addi r3,r1,4096
  r3 = uint64_t(int64_t(int64_t(r1) + 4096));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r30,r11,212
  r30 = uint64_t(int64_t(int64_t(r11) + 212));
  // addi r29,r30,1648
  r29 = uint64_t(int64_t(int64_t(r30) + 1648));
  // bl 0x820c4550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // addi r3,r1,4096
  r3 = uint64_t(int64_t(int64_t(r1) + 4096));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,4096
  r3 = uint64_t(int64_t(int64_t(r1) + 4096));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,4096
  r3 = uint64_t(int64_t(int64_t(r1) + 4096));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r4,12(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 12);
  // bl 0x820cd8a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD8A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,8000
  r3 = uint64_t(int64_t(int64_t(r1) + 8000));
  // bl 0x820aef80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820AEF80, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,8000
  r4 = uint64_t(int64_t(int64_t(r1) + 8000));
  // addi r3,r1,4096
  r3 = uint64_t(int64_t(int64_t(r1) + 4096));
  // bl 0x820cd5c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r10,6100(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 6100);
  // addi r9,r3,8
  r9 = uint64_t(int64_t(int64_t(r3) + 8));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // addi r8,r11,528
  r8 = uint64_t(int64_t(int64_t(r11) + 528));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // mtctr r11
  ctr = r11;
loc_821AF188:
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bdnz 0x821af188
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821AF188;
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // lwz r9,6116(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 6116);
  // cmpwi cr6,r10,999
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 999, so);
  // divw r11,r10,r11
  r11 = uint32_t((int32_t(r11) && !(int32_t(r10) == INT32_MIN && int32_t(r11) == -1)) ? int32_t(r10) / int32_t(r11) : 0);
  // mulli r8,r11,7
  r8 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(7))));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r9,548(r11)
  REX_STORE_U32(uint32_t(r11) + 548, uint32_t(r9));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // subf r9,r8,r10
  r9 = r10 - r8;
  // stw r9,552(r11)
  REX_STORE_U32(uint32_t(r11) + 552, uint32_t(r9));
  // ble cr6,0x821af1cc
  if (!cr6.gt) goto loc_821AF1CC;
  // li r10,999
  r10 = uint64_t(int64_t(999));
loc_821AF1CC:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // stw r10,556(r11)
  REX_STORE_U32(uint32_t(r11) + 556, uint32_t(r10));
  // lwz r11,32(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 32);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // bne cr6,0x821af1f4
  if (!cr6.eq) goto loc_821AF1F4;
  // stw r24,584(r11)
  REX_STORE_U32(uint32_t(r11) + 584, uint32_t(r24));
  // b 0x821af1f8
  goto loc_821AF1F8;
loc_821AF1F4:
  // stw r25,584(r11)
  REX_STORE_U32(uint32_t(r11) + 584, uint32_t(r25));
loc_821AF1F8:
  // lwz r11,56(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 56);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // bne cr6,0x821af210
  if (!cr6.eq) goto loc_821AF210;
  // stw r24,588(r11)
  REX_STORE_U32(uint32_t(r11) + 588, uint32_t(r24));
  // b 0x821af214
  goto loc_821AF214;
loc_821AF210:
  // stw r25,588(r11)
  REX_STORE_U32(uint32_t(r11) + 588, uint32_t(r25));
loc_821AF214:
  // lwz r11,40(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 40);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // bne cr6,0x821af22c
  if (!cr6.eq) goto loc_821AF22C;
  // stw r24,592(r11)
  REX_STORE_U32(uint32_t(r11) + 592, uint32_t(r24));
  // b 0x821af230
  goto loc_821AF230;
loc_821AF22C:
  // stw r25,592(r11)
  REX_STORE_U32(uint32_t(r11) + 592, uint32_t(r25));
loc_821AF230:
  // lwz r11,136(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 136);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821af248
  if (!cr6.eq) goto loc_821AF248;
  // lwz r11,7840(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 7840);
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // b 0x821af27c
  goto loc_821AF27C;
loc_821AF248:
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r4,12(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 12);
  // bl 0x820c9e00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C9E00, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r5,136(r29)
  r5 = REX_LOAD_U32(uint32_t(r29) + 136);
  // bl 0x820cbc90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CBC90, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // rlwinm. r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_821AF27C:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // beq 0x821af28c
  if (cr0.eq) goto loc_821AF28C;
  // stw r25,596(r11)
  REX_STORE_U32(uint32_t(r11) + 596, uint32_t(r25));
  // b 0x821af290
  goto loc_821AF290;
loc_821AF28C:
  // stw r24,596(r11)
  REX_STORE_U32(uint32_t(r11) + 596, uint32_t(r24));
loc_821AF290:
  // lbz r10,130(r28)
  r10 = REX_LOAD_U8(uint32_t(r28) + 130);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,688(r11)
  REX_STORE_U32(uint32_t(r11) + 688, uint32_t(r10));
  // lwz r10,52(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 52);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,692(r11)
  REX_STORE_U32(uint32_t(r11) + 692, uint32_t(r10));
  // lfs f0,8388(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 8388);
  f0 = double(temp.f32);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stfs f0,696(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 696, temp.u32);
  // lfs f0,9280(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 9280);
  f0 = double(temp.f32);
  // stfs f0,700(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 700, temp.u32);
  // lwz r10,100(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 100);
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r3,2
  r30 = uint64_t(int64_t(int64_t(r3) + 131072));
  // stw r25,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r25));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // addi r30,r30,-10964
  r30 = uint64_t(int64_t(int64_t(r30) + -10964));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82105bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105BF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // mr r28,r3
  r28 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82105bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105BF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // mr r27,r3
  r27 = r3;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82105bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105BF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // mr r29,r24
  r29 = r24;
  // mr r30,r24
  r30 = r24;
loc_821AF328:
  // lha r11,48(r28)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r28) + 48))));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // ble cr6,0x821af388
  if (!cr6.gt) goto loc_821AF388;
  // mr r5,r29
  r5 = r29;
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // lwz r10,-31648(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // mr r5,r29
  r5 = r29;
  // mr r4,r28
  r4 = r28;
  // add r10,r30,r10
  r10 = r30 + r10;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // stw r11,348(r10)
  REX_STORE_U32(uint32_t(r10) + 348, uint32_t(r11));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r10,352(r11)
  REX_STORE_U32(uint32_t(r11) + 352, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r25,356(r11)
  REX_STORE_U32(uint32_t(r11) + 356, uint32_t(r25));
  // b 0x821af394
  goto loc_821AF394;
loc_821AF388:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r24,356(r11)
  REX_STORE_U32(uint32_t(r11) + 356, uint32_t(r24));
loc_821AF394:
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r30,60
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 60, so);
  // blt cr6,0x821af328
  if (cr6.lt) goto loc_821AF328;
  // mr r29,r24
  r29 = r24;
  // mr r30,r24
  r30 = r24;
loc_821AF3AC:
  // lha r11,48(r27)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r27) + 48))));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // ble cr6,0x821af40c
  if (!cr6.gt) goto loc_821AF40C;
  // mr r5,r29
  r5 = r29;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // lwz r10,-31648(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // mr r5,r29
  r5 = r29;
  // mr r4,r27
  r4 = r27;
  // add r10,r30,r10
  r10 = r30 + r10;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // stw r11,408(r10)
  REX_STORE_U32(uint32_t(r10) + 408, uint32_t(r11));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r10,412(r11)
  REX_STORE_U32(uint32_t(r11) + 412, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r25,416(r11)
  REX_STORE_U32(uint32_t(r11) + 416, uint32_t(r25));
  // b 0x821af418
  goto loc_821AF418;
loc_821AF40C:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r24,416(r11)
  REX_STORE_U32(uint32_t(r11) + 416, uint32_t(r24));
loc_821AF418:
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r30,60
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 60, so);
  // blt cr6,0x821af3ac
  if (cr6.lt) goto loc_821AF3AC;
  // mr r29,r24
  r29 = r24;
  // mr r30,r24
  r30 = r24;
loc_821AF430:
  // lha r11,48(r26)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r26) + 48))));
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // ble cr6,0x821af490
  if (!cr6.gt) goto loc_821AF490;
  // mr r5,r29
  r5 = r29;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // lwz r10,-31648(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // mr r5,r29
  r5 = r29;
  // mr r4,r26
  r4 = r26;
  // add r10,r30,r10
  r10 = r30 + r10;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // stw r11,468(r10)
  REX_STORE_U32(uint32_t(r10) + 468, uint32_t(r11));
  // bl 0x821aae90
  // (left out: dead in C++)
  r3 = f821AAE90(base, r1, r13, r3, r4, r5);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r10,472(r11)
  REX_STORE_U32(uint32_t(r11) + 472, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r25,476(r11)
  REX_STORE_U32(uint32_t(r11) + 476, uint32_t(r25));
  // b 0x821af49c
  goto loc_821AF49C;
loc_821AF490:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r24,476(r11)
  REX_STORE_U32(uint32_t(r11) + 476, uint32_t(r24));
loc_821AF49C:
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r30,60
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 60, so);
  // blt cr6,0x821af430
  if (cr6.lt) goto loc_821AF430;
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lfs f0,4136(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4136);
  f0 = double(temp.f32);
  // stfs f0,116(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 116, temp.u32);
  // lfs f0,4140(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4140);
  f0 = double(temp.f32);
  // stfs f0,120(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 120, temp.u32);
  // lfs f0,4144(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4144);
  f0 = double(temp.f32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // lfs f0,4148(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4148);
  f0 = double(temp.f32);
  // stfs f0,128(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 128, temp.u32);
  // lfs f0,4152(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4152);
  f0 = double(temp.f32);
  // stfs f0,132(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 132, temp.u32);
  // lfs f0,4156(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4156);
  f0 = double(temp.f32);
  // stfs f0,136(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 136, temp.u32);
  // lfs f0,4132(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 4132);
  f0 = double(temp.f32);
  // stfs f0,140(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 140, temp.u32);
  // lbz r11,4168(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4168);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,144(r11)
  REX_STORE_U32(uint32_t(r11) + 144, uint32_t(r10));
  // lbz r11,4169(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4169);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,148(r11)
  REX_STORE_U32(uint32_t(r11) + 148, uint32_t(r10));
  // lbz r11,4170(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4170);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,152(r11)
  REX_STORE_U32(uint32_t(r11) + 152, uint32_t(r10));
  // lbz r11,4171(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4171);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,156(r11)
  REX_STORE_U32(uint32_t(r11) + 156, uint32_t(r10));
  // lbz r11,4176(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4176);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,160(r11)
  REX_STORE_U32(uint32_t(r11) + 160, uint32_t(r10));
  // lbz r11,4177(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4177);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,164(r11)
  REX_STORE_U32(uint32_t(r11) + 164, uint32_t(r10));
  // lbz r11,4174(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4174);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,168(r11)
  REX_STORE_U32(uint32_t(r11) + 168, uint32_t(r10));
  // lbz r11,4175(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4175);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,172(r11)
  REX_STORE_U32(uint32_t(r11) + 172, uint32_t(r10));
  // lbz r11,4178(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4178);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,176(r11)
  REX_STORE_U32(uint32_t(r11) + 176, uint32_t(r10));
  // lbz r11,4179(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4179);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,180(r11)
  REX_STORE_U32(uint32_t(r11) + 180, uint32_t(r10));
  // lbz r11,4180(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4180);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,184(r11)
  REX_STORE_U32(uint32_t(r11) + 184, uint32_t(r10));
  // lbz r11,4181(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4181);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,188(r11)
  REX_STORE_U32(uint32_t(r11) + 188, uint32_t(r10));
  // lbz r11,4182(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4182);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,192(r11)
  REX_STORE_U32(uint32_t(r11) + 192, uint32_t(r10));
  // lbz r11,4183(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4183);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,196(r11)
  REX_STORE_U32(uint32_t(r11) + 196, uint32_t(r10));
  // lbz r11,4184(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4184);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,200(r11)
  REX_STORE_U32(uint32_t(r11) + 200, uint32_t(r10));
  // lbz r11,4185(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4185);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,204(r11)
  REX_STORE_U32(uint32_t(r11) + 204, uint32_t(r10));
  // lbz r11,4190(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4190);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,208(r11)
  REX_STORE_U32(uint32_t(r11) + 208, uint32_t(r10));
  // lbz r11,4189(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4189);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // mulli r10,r11,5
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(5))));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,212(r11)
  REX_STORE_U32(uint32_t(r11) + 212, uint32_t(r10));
  // lbz r11,4172(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4172);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,224(r11)
  REX_STORE_U32(uint32_t(r11) + 224, uint32_t(r10));
  // lbz r11,4173(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4173);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,228(r11)
  REX_STORE_U32(uint32_t(r11) + 228, uint32_t(r10));
  // lbz r11,4186(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4186);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,216(r11)
  REX_STORE_U32(uint32_t(r11) + 216, uint32_t(r10));
  // lbz r11,4187(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 4187);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,220(r11)
  REX_STORE_U32(uint32_t(r11) + 220, uint32_t(r10));
  // lfs f0,232(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 232);
  f0 = double(temp.f32);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stfs f0,232(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 232, temp.u32);
  // lfs f0,236(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 236);
  f0 = double(temp.f32);
  // stfs f0,236(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 236, temp.u32);
  // lfs f0,240(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 240);
  f0 = double(temp.f32);
  // stfs f0,240(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 240, temp.u32);
  // lfs f0,244(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 244);
  f0 = double(temp.f32);
  // stfs f0,244(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 244, temp.u32);
  // lfs f0,248(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 248);
  f0 = double(temp.f32);
  // stfs f0,248(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 248, temp.u32);
  // lfs f0,252(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 252);
  f0 = double(temp.f32);
  // stfs f0,252(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 252, temp.u32);
  // lfs f0,228(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 228);
  f0 = double(temp.f32);
  // stfs f0,256(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 256, temp.u32);
  // lbz r11,264(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 264);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,260(r11)
  REX_STORE_U32(uint32_t(r11) + 260, uint32_t(r10));
  // lbz r11,265(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 265);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,264(r11)
  REX_STORE_U32(uint32_t(r11) + 264, uint32_t(r10));
  // lbz r11,266(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 266);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,268(r11)
  REX_STORE_U32(uint32_t(r11) + 268, uint32_t(r10));
  // lbz r11,267(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 267);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,272(r11)
  REX_STORE_U32(uint32_t(r11) + 272, uint32_t(r10));
  // lbz r11,272(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 272);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,276(r11)
  REX_STORE_U32(uint32_t(r11) + 276, uint32_t(r10));
  // lbz r11,273(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 273);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,280(r11)
  REX_STORE_U32(uint32_t(r11) + 280, uint32_t(r10));
  // lbz r11,270(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 270);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,284(r11)
  REX_STORE_U32(uint32_t(r11) + 284, uint32_t(r10));
  // lbz r11,271(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 271);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,288(r11)
  REX_STORE_U32(uint32_t(r11) + 288, uint32_t(r10));
  // lbz r11,274(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 274);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,292(r11)
  REX_STORE_U32(uint32_t(r11) + 292, uint32_t(r10));
  // lbz r11,275(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 275);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,296(r11)
  REX_STORE_U32(uint32_t(r11) + 296, uint32_t(r10));
  // lbz r11,276(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 276);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,300(r11)
  REX_STORE_U32(uint32_t(r11) + 300, uint32_t(r10));
  // lbz r11,277(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 277);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,304(r11)
  REX_STORE_U32(uint32_t(r11) + 304, uint32_t(r10));
  // lbz r11,278(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 278);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,308(r11)
  REX_STORE_U32(uint32_t(r11) + 308, uint32_t(r10));
  // lbz r11,279(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 279);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,312(r11)
  REX_STORE_U32(uint32_t(r11) + 312, uint32_t(r10));
  // lbz r11,280(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 280);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,316(r11)
  REX_STORE_U32(uint32_t(r11) + 316, uint32_t(r10));
  // lbz r11,281(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 281);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,320(r11)
  REX_STORE_U32(uint32_t(r11) + 320, uint32_t(r10));
  // lbz r11,286(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 286);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,324(r11)
  REX_STORE_U32(uint32_t(r11) + 324, uint32_t(r10));
  // lbz r11,285(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 285);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // mulli r10,r11,5
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(5))));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,328(r11)
  REX_STORE_U32(uint32_t(r11) + 328, uint32_t(r10));
  // lbz r11,268(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 268);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,340(r11)
  REX_STORE_U32(uint32_t(r11) + 340, uint32_t(r10));
  // lbz r11,269(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 269);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,344(r11)
  REX_STORE_U32(uint32_t(r11) + 344, uint32_t(r10));
  // lbz r11,282(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 282);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,332(r11)
  REX_STORE_U32(uint32_t(r11) + 332, uint32_t(r10));
  // lbz r11,283(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 283);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,336(r11)
  REX_STORE_U32(uint32_t(r11) + 336, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821af83c
  if (!cr6.eq) goto loc_821AF83C;
  // lfs f0,232(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 232);
  f0 = double(temp.f32);
  // stfs f0,116(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 116, temp.u32);
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lfs f0,236(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 236);
  f0 = double(temp.f32);
  // stfs f0,120(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 120, temp.u32);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lfs f0,240(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 240);
  f0 = double(temp.f32);
  // stfs f0,124(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // lfs f0,244(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 244);
  f0 = double(temp.f32);
  // stfs f0,128(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 128, temp.u32);
  // lfs f0,248(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 248);
  f0 = double(temp.f32);
  // stfs f0,132(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 132, temp.u32);
  // lfs f0,252(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 252);
  f0 = double(temp.f32);
  // stfs f0,136(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 136, temp.u32);
  // lfs f0,228(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 228);
  f0 = double(temp.f32);
  // stfs f0,140(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 140, temp.u32);
loc_821AF83C:
  // lwz r10,588(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821af868
  if (!cr6.eq) goto loc_821AF868;
  // lbz r10,266(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 266);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // stw r10,152(r11)
  REX_STORE_U32(uint32_t(r11) + 152, uint32_t(r10));
  // lbz r11,267(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 267);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,156(r11)
  REX_STORE_U32(uint32_t(r11) + 156, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AF868:
  // lwz r10,592(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821af934
  if (!cr6.eq) goto loc_821AF934;
  // lbz r10,272(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 272);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // stw r10,160(r11)
  REX_STORE_U32(uint32_t(r11) + 160, uint32_t(r10));
  // lbz r11,273(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 273);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,164(r11)
  REX_STORE_U32(uint32_t(r11) + 164, uint32_t(r10));
  // lbz r11,270(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 270);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,168(r11)
  REX_STORE_U32(uint32_t(r11) + 168, uint32_t(r10));
  // lbz r11,271(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 271);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,172(r11)
  REX_STORE_U32(uint32_t(r11) + 172, uint32_t(r10));
  // lbz r11,274(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 274);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,176(r11)
  REX_STORE_U32(uint32_t(r11) + 176, uint32_t(r10));
  // lbz r11,275(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 275);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,180(r11)
  REX_STORE_U32(uint32_t(r11) + 180, uint32_t(r10));
  // lbz r11,276(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 276);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,184(r11)
  REX_STORE_U32(uint32_t(r11) + 184, uint32_t(r10));
  // lbz r11,277(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 277);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,188(r11)
  REX_STORE_U32(uint32_t(r11) + 188, uint32_t(r10));
  // lbz r11,278(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 278);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,192(r11)
  REX_STORE_U32(uint32_t(r11) + 192, uint32_t(r10));
  // lbz r11,279(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 279);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,196(r11)
  REX_STORE_U32(uint32_t(r11) + 196, uint32_t(r10));
  // lbz r11,280(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 280);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,200(r11)
  REX_STORE_U32(uint32_t(r11) + 200, uint32_t(r10));
  // lbz r11,281(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 281);
  // extsb r10,r11
  r10 = uint64_t(int64_t(int8_t(r11)));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,204(r11)
  REX_STORE_U32(uint32_t(r11) + 204, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AF934:
  // lwz r10,596(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821af950
  if (!cr6.eq) goto loc_821AF950;
  // lbz r10,286(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 286);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // stw r10,208(r11)
  REX_STORE_U32(uint32_t(r11) + 208, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AF950:
  // lis r10,-32160
  r10 = uint64_t(int64_t(-2107637760));
  // lwz r10,13020(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 13020);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821af9fc
  if (cr6.eq) goto loc_821AF9FC;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x821afa00
  if (!cr6.eq) goto loc_821AFA00;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,-31648(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,6108(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 6108);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // cmpwi cr6,r11,14
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 14, so);
  // stw r25,560(r10)
  REX_STORE_U32(uint32_t(r10) + 560, uint32_t(r25));
  // lwz r10,-31648(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r9,752(r10)
  REX_STORE_U32(uint32_t(r10) + 752, uint32_t(r9));
  // beq cr6,0x821af9f0
  if (cr6.eq) goto loc_821AF9F0;
  // cmpwi cr6,r11,15
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 15, so);
  // beq cr6,0x821af9e4
  if (cr6.eq) goto loc_821AF9E4;
  // cmpwi cr6,r11,16
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 16, so);
  // beq cr6,0x821af9d4
  if (cr6.eq) goto loc_821AF9D4;
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x821af9c8
  if (cr6.eq) goto loc_821AF9C8;
  // cmpwi cr6,r11,18
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 18, so);
  // beq cr6,0x821af9c0
  if (cr6.eq) goto loc_821AF9C0;
  // cmpwi cr6,r11,19
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 19, so);
  // bne cr6,0x821afa00
  if (!cr6.eq) goto loc_821AFA00;
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // b 0x821af9d8
  goto loc_821AF9D8;
loc_821AF9C0:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x821af9d8
  goto loc_821AF9D8;
loc_821AF9C8:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r9,752(r11)
  REX_STORE_U32(uint32_t(r11) + 752, uint32_t(r9));
  // b 0x821afa00
  goto loc_821AFA00;
loc_821AF9D4:
  // li r10,2
  r10 = uint64_t(int64_t(2));
loc_821AF9D8:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r10,752(r11)
  REX_STORE_U32(uint32_t(r11) + 752, uint32_t(r10));
  // b 0x821afa00
  goto loc_821AFA00;
loc_821AF9E4:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r25,752(r11)
  REX_STORE_U32(uint32_t(r11) + 752, uint32_t(r25));
  // b 0x821afa00
  goto loc_821AFA00;
loc_821AF9F0:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r24,752(r11)
  REX_STORE_U32(uint32_t(r11) + 752, uint32_t(r24));
  // b 0x821afa00
  goto loc_821AFA00;
loc_821AF9FC:
  // stw r24,560(r11)
  REX_STORE_U32(uint32_t(r11) + 560, uint32_t(r24));
loc_821AFA00:
  // addi r1,r1,9936
  r1 = uint64_t(int64_t(int64_t(r1) + 9936));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AFA08: lifted from generated/ by lift.py.
uint64_t f821AFA08(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AFA08 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r8,r3,6120
  r8 = uint64_t(int64_t(int64_t(r3) + 6120));
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r11,r11,39200
  r11 = r11 | 39200;
  // lwzx r10,r3,r11
  r10 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r11));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r11,204(r8)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r8) + 204))));
  // b 0x821afa40
  goto loc_821AFA40;
loc_821AFA38:
  // lha r11,0(r11)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821AFA40:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r8
  r11 = r11 + r8;
  // bne 0x821afa38
  if (!cr0.eq) goto loc_821AFA38;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lha r11,2(r11)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 2))));
  // li r7,5
  r7 = uint64_t(int64_t(5));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // add r11,r11,r8
  r11 = r11 + r8;
  // lfs f0,116(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 116);
  f0 = double(temp.f32);
  // stfs f0,252(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 252, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,120(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 120);
  f0 = double(temp.f32);
  // stfs f0,256(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 256, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,124(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 124);
  f0 = double(temp.f32);
  // stfs f0,260(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 260, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,128(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 128);
  f0 = double(temp.f32);
  // stfs f0,264(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 264, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,132(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 132);
  f0 = double(temp.f32);
  // stfs f0,268(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 268, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,136(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 136);
  f0 = double(temp.f32);
  // stfs f0,272(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 272, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lfs f0,140(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 140);
  f0 = double(temp.f32);
  // stfs f0,248(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 248, temp.u32);
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,144(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 144);
  // stb r9,284(r11)
  REX_STORE_U8(uint32_t(r11) + 284, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,148(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 148);
  // stb r9,285(r11)
  REX_STORE_U8(uint32_t(r11) + 285, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,152(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 152);
  // stb r9,286(r11)
  REX_STORE_U8(uint32_t(r11) + 286, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,156(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 156);
  // stb r9,287(r11)
  REX_STORE_U8(uint32_t(r11) + 287, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,160(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 160);
  // stb r9,292(r11)
  REX_STORE_U8(uint32_t(r11) + 292, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,164(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 164);
  // stb r9,293(r11)
  REX_STORE_U8(uint32_t(r11) + 293, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,168(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 168);
  // stb r9,290(r11)
  REX_STORE_U8(uint32_t(r11) + 290, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,172(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 172);
  // stb r9,291(r11)
  REX_STORE_U8(uint32_t(r11) + 291, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,176(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 176);
  // stb r9,294(r11)
  REX_STORE_U8(uint32_t(r11) + 294, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,180(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 180);
  // stb r9,295(r11)
  REX_STORE_U8(uint32_t(r11) + 295, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,184(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 184);
  // stb r9,296(r11)
  REX_STORE_U8(uint32_t(r11) + 296, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,188(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 188);
  // stb r9,297(r11)
  REX_STORE_U8(uint32_t(r11) + 297, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,192(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 192);
  // stb r9,298(r11)
  REX_STORE_U8(uint32_t(r11) + 298, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,196(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 196);
  // stb r9,299(r11)
  REX_STORE_U8(uint32_t(r11) + 299, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,200(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 200);
  // stb r9,300(r11)
  REX_STORE_U8(uint32_t(r11) + 300, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,204(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 204);
  // stb r9,301(r11)
  REX_STORE_U8(uint32_t(r11) + 301, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,208(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 208);
  // stb r9,306(r11)
  REX_STORE_U8(uint32_t(r11) + 306, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,212(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 212);
  // divw r9,r9,r7
  r9 = uint32_t((int32_t(r7) && !(int32_t(r9) == INT32_MIN && int32_t(r7) == -1)) ? int32_t(r9) / int32_t(r7) : 0);
  // stb r9,305(r11)
  REX_STORE_U8(uint32_t(r11) + 305, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,224(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 224);
  // stb r9,288(r11)
  REX_STORE_U8(uint32_t(r11) + 288, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,228(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 228);
  // stb r9,289(r11)
  REX_STORE_U8(uint32_t(r11) + 289, uint8_t(r9));
  // lwz r9,-31648(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,216(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 216);
  // stb r9,302(r11)
  REX_STORE_U8(uint32_t(r11) + 302, uint8_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,220(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 220);
  // stb r10,303(r11)
  REX_STORE_U8(uint32_t(r11) + 303, uint8_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821AFBD0: lifted from generated/ by lift.py.
lifted::Result f821AFBD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AFBD0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0;
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
  // bl 0x821af0a8
  // (left out: dead in C++)
  { const lifted::Result o = f821AF0A8(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ab2c0
  // (left out: dead in C++)
  { const lifted::Result o = f821AB2C0(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r11,580(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 580);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821afc40
  if (cr6.eq) goto loc_821AFC40;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821afc10
  if (cr6.eq) goto loc_821AFC10;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFC10:
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // li r8,23
  r8 = uint64_t(int64_t(23));
  // li r7,5
  r7 = uint64_t(int64_t(5));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // stw r5,744(r11)
  REX_STORE_U32(uint32_t(r11) + 744, uint32_t(r5));
  // b 0x821afc74
  goto loc_821AFC74;
loc_821AFC40:
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821afc4c
  if (cr0.eq) goto loc_821AFC4C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFC4C:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // stw r10,744(r11)
  REX_STORE_U32(uint32_t(r11) + 744, uint32_t(r10));
loc_821AFC74:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821AFC88: lifted from generated/ by lift.py.
uint64_t f821AFC88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821AFC88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r3
  r31 = r3;
  // lwz r11,13020(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 13020);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821afcbc
  if (cr6.eq) goto loc_821AFCBC;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x821afd08
  if (!cr6.eq) goto loc_821AFD08;
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // b 0x821afcc0
  goto loc_821AFCC0;
loc_821AFCBC:
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_821AFCC0:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
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
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
loc_821AFD08:
  // bl 0x821afa08
  // (left out: dead in C++)
  r3 = f821AFA08(base, r1, r13);
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,9604(r31)
  REX_STORE_U32(uint32_t(r31) + 9604, uint32_t(r11));
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

// sub_821AFD28: lifted from generated/ by lift.py.
lifted::Result f821AFD28(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AFD28 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x821afa08
  // (left out: dead in C++)
  r3 = f821AFA08(base, r1, r13);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,568(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 568);
  // bl 0x821aed18
  // (left out: dead in C++)
  { const lifted::Result o = f821AED18(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // stw r10,576(r11)
  REX_STORE_U32(uint32_t(r11) + 576, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // stw r9,760(r11)
  REX_STORE_U32(uint32_t(r11) + 760, uint32_t(r9));
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

// sub_821AFD78: lifted from generated/ by lift.py.
lifted::Result f821AFD78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AFD78 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821afde0
  if (cr6.eq) goto loc_821AFDE0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821AFDAC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x821afdc4
  if (cr6.eq) goto loc_821AFDC4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFDC4:
  // li r9,29
  r9 = uint64_t(int64_t(29));
loc_821AFDC8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821aff14
  goto loc_821AFF14;
loc_821AFDE0:
  // bl 0x821a8ef8
  // (left out: dead in C++)
  r3 = f821A8EF8(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821afe14
  if (cr0.eq) goto loc_821AFE14;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x821afe0c
  if (cr6.eq) goto loc_821AFE0C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFE0C:
  // li r9,30
  r9 = uint64_t(int64_t(30));
  // b 0x821afdc8
  goto loc_821AFDC8;
loc_821AFE14:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821afeac
  if (cr0.eq) goto loc_821AFEAC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // bne cr6,0x821afe98
  if (!cr6.eq) goto loc_821AFE98;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31648(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,732(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821afe7c
  if (cr6.lt) goto loc_821AFE7C;
  // beq cr6,0x821afe68
  if (cr6.eq) goto loc_821AFE68;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821aff18
  if (!cr6.lt) goto loc_821AFF18;
  // addi r4,r3,232
  r4 = uint64_t(int64_t(int64_t(r3) + 232));
  // b 0x821afe6c
  goto loc_821AFE6C;
loc_821AFE68:
  // addi r4,r3,116
  r4 = uint64_t(int64_t(int64_t(r3) + 116));
loc_821AFE6C:
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ad840
  // (left out: dead in C++)
  { const lifted::Result o = f821AD840(base, r1, r13, r7); r3 = o.r3; f1 = o.f1; }
  // b 0x821aff18
  goto loc_821AFF18;
loc_821AFE7C:
  // addi r11,r3,116
  r11 = uint64_t(int64_t(int64_t(r3) + 116));
  // mr r4,r3
  r4 = r3;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r3,r11
  r3 = r11;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ada08
  // (left out: dead in C++)
  r3 = f821ADA08(base, r1, r13);
  // b 0x821afee0
  goto loc_821AFEE0;
loc_821AFE98:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821afdac
  goto loc_821AFDAC;
loc_821AFEAC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aff18
  if (cr0.eq) goto loc_821AFF18;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // lwz r3,-31648(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r4,r3,116
  r4 = uint64_t(int64_t(int64_t(r3) + 116));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ad840
  // (left out: dead in C++)
  { const lifted::Result o = f821AD840(base, r1, r13, r7); r3 = o.r3; f1 = o.f1; }
loc_821AFEE0:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821afef8
  if (cr6.eq) goto loc_821AFEF8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFEF8:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821AFF14:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821AFF18:
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

// sub_821AFF30: lifted from generated/ by lift.py.
lifted::Result f821AFF30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AFF30 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aff98
  if (cr6.eq) goto loc_821AFF98;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821AFF64:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821aff7c
  if (cr6.eq) goto loc_821AFF7C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFF7C:
  // li r9,34
  r9 = uint64_t(int64_t(34));
loc_821AFF80:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,9
  r8 = uint64_t(int64_t(9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b00e8
  goto loc_821B00E8;
loc_821AFF98:
  // bl 0x821a9248
  // (left out: dead in C++)
  r3 = f821A9248(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821affcc
  if (cr0.eq) goto loc_821AFFCC;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821affc4
  if (cr6.eq) goto loc_821AFFC4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AFFC4:
  // li r9,35
  r9 = uint64_t(int64_t(35));
  // b 0x821aff80
  goto loc_821AFF80;
loc_821AFFCC:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0074
  if (cr0.eq) goto loc_821B0074;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0060
  if (!cr6.eq) goto loc_821B0060;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0040
  if (cr6.lt) goto loc_821B0040;
  // beq cr6,0x821b0020
  if (cr6.eq) goto loc_821B0020;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b00ec
  if (!cr6.lt) goto loc_821B00EC;
  // addi r10,r11,260
  r10 = uint64_t(int64_t(int64_t(r11) + 260));
  // b 0x821b0024
  goto loc_821B0024;
loc_821B0020:
  // addi r10,r11,144
  r10 = uint64_t(int64_t(int64_t(r11) + 144));
loc_821B0024:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821adc48
  // (left out: dead in C++)
  { const lifted::Result o = f821ADC48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b00ec
  goto loc_821B00EC;
loc_821B0040:
  // addi r10,r11,28
  r10 = uint64_t(int64_t(int64_t(r11) + 28));
  // addi r11,r11,144
  r11 = uint64_t(int64_t(int64_t(r11) + 144));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821add00
  // (left out: dead in C++)
  r3 = f821ADD00(base, r1, r13);
  // b 0x821b00b4
  goto loc_821B00B4;
loc_821B0060:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821aff64
  goto loc_821AFF64;
loc_821B0074:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b00ec
  if (cr0.eq) goto loc_821B00EC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,144
  r10 = uint64_t(int64_t(int64_t(r11) + 144));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821adc48
  // (left out: dead in C++)
  { const lifted::Result o = f821ADC48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B00B4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b00cc
  if (cr6.eq) goto loc_821B00CC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B00CC:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,36
  r9 = uint64_t(int64_t(36));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B00E8:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B00EC:
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

// sub_821B0100: lifted from generated/ by lift.py.
lifted::Result f821B0100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0100 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0168
  if (cr6.eq) goto loc_821B0168;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0134:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x821b014c
  if (cr6.eq) goto loc_821B014C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B014C:
  // li r9,39
  r9 = uint64_t(int64_t(39));
loc_821B0150:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,10
  r8 = uint64_t(int64_t(10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b02b8
  goto loc_821B02B8;
loc_821B0168:
  // bl 0x821a93b0
  // (left out: dead in C++)
  r3 = f821A93B0(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b019c
  if (cr0.eq) goto loc_821B019C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x821b0194
  if (cr6.eq) goto loc_821B0194;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0194:
  // li r9,40
  r9 = uint64_t(int64_t(40));
  // b 0x821b0150
  goto loc_821B0150;
loc_821B019C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0244
  if (cr0.eq) goto loc_821B0244;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0230
  if (!cr6.eq) goto loc_821B0230;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0210
  if (cr6.lt) goto loc_821B0210;
  // beq cr6,0x821b01f0
  if (cr6.eq) goto loc_821B01F0;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b02bc
  if (!cr6.lt) goto loc_821B02BC;
  // addi r10,r11,268
  r10 = uint64_t(int64_t(int64_t(r11) + 268));
  // b 0x821b01f4
  goto loc_821B01F4;
loc_821B01F0:
  // addi r10,r11,152
  r10 = uint64_t(int64_t(int64_t(r11) + 152));
loc_821B01F4:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,36
  r11 = uint64_t(int64_t(int64_t(r11) + 36));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821addb8
  // (left out: dead in C++)
  { const lifted::Result o = f821ADDB8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b02bc
  goto loc_821B02BC;
loc_821B0210:
  // addi r10,r11,36
  r10 = uint64_t(int64_t(int64_t(r11) + 36));
  // addi r11,r11,152
  r11 = uint64_t(int64_t(int64_t(r11) + 152));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ade70
  // (left out: dead in C++)
  r3 = f821ADE70(base, r1, r13);
  // b 0x821b0284
  goto loc_821B0284;
loc_821B0230:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0134
  goto loc_821B0134;
loc_821B0244:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b02bc
  if (cr0.eq) goto loc_821B02BC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,152
  r10 = uint64_t(int64_t(int64_t(r11) + 152));
  // addi r11,r11,36
  r11 = uint64_t(int64_t(int64_t(r11) + 36));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821addb8
  // (left out: dead in C++)
  { const lifted::Result o = f821ADDB8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0284:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b029c
  if (cr6.eq) goto loc_821B029C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B029C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,41
  r9 = uint64_t(int64_t(41));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B02B8:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B02BC:
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

// sub_821B02D0: lifted from generated/ by lift.py.
lifted::Result f821B02D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B02D0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0338
  if (cr6.eq) goto loc_821B0338;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0304:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x821b031c
  if (cr6.eq) goto loc_821B031C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B031C:
  // li r9,44
  r9 = uint64_t(int64_t(44));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b04b0
  goto loc_821B04B0;
loc_821B0338:
  // bl 0x821a9518
  // (left out: dead in C++)
  r3 = f821A9518(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b038c
  if (cr0.eq) goto loc_821B038C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x821b0364
  if (cr6.eq) goto loc_821B0364;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0364:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,45
  r9 = uint64_t(int64_t(45));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821B0380:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b04b4
  goto loc_821B04B4;
loc_821B038C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0434
  if (cr0.eq) goto loc_821B0434;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0420
  if (!cr6.eq) goto loc_821B0420;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0400
  if (cr6.lt) goto loc_821B0400;
  // beq cr6,0x821b03e0
  if (cr6.eq) goto loc_821B03E0;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b04b4
  if (!cr6.lt) goto loc_821B04B4;
  // addi r10,r11,276
  r10 = uint64_t(int64_t(int64_t(r11) + 276));
  // b 0x821b03e4
  goto loc_821B03E4;
loc_821B03E0:
  // addi r10,r11,160
  r10 = uint64_t(int64_t(int64_t(r11) + 160));
loc_821B03E4:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,44
  r11 = uint64_t(int64_t(int64_t(r11) + 44));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821adf98
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF98(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0380
  goto loc_821B0380;
loc_821B0400:
  // addi r10,r11,44
  r10 = uint64_t(int64_t(int64_t(r11) + 44));
  // addi r11,r11,160
  r11 = uint64_t(int64_t(int64_t(r11) + 160));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae018
  // (left out: dead in C++)
  r3 = f821AE018(base, r1, r13);
  // b 0x821b047c
  goto loc_821B047C;
loc_821B0420:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0304
  goto loc_821B0304;
loc_821B0434:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b04b4
  if (cr0.eq) goto loc_821B04B4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,160
  r10 = uint64_t(int64_t(int64_t(r11) + 160));
  // addi r11,r11,44
  r11 = uint64_t(int64_t(int64_t(r11) + 44));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821adf98
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF98(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_821B047C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b0494
  if (cr6.eq) goto loc_821B0494;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0494:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,46
  r9 = uint64_t(int64_t(46));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B04B0:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B04B4:
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

// sub_821B04C8: lifted from generated/ by lift.py.
lifted::Result f821B04C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B04C8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0530
  if (cr6.eq) goto loc_821B0530;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B04FC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x821b0514
  if (cr6.eq) goto loc_821B0514;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0514:
  // li r9,49
  r9 = uint64_t(int64_t(49));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b06a8
  goto loc_821B06A8;
loc_821B0530:
  // bl 0x821a9658
  // (left out: dead in C++)
  r3 = f821A9658(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0584
  if (cr0.eq) goto loc_821B0584;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x821b055c
  if (cr6.eq) goto loc_821B055C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B055C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,50
  r9 = uint64_t(int64_t(50));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821B0578:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b06ac
  goto loc_821B06AC;
loc_821B0584:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b062c
  if (cr0.eq) goto loc_821B062C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0618
  if (!cr6.eq) goto loc_821B0618;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b05f8
  if (cr6.lt) goto loc_821B05F8;
  // beq cr6,0x821b05d8
  if (cr6.eq) goto loc_821B05D8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b06ac
  if (!cr6.lt) goto loc_821B06AC;
  // addi r10,r11,284
  r10 = uint64_t(int64_t(int64_t(r11) + 284));
  // b 0x821b05dc
  goto loc_821B05DC;
loc_821B05D8:
  // addi r10,r11,168
  r10 = uint64_t(int64_t(int64_t(r11) + 168));
loc_821B05DC:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,52
  r11 = uint64_t(int64_t(int64_t(r11) + 52));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae098
  // (left out: dead in C++)
  { const lifted::Result o = f821AE098(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0578
  goto loc_821B0578;
loc_821B05F8:
  // addi r10,r11,52
  r10 = uint64_t(int64_t(int64_t(r11) + 52));
  // addi r11,r11,168
  r11 = uint64_t(int64_t(int64_t(r11) + 168));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae118
  // (left out: dead in C++)
  r3 = f821AE118(base, r1, r13);
  // b 0x821b0674
  goto loc_821B0674;
loc_821B0618:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b04fc
  goto loc_821B04FC;
loc_821B062C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b06ac
  if (cr0.eq) goto loc_821B06AC;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,168
  r10 = uint64_t(int64_t(int64_t(r11) + 168));
  // addi r11,r11,52
  r11 = uint64_t(int64_t(int64_t(r11) + 52));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae098
  // (left out: dead in C++)
  { const lifted::Result o = f821AE098(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0674:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b068c
  if (cr6.eq) goto loc_821B068C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B068C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,51
  r9 = uint64_t(int64_t(51));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B06A8:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B06AC:
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

// sub_821B06C0: lifted from generated/ by lift.py.
lifted::Result f821B06C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B06C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0728
  if (cr6.eq) goto loc_821B0728;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B06F4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x821b070c
  if (cr6.eq) goto loc_821B070C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B070C:
  // li r9,54
  r9 = uint64_t(int64_t(54));
loc_821B0710:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,13
  r8 = uint64_t(int64_t(13));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b0878
  goto loc_821B0878;
loc_821B0728:
  // bl 0x821a9798
  // (left out: dead in C++)
  r3 = f821A9798(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b075c
  if (cr0.eq) goto loc_821B075C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x821b0754
  if (cr6.eq) goto loc_821B0754;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0754:
  // li r9,55
  r9 = uint64_t(int64_t(55));
  // b 0x821b0710
  goto loc_821B0710;
loc_821B075C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0804
  if (cr0.eq) goto loc_821B0804;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b07f0
  if (!cr6.eq) goto loc_821B07F0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b07d0
  if (cr6.lt) goto loc_821B07D0;
  // beq cr6,0x821b07b0
  if (cr6.eq) goto loc_821B07B0;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b087c
  if (!cr6.lt) goto loc_821B087C;
  // addi r10,r11,292
  r10 = uint64_t(int64_t(int64_t(r11) + 292));
  // b 0x821b07b4
  goto loc_821B07B4;
loc_821B07B0:
  // addi r10,r11,176
  r10 = uint64_t(int64_t(int64_t(r11) + 176));
loc_821B07B4:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,60
  r11 = uint64_t(int64_t(int64_t(r11) + 60));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae240
  // (left out: dead in C++)
  { const lifted::Result o = f821AE240(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b087c
  goto loc_821B087C;
loc_821B07D0:
  // addi r10,r11,60
  r10 = uint64_t(int64_t(int64_t(r11) + 60));
  // addi r11,r11,176
  r11 = uint64_t(int64_t(int64_t(r11) + 176));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae2c0
  // (left out: dead in C++)
  r3 = f821AE2C0(base, r1, r13);
  // b 0x821b0844
  goto loc_821B0844;
loc_821B07F0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b06f4
  goto loc_821B06F4;
loc_821B0804:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b087c
  if (cr0.eq) goto loc_821B087C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,176
  r10 = uint64_t(int64_t(int64_t(r11) + 176));
  // addi r11,r11,60
  r11 = uint64_t(int64_t(int64_t(r11) + 60));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae240
  // (left out: dead in C++)
  { const lifted::Result o = f821AE240(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0844:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b085c
  if (cr6.eq) goto loc_821B085C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B085C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,56
  r9 = uint64_t(int64_t(56));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B0878:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B087C:
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

// sub_821B0890: lifted from generated/ by lift.py.
lifted::Result f821B0890(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0890 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b08f8
  if (cr6.eq) goto loc_821B08F8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B08C4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,14
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 14, so);
  // beq cr6,0x821b08dc
  if (cr6.eq) goto loc_821B08DC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B08DC:
  // li r9,59
  r9 = uint64_t(int64_t(59));
loc_821B08E0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,14
  r8 = uint64_t(int64_t(14));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b0a48
  goto loc_821B0A48;
loc_821B08F8:
  // bl 0x821a98d8
  // (left out: dead in C++)
  r3 = f821A98D8(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b092c
  if (cr0.eq) goto loc_821B092C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,14
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 14, so);
  // beq cr6,0x821b0924
  if (cr6.eq) goto loc_821B0924;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0924:
  // li r9,60
  r9 = uint64_t(int64_t(60));
  // b 0x821b08e0
  goto loc_821B08E0;
loc_821B092C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b09d4
  if (cr0.eq) goto loc_821B09D4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b09c0
  if (!cr6.eq) goto loc_821B09C0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b09a0
  if (cr6.lt) goto loc_821B09A0;
  // beq cr6,0x821b0980
  if (cr6.eq) goto loc_821B0980;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b0a4c
  if (!cr6.lt) goto loc_821B0A4C;
  // addi r10,r11,300
  r10 = uint64_t(int64_t(int64_t(r11) + 300));
  // b 0x821b0984
  goto loc_821B0984;
loc_821B0980:
  // addi r10,r11,184
  r10 = uint64_t(int64_t(int64_t(r11) + 184));
loc_821B0984:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,68
  r11 = uint64_t(int64_t(int64_t(r11) + 68));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae340
  // (left out: dead in C++)
  { const lifted::Result o = f821AE340(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0a4c
  goto loc_821B0A4C;
loc_821B09A0:
  // addi r10,r11,68
  r10 = uint64_t(int64_t(int64_t(r11) + 68));
  // addi r11,r11,184
  r11 = uint64_t(int64_t(int64_t(r11) + 184));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae3c0
  // (left out: dead in C++)
  r3 = f821AE3C0(base, r1, r13);
  // b 0x821b0a14
  goto loc_821B0A14;
loc_821B09C0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b08c4
  goto loc_821B08C4;
loc_821B09D4:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0a4c
  if (cr0.eq) goto loc_821B0A4C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,184
  r10 = uint64_t(int64_t(int64_t(r11) + 184));
  // addi r11,r11,68
  r11 = uint64_t(int64_t(int64_t(r11) + 68));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae340
  // (left out: dead in C++)
  { const lifted::Result o = f821AE340(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0A14:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b0a2c
  if (cr6.eq) goto loc_821B0A2C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0A2C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,61
  r9 = uint64_t(int64_t(61));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B0A48:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B0A4C:
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

// sub_821B0A60: lifted from generated/ by lift.py.
lifted::Result f821B0A60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0A60 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0ac8
  if (cr6.eq) goto loc_821B0AC8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0A94:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // beq cr6,0x821b0aac
  if (cr6.eq) goto loc_821B0AAC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0AAC:
  // li r9,64
  r9 = uint64_t(int64_t(64));
loc_821B0AB0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b0c18
  goto loc_821B0C18;
loc_821B0AC8:
  // bl 0x821a9a18
  // (left out: dead in C++)
  r3 = f821A9A18(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0afc
  if (cr0.eq) goto loc_821B0AFC;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // beq cr6,0x821b0af4
  if (cr6.eq) goto loc_821B0AF4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0AF4:
  // li r9,65
  r9 = uint64_t(int64_t(65));
  // b 0x821b0ab0
  goto loc_821B0AB0;
loc_821B0AFC:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0ba4
  if (cr0.eq) goto loc_821B0BA4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0b90
  if (!cr6.eq) goto loc_821B0B90;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0b70
  if (cr6.lt) goto loc_821B0B70;
  // beq cr6,0x821b0b50
  if (cr6.eq) goto loc_821B0B50;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b0c1c
  if (!cr6.lt) goto loc_821B0C1C;
  // addi r10,r11,308
  r10 = uint64_t(int64_t(int64_t(r11) + 308));
  // b 0x821b0b54
  goto loc_821B0B54;
loc_821B0B50:
  // addi r10,r11,192
  r10 = uint64_t(int64_t(int64_t(r11) + 192));
loc_821B0B54:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,76
  r11 = uint64_t(int64_t(int64_t(r11) + 76));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae440
  // (left out: dead in C++)
  { const lifted::Result o = f821AE440(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0c1c
  goto loc_821B0C1C;
loc_821B0B70:
  // addi r10,r11,76
  r10 = uint64_t(int64_t(int64_t(r11) + 76));
  // addi r11,r11,192
  r11 = uint64_t(int64_t(int64_t(r11) + 192));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae4c0
  // (left out: dead in C++)
  r3 = f821AE4C0(base, r1, r13);
  // b 0x821b0be4
  goto loc_821B0BE4;
loc_821B0B90:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0a94
  goto loc_821B0A94;
loc_821B0BA4:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0c1c
  if (cr0.eq) goto loc_821B0C1C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,192
  r10 = uint64_t(int64_t(int64_t(r11) + 192));
  // addi r11,r11,76
  r11 = uint64_t(int64_t(int64_t(r11) + 76));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae440
  // (left out: dead in C++)
  { const lifted::Result o = f821AE440(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0BE4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b0bfc
  if (cr6.eq) goto loc_821B0BFC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0BFC:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,66
  r9 = uint64_t(int64_t(66));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B0C18:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B0C1C:
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

// sub_821B0C30: lifted from generated/ by lift.py.
lifted::Result f821B0C30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0C30 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0c98
  if (cr6.eq) goto loc_821B0C98;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0C64:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,16
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 16, so);
  // beq cr6,0x821b0c7c
  if (cr6.eq) goto loc_821B0C7C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0C7C:
  // li r9,69
  r9 = uint64_t(int64_t(69));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b0e10
  goto loc_821B0E10;
loc_821B0C98:
  // bl 0x821a9b58
  // (left out: dead in C++)
  r3 = f821A9B58(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0cec
  if (cr0.eq) goto loc_821B0CEC;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,16
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 16, so);
  // beq cr6,0x821b0cc4
  if (cr6.eq) goto loc_821B0CC4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0CC4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,70
  r9 = uint64_t(int64_t(70));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821B0CE0:
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0e14
  goto loc_821B0E14;
loc_821B0CEC:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0d94
  if (cr0.eq) goto loc_821B0D94;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b0d80
  if (!cr6.eq) goto loc_821B0D80;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0d60
  if (cr6.lt) goto loc_821B0D60;
  // beq cr6,0x821b0d40
  if (cr6.eq) goto loc_821B0D40;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b0e14
  if (!cr6.lt) goto loc_821B0E14;
  // addi r10,r11,316
  r10 = uint64_t(int64_t(int64_t(r11) + 316));
  // b 0x821b0d44
  goto loc_821B0D44;
loc_821B0D40:
  // addi r10,r11,200
  r10 = uint64_t(int64_t(int64_t(r11) + 200));
loc_821B0D44:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,84
  r11 = uint64_t(int64_t(int64_t(r11) + 84));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae540
  // (left out: dead in C++)
  { const lifted::Result o = f821AE540(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0ce0
  goto loc_821B0CE0;
loc_821B0D60:
  // addi r10,r11,84
  r10 = uint64_t(int64_t(int64_t(r11) + 84));
  // addi r11,r11,200
  r11 = uint64_t(int64_t(int64_t(r11) + 200));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae5c0
  // (left out: dead in C++)
  r3 = f821AE5C0(base, r1, r13);
  // b 0x821b0ddc
  goto loc_821B0DDC;
loc_821B0D80:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0c64
  goto loc_821B0C64;
loc_821B0D94:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0e14
  if (cr0.eq) goto loc_821B0E14;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,200
  r10 = uint64_t(int64_t(int64_t(r11) + 200));
  // addi r11,r11,84
  r11 = uint64_t(int64_t(int64_t(r11) + 84));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae540
  // (left out: dead in C++)
  { const lifted::Result o = f821AE540(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821aef48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEF48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0DDC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b0df4
  if (cr6.eq) goto loc_821B0DF4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0DF4:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,71
  r9 = uint64_t(int64_t(71));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B0E10:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B0E14:
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

// sub_821B0E28: lifted from generated/ by lift.py.
lifted::Result f821B0E28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0E28 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b0e90
  if (cr6.eq) goto loc_821B0E90;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0E5C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821b0e74
  if (cr6.eq) goto loc_821B0E74;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0E74:
  // li r9,74
  r9 = uint64_t(int64_t(74));
loc_821B0E78:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b0fb0
  goto loc_821B0FB0;
loc_821B0E90:
  // bl 0x821a9c98
  // (left out: dead in C++)
  r3 = f821A9C98(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0ec4
  if (cr0.eq) goto loc_821B0EC4;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821b0ebc
  if (cr6.eq) goto loc_821B0EBC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0EBC:
  // li r9,75
  r9 = uint64_t(int64_t(75));
  // b 0x821b0e78
  goto loc_821B0E78;
loc_821B0EC4:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0f4c
  if (cr0.eq) goto loc_821B0F4C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821b0f38
  if (!cr6.eq) goto loc_821B0F38;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b0f28
  if (cr6.lt) goto loc_821B0F28;
  // beq cr6,0x821b0f18
  if (cr6.eq) goto loc_821B0F18;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b0fb4
  if (!cr6.lt) goto loc_821B0FB4;
  // lwz r10,324(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 324);
  // b 0x821b0f1c
  goto loc_821B0F1C;
loc_821B0F18:
  // lwz r10,208(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 208);
loc_821B0F1C:
  // stw r10,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r10));
  // bl 0x821ae748
  // (left out: dead in C++)
  { const lifted::Result o = f821AE748(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b0fb4
  goto loc_821B0FB4;
loc_821B0F28:
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // stw r10,208(r11)
  REX_STORE_U32(uint32_t(r11) + 208, uint32_t(r10));
  // bl 0x821ae7a8
  // (left out: dead in C++)
  r3 = f821AE7A8(base, r1, r13);
  // b 0x821b0f7c
  goto loc_821B0F7C;
loc_821B0F38:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0e5c
  goto loc_821B0E5C;
loc_821B0F4C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b0fb4
  if (cr0.eq) goto loc_821B0FB4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,208(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 208);
  // stw r10,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r10));
  // bl 0x821ae748
  // (left out: dead in C++)
  { const lifted::Result o = f821AE748(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B0F7C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b0f94
  if (cr6.eq) goto loc_821B0F94;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B0F94:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,76
  r9 = uint64_t(int64_t(76));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B0FB0:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B0FB4:
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

// sub_821B0FC8: lifted from generated/ by lift.py.
lifted::Result f821B0FC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B0FC8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1030
  if (cr6.eq) goto loc_821B1030;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B0FFC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821b1014
  if (cr6.eq) goto loc_821B1014;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B1014:
  // li r9,79
  r9 = uint64_t(int64_t(79));
loc_821B1018:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b1150
  goto loc_821B1150;
loc_821B1030:
  // bl 0x821a9de0
  // (left out: dead in C++)
  r3 = f821A9DE0(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b1064
  if (cr0.eq) goto loc_821B1064;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821b105c
  if (cr6.eq) goto loc_821B105C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B105C:
  // li r9,80
  r9 = uint64_t(int64_t(80));
  // b 0x821b1018
  goto loc_821B1018;
loc_821B1064:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b10ec
  if (cr0.eq) goto loc_821B10EC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821b10d8
  if (!cr6.eq) goto loc_821B10D8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b10c8
  if (cr6.lt) goto loc_821B10C8;
  // beq cr6,0x821b10b8
  if (cr6.eq) goto loc_821B10B8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b1154
  if (!cr6.lt) goto loc_821B1154;
  // lwz r10,344(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 344);
  // b 0x821b10bc
  goto loc_821B10BC;
loc_821B10B8:
  // lwz r10,228(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 228);
loc_821B10BC:
  // stw r10,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r10));
  // bl 0x821ae808
  // (left out: dead in C++)
  { const lifted::Result o = f821AE808(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b1154
  goto loc_821B1154;
loc_821B10C8:
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // stw r10,228(r11)
  REX_STORE_U32(uint32_t(r11) + 228, uint32_t(r10));
  // bl 0x821ae868
  // (left out: dead in C++)
  r3 = f821AE868(base, r1, r13);
  // b 0x821b111c
  goto loc_821B111C;
loc_821B10D8:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b0ffc
  goto loc_821B0FFC;
loc_821B10EC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b1154
  if (cr0.eq) goto loc_821B1154;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,228(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 228);
  // stw r10,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r10));
  // bl 0x821ae808
  // (left out: dead in C++)
  { const lifted::Result o = f821AE808(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B111C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b1134
  if (cr6.eq) goto loc_821B1134;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B1134:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,81
  r9 = uint64_t(int64_t(81));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B1150:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B1154:
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

// sub_821B1168: lifted from generated/ by lift.py.
lifted::Result f821B1168(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1168 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b11d0
  if (cr6.eq) goto loc_821B11D0;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B119C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821b11b4
  if (cr6.eq) goto loc_821B11B4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B11B4:
  // li r9,84
  r9 = uint64_t(int64_t(84));
loc_821B11B8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b12f0
  goto loc_821B12F0;
loc_821B11D0:
  // bl 0x821a9f28
  // (left out: dead in C++)
  r3 = f821A9F28(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b1204
  if (cr0.eq) goto loc_821B1204;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821b11fc
  if (cr6.eq) goto loc_821B11FC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B11FC:
  // li r9,85
  r9 = uint64_t(int64_t(85));
  // b 0x821b11b8
  goto loc_821B11B8;
loc_821B1204:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b128c
  if (cr0.eq) goto loc_821B128C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821b1278
  if (!cr6.eq) goto loc_821B1278;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b1268
  if (cr6.lt) goto loc_821B1268;
  // beq cr6,0x821b1258
  if (cr6.eq) goto loc_821B1258;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b12f4
  if (!cr6.lt) goto loc_821B12F4;
  // lwz r10,340(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 340);
  // b 0x821b125c
  goto loc_821B125C;
loc_821B1258:
  // lwz r10,224(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 224);
loc_821B125C:
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // bl 0x821ae8c8
  // (left out: dead in C++)
  { const lifted::Result o = f821AE8C8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b12f4
  goto loc_821B12F4;
loc_821B1268:
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // stw r10,224(r11)
  REX_STORE_U32(uint32_t(r11) + 224, uint32_t(r10));
  // bl 0x821ae928
  // (left out: dead in C++)
  r3 = f821AE928(base, r1, r13);
  // b 0x821b12bc
  goto loc_821B12BC;
loc_821B1278:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b119c
  goto loc_821B119C;
loc_821B128C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b12f4
  if (cr0.eq) goto loc_821B12F4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,224(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 224);
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // bl 0x821ae8c8
  // (left out: dead in C++)
  { const lifted::Result o = f821AE8C8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B12BC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b12d4
  if (cr6.eq) goto loc_821B12D4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B12D4:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,86
  r9 = uint64_t(int64_t(86));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B12F0:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B12F4:
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

// sub_821B1308: lifted from generated/ by lift.py.
lifted::Result f821B1308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1308 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1370
  if (cr6.eq) goto loc_821B1370;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B133C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,20
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 20, so);
  // beq cr6,0x821b1354
  if (cr6.eq) goto loc_821B1354;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B1354:
  // li r9,89
  r9 = uint64_t(int64_t(89));
loc_821B1358:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,20
  r8 = uint64_t(int64_t(20));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b14c0
  goto loc_821B14C0;
loc_821B1370:
  // bl 0x821aa070
  // (left out: dead in C++)
  r3 = f821AA070(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b13a4
  if (cr0.eq) goto loc_821B13A4;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,20
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 20, so);
  // beq cr6,0x821b139c
  if (cr6.eq) goto loc_821B139C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B139C:
  // li r9,90
  r9 = uint64_t(int64_t(90));
  // b 0x821b1358
  goto loc_821B1358;
loc_821B13A4:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b144c
  if (cr0.eq) goto loc_821B144C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x821b1438
  if (!cr6.eq) goto loc_821B1438;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b1418
  if (cr6.lt) goto loc_821B1418;
  // beq cr6,0x821b13f8
  if (cr6.eq) goto loc_821B13F8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b14c4
  if (!cr6.lt) goto loc_821B14C4;
  // addi r10,r11,332
  r10 = uint64_t(int64_t(int64_t(r11) + 332));
  // b 0x821b13fc
  goto loc_821B13FC;
loc_821B13F8:
  // addi r10,r11,216
  r10 = uint64_t(int64_t(int64_t(r11) + 216));
loc_821B13FC:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,100
  r11 = uint64_t(int64_t(int64_t(r11) + 100));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae988
  // (left out: dead in C++)
  { const lifted::Result o = f821AE988(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b14c4
  goto loc_821B14C4;
loc_821B1418:
  // addi r10,r11,100
  r10 = uint64_t(int64_t(int64_t(r11) + 100));
  // addi r11,r11,216
  r11 = uint64_t(int64_t(int64_t(r11) + 216));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821aea08
  // (left out: dead in C++)
  r3 = f821AEA08(base, r1, r13);
  // b 0x821b148c
  goto loc_821B148C;
loc_821B1438:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b133c
  goto loc_821B133C;
loc_821B144C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b14c4
  if (cr0.eq) goto loc_821B14C4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r10,r11,216
  r10 = uint64_t(int64_t(int64_t(r11) + 216));
  // addi r11,r11,100
  r11 = uint64_t(int64_t(int64_t(r11) + 100));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x821ae988
  // (left out: dead in C++)
  { const lifted::Result o = f821AE988(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821B148C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b14a4
  if (cr6.eq) goto loc_821B14A4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B14A4:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,91
  r9 = uint64_t(int64_t(91));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B14C0:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B14C4:
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

// sub_821B14D8: lifted from generated/ by lift.py.
lifted::Result f821B14D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B14D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1540
  if (cr6.eq) goto loc_821B1540;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B150C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821b1524
  if (cr6.eq) goto loc_821B1524;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B1524:
  // li r9,94
  r9 = uint64_t(int64_t(94));
loc_821B1528:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821b1660
  goto loc_821B1660;
loc_821B1540:
  // bl 0x821aa1b0
  // (left out: dead in C++)
  r3 = f821AA1B0(base);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b1574
  if (cr0.eq) goto loc_821B1574;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821b156c
  if (cr6.eq) goto loc_821B156C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B156C:
  // li r9,95
  r9 = uint64_t(int64_t(95));
  // b 0x821b1528
  goto loc_821B1528;
loc_821B1574:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b15fc
  if (cr0.eq) goto loc_821B15FC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821b15e8
  if (!cr6.eq) goto loc_821B15E8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821b15d8
  if (cr6.lt) goto loc_821B15D8;
  // beq cr6,0x821b15c8
  if (cr6.eq) goto loc_821B15C8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821b1664
  if (!cr6.lt) goto loc_821B1664;
  // lwz r10,328(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 328);
  // b 0x821b15cc
  goto loc_821B15CC;
loc_821B15C8:
  // lwz r10,212(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 212);
loc_821B15CC:
  // stw r10,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r10));
  // bl 0x821aea88
  // (left out: dead in C++)
  { const lifted::Result o = f821AEA88(base, r1, r13, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821b1664
  goto loc_821B1664;
loc_821B15D8:
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // stw r10,212(r11)
  REX_STORE_U32(uint32_t(r11) + 212, uint32_t(r10));
  // bl 0x821aeae8
  // (left out: dead in C++)
  r3 = f821AEAE8(base, r1, r13);
  // b 0x821b162c
  goto loc_821B162C;
loc_821B15E8:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b150c
  goto loc_821B150C;
loc_821B15FC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b1664
  if (cr0.eq) goto loc_821B1664;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,212(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 212);
  // stw r10,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r10));
  // bl 0x821aea88
  // (left out: dead in C++)
  { const lifted::Result o = f821AEA88(base, r1, r13, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B162C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b1644
  if (cr6.eq) goto loc_821B1644;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B1644:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r9,96
  r9 = uint64_t(int64_t(96));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821B1660:
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821B1664:
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

// sub_821B1678: lifted from generated/ by lift.py.
lifted::Result f821B1678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1678 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-72(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r25,r3
  r25 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b1944
  if (!cr6.eq) goto loc_821B1944;
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r28,56
  r4 = uint64_t(int64_t(int64_t(r28) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // lis r26,-32146
  r26 = uint64_t(int64_t(-2106720256));
  // blt cr6,0x821b171c
  if (cr6.lt) goto loc_821B171C;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // bge cr6,0x821b1798
  if (!cr6.lt) goto loc_821B1798;
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1758
  if (cr6.eq) goto loc_821B1758;
  // addi r30,r29,56
  r30 = uint64_t(int64_t(int64_t(r29) + 56));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821b174c
  goto loc_821B174C;
loc_821B171C:
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,584(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1758
  if (cr6.eq) goto loc_821B1758;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,56
  r4 = uint64_t(int64_t(56));
loc_821B174C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b1798
  goto loc_821B1798;
loc_821B1758:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B1798:
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x821b17c0
  if (!cr6.eq) goto loc_821B17C0;
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // beq cr6,0x821b17d0
  if (cr6.eq) goto loc_821B17D0;
loc_821B17C0:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b1800
  if (!cr6.eq) goto loc_821B1800;
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // bne cr6,0x821b1800
  if (!cr6.eq) goto loc_821B1800;
loc_821B17D0:
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821b183c
  goto loc_821B183C;
loc_821B1800:
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b1818
  if (!cr6.eq) goto loc_821B1818;
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // b 0x821b1824
  goto loc_821B1824;
loc_821B1818:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821b183c
  if (!cr6.eq) goto loc_821B183C;
  // li r4,66
  r4 = uint64_t(int64_t(66));
loc_821B1824:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B183C:
  // li r4,201
  r4 = uint64_t(int64_t(201));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,202
  r4 = uint64_t(int64_t(202));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x821b18b8
  if (cr6.lt) goto loc_821B18B8;
  // beq cr6,0x821b1890
  if (cr6.eq) goto loc_821B1890;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // bge cr6,0x821b18dc
  if (!cr6.lt) goto loc_821B18DC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,202
  r4 = uint64_t(int64_t(202));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821addb8
  // (left out: dead in C++)
  { const lifted::Result o = f821ADDB8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ade70
  // (left out: dead in C++)
  r3 = f821ADE70(base, r1, r13);
  // b 0x821b1904
  goto loc_821B1904;
loc_821B1890:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,202
  r4 = uint64_t(int64_t(202));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821adc48
  // (left out: dead in C++)
  { const lifted::Result o = f821ADC48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821add00
  // (left out: dead in C++)
  r3 = f821ADD00(base, r1, r13);
  // b 0x821b18e4
  goto loc_821B18E4;
loc_821B18B8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,201
  r4 = uint64_t(int64_t(201));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ad840
  // (left out: dead in C++)
  { const lifted::Result o = f821AD840(base, r1, r13, r7); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ada08
  // (left out: dead in C++)
  r3 = f821ADA08(base, r1, r13);
loc_821B18DC:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x821b18fc
  if (!cr6.eq) goto loc_821B18FC;
loc_821B18E4:
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b1918
  if (!cr6.eq) goto loc_821B1918;
  // li r6,912
  r6 = uint64_t(int64_t(912));
  // b 0x821b191c
  goto loc_821B191C;
loc_821B18FC:
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // bne cr6,0x821b1918
  if (!cr6.eq) goto loc_821B1918;
loc_821B1904:
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // li r6,913
  r6 = uint64_t(int64_t(913));
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b191c
  if (cr6.eq) goto loc_821B191C;
loc_821B1918:
  // addi r6,r29,889
  r6 = uint64_t(int64_t(int64_t(r29) + 889));
loc_821B191C:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r11));
loc_821B1944:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B1960: lifted from generated/ by lift.py.
lifted::Result f821B1960(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1960 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b1b24
  if (!cr6.eq) goto loc_821B1B24;
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r28,56
  r4 = uint64_t(int64_t(int64_t(r28) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r29,56
  r30 = uint64_t(int64_t(int64_t(r29) + 56));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x821b1a0c
  if (!cr6.eq) goto loc_821B1A0C;
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // beq cr6,0x821b1a1c
  if (cr6.eq) goto loc_821B1A1C;
loc_821B1A0C:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b1a4c
  if (!cr6.eq) goto loc_821B1A4C;
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x821b1a4c
  if (!cr6.eq) goto loc_821B1A4C;
loc_821B1A1C:
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821b1a88
  goto loc_821B1A88;
loc_821B1A4C:
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b1a64
  if (!cr6.eq) goto loc_821B1A64;
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // b 0x821b1a70
  goto loc_821B1A70;
loc_821B1A64:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821b1a88
  if (!cr6.eq) goto loc_821B1A88;
  // li r4,66
  r4 = uint64_t(int64_t(66));
loc_821B1A70:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B1A88:
  // li r4,211
  r4 = uint64_t(int64_t(211));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,212
  r4 = uint64_t(int64_t(212));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x821b1ad4
  if (cr6.lt) goto loc_821B1AD4;
  // bne cr6,0x821b1af8
  if (!cr6.eq) goto loc_821B1AF8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,212
  r4 = uint64_t(int64_t(212));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae098
  // (left out: dead in C++)
  { const lifted::Result o = f821AE098(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae118
  // (left out: dead in C++)
  r3 = f821AE118(base, r1, r13);
  // b 0x821b1af8
  goto loc_821B1AF8;
loc_821B1AD4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,211
  r4 = uint64_t(int64_t(211));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821adf98
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF98(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae018
  // (left out: dead in C++)
  r3 = f821AE018(base, r1, r13);
loc_821B1AF8:
  // addi r6,r29,892
  r6 = uint64_t(int64_t(int64_t(r29) + 892));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_821B1B24:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B1B40: lifted from generated/ by lift.py.
lifted::Result f821B1B40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1B40 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b1d80
  if (!cr6.eq) goto loc_821B1D80;
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r28,56
  r4 = uint64_t(int64_t(int64_t(r28) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r30,56
  r29 = uint64_t(int64_t(int64_t(r30) + 56));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821b1bec
  if (!cr6.eq) goto loc_821B1BEC;
  // cmpwi cr6,r28,3
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 3, so);
  // beq cr6,0x821b1bfc
  if (cr6.eq) goto loc_821B1BFC;
loc_821B1BEC:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b1c2c
  if (!cr6.eq) goto loc_821B1C2C;
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // bne cr6,0x821b1c2c
  if (!cr6.eq) goto loc_821B1C2C;
loc_821B1BFC:
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821b1c68
  goto loc_821B1C68;
loc_821B1C2C:
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b1c44
  if (!cr6.eq) goto loc_821B1C44;
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // b 0x821b1c50
  goto loc_821B1C50;
loc_821B1C44:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821b1c68
  if (!cr6.eq) goto loc_821B1C68;
  // li r4,66
  r4 = uint64_t(int64_t(66));
loc_821B1C50:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B1C68:
  // li r4,219
  r4 = uint64_t(int64_t(219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,220
  r4 = uint64_t(int64_t(220));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // blt cr6,0x821b1d10
  if (cr6.lt) goto loc_821B1D10;
  // beq cr6,0x821b1ce8
  if (cr6.eq) goto loc_821B1CE8;
  // cmplwi cr6,r30,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 3, so);
  // blt cr6,0x821b1cc0
  if (cr6.lt) goto loc_821B1CC0;
  // bne cr6,0x821b1d34
  if (!cr6.eq) goto loc_821B1D34;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,220
  r4 = uint64_t(int64_t(220));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae540
  // (left out: dead in C++)
  { const lifted::Result o = f821AE540(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae5c0
  // (left out: dead in C++)
  r3 = f821AE5C0(base, r1, r13);
  // b 0x821b1d34
  goto loc_821B1D34;
loc_821B1CC0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,219
  r4 = uint64_t(int64_t(219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae440
  // (left out: dead in C++)
  { const lifted::Result o = f821AE440(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae4c0
  // (left out: dead in C++)
  r3 = f821AE4C0(base, r1, r13);
  // b 0x821b1d34
  goto loc_821B1D34;
loc_821B1CE8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,219
  r4 = uint64_t(int64_t(219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae340
  // (left out: dead in C++)
  { const lifted::Result o = f821AE340(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae3c0
  // (left out: dead in C++)
  r3 = f821AE3C0(base, r1, r13);
  // b 0x821b1d34
  goto loc_821B1D34;
loc_821B1D10:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,219
  r4 = uint64_t(int64_t(219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae240
  // (left out: dead in C++)
  { const lifted::Result o = f821AE240(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae2c0
  // (left out: dead in C++)
  r3 = f821AE2C0(base, r1, r13);
loc_821B1D34:
  // addi r6,r30,894
  r6 = uint64_t(int64_t(int64_t(r30) + 894));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // bne cr6,0x821b1d74
  if (!cr6.eq) goto loc_821B1D74;
  // stw r10,740(r11)
  REX_STORE_U32(uint32_t(r11) + 740, uint32_t(r10));
  // b 0x821b1d7c
  goto loc_821B1D7C;
loc_821B1D74:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,740(r11)
  REX_STORE_U32(uint32_t(r11) + 740, uint32_t(r9));
loc_821B1D7C:
  // stw r10,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r10));
loc_821B1D80:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B1DA0: lifted from generated/ by lift.py.
lifted::Result f821B1DA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B1DA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-72(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r25,r3
  r25 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b20e4
  if (!cr6.eq) goto loc_821B20E4;
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r28,56
  r4 = uint64_t(int64_t(int64_t(r28) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // lis r26,-32146
  r26 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821b1e38
  if (!cr6.eq) goto loc_821B1E38;
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // lwz r11,596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b1e64
  if (cr6.eq) goto loc_821B1E64;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // b 0x821b1ebc
  goto loc_821B1EBC;
loc_821B1E38:
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // bne cr6,0x821b1ea8
  if (!cr6.eq) goto loc_821B1EA8;
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821b1e64
  if (!cr6.eq) goto loc_821B1E64;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,60
  r4 = uint64_t(int64_t(60));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,60
  r4 = uint64_t(int64_t(60));
  // b 0x821b1ebc
  goto loc_821B1EBC;
loc_821B1E64:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b1ec4
  goto loc_821B1EC4;
loc_821B1EA8:
  // addi r30,r29,56
  r30 = uint64_t(int64_t(int64_t(r29) + 56));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
loc_821B1EBC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B1EC4:
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,66
  r4 = uint64_t(int64_t(66));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x821b1eec
  if (!cr6.eq) goto loc_821B1EEC;
  // cmpwi cr6,r28,4
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 4, so);
  // beq cr6,0x821b1efc
  if (cr6.eq) goto loc_821B1EFC;
loc_821B1EEC:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b1f2c
  if (!cr6.eq) goto loc_821B1F2C;
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // bne cr6,0x821b1f2c
  if (!cr6.eq) goto loc_821B1F2C;
loc_821B1EFC:
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821b1f68
  goto loc_821B1F68;
loc_821B1F2C:
  // lwz r11,-31652(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b1f44
  if (!cr6.eq) goto loc_821B1F44;
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // b 0x821b1f50
  goto loc_821B1F50;
loc_821B1F44:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821b1f68
  if (!cr6.eq) goto loc_821B1F68;
  // li r4,66
  r4 = uint64_t(int64_t(66));
loc_821B1F50:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B1F68:
  // li r4,231
  r4 = uint64_t(int64_t(231));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,232
  r4 = uint64_t(int64_t(232));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,233
  r4 = uint64_t(int64_t(233));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x821b2058
  if (cr6.lt) goto loc_821B2058;
  // beq cr6,0x821b2030
  if (cr6.eq) goto loc_821B2030;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // blt cr6,0x821b2008
  if (cr6.lt) goto loc_821B2008;
  // beq cr6,0x821b1fe0
  if (cr6.eq) goto loc_821B1FE0;
  // cmplwi cr6,r29,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 5, so);
  // bge cr6,0x821b207c
  if (!cr6.lt) goto loc_821B207C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,234
  r4 = uint64_t(int64_t(234));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821aea88
  // (left out: dead in C++)
  { const lifted::Result o = f821AEA88(base, r1, r13, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821aeae8
  // (left out: dead in C++)
  r3 = f821AEAE8(base, r1, r13);
  // b 0x821b20a4
  goto loc_821B20A4;
loc_821B1FE0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,233
  r4 = uint64_t(int64_t(233));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae988
  // (left out: dead in C++)
  { const lifted::Result o = f821AE988(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821aea08
  // (left out: dead in C++)
  r3 = f821AEA08(base, r1, r13);
  // b 0x821b20b8
  goto loc_821B20B8;
loc_821B2008:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,232
  r4 = uint64_t(int64_t(232));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae8c8
  // (left out: dead in C++)
  { const lifted::Result o = f821AE8C8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae928
  // (left out: dead in C++)
  r3 = f821AE928(base, r1, r13);
  // b 0x821b20b8
  goto loc_821B20B8;
loc_821B2030:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,232
  r4 = uint64_t(int64_t(232));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae808
  // (left out: dead in C++)
  { const lifted::Result o = f821AE808(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae868
  // (left out: dead in C++)
  r3 = f821AE868(base, r1, r13);
  // b 0x821b20b8
  goto loc_821B20B8;
loc_821B2058:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,231
  r4 = uint64_t(int64_t(231));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae748
  // (left out: dead in C++)
  { const lifted::Result o = f821AE748(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae7a8
  // (left out: dead in C++)
  r3 = f821AE7A8(base, r1, r13);
loc_821B207C:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x821b209c
  if (!cr6.eq) goto loc_821B209C;
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // lwz r11,596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b20b8
  if (!cr6.eq) goto loc_821B20B8;
  // li r6,909
  r6 = uint64_t(int64_t(909));
  // b 0x821b20bc
  goto loc_821B20BC;
loc_821B209C:
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // bne cr6,0x821b20b8
  if (!cr6.eq) goto loc_821B20B8;
loc_821B20A4:
  // lwz r11,-31648(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31648);
  // li r6,910
  r6 = uint64_t(int64_t(910));
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821b20bc
  if (!cr6.eq) goto loc_821B20BC;
loc_821B20B8:
  // addi r6,r29,898
  r6 = uint64_t(int64_t(int64_t(r29) + 898));
loc_821B20BC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r11));
loc_821B20E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2100: lifted from generated/ by lift.py.
lifted::Result f821B2100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2100 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2394
  if (!cr6.eq) goto loc_821B2394;
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // lwz r27,4(r11)
  r27 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r27,17
  r4 = uint64_t(int64_t(int64_t(r27) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r30,17
  r29 = uint64_t(int64_t(int64_t(r30) + 17));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821b21ac
  if (!cr6.eq) goto loc_821B21AC;
  // cmpwi cr6,r27,3
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 3, so);
  // beq cr6,0x821b21bc
  if (cr6.eq) goto loc_821B21BC;
loc_821B21AC:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x821b21ec
  if (!cr6.eq) goto loc_821B21EC;
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // bne cr6,0x821b21ec
  if (!cr6.eq) goto loc_821B21EC;
loc_821B21BC:
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821b2228
  goto loc_821B2228;
loc_821B21EC:
  // lwz r11,-31652(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b2204
  if (!cr6.eq) goto loc_821B2204;
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // b 0x821b2210
  goto loc_821B2210;
loc_821B2204:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821b2228
  if (!cr6.eq) goto loc_821B2228;
  // li r4,27
  r4 = uint64_t(int64_t(27));
loc_821B2210:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2228:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x821b2270
  if (!cr6.eq) goto loc_821B2270;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x821b22f0
  if (cr6.eq) goto loc_821B22F0;
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b22b8
  goto loc_821B22B8;
loc_821B2270:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821b22ac
  if (!cr6.eq) goto loc_821B22AC;
  // li r4,248
  r4 = uint64_t(int64_t(248));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B22AC:
  // addi r4,r27,102
  r4 = uint64_t(int64_t(int64_t(r27) + 102));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B22B8:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x821b22f0
  if (cr6.eq) goto loc_821B22F0;
  // addi r29,r30,102
  r29 = uint64_t(int64_t(int64_t(r30) + 102));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2318
  goto loc_821B2318;
loc_821B22F0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2318:
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r6,r30,903
  r6 = uint64_t(int64_t(int64_t(r30) + 903));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x821b238c
  if (cr6.eq) goto loc_821B238C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r10,r30,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // addi r8,r11,348
  r8 = uint64_t(int64_t(int64_t(r11) + 348));
  // addi r9,r11,408
  r9 = uint64_t(int64_t(int64_t(r11) + 408));
  // addi r11,r11,468
  r11 = uint64_t(int64_t(int64_t(r11) + 468));
  // stw r8,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r8));
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lwz r3,-4(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + -4);
  // bl 0x821aeb48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEB48(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B238C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_821B2394:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B23B0: lifted from generated/ by lift.py.
lifted::Result f821B23B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B23B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2450
  if (!cr6.eq) goto loc_821B2450;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,203
  r4 = uint64_t(int64_t(203));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac338
  // (left out: dead in C++)
  { const lifted::Result o = f821AC338(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2450:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B2480: lifted from generated/ by lift.py.
lifted::Result f821B2480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2480 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b25ac
  if (!cr6.eq) goto loc_821B25AC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpwi cr6,r31,6
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 6, so);
  // bge cr6,0x821b2508
  if (!cr6.lt) goto loc_821B2508;
  // lwz r10,-31648(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,600(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 600);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r31,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r10), so);
  // ble cr6,0x821b2508
  if (!cr6.gt) goto loc_821B2508;
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // li r31,6
  r31 = uint64_t(int64_t(6));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bgt cr6,0x821b24fc
  if (cr6.gt) goto loc_821B24FC;
  // mr r31,r10
  r31 = r10;
loc_821B24FC:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
loc_821B2508:
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // beq cr6,0x821b2520
  if (cr6.eq) goto loc_821B2520;
  // addi r4,r9,134
  r4 = uint64_t(int64_t(int64_t(r9) + 134));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2538
  goto loc_821B2538;
loc_821B2520:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B2538:
  // cmpwi cr6,r31,7
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 7, so);
  // beq cr6,0x821b2570
  if (cr6.eq) goto loc_821B2570;
  // addi r31,r31,134
  r31 = uint64_t(int64_t(int64_t(r31) + 134));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b25a4
  goto loc_821B25A4;
loc_821B2570:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,133
  r4 = uint64_t(int64_t(133));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,133
  r4 = uint64_t(int64_t(133));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B25A4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821B25AC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B25C8: lifted from generated/ by lift.py.
lifted::Result f821B25C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B25C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b263c
  if (!cr6.eq) goto loc_821B263C;
  // bl 0x821ad840
  // (left out: dead in C++)
  { const lifted::Result o = f821AD840(base, r1, r13, r7); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x821b2634
  if (!cr6.eq) goto loc_821B2634;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B2634:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B263C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2658: lifted from generated/ by lift.py.
lifted::Result f821B2658(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2658 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b26f8
  if (!cr6.eq) goto loc_821B26F8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,205
  r4 = uint64_t(int64_t(205));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac408
  // (left out: dead in C++)
  { const lifted::Result o = f821AC408(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B26F8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B2728: lifted from generated/ by lift.py.
lifted::Result f821B2728(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2728 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2858
  if (!cr6.eq) goto loc_821B2858;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,688(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 688);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // bge cr6,0x821b27b8
  if (!cr6.lt) goto loc_821B27B8;
  // cmpwi cr6,r31,1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 1, so);
  // bne cr6,0x821b27b8
  if (!cr6.eq) goto loc_821B27B8;
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // rotlwi r9,r31,0
  r9 = __builtin_rotateleft32(uint32_t(r31), 0);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // ble cr6,0x821b27b0
  if (!cr6.gt) goto loc_821B27B0;
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // b 0x821b27b8
  goto loc_821B27B8;
loc_821B27B0:
  // mr r31,r29
  r31 = r29;
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
loc_821B27B8:
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b27d0
  if (cr6.eq) goto loc_821B27D0;
  // addi r4,r10,149
  r4 = uint64_t(int64_t(int64_t(r10) + 149));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b27e4
  goto loc_821B27E4;
loc_821B27D0:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r29,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r29));
loc_821B27E4:
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // beq cr6,0x821b281c
  if (cr6.eq) goto loc_821B281C;
  // addi r31,r31,149
  r31 = uint64_t(int64_t(int64_t(r31) + 149));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2850
  goto loc_821B2850;
loc_821B281C:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,148
  r4 = uint64_t(int64_t(148));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,148
  r4 = uint64_t(int64_t(148));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2850:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821B2858:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2870: lifted from generated/ by lift.py.
lifted::Result f821B2870(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2870 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b28e4
  if (!cr6.eq) goto loc_821B28E4;
  // bl 0x821adc48
  // (left out: dead in C++)
  { const lifted::Result o = f821ADC48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b28dc
  if (!cr6.eq) goto loc_821B28DC;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B28DC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B28E4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2900: lifted from generated/ by lift.py.
lifted::Result f821B2900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2900 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b29a0
  if (!cr6.eq) goto loc_821B29A0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,207
  r4 = uint64_t(int64_t(207));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac4d8
  // (left out: dead in C++)
  { const lifted::Result o = f821AC4D8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B29A0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B29D0: lifted from generated/ by lift.py.
lifted::Result f821B29D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B29D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2a44
  if (!cr6.eq) goto loc_821B2A44;
  // bl 0x821addb8
  // (left out: dead in C++)
  { const lifted::Result o = f821ADDB8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b2a3c
  if (!cr6.eq) goto loc_821B2A3C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B2A3C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2A44:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2A60: lifted from generated/ by lift.py.
lifted::Result f821B2A60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2A60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2b00
  if (!cr6.eq) goto loc_821B2B00;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,213
  r4 = uint64_t(int64_t(213));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac5a8
  // (left out: dead in C++)
  { const lifted::Result o = f821AC5A8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2B00:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B2B30: lifted from generated/ by lift.py.
lifted::Result f821B2B30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2B30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2c5c
  if (!cr6.eq) goto loc_821B2C5C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b2b94
  if (cr6.eq) goto loc_821B2B94;
  // addi r4,r10,159
  r4 = uint64_t(int64_t(int64_t(r10) + 159));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2bac
  goto loc_821B2BAC;
loc_821B2B94:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B2BAC:
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // beq cr6,0x821b2be4
  if (cr6.eq) goto loc_821B2BE4;
  // addi r30,r28,159
  r30 = uint64_t(int64_t(int64_t(r28) + 159));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2c18
  goto loc_821B2C18;
loc_821B2BE4:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2C18:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b2c54
  if (cr6.eq) goto loc_821B2C54;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b2c38
  if (!cr6.eq) goto loc_821B2C38;
  // li r4,59
  r4 = uint64_t(int64_t(59));
  // b 0x821b2c44
  goto loc_821B2C44;
loc_821B2C38:
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x821b2c54
  if (!cr6.eq) goto loc_821B2C54;
  // li r4,60
  r4 = uint64_t(int64_t(60));
loc_821B2C44:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82180108, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2C54:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821B2C5C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2C78: lifted from generated/ by lift.py.
lifted::Result f821B2C78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2C78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2cec
  if (!cr6.eq) goto loc_821B2CEC;
  // bl 0x821adf98
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF98(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b2ce4
  if (!cr6.eq) goto loc_821B2CE4;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B2CE4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2CEC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2D08: lifted from generated/ by lift.py.
lifted::Result f821B2D08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2D08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2da8
  if (!cr6.eq) goto loc_821B2DA8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,215
  r4 = uint64_t(int64_t(215));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac6a8
  // (left out: dead in C++)
  { const lifted::Result o = f821AC6A8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2DA8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B2DD8: lifted from generated/ by lift.py.
lifted::Result f821B2DD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2DD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2f04
  if (!cr6.eq) goto loc_821B2F04;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b2e3c
  if (cr6.eq) goto loc_821B2E3C;
  // addi r4,r10,159
  r4 = uint64_t(int64_t(int64_t(r10) + 159));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2e54
  goto loc_821B2E54;
loc_821B2E3C:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B2E54:
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // beq cr6,0x821b2e8c
  if (cr6.eq) goto loc_821B2E8C;
  // addi r30,r28,159
  r30 = uint64_t(int64_t(int64_t(r28) + 159));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b2ec0
  goto loc_821B2EC0;
loc_821B2E8C:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2EC0:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b2efc
  if (cr6.eq) goto loc_821B2EFC;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821b2ee0
  if (!cr6.eq) goto loc_821B2EE0;
  // li r4,61
  r4 = uint64_t(int64_t(61));
  // b 0x821b2eec
  goto loc_821B2EEC;
loc_821B2EE0:
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x821b2efc
  if (!cr6.eq) goto loc_821B2EFC;
  // li r4,62
  r4 = uint64_t(int64_t(62));
loc_821B2EEC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82180108, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B2EFC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821B2F04:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2F20: lifted from generated/ by lift.py.
lifted::Result f821B2F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2F20 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b2f94
  if (!cr6.eq) goto loc_821B2F94;
  // bl 0x821ae098
  // (left out: dead in C++)
  { const lifted::Result o = f821AE098(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b2f8c
  if (!cr6.eq) goto loc_821B2F8C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B2F8C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B2F94:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B2FB0: lifted from generated/ by lift.py.
lifted::Result f821B2FB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B2FB0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3050
  if (!cr6.eq) goto loc_821B3050;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,221
  r4 = uint64_t(int64_t(221));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac7a8
  // (left out: dead in C++)
  { const lifted::Result o = f821AC7A8(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3050:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3080: lifted from generated/ by lift.py.
lifted::Result f821B3080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3080 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b30f4
  if (!cr6.eq) goto loc_821B30F4;
  // bl 0x821ae240
  // (left out: dead in C++)
  { const lifted::Result o = f821AE240(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b30ec
  if (!cr6.eq) goto loc_821B30EC;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B30EC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B30F4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3110: lifted from generated/ by lift.py.
lifted::Result f821B3110(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3110 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b31b0
  if (!cr6.eq) goto loc_821B31B0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,223
  r4 = uint64_t(int64_t(223));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac878
  // (left out: dead in C++)
  { const lifted::Result o = f821AC878(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B31B0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B31E0: lifted from generated/ by lift.py.
lifted::Result f821B31E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B31E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3254
  if (!cr6.eq) goto loc_821B3254;
  // bl 0x821ae340
  // (left out: dead in C++)
  { const lifted::Result o = f821AE340(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b324c
  if (!cr6.eq) goto loc_821B324C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B324C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3254:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3270: lifted from generated/ by lift.py.
lifted::Result f821B3270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3270 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3310
  if (!cr6.eq) goto loc_821B3310;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,225
  r4 = uint64_t(int64_t(225));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ac948
  // (left out: dead in C++)
  { const lifted::Result o = f821AC948(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3310:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3340: lifted from generated/ by lift.py.
lifted::Result f821B3340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3340 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b33b4
  if (!cr6.eq) goto loc_821B33B4;
  // bl 0x821ae440
  // (left out: dead in C++)
  { const lifted::Result o = f821AE440(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b33ac
  if (!cr6.eq) goto loc_821B33AC;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B33AC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B33B4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B33D0: lifted from generated/ by lift.py.
lifted::Result f821B33D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B33D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3470
  if (!cr6.eq) goto loc_821B3470;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,227
  r4 = uint64_t(int64_t(227));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821aca18
  // (left out: dead in C++)
  { const lifted::Result o = f821ACA18(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3470:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B34A0: lifted from generated/ by lift.py.
lifted::Result f821B34A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B34A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b35c4
  if (!cr6.eq) goto loc_821B35C4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b3504
  if (cr6.eq) goto loc_821B3504;
  // addi r4,r10,159
  r4 = uint64_t(int64_t(int64_t(r10) + 159));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b351c
  goto loc_821B351C;
loc_821B3504:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B351C:
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // beq cr6,0x821b3554
  if (cr6.eq) goto loc_821B3554;
  // addi r30,r28,159
  r30 = uint64_t(int64_t(int64_t(r28) + 159));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3588
  goto loc_821B3588;
loc_821B3554:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B3588:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b35bc
  if (cr6.eq) goto loc_821B35BC;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x821b35a8
  if (cr6.eq) goto loc_821B35A8;
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x821b35bc
  if (!cr6.eq) goto loc_821B35BC;
loc_821B35A8:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,63
  r4 = uint64_t(int64_t(63));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82180108, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B35BC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821B35C4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B35E0: lifted from generated/ by lift.py.
lifted::Result f821B35E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B35E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3654
  if (!cr6.eq) goto loc_821B3654;
  // bl 0x821ae540
  // (left out: dead in C++)
  { const lifted::Result o = f821AE540(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b364c
  if (!cr6.eq) goto loc_821B364C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B364C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3654:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3670: lifted from generated/ by lift.py.
lifted::Result f821B3670(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3670 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3710
  if (!cr6.eq) goto loc_821B3710;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,235
  r4 = uint64_t(int64_t(235));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acb18
  // (left out: dead in C++)
  { const lifted::Result o = f821ACB18(base, r1, r13, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3710:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3740: lifted from generated/ by lift.py.
lifted::Result f821B3740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3740 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b37b4
  if (!cr6.eq) goto loc_821B37B4;
  // bl 0x821ae748
  // (left out: dead in C++)
  { const lifted::Result o = f821AE748(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b37ac
  if (!cr6.eq) goto loc_821B37AC;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B37AC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B37B4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B37D0: lifted from generated/ by lift.py.
lifted::Result f821B37D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B37D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3870
  if (!cr6.eq) goto loc_821B3870;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,237
  r4 = uint64_t(int64_t(237));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acbe8
  // (left out: dead in C++)
  { const lifted::Result o = f821ACBE8(base, r1, r13, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3870:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B38A0: lifted from generated/ by lift.py.
lifted::Result f821B38A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B38A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3990
  if (!cr6.eq) goto loc_821B3990;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b3904
  if (cr6.eq) goto loc_821B3904;
  // addi r4,r10,186
  r4 = uint64_t(int64_t(int64_t(r10) + 186));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b391c
  goto loc_821B391C;
loc_821B3904:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B391C:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // beq cr6,0x821b3954
  if (cr6.eq) goto loc_821B3954;
  // addi r31,r29,186
  r31 = uint64_t(int64_t(int64_t(r29) + 186));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3988
  goto loc_821B3988;
loc_821B3954:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,185
  r4 = uint64_t(int64_t(185));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,185
  r4 = uint64_t(int64_t(185));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B3988:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821B3990:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B39A8: lifted from generated/ by lift.py.
lifted::Result f821B39A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B39A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3a1c
  if (!cr6.eq) goto loc_821B3A1C;
  // bl 0x821ae808
  // (left out: dead in C++)
  { const lifted::Result o = f821AE808(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b3a14
  if (!cr6.eq) goto loc_821B3A14;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B3A14:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3A1C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3A38: lifted from generated/ by lift.py.
lifted::Result f821B3A38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3A38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3ad8
  if (!cr6.eq) goto loc_821B3AD8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,239
  r4 = uint64_t(int64_t(239));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821accb8
  // (left out: dead in C++)
  { const lifted::Result o = f821ACCB8(base, r1, r13, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3AD8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3B08: lifted from generated/ by lift.py.
lifted::Result f821B3B08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3B08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3b7c
  if (!cr6.eq) goto loc_821B3B7C;
  // bl 0x821ae8c8
  // (left out: dead in C++)
  { const lifted::Result o = f821AE8C8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b3b74
  if (!cr6.eq) goto loc_821B3B74;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B3B74:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3B7C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3B98: lifted from generated/ by lift.py.
lifted::Result f821B3B98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3B98 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3c38
  if (!cr6.eq) goto loc_821B3C38;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,241
  r4 = uint64_t(int64_t(241));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acd88
  // (left out: dead in C++)
  { const lifted::Result o = f821ACD88(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3C38:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3C68: lifted from generated/ by lift.py.
lifted::Result f821B3C68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3C68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3d58
  if (!cr6.eq) goto loc_821B3D58;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b3ccc
  if (cr6.eq) goto loc_821B3CCC;
  // addi r4,r10,174
  r4 = uint64_t(int64_t(int64_t(r10) + 174));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3ce4
  goto loc_821B3CE4;
loc_821B3CCC:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B3CE4:
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // beq cr6,0x821b3d1c
  if (cr6.eq) goto loc_821B3D1C;
  // addi r31,r29,174
  r31 = uint64_t(int64_t(int64_t(r29) + 174));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3d50
  goto loc_821B3D50;
loc_821B3D1C:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,173
  r4 = uint64_t(int64_t(173));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,173
  r4 = uint64_t(int64_t(173));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B3D50:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821B3D58:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3D70: lifted from generated/ by lift.py.
lifted::Result f821B3D70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3D70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3de4
  if (!cr6.eq) goto loc_821B3DE4;
  // bl 0x821ae988
  // (left out: dead in C++)
  { const lifted::Result o = f821AE988(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821b3ddc
  if (!cr6.eq) goto loc_821B3DDC;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B3DDC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3DE4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3E00: lifted from generated/ by lift.py.
lifted::Result f821B3E00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3E00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3ea0
  if (!cr6.eq) goto loc_821B3EA0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r9,20(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r4,r10,56
  r4 = uint64_t(int64_t(int64_t(r10) + 56));
  // addi r8,r9,1
  r8 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,243
  r4 = uint64_t(int64_t(243));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ace58
  // (left out: dead in C++)
  { const lifted::Result o = f821ACE58(base, r1, r13, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B3EA0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821B3ED0: lifted from generated/ by lift.py.
lifted::Result f821B3ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3ED0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b3fc0
  if (!cr6.eq) goto loc_821B3FC0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b3f34
  if (cr6.eq) goto loc_821B3F34;
  // addi r4,r10,194
  r4 = uint64_t(int64_t(int64_t(r10) + 194));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3f4c
  goto loc_821B3F4C;
loc_821B3F34:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821B3F4C:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // beq cr6,0x821b3f84
  if (cr6.eq) goto loc_821B3F84;
  // addi r31,r29,194
  r31 = uint64_t(int64_t(int64_t(r29) + 194));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b3fb8
  goto loc_821B3FB8;
loc_821B3F84:
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,193
  r4 = uint64_t(int64_t(193));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,193
  r4 = uint64_t(int64_t(193));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B3FB8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821B3FC0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B3FD8: lifted from generated/ by lift.py.
lifted::Result f821B3FD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B3FD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b404c
  if (!cr6.eq) goto loc_821B404C;
  // bl 0x821aea88
  // (left out: dead in C++)
  { const lifted::Result o = f821AEA88(base, r1, r13, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821b4044
  if (!cr6.eq) goto loc_821B4044;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,736(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 736);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad2e0
  // (left out: dead in C++)
  r3 = f821AD2E0(base, r1, r13, r3);
loc_821B4044:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B404C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4068: lifted from generated/ by lift.py.
lifted::Result f821B4068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4068 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r29
  r3 = r29;
  // lwz r11,564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 564);
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // lwz r11,-31652(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // lwz r11,-31652(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // lwz r27,0(r11)
  r27 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r27,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r29
  r3 = r29;
  // stfs f31,16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 564);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x821b41a4
  if (!cr6.gt) goto loc_821B41A4;
  // li r26,5
  r26 = uint64_t(int64_t(5));
loc_821B4178:
  // addi r6,r26,139
  r6 = uint64_t(int64_t(int64_t(r26) + 139));
  // mr r5,r26
  r5 = r26;
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r10,r26,-5
  r10 = uint64_t(int64_t(int64_t(r26) + -5));
  // lwz r11,564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 564);
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // blt cr6,0x821b4178
  if (cr6.lt) goto loc_821B4178;
loc_821B41A4:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // stw r27,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r27));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // stw r27,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r27));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x821b4214
  if (!cr6.eq) goto loc_821B4214;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821b4254
  if (!cr6.eq) goto loc_821B4254;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4254
  if (!cr6.eq) goto loc_821B4254;
  // li r6,911
  r6 = uint64_t(int64_t(911));
  // b 0x821b4260
  goto loc_821B4260;
loc_821B4214:
  // cmpwi cr6,r27,2
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 2, so);
  // bne cr6,0x821b4234
  if (!cr6.eq) goto loc_821B4234;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4254
  if (!cr6.eq) goto loc_821B4254;
  // li r6,908
  r6 = uint64_t(int64_t(908));
  // b 0x821b4260
  goto loc_821B4260;
loc_821B4234:
  // cmpwi cr6,r27,1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 1, so);
  // bne cr6,0x821b4254
  if (!cr6.eq) goto loc_821B4254;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4254
  if (!cr6.eq) goto loc_821B4254;
  // li r6,907
  r6 = uint64_t(int64_t(907));
  // b 0x821b4260
  goto loc_821B4260;
loc_821B4254:
  // lwz r11,-31652(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r6,r11,866
  r6 = uint64_t(int64_t(int64_t(r11) + 866));
loc_821B4260:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r31,740(r11)
  REX_STORE_U32(uint32_t(r11) + 740, uint32_t(r31));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4298: lifted from generated/ by lift.py.
lifted::Result f821B4298(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4298 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r26,3
  r26 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // stw r26,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r26));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r28,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // lwz r3,-31648(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // li r5,44
  r5 = uint64_t(int64_t(44));
  // addi r4,r3,116
  r4 = uint64_t(int64_t(int64_t(r3) + 116));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r27,5
  r27 = uint64_t(int64_t(5));
loc_821B439C:
  // addi r6,r27,144
  r6 = uint64_t(int64_t(int64_t(r27) + 144));
  // mr r5,r27
  r5 = r27;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // bne 0x821b439c
  if (!cr0.eq) goto loc_821B439C;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // bne cr6,0x821b43f0
  if (!cr6.eq) goto loc_821B43F0;
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b440c
  if (!cr6.eq) goto loc_821B440C;
  // li r6,912
  r6 = uint64_t(int64_t(912));
  // b 0x821b4410
  goto loc_821B4410;
loc_821B43F0:
  // cmpwi cr6,r28,2
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 2, so);
  // bne cr6,0x821b440c
  if (!cr6.eq) goto loc_821B440C;
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // li r6,913
  r6 = uint64_t(int64_t(913));
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b4410
  if (cr6.eq) goto loc_821B4410;
loc_821B440C:
  // addi r6,r28,889
  r6 = uint64_t(int64_t(int64_t(r28) + 889));
loc_821B4410:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // addi r10,r11,604
  r10 = uint64_t(int64_t(int64_t(r11) + 604));
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,232(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 232);
  f0 = double(temp.f32);
  // stfs f0,128(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // addi r10,r11,616
  r10 = uint64_t(int64_t(int64_t(r11) + 616));
  // lfs f0,236(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 236);
  f0 = double(temp.f32);
  // stfs f0,132(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // lfs f0,240(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 240);
  f0 = double(temp.f32);
  // stfs f0,136(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // addi r10,r11,628
  r10 = uint64_t(int64_t(int64_t(r11) + 628));
  // lfs f0,244(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 244);
  f0 = double(temp.f32);
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // lfs f0,248(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 248);
  f0 = double(temp.f32);
  // stfs f0,144(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // addi r10,r11,640
  r10 = uint64_t(int64_t(int64_t(r11) + 640));
  // lfs f0,252(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 252);
  f0 = double(temp.f32);
  // stfs f0,148(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 148, temp.u32);
  // lfs f0,256(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 256);
  f0 = double(temp.f32);
  // lfs f12,2608(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f12 = double(temp.f32);
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // addi r10,r11,652
  r10 = uint64_t(int64_t(int64_t(r11) + 652));
  // lfs f11,-23248(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -23248);
  f11 = double(temp.f32);
  // lfs f10,7140(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 7140);
  f10 = double(temp.f32);
  // stfs f0,152(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 152, temp.u32);
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // addi r10,r11,664
  r10 = uint64_t(int64_t(int64_t(r11) + 664));
  // addi r11,r11,676
  r11 = uint64_t(int64_t(int64_t(r11) + 676));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f8,9844(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 9844);
  f8 = double(temp.f32);
  // lfs f9,11436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11436);
  f9 = double(temp.f32);
loc_821B44CC:
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // cmplwi cr6,r31,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 6, so);
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // lfsx f13,r10,r9
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  f13 = double(temp.f32);
  // blt cr6,0x821b44f8
  if (cr6.lt) goto loc_821B44F8;
  // bne cr6,0x821b4520
  if (!cr6.eq) goto loc_821B4520;
  // stfs f9,4(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f8,0(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // b 0x821b4520
  goto loc_821B4520;
loc_821B44F8:
  // fadds f0,f13,f12
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f12));
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fcmpu cr6,f0,f11
  cr6 = lifted::CompareFloat(f0, f11);
  // ble cr6,0x821b450c
  if (!cr6.gt) goto loc_821B450C;
  // stfs f11,4(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
loc_821B450C:
  // fsubs f0,f13,f12
  lifted::ScalarFloatMode();
  f0 = double(float(f13 - f12));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fcmpu cr6,f0,f10
  cr6 = lifted::CompareFloat(f0, f10);
  // bge cr6,0x821b4520
  if (!cr6.lt) goto loc_821B4520;
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
loc_821B4520:
  // lfs f0,4(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // cmpwi cr6,r31,7
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 7, so);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // blt cr6,0x821b44cc
  if (cr6.lt) goto loc_821B44CC;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // lfd f31,-64(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4548: lifted from generated/ by lift.py.
lifted::Result f821B4548(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4548 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stfs f31,16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r10,r11,160
  r10 = uint64_t(int64_t(int64_t(r11) + 160));
  // addi r9,r11,44
  r9 = uint64_t(int64_t(int64_t(r11) + 44));
  // mr r11,r10
  r11 = r10;
  // mr r10,r9
  r10 = r9;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r11,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // li r28,2
  r28 = uint64_t(int64_t(2));
loc_821B4674:
  // addi r6,r30,147
  r6 = uint64_t(int64_t(int64_t(r30) + 147));
  // mr r5,r30
  r5 = r30;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821b4674
  if (!cr0.eq) goto loc_821B4674;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r6,r29,892
  r6 = uint64_t(int64_t(int64_t(r29) + 892));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B46D8: lifted from generated/ by lift.py.
lifted::Result f821B46D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821B46D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // mr r3,r31
  r3 = r31;
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // addi r4,r11,176
  r4 = uint64_t(int64_t(int64_t(r11) + 176));
  // addi r3,r11,60
  r3 = uint64_t(int64_t(int64_t(r11) + 60));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // li r28,4
  r28 = uint64_t(int64_t(4));
loc_821B47E0:
  // addi r6,r30,149
  r6 = uint64_t(int64_t(int64_t(r30) + 149));
  // mr r5,r30
  r5 = r30;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821b47e0
  if (!cr0.eq) goto loc_821B47E0;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r6,r29,894
  r6 = uint64_t(int64_t(int64_t(r29) + 894));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4848: lifted from generated/ by lift.py.
lifted::Result f821B4848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4848 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-56(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // addi r4,r11,208
  r4 = uint64_t(int64_t(int64_t(r11) + 208));
  // addi r3,r11,92
  r3 = uint64_t(int64_t(int64_t(r11) + 92));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // li r27,5
  r27 = uint64_t(int64_t(5));
loc_821B4954:
  // addi r6,r28,153
  r6 = uint64_t(int64_t(int64_t(r28) + 153));
  // mr r5,r28
  r5 = r28;
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // bne 0x821b4954
  if (!cr0.eq) goto loc_821B4954;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x821b49a8
  if (!cr6.eq) goto loc_821B49A8;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b49c4
  if (!cr6.eq) goto loc_821B49C4;
  // li r6,909
  r6 = uint64_t(int64_t(909));
  // b 0x821b49c8
  goto loc_821B49C8;
loc_821B49A8:
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // bne cr6,0x821b49c4
  if (!cr6.eq) goto loc_821B49C4;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // li r6,910
  r6 = uint64_t(int64_t(910));
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821b49c8
  if (!cr6.eq) goto loc_821B49C8;
loc_821B49C4:
  // addi r6,r29,898
  r6 = uint64_t(int64_t(int64_t(r29) + 898));
loc_821B49C8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B49F8: lifted from generated/ by lift.py.
lifted::Result f821B49F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821B49F8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-56(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // mr r3,r31
  r3 = r31;
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r10,-31652(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,-31652(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31652);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mulli r11,r29,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r29 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // addi r4,r11,208
  r4 = uint64_t(int64_t(int64_t(r11) + 208));
  // addi r3,r11,92
  r3 = uint64_t(int64_t(int64_t(r11) + 92));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821acf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ACF28(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // li r27,4
  r27 = uint64_t(int64_t(4));
loc_821B4B00:
  // addi r6,r28,158
  r6 = uint64_t(int64_t(int64_t(r28) + 158));
  // mr r5,r28
  r5 = r28;
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // bne 0x821b4b00
  if (!cr0.eq) goto loc_821B4B00;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r6,r29,903
  r6 = uint64_t(int64_t(int64_t(r29) + 903));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C5F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x821b4b90
  if (cr6.eq) goto loc_821B4B90;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r11,348
  r8 = uint64_t(int64_t(int64_t(r11) + 348));
  // add r10,r10,r9
  r10 = r10 + r9;
  // addi r9,r11,408
  r9 = uint64_t(int64_t(int64_t(r11) + 408));
  // addi r11,r11,468
  r11 = uint64_t(int64_t(int64_t(r11) + 468));
  // stw r8,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r8));
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lwz r3,-4(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + -4);
  // bl 0x821aeb48
  // (left out: dead in C++)
  { const lifted::Result o = f821AEB48(base, r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821B4B90:
  // bl 0x821aec18
  // (left out: dead in C++)
  r3 = f821AEC18(base, r1, r13);
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4BA0: lifted from generated/ by lift.py.
lifted::Result f821B4BA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4BA0 (lifted)");
  uint64_t r3 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e58
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-304(r1)
  ea = -304 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r16,-32229
  r16 = uint64_t(int64_t(-2112159744));
  // lis r17,-32229
  r17 = uint64_t(int64_t(-2112159744));
  // addi r3,r16,-23200
  r3 = uint64_t(int64_t(int64_t(r16) + -23200));
  // lis r18,-32229
  r18 = uint64_t(int64_t(-2112159744));
  // lis r19,-32229
  r19 = uint64_t(int64_t(-2112159744));
  // lis r20,-32229
  r20 = uint64_t(int64_t(-2112159744));
  // lis r21,-32229
  r21 = uint64_t(int64_t(-2112159744));
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // addi r3,r17,-22600
  r3 = uint64_t(int64_t(int64_t(r17) + -22600));
  // lis r22,-32229
  r22 = uint64_t(int64_t(-2112159744));
  // lis r23,-32229
  r23 = uint64_t(int64_t(-2112159744));
  // lis r24,-32229
  r24 = uint64_t(int64_t(-2112159744));
  // lis r25,-32229
  r25 = uint64_t(int64_t(-2112159744));
  // stw r3,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r3));
  // addi r3,r18,-22160
  r3 = uint64_t(int64_t(int64_t(r18) + -22160));
  // lis r26,-32229
  r26 = uint64_t(int64_t(-2112159744));
  // lis r27,-32229
  r27 = uint64_t(int64_t(-2112159744));
  // lis r28,-32229
  r28 = uint64_t(int64_t(-2112159744));
  // lis r29,-32229
  r29 = uint64_t(int64_t(-2112159744));
  // stw r3,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r3));
  // addi r3,r19,-21832
  r3 = uint64_t(int64_t(int64_t(r19) + -21832));
  // lis r30,-32229
  r30 = uint64_t(int64_t(-2112159744));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r31,-32229
  r31 = uint64_t(int64_t(-2112159744));
  // lis r5,-32229
  r5 = uint64_t(int64_t(-2112159744));
  // stw r3,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r3));
  // addi r3,r20,-21408
  r3 = uint64_t(int64_t(int64_t(r20) + -21408));
  // lis r6,-32229
  r6 = uint64_t(int64_t(-2112159744));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lis r7,-32229
  r7 = uint64_t(int64_t(-2112159744));
  // lis r8,-32229
  r8 = uint64_t(int64_t(-2112159744));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r3));
  // addi r3,r21,-20424
  r3 = uint64_t(int64_t(int64_t(r21) + -20424));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // addi r5,r5,3120
  r5 = uint64_t(int64_t(int64_t(r5) + 3120));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r6,r6,3624
  r6 = uint64_t(int64_t(int64_t(r6) + 3624));
  // addi r7,r7,4040
  r7 = uint64_t(int64_t(int64_t(r7) + 4040));
  // stw r3,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r3));
  // addi r3,r22,-19888
  r3 = uint64_t(int64_t(int64_t(r22) + -19888));
  // addi r8,r8,4456
  r8 = uint64_t(int64_t(int64_t(r8) + 4456));
  // addi r9,r9,4872
  r9 = uint64_t(int64_t(int64_t(r9) + 4872));
  // stw r5,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r5));
  // addi r10,r10,5336
  r10 = uint64_t(int64_t(int64_t(r10) + 5336));
  // stw r6,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r6));
  // stw r7,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r7));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // stw r3,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r3));
  // addi r3,r23,-728
  r3 = uint64_t(int64_t(int64_t(r23) + -728));
  // stw r8,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r8));
  // stw r9,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r9));
  // stw r10,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r10));
  // stw r3,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r3));
  // addi r3,r24,-648
  r3 = uint64_t(int64_t(int64_t(r24) + -648));
  // stw r3,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r3));
  // addi r3,r25,-208
  r3 = uint64_t(int64_t(int64_t(r25) + -208));
  // stw r3,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r3));
  // addi r3,r26,256
  r3 = uint64_t(int64_t(int64_t(r26) + 256));
  // stw r3,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r3));
  // addi r3,r27,720
  r3 = uint64_t(int64_t(int64_t(r27) + 720));
  // stw r3,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r3));
  // addi r3,r28,1224
  r3 = uint64_t(int64_t(int64_t(r28) + 1224));
  // stw r3,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r3));
  // addi r3,r29,1728
  r3 = uint64_t(int64_t(int64_t(r29) + 1728));
  // stw r3,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r3));
  // addi r3,r30,2192
  r3 = uint64_t(int64_t(int64_t(r30) + 2192));
  // stw r3,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r3));
  // addi r3,r31,2656
  r3 = uint64_t(int64_t(int64_t(r31) + 2656));
  // stw r3,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r3));
  // beq cr6,0x821b4ce0
  if (cr6.eq) goto loc_821B4CE0;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r4
  r3 = r4;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B4CE0:
  // addi r1,r1,304
  r1 = uint64_t(int64_t(int64_t(r1) + 304));
  // b 0x825a6ea8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4CE8: lifted from generated/ by lift.py.
lifted::Result f821B4CE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4CE8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4dbc
  if (!cr6.eq) goto loc_821B4DBC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,197
  r4 = uint64_t(int64_t(197));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,198
  r4 = uint64_t(int64_t(198));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,198
  r4 = uint64_t(int64_t(198));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D038, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B4DBC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x821b4dd8
  if (cr0.eq) goto loc_821B4DD8;
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B4DD8:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // and r3,r11,r31
  r3 = r11 & r31;
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

// sub_821B4E10: lifted from generated/ by lift.py.
lifted::Result f821B4E10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4E10 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4ed8
  if (!cr6.eq) goto loc_821B4ED8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r30
  r3 = r30;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r31,17
  r4 = uint64_t(int64_t(int64_t(r31) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,199
  r4 = uint64_t(int64_t(199));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r7,r10,1
  r7 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stwx r31,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r31));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stwx r8,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r8));
  // bl 0x821b4298
  // (left out: dead in C++)
  { const lifted::Result o = f821B4298(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ad5d8
  // (left out: dead in C++)
  { const lifted::Result o = f821AD5D8(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ad840
  // (left out: dead in C++)
  { const lifted::Result o = f821AD840(base, r1, r13, r7); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ada08
  // (left out: dead in C++)
  r3 = f821ADA08(base, r1, r13);
  // bl 0x821adaf0
  // (left out: dead in C++)
  { const lifted::Result o = f821ADAF0(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821B4ED8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x821b4ef4
  if (cr0.eq) goto loc_821B4EF4;
  // bl 0x821ad530
  // (left out: dead in C++)
  r3 = f821AD530(base, r1, r13);
loc_821B4EF4:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4F00: lifted from generated/ by lift.py.
lifted::Result f821B4F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4F00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b4f78
  if (!cr6.eq) goto loc_821B4F78;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r11,56
  r4 = uint64_t(int64_t(int64_t(r11) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,200
  r4 = uint64_t(int64_t(200));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B4F78:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b4fc8
  if (!cr0.eq) goto loc_821B4FC8;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b4fa4
  if (cr0.eq) goto loc_821B4FA4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B4FA4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B4FC8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B4FD8: lifted from generated/ by lift.py.
lifted::Result f821B4FD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B4FD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b509c
  if (!cr6.eq) goto loc_821B509C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r30
  r3 = r30;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r31,17
  r4 = uint64_t(int64_t(int64_t(r31) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,209
  r4 = uint64_t(int64_t(209));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r31,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r31));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r9,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // bl 0x821b4548
  // (left out: dead in C++)
  { const lifted::Result o = f821B4548(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821adf28
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF28(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821adf98
  // (left out: dead in C++)
  { const lifted::Result o = f821ADF98(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae018
  // (left out: dead in C++)
  r3 = f821AE018(base, r1, r13);
  // stw r27,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r27));
loc_821B509C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x821b50dc
  if (cr0.eq) goto loc_821B50DC;
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r10,560(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b50dc
  if (cr6.eq) goto loc_821B50DC;
  // stw r27,740(r11)
  REX_STORE_U32(uint32_t(r11) + 740, uint32_t(r27));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,59
  r4 = uint64_t(int64_t(59));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82180108, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B50DC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B50E8: lifted from generated/ by lift.py.
lifted::Result f821B50E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B50E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5160
  if (!cr6.eq) goto loc_821B5160;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r11,56
  r4 = uint64_t(int64_t(int64_t(r11) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,210
  r4 = uint64_t(int64_t(210));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B5160:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b51b0
  if (!cr0.eq) goto loc_821B51B0;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b518c
  if (cr0.eq) goto loc_821B518C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B518C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B51B0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B51C0: lifted from generated/ by lift.py.
lifted::Result f821B51C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B51C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5284
  if (!cr6.eq) goto loc_821B5284;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r30
  r3 = r30;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r31,17
  r4 = uint64_t(int64_t(int64_t(r31) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,217
  r4 = uint64_t(int64_t(217));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r31,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r31));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stwx r9,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // bl 0x821b46d8
  // (left out: dead in C++)
  { const lifted::Result o = f821B46D8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae198
  // (left out: dead in C++)
  { const lifted::Result o = f821AE198(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae240
  // (left out: dead in C++)
  { const lifted::Result o = f821AE240(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae2c0
  // (left out: dead in C++)
  r3 = f821AE2C0(base, r1, r13);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821B5284:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x821b52c0
  if (cr0.eq) goto loc_821B52C0;
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b52c0
  if (cr6.eq) goto loc_821B52C0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,63
  r4 = uint64_t(int64_t(63));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82180108, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B52C0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B52D0: lifted from generated/ by lift.py.
lifted::Result f821B52D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B52D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5348
  if (!cr6.eq) goto loc_821B5348;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r11,56
  r4 = uint64_t(int64_t(int64_t(r11) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,218
  r4 = uint64_t(int64_t(218));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B5348:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5398
  if (!cr0.eq) goto loc_821B5398;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b5374
  if (cr0.eq) goto loc_821B5374;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5374:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B5398:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B53A8: lifted from generated/ by lift.py.
lifted::Result f821B53A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B53A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b546c
  if (!cr6.eq) goto loc_821B546C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r30
  r3 = r30;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r31,17
  r4 = uint64_t(int64_t(int64_t(r31) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,229
  r4 = uint64_t(int64_t(229));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r7,r10,1
  r7 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stwx r31,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r31));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stwx r8,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r8));
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae6d8
  // (left out: dead in C++)
  { const lifted::Result o = f821AE6D8(base, r1, r13, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae748
  // (left out: dead in C++)
  { const lifted::Result o = f821AE748(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821ae7a8
  // (left out: dead in C++)
  r3 = f821AE7A8(base, r1, r13);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821B546C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x821b5488
  if (cr0.eq) goto loc_821B5488;
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B5488:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5498: lifted from generated/ by lift.py.
lifted::Result f821B5498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5498 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5510
  if (!cr6.eq) goto loc_821B5510;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r11,56
  r4 = uint64_t(int64_t(int64_t(r11) + 56));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,230
  r4 = uint64_t(int64_t(230));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B5510:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5560
  if (!cr0.eq) goto loc_821B5560;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b553c
  if (cr0.eq) goto loc_821B553C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B553C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B5560:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5570: lifted from generated/ by lift.py.
lifted::Result f821B5570(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5570 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5684
  if (!cr6.eq) goto loc_821B5684;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r29,17
  r4 = uint64_t(int64_t(int64_t(r29) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,245
  r4 = uint64_t(int64_t(245));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,756(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 756);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821b5644
  if (cr6.eq) goto loc_821B5644;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
loc_821B5644:
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r7,r10,1
  r7 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stwx r29,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r29));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r7,r10,1
  r7 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stwx r9,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // stw r8,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r8));
loc_821B5684:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // li r4,245
  r4 = uint64_t(int64_t(245));
  // rlwinm r31,r11,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x8211bb18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BB18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8152(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bne cr6,0x821b56bc
  if (!cr6.eq) goto loc_821B56BC;
  // bl 0x821b49f8
  // (left out: dead in C++)
  { const lifted::Result o = f821B49F8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B56BC:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B56C8: lifted from generated/ by lift.py.
lifted::Result f821B56C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B56C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
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
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5774
  if (!cr6.eq) goto loc_821B5774;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r4,r30,17
  r4 = uint64_t(int64_t(int64_t(r30) + 17));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r30,756(r11)
  REX_STORE_U32(uint32_t(r11) + 756, uint32_t(r30));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821b5748
  if (!cr6.eq) goto loc_821B5748;
  // li r4,246
  r4 = uint64_t(int64_t(246));
  // b 0x821b5758
  goto loc_821B5758;
loc_821B5748:
  // addi r4,r30,102
  r4 = uint64_t(int64_t(int64_t(r30) + 102));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,247
  r4 = uint64_t(int64_t(247));
  // mr r3,r31
  r3 = r31;
loc_821B5758:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821B5774:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,247
  r4 = uint64_t(int64_t(247));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // bl 0x8211bb18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BB18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,8152(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8152);
  f31 = double(temp.f32);
  // fcmpu cr6,f1,f31
  cr6 = lifted::CompareFloat(f1, f31);
  // beq cr6,0x821b57bc
  if (cr6.eq) goto loc_821B57BC;
  // li r4,246
  r4 = uint64_t(int64_t(246));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211bb18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211BB18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f31);
  // bne cr6,0x821b57c0
  if (!cr6.eq) goto loc_821B57C0;
loc_821B57BC:
  // bl 0x821b4068
  // (left out: dead in C++)
  { const lifted::Result o = f821B4068(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_821B57C0:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x821b5804
  if (cr6.eq) goto loc_821B5804;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b57e0
  if (cr0.eq) goto loc_821B57E0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B57E0:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r28,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r28));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31656(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31656);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // bl 0x821ad458
  // (left out: dead in C++)
  r3 = f821AD458(base, r1, r13);
loc_821B5804:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5818: lifted from generated/ by lift.py.
lifted::Result f821B5818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5818 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b59b0
  if (!cr6.eq) goto loc_821B59B0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,197
  r4 = uint64_t(int64_t(197));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D038, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,250
  r4 = uint64_t(int64_t(250));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,198
  r4 = uint64_t(int64_t(198));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,198
  r4 = uint64_t(int64_t(198));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,756(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 756);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821b5968
  if (cr6.eq) goto loc_821B5968;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,249
  r4 = uint64_t(int64_t(249));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
loc_821B5968:
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r7,r10,1
  r7 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // stwx r9,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stwx r8,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r8));
  // bl 0x821b49f8
  // (left out: dead in C++)
  { const lifted::Result o = f821B49F8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821B59B0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x821b59f0
  if (cr0.eq) goto loc_821B59F0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B59F0:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5A00: lifted from generated/ by lift.py.
lifted::Result f821B5A00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5A00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5a94
  if (!cr6.eq) goto loc_821B5A94;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x821b5a60
  if (cr6.eq) goto loc_821B5A60;
  // addi r4,r11,134
  r4 = uint64_t(int64_t(int64_t(r11) + 134));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5a70
  goto loc_821B5A70;
loc_821B5A60:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5A70:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,204
  r4 = uint64_t(int64_t(204));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4298
  // (left out: dead in C++)
  { const lifted::Result o = f821B4298(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
loc_821B5A94:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5ae8
  if (!cr0.eq) goto loc_821B5AE8;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b5ac0
  if (cr6.eq) goto loc_821B5AC0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5AC0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // bl 0x821ad530
  // (left out: dead in C++)
  r3 = f821AD530(base, r1, r13);
loc_821B5AE8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5AF8: lifted from generated/ by lift.py.
lifted::Result f821B5AF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5AF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5b8c
  if (!cr6.eq) goto loc_821B5B8C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b5b58
  if (cr6.eq) goto loc_821B5B58;
  // addi r4,r11,149
  r4 = uint64_t(int64_t(int64_t(r11) + 149));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5b68
  goto loc_821B5B68;
loc_821B5B58:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5B68:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,206
  r4 = uint64_t(int64_t(206));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4298
  // (left out: dead in C++)
  { const lifted::Result o = f821B4298(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
loc_821B5B8C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5be0
  if (!cr0.eq) goto loc_821B5BE0;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b5bb8
  if (cr6.eq) goto loc_821B5BB8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5BB8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // bl 0x821ad530
  // (left out: dead in C++)
  r3 = f821AD530(base, r1, r13);
loc_821B5BE0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

namespace {

lifted::Result e821AE4C0(uint8_t* base, const lifted::Args& a) {
  return {f821AE4C0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE540(uint8_t* base, const lifted::Args& a) {
  return f821AE540(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AE5C0(uint8_t* base, const lifted::Args& a) {
  return {f821AE5C0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE640(uint8_t* base, const lifted::Args& a) {
  return {f821AE640(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE6D8(uint8_t* base, const lifted::Args& a) {
  return f821AE6D8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AE748(uint8_t* base, const lifted::Args& a) {
  return f821AE748(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AE7A8(uint8_t* base, const lifted::Args& a) {
  return {f821AE7A8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE808(uint8_t* base, const lifted::Args& a) {
  return f821AE808(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AE868(uint8_t* base, const lifted::Args& a) {
  return {f821AE868(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE8C8(uint8_t* base, const lifted::Args& a) {
  return f821AE8C8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AE928(uint8_t* base, const lifted::Args& a) {
  return {f821AE928(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE988(uint8_t* base, const lifted::Args& a) {
  return f821AE988(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AEA08(uint8_t* base, const lifted::Args& a) {
  return {f821AEA08(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AEA88(uint8_t* base, const lifted::Args& a) {
  return f821AEA88(base, a.r1, a.r13, a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AEAE8(uint8_t* base, const lifted::Args& a) {
  return {f821AEAE8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AEB48(uint8_t* base, const lifted::Args& a) {
  return f821AEB48(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AEC18(uint8_t* base, const lifted::Args& a) {
  return {f821AEC18(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AED18(uint8_t* base, const lifted::Args& a) {
  return f821AED18(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e821AEF48(uint8_t* base, const lifted::Args& a) {
  return f821AEF48(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e821AF0A8(uint8_t* base, const lifted::Args& a) {
  return f821AF0A8(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821AFA08(uint8_t* base, const lifted::Args& a) {
  return {f821AFA08(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AFBD0(uint8_t* base, const lifted::Args& a) {
  return f821AFBD0(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AFC88(uint8_t* base, const lifted::Args& a) {
  return {f821AFC88(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821AFD28(uint8_t* base, const lifted::Args& a) {
  return f821AFD28(base, a.r1, a.r13);
}

lifted::Result e821AFD78(uint8_t* base, const lifted::Args& a) {
  return f821AFD78(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AFF30(uint8_t* base, const lifted::Args& a) {
  return f821AFF30(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0100(uint8_t* base, const lifted::Args& a) {
  return f821B0100(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B02D0(uint8_t* base, const lifted::Args& a) {
  return f821B02D0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B04C8(uint8_t* base, const lifted::Args& a) {
  return f821B04C8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B06C0(uint8_t* base, const lifted::Args& a) {
  return f821B06C0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0890(uint8_t* base, const lifted::Args& a) {
  return f821B0890(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0A60(uint8_t* base, const lifted::Args& a) {
  return f821B0A60(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0C30(uint8_t* base, const lifted::Args& a) {
  return f821B0C30(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0E28(uint8_t* base, const lifted::Args& a) {
  return f821B0E28(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B0FC8(uint8_t* base, const lifted::Args& a) {
  return f821B0FC8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1168(uint8_t* base, const lifted::Args& a) {
  return f821B1168(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1308(uint8_t* base, const lifted::Args& a) {
  return f821B1308(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B14D8(uint8_t* base, const lifted::Args& a) {
  return f821B14D8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1678(uint8_t* base, const lifted::Args& a) {
  return f821B1678(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1960(uint8_t* base, const lifted::Args& a) {
  return f821B1960(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1B40(uint8_t* base, const lifted::Args& a) {
  return f821B1B40(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B1DA0(uint8_t* base, const lifted::Args& a) {
  return f821B1DA0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B2100(uint8_t* base, const lifted::Args& a) {
  return f821B2100(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B23B0(uint8_t* base, const lifted::Args& a) {
  return f821B23B0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2480(uint8_t* base, const lifted::Args& a) {
  return f821B2480(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B25C8(uint8_t* base, const lifted::Args& a) {
  return f821B25C8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2658(uint8_t* base, const lifted::Args& a) {
  return f821B2658(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2728(uint8_t* base, const lifted::Args& a) {
  return f821B2728(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2870(uint8_t* base, const lifted::Args& a) {
  return f821B2870(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B2900(uint8_t* base, const lifted::Args& a) {
  return f821B2900(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B29D0(uint8_t* base, const lifted::Args& a) {
  return f821B29D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B2A60(uint8_t* base, const lifted::Args& a) {
  return f821B2A60(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2B30(uint8_t* base, const lifted::Args& a) {
  return f821B2B30(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2C78(uint8_t* base, const lifted::Args& a) {
  return f821B2C78(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B2D08(uint8_t* base, const lifted::Args& a) {
  return f821B2D08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2DD8(uint8_t* base, const lifted::Args& a) {
  return f821B2DD8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B2F20(uint8_t* base, const lifted::Args& a) {
  return f821B2F20(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B2FB0(uint8_t* base, const lifted::Args& a) {
  return f821B2FB0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3080(uint8_t* base, const lifted::Args& a) {
  return f821B3080(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B3110(uint8_t* base, const lifted::Args& a) {
  return f821B3110(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B31E0(uint8_t* base, const lifted::Args& a) {
  return f821B31E0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B3270(uint8_t* base, const lifted::Args& a) {
  return f821B3270(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3340(uint8_t* base, const lifted::Args& a) {
  return f821B3340(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B33D0(uint8_t* base, const lifted::Args& a) {
  return f821B33D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B34A0(uint8_t* base, const lifted::Args& a) {
  return f821B34A0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B35E0(uint8_t* base, const lifted::Args& a) {
  return f821B35E0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B3670(uint8_t* base, const lifted::Args& a) {
  return f821B3670(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3740(uint8_t* base, const lifted::Args& a) {
  return f821B3740(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B37D0(uint8_t* base, const lifted::Args& a) {
  return f821B37D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B38A0(uint8_t* base, const lifted::Args& a) {
  return f821B38A0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B39A8(uint8_t* base, const lifted::Args& a) {
  return f821B39A8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B3A38(uint8_t* base, const lifted::Args& a) {
  return f821B3A38(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3B08(uint8_t* base, const lifted::Args& a) {
  return f821B3B08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B3B98(uint8_t* base, const lifted::Args& a) {
  return f821B3B98(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3C68(uint8_t* base, const lifted::Args& a) {
  return f821B3C68(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3D70(uint8_t* base, const lifted::Args& a) {
  return f821B3D70(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821B3E00(uint8_t* base, const lifted::Args& a) {
  return f821B3E00(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3ED0(uint8_t* base, const lifted::Args& a) {
  return f821B3ED0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B3FD8(uint8_t* base, const lifted::Args& a) {
  return f821B3FD8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B4068(uint8_t* base, const lifted::Args& a) {
  return f821B4068(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B4298(uint8_t* base, const lifted::Args& a) {
  return f821B4298(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821B4548(uint8_t* base, const lifted::Args& a) {
  return f821B4548(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821B46D8(uint8_t* base, const lifted::Args& a) {
  return f821B46D8(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821B4848(uint8_t* base, const lifted::Args& a) {
  return f821B4848(base, a.r1, a.r13, a.r[3], a.f[0]);
}

lifted::Result e821B49F8(uint8_t* base, const lifted::Args& a) {
  return f821B49F8(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821B4BA0(uint8_t* base, const lifted::Args& a) {
  return f821B4BA0(base, a.r1, a.r13, a.r[1], a.f[0]);
}

lifted::Result e821B4CE8(uint8_t* base, const lifted::Args& a) {
  return f821B4CE8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821B4E10(uint8_t* base, const lifted::Args& a) {
  return f821B4E10(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B4F00(uint8_t* base, const lifted::Args& a) {
  return f821B4F00(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B4FD8(uint8_t* base, const lifted::Args& a) {
  return f821B4FD8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B50E8(uint8_t* base, const lifted::Args& a) {
  return f821B50E8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B51C0(uint8_t* base, const lifted::Args& a) {
  return f821B51C0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B52D0(uint8_t* base, const lifted::Args& a) {
  return f821B52D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B53A8(uint8_t* base, const lifted::Args& a) {
  return f821B53A8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5498(uint8_t* base, const lifted::Args& a) {
  return f821B5498(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5570(uint8_t* base, const lifted::Args& a) {
  return f821B5570(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B56C8(uint8_t* base, const lifted::Args& a) {
  return f821B56C8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5818(uint8_t* base, const lifted::Args& a) {
  return f821B5818(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821B5A00(uint8_t* base, const lifted::Args& a) {
  return f821B5A00(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5AF8(uint8_t* base, const lifted::Args& a) {
  return f821B5AF8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

const Registration kEntries[] = {
    {0x821AE4C0, e821AE4C0},
    {0x821AE540, e821AE540},
    {0x821AE5C0, e821AE5C0},
    {0x821AE640, e821AE640},
    {0x821AE6D8, e821AE6D8},
    {0x821AE748, e821AE748},
    {0x821AE7A8, e821AE7A8},
    {0x821AE808, e821AE808},
    {0x821AE868, e821AE868},
    {0x821AE8C8, e821AE8C8},
    {0x821AE928, e821AE928},
    {0x821AE988, e821AE988},
    {0x821AEA08, e821AEA08},
    {0x821AEA88, e821AEA88},
    {0x821AEAE8, e821AEAE8},
    {0x821AEB48, e821AEB48},
    {0x821AEC18, e821AEC18},
    {0x821AED18, e821AED18},
    {0x821AEF48, e821AEF48},
    {0x821AF0A8, e821AF0A8},
    {0x821AFA08, e821AFA08},
    {0x821AFBD0, e821AFBD0},
    {0x821AFC88, e821AFC88},
    {0x821AFD28, e821AFD28},
    {0x821AFD78, e821AFD78},
    {0x821AFF30, e821AFF30},
    {0x821B0100, e821B0100},
    {0x821B02D0, e821B02D0},
    {0x821B04C8, e821B04C8},
    {0x821B06C0, e821B06C0},
    {0x821B0890, e821B0890},
    {0x821B0A60, e821B0A60},
    {0x821B0C30, e821B0C30},
    {0x821B0E28, e821B0E28},
    {0x821B0FC8, e821B0FC8},
    {0x821B1168, e821B1168},
    {0x821B1308, e821B1308},
    {0x821B14D8, e821B14D8},
    {0x821B1678, e821B1678},
    {0x821B1960, e821B1960},
    {0x821B1B40, e821B1B40},
    {0x821B1DA0, e821B1DA0},
    {0x821B2100, e821B2100},
    {0x821B23B0, e821B23B0},
    {0x821B2480, e821B2480},
    {0x821B25C8, e821B25C8},
    {0x821B2658, e821B2658},
    {0x821B2728, e821B2728},
    {0x821B2870, e821B2870},
    {0x821B2900, e821B2900},
    {0x821B29D0, e821B29D0},
    {0x821B2A60, e821B2A60},
    {0x821B2B30, e821B2B30},
    {0x821B2C78, e821B2C78},
    {0x821B2D08, e821B2D08},
    {0x821B2DD8, e821B2DD8},
    {0x821B2F20, e821B2F20},
    {0x821B2FB0, e821B2FB0},
    {0x821B3080, e821B3080},
    {0x821B3110, e821B3110},
    {0x821B31E0, e821B31E0},
    {0x821B3270, e821B3270},
    {0x821B3340, e821B3340},
    {0x821B33D0, e821B33D0},
    {0x821B34A0, e821B34A0},
    {0x821B35E0, e821B35E0},
    {0x821B3670, e821B3670},
    {0x821B3740, e821B3740},
    {0x821B37D0, e821B37D0},
    {0x821B38A0, e821B38A0},
    {0x821B39A8, e821B39A8},
    {0x821B3A38, e821B3A38},
    {0x821B3B08, e821B3B08},
    {0x821B3B98, e821B3B98},
    {0x821B3C68, e821B3C68},
    {0x821B3D70, e821B3D70},
    {0x821B3E00, e821B3E00},
    {0x821B3ED0, e821B3ED0},
    {0x821B3FD8, e821B3FD8},
    {0x821B4068, e821B4068},
    {0x821B4298, e821B4298},
    {0x821B4548, e821B4548},
    {0x821B46D8, e821B46D8},
    {0x821B4848, e821B4848},
    {0x821B49F8, e821B49F8},
    {0x821B4BA0, e821B4BA0},
    {0x821B4CE8, e821B4CE8},
    {0x821B4E10, e821B4E10},
    {0x821B4F00, e821B4F00},
    {0x821B4FD8, e821B4FD8},
    {0x821B50E8, e821B50E8},
    {0x821B51C0, e821B51C0},
    {0x821B52D0, e821B52D0},
    {0x821B53A8, e821B53A8},
    {0x821B5498, e821B5498},
    {0x821B5570, e821B5570},
    {0x821B56C8, e821B56C8},
    {0x821B5818, e821B5818},
    {0x821B5A00, e821B5A00},
    {0x821B5AF8, e821B5AF8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_010)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_821AE4C0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE4C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE4C0);
}

extern "C" REX_FUNC(sub_821AE540) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE540(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE540);
}

extern "C" REX_FUNC(sub_821AE5C0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE5C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE5C0);
}

extern "C" REX_FUNC(sub_821AE640) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE640(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE640);
}

extern "C" REX_FUNC(sub_821AE6D8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE6D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE6D8);
}

extern "C" REX_FUNC(sub_821AE748) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE748(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE748);
}

extern "C" REX_FUNC(sub_821AE7A8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE7A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE7A8);
}

extern "C" REX_FUNC(sub_821AE808) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE808(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE808);
}

extern "C" REX_FUNC(sub_821AE868) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE868(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE868);
}

extern "C" REX_FUNC(sub_821AE8C8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE8C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE8C8);
}

extern "C" REX_FUNC(sub_821AE928) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE928(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE928);
}

extern "C" REX_FUNC(sub_821AE988) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AE988(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AE988);
}

extern "C" REX_FUNC(sub_821AEA08) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEA08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEA08);
}

extern "C" REX_FUNC(sub_821AEA88) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEA88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEA88);
}

extern "C" REX_FUNC(sub_821AEAE8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEAE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEAE8);
}

extern "C" REX_FUNC(sub_821AEB48) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEB48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEB48);
}

extern "C" REX_FUNC(sub_821AEC18) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEC18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEC18);
}

extern "C" REX_FUNC(sub_821AED18) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AED18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AED18);
}

extern "C" REX_FUNC(sub_821AEF48) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AEF48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AEF48);
}

extern "C" REX_FUNC(sub_821AF0A8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AF0A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AF0A8);
}

extern "C" REX_FUNC(sub_821AFA08) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFA08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFA08);
}

extern "C" REX_FUNC(sub_821AFBD0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFBD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFBD0);
}

extern "C" REX_FUNC(sub_821AFC88) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFC88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFC88);
}

extern "C" REX_FUNC(sub_821AFD28) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFD28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFD28);
}

extern "C" REX_FUNC(sub_821AFD78) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFD78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFD78);
}

extern "C" REX_FUNC(sub_821AFF30) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821AFF30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821AFF30);
}

extern "C" REX_FUNC(sub_821B0100) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0100(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0100);
}

extern "C" REX_FUNC(sub_821B02D0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B02D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B02D0);
}

extern "C" REX_FUNC(sub_821B04C8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B04C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B04C8);
}

extern "C" REX_FUNC(sub_821B06C0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B06C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B06C0);
}

extern "C" REX_FUNC(sub_821B0890) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0890(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0890);
}

extern "C" REX_FUNC(sub_821B0A60) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0A60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0A60);
}

extern "C" REX_FUNC(sub_821B0C30) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0C30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0C30);
}

extern "C" REX_FUNC(sub_821B0E28) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0E28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0E28);
}

extern "C" REX_FUNC(sub_821B0FC8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B0FC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B0FC8);
}

extern "C" REX_FUNC(sub_821B1168) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1168(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1168);
}

extern "C" REX_FUNC(sub_821B1308) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1308(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1308);
}

extern "C" REX_FUNC(sub_821B14D8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B14D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B14D8);
}

extern "C" REX_FUNC(sub_821B1678) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1678(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1678);
}

extern "C" REX_FUNC(sub_821B1960) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1960(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1960);
}

extern "C" REX_FUNC(sub_821B1B40) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1B40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1B40);
}

extern "C" REX_FUNC(sub_821B1DA0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B1DA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B1DA0);
}

extern "C" REX_FUNC(sub_821B2100) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2100(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2100);
}

extern "C" REX_FUNC(sub_821B23B0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B23B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B23B0);
}

extern "C" REX_FUNC(sub_821B2480) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2480(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2480);
}

extern "C" REX_FUNC(sub_821B25C8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B25C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B25C8);
}

extern "C" REX_FUNC(sub_821B2658) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2658(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2658);
}

extern "C" REX_FUNC(sub_821B2728) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2728(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2728);
}

extern "C" REX_FUNC(sub_821B2870) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2870(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2870);
}

extern "C" REX_FUNC(sub_821B2900) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2900(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2900);
}

extern "C" REX_FUNC(sub_821B29D0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B29D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B29D0);
}

extern "C" REX_FUNC(sub_821B2A60) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2A60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2A60);
}

extern "C" REX_FUNC(sub_821B2B30) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2B30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2B30);
}

extern "C" REX_FUNC(sub_821B2C78) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2C78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2C78);
}

extern "C" REX_FUNC(sub_821B2D08) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2D08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2D08);
}

extern "C" REX_FUNC(sub_821B2DD8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2DD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2DD8);
}

extern "C" REX_FUNC(sub_821B2F20) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2F20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2F20);
}

extern "C" REX_FUNC(sub_821B2FB0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B2FB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B2FB0);
}

extern "C" REX_FUNC(sub_821B3080) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3080(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3080);
}

extern "C" REX_FUNC(sub_821B3110) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3110(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3110);
}

extern "C" REX_FUNC(sub_821B31E0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B31E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B31E0);
}

extern "C" REX_FUNC(sub_821B3270) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3270);
}

extern "C" REX_FUNC(sub_821B3340) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3340(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3340);
}

extern "C" REX_FUNC(sub_821B33D0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B33D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B33D0);
}

extern "C" REX_FUNC(sub_821B34A0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B34A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B34A0);
}

extern "C" REX_FUNC(sub_821B35E0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B35E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B35E0);
}

extern "C" REX_FUNC(sub_821B3670) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3670(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3670);
}

extern "C" REX_FUNC(sub_821B3740) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3740(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3740);
}

extern "C" REX_FUNC(sub_821B37D0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B37D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B37D0);
}

extern "C" REX_FUNC(sub_821B38A0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B38A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B38A0);
}

extern "C" REX_FUNC(sub_821B39A8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B39A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B39A8);
}

extern "C" REX_FUNC(sub_821B3A38) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3A38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3A38);
}

extern "C" REX_FUNC(sub_821B3B08) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3B08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3B08);
}

extern "C" REX_FUNC(sub_821B3B98) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3B98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3B98);
}

extern "C" REX_FUNC(sub_821B3C68) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3C68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3C68);
}

extern "C" REX_FUNC(sub_821B3D70) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3D70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3D70);
}

extern "C" REX_FUNC(sub_821B3E00) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3E00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3E00);
}

extern "C" REX_FUNC(sub_821B3ED0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3ED0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3ED0);
}

extern "C" REX_FUNC(sub_821B3FD8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B3FD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B3FD8);
}

extern "C" REX_FUNC(sub_821B4068) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4068(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4068);
}

extern "C" REX_FUNC(sub_821B4298) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4298(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4298);
}

extern "C" REX_FUNC(sub_821B4548) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4548(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4548);
}

extern "C" REX_FUNC(sub_821B46D8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B46D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B46D8);
}

extern "C" REX_FUNC(sub_821B4848) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4848(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4848);
}

extern "C" REX_FUNC(sub_821B49F8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B49F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B49F8);
}

extern "C" REX_FUNC(sub_821B4BA0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4BA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4BA0);
}

extern "C" REX_FUNC(sub_821B4CE8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4CE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4CE8);
}

extern "C" REX_FUNC(sub_821B4E10) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4E10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4E10);
}

extern "C" REX_FUNC(sub_821B4F00) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4F00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4F00);
}

extern "C" REX_FUNC(sub_821B4FD8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B4FD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B4FD8);
}

extern "C" REX_FUNC(sub_821B50E8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B50E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B50E8);
}

extern "C" REX_FUNC(sub_821B51C0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B51C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B51C0);
}

extern "C" REX_FUNC(sub_821B52D0) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B52D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B52D0);
}

extern "C" REX_FUNC(sub_821B53A8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B53A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B53A8);
}

extern "C" REX_FUNC(sub_821B5498) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B5498(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5498);
}

extern "C" REX_FUNC(sub_821B5570) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B5570(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5570);
}

extern "C" REX_FUNC(sub_821B56C8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B56C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B56C8);
}

extern "C" REX_FUNC(sub_821B5818) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B5818(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5818);
}

extern "C" REX_FUNC(sub_821B5A00) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B5A00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5A00);
}

extern "C" REX_FUNC(sub_821B5AF8) {
  if (!ITC_FLAG(native_lifted_010)) {
    __imp__sub_821B5AF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5AF8);
}
