// Lift sweep batch 028 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_028, true, "ITC",
              "Run lift sweep batch 028's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_825DB230: lifted from generated/ by lift.py.
void f825DB230(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB230 (lifted)");
  // blr
  return;
}

// sub_825DB238: lifted from generated/ by lift.py.
void f825DB238(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB238 (lifted)");
  // blr
  return;
}

// sub_825DB240: lifted from generated/ by lift.py.
void f825DB240(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB240 (lifted)");
  // blr
  return;
}

// sub_825DB248: lifted from generated/ by lift.py.
void f825DB248(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB248 (lifted)");
  // blr
  return;
}

// sub_825DB250: lifted from generated/ by lift.py.
void f825DB250(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB250 (lifted)");
  // blr
  return;
}

// sub_825DB258: lifted from generated/ by lift.py.
void f825DB258(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB258 (lifted)");
  // blr
  return;
}

// sub_825DB260: lifted from generated/ by lift.py.
void f825DB260(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB260 (lifted)");
  // blr
  return;
}

// sub_825DB268: lifted from generated/ by lift.py.
void f825DB268(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB268 (lifted)");
  // blr
  return;
}

// sub_825DB270: lifted from generated/ by lift.py.
void f825DB270(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB270 (lifted)");
  // blr
  return;
}

// sub_825DB278: lifted from generated/ by lift.py.
void f825DB278(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB278 (lifted)");
  // blr
  return;
}

// sub_825DB280: lifted from generated/ by lift.py.
void f825DB280(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB280 (lifted)");
  // blr
  return;
}

// sub_825DB288: lifted from generated/ by lift.py.
void f825DB288(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB288 (lifted)");
  // blr
  return;
}

// sub_825DB290: lifted from generated/ by lift.py.
void f825DB290(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB290 (lifted)");
  // blr
  return;
}

// sub_825DB298: lifted from generated/ by lift.py.
void f825DB298(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB298 (lifted)");
  // blr
  return;
}

// sub_825DB2A0: lifted from generated/ by lift.py.
void f825DB2A0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB2A0 (lifted)");
  // blr
  return;
}

// sub_825DB2A8: lifted from generated/ by lift.py.
void f825DB2A8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB2A8 (lifted)");
  // blr
  return;
}

// sub_825DB2B0: lifted from generated/ by lift.py.
uint64_t f825DB2B0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DB2B0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17140
  r3 = uint64_t(int64_t(int64_t(r11) + -17140));
  // b 0x82106540
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825DB2C0: lifted from generated/ by lift.py.
lifted::Result f825DB2C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB2C0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17100
  r3 = uint64_t(int64_t(int64_t(r11) + -17100));
  // b 0x8258dd60
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB2D0: lifted from generated/ by lift.py.
uint64_t f825DB2D0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_825DB2D0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17096
  r3 = uint64_t(int64_t(int64_t(r11) + -17096));
  // b 0x82106540
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_825DB2E0: lifted from generated/ by lift.py.
lifted::Result f825DB2E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB2E0 (lifted)");
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,-17068
  r31 = uint64_t(int64_t(int64_t(r11) + -17068));
  // mr r3,r31
  r3 = r31;
  // bl 0x820acf88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820ACF88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
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

// sub_825DB328: lifted from generated/ by lift.py.
lifted::Result f825DB328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB328 (lifted)");
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
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r31,r11,-4484
  r31 = uint64_t(int64_t(int64_t(r11) + -4484));
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x825db354
  if (cr6.lt) goto loc_825DB354;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825DB354:
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // stb r11,4(r31)
  REX_STORE_U8(uint32_t(r31) + 4, uint8_t(r11));
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

// sub_825DB380: lifted from generated/ by lift.py.
lifted::Result f825DB380(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB380 (lifted)");
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
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r31,r11,-4456
  r31 = uint64_t(int64_t(int64_t(r11) + -4456));
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x825db3ac
  if (cr6.lt) goto loc_825DB3AC;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825DB3AC:
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // stb r11,4(r31)
  REX_STORE_U8(uint32_t(r31) + 4, uint8_t(r11));
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

// sub_825DB3D8: lifted from generated/ by lift.py.
void f825DB3D8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB3D8 (lifted)");
  // blr
  return;
}

// sub_825DB3F0: lifted from generated/ by lift.py.
void f825DB3F0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825DB3F0 (lifted)");
  // blr
  return;
}

