// Lift sweep batch 027 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_027, true, "ITC",
              "Run lift sweep batch 027's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_825DACC8: lifted from generated/ by lift.py.
lifted::Result f825DACC8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DACC8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,-21792
  r3 = uint64_t(int64_t(int64_t(r11) + -21792));
  // b 0x82592638
  { const lifted::Result o = game::Semaphore::Dtor(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DACD8: lifted from generated/ by lift.py.
lifted::Result f825DACD8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DACD8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,-21872
  r3 = uint64_t(int64_t(int64_t(r11) + -21872));
  // b 0x82592638
  { const lifted::Result o = game::Semaphore::Dtor(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DACE8: lifted from generated/ by lift.py.
lifted::Result f825DACE8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DACE8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,-21832
  r3 = uint64_t(int64_t(int64_t(r11) + -21832));
  // b 0x82592638
  { const lifted::Result o = game::Semaphore::Dtor(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DACF8: lifted from generated/ by lift.py.
lifted::Result f825DACF8(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DACF8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,-21912
  r3 = uint64_t(int64_t(int64_t(r11) + -21912));
  // b 0x82592638
  { const lifted::Result o = game::Semaphore::Dtor(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DAD08: lifted from generated/ by lift.py.
void f825DAD08() {
  ITC_NATIVE_RUNNING("sub_825DAD08 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r11,2748
  r10 = uint64_t(int64_t(int64_t(r11) + 2748));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r9,r11,2740
  r9 = uint64_t(int64_t(int64_t(r11) + 2740));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r8,r11,3768
  r8 = uint64_t(int64_t(int64_t(r11) + 3768));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // addi r11,r11,2896
  r11 = uint64_t(int64_t(int64_t(r11) + 2896));
  // stw r10,24(r11)
  guest::StoreU32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r9,4(r11)
  guest::StoreU32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,0(r11)
  guest::StoreU32(uint32_t(r11) + 0, uint32_t(r8));
  // blr
  return;
}

// sub_825DAD38: lifted from generated/ by lift.py.
uint64_t f825DAD38(uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DAD38 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // li r30,959
  r30 = uint64_t(int64_t(959));
  // addi r11,r11,20840
  r11 = uint64_t(int64_t(int64_t(r11) + 20840));
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r31,r11,22784
  r31 = uint64_t(int64_t(int64_t(r11) + 22784));
loc_825DAD60:
  // addi r31,r31,-92
  r31 = uint64_t(int64_t(int64_t(r31) + -92));
  // mr r3,r31
  r3 = r31;
  // bl 0x8224fb10
  // (left out: dead in C++)
  { const lifted::Result o = game::ListNode::Dtor(LIFTED_ARGS(0x825DAD6C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x825dad60
  if (!cr0.lt) goto loc_825DAD60;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825DAD98: lifted from generated/ by lift.py.
lifted::Result f825DAD98(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DAD98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r30,20
  r30 = uint64_t(int64_t(20));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // addi r31,r11,12432
  r31 = uint64_t(int64_t(int64_t(r11) + 12432));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r29,r11,7872
  r29 = uint64_t(int64_t(int64_t(r11) + 7872));
loc_825DADBC:
  // addi r31,r31,-592
  r31 = uint64_t(int64_t(int64_t(r31) + -592));
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // addi r6,r11,25920
  r6 = uint64_t(int64_t(int64_t(r11) + 25920));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // addi r3,r31,304
  r3 = uint64_t(int64_t(int64_t(r31) + 304));
  // stw r29,0(r31)
  guest::StoreU32(uint32_t(r31) + 0, uint32_t(r29));
  // bl 0x820900c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820900C0(LIFTED_ARGS(0x825DADDC, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x825dadbc
  if (!cr0.lt) goto loc_825DADBC;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825DADF0: lifted from generated/ by lift.py.
void f825DADF0() {
  ITC_NATIVE_RUNNING("sub_825DADF0 (lifted)");
  // blr
  return;
}

// sub_825DADF8: lifted from generated/ by lift.py.
void f825DADF8() {
  ITC_NATIVE_RUNNING("sub_825DADF8 (lifted)");
  // blr
  return;
}

// sub_825DAE00: lifted from generated/ by lift.py.
void f825DAE00() {
  ITC_NATIVE_RUNNING("sub_825DAE00 (lifted)");
  // blr
  return;
}

// sub_825DAE08: lifted from generated/ by lift.py.
void f825DAE08() {
  ITC_NATIVE_RUNNING("sub_825DAE08 (lifted)");
  // blr
  return;
}

// sub_825DAE10: lifted from generated/ by lift.py.
void f825DAE10() {
  ITC_NATIVE_RUNNING("sub_825DAE10 (lifted)");
  // blr
  return;
}

// sub_825DAE18: lifted from generated/ by lift.py.
void f825DAE18() {
  ITC_NATIVE_RUNNING("sub_825DAE18 (lifted)");
  // blr
  return;
}

// sub_825DAE20: lifted from generated/ by lift.py.
void f825DAE20() {
  ITC_NATIVE_RUNNING("sub_825DAE20 (lifted)");
  // blr
  return;
}

// sub_825DAE28: lifted from generated/ by lift.py.
lifted::Result f825DAE28(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DAE28 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // li r30,44
  r30 = uint64_t(int64_t(44));
  // addi r11,r11,6656
  r11 = uint64_t(int64_t(int64_t(r11) + 6656));
  // addi r31,r11,2880
  r31 = uint64_t(int64_t(int64_t(r11) + 2880));
loc_825DAE4C:
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // addi r31,r31,-64
  r31 = uint64_t(int64_t(int64_t(r31) + -64));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // addi r6,r11,25920
  r6 = uint64_t(int64_t(int64_t(r11) + 25920));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r31
  r3 = r31;
  // bl 0x820900c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820900C0(LIFTED_ARGS(0x825DAE68, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x825dae4c
  if (!cr0.lt) goto loc_825DAE4C;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825DAE88: lifted from generated/ by lift.py.
void f825DAE88() {
  ITC_NATIVE_RUNNING("sub_825DAE88 (lifted)");
  // blr
  return;
}

// sub_825DAE90: lifted from generated/ by lift.py.
void f825DAE90() {
  ITC_NATIVE_RUNNING("sub_825DAE90 (lifted)");
  // blr
  return;
}

// sub_825DAE98: lifted from generated/ by lift.py.
void f825DAE98() {
  ITC_NATIVE_RUNNING("sub_825DAE98 (lifted)");
  // blr
  return;
}

// sub_825DAEA0: lifted from generated/ by lift.py.
void f825DAEA0() {
  ITC_NATIVE_RUNNING("sub_825DAEA0 (lifted)");
  // blr
  return;
}

// sub_825DAEA8: lifted from generated/ by lift.py.
void f825DAEA8() {
  ITC_NATIVE_RUNNING("sub_825DAEA8 (lifted)");
  // blr
  return;
}

// sub_825DAEB0: lifted from generated/ by lift.py.
void f825DAEB0() {
  ITC_NATIVE_RUNNING("sub_825DAEB0 (lifted)");
  // blr
  return;
}

// sub_825DAEB8: lifted from generated/ by lift.py.
void f825DAEB8() {
  ITC_NATIVE_RUNNING("sub_825DAEB8 (lifted)");
  // blr
  return;
}

// sub_825DAEC0: lifted from generated/ by lift.py.
void f825DAEC0() {
  ITC_NATIVE_RUNNING("sub_825DAEC0 (lifted)");
  // blr
  return;
}

// sub_825DAEC8: lifted from generated/ by lift.py.
void f825DAEC8() {
  ITC_NATIVE_RUNNING("sub_825DAEC8 (lifted)");
  // blr
  return;
}

// sub_825DAED0: lifted from generated/ by lift.py.
void f825DAED0() {
  ITC_NATIVE_RUNNING("sub_825DAED0 (lifted)");
  // blr
  return;
}

// sub_825DAED8: lifted from generated/ by lift.py.
void f825DAED8() {
  ITC_NATIVE_RUNNING("sub_825DAED8 (lifted)");
  // blr
  return;
}

// sub_825DAEE0: lifted from generated/ by lift.py.
void f825DAEE0() {
  ITC_NATIVE_RUNNING("sub_825DAEE0 (lifted)");
  // blr
  return;
}

// sub_825DAEE8: lifted from generated/ by lift.py.
void f825DAEE8() {
  ITC_NATIVE_RUNNING("sub_825DAEE8 (lifted)");
  // blr
  return;
}

// sub_825DAEF0: lifted from generated/ by lift.py.
void f825DAEF0() {
  ITC_NATIVE_RUNNING("sub_825DAEF0 (lifted)");
  // blr
  return;
}

// sub_825DAEF8: lifted from generated/ by lift.py.
void f825DAEF8() {
  ITC_NATIVE_RUNNING("sub_825DAEF8 (lifted)");
  // blr
  return;
}

// sub_825DAF00: lifted from generated/ by lift.py.
void f825DAF00() {
  ITC_NATIVE_RUNNING("sub_825DAF00 (lifted)");
  // blr
  return;
}

// sub_825DAF08: lifted from generated/ by lift.py.
void f825DAF08() {
  ITC_NATIVE_RUNNING("sub_825DAF08 (lifted)");
  // blr
  return;
}

// sub_825DAF10: lifted from generated/ by lift.py.
void f825DAF10() {
  ITC_NATIVE_RUNNING("sub_825DAF10 (lifted)");
  // blr
  return;
}

// sub_825DAF18: lifted from generated/ by lift.py.
lifted::Result f825DAF18(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825DAF18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23756(r11)
  r3 = guest::LoadU32(uint32_t(r11) + 23756);
  // b 0x825a71c0
  { const lifted::Result o = lifted::entry::e825A71C0(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DAF28: lifted from generated/ by lift.py.
void f825DAF28() {
  ITC_NATIVE_RUNNING("sub_825DAF28 (lifted)");
  // blr
  return;
}

// sub_825DAF30: lifted from generated/ by lift.py.
void f825DAF30() {
  ITC_NATIVE_RUNNING("sub_825DAF30 (lifted)");
  // blr
  return;
}

// sub_825DAF38: lifted from generated/ by lift.py.
void f825DAF38() {
  ITC_NATIVE_RUNNING("sub_825DAF38 (lifted)");
  // blr
  return;
}

// sub_825DAF40: lifted from generated/ by lift.py.
void f825DAF40() {
  ITC_NATIVE_RUNNING("sub_825DAF40 (lifted)");
  // blr
  return;
}

// sub_825DAF48: lifted from generated/ by lift.py.
void f825DAF48() {
  ITC_NATIVE_RUNNING("sub_825DAF48 (lifted)");
  // blr
  return;
}

// sub_825DAF50: lifted from generated/ by lift.py.
void f825DAF50() {
  ITC_NATIVE_RUNNING("sub_825DAF50 (lifted)");
  // blr
  return;
}

// sub_825DAF58: lifted from generated/ by lift.py.
void f825DAF58() {
  ITC_NATIVE_RUNNING("sub_825DAF58 (lifted)");
  // blr
  return;
}

// sub_825DAF60: lifted from generated/ by lift.py.
void f825DAF60() {
  ITC_NATIVE_RUNNING("sub_825DAF60 (lifted)");
  // blr
  return;
}

// sub_825DAF68: lifted from generated/ by lift.py.
void f825DAF68() {
  ITC_NATIVE_RUNNING("sub_825DAF68 (lifted)");
  // blr
  return;
}

// sub_825DAF70: lifted from generated/ by lift.py.
void f825DAF70() {
  ITC_NATIVE_RUNNING("sub_825DAF70 (lifted)");
  // blr
  return;
}

// sub_825DAF78: lifted from generated/ by lift.py.
void f825DAF78() {
  ITC_NATIVE_RUNNING("sub_825DAF78 (lifted)");
  // blr
  return;
}

// sub_825DAF80: lifted from generated/ by lift.py.
void f825DAF80() {
  ITC_NATIVE_RUNNING("sub_825DAF80 (lifted)");
  // blr
  return;
}

// sub_825DAF88: lifted from generated/ by lift.py.
void f825DAF88() {
  ITC_NATIVE_RUNNING("sub_825DAF88 (lifted)");
  // blr
  return;
}

// sub_825DAF90: lifted from generated/ by lift.py.
void f825DAF90() {
  ITC_NATIVE_RUNNING("sub_825DAF90 (lifted)");
  // blr
  return;
}

// sub_825DAF98: lifted from generated/ by lift.py.
void f825DAF98() {
  ITC_NATIVE_RUNNING("sub_825DAF98 (lifted)");
  // blr
  return;
}

// sub_825DAFA0: lifted from generated/ by lift.py.
void f825DAFA0() {
  ITC_NATIVE_RUNNING("sub_825DAFA0 (lifted)");
  // blr
  return;
}

// sub_825DAFA8: lifted from generated/ by lift.py.
void f825DAFA8() {
  ITC_NATIVE_RUNNING("sub_825DAFA8 (lifted)");
  // blr
  return;
}

// sub_825DAFB0: lifted from generated/ by lift.py.
void f825DAFB0() {
  ITC_NATIVE_RUNNING("sub_825DAFB0 (lifted)");
  // blr
  return;
}

// sub_825DAFB8: lifted from generated/ by lift.py.
void f825DAFB8() {
  ITC_NATIVE_RUNNING("sub_825DAFB8 (lifted)");
  // blr
  return;
}

// sub_825DAFC0: lifted from generated/ by lift.py.
void f825DAFC0() {
  ITC_NATIVE_RUNNING("sub_825DAFC0 (lifted)");
  // blr
  return;
}

// sub_825DAFC8: lifted from generated/ by lift.py.
void f825DAFC8() {
  ITC_NATIVE_RUNNING("sub_825DAFC8 (lifted)");
  // blr
  return;
}

// sub_825DAFD0: lifted from generated/ by lift.py.
void f825DAFD0() {
  ITC_NATIVE_RUNNING("sub_825DAFD0 (lifted)");
  // blr
  return;
}

// sub_825DAFD8: lifted from generated/ by lift.py.
void f825DAFD8() {
  ITC_NATIVE_RUNNING("sub_825DAFD8 (lifted)");
  // blr
  return;
}

// sub_825DAFE0: lifted from generated/ by lift.py.
void f825DAFE0() {
  ITC_NATIVE_RUNNING("sub_825DAFE0 (lifted)");
  // blr
  return;
}

// sub_825DAFE8: lifted from generated/ by lift.py.
void f825DAFE8() {
  ITC_NATIVE_RUNNING("sub_825DAFE8 (lifted)");
  // blr
  return;
}

// sub_825DAFF0: lifted from generated/ by lift.py.
void f825DAFF0() {
  ITC_NATIVE_RUNNING("sub_825DAFF0 (lifted)");
  // blr
  return;
}

// sub_825DAFF8: lifted from generated/ by lift.py.
void f825DAFF8() {
  ITC_NATIVE_RUNNING("sub_825DAFF8 (lifted)");
  // blr
  return;
}

// sub_825DB000: lifted from generated/ by lift.py.
void f825DB000() {
  ITC_NATIVE_RUNNING("sub_825DB000 (lifted)");
  // blr
  return;
}

// sub_825DB008: lifted from generated/ by lift.py.
void f825DB008() {
  ITC_NATIVE_RUNNING("sub_825DB008 (lifted)");
  // blr
  return;
}

// sub_825DB010: lifted from generated/ by lift.py.
void f825DB010() {
  ITC_NATIVE_RUNNING("sub_825DB010 (lifted)");
  // blr
  return;
}

// sub_825DB018: lifted from generated/ by lift.py.
void f825DB018() {
  ITC_NATIVE_RUNNING("sub_825DB018 (lifted)");
  // blr
  return;
}

// sub_825DB020: lifted from generated/ by lift.py.
void f825DB020() {
  ITC_NATIVE_RUNNING("sub_825DB020 (lifted)");
  // blr
  return;
}

// sub_825DB028: lifted from generated/ by lift.py.
void f825DB028() {
  ITC_NATIVE_RUNNING("sub_825DB028 (lifted)");
  // blr
  return;
}

// sub_825DB030: lifted from generated/ by lift.py.
lifted::Result f825DB030(uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB030 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // b 0x8214e678
  { const lifted::Result o = f8214E678(r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB040: lifted from generated/ by lift.py.
lifted::Result f825DB040(uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825DB040 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f1 = 0.0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // b 0x8214f690
  { const lifted::Result o = f8214F690(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB050: lifted from generated/ by lift.py.
lifted::Result f825DB050(uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB050 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,29448
  r31 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825db078
  if (cr6.eq) goto loc_825DB078;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(LIFTED_ARGS(0x825DB078, r31)); r3 = o.r3; f1 = o.f1; }
loc_825DB078:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  guest::StoreU32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  guest::StoreU32(uint32_t(r31) + 12, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825DB0A0: lifted from generated/ by lift.py.
lifted::Result f825DB0A0(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB0A0 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r30,3
  r30 = uint64_t(int64_t(3));
  // addi r11,r11,30320
  r11 = uint64_t(int64_t(int64_t(r11) + 30320));
  // addi r31,r11,432
  r31 = uint64_t(int64_t(int64_t(r11) + 432));
loc_825DB0C4:
  // addi r31,r31,-12
  r31 = uint64_t(int64_t(int64_t(r31) + -12));
  // mr r3,r31
  r3 = r31;
  // bl 0x8214c930
  // (left out: dead in C++)
  { const lifted::Result o = f8214C930(r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x825db0c4
  if (!cr0.lt) goto loc_825DB0C4;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825DB0F0: lifted from generated/ by lift.py.
void f825DB0F0() {
  ITC_NATIVE_RUNNING("sub_825DB0F0 (lifted)");
  // blr
  return;
}

// sub_825DB0F8: lifted from generated/ by lift.py.
void f825DB0F8() {
  ITC_NATIVE_RUNNING("sub_825DB0F8 (lifted)");
  // blr
  return;
}

// sub_825DB100: lifted from generated/ by lift.py.
void f825DB100() {
  ITC_NATIVE_RUNNING("sub_825DB100 (lifted)");
  // blr
  return;
}

// sub_825DB108: lifted from generated/ by lift.py.
void f825DB108() {
  ITC_NATIVE_RUNNING("sub_825DB108 (lifted)");
  // blr
  return;
}

// sub_825DB110: lifted from generated/ by lift.py.
void f825DB110() {
  ITC_NATIVE_RUNNING("sub_825DB110 (lifted)");
  // blr
  return;
}

// sub_825DB118: lifted from generated/ by lift.py.
void f825DB118() {
  ITC_NATIVE_RUNNING("sub_825DB118 (lifted)");
  // blr
  return;
}

// sub_825DB120: lifted from generated/ by lift.py.
void f825DB120() {
  ITC_NATIVE_RUNNING("sub_825DB120 (lifted)");
  // blr
  return;
}

// sub_825DB128: lifted from generated/ by lift.py.
void f825DB128() {
  ITC_NATIVE_RUNNING("sub_825DB128 (lifted)");
  // blr
  return;
}

// sub_825DB130: lifted from generated/ by lift.py.
void f825DB130() {
  ITC_NATIVE_RUNNING("sub_825DB130 (lifted)");
  // blr
  return;
}

// sub_825DB138: lifted from generated/ by lift.py.
void f825DB138() {
  ITC_NATIVE_RUNNING("sub_825DB138 (lifted)");
  // blr
  return;
}

// sub_825DB140: lifted from generated/ by lift.py.
void f825DB140() {
  ITC_NATIVE_RUNNING("sub_825DB140 (lifted)");
  // blr
  return;
}

// sub_825DB148: lifted from generated/ by lift.py.
void f825DB148() {
  ITC_NATIVE_RUNNING("sub_825DB148 (lifted)");
  // blr
  return;
}

// sub_825DB150: lifted from generated/ by lift.py.
void f825DB150() {
  ITC_NATIVE_RUNNING("sub_825DB150 (lifted)");
  // blr
  return;
}

// sub_825DB158: lifted from generated/ by lift.py.
void f825DB158() {
  ITC_NATIVE_RUNNING("sub_825DB158 (lifted)");
  // blr
  return;
}

// sub_825DB160: lifted from generated/ by lift.py.
void f825DB160() {
  ITC_NATIVE_RUNNING("sub_825DB160 (lifted)");
  // blr
  return;
}

// sub_825DB168: lifted from generated/ by lift.py.
void f825DB168() {
  ITC_NATIVE_RUNNING("sub_825DB168 (lifted)");
  // blr
  return;
}

// sub_825DB170: lifted from generated/ by lift.py.
lifted::Result f825DB170(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB170 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r11,r11,-31456
  r11 = uint64_t(int64_t(int64_t(r11) + -31456));
  // addis r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r3,r11,-18928
  r3 = uint64_t(int64_t(int64_t(r11) + -18928));
  // b 0x82104630
  { const lifted::Result o = game::RaceSnapshot::Destroy(r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB188: lifted from generated/ by lift.py.
void f825DB188() {
  ITC_NATIVE_RUNNING("sub_825DB188 (lifted)");
  // blr
  return;
}

// sub_825DB190: lifted from generated/ by lift.py.
void f825DB190() {
  ITC_NATIVE_RUNNING("sub_825DB190 (lifted)");
  // blr
  return;
}

// sub_825DB198: lifted from generated/ by lift.py.
void f825DB198() {
  ITC_NATIVE_RUNNING("sub_825DB198 (lifted)");
  // blr
  return;
}

// sub_825DB1A0: lifted from generated/ by lift.py.
void f825DB1A0() {
  ITC_NATIVE_RUNNING("sub_825DB1A0 (lifted)");
  // blr
  return;
}

// sub_825DB1A8: lifted from generated/ by lift.py.
void f825DB1A8() {
  ITC_NATIVE_RUNNING("sub_825DB1A8 (lifted)");
  // blr
  return;
}

// sub_825DB1B0: lifted from generated/ by lift.py.
lifted::Result f825DB1B0(uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB1B0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r11,r11,28240
  r11 = uint64_t(int64_t(int64_t(r11) + 28240));
  // addis r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r3,r11,-18928
  r3 = uint64_t(int64_t(int64_t(r11) + -18928));
  // b 0x82104630
  { const lifted::Result o = game::RaceSnapshot::Destroy(r1, r13, r3, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB1C8: lifted from generated/ by lift.py.
void f825DB1C8() {
  ITC_NATIVE_RUNNING("sub_825DB1C8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32155
  r10 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-22424
  r11 = uint64_t(int64_t(int64_t(r11) + -22424));
  // stw r11,-11020(r10)
  guest::StoreU32(uint32_t(r10) + -11020, uint32_t(r11));
  // blr
  return;
}

// sub_825DB1E0: lifted from generated/ by lift.py.
void f825DB1E0() {
  ITC_NATIVE_RUNNING("sub_825DB1E0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lis r10,-32155
  r10 = uint64_t(int64_t(-2107310080));
  // addi r11,r11,-22424
  r11 = uint64_t(int64_t(int64_t(r11) + -22424));
  // stw r11,-10988(r10)
  guest::StoreU32(uint32_t(r10) + -10988, uint32_t(r11));
  // blr
  return;
}

// sub_825DB1F8: lifted from generated/ by lift.py.
void f825DB1F8() {
  ITC_NATIVE_RUNNING("sub_825DB1F8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // addi r11,r11,2908
  r11 = uint64_t(int64_t(int64_t(r11) + 2908));
  // stw r11,-9744(r10)
  guest::StoreU32(uint32_t(r10) + -9744, uint32_t(r11));
  // blr
  return;
}

// sub_825DB210: lifted from generated/ by lift.py.
uint64_t f825DB210(uint64_t r1, uint64_t r13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825DB210 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-18128
  r3 = uint64_t(int64_t(int64_t(r11) + -18128));
  // b 0x82106540
  { const lifted::Result o = lifted::entry::e82106540(LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825DB220: lifted from generated/ by lift.py.
void f825DB220() {
  ITC_NATIVE_RUNNING("sub_825DB220 (lifted)");
  // blr
  return;
}

// sub_825DB228: lifted from generated/ by lift.py.
void f825DB228() {
  ITC_NATIVE_RUNNING("sub_825DB228 (lifted)");
  // blr
  return;
}

lifted::Result e825DACC8(const lifted::Args& a) {
  return f825DACC8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DACD8(const lifted::Args& a) {
  return f825DACD8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DACE8(const lifted::Args& a) {
  return f825DACE8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DACF8(const lifted::Args& a) {
  return f825DACF8(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DAD08(const lifted::Args& a) {
  f825DAD08();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAD38(const lifted::Args& a) {
  return {f825DAD38(a.r1, a.r13), a.f[0]};
}

lifted::Result e825DAD98(const lifted::Args& a) {
  return f825DAD98(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DADF0(const lifted::Args& a) {
  f825DADF0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DADF8(const lifted::Args& a) {
  f825DADF8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE00(const lifted::Args& a) {
  f825DAE00();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE08(const lifted::Args& a) {
  f825DAE08();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE10(const lifted::Args& a) {
  f825DAE10();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE18(const lifted::Args& a) {
  f825DAE18();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE20(const lifted::Args& a) {
  f825DAE20();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE28(const lifted::Args& a) {
  return f825DAE28(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DAE88(const lifted::Args& a) {
  f825DAE88();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE90(const lifted::Args& a) {
  f825DAE90();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAE98(const lifted::Args& a) {
  f825DAE98();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEA0(const lifted::Args& a) {
  f825DAEA0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEA8(const lifted::Args& a) {
  f825DAEA8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEB0(const lifted::Args& a) {
  f825DAEB0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEB8(const lifted::Args& a) {
  f825DAEB8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEC0(const lifted::Args& a) {
  f825DAEC0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEC8(const lifted::Args& a) {
  f825DAEC8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAED0(const lifted::Args& a) {
  f825DAED0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAED8(const lifted::Args& a) {
  f825DAED8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEE0(const lifted::Args& a) {
  f825DAEE0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEE8(const lifted::Args& a) {
  f825DAEE8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEF0(const lifted::Args& a) {
  f825DAEF0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAEF8(const lifted::Args& a) {
  f825DAEF8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF00(const lifted::Args& a) {
  f825DAF00();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF08(const lifted::Args& a) {
  f825DAF08();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF10(const lifted::Args& a) {
  f825DAF10();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF18(const lifted::Args& a) {
  return f825DAF18(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825DAF28(const lifted::Args& a) {
  f825DAF28();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF30(const lifted::Args& a) {
  f825DAF30();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF38(const lifted::Args& a) {
  f825DAF38();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF40(const lifted::Args& a) {
  f825DAF40();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF48(const lifted::Args& a) {
  f825DAF48();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF50(const lifted::Args& a) {
  f825DAF50();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF58(const lifted::Args& a) {
  f825DAF58();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF60(const lifted::Args& a) {
  f825DAF60();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF68(const lifted::Args& a) {
  f825DAF68();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF70(const lifted::Args& a) {
  f825DAF70();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF78(const lifted::Args& a) {
  f825DAF78();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF80(const lifted::Args& a) {
  f825DAF80();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF88(const lifted::Args& a) {
  f825DAF88();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF90(const lifted::Args& a) {
  f825DAF90();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAF98(const lifted::Args& a) {
  f825DAF98();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFA0(const lifted::Args& a) {
  f825DAFA0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFA8(const lifted::Args& a) {
  f825DAFA8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFB0(const lifted::Args& a) {
  f825DAFB0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFB8(const lifted::Args& a) {
  f825DAFB8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFC0(const lifted::Args& a) {
  f825DAFC0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFC8(const lifted::Args& a) {
  f825DAFC8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFD0(const lifted::Args& a) {
  f825DAFD0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFD8(const lifted::Args& a) {
  f825DAFD8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFE0(const lifted::Args& a) {
  f825DAFE0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFE8(const lifted::Args& a) {
  f825DAFE8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFF0(const lifted::Args& a) {
  f825DAFF0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DAFF8(const lifted::Args& a) {
  f825DAFF8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB000(const lifted::Args& a) {
  f825DB000();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB008(const lifted::Args& a) {
  f825DB008();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB010(const lifted::Args& a) {
  f825DB010();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB018(const lifted::Args& a) {
  f825DB018();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB020(const lifted::Args& a) {
  f825DB020();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB028(const lifted::Args& a) {
  f825DB028();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB030(const lifted::Args& a) {
  return f825DB030(a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB040(const lifted::Args& a) {
  return f825DB040(a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825DB050(const lifted::Args& a) {
  return f825DB050(a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB0A0(const lifted::Args& a) {
  return f825DB0A0(a.r1, a.r13, a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825DB0F0(const lifted::Args& a) {
  f825DB0F0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB0F8(const lifted::Args& a) {
  f825DB0F8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB100(const lifted::Args& a) {
  f825DB100();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB108(const lifted::Args& a) {
  f825DB108();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB110(const lifted::Args& a) {
  f825DB110();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB118(const lifted::Args& a) {
  f825DB118();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB120(const lifted::Args& a) {
  f825DB120();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB128(const lifted::Args& a) {
  f825DB128();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB130(const lifted::Args& a) {
  f825DB130();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB138(const lifted::Args& a) {
  f825DB138();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB140(const lifted::Args& a) {
  f825DB140();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB148(const lifted::Args& a) {
  f825DB148();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB150(const lifted::Args& a) {
  f825DB150();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB158(const lifted::Args& a) {
  f825DB158();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB160(const lifted::Args& a) {
  f825DB160();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB168(const lifted::Args& a) {
  f825DB168();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB170(const lifted::Args& a) {
  return f825DB170(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB188(const lifted::Args& a) {
  f825DB188();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB190(const lifted::Args& a) {
  f825DB190();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB198(const lifted::Args& a) {
  f825DB198();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB1A0(const lifted::Args& a) {
  f825DB1A0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB1A8(const lifted::Args& a) {
  f825DB1A8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB1B0(const lifted::Args& a) {
  return f825DB1B0(a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB1C8(const lifted::Args& a) {
  f825DB1C8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB1E0(const lifted::Args& a) {
  f825DB1E0();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB1F8(const lifted::Args& a) {
  f825DB1F8();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB210(const lifted::Args& a) {
  return {f825DB210(a.r1, a.r13, a.lr), a.f[0]};
}

lifted::Result e825DB220(const lifted::Args& a) {
  f825DB220();
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB228(const lifted::Args& a) {
  f825DB228();
  return {a.r[0], a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x825DACC8, e825DACC8},
    {0x825DACD8, e825DACD8},
    {0x825DACE8, e825DACE8},
    {0x825DACF8, e825DACF8},
    {0x825DAD08, e825DAD08},
    {0x825DAD38, e825DAD38},
    {0x825DAD98, e825DAD98},
    {0x825DADF0, e825DADF0},
    {0x825DADF8, e825DADF8},
    {0x825DAE00, e825DAE00},
    {0x825DAE08, e825DAE08},
    {0x825DAE10, e825DAE10},
    {0x825DAE18, e825DAE18},
    {0x825DAE20, e825DAE20},
    {0x825DAE28, e825DAE28},
    {0x825DAE88, e825DAE88},
    {0x825DAE90, e825DAE90},
    {0x825DAE98, e825DAE98},
    {0x825DAEA0, e825DAEA0},
    {0x825DAEA8, e825DAEA8},
    {0x825DAEB0, e825DAEB0},
    {0x825DAEB8, e825DAEB8},
    {0x825DAEC0, e825DAEC0},
    {0x825DAEC8, e825DAEC8},
    {0x825DAED0, e825DAED0},
    {0x825DAED8, e825DAED8},
    {0x825DAEE0, e825DAEE0},
    {0x825DAEE8, e825DAEE8},
    {0x825DAEF0, e825DAEF0},
    {0x825DAEF8, e825DAEF8},
    {0x825DAF00, e825DAF00},
    {0x825DAF08, e825DAF08},
    {0x825DAF10, e825DAF10},
    {0x825DAF18, e825DAF18},
    {0x825DAF28, e825DAF28},
    {0x825DAF30, e825DAF30},
    {0x825DAF38, e825DAF38},
    {0x825DAF40, e825DAF40},
    {0x825DAF48, e825DAF48},
    {0x825DAF50, e825DAF50},
    {0x825DAF58, e825DAF58},
    {0x825DAF60, e825DAF60},
    {0x825DAF68, e825DAF68},
    {0x825DAF70, e825DAF70},
    {0x825DAF78, e825DAF78},
    {0x825DAF80, e825DAF80},
    {0x825DAF88, e825DAF88},
    {0x825DAF90, e825DAF90},
    {0x825DAF98, e825DAF98},
    {0x825DAFA0, e825DAFA0},
    {0x825DAFA8, e825DAFA8},
    {0x825DAFB0, e825DAFB0},
    {0x825DAFB8, e825DAFB8},
    {0x825DAFC0, e825DAFC0},
    {0x825DAFC8, e825DAFC8},
    {0x825DAFD0, e825DAFD0},
    {0x825DAFD8, e825DAFD8},
    {0x825DAFE0, e825DAFE0},
    {0x825DAFE8, e825DAFE8},
    {0x825DAFF0, e825DAFF0},
    {0x825DAFF8, e825DAFF8},
    {0x825DB000, e825DB000},
    {0x825DB008, e825DB008},
    {0x825DB010, e825DB010},
    {0x825DB018, e825DB018},
    {0x825DB020, e825DB020},
    {0x825DB028, e825DB028},
    {0x825DB030, e825DB030},
    {0x825DB040, e825DB040},
    {0x825DB050, e825DB050},
    {0x825DB0A0, e825DB0A0},
    {0x825DB0F0, e825DB0F0},
    {0x825DB0F8, e825DB0F8},
    {0x825DB100, e825DB100},
    {0x825DB108, e825DB108},
    {0x825DB110, e825DB110},
    {0x825DB118, e825DB118},
    {0x825DB120, e825DB120},
    {0x825DB128, e825DB128},
    {0x825DB130, e825DB130},
    {0x825DB138, e825DB138},
    {0x825DB140, e825DB140},
    {0x825DB148, e825DB148},
    {0x825DB150, e825DB150},
    {0x825DB158, e825DB158},
    {0x825DB160, e825DB160},
    {0x825DB168, e825DB168},
    {0x825DB170, e825DB170},
    {0x825DB188, e825DB188},
    {0x825DB190, e825DB190},
    {0x825DB198, e825DB198},
    {0x825DB1A0, e825DB1A0},
    {0x825DB1A8, e825DB1A8},
    {0x825DB1B0, e825DB1B0},
    {0x825DB1C8, e825DB1C8},
    {0x825DB1E0, e825DB1E0},
    {0x825DB1F8, e825DB1F8},
    {0x825DB210, e825DB210},
    {0x825DB220, e825DB220},
    {0x825DB228, e825DB228},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_027)), true);

}  // namespace

}  // namespace lifted