// sub_825DB428: lifted from generated/ by lift.py.
lifted::Result f825DB428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825DB428 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,4224
  r3 = uint64_t(int64_t(int64_t(r11) + 4224));
  // b 0x825a6c40
  { const lifted::Result o = lifted::Call(base, 0x825A6C40, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB438: lifted from generated/ by lift.py.
lifted::Result f825DB438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB438 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, ctr = 0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r3,4260(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 4260);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825db474
  if (cr6.eq) goto loc_825DB474;
  // bl 0x82592900
  // (left out: dead in C++)
  { const lifted::Result o = f82592900(base, r1, r13, r3, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825db474
  if (cr0.eq) goto loc_825DB474;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825DB474:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825DB488: lifted from generated/ by lift.py.
lifted::Result f825DB488(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825DB488 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,4408
  r3 = uint64_t(int64_t(int64_t(r11) + 4408));
  // b 0x825a6c40
  { const lifted::Result o = lifted::Call(base, 0x825A6C40, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825DB498: lifted from generated/ by lift.py.
lifted::Result f825DB498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825DB498 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,4409
  r3 = uint64_t(int64_t(int64_t(r11) + 4409));
  // b 0x825a6d30
  { const lifted::Result o = f825A6D30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

namespace {

lifted::Result e825DB230(uint8_t* base, const lifted::Args& a) {
  f825DB230(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB238(uint8_t* base, const lifted::Args& a) {
  f825DB238(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB240(uint8_t* base, const lifted::Args& a) {
  f825DB240(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB248(uint8_t* base, const lifted::Args& a) {
  f825DB248(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB250(uint8_t* base, const lifted::Args& a) {
  f825DB250(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB258(uint8_t* base, const lifted::Args& a) {
  f825DB258(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB260(uint8_t* base, const lifted::Args& a) {
  f825DB260(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB268(uint8_t* base, const lifted::Args& a) {
  f825DB268(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB270(uint8_t* base, const lifted::Args& a) {
  f825DB270(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB278(uint8_t* base, const lifted::Args& a) {
  f825DB278(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB280(uint8_t* base, const lifted::Args& a) {
  f825DB280(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB288(uint8_t* base, const lifted::Args& a) {
  f825DB288(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB290(uint8_t* base, const lifted::Args& a) {
  f825DB290(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB298(uint8_t* base, const lifted::Args& a) {
  f825DB298(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB2A0(uint8_t* base, const lifted::Args& a) {
  f825DB2A0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB2A8(uint8_t* base, const lifted::Args& a) {
  f825DB2A8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB2B0(uint8_t* base, const lifted::Args& a) {
  return {f825DB2B0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DB2C0(uint8_t* base, const lifted::Args& a) {
  return f825DB2C0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB2D0(uint8_t* base, const lifted::Args& a) {
  return {f825DB2D0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e825DB2E0(uint8_t* base, const lifted::Args& a) {
  return f825DB2E0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB328(uint8_t* base, const lifted::Args& a) {
  return f825DB328(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB380(uint8_t* base, const lifted::Args& a) {
  return f825DB380(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB3D8(uint8_t* base, const lifted::Args& a) {
  f825DB3D8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB3F0(uint8_t* base, const lifted::Args& a) {
  f825DB3F0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825DB428(uint8_t* base, const lifted::Args& a) {
  return f825DB428(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825DB438(uint8_t* base, const lifted::Args& a) {
  return f825DB438(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825DB488(uint8_t* base, const lifted::Args& a) {
  return f825DB488(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825DB498(uint8_t* base, const lifted::Args& a) {
  return f825DB498(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

const Registration kEntries[] = {
    {0x825DB230, e825DB230},
    {0x825DB238, e825DB238},
    {0x825DB240, e825DB240},
    {0x825DB248, e825DB248},
    {0x825DB250, e825DB250},
    {0x825DB258, e825DB258},
    {0x825DB260, e825DB260},
    {0x825DB268, e825DB268},
    {0x825DB270, e825DB270},
    {0x825DB278, e825DB278},
    {0x825DB280, e825DB280},
    {0x825DB288, e825DB288},
    {0x825DB290, e825DB290},
    {0x825DB298, e825DB298},
    {0x825DB2A0, e825DB2A0},
    {0x825DB2A8, e825DB2A8},
    {0x825DB2B0, e825DB2B0},
    {0x825DB2C0, e825DB2C0},
    {0x825DB2D0, e825DB2D0},
    {0x825DB2E0, e825DB2E0},
    {0x825DB328, e825DB328},
    {0x825DB380, e825DB380},
    {0x825DB3D8, e825DB3D8},
    {0x825DB3F0, e825DB3F0},
    {0x825DB428, e825DB428},
    {0x825DB438, e825DB438},
    {0x825DB488, e825DB488},
    {0x825DB498, e825DB498},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_028)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_825DB230) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB230(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB230);
}

extern "C" REX_FUNC(sub_825DB238) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB238);
}

extern "C" REX_FUNC(sub_825DB240) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB240(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB240);
}

extern "C" REX_FUNC(sub_825DB248) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB248(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB248);
}

extern "C" REX_FUNC(sub_825DB250) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB250(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB250);
}

extern "C" REX_FUNC(sub_825DB258) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB258(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB258);
}

extern "C" REX_FUNC(sub_825DB260) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB260(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB260);
}

extern "C" REX_FUNC(sub_825DB268) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB268(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB268);
}

extern "C" REX_FUNC(sub_825DB270) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB270);
}

extern "C" REX_FUNC(sub_825DB278) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB278(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB278);
}

extern "C" REX_FUNC(sub_825DB280) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB280(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB280);
}

extern "C" REX_FUNC(sub_825DB288) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB288(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB288);
}

extern "C" REX_FUNC(sub_825DB290) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB290(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB290);
}

extern "C" REX_FUNC(sub_825DB298) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB298(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB298);
}

extern "C" REX_FUNC(sub_825DB2A0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2A0);
}

extern "C" REX_FUNC(sub_825DB2A8) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2A8);
}

extern "C" REX_FUNC(sub_825DB2B0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2B0);
}

extern "C" REX_FUNC(sub_825DB2C0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2C0);
}

extern "C" REX_FUNC(sub_825DB2D0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2D0);
}

extern "C" REX_FUNC(sub_825DB2E0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB2E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB2E0);
}

extern "C" REX_FUNC(sub_825DB328) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB328(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB328);
}

extern "C" REX_FUNC(sub_825DB380) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB380(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB380);
}

extern "C" REX_FUNC(sub_825DB3D8) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB3D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB3D8);
}

extern "C" REX_FUNC(sub_825DB3F0) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB3F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB3F0);
}

extern "C" REX_FUNC(sub_825DB428) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB428(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB428);
}

extern "C" REX_FUNC(sub_825DB438) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB438);
}

extern "C" REX_FUNC(sub_825DB488) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB488(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB488);
}

extern "C" REX_FUNC(sub_825DB498) {
  if (!ITC_FLAG(native_lifted_028)) {
    __imp__sub_825DB498(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825DB498);
}
