// Lift sweep batch 021 (docs/loops/lift-sweep.md): written by
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

ITC_FLAG_BOOL(native_lifted_021, true, "ITC",
              "Run lift sweep batch 021's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82594440: lifted from generated/ by lift.py.
lifted::Result f82594440(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82594440 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r3
  r30 = r3;
  // addi r11,r11,-13132
  r11 = uint64_t(int64_t(int64_t(r11) + -13132));
  // addi r31,r30,4
  r31 = uint64_t(int64_t(int64_t(r30) + 4));
  // mr r29,r4
  r29 = r4;
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // bl 0x82590d68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82590D68(base, LIFTED_ARGS(0x8259446C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259447c
  if (cr6.eq) goto loc_8259447C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8259447C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8259447C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // clrlwi r10,r29,31
  r10 = uint32_t(r29) & 0x1;
  // mr r3,r30
  r3 = r30;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // beq cr6,0x825944a4
  if (cr6.eq) goto loc_825944A4;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x825944A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_825944A4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825944B0: lifted from generated/ by lift.py.
lifted::Result f825944B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825944B0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r5,27
  r5 = uint64_t(int64_t(27));
  // addi r4,r11,8716
  r4 = uint64_t(int64_t(int64_t(r11) + 8716));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // stb r11,84(r1)
  REX_STORE_U8(uint32_t(r1) + 84, uint8_t(r11));
  // bl 0x82091808
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82091808(base, LIFTED_ARGS(0x825944E8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820919b8
  // (left out: dead in C++)
  { const lifted::Result o = f820919B8(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // mr r31,r3
  r31 = r3;
  // lwz r10,4104(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4104);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82594514
  if (cr6.eq) goto loc_82594514;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82594514, r31)); r3 = o.r3; f1 = o.f1; }
loc_82594514:
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82594528, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a69d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A69D8(base, LIFTED_ARGS(0x82594530, r31)); r3 = o.r3; f1 = o.f1; }
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
  // lwz r11,104(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 104);
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // blt cr6,0x82594558
  if (cr6.lt) goto loc_82594558;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82594558, r31)); r3 = o.r3; f1 = o.f1; }
loc_82594558:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825946B0: lifted from generated/ by lift.py.
lifted::Result f825946B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825946B0 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // b 0x82594570
  { const lifted::Result o = lifted::Call(base, 0x82594570, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82594FD0: lifted from generated/ by lift.py.
lifted::Result f82594FD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82594FD0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // mr r22,r3
  r22 = r3;
  // mulli r11,r6,544
  r11 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(544))));
  // lwz r10,40(r22)
  r10 = REX_LOAD_U32(uint32_t(r22) + 40);
  // add r31,r11,r10
  r31 = r11 + r10;
  // mr r25,r4
  r25 = r4;
  // mr r24,r5
  r24 = r5;
  // lwz r11,428(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 428);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82595104
  if (!cr6.eq) goto loc_82595104;
  // lwz r29,400(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 400);
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // lwz r28,404(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 404);
  // lwz r30,408(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 408);
  // addi r27,r29,16
  r27 = uint64_t(int64_t(int64_t(r29) + 16));
  // addi r26,r28,16
  r26 = uint64_t(int64_t(int64_t(r28) + 16));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // beq cr6,0x82595058
  if (cr6.eq) goto loc_82595058;
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // beq cr6,0x82595058
  if (cr6.eq) goto loc_82595058;
  // addi r3,r31,412
  r3 = uint64_t(int64_t(int64_t(r31) + 412));
  // lwz r4,12(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 12);
  // bl 0x8258a9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258A9E8, LIFTED_ARGS(0x82595038, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,416(r31)
  REX_STORE_U32(uint32_t(r31) + 416, uint32_t(r3));
  // lwz r4,12(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 12);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82595068
  if (cr6.eq) goto loc_82595068;
  // addi r3,r31,420
  r3 = uint64_t(int64_t(int64_t(r31) + 420));
  // bl 0x8258a9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258A9E8, LIFTED_ARGS(0x82595050, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,424(r31)
  REX_STORE_U32(uint32_t(r31) + 424, uint32_t(r3));
  // b 0x8259506c
  goto loc_8259506C;
loc_82595058:
  // addi r3,r31,412
  r3 = uint64_t(int64_t(int64_t(r31) + 412));
  // lwz r4,12(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 12);
  // bl 0x8258a9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258A9E8, LIFTED_ARGS(0x82595064, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,416(r31)
  REX_STORE_U32(uint32_t(r31) + 416, uint32_t(r3));
loc_82595068:
  // stw r23,424(r31)
  REX_STORE_U32(uint32_t(r31) + 424, uint32_t(r23));
loc_8259506C:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r3,-18160(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -18160);
  // lwz r11,12(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 12);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,16(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 16);
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lwz r11,416(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 416);
  // stw r26,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r26));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // lwz r11,424(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 424);
  // stw r25,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r25));
  // stw r24,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r24));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // beq cr6,0x825950f8
  if (cr6.eq) goto loc_825950F8;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825950E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r22
  r3 = r22;
  // stw r4,20(r30)
  REX_STORE_U32(uint32_t(r30) + 20, uint32_t(r4));
  // bl 0x82594e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82594E88, LIFTED_ARGS(0x825950F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825950F8:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258bcd8
  // (left out: dead in C++)
  r3 = f8258BCD8(base, r1, r13, r3);
  // stw r23,20(r30)
  REX_STORE_U32(uint32_t(r30) + 20, uint32_t(r23));
loc_82595104:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825968D0: lifted from generated/ by lift.py.
lifted::Result f825968D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825968D0 (lifted)");
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
  // bl 0x82594f58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82594F58(base, LIFTED_ARGS(0x825968F0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82596908
  if (cr6.eq) goto loc_82596908;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82596904, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82596908:
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

// sub_82596E88: lifted from generated/ by lift.py.
uint64_t f82596E88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82596E88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r4
  r28 = r4;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82596ef4
  if (cr6.eq) goto loc_82596EF4;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82596ef4
  if (cr6.eq) goto loc_82596EF4;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82596ef4
  if (!cr6.gt) goto loc_82596EF4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82596EC4:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // mr r3,r28
  r3 = r28;
  // add r11,r30,r11
  r11 = r30 + r11;
  // lwz r4,12(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 12);
  // bl 0x825aa100
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825AA100(base, LIFTED_ARGS(0x82596ED8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x82596f00
  if (cr6.eq) goto loc_82596F00;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,16
  r30 = uint64_t(int64_t(int64_t(r30) + 16));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x82596ec4
  if (cr6.lt) goto loc_82596EC4;
loc_82596EF4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
loc_82596F00:
  // lwz r9,16(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 16);
  // rlwinm r11,r29,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 4) & 0xFFFFFFF0;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r3,r11,r10
  r3 = r11 + r10;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82599BE0: lifted from generated/ by lift.py.
lifted::Result f82599BE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82599BE0 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,-12720
  r11 = uint64_t(int64_t(int64_t(r11) + -12720));
  // lwz r3,176(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 176);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x82599c18
  if (cr6.eq) goto loc_82599C18;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x82599C18, r31)); r3 = o.r3; f1 = o.f1; }
loc_82599C18:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,176(r31)
  REX_STORE_U32(uint32_t(r31) + 176, uint32_t(r11));
  // bl 0x8259a9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259A9D0(base, LIFTED_ARGS(0x82599C28, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82599c40
  if (cr6.eq) goto loc_82599C40;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82599C3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82599C40:
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

// sub_8259A2B0: lifted from generated/ by lift.py.
uint64_t f8259A2B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8259A2B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-64(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f30));
  // stfd f31,-56(r1)
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,80(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 80);
  // lwz r7,148(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 148);
  // mr r6,r11
  r6 = r11;
  // lwz r9,84(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 84);
  // lwz r30,144(r29)
  r30 = REX_LOAD_U32(uint32_t(r29) + 144);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r8,r11,6,0,25
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
  // lwz r11,12(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 12);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r31,r8,r7
  r31 = r8 + r7;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // add r11,r11,r9
  r11 = r11 + r9;
  // ble cr6,0x8259a3ec
  if (!cr6.gt) goto loc_8259A3EC;
  // addi r28,r11,128
  r28 = uint64_t(int64_t(int64_t(r11) + 128));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f30,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f30 = double(temp.f32);
loc_8259A318:
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // rlwinm r11,r11,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x100;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8259a3cc
  if (cr6.eq) goto loc_8259A3CC;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x8259a3cc
  if (cr6.eq) goto loc_8259A3CC;
  // lwz r10,148(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 148);
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
  // mr r3,r31
  r3 = r31;
  // lfs f31,12(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 12);
  f31 = double(temp.f32);
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820A0AC0(base, LIFTED_ARGS(0x8259A34C, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f12,4(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f12 = double(temp.f32);
  // lfs f13,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f13 = double(temp.f32);
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // lfs f11,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f11 = double(temp.f32);
  // lfs f12,8(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8);
  f12 = double(temp.f32);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // lfs f0,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f0 = double(temp.f32);
  // lfs f11,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f11 = double(temp.f32);
  // fsubs f11,f11,f0
  f11 = double(float(f11 - f0));
  // fmuls f10,f13,f13
  f10 = double(float(f13 * f13));
  // fmadds f0,f12,f12,f10
  f0 = double(float(std::fma(f12, f12, f10)));
  // fmadds f0,f11,f11,f0
  f0 = double(float(std::fma(f11, f11, f0)));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8259a3b4
  if (cr6.lt) goto loc_8259A3B4;
  // fdivs f0,f30,f0
  f0 = double(float(f30 / f0));
  // lfs f10,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f10 = double(temp.f32);
  // lfs f9,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f9 = double(temp.f32);
  // lfs f8,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f8 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fnmsubs f11,f11,f0,f10
  f11 = double(float(-std::fma(f11, f0, -f10)));
  // stfs f11,0(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // fnmsubs f13,f13,f0,f9
  f13 = double(float(-std::fma(f13, f0, -f9)));
  // stfs f13,4(r30)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + 4, temp.u32);
  // fnmsubs f0,f12,f0,f8
  f0 = double(float(-std::fma(f12, f0, -f8)));
  // stfs f0,8(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 8, temp.u32);
loc_8259A3B4:
  // lfs f0,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f13 = double(temp.f32);
  // lfs f12,8(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 8);
  f12 = double(temp.f32);
  // stfs f0,48(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stfs f13,52(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // stfs f12,56(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 56, temp.u32);
loc_8259A3CC:
  // lwz r11,80(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 80);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r30,r30,64
  r30 = uint64_t(int64_t(int64_t(r30) + 64));
  // addi r28,r28,160
  r28 = uint64_t(int64_t(int64_t(r28) + 160));
  // addi r31,r31,64
  r31 = uint64_t(int64_t(int64_t(r31) + 64));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpw cr6,r27,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r11), so);
  // blt cr6,0x8259a318
  if (cr6.lt) goto loc_8259A318;
loc_8259A3EC:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f30,-64(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // lfd f31,-56(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_8259B600: lifted from generated/ by lift.py.
lifted::Result f8259B600(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259B600 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-12704
  r11 = uint64_t(int64_t(int64_t(r11) + -12704));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8259a228
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259A228(base, LIFTED_ARGS(0x8259B62C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8259b644
  if (cr6.eq) goto loc_8259B644;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8259B640, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8259B644:
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

// sub_8259BB98: lifted from generated/ by lift.py.
lifted::Result f8259BB98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8259BB98 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-64
  r12 = uint64_t(int64_t(int64_t(r1) + -64));
  // bl 0x825a7904
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // lfs f28,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f28 = double(temp.f32);
  // bne cr6,0x8259bbd0
  if (!cr6.eq) goto loc_8259BBD0;
  // fmr f23,f28
  f23 = f28;
  // fmr f24,f28
  f24 = f28;
  // fmr f25,f28
  f25 = f28;
  // b 0x8259bbf0
  goto loc_8259BBF0;
loc_8259BBD0:
  // ld r10,0(r4)
  r10 = REX_LOAD_U64(uint32_t(r4) + 0);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r9,8(r4)
  r9 = REX_LOAD_U64(uint32_t(r4) + 8);
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // std r9,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r9);
  // lfs f25,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f25 = double(temp.f32);
  // lfs f24,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f24 = double(temp.f32);
  // lfs f23,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f23 = double(temp.f32);
loc_8259BBF0:
  // lwz r11,80(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 80);
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lwz r7,148(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 148);
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // mr r6,r11
  r6 = r11;
  // lwz r9,84(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 84);
  // lwz r28,144(r30)
  r28 = REX_LOAD_U32(uint32_t(r30) + 144);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r8,r11,6,0,25
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
  // lwz r11,12(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 12);
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r29,r8,r7
  r29 = r8 + r7;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // add r11,r11,r9
  r11 = r11 + r9;
  // ble cr6,0x8259bde8
  if (!cr6.gt) goto loc_8259BDE8;
  // addi r27,r11,128
  r27 = uint64_t(int64_t(int64_t(r11) + 128));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r31,r28,36
  r31 = uint64_t(int64_t(int64_t(r28) + 36));
  // lfs f30,4208(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4208);
  f30 = double(temp.f32);
  // lfs f31,8708(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8708);
  f31 = double(temp.f32);
  // lfd f26,10840(r10)
  f26 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r10) + 10840)));
  // lfs f27,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f27 = double(temp.f32);
loc_8259BC60:
  // lwz r11,4(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 4);
  // rlwinm r11,r11,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x100;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8259bdc4
  if (cr6.eq) goto loc_8259BDC4;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x8259bdc4
  if (cr6.eq) goto loc_8259BDC4;
  // lwz r10,148(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 148);
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
  // mr r3,r29
  r3 = r29;
  // lfs f29,12(r27)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 12);
  f29 = double(temp.f32);
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x820a0ac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820A0AC0(base, LIFTED_ARGS(0x8259BC94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,152(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 152);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8259bcdc
  if (!cr6.eq) goto loc_8259BCDC;
  // lfs f0,48(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 48);
  f0 = double(temp.f32);
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lfs f0,52(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 52);
  f0 = double(temp.f32);
  // lfs f13,56(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 56);
  f13 = double(temp.f32);
  // fsubs f0,f0,f29
  f0 = double(float(f0 - f29));
  // stfs f0,-32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + -32, temp.u32);
  // stfs f13,-28(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + -28, temp.u32);
  // stfs f27,-24(r31)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r31) + -24, temp.u32);
  // stfs f28,-4(r31)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r31) + -4, temp.u32);
  // stfs f28,0(r31)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // stfs f28,4(r31)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // stfs f27,8(r31)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // lfs f0,-32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -32);
  f0 = double(temp.f32);
  // stfs f0,52(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 52, temp.u32);
  // b 0x8259bdc4
  goto loc_8259BDC4;
loc_8259BCDC:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8259bd14
  if (!cr6.eq) goto loc_8259BD14;
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // fadds f0,f0,f23
  f0 = double(float(f0 + f23));
  // stfs f0,0(r28)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // lfs f0,-32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -32);
  f0 = double(temp.f32);
  // fadds f0,f24,f0
  f0 = double(float(f24 + f0));
  // lfs f13,-28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -28);
  f13 = double(temp.f32);
  // fadds f13,f13,f25
  f13 = double(float(f13 + f25));
  // stfs f13,-28(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + -28, temp.u32);
  // fsub f0,f0,f26
  f0 = f0 - f26;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,-32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + -32, temp.u32);
loc_8259BD14:
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f11,-32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -32);
  f11 = double(temp.f32);
  // lfs f8,52(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 52);
  f8 = double(temp.f32);
  // fmadds f11,f0,f31,f11
  f11 = double(float(std::fma(f0, f31, f11)));
  // fsubs f8,f8,f29
  f8 = double(float(f8 - f29));
  // lfs f13,-4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -4);
  f13 = double(temp.f32);
  // lfs f10,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f10 = double(temp.f32);
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // fmadds f10,f13,f31,f10
  f10 = double(float(std::fma(f13, f31, f10)));
  // lfs f9,-28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -28);
  f9 = double(temp.f32);
  // fmadds f9,f12,f31,f9
  f9 = double(float(std::fma(f12, f31, f9)));
  // lfs f7,48(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 48);
  f7 = double(temp.f32);
  // lfs f6,56(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 56);
  f6 = double(temp.f32);
  // stfs f11,-32(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + -32, temp.u32);
  // stfs f10,0(r28)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // stfs f9,-28(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + -28, temp.u32);
  // fsubs f8,f8,f11
  f8 = double(float(f8 - f11));
  // fsubs f11,f6,f9
  f11 = double(float(f6 - f9));
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // fsubs f0,f7,f10
  f0 = double(float(f7 - f10));
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,-4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + -4, temp.u32);
  // fadds f0,f11,f12
  f0 = double(float(f11 + f12));
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // lfs f0,-4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -4);
  f0 = double(temp.f32);
  // lfs f13,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // fmuls f13,f13,f30
  f13 = double(float(f13 * f30));
  // fmuls f12,f12,f30
  f12 = double(float(f12 * f30));
  // stfs f0,-4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + -4, temp.u32);
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // stfs f12,4(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lfs f0,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // lfs f13,-32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -32);
  f13 = double(temp.f32);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lfs f12,-28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + -28);
  f12 = double(temp.f32);
  // stfs f0,48(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 48, temp.u32);
  // stfs f13,52(r29)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r29) + 52, temp.u32);
  // stfs f12,56(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 56, temp.u32);
  // bne cr6,0x8259bdc4
  if (!cr6.eq) goto loc_8259BDC4;
  // stw r25,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r25));
loc_8259BDC4:
  // lwz r11,80(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 80);
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r28,r28,64
  r28 = uint64_t(int64_t(int64_t(r28) + 64));
  // addi r31,r31,64
  r31 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r27,r27,160
  r27 = uint64_t(int64_t(int64_t(r27) + 160));
  // addi r29,r29,64
  r29 = uint64_t(int64_t(int64_t(r29) + 64));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpw cr6,r26,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r11), so);
  // blt cr6,0x8259bc60
  if (cr6.lt) goto loc_8259BC60;
loc_8259BDE8:
  // mr r3,r30
  r3 = r30;
  // bl 0x8259a2b0
  // (left out: dead in C++)
  r3 = f8259A2B0(base, r1, r13, r3);
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259BE04, r31)); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259BE18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8259a2b0
  // (left out: dead in C++)
  r3 = f8259A2B0(base, r1, r13, r3);
  // mr r3,r30
  r3 = r30;
  // bl 0x8259a400
  // (left out: dead in C++)
  f8259A400(base, r1, r3);
  // bl 0x8259b660
  // (left out: dead in C++)
  { const lifted::Result o = f8259B660(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // stw r25,152(r30)
  REX_STORE_U32(uint32_t(r30) + 152, uint32_t(r25));
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // addi r12,r1,-64
  r12 = uint64_t(int64_t(int64_t(r1) + -64));
  // bl 0x825a7950
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8259C550: lifted from generated/ by lift.py.
uint64_t f8259C550(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8259C550 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8259c5b0
  if (!cr6.eq) goto loc_8259C5B0;
  // cmplwi cr6,r4,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 128, so);
  // bge cr6,0x8259c588
  if (!cr6.lt) goto loc_8259C588;
  // cmplwi cr6,r4,92
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 92, so);
  // bne cr6,0x8259c574
  if (!cr6.eq) goto loc_8259C574;
  // li r4,165
  r4 = uint64_t(int64_t(165));
  // clrlwi r3,r4,16
  r3 = uint32_t(r4) & 0xFFFF;
  // blr
  return r3;
loc_8259C574:
  // cmplwi cr6,r4,126
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 126, so);
  // bne cr6,0x8259c5a8
  if (!cr6.eq) goto loc_8259C5A8;
  // li r4,8254
  r4 = uint64_t(int64_t(8254));
  // clrlwi r3,r4,16
  r3 = uint32_t(r4) & 0xFFFF;
  // blr
  return r3;
loc_8259C588:
  // addi r11,r4,-161
  r11 = uint64_t(int64_t(int64_t(r4) + -161));
  // cmplwi cr6,r11,62
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 62, so);
  // bgt cr6,0x8259c5a4
  if (cr6.gt) goto loc_8259C5A4;
  // addis r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 65536));
  // addi r4,r4,-320
  r4 = uint64_t(int64_t(int64_t(r4) + -320));
  // clrlwi r3,r4,16
  r3 = uint32_t(r4) & 0xFFFF;
  // blr
  return r3;
loc_8259C5A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_8259C5A8:
  // clrlwi r3,r4,16
  r3 = uint32_t(r4) & 0xFFFF;
  // blr
  return r3;
loc_8259C5B0:
  // cmplwi cr6,r3,129
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 129, so);
  // blt cr6,0x8259c5c0
  if (cr6.lt) goto loc_8259C5C0;
  // cmplwi cr6,r3,159
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 159, so);
  // ble cr6,0x8259c5cc
  if (!cr6.gt) goto loc_8259C5CC;
loc_8259C5C0:
  // addi r11,r3,-224
  r11 = uint64_t(int64_t(int64_t(r3) + -224));
  // cmplwi cr6,r11,28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 28, so);
  // bgt cr6,0x8259c6b8
  if (cr6.gt) goto loc_8259C6B8;
loc_8259C5CC:
  // cmplwi cr6,r4,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 64, so);
  // blt cr6,0x8259c6b8
  if (cr6.lt) goto loc_8259C6B8;
  // cmplwi cr6,r4,127
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 127, so);
  // beq cr6,0x8259c6b8
  if (cr6.eq) goto loc_8259C6B8;
  // cmplwi cr6,r4,252
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 252, so);
  // bgt cr6,0x8259c6b8
  if (cr6.gt) goto loc_8259C6B8;
  // cmplwi cr6,r3,129
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 129, so);
  // blt cr6,0x8259c5f4
  if (cr6.lt) goto loc_8259C5F4;
  // cmplwi cr6,r3,159
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 159, so);
  // ble cr6,0x8259c600
  if (!cr6.gt) goto loc_8259C600;
loc_8259C5F4:
  // addi r11,r3,-224
  r11 = uint64_t(int64_t(int64_t(r3) + -224));
  // cmplwi cr6,r11,15
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 15, so);
  // bgt cr6,0x8259c664
  if (cr6.gt) goto loc_8259C664;
loc_8259C600:
  // cmplwi cr6,r4,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 64, so);
  // blt cr6,0x8259c664
  if (cr6.lt) goto loc_8259C664;
  // subfic r11,r3,159
  ca = uint32_t(r3) <= 159;
  r11 = static_cast<uint64_t>(159) - r3;
  // cmplwi cr6,r4,159
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 159, so);
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r10,r3,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r11,r11,0,24,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80;
  // bge cr6,0x8259c648
  if (!cr6.lt) goto loc_8259C648;
  // subfic r9,r4,127
  ca = uint32_t(r4) <= 127;
  r9 = static_cast<uint64_t>(127) - r4;
  // addi r8,r11,225
  r8 = uint64_t(int64_t(int64_t(r11) + 225));
  // subfe r9,r9,r9
  temp.u8 = (~uint32_t(r9) + uint32_t(r9) < ~uint32_t(r9)) | (~uint32_t(r9) + uint32_t(r9) + ca < ca);
  r9 = ~r9 + r9 + ca;
  ca = temp.u8;
  // subf r10,r8,r10
  r10 = r10 - r8;
  // clrlwi r11,r9,31
  r11 = uint32_t(r9) & 0x1;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // addi r11,r11,31
  r11 = uint64_t(int64_t(int64_t(r11) + 31));
  // subf r11,r11,r4
  r11 = r4 - r11;
  // or r11,r10,r11
  r11 = r10 | r11;
  // b 0x8259c668
  goto loc_8259C668;
loc_8259C648:
  // addi r11,r11,225
  r11 = uint64_t(int64_t(int64_t(r11) + 225));
  // addi r9,r4,-126
  r9 = uint64_t(int64_t(int64_t(r4) + -126));
  // subf r11,r11,r10
  r11 = r10 - r11;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // or r11,r11,r9
  r11 = r11 | r9;
  // b 0x8259c668
  goto loc_8259C668;
loc_8259C664:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8259C668:
  // rlwinm r10,r11,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFF;
  // clrlwi r9,r11,24
  r9 = uint32_t(r11) & 0xFF;
  // addi r11,r10,-33
  r11 = uint64_t(int64_t(int64_t(r10) + -33));
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // bgt cr6,0x8259c6ac
  if (cr6.gt) goto loc_8259C6AC;
  // addi r11,r9,-33
  r11 = uint64_t(int64_t(int64_t(r9) + -33));
  // cmplwi cr6,r11,93
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 93, so);
  // bgt cr6,0x8259c6ac
  if (cr6.gt) goto loc_8259C6AC;
  // mulli r11,r10,94
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(94))));
  // add r10,r11,r9
  r10 = r11 + r9;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r10,r10,-3135
  r10 = uint64_t(int64_t(int64_t(r10) + -3135));
  // addi r11,r11,-12680
  r11 = uint64_t(int64_t(int64_t(r11) + -12680));
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // lhzx r11,r10,r11
  r11 = REX_LOAD_U16(uint32_t(r10) + uint32_t(r11));
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // blr
  return r3;
loc_8259C6AC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // blr
  return r3;
loc_8259C6B8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_8259C6C0: lifted from generated/ by lift.py.
uint64_t f8259C6C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8259C6C0 (lifted)");
  uint64_t r4 = 0;
  double f1 = 0.0;
  // clrlwi r4,r3,24
  r4 = uint32_t(r3) & 0xFF;
  // rlwinm r3,r3,24,24,31
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 24) & 0xFF;
  // b 0x8259c550
  r3 = f8259C550(base, r3, r4);
  return r3;
}

// sub_8259C6F0: lifted from generated/ by lift.py.
void f8259C6F0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8259C6F0 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,4992
  r11 = uint64_t(int64_t(int64_t(r11) + 4992));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_8259C700: lifted from generated/ by lift.py.
lifted::Result f8259C700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259C700 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259C72C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8259c750
  if (cr0.eq) goto loc_8259C750;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259C74C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8259c754
  goto loc_8259C754;
loc_8259C750:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8259C754:
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

// sub_8259C770: lifted from generated/ by lift.py.
lifted::Result f8259C770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259C770 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
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
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259C78C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8259c7a0
  if (cr0.eq) goto loc_8259C7A0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f1 = double(temp.f32);
  // b 0x8259c7a8
  goto loc_8259C7A8;
loc_8259C7A0:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
loc_8259C7A8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8259C7F8: lifted from generated/ by lift.py.
void f8259C7F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8259C7F8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,5056
  r11 = uint64_t(int64_t(int64_t(r11) + 5056));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,3092(r10)
  REX_STORE_U32(uint32_t(r10) + 3092, uint32_t(r11));
  // blr
  return;
}

// sub_8259C8C8: lifted from generated/ by lift.py.
lifted::Result f8259C8C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8259C8C8 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,4992
  r11 = uint64_t(int64_t(int64_t(r11) + 4992));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x8259c8f4
  if (cr0.eq) goto loc_8259C8F4;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8259C8F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8259C8F4:
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

// sub_8259C910: lifted from generated/ by lift.py.
lifted::Result f8259C910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8259C910 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,5056
  r11 = uint64_t(int64_t(int64_t(r11) + 5056));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,3092(r10)
  REX_STORE_U32(uint32_t(r10) + 3092, uint32_t(r11));
  // beq 0x8259c948
  if (cr0.eq) goto loc_8259C948;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8259C948, r31)); r3 = o.r3; f1 = o.f1; }
loc_8259C948:
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

// sub_8259CB48: lifted from generated/ by lift.py.
lifted::Result f8259CB48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259CB48 (lifted)");
  uint64_t r6 = 0, r7 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,8(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 8);
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8259cbd0
  if (cr6.eq) goto loc_8259CBD0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8259cbd0
  if (!cr6.eq) goto loc_8259CBD0;
  // lwz r3,8(r4)
  r3 = REX_LOAD_U32(uint32_t(r4) + 8);
  // mr r7,r31
  r7 = r31;
  // lhz r4,0(r5)
  r4 = REX_LOAD_U16(uint32_t(r5) + 0);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CB98, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8259cbd0
  if (!cr6.eq) goto loc_8259CBD0;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r4,r31,136
  r4 = uint64_t(int64_t(int64_t(r31) + 136));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CBB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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
loc_8259CBD0:
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

// sub_8259CBE8: lifted from generated/ by lift.py.
lifted::Result f8259CBE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259CBE8 (lifted)");
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
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259cc20
  if (cr6.eq) goto loc_8259CC20;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CC18, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8259CC20:
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

// sub_8259CC38: lifted from generated/ by lift.py.
lifted::Result f8259CC38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259CC38 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
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

// sub_8259CC58: lifted from generated/ by lift.py.
lifted::Result f8259CC58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259CC58 (lifted)");
  uint64_t r4 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259CCA0: lifted from generated/ by lift.py.
lifted::Result f8259CCA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259CCA0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259cce0
  if (cr6.eq) goto loc_8259CCE0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CCCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8259cce4
  if (!cr6.eq) goto loc_8259CCE4;
loc_8259CCE0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8259CCE4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8259CCF8: lifted from generated/ by lift.py.
lifted::Result f8259CCF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259CCF8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259cd38
  if (cr6.eq) goto loc_8259CD38;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CD24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x8259cd3c
  if (cr6.eq) goto loc_8259CD3C;
loc_8259CD38:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8259CD3C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8259CE78: lifted from generated/ by lift.py.
lifted::Result f8259CE78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259CE78 (lifted)");
  uint64_t r7 = 0, r8 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r4
  r11 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r5,r6
  r5 = r6;
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r4,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r4));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CEBC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x8259cef8
  if (!cr6.lt) goto loc_8259CEF8;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r5,279
  r5 = uint64_t(int64_t(279));
  // addi r4,r11,5252
  r4 = uint64_t(int64_t(int64_t(r11) + 5252));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,5288
  r3 = uint64_t(int64_t(int64_t(r11) + 5288));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
loc_8259CEF8:
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

// sub_8259CF10: lifted from generated/ by lift.py.
lifted::Result f8259CF10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259CF10 (lifted)");
  uint64_t r7 = 0, r8 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r4
  r11 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r5,r6
  r5 = r6;
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r4,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r4));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CF54, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x8259cf90
  if (!cr6.lt) goto loc_8259CF90;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r5,290
  r5 = uint64_t(int64_t(290));
  // addi r4,r11,5252
  r4 = uint64_t(int64_t(int64_t(r11) + 5252));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,5332
  r3 = uint64_t(int64_t(int64_t(r11) + 5332));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
loc_8259CF90:
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

// sub_8259CFA8: lifted from generated/ by lift.py.
lifted::Result f8259CFA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259CFA8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r4
  r11 = r4;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r4,0(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259CFE4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x8259d020
  if (!cr6.lt) goto loc_8259D020;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r5,300
  r5 = uint64_t(int64_t(300));
  // addi r4,r11,5252
  r4 = uint64_t(int64_t(int64_t(r11) + 5252));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,5288
  r3 = uint64_t(int64_t(int64_t(r11) + 5288));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
loc_8259D020:
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

// sub_8259D038: lifted from generated/ by lift.py.
lifted::Result f8259D038(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259D038 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r11,r4
  r11 = r4;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r4,0(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D074, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x8259d0b0
  if (!cr6.lt) goto loc_8259D0B0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r5,308
  r5 = uint64_t(int64_t(308));
  // addi r4,r11,5252
  r4 = uint64_t(int64_t(int64_t(r11) + 5252));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,5332
  r3 = uint64_t(int64_t(int64_t(r11) + 5332));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
loc_8259D0B0:
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

// sub_8259D0C8: lifted from generated/ by lift.py.
lifted::Result f8259D0C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8259D0C8 (lifted)");
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
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D0E8, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8259D100: lifted from generated/ by lift.py.
lifted::Result f8259D100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259D100 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lfs f1,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f1 = double(temp.f32);
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // clrlwi r4,r11,16
  r4 = uint32_t(r11) & 0xFFFF;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259D120: lifted from generated/ by lift.py.
lifted::Result f8259D120(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259D120 (lifted)");
  uint64_t r5 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // clrlwi r4,r11,16
  r4 = uint32_t(r11) & 0xFFFF;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259D140: lifted from generated/ by lift.py.
lifted::Result f8259D140(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259D140 (lifted)");
  uint64_t r5 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // clrlwi r4,r11,16
  r4 = uint32_t(r11) & 0xFFFF;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259D160: lifted from generated/ by lift.py.
lifted::Result f8259D160(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8259D160 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r3,r4
  r3 = r4;
  // mr r30,r5
  r30 = r5;
  // bl 0x820988b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820988B8(base, LIFTED_ARGS(0x8259D17C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r3,r30
  r3 = r30;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8259D188, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r7,r31,28
  r7 = uint64_t(int64_t(int64_t(r31) + 28));
  // li r6,61
  r6 = uint64_t(int64_t(61));
  // mr r5,r29
  r5 = r29;
  // addi r3,r31,12
  r3 = uint64_t(int64_t(int64_t(r31) + 12));
  // bl 0x82481880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82481880, LIFTED_ARGS(0x8259D1A0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8259D1C0: lifted from generated/ by lift.py.
lifted::Result f8259D1C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259D1C0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r4,r3
  r4 = r3;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,5(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 5);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r3,r11
  r3 = r11;
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8259D1F0: lifted from generated/ by lift.py.
lifted::Result f8259D1F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259D1F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // addi r28,r31,8
  r28 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r4,r28
  r4 = r28;
  // bl 0x825bf5c8
  // (left out: dead in C++)
  { const lifted::Result o = f825BF5C8(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r3
  r30 = r3;
  // addi r26,r11,5368
  r26 = uint64_t(int64_t(int64_t(r11) + 5368));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r25,r11,-16324
  r25 = uint64_t(int64_t(int64_t(r11) + -16324));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r24,r11,5252
  r24 = uint64_t(int64_t(int64_t(r11) + 5252));
  // bge cr6,0x8259d260
  if (!cr6.lt) goto loc_8259D260;
  // li r5,206
  r5 = uint64_t(int64_t(206));
  // mr r4,r24
  r4 = r24;
  // mr r3,r25
  r3 = r25;
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r27
  r11 = r27;
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
loc_8259D260:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8259d2b0
  if (cr6.eq) goto loc_8259D2B0;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D27C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x8259D284, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r4,r3
  r4 = r3;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // stw r4,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r4));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D2AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8259d2b8
  goto loc_8259D2B8;
loc_8259D2B0:
  // stw r27,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r27));
  // stw r27,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r27));
loc_8259D2B8:
  // stw r27,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r27));
  // addi r11,r1,100
  r11 = uint64_t(int64_t(int64_t(r1) + 100));
  // mr r10,r27
  r10 = r27;
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // mtctr r9
  ctr = r9;
loc_8259D2CC:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x8259d2cc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8259D2CC;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lis r11,-32166
  r11 = uint64_t(int64_t(-2108030976));
  // lwz r3,0(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 0);
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r11,r11,-11840
  r11 = uint64_t(int64_t(int64_t(r11) + -11840));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D30C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x8259d33c
  if (!cr6.lt) goto loc_8259D33C;
  // li r5,228
  r5 = uint64_t(int64_t(228));
  // mr r4,r24
  r4 = r24;
  // mr r3,r25
  r3 = r25;
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r27
  r11 = r27;
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
loc_8259D33C:
  // addi r11,r31,76
  r11 = uint64_t(int64_t(int64_t(r31) + 76));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // lis r9,16256
  r9 = uint64_t(int64_t(1065353216));
  // stw r11,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r11));
  // stw r8,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r8));
  // stw r10,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r10));
  // mtctr r10
  ctr = r10;
loc_8259D35C:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x8259d35c
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8259D35C;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8259D370: lifted from generated/ by lift.py.
lifted::Result f8259D370(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8259D370 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259d3a0
  if (cr6.eq) goto loc_8259D3A0;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8259D39C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
loc_8259D3A0:
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8259d3c0
  if (cr6.eq) goto loc_8259D3C0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259D3BC, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_8259D3C0:
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

// sub_8259D3F0: lifted from generated/ by lift.py.
lifted::Result f8259D3F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8259D3F0 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8259d410
  if (cr6.eq) goto loc_8259D410;
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8259D410:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // blr
  return {r3, f1};
}

// sub_8259D5B8: lifted from generated/ by lift.py.
void f8259D5B8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8259D5B8 (lifted)");
  double f0 = 0.0;
  lifted::Temp temp{};
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // stfs f0,52(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // blr
  return;
}

// sub_825A1318: lifted from generated/ by lift.py.
lifted::Result f825A1318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A1318 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r4,24(r1)
  REX_STORE_U64(uint32_t(r1) + 24, r4);
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
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-4208(r1)
  ea = -4208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r10,r1,4232
  r10 = uint64_t(int64_t(int64_t(r1) + 4232));
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r5,80(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825a71b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71B0(base, LIFTED_ARGS(0x825A1360, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296270(base, LIFTED_ARGS(0x825A1368, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,4208
  r1 = uint64_t(int64_t(int64_t(r1) + 4208));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825A13C0: lifted from generated/ by lift.py.
lifted::Result f825A13C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825A13C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,8660
  r10 = uint64_t(int64_t(int64_t(r10) + 8660));
  // lwz r3,4(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x825a70e8
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A13D8: lifted from generated/ by lift.py.
lifted::Result f825A13D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A13D8 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8660
  r11 = uint64_t(int64_t(int64_t(r11) + 8660));
  // mr r30,r4
  r30 = r4;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x825A1408, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825a1420
  if (cr6.eq) goto loc_825A1420;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x825A141C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825A1420:
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

// sub_825A1438: lifted from generated/ by lift.py.
lifted::Result f825A1438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825A1438 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // lis r10,-32153
  r10 = uint64_t(int64_t(-2107179008));
  // addi r3,r10,-19732
  r3 = uint64_t(int64_t(int64_t(r10) + -19732));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // bl 0x822a8980
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8980(base, LIFTED_ARGS(0x825A1458, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // bne cr6,0x825a1470
  if (!cr6.eq) goto loc_825A1470;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,8684
  r3 = uint64_t(int64_t(int64_t(r11) + 8684));
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296270(base, LIFTED_ARGS(0x825A1470, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A1470:
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r29,r11,3884
  r29 = uint64_t(int64_t(int64_t(r11) + 3884));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r31,r29,8
  r31 = uint64_t(int64_t(int64_t(r29) + 8));
  // addi r28,r11,7116
  r28 = uint64_t(int64_t(int64_t(r11) + 7116));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r30,r11,7124
  r30 = uint64_t(int64_t(int64_t(r11) + 7124));
loc_825A1490:
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r4,-4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + -4);
  // addi r10,r1,100
  r10 = uint64_t(int64_t(int64_t(r1) + 100));
  // lwz r3,-8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -8);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // mr r8,r30
  r8 = r30;
  // mr r7,r28
  r7 = r28;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x822b8f10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822B8F10(base, LIFTED_ARGS(0x825A14C0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825a14fc
  if (!cr6.lt) goto loc_825A14FC;
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a14fc
  if (cr6.eq) goto loc_825A14FC;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A14E4, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296270(base, LIFTED_ARGS(0x825A14E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A14FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A14FC:
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A1510, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x822aa018
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AA018(base, LIFTED_ARGS(0x825A1514, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A1530, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,12
  r31 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r29,32
  r11 = uint64_t(int64_t(int64_t(r29) + 32));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a1490
  if (cr6.lt) goto loc_825A1490;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,3908
  r30 = uint64_t(int64_t(int64_t(r11) + 3908));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r29,r11,7108
  r29 = uint64_t(int64_t(int64_t(r11) + 7108));
loc_825A1554:
  // addi r11,r1,100
  r11 = uint64_t(int64_t(int64_t(r1) + 100));
  // lwz r4,-4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + -4);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,-8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -8);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // mr r8,r29
  r8 = r29;
  // mr r7,r28
  r7 = r28;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x822b8f10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822B8F10(base, LIFTED_ARGS(0x825A1584, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825a15c0
  if (!cr6.lt) goto loc_825A15C0;
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a15c0
  if (cr6.eq) goto loc_825A15C0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A15A8, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x82296270
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296270(base, LIFTED_ARGS(0x825A15AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A15C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A15C0:
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A15D4, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x822a9ee8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A9EE8(base, LIFTED_ARGS(0x825A15D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A15F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,12
  r31 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r30,68
  r11 = uint64_t(int64_t(int64_t(r30) + 68));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a1554
  if (cr6.lt) goto loc_825A1554;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A1610: lifted from generated/ by lift.py.
lifted::Result f825A1610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825A1610 (lifted)");
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
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825a163c
  if (cr6.eq) goto loc_825A163C;
  // rotlwi r3,r10,0
  r3 = __builtin_rotateleft32(uint32_t(r10), 0);
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x825A163C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A163C:
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,3884
  r30 = uint64_t(int64_t(int64_t(r11) + 3884));
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
loc_825A1648:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a1658
  if (cr6.eq) goto loc_825A1658;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x825A1658, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A1658:
  // addi r31,r31,12
  r31 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r30,32
  r11 = uint64_t(int64_t(int64_t(r30) + 32));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a1648
  if (cr6.lt) goto loc_825A1648;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,3908
  r30 = uint64_t(int64_t(int64_t(r11) + 3908));
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
loc_825A1674:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a1684
  if (cr6.eq) goto loc_825A1684;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x825A1684, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A1684:
  // addi r31,r31,12
  r31 = uint64_t(int64_t(int64_t(r31) + 12));
  // addi r11,r30,68
  r11 = uint64_t(int64_t(int64_t(r30) + 68));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a1674
  if (cr6.lt) goto loc_825A1674;
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

// sub_825A16B0: lifted from generated/ by lift.py.
lifted::Result f825A16B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825A16B0 (lifted)");
  uint64_t r0 = 0, r3 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r4
  r30 = r4;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r31,-1
  r31 = uint64_t(int64_t(-1));
  // cmplwi cr6,r5,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 1, so);
  // blt cr6,0x825a16e4
  if (cr6.lt) goto loc_825A16E4;
  // bne cr6,0x825a16e8
  if (!cr6.eq) goto loc_825A16E8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825a16e8
  goto loc_825A16E8;
loc_825A16E4:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825A16E8:
  // cmplwi cr6,r6,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 4, so);
  // bgt cr6,0x825a1740
  if (cr6.gt) goto loc_825A1740;
  // lis r12,-32166
  r12 = uint64_t(int64_t(-2108030976));
  // addi r12,r12,5896
  r12 = uint64_t(int64_t(int64_t(r12) + 5896));
  // rlwinm r0,r6,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r6)) {
  case 0:
  goto loc_825A171C;
  case 1:
  goto loc_825A1724;
  case 2:
  goto loc_825A172C;
  case 3:
  goto loc_825A1734;
  case 4:
  goto loc_825A173C;
  default:
  std::abort();
  }
loc_825A171C:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x825a1740
  goto loc_825A1740;
loc_825A1724:
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // b 0x825a1740
  goto loc_825A1740;
loc_825A172C:
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // b 0x825a1740
  goto loc_825A1740;
loc_825A1734:
  // li r31,3
  r31 = uint64_t(int64_t(3));
  // b 0x825a1740
  goto loc_825A1740;
loc_825A173C:
  // li r31,4
  r31 = uint64_t(int64_t(4));
loc_825A1740:
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // mr r3,r30
  r3 = r30;
  // addi r10,r10,3884
  r10 = uint64_t(int64_t(int64_t(r10) + 3884));
  // addi r9,r10,8
  r9 = uint64_t(int64_t(int64_t(r10) + 8));
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r9
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // bl 0x822a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8958(base, LIFTED_ARGS(0x825A1764, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // mr r3,r30
  r3 = r30;
  // addi r11,r11,3908
  r11 = uint64_t(int64_t(int64_t(r11) + 3908));
  // addi r10,r11,8
  r10 = uint64_t(int64_t(int64_t(r11) + 8));
  // rlwinm r11,r31,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 1) & 0xFFFFFFFE;
  // add r11,r31,r11
  r11 = r31 + r11;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x822a8618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8618(base, LIFTED_ARGS(0x825A1788, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_825A17A0: lifted from generated/ by lift.py.
lifted::Result f825A17A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825A17A0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // mr r31,r4
  r31 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r3,r31
  r3 = r31;
  // mr r28,r6
  r28 = r6;
  // mr r30,r7
  r30 = r7;
  // lwz r4,0(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x822a8960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8960(base, LIFTED_ARGS(0x825A17CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,24
  r6 = uint64_t(int64_t(24));
  // mr r5,r30
  r5 = r30;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825bd8f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BD8F0(base, LIFTED_ARGS(0x825A17E0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a1804
  if (cr6.eq) goto loc_825A1804;
  // rlwinm r11,r30,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // mr r4,r28
  r4 = r28;
  // add r11,r30,r11
  r11 = r30 + r11;
  // rlwinm r5,r11,3,0,28
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A17FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bdda0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BDDA0(base, LIFTED_ARGS(0x825A1804, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A1804:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A1810: lifted from generated/ by lift.py.
lifted::Result f825A1810(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825A1810 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // mr r31,r4
  r31 = r4;
  // mr r27,r5
  r27 = r5;
  // mr r3,r31
  r3 = r31;
  // mr r26,r6
  r26 = r6;
  // mr r30,r7
  r30 = r7;
  // lwz r4,0(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r25,r8
  r25 = r8;
  // mr r29,r9
  r29 = r9;
  // mr r28,r10
  r28 = r10;
  // bl 0x822a8960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8960(base, LIFTED_ARGS(0x825A1848, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r28,1
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 1, so);
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // bne cr6,0x825a1868
  if (!cr6.eq) goto loc_825A1868;
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // b 0x825a1870
  goto loc_825A1870;
loc_825A1868:
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r28,4
  r28 = uint64_t(int64_t(4));
loc_825A1870:
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r10,r1,100
  r10 = uint64_t(int64_t(int64_t(r1) + 100));
  // li r9,24
  r9 = uint64_t(int64_t(24));
  // mr r7,r29
  r7 = r29;
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x825bddc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BDDC0(base, LIFTED_ARGS(0x825A1898, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825a18d0
  if (!cr6.eq) goto loc_825A18D0;
  // rlwinm r11,r30,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // mr r4,r26
  r4 = r26;
  // add r11,r30,r11
  r11 = r30 + r11;
  // rlwinm r5,r11,3,0,28
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A18B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mullw r5,r28,r29
  r5 = uint64_t(int64_t(int64_t(int32_t(r28)) * int64_t(int32_t(r29))));
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // mr r4,r25
  r4 = r25;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A18C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825bdda0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BDDA0(base, LIFTED_ARGS(0x825A18D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A18D0:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A2238: lifted from generated/ by lift.py.
uint64_t f825A2238(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A2238 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // sth r10,2(r4)
  REX_STORE_U16(uint32_t(r4) + 2, uint16_t(r10));
  // blr
  return r3;
}

// sub_825A2250: lifted from generated/ by lift.py.
lifted::Result f825A2250(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825A2250 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r29,r3
  r29 = r3;
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // mr r31,r4
  r31 = r4;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // ori r3,r3,1
  r3 = r3 | 1;
  // stw r28,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r28));
  // mr r30,r6
  r30 = r6;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r31,24
  r11 = uint32_t(r31) & 0xFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825a22b4
  if (cr6.lt) goto loc_825A22B4;
  // beq cr6,0x825a22a8
  if (cr6.eq) goto loc_825A22A8;
  // lis r28,-32768
  r28 = uint64_t(int64_t(-2147483648));
  // ori r28,r28,16389
  r28 = r28 | 16389;
  // b 0x825a22c4
  goto loc_825A22C4;
loc_825A22A8:
  // addi r4,r29,2060
  r4 = uint64_t(int64_t(int64_t(r29) + 2060));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // b 0x825a22bc
  goto loc_825A22BC;
loc_825A22B4:
  // addi r4,r29,1036
  r4 = uint64_t(int64_t(int64_t(r29) + 1036));
  // li r5,1024
  r5 = uint64_t(int64_t(1024));
loc_825A22BC:
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A22C4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A22C4:
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A22E8: lifted from generated/ by lift.py.
lifted::Result f825A22E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A22E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // mr r30,r4
  r30 = r4;
  // ori r3,r3,1
  r3 = r3 | 1;
  // mr r29,r6
  r29 = r6;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825a2338
  if (cr6.lt) goto loc_825A2338;
  // beq cr6,0x825a232c
  if (cr6.eq) goto loc_825A232C;
  // lis r28,-32768
  r28 = uint64_t(int64_t(-2147483648));
  // ori r28,r28,16389
  r28 = r28 | 16389;
  // b 0x825a2348
  goto loc_825A2348;
loc_825A232C:
  // addi r3,r31,2060
  r3 = uint64_t(int64_t(int64_t(r31) + 2060));
  // li r5,32
  r5 = uint64_t(int64_t(32));
  // b 0x825a2340
  goto loc_825A2340;
loc_825A2338:
  // addi r3,r31,1036
  r3 = uint64_t(int64_t(int64_t(r31) + 1036));
  // li r5,1024
  r5 = uint64_t(int64_t(1024));
loc_825A2340:
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A2348, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A2348:
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A2360: lifted from generated/ by lift.py.
uint64_t f825A2360(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A2360 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // li r11,2092
  r11 = uint64_t(int64_t(2092));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_825A2370: lifted from generated/ by lift.py.
uint64_t f825A2370(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A2370 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r3,r10,1
  r3 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r3,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r3));
  // blr
  return r3;
}

// sub_825A2388: lifted from generated/ by lift.py.
uint64_t f825A2388(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A2388 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r3,r10,-1
  r3 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r3,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r3));
  // blr
  return r3;
}

// sub_825A23A0: lifted from generated/ by lift.py.
uint64_t f825A23A0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A23A0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // srawi r11,r3,2
  ca = (int32_t(r3) < 0) & ((uint32_t(r3) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r3) >> 2));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // ble cr6,0x825a23c8
  if (!cr6.gt) goto loc_825A23C8;
loc_825A23B8:
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // rlwinm r9,r9,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // bgt cr6,0x825a23b8
  if (cr6.gt) goto loc_825A23B8;
loc_825A23C8:
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // srawi r26,r3,1
  ca = (int32_t(r3) < 0) & ((uint32_t(r3) & 0x1) != 0);
  r26 = uint64_t(int64_t(int32_t(r3) >> 1));
  // li r20,0
  r20 = uint64_t(int64_t(0));
  // bne cr6,0x825a29e8
  if (!cr6.eq) goto loc_825A29E8;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stw r25,-160(r1)
  REX_STORE_U32(uint32_t(r1) + -160, uint32_t(r25));
  // ble cr6,0x825a2cd4
  if (!cr6.gt) goto loc_825A2CD4;
  // rlwinm r10,r9,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // addi r17,r26,2
  r17 = uint64_t(int64_t(int64_t(r26) + 2));
  // srawi r21,r26,1
  ca = (int32_t(r26) < 0) & ((uint32_t(r26) & 0x1) != 0);
  r21 = uint64_t(int64_t(int32_t(r26) >> 1));
  // b 0x825a23fc
  goto loc_825A23FC;
loc_825A23F8:
  // lwz r25,-160(r1)
  r25 = REX_LOAD_U32(uint32_t(r1) + -160);
loc_825A23FC:
  // add r19,r25,r20
  r19 = r25 + r20;
  // mr r6,r25
  r6 = r25;
  // cmpw cr6,r20,r19
  cr6 = lifted::Compare<int32_t>(int32_t(r20), int32_t(r19), so);
  // bge cr6,0x825a2838
  if (!cr6.lt) goto loc_825A2838;
  // subf r7,r20,r19
  r7 = r19 - r20;
  // add r11,r10,r20
  r11 = r10 + r20;
  // addi r31,r7,-1
  r31 = uint64_t(int64_t(int64_t(r7) + -1));
  // add r7,r20,r26
  r7 = r20 + r26;
  // add r3,r11,r26
  r3 = r11 + r26;
  // addi r14,r7,2
  r14 = uint64_t(int64_t(int64_t(r7) + 2));
  // addi r7,r3,2
  r7 = uint64_t(int64_t(int64_t(r3) + 2));
  // add r8,r20,r9
  r8 = r20 + r9;
  // rlwinm r5,r20,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r20) | (r20 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r22,r8,2,0,29
  r22 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // mr r16,r8
  r16 = r8;
  // stw r7,-156(r1)
  REX_STORE_U32(uint32_t(r1) + -156, uint32_t(r7));
  // rlwinm r7,r9,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r31,r31,30,2,31
  r31 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 30) & 0x3FFFFFFF;
  // add r8,r9,r7
  r8 = r9 + r7;
  // rlwinm r11,r9,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r25,r26,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // add r5,r5,r4
  r5 = r5 + r4;
  // addi r18,r31,1
  r18 = uint64_t(int64_t(int64_t(r31) + 1));
  // add r15,r8,r20
  r15 = r8 + r20;
  // add r3,r22,r4
  r3 = r22 + r4;
loc_825A2460:
  // add r31,r10,r6
  r31 = r10 + r6;
  // lfs f0,0(r5)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // rlwinm r8,r6,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfs f13,4(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 4);
  f13 = double(temp.f32);
  // rlwinm r7,r31,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r8,r8,r4
  r8 = r8 + r4;
  // add r7,r7,r4
  r7 = r7 + r4;
  // subf r29,r9,r31
  r29 = r31 - r9;
  // addi r28,r7,4
  r28 = uint64_t(int64_t(int64_t(r7) + 4));
  // rlwinm r24,r29,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // add r30,r22,r11
  r30 = r22 + r11;
  // lfs f12,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // add r29,r10,r29
  r29 = r10 + r29;
  // stfs f11,0(r5)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // add r31,r30,r4
  r31 = r30 + r4;
  // stfs f12,4(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 4, temp.u32);
  // rlwinm r23,r29,2,0,29
  r23 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r27,r31,4
  r27 = uint64_t(int64_t(int64_t(r31) + 4));
  // stfs f13,4(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // add r8,r24,r4
  r8 = r24 + r4;
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // add r30,r30,r11
  r30 = r30 + r11;
  // lfs f0,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // addi r29,r29,2
  r29 = uint64_t(int64_t(int64_t(r29) + 2));
  // lfs f13,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // lfs f12,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f12 = double(temp.f32);
  // rlwinm r24,r29,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // stfs f11,0(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // stfs f12,0(r28)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // addi r28,r8,4
  r28 = uint64_t(int64_t(int64_t(r8) + 4));
  // stfs f13,0(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r30,r4
  r7 = r30 + r4;
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // add r30,r30,r25
  r30 = r30 + r25;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // add r31,r23,r4
  r31 = r23 + r4;
  // stfs f12,0(r27)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // addi r27,r31,4
  r27 = uint64_t(int64_t(int64_t(r31) + 4));
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r28,r7,4
  r28 = uint64_t(int64_t(int64_t(r7) + 4));
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // lfs f11,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f11 = double(temp.f32);
  // subf r30,r11,r30
  r30 = r30 - r11;
  // lfs f12,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r24,r4
  r7 = r24 + r4;
  // lfs f13,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // stfs f12,0(r28)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // subf r31,r10,r29
  r31 = r29 - r10;
  // stfs f13,0(r27)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f12 = double(temp.f32);
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f13 = double(temp.f32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // subf r30,r11,r30
  r30 = r30 - r11;
  // stfs f13,4(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // rlwinm r7,r31,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r29,r8,4
  r29 = uint64_t(int64_t(int64_t(r8) + 4));
  // add r7,r7,r4
  r7 = r7 + r4;
  // add r31,r31,r9
  r31 = r31 + r9;
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // addi r28,r7,4
  r28 = uint64_t(int64_t(int64_t(r7) + 4));
  // rlwinm r24,r31,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // subf r31,r10,r31
  r31 = r31 - r10;
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f12,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // rlwinm r27,r31,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r31,r31,r26
  r31 = r31 + r26;
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // add r7,r24,r4
  r7 = r24 + r4;
  // add r28,r10,r31
  r28 = r10 + r31;
  // rlwinm r24,r31,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r29,r8,4
  r29 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r31,r7,4
  r31 = uint64_t(int64_t(int64_t(r7) + 4));
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // subf r30,r11,r30
  r30 = r30 - r11;
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r27,r4
  r7 = r27 + r4;
  // addi r29,r8,4
  r29 = uint64_t(int64_t(int64_t(r8) + 4));
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // addi r27,r7,4
  r27 = uint64_t(int64_t(int64_t(r7) + 4));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // add r31,r30,r4
  r31 = r30 + r4;
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // add r30,r30,r11
  r30 = r30 + r11;
  // lfs f12,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f12 = double(temp.f32);
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addi r29,r31,4
  r29 = uint64_t(int64_t(int64_t(r31) + 4));
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r24,r4
  r8 = r24 + r4;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r30,r4
  r7 = r30 + r4;
  // stfs f13,0(r27)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // add r30,r30,r11
  r30 = r30 + r11;
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // subf r29,r9,r28
  r29 = r28 - r9;
  // stfs f11,0(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // addi r31,r7,4
  r31 = uint64_t(int64_t(int64_t(r7) + 4));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // rlwinm r27,r29,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // stfs f13,4(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // rlwinm r8,r28,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // add r29,r10,r29
  r29 = r10 + r29;
  // add r8,r8,r4
  r8 = r8 + r4;
  // lfs f13,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // rlwinm r23,r29,2,0,29
  r23 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r28,r8,4
  r28 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r29,r29,-2
  r29 = uint64_t(int64_t(int64_t(r29) + -2));
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f12,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r30,r4
  r7 = r30 + r4;
  // stfs f12,0(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // add r30,r30,r11
  r30 = r30 + r11;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r27,r4
  r8 = r27 + r4;
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // addi r31,r7,4
  r31 = uint64_t(int64_t(int64_t(r7) + 4));
  // addi r28,r8,4
  r28 = uint64_t(int64_t(int64_t(r8) + 4));
  // rlwinm r27,r29,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // subf r29,r10,r29
  r29 = r29 - r10;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // rlwinm r24,r29,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lfs f12,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r30,r4
  r7 = r30 + r4;
  // stfs f12,0(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // subf r30,r25,r30
  r30 = r30 - r25;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r23,r4
  r8 = r23 + r4;
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // addi r31,r7,4
  r31 = uint64_t(int64_t(int64_t(r7) + 4));
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // addi r28,r8,4
  r28 = uint64_t(int64_t(int64_t(r8) + 4));
  // add r29,r29,r9
  r29 = r29 + r9;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f12 = double(temp.f32);
  // stfs f12,0(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // add r31,r27,r4
  r31 = r27 + r4;
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r30,r4
  r7 = r30 + r4;
  // addi r27,r31,4
  r27 = uint64_t(int64_t(int64_t(r31) + 4));
  // stfs f13,0(r28)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // addi r28,r7,4
  r28 = uint64_t(int64_t(int64_t(r7) + 4));
  // subf r30,r11,r30
  r30 = r30 - r11;
  // lfs f11,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f11 = double(temp.f32);
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // lfs f12,0(r27)
  temp.u32 = REX_LOAD_U32(uint32_t(r27) + 0);
  f12 = double(temp.f32);
  // subf r30,r11,r30
  r30 = r30 - r11;
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r24,r4
  r7 = r24 + r4;
  // lfs f13,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // stfs f12,0(r28)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r28) + 0, temp.u32);
  // subf r28,r10,r29
  r28 = r29 - r10;
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // mr r31,r21
  r31 = r21;
  // stfs f13,0(r27)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r27) + 0, temp.u32);
  // subf r27,r11,r30
  r27 = r30 - r11;
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // rlwinm r28,r28,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // lfs f12,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f12 = double(temp.f32);
  // xor r6,r31,r6
  r6 = r31 ^ r6;
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f13 = double(temp.f32);
  // cmpw cr6,r31,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r6), so);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // stfs f13,4(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // rlwinm r7,r29,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r30,r8,4
  r30 = uint64_t(int64_t(int64_t(r8) + 4));
  // add r7,r7,r4
  r7 = r7 + r4;
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // addi r29,r7,4
  r29 = uint64_t(int64_t(int64_t(r7) + 4));
  // lfs f13,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f13 = double(temp.f32);
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f12,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r27,r4
  r8 = r27 + r4;
  // stfs f12,0(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r7,r28,r4
  r7 = r28 + r4;
  // stfs f13,0(r29)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f13 = double(temp.f32);
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f12 = double(temp.f32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f12,4(r8)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // stfs f13,4(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // ble cr6,0x825a2804
  if (!cr6.gt) goto loc_825A2804;
loc_825A27F4:
  // srawi r31,r31,1
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x1) != 0);
  r31 = uint64_t(int64_t(int32_t(r31) >> 1));
  // xor r6,r31,r6
  r6 = r31 ^ r6;
  // cmpw cr6,r31,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r6), so);
  // bgt cr6,0x825a27f4
  if (cr6.gt) goto loc_825A27F4;
loc_825A2804:
  // lwz r8,-156(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + -156);
  // addi r18,r18,-1
  r18 = uint64_t(int64_t(int64_t(r18) + -1));
  // addi r5,r5,16
  r5 = uint64_t(int64_t(int64_t(r5) + 16));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r22,r22,16
  r22 = uint64_t(int64_t(int64_t(r22) + 16));
  // addi r3,r3,16
  r3 = uint64_t(int64_t(int64_t(r3) + 16));
  // addi r16,r16,4
  r16 = uint64_t(int64_t(int64_t(r16) + 4));
  // addi r15,r15,4
  r15 = uint64_t(int64_t(int64_t(r15) + 4));
  // addi r14,r14,4
  r14 = uint64_t(int64_t(int64_t(r14) + 4));
  // stw r8,-156(r1)
  REX_STORE_U32(uint32_t(r1) + -156, uint32_t(r8));
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // bne cr6,0x825a2460
  if (!cr6.eq) goto loc_825A2460;
  // lwz r25,-160(r1)
  r25 = REX_LOAD_U32(uint32_t(r1) + -160);
loc_825A2838:
  // addi r7,r19,2
  r7 = uint64_t(int64_t(int64_t(r19) + 2));
  // add r6,r19,r26
  r6 = r19 + r26;
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r8,r6,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r4
  r11 = r11 + r4;
  // add r8,r8,r4
  r8 = r8 + r4;
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
  // add r7,r7,r9
  r7 = r7 + r9;
  // addi r3,r8,4
  r3 = uint64_t(int64_t(int64_t(r8) + 4));
  // rlwinm r31,r7,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // add r6,r10,r6
  r6 = r10 + r6;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // add r7,r7,r9
  r7 = r7 + r9;
  // lfs f13,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f13 = double(temp.f32);
  // rlwinm r30,r6,2,0,29
  r30 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfs f12,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f12 = double(temp.f32);
  // rlwinm r27,r7,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r7,r7,-2
  r7 = uint64_t(int64_t(int64_t(r7) + -2));
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // add r11,r31,r4
  r11 = r31 + r4;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f13,0(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // add r3,r17,r7
  r3 = r17 + r7;
  // rlwinm r28,r7,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r7,r8,4
  r7 = uint64_t(int64_t(int64_t(r8) + 4));
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // subf r6,r9,r6
  r6 = r6 - r9;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // rlwinm r30,r3,2,0,29
  r30 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r31,r6,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfs f13,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f13 = double(temp.f32);
  // subf r6,r26,r6
  r6 = r6 - r26;
  // lfs f12,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f12 = double(temp.f32);
  // subf r3,r26,r3
  r3 = r3 - r26;
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // add r11,r27,r4
  r11 = r27 + r4;
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // rlwinm r29,r6,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r31,r4
  r8 = r31 + r4;
  // addi r5,r11,4
  r5 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,0(r7)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // addi r31,r8,4
  r31 = uint64_t(int64_t(int64_t(r8) + 4));
  // add r7,r28,r4
  r7 = r28 + r4;
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // add r6,r17,r6
  r6 = r17 + r6;
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // add r11,r29,r4
  r11 = r29 + r4;
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // addi r5,r7,4
  r5 = uint64_t(int64_t(int64_t(r7) + 4));
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // add r31,r3,r9
  r31 = r3 + r9;
  // lfs f0,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // lfs f13,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f13 = double(temp.f32);
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // addi r5,r8,4
  r5 = uint64_t(int64_t(int64_t(r8) + 4));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // rlwinm r11,r6,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // add r7,r11,r4
  r7 = r11 + r4;
  // addi r11,r10,-2
  r11 = uint64_t(int64_t(int64_t(r10) + -2));
  // lfs f13,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f13 = double(temp.f32);
  // addi r3,r7,4
  r3 = uint64_t(int64_t(int64_t(r7) + 4));
  // add r11,r11,r6
  r11 = r11 + r6;
  // mr r6,r21
  r6 = r21;
  // rlwinm r30,r11,2,0,29
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lfs f11,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f11 = double(temp.f32);
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // lfs f12,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f12 = double(temp.f32);
  // add r11,r11,r4
  r11 = r11 + r4;
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // add r8,r30,r4
  r8 = r30 + r4;
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // xor r20,r6,r20
  r20 = r6 ^ r20;
  // stfs f0,0(r7)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // stfs f13,0(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // cmpw cr6,r6,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r20), so);
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // lfs f13,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // lfs f12,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f12 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f13,4(r8)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // ble cr6,0x825a29d4
  if (!cr6.gt) goto loc_825A29D4;
loc_825A29C4:
  // srawi r6,r6,1
  ca = (int32_t(r6) < 0) & ((uint32_t(r6) & 0x1) != 0);
  r6 = uint64_t(int64_t(int32_t(r6) >> 1));
  // xor r20,r6,r20
  r20 = r6 ^ r20;
  // cmpw cr6,r6,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r20), so);
  // bgt cr6,0x825a29c4
  if (cr6.gt) goto loc_825A29C4;
loc_825A29D4:
  // addi r11,r25,4
  r11 = uint64_t(int64_t(int64_t(r25) + 4));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // stw r11,-160(r1)
  REX_STORE_U32(uint32_t(r1) + -160, uint32_t(r11));
  // blt cr6,0x825a23f8
  if (cr6.lt) goto loc_825A23F8;
  // b 0x825a6ea0
  // (left out: dead in C++)
  return r3;
loc_825A29E8:
  // li r15,0
  r15 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // ble cr6,0x825a2cd4
  if (!cr6.gt) goto loc_825A2CD4;
  // srawi r21,r26,1
  ca = (int32_t(r26) < 0) & ((uint32_t(r26) & 0x1) != 0);
  r21 = uint64_t(int64_t(int32_t(r26) >> 1));
loc_825A29F8:
  // add r19,r15,r20
  r19 = r15 + r20;
  // mr r8,r15
  r8 = r15;
  // cmpw cr6,r20,r19
  cr6 = lifted::Compare<int32_t>(int32_t(r20), int32_t(r19), so);
  // bge cr6,0x825a2c30
  if (!cr6.lt) goto loc_825A2C30;
  // subf r10,r20,r19
  r10 = r19 - r20;
  // add r11,r20,r9
  r11 = r20 + r9;
  // addi r6,r10,-1
  r6 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r7,r20,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r20) | (r20 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r6,30,2,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 30) & 0x3FFFFFFF;
  // add r10,r20,r26
  r10 = r20 + r26;
  // rlwinm r22,r11,2,0,29
  r22 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r18,r6,1
  r18 = uint64_t(int64_t(int64_t(r6) + 1));
  // rlwinm r28,r9,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r23,r26,2,0,29
  r23 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // add r7,r7,r4
  r7 = r7 + r4;
  // addi r16,r10,2
  r16 = uint64_t(int64_t(int64_t(r10) + 2));
  // mr r17,r11
  r17 = r11;
  // add r6,r22,r4
  r6 = r22 + r4;
loc_825A2A40:
  // add r5,r8,r9
  r5 = r8 + r9;
  // lfs f0,0(r7)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lfs f13,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f13 = double(temp.f32);
  // rlwinm r10,r5,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r4
  r11 = r11 + r4;
  // add r10,r10,r4
  r10 = r10 + r4;
  // addi r31,r5,2
  r31 = uint64_t(int64_t(int64_t(r5) + 2));
  // addi r30,r10,4
  r30 = uint64_t(int64_t(int64_t(r10) + 4));
  // rlwinm r27,r31,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // add r3,r22,r23
  r3 = r22 + r23;
  // lfs f12,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f12 = double(temp.f32);
  // subf r31,r9,r31
  r31 = r31 - r9;
  // stfs f11,0(r7)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // add r5,r3,r4
  r5 = r3 + r4;
  // stfs f12,4(r7)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // rlwinm r25,r31,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r29,r5,4
  r29 = uint64_t(int64_t(int64_t(r5) + 4));
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // add r11,r27,r4
  r11 = r27 + r4;
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // subf r3,r28,r3
  r3 = r3 - r28;
  // lfs f0,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f0 = double(temp.f32);
  // add r31,r31,r26
  r31 = r31 + r26;
  // lfs f13,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f13 = double(temp.f32);
  // lfs f12,4(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 4);
  f12 = double(temp.f32);
  // rlwinm r27,r31,2,0,29
  r27 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // add r31,r31,r9
  r31 = r31 + r9;
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 4, temp.u32);
  // stfs f12,0(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // addi r30,r11,4
  r30 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // add r10,r3,r4
  r10 = r3 + r4;
  // lfs f0,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f0 = double(temp.f32);
  // addi r3,r3,8
  r3 = uint64_t(int64_t(int64_t(r3) + 8));
  // lfs f11,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r5)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // add r5,r25,r4
  r5 = r25 + r4;
  // stfs f12,0(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addi r29,r5,4
  r29 = uint64_t(int64_t(int64_t(r5) + 4));
  // stfs f13,0(r30)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r30,r10,4
  r30 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // add r11,r3,r4
  r11 = r3 + r4;
  // lfs f11,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f11 = double(temp.f32);
  // add r3,r3,r28
  r3 = r3 + r28;
  // lfs f12,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r10)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // add r10,r27,r4
  r10 = r27 + r4;
  // lfs f13,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f13 = double(temp.f32);
  // stfs f12,0(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r5)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // mr r5,r21
  r5 = r21;
  // stfs f13,0(r29)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // xor r8,r5,r8
  r8 = r5 ^ r8;
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f12,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f12 = double(temp.f32);
  // cmpw cr6,r5,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r8), so);
  // lfs f13,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // add r11,r3,r4
  r11 = r3 + r4;
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // subf r3,r23,r3
  r3 = r3 - r23;
  // stfs f13,4(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // rlwinm r10,r31,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r31,r31,-2
  r31 = uint64_t(int64_t(int64_t(r31) + -2));
  // add r10,r10,r4
  r10 = r10 + r4;
  // rlwinm r24,r31,2,0,29
  r24 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r30,r11,4
  r30 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r29,r10,4
  r29 = uint64_t(int64_t(int64_t(r10) + 4));
  // subf r31,r9,r31
  r31 = r31 - r9;
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 0);
  f13 = double(temp.f32);
  // subf r27,r28,r3
  r27 = r3 - r28;
  // lfs f12,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f12 = double(temp.f32);
  // rlwinm r25,r31,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // add r11,r3,r4
  r11 = r3 + r4;
  // stfs f12,0(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // add r10,r24,r4
  r10 = r24 + r4;
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,0(r29)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addi r31,r10,4
  r31 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // lfs f13,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // add r11,r27,r4
  r11 = r27 + r4;
  // stfs f12,0(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // add r10,r25,r4
  r10 = r25 + r4;
  // stfs f13,0(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f12 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // stfs f13,4(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // ble cr6,0x825a2c10
  if (!cr6.gt) goto loc_825A2C10;
loc_825A2C00:
  // srawi r5,r5,1
  ca = (int32_t(r5) < 0) & ((uint32_t(r5) & 0x1) != 0);
  r5 = uint64_t(int64_t(int32_t(r5) >> 1));
  // xor r8,r5,r8
  r8 = r5 ^ r8;
  // cmpw cr6,r5,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r8), so);
  // bgt cr6,0x825a2c00
  if (cr6.gt) goto loc_825A2C00;
loc_825A2C10:
  // addi r18,r18,-1
  r18 = uint64_t(int64_t(int64_t(r18) + -1));
  // addi r7,r7,16
  r7 = uint64_t(int64_t(int64_t(r7) + 16));
  // addi r22,r22,16
  r22 = uint64_t(int64_t(int64_t(r22) + 16));
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // addi r17,r17,4
  r17 = uint64_t(int64_t(int64_t(r17) + 4));
  // addi r16,r16,4
  r16 = uint64_t(int64_t(int64_t(r16) + 4));
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // bne cr6,0x825a2a40
  if (!cr6.eq) goto loc_825A2A40;
loc_825A2C30:
  // addi r7,r19,2
  r7 = uint64_t(int64_t(int64_t(r19) + 2));
  // add r6,r19,r26
  r6 = r19 + r26;
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r6,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r4
  r11 = r11 + r4;
  // add r10,r10,r4
  r10 = r10 + r4;
  // add r5,r6,r9
  r5 = r6 + r9;
  // add r3,r7,r9
  r3 = r7 + r9;
  // addi r7,r11,4
  r7 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r6,r10,4
  r6 = uint64_t(int64_t(int64_t(r10) + 4));
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // rlwinm r5,r5,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r3,r3,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // mr r8,r21
  r8 = r21;
  // lfs f13,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f13 = double(temp.f32);
  // lfs f12,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f12 = double(temp.f32);
  // xor r20,r8,r20
  r20 = r8 ^ r20;
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // add r11,r3,r4
  r11 = r3 + r4;
  // stfs f12,0(r7)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // cmpw cr6,r8,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r20), so);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // add r10,r5,r4
  r10 = r5 + r4;
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // lfs f0,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // lfs f12,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f12 = double(temp.f32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // stfs f13,4(r10)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // ble cr6,0x825a2cc8
  if (!cr6.gt) goto loc_825A2CC8;
loc_825A2CB8:
  // srawi r8,r8,1
  ca = (int32_t(r8) < 0) & ((uint32_t(r8) & 0x1) != 0);
  r8 = uint64_t(int64_t(int32_t(r8) >> 1));
  // xor r20,r8,r20
  r20 = r8 ^ r20;
  // cmpw cr6,r8,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r20), so);
  // bgt cr6,0x825a2cb8
  if (cr6.gt) goto loc_825A2CB8;
loc_825A2CC8:
  // addi r15,r15,4
  r15 = uint64_t(int64_t(int64_t(r15) + 4));
  // cmpw cr6,r15,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r15), int32_t(r9), so);
  // blt cr6,0x825a29f8
  if (cr6.lt) goto loc_825A29F8;
loc_825A2CD4:
  // b 0x825a6ea0
  // (left out: dead in C++)
  return r3;
}

// sub_825A2CD8: lifted from generated/ by lift.py.
double f825A2CD8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A2CD8 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f14 = 0.0, f15 = 0.0, f16 = 0.0, f17 = 0.0, f18 = 0.0, f19 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a78e0
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lfs f11,64(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 64);
  f11 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f12,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f12 = double(temp.f32);
  // fadds f27,f12,f11
  f27 = double(float(f12 + f11));
  // lfs f9,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f9 = double(temp.f32);
  // lfs f10,68(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 68);
  f10 = double(temp.f32);
  // fsubs f12,f12,f11
  f12 = double(float(f12 - f11));
  // lfs f7,32(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f7 = double(temp.f32);
  // fadds f11,f9,f10
  f11 = double(float(f9 + f10));
  // lfs f8,96(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 96);
  f8 = double(temp.f32);
  // fsubs f10,f9,f10
  f10 = double(float(f9 - f10));
  // fadds f9,f7,f8
  f9 = double(float(f7 + f8));
  // lfs f5,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f5 = double(temp.f32);
  // lfs f6,100(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 100);
  f6 = double(temp.f32);
  // fsubs f8,f7,f8
  f8 = double(float(f7 - f8));
  // fadds f7,f5,f6
  f7 = double(float(f5 + f6));
  // lfs f3,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f3 = double(temp.f32);
  // fsubs f6,f5,f6
  f6 = double(float(f5 - f6));
  // lfs f4,72(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 72);
  f4 = double(temp.f32);
  // lfs f31,104(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 104);
  f31 = double(temp.f32);
  // lfs f30,40(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f30 = double(temp.f32);
  // lfs f2,76(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 76);
  f2 = double(temp.f32);
  // lfs f1,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f1 = double(temp.f32);
  // fadds f26,f1,f2
  f26 = double(float(f1 + f2));
  // lfs f29,108(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 108);
  f29 = double(temp.f32);
  // lfs f28,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f28 = double(temp.f32);
  // fadds f5,f9,f27
  f5 = double(float(f9 + f27));
  // lfs f0,8732(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8732);
  f0 = double(temp.f32);
  // fsubs f9,f27,f9
  f9 = double(float(f27 - f9));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fadds f27,f7,f11
  f27 = double(float(f7 + f11));
  // lfs f22,112(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 112);
  f22 = double(temp.f32);
  // fsubs f25,f12,f6
  f25 = double(float(f12 - f6));
  // lfs f21,116(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f21 = double(temp.f32);
  // fsubs f11,f11,f7
  f11 = double(float(f11 - f7));
  // lfs f20,52(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 52);
  f20 = double(temp.f32);
  // fadds f7,f3,f4
  f7 = double(float(f3 + f4));
  // lfs f19,88(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 88);
  f19 = double(temp.f32);
  // fadds f6,f6,f12
  f6 = double(float(f6 + f12));
  // lfs f13,8728(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8728);
  f13 = double(temp.f32);
  // fsubs f4,f3,f4
  f4 = double(float(f3 - f4));
  // lfs f18,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f18 = double(temp.f32);
  // fadds f12,f30,f31
  f12 = double(float(f30 + f31));
  // lfs f17,92(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 92);
  f17 = double(temp.f32);
  // fsubs f3,f1,f2
  f3 = double(float(f1 - f2));
  // lfs f16,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f16 = double(temp.f32);
  // fsubs f2,f30,f31
  f2 = double(float(f30 - f31));
  // lfs f15,120(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 120);
  f15 = double(temp.f32);
  // fadds f24,f8,f10
  f24 = double(float(f8 + f10));
  // fsubs f10,f10,f8
  f10 = double(float(f10 - f8));
  // fadds f8,f28,f29
  f8 = double(float(f28 + f29));
  // fsubs f1,f28,f29
  f1 = double(float(f28 - f29));
  // fadds f31,f12,f7
  f31 = double(float(f12 + f7));
  // fsubs f7,f7,f12
  f7 = double(float(f7 - f12));
  // fadds f12,f2,f3
  f12 = double(float(f2 + f3));
  // fadds f30,f8,f26
  f30 = double(float(f8 + f26));
  // fsubs f8,f26,f8
  f8 = double(float(f26 - f8));
  // fsubs f29,f4,f1
  f29 = double(float(f4 - f1));
  // fmuls f28,f12,f13
  f28 = double(float(f12 * f13));
  // fmr f26,f12
  f26 = f12;
  // fadds f12,f1,f4
  f12 = double(float(f1 + f4));
  // fsubs f4,f3,f2
  f4 = double(float(f3 - f2));
  // fmuls f23,f29,f13
  f23 = double(float(f29 * f13));
  // fmsubs f3,f29,f0,f28
  f3 = double(float(std::fma(f29, f0, -f28)));
  // lfs f28,84(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 84);
  f28 = double(temp.f32);
  // fmuls f1,f4,f0
  f1 = double(float(f4 * f0));
  // fmuls f29,f4,f13
  f29 = double(float(f4 * f13));
  // fmadds f2,f26,f0,f23
  f2 = double(float(std::fma(f26, f0, f23)));
  // lfs f26,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f26 = double(temp.f32);
  // fadds f14,f26,f28
  f14 = double(float(f26 + f28));
  // fsubs f26,f26,f28
  f26 = double(float(f26 - f28));
  // stfs f26,-188(r1)
  temp.f32 = float(f26);
  REX_STORE_U32(uint32_t(r1) + -188, temp.u32);
  // fmsubs f4,f12,f13,f1
  f4 = double(float(std::fma(f12, f13, -f1)));
  // fmadds f1,f12,f0,f29
  f1 = double(float(std::fma(f12, f0, f29)));
  // lfs f29,16(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f29 = double(temp.f32);
  // lfs f12,80(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 80);
  f12 = double(temp.f32);
  // fadds f23,f29,f12
  f23 = double(float(f29 + f12));
  // fsubs f12,f29,f12
  f12 = double(float(f29 - f12));
  // lfs f29,48(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 48);
  f29 = double(temp.f32);
  // fadds f28,f29,f22
  f28 = double(float(f29 + f22));
  // fsubs f22,f29,f22
  f22 = double(float(f29 - f22));
  // fadds f29,f20,f21
  f29 = double(float(f20 + f21));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f21,f20,f21
  f21 = double(float(f20 - f21));
  // fadds f20,f28,f23
  f20 = double(float(f28 + f23));
  // stfs f20,-168(r1)
  temp.f32 = float(f20);
  REX_STORE_U32(uint32_t(r1) + -168, temp.u32);
  // fsubs f28,f23,f28
  f28 = double(float(f23 - f28));
  // stfs f28,-176(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // fadds f20,f22,f26
  f20 = double(float(f22 + f26));
  // lfs f26,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f26 = double(temp.f32);
  // fadds f28,f29,f14
  f28 = double(float(f29 + f14));
  // stfs f28,-164(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + -164, temp.u32);
  // fsubs f23,f12,f21
  f23 = double(float(f12 - f21));
  // fsubs f29,f14,f29
  f29 = double(float(f14 - f29));
  // stfs f29,-172(r1)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r1) + -172, temp.u32);
  // lfs f29,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f29 = double(temp.f32);
  // fadds f28,f29,f15
  f28 = double(float(f29 + f15));
  // stfs f28,-180(r1)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r1) + -180, temp.u32);
  // lfs f28,124(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 124);
  f28 = double(temp.f32);
  // fsubs f29,f29,f15
  f29 = double(float(f29 - f15));
  // fadds f14,f26,f28
  f14 = double(float(f26 + f28));
  // stfs f14,-192(r1)
  temp.f32 = float(f14);
  REX_STORE_U32(uint32_t(r1) + -192, temp.u32);
  // fsubs f28,f26,f28
  f28 = double(float(f26 - f28));
  // fsubs f14,f23,f20
  f14 = double(float(f23 - f20));
  // fadds f23,f20,f23
  f23 = double(float(f20 + f23));
  // stfs f23,-184(r1)
  temp.f32 = float(f23);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // fadds f23,f21,f12
  f23 = double(float(f21 + f12));
  // lfs f12,-188(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -188);
  f12 = double(temp.f32);
  // fsubs f22,f12,f22
  f22 = double(float(f12 - f22));
  // lfs f12,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f12 = double(temp.f32);
  // fsubs f20,f16,f17
  f20 = double(float(f16 - f17));
  // fsubs f21,f18,f19
  f21 = double(float(f18 - f19));
  // fmuls f26,f14,f12
  f26 = double(float(f14 * f12));
  // lfs f14,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f14 = double(temp.f32);
  // fmuls f15,f14,f12
  f15 = double(float(f14 * f12));
  // fadds f14,f22,f23
  f14 = double(float(f22 + f23));
  // fsubs f23,f22,f23
  f23 = double(float(f22 - f23));
  // stfs f23,-184(r1)
  temp.f32 = float(f23);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // fadds f23,f18,f19
  f23 = double(float(f18 + f19));
  // fadds f22,f16,f17
  f22 = double(float(f16 + f17));
  // lfs f16,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f16 = double(temp.f32);
  // fmuls f19,f14,f12
  f19 = double(float(f14 * f12));
  // fadds f17,f16,f23
  f17 = double(float(f16 + f23));
  // fsubs f23,f23,f16
  f23 = double(float(f23 - f16));
  // lfs f14,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f14 = double(temp.f32);
  // fmuls f18,f14,f12
  f18 = double(float(f14 * f12));
  // stfs f23,-184(r1)
  temp.f32 = float(f23);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // lfs f14,-192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -192);
  f14 = double(temp.f32);
  // lfs f23,-192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -192);
  f23 = double(temp.f32);
  // fadds f14,f14,f22
  f14 = double(float(f14 + f22));
  // fsubs f23,f22,f23
  f23 = double(float(f22 - f23));
  // stfs f23,-180(r1)
  temp.f32 = float(f23);
  REX_STORE_U32(uint32_t(r1) + -180, temp.u32);
  // fadds f22,f29,f20
  f22 = double(float(f29 + f20));
  // stfs f14,-188(r1)
  temp.f32 = float(f14);
  REX_STORE_U32(uint32_t(r1) + -188, temp.u32);
  // fsubs f29,f20,f29
  f29 = double(float(f20 - f29));
  // fsubs f23,f21,f28
  f23 = double(float(f21 - f28));
  // fadds f28,f28,f21
  f28 = double(float(f28 + f21));
  // fmuls f16,f22,f0
  f16 = double(float(f22 * f0));
  // fmuls f21,f29,f13
  f21 = double(float(f29 * f13));
  // fmr f14,f23
  f14 = f23;
  // fmuls f22,f22,f13
  f22 = double(float(f22 * f13));
  // fmr f20,f29
  f20 = f29;
  // fsubs f29,f6,f19
  f29 = double(float(f6 - f19));
  // fadds f6,f19,f6
  f6 = double(float(f19 + f6));
  // fmsubs f23,f23,f13,f16
  f23 = double(float(std::fma(f23, f13, -f16)));
  // fmuls f16,f28,f13
  f16 = double(float(f28 * f13));
  // fmsubs f13,f28,f0,f21
  f13 = double(float(std::fma(f28, f0, -f21)));
  // fmadds f22,f14,f0,f22
  f22 = double(float(std::fma(f14, f0, f22)));
  // fsubs f28,f10,f18
  f28 = double(float(f10 - f18));
  // fadds f10,f18,f10
  f10 = double(float(f18 + f10));
  // fmadds f0,f20,f0,f16
  f0 = double(float(std::fma(f20, f0, f16)));
  // fsubs f21,f4,f13
  f21 = double(float(f4 - f13));
  // fadds f13,f13,f4
  f13 = double(float(f13 + f4));
  // fsubs f4,f1,f0
  f4 = double(float(f1 - f0));
  // fadds f0,f0,f1
  f0 = double(float(f0 + f1));
  // fadds f1,f21,f29
  f1 = double(float(f21 + f29));
  // stfs f1,96(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 96, temp.u32);
  // fsubs f1,f29,f21
  f1 = double(float(f29 - f21));
  // stfs f1,104(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 104, temp.u32);
  // fadds f1,f26,f25
  f1 = double(float(f26 + f25));
  // fadds f29,f23,f3
  f29 = double(float(f23 + f3));
  // fadds f21,f13,f10
  f21 = double(float(f13 + f10));
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // stfs f13,124(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 124, temp.u32);
  // fsubs f10,f3,f23
  f10 = double(float(f3 - f23));
  // stfs f21,116(r3)
  temp.f32 = float(f21);
  REX_STORE_U32(uint32_t(r3) + 116, temp.u32);
  // fadds f3,f4,f28
  f3 = double(float(f4 + f28));
  // stfs f3,100(r3)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r3) + 100, temp.u32);
  // fsubs f4,f28,f4
  f4 = double(float(f28 - f4));
  // stfs f4,108(r3)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + 108, temp.u32);
  // fsubs f13,f24,f15
  f13 = double(float(f24 - f15));
  // fadds f3,f22,f2
  f3 = double(float(f22 + f2));
  // fadds f4,f15,f24
  f4 = double(float(f15 + f24));
  // fsubs f28,f6,f0
  f28 = double(float(f6 - f0));
  // stfs f28,112(r3)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r3) + 112, temp.u32);
  // fadds f0,f0,f6
  f0 = double(float(f0 + f6));
  // stfs f0,120(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 120, temp.u32);
  // fsubs f6,f2,f22
  f6 = double(float(f2 - f22));
  // fadds f2,f29,f1
  f2 = double(float(f29 + f1));
  // stfs f2,64(r3)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r3) + 64, temp.u32);
  // fsubs f2,f1,f29
  f2 = double(float(f1 - f29));
  // lfs f29,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f29 = double(temp.f32);
  // fsubs f0,f25,f26
  f0 = double(float(f25 - f26));
  // stfs f2,72(r3)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r3) + 72, temp.u32);
  // fsubs f2,f7,f29
  f2 = double(float(f7 - f29));
  // fadds f1,f10,f13
  f1 = double(float(f10 + f13));
  // stfs f1,84(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 84, temp.u32);
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // lfs f1,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f1 = double(temp.f32);
  // fadds f10,f3,f4
  f10 = double(float(f3 + f4));
  // stfs f10,68(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // fsubs f10,f4,f3
  f10 = double(float(f4 - f3));
  // lfs f3,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f3 = double(temp.f32);
  // stfs f10,76(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 76, temp.u32);
  // fadds f10,f3,f8
  f10 = double(float(f3 + f8));
  // fsubs f8,f8,f3
  f8 = double(float(f8 - f3));
  // stfs f13,92(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 92, temp.u32);
  // fsubs f13,f11,f1
  f13 = double(float(f11 - f1));
  // fadds f11,f1,f11
  f11 = double(float(f1 + f11));
  // fsubs f4,f0,f6
  f4 = double(float(f0 - f6));
  // stfs f4,80(r3)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // fadds f0,f6,f0
  f0 = double(float(f6 + f0));
  // lfs f4,-172(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -172);
  f4 = double(temp.f32);
  // stfs f0,88(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 88, temp.u32);
  // fadds f0,f4,f9
  f0 = double(float(f4 + f9));
  // fsubs f6,f2,f10
  f6 = double(float(f2 - f10));
  // fadds f2,f10,f2
  f2 = double(float(f10 + f2));
  // fadds f10,f29,f7
  f10 = double(float(f29 + f7));
  // fmuls f7,f6,f12
  f7 = double(float(f6 * f12));
  // fmuls f6,f2,f12
  f6 = double(float(f2 * f12));
  // fsubs f3,f10,f8
  f3 = double(float(f10 - f8));
  // fadds f8,f8,f10
  f8 = double(float(f8 + f10));
  // fsubs f10,f9,f4
  f10 = double(float(f9 - f4));
  // fmuls f9,f3,f12
  f9 = double(float(f3 * f12));
  // fmuls f12,f8,f12
  f12 = double(float(f8 * f12));
  // fadds f8,f7,f10
  f8 = double(float(f7 + f10));
  // stfs f8,32(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // fadds f8,f6,f11
  f8 = double(float(f6 + f11));
  // stfs f8,36(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // fsubs f11,f11,f6
  f11 = double(float(f11 - f6));
  // stfs f11,44(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // fsubs f10,f10,f7
  f10 = double(float(f10 - f7));
  // stfs f10,40(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // fadds f10,f17,f31
  f10 = double(float(f17 + f31));
  // lfs f7,-188(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -188);
  f7 = double(temp.f32);
  // fsubs f8,f31,f17
  f8 = double(float(f31 - f17));
  // fadds f11,f9,f13
  f11 = double(float(f9 + f13));
  // stfs f11,52(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // fsubs f11,f0,f12
  f11 = double(float(f0 - f12));
  // stfs f11,48(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // stfs f0,56(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // lfs f12,-168(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -168);
  f12 = double(temp.f32);
  // fsubs f0,f13,f9
  f0 = double(float(f13 - f9));
  // stfs f0,60(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // fadds f0,f12,f5
  f0 = double(float(f12 + f5));
  // lfs f11,-164(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -164);
  f11 = double(temp.f32);
  // fadds f9,f7,f30
  f9 = double(float(f7 + f30));
  // fadds f13,f11,f27
  f13 = double(float(f11 + f27));
  // fsubs f12,f5,f12
  f12 = double(float(f5 - f12));
  // fsubs f11,f27,f11
  f11 = double(float(f27 - f11));
  // fsubs f7,f30,f7
  f7 = double(float(f30 - f7));
  // fadds f6,f10,f0
  f6 = double(float(f10 + f0));
  // fsubs f0,f0,f10
  f0 = double(float(f0 - f10));
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // fsubs f0,f13,f9
  f0 = double(float(f13 - f9));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fsubs f0,f12,f7
  f0 = double(float(f12 - f7));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // fadds f0,f8,f11
  f0 = double(float(f8 + f11));
  // stfs f0,20(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // fadds f0,f7,f12
  f0 = double(float(f7 + f12));
  // stfs f6,0(r3)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // stfs f0,24(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // fadds f6,f9,f13
  f6 = double(float(f9 + f13));
  // fsubs f0,f11,f8
  f0 = double(float(f11 - f8));
  // stfs f6,4(r3)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // stfs f0,28(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a792c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_825A3108: lifted from generated/ by lift.py.
double f825A3108(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A3108 (lifted)");
  uint64_t r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a7900
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lfs f0,8(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f0 = double(temp.f32);
  // lfs f13,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f13 = double(temp.f32);
  // lfs f12,16(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f12 = double(temp.f32);
  // lfs f11,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f11 = double(temp.f32);
  // lfs f10,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f10 = double(temp.f32);
  // lfs f9,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f9 = double(temp.f32);
  // lfs f8,40(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f8 = double(temp.f32);
  // lfs f7,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f7 = double(temp.f32);
  // lfs f6,88(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 88);
  f6 = double(temp.f32);
  // lfs f5,92(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 92);
  f5 = double(temp.f32);
  // lfs f4,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f4 = double(temp.f32);
  // lfs f3,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f3 = double(temp.f32);
  // lfs f2,64(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 64);
  f2 = double(temp.f32);
  // lfs f1,68(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 68);
  f1 = double(temp.f32);
  // lfs f31,32(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f31 = double(temp.f32);
  // lfs f30,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f30 = double(temp.f32);
  // lfs f29,96(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 96);
  f29 = double(temp.f32);
  // lfs f28,100(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 100);
  f28 = double(temp.f32);
  // lfs f27,80(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 80);
  f27 = double(temp.f32);
  // lfs f26,84(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 84);
  f26 = double(temp.f32);
  // lfs f25,104(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 104);
  f25 = double(temp.f32);
  // lfs f24,108(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 108);
  f24 = double(temp.f32);
  // lfs f23,112(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 112);
  f23 = double(temp.f32);
  // lfs f22,116(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f22 = double(temp.f32);
  // stfs f2,8(r3)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // stfs f1,12(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // stfs f31,16(r3)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // stfs f30,20(r3)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // stfs f29,24(r3)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // stfs f28,28(r3)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // stfs f27,40(r3)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // stfs f26,44(r3)
  temp.f32 = float(f26);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // stfs f23,56(r3)
  temp.f32 = float(f23);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // stfs f22,60(r3)
  temp.f32 = float(f22);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // stfs f25,88(r3)
  temp.f32 = float(f25);
  REX_STORE_U32(uint32_t(r3) + 88, temp.u32);
  // stfs f24,92(r3)
  temp.f32 = float(f24);
  REX_STORE_U32(uint32_t(r3) + 92, temp.u32);
  // stfs f0,64(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 64, temp.u32);
  // stfs f13,68(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // stfs f12,32(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // stfs f11,36(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // stfs f10,96(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 96, temp.u32);
  // stfs f9,100(r3)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r3) + 100, temp.u32);
  // stfs f8,80(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // stfs f7,84(r3)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r3) + 84, temp.u32);
  // stfs f6,104(r3)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r3) + 104, temp.u32);
  // stfs f5,108(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 108, temp.u32);
  // stfs f4,112(r3)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + 112, temp.u32);
  // stfs f3,116(r3)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r3) + 116, temp.u32);
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a794c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_825A31F0: lifted from generated/ by lift.py.
double f825A31F0(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A31F0 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a790c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lfs f13,32(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fadds f29,f0,f13
  f29 = double(float(f0 + f13));
  // lfs f10,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f10 = double(temp.f32);
  // lfs f12,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f12 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // lfs f11,48(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 48);
  f11 = double(temp.f32);
  // fadds f13,f10,f12
  f13 = double(float(f10 + f12));
  // lfs f9,16(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f9 = double(temp.f32);
  // fsubs f12,f10,f12
  f12 = double(float(f10 - f12));
  // lfs f8,52(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 52);
  f8 = double(temp.f32);
  // fadds f28,f9,f11
  f28 = double(float(f9 + f11));
  // lfs f7,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f7 = double(temp.f32);
  // fsubs f11,f9,f11
  f11 = double(float(f9 - f11));
  // fadds f10,f7,f8
  f10 = double(float(f7 + f8));
  // lfs f6,40(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f6 = double(temp.f32);
  // fsubs f9,f7,f8
  f9 = double(float(f7 - f8));
  // lfs f4,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f4 = double(temp.f32);
  // lfs f5,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f5 = double(temp.f32);
  // lfs f2,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f2 = double(temp.f32);
  // lfs f3,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f3 = double(temp.f32);
  // lfs f1,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f1 = double(temp.f32);
  // fadds f27,f3,f5
  f27 = double(float(f3 + f5));
  // lfs f31,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f31 = double(temp.f32);
  // fadds f26,f1,f2
  f26 = double(float(f1 + f2));
  // lfs f30,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f30 = double(temp.f32);
  // fadds f8,f28,f29
  f8 = double(float(f28 + f29));
  // fsubs f7,f29,f28
  f7 = double(float(f29 - f28));
  // fadds f29,f10,f13
  f29 = double(float(f10 + f13));
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // fsubs f10,f0,f9
  f10 = double(float(f0 - f9));
  // fadds f28,f11,f12
  f28 = double(float(f11 + f12));
  // fadds f9,f9,f0
  f9 = double(float(f9 + f0));
  // fsubs f12,f12,f11
  f12 = double(float(f12 - f11));
  // fadds f0,f4,f6
  f0 = double(float(f4 + f6));
  // fsubs f11,f4,f6
  f11 = double(float(f4 - f6));
  // fsubs f6,f3,f5
  f6 = double(float(f3 - f5));
  // fsubs f5,f1,f2
  f5 = double(float(f1 - f2));
  // fsubs f4,f30,f31
  f4 = double(float(f30 - f31));
  // fadds f25,f30,f31
  f25 = double(float(f30 + f31));
  // fadds f3,f26,f0
  f3 = double(float(f26 + f0));
  // fsubs f1,f0,f26
  f1 = double(float(f0 - f26));
  // fadds f0,f5,f6
  f0 = double(float(f5 + f6));
  // fsubs f30,f11,f4
  f30 = double(float(f11 - f4));
  // fadds f11,f4,f11
  f11 = double(float(f4 + f11));
  // fsubs f6,f6,f5
  f6 = double(float(f6 - f5));
  // fadds f2,f25,f27
  f2 = double(float(f25 + f27));
  // fsubs f31,f27,f25
  f31 = double(float(f27 - f25));
  // fsubs f5,f30,f0
  f5 = double(float(f30 - f0));
  // fadds f4,f0,f30
  f4 = double(float(f0 + f30));
  // lfs f0,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f0 = double(temp.f32);
  // fadds f30,f6,f11
  f30 = double(float(f6 + f11));
  // fsubs f27,f11,f6
  f27 = double(float(f11 - f6));
  // fmuls f11,f5,f0
  f11 = double(float(f5 * f0));
  // fmuls f6,f4,f0
  f6 = double(float(f4 * f0));
  // fmuls f5,f30,f0
  f5 = double(float(f30 * f0));
  // fmuls f0,f27,f0
  f0 = double(float(f27 * f0));
  // fadds f4,f11,f10
  f4 = double(float(f11 + f10));
  // stfs f4,32(r3)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // fsubs f11,f10,f11
  f11 = double(float(f10 - f11));
  // stfs f11,40(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // fsubs f11,f28,f6
  f11 = double(float(f28 - f6));
  // stfs f11,44(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // fsubs f11,f9,f5
  f11 = double(float(f9 - f5));
  // stfs f11,48(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // fadds f11,f0,f12
  f11 = double(float(f0 + f12));
  // stfs f11,52(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // stfs f0,60(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // fadds f0,f3,f8
  f0 = double(float(f3 + f8));
  // stfs f0,0(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fadds f0,f2,f29
  f0 = double(float(f2 + f29));
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fsubs f0,f8,f3
  f0 = double(float(f8 - f3));
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // fadds f4,f6,f28
  f4 = double(float(f6 + f28));
  // stfs f4,36(r3)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // fadds f11,f5,f9
  f11 = double(float(f5 + f9));
  // stfs f11,56(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // fsubs f0,f29,f2
  f0 = double(float(f29 - f2));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fsubs f0,f7,f31
  f0 = double(float(f7 - f31));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // fadds f0,f1,f13
  f0 = double(float(f1 + f13));
  // stfs f0,20(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // fadds f0,f31,f7
  f0 = double(float(f31 + f7));
  // stfs f0,24(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // fsubs f0,f13,f1
  f0 = double(float(f13 - f1));
  // stfs f0,28(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a7958
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_825A3380: lifted from generated/ by lift.py.
void f825A3380(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A3380 (lifted)");
  double f0 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lfs f13,16(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f13 = double(temp.f32);
  // lfs f0,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // lfs f11,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f11 = double(temp.f32);
  // fadds f6,f0,f13
  f6 = double(float(f0 + f13));
  // lfs f12,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f12 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // lfs f9,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f9 = double(temp.f32);
  // fadds f13,f11,f12
  f13 = double(float(f11 + f12));
  // lfs f10,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f10 = double(temp.f32);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // fadds f11,f9,f10
  f11 = double(float(f9 + f10));
  // lfs f7,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f7 = double(temp.f32);
  // lfs f8,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f8 = double(temp.f32);
  // fsubs f10,f9,f10
  f10 = double(float(f9 - f10));
  // fadds f9,f7,f8
  f9 = double(float(f7 + f8));
  // fsubs f8,f7,f8
  f8 = double(float(f7 - f8));
  // fadds f7,f11,f6
  f7 = double(float(f11 + f6));
  // stfs f7,0(r3)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fadds f7,f10,f12
  f7 = double(float(f10 + f12));
  // stfs f7,12(r3)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fadds f7,f9,f13
  f7 = double(float(f9 + f13));
  // stfs f7,4(r3)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fsubs f7,f0,f8
  f7 = double(float(f0 - f8));
  // stfs f7,8(r3)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // stfs f0,24(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // fsubs f11,f6,f11
  f11 = double(float(f6 - f11));
  // stfs f11,16(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // fsubs f13,f13,f9
  f13 = double(float(f13 - f9));
  // stfs f13,20(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // fsubs f0,f12,f10
  f0 = double(float(f12 - f10));
  // stfs f0,28(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // blr
  return;
}

// sub_825A3408: lifted from generated/ by lift.py.
double f825A3408(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A3408 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f14 = 0.0, f15 = 0.0, f16 = 0.0, f17 = 0.0, f18 = 0.0, f19 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a78e0
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lfs f0,32(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f8,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f8 = double(temp.f32);
  // lfs f10,100(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 100);
  f10 = double(temp.f32);
  // lfs f9,96(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 96);
  f9 = double(temp.f32);
  // fsubs f27,f0,f10
  f27 = double(float(f0 - f10));
  // fadds f26,f8,f9
  f26 = double(float(f8 + f9));
  // lfs f11,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f11 = double(temp.f32);
  // lfs f13,8748(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8748);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f9,f8,f9
  f9 = double(float(f8 - f9));
  // lfs f3,68(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 68);
  f3 = double(temp.f32);
  // fadds f10,f10,f0
  f10 = double(float(f10 + f0));
  // lfs f5,72(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 72);
  f5 = double(temp.f32);
  // fsubs f25,f11,f3
  f25 = double(float(f11 - f3));
  // lfs f4,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f4 = double(temp.f32);
  // lfs f2,64(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 64);
  f2 = double(temp.f32);
  // lfs f12,8744(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8744);
  f12 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f1,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f1 = double(temp.f32);
  // lfs f7,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f7 = double(temp.f32);
  // fadds f24,f1,f2
  f24 = double(float(f1 + f2));
  // lfs f6,76(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 76);
  f6 = double(temp.f32);
  // fsubs f23,f27,f26
  f23 = double(float(f27 - f26));
  // lfs f31,40(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f31 = double(temp.f32);
  // fadds f27,f26,f27
  f27 = double(float(f26 + f27));
  // lfs f0,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f30,108(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 108);
  f30 = double(temp.f32);
  // fadds f21,f9,f10
  f21 = double(float(f9 + f10));
  // lfs f29,104(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 104);
  f29 = double(temp.f32);
  // fsubs f22,f10,f9
  f22 = double(float(f10 - f9));
  // lfs f28,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f28 = double(temp.f32);
  // fadds f9,f4,f5
  f9 = double(float(f4 + f5));
  // fsubs f10,f7,f6
  f10 = double(float(f7 - f6));
  // fmuls f8,f23,f0
  f8 = double(float(f23 * f0));
  // fmuls f27,f27,f0
  f27 = double(float(f27 * f0));
  // fmr f19,f9
  f19 = f9;
  // fadds f26,f8,f25
  f26 = double(float(f8 + f25));
  // fsubs f25,f25,f8
  f25 = double(float(f25 - f8));
  // fadds f8,f3,f11
  f8 = double(float(f3 + f11));
  // lfs f11,8740(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8740);
  f11 = double(temp.f32);
  // fsubs f3,f1,f2
  f3 = double(float(f1 - f2));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f1,f21,f0
  f1 = double(float(f21 * f0));
  // fmuls f20,f9,f11
  f20 = double(float(f9 * f11));
  // fmuls f2,f22,f0
  f2 = double(float(f22 * f0));
  // fadds f23,f27,f24
  f23 = double(float(f27 + f24));
  // fmuls f18,f10,f11
  f18 = double(float(f10 * f11));
  // fsubs f27,f24,f27
  f27 = double(float(f24 - f27));
  // fsubs f9,f31,f30
  f9 = double(float(f31 - f30));
  // fadds f24,f28,f29
  f24 = double(float(f28 + f29));
  // fsubs f21,f8,f1
  f21 = double(float(f8 - f1));
  // fadds f1,f1,f8
  f1 = double(float(f1 + f8));
  // fmsubs f8,f10,f13,f20
  f8 = double(float(std::fma(f10, f13, -f20)));
  // lfs f10,8736(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8736);
  f10 = double(temp.f32);
  // fadds f22,f2,f3
  f22 = double(float(f2 + f3));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f3,f3,f2
  f3 = double(float(f3 - f2));
  // fmadds f2,f19,f13,f18
  f2 = double(float(std::fma(f19, f13, f18)));
  // fmr f20,f9
  f20 = f9;
  // fmuls f18,f9,f10
  f18 = double(float(f9 * f10));
  // fmuls f19,f24,f10
  f19 = double(float(f24 * f10));
  // fadds f9,f6,f7
  f9 = double(float(f6 + f7));
  // fsubs f7,f4,f5
  f7 = double(float(f4 - f5));
  // fmadds f5,f24,f12,f18
  f5 = double(float(std::fma(f24, f12, f18)));
  // fmsubs f6,f20,f12,f19
  f6 = double(float(std::fma(f20, f12, -f19)));
  // fmr f20,f9
  f20 = f9;
  // fmuls f19,f7,f12
  f19 = double(float(f7 * f12));
  // fmr f18,f9
  f18 = f9;
  // lfs f9,8732(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8732);
  f9 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmuls f17,f7,f10
  f17 = double(float(f7 * f10));
  // fadds f7,f30,f31
  f7 = double(float(f30 + f31));
  // fadds f24,f5,f2
  f24 = double(float(f5 + f2));
  // fadds f4,f6,f8
  f4 = double(float(f6 + f8));
  // fsubs f6,f8,f6
  f6 = double(float(f8 - f6));
  // lfs f8,8728(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8728);
  f8 = double(temp.f32);
  // fsubs f5,f2,f5
  f5 = double(float(f2 - f5));
  // fsubs f2,f28,f29
  f2 = double(float(f28 - f29));
  // fmsubs f31,f20,f10,f19
  f31 = double(float(std::fma(f20, f10, -f19)));
  // fmuls f15,f7,f13
  f15 = double(float(f7 * f13));
  // lfs f29,16(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f29 = double(temp.f32);
  // fmuls f14,f7,f11
  f14 = double(float(f7 * f11));
  // lfs f28,84(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 84);
  f28 = double(temp.f32);
  // fmadds f30,f18,f12,f17
  f30 = double(float(std::fma(f18, f12, f17)));
  // lfs f20,80(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 80);
  f20 = double(temp.f32);
  // lfs f19,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f19 = double(temp.f32);
  // lfs f16,112(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 112);
  f16 = double(temp.f32);
  // lfs f7,52(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 52);
  f7 = double(temp.f32);
  // lfs f18,48(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 48);
  f18 = double(temp.f32);
  // lfs f17,116(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f17 = double(temp.f32);
  // fmadds f15,f2,f11,f15
  f15 = double(float(std::fma(f2, f11, f15)));
  // fmsubs f2,f2,f13,f14
  f2 = double(float(std::fma(f2, f13, -f14)));
  // fsubs f14,f31,f15
  f14 = double(float(f31 - f15));
  // stfs f14,-164(r1)
  temp.f32 = float(f14);
  REX_STORE_U32(uint32_t(r1) + -164, temp.u32);
  // fadds f31,f15,f31
  f31 = double(float(f15 + f31));
  // stfs f31,-172(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + -172, temp.u32);
  // fsubs f31,f30,f2
  f31 = double(float(f30 - f2));
  // stfs f31,-160(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + -160, temp.u32);
  // fadds f2,f2,f30
  f2 = double(float(f2 + f30));
  // stfs f2,-168(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -168, temp.u32);
  // fsubs f2,f29,f28
  f2 = double(float(f29 - f28));
  // fadds f31,f7,f16
  f31 = double(float(f7 + f16));
  // fsubs f7,f7,f16
  f7 = double(float(f7 - f16));
  // lfs f16,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f16 = double(temp.f32);
  // fmuls f15,f2,f8
  f15 = double(float(f2 * f8));
  // fmr f30,f2
  f30 = f2;
  // fadds f2,f19,f20
  f2 = double(float(f19 + f20));
  // fmuls f14,f2,f8
  f14 = double(float(f2 * f8));
  // fmadds f2,f2,f9,f15
  f2 = double(float(std::fma(f2, f9, f15)));
  // stfs f2,-184(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // fsubs f2,f18,f17
  f2 = double(float(f18 - f17));
  // stfs f2,-176(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // fmsubs f30,f30,f9,f14
  f30 = double(float(std::fma(f30, f9, -f14)));
  // fmuls f14,f31,f9
  f14 = double(float(f31 * f9));
  // fmuls f31,f31,f8
  f31 = double(float(f31 * f8));
  // stfs f31,-180(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + -180, temp.u32);
  // fmr f15,f2
  f15 = f2;
  // fadds f2,f28,f29
  f2 = double(float(f28 + f29));
  // fsubs f31,f19,f20
  f31 = double(float(f19 - f20));
  // fmsubs f29,f15,f8,f14
  f29 = double(float(std::fma(f15, f8, -f14)));
  // fmr f19,f2
  f19 = f2;
  // lfs f20,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f20 = double(temp.f32);
  // lfs f14,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f14 = double(temp.f32);
  // fmadds f28,f15,f9,f14
  f28 = double(float(std::fma(f15, f9, f14)));
  // fmr f14,f2
  f14 = f2;
  // fmuls f2,f31,f8
  f2 = double(float(f31 * f8));
  // stfs f2,-176(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // fadds f2,f17,f18
  f2 = double(float(f17 + f18));
  // fmuls f15,f31,f9
  f15 = double(float(f31 * f9));
  // fmuls f18,f7,f8
  f18 = double(float(f7 * f8));
  // fadds f31,f29,f30
  f31 = double(float(f29 + f30));
  // fsubs f30,f30,f29
  f30 = double(float(f30 - f29));
  // fadds f29,f28,f20
  f29 = double(float(f28 + f20));
  // fsubs f28,f20,f28
  f28 = double(float(f20 - f28));
  // fmuls f17,f2,f8
  f17 = double(float(f2 * f8));
  // fmsubs f20,f19,f8,f15
  f20 = double(float(std::fma(f19, f8, -f15)));
  // lfs f15,124(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 124);
  f15 = double(temp.f32);
  // fmsubs f8,f2,f9,f18
  f8 = double(float(std::fma(f2, f9, -f18)));
  // lfs f2,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f2 = double(temp.f32);
  // lfs f18,92(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 92);
  f18 = double(temp.f32);
  // lfs f19,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f19 = double(temp.f32);
  // fmadds f19,f14,f9,f19
  f19 = double(float(std::fma(f14, f9, f19)));
  // fmadds f9,f7,f9,f17
  f9 = double(float(std::fma(f7, f9, f17)));
  // lfs f17,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f17 = double(temp.f32);
  // fsubs f7,f20,f8
  f7 = double(float(f20 - f8));
  // fadds f8,f8,f20
  f8 = double(float(f8 + f20));
  // fsubs f20,f19,f9
  f20 = double(float(f19 - f9));
  // fadds f9,f9,f19
  f9 = double(float(f9 + f19));
  // stfs f9,-156(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -156, temp.u32);
  // lfs f9,120(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 120);
  f9 = double(temp.f32);
  // stfs f9,-184(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // lfs f9,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f9 = double(temp.f32);
  // lfs f19,88(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 88);
  f19 = double(temp.f32);
  // stfs f9,-188(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -188, temp.u32);
  // fsubs f9,f2,f18
  f9 = double(float(f2 - f18));
  // fadds f19,f17,f19
  f19 = double(float(f17 + f19));
  // fmr f17,f9
  f17 = f9;
  // fmr f14,f9
  f14 = f9;
  // fmuls f9,f19,f12
  f9 = double(float(f19 * f12));
  // fmuls f19,f19,f10
  f19 = double(float(f19 * f10));
  // stfs f9,-176(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // fsubs f16,f16,f15
  f16 = double(float(f16 - f15));
  // stfs f16,-192(r1)
  temp.f32 = float(f16);
  REX_STORE_U32(uint32_t(r1) + -192, temp.u32);
  // fadds f2,f18,f2
  f2 = double(float(f18 + f2));
  // lfs f15,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f15 = double(temp.f32);
  // lfs f16,-188(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -188);
  f16 = double(temp.f32);
  // fadds f9,f16,f15
  f9 = double(float(f16 + f15));
  // fmuls f15,f9,f13
  f15 = double(float(f9 * f13));
  // lfs f16,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f16 = double(temp.f32);
  // stfs f19,-176(r1)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // fmsubs f19,f17,f10,f16
  f19 = double(float(std::fma(f17, f10, -f16)));
  // lfs f16,-192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -192);
  f16 = double(temp.f32);
  // stfs f2,-192(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -192, temp.u32);
  // lfs f2,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f2 = double(temp.f32);
  // lfs f17,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f17 = double(temp.f32);
  // fmadds f17,f14,f12,f17
  f17 = double(float(std::fma(f14, f12, f17)));
  // fmr f14,f9
  f14 = f9;
  // fmuls f9,f16,f13
  f9 = double(float(f16 * f13));
  // stfs f9,-176(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // lfs f9,88(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 88);
  f9 = double(temp.f32);
  // fsubs f9,f2,f9
  f9 = double(float(f2 - f9));
  // fmsubs f2,f16,f11,f15
  f2 = double(float(std::fma(f16, f11, -f15)));
  // stfs f2,-180(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -180, temp.u32);
  // fmr f16,f9
  f16 = f9;
  // lfs f15,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f15 = double(temp.f32);
  // fmadds f2,f14,f11,f15
  f2 = double(float(std::fma(f14, f11, f15)));
  // stfs f2,-176(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -176, temp.u32);
  // lfs f2,-192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -192);
  f2 = double(temp.f32);
  // fmr f15,f9
  f15 = f9;
  // fmuls f18,f2,f11
  f18 = double(float(f2 * f11));
  // lfs f9,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f9 = double(temp.f32);
  // fmuls f14,f2,f13
  f14 = double(float(f2 * f13));
  // lfs f2,124(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 124);
  f2 = double(temp.f32);
  // fadds f9,f2,f9
  f9 = double(float(f2 + f9));
  // stfs f9,-192(r1)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r1) + -192, temp.u32);
  // lfs f2,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f2 = double(temp.f32);
  // lfs f9,-188(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -188);
  f9 = double(temp.f32);
  // fsubs f9,f9,f2
  f9 = double(float(f9 - f2));
  // lfs f2,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f2 = double(temp.f32);
  // fadds f2,f2,f19
  f2 = double(float(f2 + f19));
  // stfs f2,-184(r1)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r1) + -184, temp.u32);
  // lfs f2,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f2 = double(temp.f32);
  // fsubs f2,f19,f2
  f2 = double(float(f19 - f2));
  // fmadds f13,f16,f13,f18
  f13 = double(float(std::fma(f16, f13, f18)));
  // fmsubs f11,f15,f11,f14
  f11 = double(float(std::fma(f15, f11, -f14)));
  // lfs f19,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f19 = double(temp.f32);
  // fadds f19,f19,f17
  f19 = double(float(f19 + f17));
  // stfs f19,-180(r1)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r1) + -180, temp.u32);
  // lfs f19,-176(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -176);
  f19 = double(temp.f32);
  // fsubs f19,f17,f19
  f19 = double(float(f17 - f19));
  // lfs f18,-192(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -192);
  f18 = double(temp.f32);
  // fmuls f17,f9,f10
  f17 = double(float(f9 * f10));
  // fmuls f16,f18,f10
  f16 = double(float(f18 * f10));
  // lfs f15,-184(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -184);
  f15 = double(temp.f32);
  // fmsubs f10,f18,f12,f17
  f10 = double(float(std::fma(f18, f12, -f17)));
  // fmadds f12,f9,f12,f16
  f12 = double(float(std::fma(f9, f12, f16)));
  // fadds f17,f15,f4
  f17 = double(float(f15 + f4));
  // fadds f18,f29,f23
  f18 = double(float(f29 + f23));
  // fsubs f29,f23,f29
  f29 = double(float(f23 - f29));
  // fsubs f4,f4,f15
  f4 = double(float(f4 - f15));
  // fadds f9,f10,f13
  f9 = double(float(f10 + f13));
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // fadds f10,f12,f11
  f10 = double(float(f12 + f11));
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // fadds f11,f31,f26
  f11 = double(float(f31 + f26));
  // fsubs f31,f26,f31
  f31 = double(float(f26 - f31));
  // fadds f14,f17,f11
  f14 = double(float(f17 + f11));
  // stfs f14,0(r3)
  temp.f32 = float(f14);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fsubs f11,f11,f17
  f11 = double(float(f11 - f17));
  // stfs f11,8(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // lfs f16,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f16 = double(temp.f32);
  // fadds f16,f16,f24
  f16 = double(float(f16 + f24));
  // lfs f26,-180(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -180);
  f26 = double(temp.f32);
  // fsubs f26,f24,f26
  f26 = double(float(f24 - f26));
  // fsubs f11,f18,f16
  f11 = double(float(f18 - f16));
  // stfs f11,12(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fadds f14,f16,f18
  f14 = double(float(f16 + f18));
  // stfs f14,4(r3)
  temp.f32 = float(f14);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fsubs f11,f6,f19
  f11 = double(float(f6 - f19));
  // fadds f18,f2,f5
  f18 = double(float(f2 + f5));
  // fsubs f24,f11,f18
  f24 = double(float(f11 - f18));
  // fadds f23,f18,f11
  f23 = double(float(f18 + f11));
  // fadds f11,f19,f6
  f11 = double(float(f19 + f6));
  // fsubs f6,f5,f2
  f6 = double(float(f5 - f2));
  // fsubs f5,f31,f26
  f5 = double(float(f31 - f26));
  // stfs f5,16(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // fadds f5,f4,f29
  f5 = double(float(f4 + f29));
  // stfs f5,20(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // fadds f5,f26,f31
  f5 = double(float(f26 + f31));
  // stfs f5,24(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // fsubs f5,f29,f4
  f5 = double(float(f29 - f4));
  // stfs f5,28(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // fmuls f2,f24,f0
  f2 = double(float(f24 * f0));
  // lfs f29,-172(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -172);
  f29 = double(temp.f32);
  // fsubs f5,f25,f28
  f5 = double(float(f25 - f28));
  // lfs f26,-168(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -168);
  f26 = double(temp.f32);
  // fadds f4,f30,f27
  f4 = double(float(f30 + f27));
  // fmuls f31,f23,f0
  f31 = double(float(f23 * f0));
  // fsubs f24,f11,f6
  f24 = double(float(f11 - f6));
  // fadds f23,f6,f11
  f23 = double(float(f6 + f11));
  // fadds f11,f12,f29
  f11 = double(float(f12 + f29));
  // fsubs f6,f26,f13
  f6 = double(float(f26 - f13));
  // fsubs f12,f29,f12
  f12 = double(float(f29 - f12));
  // fadds f19,f2,f5
  f19 = double(float(f2 + f5));
  // stfs f19,32(r3)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // fsubs f5,f5,f2
  f5 = double(float(f5 - f2));
  // stfs f5,40(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // fsubs f5,f4,f31
  f5 = double(float(f4 - f31));
  // stfs f5,44(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // fadds f19,f31,f4
  f19 = double(float(f31 + f4));
  // stfs f19,36(r3)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // fsubs f4,f27,f30
  f4 = double(float(f27 - f30));
  // fmuls f2,f24,f0
  f2 = double(float(f24 * f0));
  // fadds f5,f28,f25
  f5 = double(float(f28 + f25));
  // fmuls f31,f23,f0
  f31 = double(float(f23 * f0));
  // fsubs f30,f11,f6
  f30 = double(float(f11 - f6));
  // fadds f28,f6,f11
  f28 = double(float(f6 + f11));
  // fadds f6,f20,f22
  f6 = double(float(f20 + f22));
  // fadds f11,f2,f4
  f11 = double(float(f2 + f4));
  // stfs f11,52(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // fsubs f11,f5,f31
  f11 = double(float(f5 - f31));
  // stfs f11,48(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // fadds f11,f31,f5
  f11 = double(float(f31 + f5));
  // stfs f11,56(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // fsubs f11,f4,f2
  f11 = double(float(f4 - f2));
  // lfs f2,-164(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -164);
  f2 = double(temp.f32);
  // stfs f11,60(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // fsubs f5,f2,f9
  f5 = double(float(f2 - f9));
  // fadds f11,f7,f21
  f11 = double(float(f7 + f21));
  // lfs f31,-160(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -160);
  f31 = double(temp.f32);
  // fsubs f4,f31,f10
  f4 = double(float(f31 - f10));
  // fadds f9,f9,f2
  f9 = double(float(f9 + f2));
  // fadds f10,f10,f31
  f10 = double(float(f10 + f31));
  // fadds f27,f5,f11
  f27 = double(float(f5 + f11));
  // stfs f27,64(r3)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r3) + 64, temp.u32);
  // fsubs f11,f11,f5
  f11 = double(float(f11 - f5));
  // stfs f11,72(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 72, temp.u32);
  // fsubs f11,f6,f4
  f11 = double(float(f6 - f4));
  // stfs f11,76(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 76, temp.u32);
  // fsubs f11,f21,f7
  f11 = double(float(f21 - f7));
  // fsubs f7,f22,f20
  f7 = double(float(f22 - f20));
  // fadds f27,f4,f6
  f27 = double(float(f4 + f6));
  // stfs f27,68(r3)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // fadds f6,f9,f7
  f6 = double(float(f9 + f7));
  // stfs f6,84(r3)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r3) + 84, temp.u32);
  // fsubs f6,f11,f10
  f6 = double(float(f11 - f10));
  // stfs f6,80(r3)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // stfs f11,88(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 88, temp.u32);
  // lfs f6,-156(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -156);
  f6 = double(temp.f32);
  // fsubs f11,f7,f9
  f11 = double(float(f7 - f9));
  // stfs f11,92(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 92, temp.u32);
  // fmuls f10,f30,f0
  f10 = double(float(f30 * f0));
  // fsubs f11,f1,f6
  f11 = double(float(f1 - f6));
  // fadds f9,f8,f3
  f9 = double(float(f8 + f3));
  // fmuls f7,f28,f0
  f7 = double(float(f28 * f0));
  // fadds f5,f10,f11
  f5 = double(float(f10 + f11));
  // stfs f5,96(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 96, temp.u32);
  // fsubs f11,f11,f10
  f11 = double(float(f11 - f10));
  // stfs f11,104(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 104, temp.u32);
  // fadds f10,f7,f9
  f10 = double(float(f7 + f9));
  // stfs f10,100(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 100, temp.u32);
  // fadds f13,f13,f26
  f13 = double(float(f13 + f26));
  // fsubs f11,f9,f7
  f11 = double(float(f9 - f7));
  // stfs f11,108(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 108, temp.u32);
  // fsubs f10,f3,f8
  f10 = double(float(f3 - f8));
  // fadds f11,f6,f1
  f11 = double(float(f6 + f1));
  // fsubs f9,f12,f13
  f9 = double(float(f12 - f13));
  // fadds f12,f13,f12
  f12 = double(float(f13 + f12));
  // fmuls f13,f9,f0
  f13 = double(float(f9 * f0));
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // fadds f12,f13,f10
  f12 = double(float(f13 + f10));
  // stfs f12,116(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 116, temp.u32);
  // fsubs f12,f11,f0
  f12 = double(float(f11 - f0));
  // stfs f12,112(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 112, temp.u32);
  // fadds f0,f0,f11
  f0 = double(float(f0 + f11));
  // stfs f0,120(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 120, temp.u32);
  // fsubs f0,f10,f13
  f0 = double(float(f10 - f13));
  // stfs f0,124(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 124, temp.u32);
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a792c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_825A3980: lifted from generated/ by lift.py.
double f825A3980(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A3980 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a7904
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lfs f10,52(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 52);
  f10 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f9,48(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 48);
  f9 = double(temp.f32);
  // lfs f8,20(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20);
  f8 = double(temp.f32);
  // lfs f11,16(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 16);
  f11 = double(temp.f32);
  // fadds f26,f8,f9
  f26 = double(float(f8 + f9));
  // fsubs f27,f11,f10
  f27 = double(float(f11 - f10));
  // lfs f28,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f28 = double(temp.f32);
  // lfs f0,8732(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8732);
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f29,32(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 32);
  f29 = double(temp.f32);
  // fadds f11,f10,f11
  f11 = double(float(f10 + f11));
  // fadds f24,f28,f29
  f24 = double(float(f28 + f29));
  // lfs f12,0(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 0);
  f12 = double(temp.f32);
  // fsubs f29,f28,f29
  f29 = double(float(f28 - f29));
  // lfs f30,36(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 36);
  f30 = double(temp.f32);
  // fsubs f10,f8,f9
  f10 = double(float(f8 - f9));
  // lfs f7,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f7 = double(temp.f32);
  // lfs f13,8728(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8728);
  f13 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f25,f12,f30
  f25 = double(float(f12 - f30));
  // lfs f6,44(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 44);
  f6 = double(temp.f32);
  // fadds f30,f30,f12
  f30 = double(float(f30 + f12));
  // lfs f5,40(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 40);
  f5 = double(temp.f32);
  // lfs f4,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f4 = double(temp.f32);
  // fsubs f28,f27,f26
  f28 = double(float(f27 - f26));
  // lfs f3,24(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 24);
  f3 = double(temp.f32);
  // fadds f27,f26,f27
  f27 = double(float(f26 + f27));
  // lfs f12,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f12 = double(temp.f32);
  // lfs f2,60(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 60);
  f2 = double(temp.f32);
  // lfs f1,56(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 56);
  f1 = double(temp.f32);
  // lfs f31,28(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 28);
  f31 = double(temp.f32);
  // fmuls f9,f28,f12
  f9 = double(float(f28 * f12));
  // fsubs f28,f11,f10
  f28 = double(float(f11 - f10));
  // fmuls f8,f27,f12
  f8 = double(float(f27 * f12));
  // fadds f27,f10,f11
  f27 = double(float(f10 + f11));
  // fsubs f11,f7,f6
  f11 = double(float(f7 - f6));
  // fadds f10,f4,f5
  f10 = double(float(f4 + f5));
  // fmuls f28,f28,f12
  f28 = double(float(f28 * f12));
  // fmuls f12,f27,f12
  f12 = double(float(f27 * f12));
  // fmuls f26,f11,f13
  f26 = double(float(f11 * f13));
  // fmr f27,f11
  f27 = f11;
  // fadds f11,f6,f7
  f11 = double(float(f6 + f7));
  // fmuls f7,f10,f13
  f7 = double(float(f10 * f13));
  // fmr f6,f10
  f6 = f10;
  // fsubs f10,f4,f5
  f10 = double(float(f4 - f5));
  // fmr f5,f11
  f5 = f11;
  // fmsubs f7,f27,f0,f7
  f7 = double(float(std::fma(f27, f0, -f7)));
  // fmadds f6,f6,f0,f26
  f6 = double(float(std::fma(f6, f0, f26)));
  // fmuls f26,f10,f13
  f26 = double(float(f10 * f13));
  // fmuls f27,f10,f0
  f27 = double(float(f10 * f0));
  // fmr f4,f11
  f4 = f11;
  // fsubs f11,f3,f2
  f11 = double(float(f3 - f2));
  // fadds f10,f31,f1
  f10 = double(float(f31 + f1));
  // fmsubs f5,f5,f13,f27
  f5 = double(float(std::fma(f5, f13, -f27)));
  // fmadds f4,f4,f0,f26
  f4 = double(float(std::fma(f4, f0, f26)));
  // fmr f27,f11
  f27 = f11;
  // fmr f26,f11
  f26 = f11;
  // fadds f11,f2,f3
  f11 = double(float(f2 + f3));
  // fmuls f3,f10,f0
  f3 = double(float(f10 * f0));
  // fmuls f2,f10,f13
  f2 = double(float(f10 * f13));
  // fsubs f10,f31,f1
  f10 = double(float(f31 - f1));
  // fmuls f23,f11,f13
  f23 = double(float(f11 * f13));
  // fmsubs f3,f27,f13,f3
  f3 = double(float(std::fma(f27, f13, -f3)));
  // fmadds f2,f26,f0,f2
  f2 = double(float(std::fma(f26, f0, f2)));
  // fmuls f27,f10,f13
  f27 = double(float(f10 * f13));
  // fmr f31,f11
  f31 = f11;
  // fmr f26,f10
  f26 = f10;
  // fadds f13,f9,f25
  f13 = double(float(f9 + f25));
  // fadds f11,f8,f24
  f11 = double(float(f8 + f24));
  // fadds f10,f3,f7
  f10 = double(float(f3 + f7));
  // fadds f1,f2,f6
  f1 = double(float(f2 + f6));
  // fmsubs f31,f31,f0,f27
  f31 = double(float(std::fma(f31, f0, -f27)));
  // fmadds f0,f26,f0,f23
  f0 = double(float(std::fma(f26, f0, f23)));
  // fadds f27,f10,f13
  f27 = double(float(f10 + f13));
  // stfs f27,0(r3)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // stfs f13,8(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // fadds f27,f1,f11
  f27 = double(float(f1 + f11));
  // stfs f27,4(r3)
  temp.f32 = float(f27);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fsubs f13,f11,f1
  f13 = double(float(f11 - f1));
  // stfs f13,12(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fsubs f11,f24,f8
  f11 = double(float(f24 - f8));
  // fsubs f10,f7,f3
  f10 = double(float(f7 - f3));
  // fsubs f13,f25,f9
  f13 = double(float(f25 - f9));
  // fsubs f9,f6,f2
  f9 = double(float(f6 - f2));
  // fadds f8,f10,f11
  f8 = double(float(f10 + f11));
  // stfs f8,20(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // fsubs f8,f13,f9
  f8 = double(float(f13 - f9));
  // stfs f8,16(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // fadds f13,f9,f13
  f13 = double(float(f9 + f13));
  // stfs f13,24(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // fsubs f13,f11,f10
  f13 = double(float(f11 - f10));
  // stfs f13,28(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // fsubs f13,f30,f12
  f13 = double(float(f30 - f12));
  // fsubs f10,f5,f31
  f10 = double(float(f5 - f31));
  // fsubs f9,f4,f0
  f9 = double(float(f4 - f0));
  // fadds f11,f28,f29
  f11 = double(float(f28 + f29));
  // fadds f0,f0,f4
  f0 = double(float(f0 + f4));
  // fadds f8,f10,f13
  f8 = double(float(f10 + f13));
  // stfs f8,32(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // stfs f13,40(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // fsubs f13,f11,f9
  f13 = double(float(f11 - f9));
  // stfs f13,44(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // fadds f13,f12,f30
  f13 = double(float(f12 + f30));
  // fadds f8,f9,f11
  f8 = double(float(f9 + f11));
  // stfs f8,36(r3)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // fsubs f12,f29,f28
  f12 = double(float(f29 - f28));
  // fadds f11,f31,f5
  f11 = double(float(f31 + f5));
  // fadds f10,f11,f12
  f10 = double(float(f11 + f12));
  // stfs f10,52(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // fsubs f10,f13,f0
  f10 = double(float(f13 - f0));
  // stfs f10,48(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,56(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // fsubs f0,f12,f11
  f0 = double(float(f12 - f11));
  // stfs f0,60(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // addi r12,r1,-8
  r12 = uint64_t(int64_t(int64_t(r1) + -8));
  // bl 0x825a7950
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return f1;
}

// sub_825A3B80: lifted from generated/ by lift.py.
double f825A3B80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A3B80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a790c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // extsw r10,r29
  r10 = uint64_t(int64_t(int32_t(r29)));
  // mr r31,r4
  r31 = r4;
  // lfs f30,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // fmr f31,f30
  f31 = f30;
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f0,8752(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8752)));
  // fdiv f0,f0,f13
  f0 = f0 / f13;
  // frsp f25,f0
  f25 = double(float(f0));
  // fmr f29,f25
  f29 = f25;
  // fmr f1,f29
  f1 = f29;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A3BD4, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f28,f1
  lifted::ScalarFloatMode();
  f28 = f1;
  // fmr f1,f29
  f1 = f29;
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A3BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f28
  lifted::ScalarFloatMode();
  f0 = double(float(f28));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f1
  f13 = double(float(f1));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f12,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f12 = double(temp.f32);
  // srawi r11,r29,1
  ca = (int32_t(r29) < 0) & ((uint32_t(r29) & 0x1) != 0);
  r11 = uint64_t(int64_t(int32_t(r29) >> 1));
  // lfs f11,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f11 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f28,7140(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7140);
  f28 = double(temp.f32);
  // fmuls f13,f13,f13
  f13 = double(float(f13 * f13));
  // fmuls f27,f0,f12
  f27 = double(float(f0 * f12));
  // fnmsubs f26,f13,f12,f11
  f26 = double(float(-std::fma(f13, f12, -f11)));
  // fmuls f29,f27,f12
  f29 = double(float(f27 * f12));
loc_825A3C18:
  // addi r30,r11,-256
  r30 = uint64_t(int64_t(int64_t(r11) + -256));
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // bge cr6,0x825a3c28
  if (!cr6.lt) goto loc_825A3C28;
  // li r30,4
  r30 = uint64_t(int64_t(4));
loc_825A3C28:
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // blt cr6,0x825a3d40
  if (cr6.lt) goto loc_825A3D40;
  // subf r10,r11,r29
  r10 = r29 - r11;
  // addi r9,r11,3
  r9 = uint64_t(int64_t(int64_t(r11) + 3));
  // subf r7,r30,r11
  r7 = r11 - r30;
  // addi r8,r10,-1
  r8 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r10,r9,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r7,30,2,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0x3FFFFFFF;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r7,r9,1
  r7 = uint64_t(int64_t(int64_t(r9) + 1));
loc_825A3C5C:
  // fsubs f11,f28,f13
  lifted::ScalarFloatMode();
  f11 = double(float(f28 - f13));
  // lfs f9,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f9 = double(temp.f32);
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // fmr f7,f13
  f7 = f13;
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // fmr f8,f13
  f8 = f13;
  // fadds f10,f12,f10
  f10 = double(float(f12 + f10));
  // addi r8,r11,8
  r8 = uint64_t(int64_t(int64_t(r11) + 8));
  // addi r5,r10,-8
  r5 = uint64_t(int64_t(int64_t(r10) + -8));
  // fmadds f30,f29,f0,f30
  f30 = double(float(std::fma(f29, f0, f30)));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r6,r10,-12
  r6 = uint64_t(int64_t(int64_t(r10) + -12));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // fmadds f31,f11,f29,f31
  f31 = double(float(std::fma(f11, f29, f31)));
  // lfs f11,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f11 = double(temp.f32);
  // fsubs f9,f11,f9
  f9 = double(float(f11 - f9));
  // fmuls f5,f10,f0
  f5 = double(float(f10 * f0));
  // fsubs f6,f28,f30
  f6 = double(float(f28 - f30));
  // fmadds f13,f29,f31,f13
  f13 = double(float(std::fma(f29, f31, f13)));
  // fmuls f4,f9,f0
  f4 = double(float(f9 * f0));
  // fmsubs f9,f9,f8,f5
  f9 = double(float(std::fma(f9, f8, -f5)));
  // fmadds f0,f6,f29,f0
  f0 = double(float(std::fma(f6, f29, f0)));
  // fmadds f10,f10,f7,f4
  f10 = double(float(std::fma(f10, f7, f4)));
  // fsubs f12,f12,f10
  f12 = double(float(f12 - f10));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f12,f11,f9
  f12 = double(float(f11 - f9));
  // stfs f12,-4(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // addi r10,r10,-16
  r10 = uint64_t(int64_t(int64_t(r10) + -16));
  // lfs f11,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f11 = double(temp.f32);
  // fsubs f12,f12,f10
  f12 = double(float(f12 - f10));
  // fadds f11,f9,f11
  f11 = double(float(f9 + f11));
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f11,-4(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lfs f11,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f11 = double(temp.f32);
  // lfs f9,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f9 = double(temp.f32);
  // fadds f9,f11,f9
  f9 = double(float(f11 + f9));
  // lfs f12,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f12 = double(temp.f32);
  // lfs f10,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f10 = double(temp.f32);
  // fsubs f10,f12,f10
  f10 = double(float(f12 - f10));
  // fmuls f8,f9,f31
  f8 = double(float(f9 * f31));
  // fmuls f7,f9,f30
  f7 = double(float(f9 * f30));
  // fmsubs f9,f10,f30,f8
  f9 = double(float(std::fma(f10, f30, -f8)));
  // fmadds f10,f10,f31,f7
  f10 = double(float(std::fma(f10, f31, f7)));
  // fsubs f12,f12,f9
  f12 = double(float(f12 - f9));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // fsubs f12,f11,f10
  f12 = double(float(f11 - f10));
  // stfs f12,0(r5)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // lfs f12,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f12 = double(temp.f32);
  // lfs f11,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f11 = double(temp.f32);
  // fadds f12,f12,f9
  f12 = double(float(f12 + f9));
  // fsubs f11,f11,f10
  f11 = double(float(f11 - f10));
  // stfs f12,0(r9)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // stfs f11,0(r8)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // bne cr6,0x825a3c5c
  if (!cr6.eq) goto loc_825A3C5C;
loc_825A3D40:
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // beq cr6,0x825a3da8
  if (cr6.eq) goto loc_825A3DA8;
  // extsw r11,r30
  r11 = uint64_t(int64_t(int32_t(r30)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f25
  f31 = double(float(f0 * f25));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A3D68, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f28
  f31 = double(float(f0 * f28));
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A3D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f12,f1
  lifted::ScalarFloatMode();
  f12 = double(float(f1));
  // mr r11,r30
  r11 = r30;
  // fmr f0,f31
  f0 = f31;
  // fmuls f13,f27,f31
  f13 = double(float(f27 * f31));
  // fsubs f30,f28,f31
  f30 = double(float(f28 - f31));
  // fmuls f31,f12,f28
  f31 = double(float(f12 * f28));
  // fmuls f12,f27,f31
  f12 = double(float(f27 * f31));
  // fmsubs f12,f26,f0,f12
  f12 = double(float(std::fma(f26, f0, -f12)));
  // fmadds f0,f26,f31,f13
  f0 = double(float(std::fma(f26, f31, f13)));
  // fsubs f13,f28,f12
  f13 = double(float(f28 - f12));
  // b 0x825a3c18
  goto loc_825A3C18;
loc_825A3DA8:
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lfs f11,12(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f11 = double(temp.f32);
  // addi r10,r29,-2
  r10 = uint64_t(int64_t(int64_t(r29) + -2));
  // lfs f12,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f12 = double(temp.f32);
  // add r11,r11,r31
  r11 = r11 + r31;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lfs f9,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f9 = double(temp.f32);
  // lfsx f10,r10,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  f10 = double(temp.f32);
  // fadds f9,f11,f9
  f9 = double(float(f11 + f9));
  // fsubs f10,f12,f10
  f10 = double(float(f12 - f10));
  // fmuls f8,f9,f0
  f8 = double(float(f9 * f0));
  // fmuls f7,f10,f0
  f7 = double(float(f10 * f0));
  // fmsubs f0,f10,f13,f8
  f0 = double(float(std::fma(f10, f13, -f8)));
  // fmadds f13,f9,f13,f7
  f13 = double(float(std::fma(f9, f13, f7)));
  // fsubs f12,f12,f0
  f12 = double(float(f12 - f0));
  // stfs f12,8(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // fsubs f12,f11,f13
  f12 = double(float(f11 - f13));
  // stfs f12,12(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // lfsx f12,r10,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  f12 = double(temp.f32);
  // lfs f11,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f11 = double(temp.f32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // fsubs f13,f11,f13
  f13 = double(float(f11 - f13));
  // stfsx f0,r10,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r31), temp.u32);
  // stfs f13,-4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-32
  r12 = uint64_t(int64_t(int64_t(r1) + -32));
  // bl 0x825a7958
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6edc
  // (left out: dead in C++)
  return f1;
}

// sub_825A3E18: lifted from generated/ by lift.py.
lifted::Result f825A3E18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A3E18 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-64
  r12 = uint64_t(int64_t(int64_t(r1) + -64));
  // bl 0x825a78fc
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // srawi r27,r3,3
  ca = (int32_t(r3) < 0) & ((uint32_t(r3) & 0x7) != 0);
  r27 = uint64_t(int64_t(int32_t(r3) >> 3));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // rlwinm r30,r27,1,0,30
  r30 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 1) & 0xFFFFFFFE;
  // lis r8,-32251
  r8 = uint64_t(int64_t(-2113601536));
  // extsw r10,r30
  r10 = uint64_t(int64_t(int32_t(r30)));
  // mr r31,r4
  r31 = r4;
  // lfs f31,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // rlwinm r26,r30,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // fmr f29,f31
  f29 = f31;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f30,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // rlwinm r11,r30,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // fmr f28,f30
  f28 = f30;
  // add r9,r11,r30
  r9 = r11 + r30;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r11,r26,r31
  r11 = r26 + r31;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r9,r9,r31
  r9 = r9 + r31;
  // lfs f7,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f7 = double(temp.f32);
  // lfs f9,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f9 = double(temp.f32);
  // lfs f10,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f10 = double(temp.f32);
  // fadds f5,f13,f9
  f5 = double(float(f13 + f9));
  // fsubs f13,f13,f9
  f13 = double(float(f13 - f9));
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f11,f12
  f11 = double(std::bit_cast<int64_t>(f12));
  // lfd f12,-6072(r8)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r8) + -6072)));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f26,8680(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 8680);
  f26 = double(temp.f32);
  // fdiv f12,f12,f11
  f12 = f12 / f11;
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // fadds f8,f0,f11
  f8 = double(float(f0 + f11));
  // fsubs f0,f0,f11
  f0 = double(float(f0 - f11));
  // lfs f11,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f11 = double(temp.f32);
  // frsp f22,f12
  f22 = double(float(f12));
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fadds f6,f12,f10
  f6 = double(float(f12 + f10));
  // fsubs f12,f12,f10
  f12 = double(float(f12 - f10));
  // fadds f10,f7,f11
  f10 = double(float(f7 + f11));
  // fsubs f11,f7,f11
  f11 = double(float(f7 - f11));
  // fmuls f27,f22,f26
  f27 = double(float(f22 * f26));
  // fadds f9,f6,f8
  f9 = double(float(f6 + f8));
  // stfs f9,0(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // fsubs f9,f8,f6
  f9 = double(float(f8 - f6));
  // fadds f8,f12,f13
  f8 = double(float(f12 + f13));
  // fsubs f13,f13,f12
  f13 = double(float(f13 - f12));
  // fadds f12,f10,f5
  f12 = double(float(f10 + f5));
  // stfs f12,4(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // fsubs f12,f5,f10
  f12 = double(float(f5 - f10));
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f9,0(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fsubs f12,f0,f11
  f12 = double(float(f0 - f11));
  // stfs f12,0(r10)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // stfs f8,4(r10)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fmr f1,f27
  f1 = f27;
  // stfs f13,4(r9)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A3F24, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f27
  f1 = f27;
  // frsp f24,f0
  f24 = double(float(f0));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A3F34, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f23,f1
  lifted::ScalarFloatMode();
  f23 = double(float(f1));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // fmr f0,f24
  f0 = f24;
  // addi r25,r27,-4
  r25 = uint64_t(int64_t(int64_t(r27) + -4));
  // lfs f21,8696(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8696);
  f21 = double(temp.f32);
  // fmuls f27,f23,f26
  f27 = double(float(f23 * f26));
  // fmr f13,f23
  f13 = f23;
  // fmuls f12,f27,f24
  f12 = double(float(f27 * f24));
  // fmuls f12,f12,f26
  f12 = double(float(f12 * f26));
  // fmr f11,f12
  f11 = f12;
  // fnmsubs f12,f12,f24,f23
  f12 = double(float(-std::fma(f12, f24, -f23)));
  // fnmsubs f11,f11,f23,f24
  f11 = double(float(-std::fma(f11, f23, -f24)));
  // fmuls f26,f12,f26
  f26 = double(float(f12 * f26));
loc_825A3F6C:
  // addi r28,r11,128
  r28 = uint64_t(int64_t(int64_t(r11) + 128));
  // cmpw cr6,r28,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r25), so);
  // ble cr6,0x825a3f7c
  if (!cr6.gt) goto loc_825A3F7C;
  // mr r28,r25
  r28 = r25;
loc_825A3F7C:
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmpw cr6,r11,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r28), so);
  // bge cr6,0x825a4300
  if (!cr6.lt) goto loc_825A4300;
  // add r8,r11,r30
  r8 = r11 + r30;
  // subf r9,r11,r28
  r9 = r28 - r11;
  // addi r6,r11,2
  r6 = uint64_t(int64_t(int64_t(r11) + 2));
  // addi r5,r8,2
  r5 = uint64_t(int64_t(int64_t(r8) + 2));
  // addi r4,r9,-1
  r4 = uint64_t(int64_t(int64_t(r9) + -1));
  // rlwinm r7,r30,1,0,30
  r7 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r8,r6,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // neg r10,r11
  r10 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // rlwinm r6,r4,30,2,31
  r6 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 30) & 0x3FFFFFFF;
  // rlwinm r9,r5,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // add r5,r7,r11
  r5 = r7 + r11;
  // subf r7,r11,r7
  r7 = r7 - r11;
  // add r4,r10,r30
  r4 = r10 + r30;
  // addi r3,r6,1
  r3 = uint64_t(int64_t(int64_t(r6) + 1));
  // add r10,r10,r26
  r10 = r10 + r26;
  // addi r5,r5,2
  r5 = uint64_t(int64_t(int64_t(r5) + 2));
  // rlwinm r6,r30,1,0,30
  r6 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // addi r7,r7,-2
  r7 = uint64_t(int64_t(int64_t(r7) + -2));
  // addi r29,r10,-2
  r29 = uint64_t(int64_t(int64_t(r10) + -2));
  // add r6,r30,r6
  r6 = r30 + r6;
  // rlwinm r10,r5,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r5,r7,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r7,r29,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // add r29,r6,r11
  r29 = r6 + r11;
  // subf r11,r11,r6
  r11 = r6 - r11;
  // addi r6,r29,2
  r6 = uint64_t(int64_t(int64_t(r29) + 2));
  // addi r29,r11,-2
  r29 = uint64_t(int64_t(int64_t(r11) + -2));
  // addi r4,r4,-2
  r4 = uint64_t(int64_t(int64_t(r4) + -2));
  // rlwinm r11,r6,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r4,r4,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r29,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // add r8,r8,r31
  r8 = r8 + r31;
  // add r9,r9,r31
  r9 = r9 + r31;
  // add r4,r4,r31
  r4 = r4 + r31;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r5,r5,r31
  r5 = r5 + r31;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // add r6,r6,r31
  r6 = r6 + r31;
loc_825A4024:
  // lfs f9,-8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f9 = double(temp.f32);
  // fmadds f30,f27,f0,f30
  f30 = double(float(std::fma(f27, f0, f30)));
  // lfs f10,-8(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -8);
  f10 = double(temp.f32);
  // fnmsubs f31,f27,f13,f31
  f31 = double(float(-std::fma(f27, f13, -f31)));
  // lfs f7,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f7 = double(temp.f32);
  // fadds f2,f10,f9
  f2 = double(float(f10 + f9));
  // lfs f8,-4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -4);
  f8 = double(temp.f32);
  // fsubs f10,f10,f9
  f10 = double(float(f10 - f9));
  // lfs f6,-8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -8);
  f6 = double(temp.f32);
  // fadds f9,f7,f8
  f9 = double(float(f7 + f8));
  // lfs f5,-8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -8);
  f5 = double(temp.f32);
  // fsubs f8,f8,f7
  f8 = double(float(f8 - f7));
  // lfs f4,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f4 = double(temp.f32);
  // fadds f7,f6,f5
  f7 = double(float(f6 + f5));
  // lfs f3,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f3 = double(temp.f32);
  // fsubs f6,f5,f6
  f6 = double(float(f5 - f6));
  // fadds f5,f4,f3
  f5 = double(float(f4 + f3));
  // fsubs f4,f3,f4
  f4 = double(float(f3 - f4));
  // fnmsubs f29,f26,f12,f29
  f29 = double(float(-std::fma(f26, f12, -f29)));
  // fmadds f28,f26,f11,f28
  f28 = double(float(std::fma(f26, f11, f28)));
  // fadds f3,f7,f2
  f3 = double(float(f7 + f2));
  // stfs f3,-8(r8)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r8) + -8, temp.u32);
  // fsubs f7,f2,f7
  f7 = double(float(f2 - f7));
  // fadds f3,f5,f9
  f3 = double(float(f5 + f9));
  // stfs f3,-4(r8)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r8) + -4, temp.u32);
  // stfs f7,-8(r9)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r9) + -8, temp.u32);
  // fsubs f5,f9,f5
  f5 = double(float(f9 - f5));
  // fadds f7,f6,f8
  f7 = double(float(f6 + f8));
  // stfs f5,-4(r9)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r9) + -4, temp.u32);
  // fsubs f9,f10,f4
  f9 = double(float(f10 - f4));
  // fadds f10,f4,f10
  f10 = double(float(f4 + f10));
  // fmuls f3,f13,f7
  f3 = double(float(f13 * f7));
  // fmr f5,f9
  f5 = f9;
  // fmuls f2,f13,f9
  f2 = double(float(f13 * f9));
  // fsubs f9,f8,f6
  f9 = double(float(f8 - f6));
  // fmsubs f8,f0,f5,f3
  f8 = double(float(std::fma(f0, f5, -f3)));
  // stfs f8,-8(r10)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r10) + -8, temp.u32);
  // fmadds f8,f0,f7,f2
  f8 = double(float(std::fma(f0, f7, f2)));
  // stfs f8,-4(r10)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // fmuls f7,f11,f10
  f7 = double(float(f11 * f10));
  // lfs f8,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f8 = double(temp.f32);
  // fmuls f10,f12,f10
  f10 = double(float(f12 * f10));
  // fmadds f7,f12,f9,f7
  f7 = double(float(std::fma(f12, f9, f7)));
  // stfs f7,-8(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // fmsubs f10,f11,f9,f10
  f10 = double(float(std::fma(f11, f9, -f10)));
  // stfs f10,-4(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // lfs f9,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f9 = double(temp.f32);
  // lfs f10,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f10 = double(temp.f32);
  // lfs f6,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f6 = double(temp.f32);
  // fadds f4,f9,f10
  f4 = double(float(f9 + f10));
  // lfs f7,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f7 = double(temp.f32);
  // fsubs f10,f10,f9
  f10 = double(float(f10 - f9));
  // lfs f5,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f5 = double(temp.f32);
  // fadds f9,f7,f6
  f9 = double(float(f7 + f6));
  // fsubs f7,f7,f6
  f7 = double(float(f7 - f6));
  // fadds f6,f8,f5
  f6 = double(float(f8 + f5));
  // fsubs f8,f5,f8
  f8 = double(float(f5 - f8));
  // lfs f5,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f5 = double(temp.f32);
  // fadds f3,f6,f4
  f3 = double(float(f6 + f4));
  // fsubs f2,f4,f6
  f2 = double(float(f4 - f6));
  // lfs f6,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f6 = double(temp.f32);
  // fadds f4,f6,f5
  f4 = double(float(f6 + f5));
  // stfs f3,0(r8)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fsubs f6,f5,f6
  f6 = double(float(f5 - f6));
  // fadds f5,f4,f9
  f5 = double(float(f4 + f9));
  // stfs f5,4(r8)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // fsubs f5,f9,f4
  f5 = double(float(f9 - f4));
  // stfs f5,4(r9)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // fsubs f9,f10,f6
  f9 = double(float(f10 - f6));
  // stfs f2,0(r9)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fadds f5,f8,f7
  f5 = double(float(f8 + f7));
  // fadds f10,f6,f10
  f10 = double(float(f6 + f10));
  // fmr f4,f9
  f4 = f9;
  // fmuls f3,f30,f5
  f3 = double(float(f30 * f5));
  // fmuls f2,f30,f9
  f2 = double(float(f30 * f9));
  // fsubs f9,f7,f8
  f9 = double(float(f7 - f8));
  // fmsubs f8,f31,f4,f3
  f8 = double(float(std::fma(f31, f4, -f3)));
  // stfs f8,0(r10)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fmadds f8,f31,f5,f2
  f8 = double(float(std::fma(f31, f5, f2)));
  // stfs f8,4(r10)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // fmuls f8,f29,f10
  f8 = double(float(f29 * f10));
  // fmuls f10,f28,f10
  f10 = double(float(f28 * f10));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // fmadds f8,f28,f9,f8
  f8 = double(float(std::fma(f28, f9, f8)));
  // stfs f8,0(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // fmsubs f10,f29,f9,f10
  f10 = double(float(std::fma(f29, f9, -f10)));
  // stfs f10,4(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lfs f10,8(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 8);
  f10 = double(temp.f32);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // lfs f9,8(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8);
  f9 = double(temp.f32);
  // lfs f7,8(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8);
  f7 = double(temp.f32);
  // lfs f4,8(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8);
  f4 = double(temp.f32);
  // fadds f2,f10,f7
  f2 = double(float(f10 + f7));
  // lfs f5,12(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 12);
  f5 = double(temp.f32);
  // fadds f1,f4,f9
  f1 = double(float(f4 + f9));
  // lfs f8,12(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 12);
  f8 = double(temp.f32);
  // fsubs f10,f7,f10
  f10 = double(float(f7 - f10));
  // lfs f3,12(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 12);
  f3 = double(temp.f32);
  // fsubs f9,f4,f9
  f9 = double(float(f4 - f9));
  // lfs f6,12(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 12);
  f6 = double(temp.f32);
  // fadds f25,f3,f8
  f25 = double(float(f3 + f8));
  // fadds f7,f5,f6
  f7 = double(float(f5 + f6));
  // fsubs f6,f6,f5
  f6 = double(float(f6 - f5));
  // fsubs f8,f3,f8
  f8 = double(float(f3 - f8));
  // fadds f5,f1,f2
  f5 = double(float(f1 + f2));
  // stfs f5,8(r4)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r4) + 8, temp.u32);
  // fadds f5,f25,f7
  f5 = double(float(f25 + f7));
  // stfs f5,12(r4)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r4) + 12, temp.u32);
  // fsubs f5,f2,f1
  f5 = double(float(f2 - f1));
  // stfs f5,8(r5)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r5) + 8, temp.u32);
  // fsubs f7,f7,f25
  f7 = double(float(f7 - f25));
  // stfs f7,12(r5)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r5) + 12, temp.u32);
  // fadds f5,f9,f6
  f5 = double(float(f9 + f6));
  // fsubs f7,f10,f8
  f7 = double(float(f10 - f8));
  // fadds f10,f8,f10
  f10 = double(float(f8 + f10));
  // fsubs f9,f6,f9
  f9 = double(float(f6 - f9));
  // fmr f6,f12
  f6 = f12;
  // fmuls f4,f0,f5
  f4 = double(float(f0 * f5));
  // fmuls f3,f0,f7
  f3 = double(float(f0 * f7));
  // fnmsubs f0,f27,f30,f0
  f0 = double(float(-std::fma(f27, f30, -f0)));
  // fmsubs f7,f13,f7,f4
  f7 = double(float(std::fma(f13, f7, -f4)));
  // stfs f7,8(r6)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r6) + 8, temp.u32);
  // fmadds f8,f13,f5,f3
  f8 = double(float(std::fma(f13, f5, f3)));
  // stfs f8,12(r6)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r6) + 12, temp.u32);
  // fmuls f5,f12,f10
  f5 = double(float(f12 * f10));
  // lfs f8,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f8 = double(temp.f32);
  // fmr f7,f11
  f7 = f11;
  // fmuls f4,f11,f10
  f4 = double(float(f11 * f10));
  // lfs f10,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f10 = double(temp.f32);
  // fmadds f13,f27,f31,f13
  f13 = double(float(std::fma(f27, f31, f13)));
  // fnmsubs f11,f26,f28,f11
  f11 = double(float(-std::fma(f26, f28, -f11)));
  // fmadds f12,f26,f29,f12
  f12 = double(float(std::fma(f26, f29, f12)));
  // fmadds f7,f7,f9,f5
  f7 = double(float(std::fma(f7, f9, f5)));
  // stfs f7,8(r7)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r7) + 8, temp.u32);
  // fmsubs f9,f6,f9,f4
  f9 = double(float(std::fma(f6, f9, -f4)));
  // stfs f9,12(r7)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r7) + 12, temp.u32);
  // lfs f7,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f7 = double(temp.f32);
  // lfs f4,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f4 = double(temp.f32);
  // lfs f9,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f9 = double(temp.f32);
  // fadds f1,f8,f4
  f1 = double(float(f8 + f4));
  // lfs f5,4(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 4);
  f5 = double(temp.f32);
  // fadds f2,f9,f7
  f2 = double(float(f9 + f7));
  // lfs f6,4(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f6 = double(temp.f32);
  // fsubs f9,f9,f7
  f9 = double(float(f9 - f7));
  // lfs f3,4(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 4);
  f3 = double(temp.f32);
  // fadds f7,f5,f6
  f7 = double(float(f5 + f6));
  // fadds f25,f3,f10
  f25 = double(float(f3 + f10));
  // fsubs f6,f6,f5
  f6 = double(float(f6 - f5));
  // fsubs f8,f4,f8
  f8 = double(float(f4 - f8));
  // fsubs f10,f3,f10
  f10 = double(float(f3 - f10));
  // fadds f5,f1,f2
  f5 = double(float(f1 + f2));
  // stfs f5,0(r4)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // fadds f5,f25,f7
  f5 = double(float(f25 + f7));
  // stfs f5,4(r4)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // fsubs f5,f2,f1
  f5 = double(float(f2 - f1));
  // stfs f5,0(r5)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // fadds f5,f8,f6
  f5 = double(float(f8 + f6));
  // fsubs f7,f7,f25
  f7 = double(float(f7 - f25));
  // stfs f7,4(r5)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r5) + 4, temp.u32);
  // fsubs f7,f9,f10
  f7 = double(float(f9 - f10));
  // fmuls f4,f31,f5
  f4 = double(float(f31 * f5));
  // fmuls f3,f31,f7
  f3 = double(float(f31 * f7));
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // fadds f10,f10,f9
  f10 = double(float(f10 + f9));
  // addi r8,r8,16
  r8 = uint64_t(int64_t(int64_t(r8) + 16));
  // fsubs f9,f6,f8
  f9 = double(float(f6 - f8));
  // addi r5,r5,-16
  r5 = uint64_t(int64_t(int64_t(r5) + -16));
  // fmsubs f8,f30,f7,f4
  f8 = double(float(std::fma(f30, f7, -f4)));
  // stfs f8,0(r6)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // addi r4,r4,-16
  r4 = uint64_t(int64_t(int64_t(r4) + -16));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // fmadds f8,f30,f5,f3
  f8 = double(float(std::fma(f30, f5, f3)));
  // stfs f8,4(r6)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r6) + 4, temp.u32);
  // fmuls f8,f28,f10
  f8 = double(float(f28 * f10));
  // addi r6,r6,-16
  r6 = uint64_t(int64_t(int64_t(r6) + -16));
  // fmuls f10,f29,f10
  f10 = double(float(f29 * f10));
  // fmadds f8,f29,f9,f8
  f8 = double(float(std::fma(f29, f9, f8)));
  // stfs f8,0(r7)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fmsubs f10,f28,f9,f10
  f10 = double(float(std::fma(f28, f9, -f10)));
  // stfs f10,4(r7)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // addi r7,r7,-16
  r7 = uint64_t(int64_t(int64_t(r7) + -16));
  // bne cr6,0x825a4024
  if (!cr6.eq) goto loc_825A4024;
loc_825A4300:
  // cmpw cr6,r28,r25
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r25), so);
  // beq cr6,0x825a4374
  if (cr6.eq) goto loc_825A4374;
  // extsw r11,r28
  r11 = uint64_t(int64_t(int32_t(r28)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f22
  f31 = double(float(f0 * f22));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A4328, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A4338, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f30,f1
  lifted::ScalarFloatMode();
  f30 = double(float(f1));
  // mr r11,r28
  r11 = r28;
  // fmuls f13,f23,f31
  f13 = double(float(f23 * f31));
  // fmuls f0,f23,f30
  f0 = double(float(f23 * f30));
  // fmadds f13,f24,f30,f13
  f13 = double(float(std::fma(f24, f30, f13)));
  // fmuls f12,f30,f31
  f12 = double(float(f30 * f31));
  // fmsubs f0,f24,f31,f0
  f0 = double(float(std::fma(f24, f31, -f0)));
  // fmuls f12,f12,f21
  f12 = double(float(f12 * f21));
  // fmuls f11,f13,f0
  f11 = double(float(f13 * f0));
  // fnmsubs f29,f12,f30,f31
  f29 = double(float(-std::fma(f12, f30, -f31)));
  // fnmsubs f28,f12,f31,f30
  f28 = double(float(-std::fma(f12, f31, -f30)));
  // fmuls f10,f11,f21
  f10 = double(float(f11 * f21));
  // fnmsubs f11,f10,f13,f0
  f11 = double(float(-std::fma(f10, f13, -f0)));
  // fnmsubs f12,f10,f0,f13
  f12 = double(float(-std::fma(f10, f0, -f13)));
  // b 0x825a3f6c
  goto loc_825A3F6C;
loc_825A4374:
  // add r7,r30,r27
  r7 = r30 + r27;
  // addi r10,r27,-2
  r10 = uint64_t(int64_t(int64_t(r27) + -2));
  // add r6,r7,r30
  r6 = r7 + r30;
  // addi r9,r7,-2
  r9 = uint64_t(int64_t(int64_t(r7) + -2));
  // addi r8,r6,-2
  r8 = uint64_t(int64_t(int64_t(r6) + -2));
  // add r5,r6,r30
  r5 = r6 + r30;
  // rlwinm r29,r9,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r6,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r28,r10,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r3,r8,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r7,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lfsx f8,r29,r31
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  f8 = double(temp.f32);
  // rlwinm r8,r5,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // addi r4,r5,-2
  r4 = uint64_t(int64_t(int64_t(r5) + -2));
  // add r11,r11,r31
  r11 = r11 + r31;
  // lfsx f10,r28,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r31));
  f10 = double(temp.f32);
  // add r9,r9,r31
  r9 = r9 + r31;
  // lfsx f6,r3,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r31));
  f6 = double(temp.f32);
  // rlwinm r4,r4,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // fadds f3,f10,f6
  f3 = double(float(f10 + f6));
  // add r10,r10,r31
  r10 = r10 + r31;
  // fsubs f10,f10,f6
  f10 = double(float(f10 - f6));
  // add r8,r8,r31
  r8 = r8 + r31;
  // lfs f9,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f9 = double(temp.f32);
  // addi r30,r5,2
  r30 = uint64_t(int64_t(int64_t(r5) + 2));
  // lfs f5,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f5 = double(temp.f32);
  // addi r26,r7,2
  r26 = uint64_t(int64_t(int64_t(r7) + 2));
  // lfsx f4,r4,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + uint32_t(r31));
  f4 = double(temp.f32);
  // fadds f2,f9,f5
  f2 = double(float(f9 + f5));
  // lfs f7,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f7 = double(temp.f32);
  // fsubs f9,f9,f5
  f9 = double(float(f9 - f5));
  // lfs f6,-4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -4);
  f6 = double(temp.f32);
  // fadds f1,f8,f4
  f1 = double(float(f8 + f4));
  // fadds f5,f7,f6
  f5 = double(float(f7 + f6));
  // addi r25,r5,3
  r25 = uint64_t(int64_t(int64_t(r5) + 3));
  // fsubs f8,f8,f4
  f8 = double(float(f8 - f4));
  // addi r7,r7,3
  r7 = uint64_t(int64_t(int64_t(r7) + 3));
  // fsubs f7,f7,f6
  f7 = double(float(f7 - f6));
  // rlwinm r30,r30,2,0,29
  r30 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // fadds f6,f1,f3
  f6 = double(float(f1 + f3));
  // stfsx f6,r28,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r28) + uint32_t(r31), temp.u32);
  // fadds f4,f5,f2
  f4 = double(float(f5 + f2));
  // stfs f4,-4(r11)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // fsubs f5,f2,f5
  f5 = double(float(f2 - f5));
  // stfs f5,-4(r10)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // fadds f5,f8,f9
  f5 = double(float(f8 + f9));
  // fsubs f6,f3,f1
  f6 = double(float(f3 - f1));
  // stfsx f6,r29,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r29) + uint32_t(r31), temp.u32);
  // fsubs f6,f10,f7
  f6 = double(float(f10 - f7));
  // fadds f10,f7,f10
  f10 = double(float(f7 + f10));
  // fsubs f9,f9,f8
  f9 = double(float(f9 - f8));
  // fmuls f3,f13,f5
  f3 = double(float(f13 * f5));
  // fmuls f4,f13,f6
  f4 = double(float(f13 * f6));
  // fmsubs f8,f0,f6,f3
  f8 = double(float(std::fma(f0, f6, -f3)));
  // stfsx f8,r3,r31
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r3) + uint32_t(r31), temp.u32);
  // addi r3,r6,2
  r3 = uint64_t(int64_t(int64_t(r6) + 2));
  // fmadds f8,f0,f5,f4
  f8 = double(float(std::fma(f0, f5, f4)));
  // stfs f8,-4(r9)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r9) + -4, temp.u32);
  // fmuls f8,f11,f10
  f8 = double(float(f11 * f10));
  // addi r6,r6,3
  r6 = uint64_t(int64_t(int64_t(r6) + 3));
  // fmuls f10,f12,f10
  f10 = double(float(f12 * f10));
  // rlwinm r29,r3,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lfs f5,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f5 = double(temp.f32);
  // rlwinm r3,r26,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r5,r6,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // fmadds f8,f12,f9,f8
  f8 = double(float(std::fma(f12, f9, f8)));
  // stfsx f8,r4,r31
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r4) + uint32_t(r31), temp.u32);
  // fmsubs f10,f11,f9,f10
  f10 = double(float(std::fma(f11, f9, -f10)));
  // stfs f10,-4(r8)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r8) + -4, temp.u32);
  // addi r4,r27,2
  r4 = uint64_t(int64_t(int64_t(r27) + 2));
  // lfs f10,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f10 = double(temp.f32);
  // lfs f9,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f9 = double(temp.f32);
  // addi r27,r27,3
  r27 = uint64_t(int64_t(int64_t(r27) + 3));
  // fadds f7,f10,f9
  f7 = double(float(f10 + f9));
  // rlwinm r28,r4,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // lfs f8,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f8 = double(temp.f32);
  // fsubs f10,f10,f9
  f10 = double(float(f10 - f9));
  // lfs f6,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f6 = double(temp.f32);
  // rlwinm r4,r27,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lfs f4,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f4 = double(temp.f32);
  // fadds f3,f8,f6
  f3 = double(float(f8 + f6));
  // lfs f2,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f2 = double(temp.f32);
  // fsubs f8,f8,f6
  f8 = double(float(f8 - f6));
  // lfs f9,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f9 = double(temp.f32);
  // fadds f6,f4,f5
  f6 = double(float(f4 + f5));
  // rlwinm r6,r25,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 2) & 0xFFFFFFFC;
  // fsubs f5,f4,f5
  f5 = double(float(f4 - f5));
  // rlwinm r7,r7,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // fadds f4,f2,f9
  f4 = double(float(f2 + f9));
  // fsubs f9,f2,f9
  f9 = double(float(f2 - f9));
  // fadds f2,f6,f7
  f2 = double(float(f6 + f7));
  // stfs f2,0(r11)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fsubs f7,f7,f6
  f7 = double(float(f7 - f6));
  // fadds f6,f4,f3
  f6 = double(float(f4 + f3));
  // stfs f6,4(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fsubs f6,f3,f4
  f6 = double(float(f3 - f4));
  // stfs f6,4(r10)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // stfs f7,0(r10)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f7,f10,f9
  f7 = double(float(f10 - f9));
  // fadds f6,f5,f8
  f6 = double(float(f5 + f8));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fadds f10,f9,f10
  f10 = double(float(f9 + f10));
  // fsubs f9,f8,f5
  f9 = double(float(f8 - f5));
  // lfs f8,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f8 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fsubs f4,f7,f6
  f4 = double(float(f7 - f6));
  // fadds f7,f6,f7
  f7 = double(float(f6 + f7));
  // fmuls f6,f4,f8
  f6 = double(float(f4 * f8));
  // stfs f6,0(r9)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fadds f6,f9,f10
  f6 = double(float(f9 + f10));
  // fsubs f9,f9,f10
  f9 = double(float(f9 - f10));
  // lfs f10,8760(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8760);
  f10 = double(temp.f32);
  // fmuls f8,f7,f8
  f8 = double(float(f7 * f8));
  // stfs f8,4(r9)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // lfsx f8,r30,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r31));
  f8 = double(temp.f32);
  // lfsx f7,r6,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + uint32_t(r31));
  f7 = double(temp.f32);
  // fmuls f6,f6,f10
  f6 = double(float(f6 * f10));
  // stfs f6,0(r8)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fmuls f10,f9,f10
  f10 = double(float(f9 * f10));
  // stfs f10,4(r8)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // lfsx f9,r29,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  f9 = double(temp.f32);
  // lfsx f6,r3,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r31));
  f6 = double(temp.f32);
  // lfsx f10,r28,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r31));
  f10 = double(temp.f32);
  // fadds f1,f6,f8
  f1 = double(float(f6 + f8));
  // lfsx f4,r7,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r31));
  f4 = double(temp.f32);
  // fadds f2,f10,f9
  f2 = double(float(f10 + f9));
  // lfsx f3,r4,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + uint32_t(r31));
  f3 = double(temp.f32);
  // fsubs f10,f10,f9
  f10 = double(float(f10 - f9));
  // lfsx f5,r5,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + uint32_t(r31));
  f5 = double(temp.f32);
  // fadds f9,f4,f7
  f9 = double(float(f4 + f7));
  // fadds f31,f3,f5
  f31 = double(float(f3 + f5));
  // fsubs f8,f6,f8
  f8 = double(float(f6 - f8));
  // fsubs f5,f3,f5
  f5 = double(float(f3 - f5));
  // fsubs f7,f4,f7
  f7 = double(float(f4 - f7));
  // fadds f6,f1,f2
  f6 = double(float(f1 + f2));
  // stfsx f6,r28,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r28) + uint32_t(r31), temp.u32);
  // fsubs f4,f2,f1
  f4 = double(float(f2 - f1));
  // fadds f6,f9,f31
  f6 = double(float(f9 + f31));
  // stfsx f6,r4,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r4) + uint32_t(r31), temp.u32);
  // fsubs f6,f31,f9
  f6 = double(float(f31 - f9));
  // stfsx f6,r7,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + uint32_t(r31), temp.u32);
  // fadds f6,f8,f5
  f6 = double(float(f8 + f5));
  // stfsx f4,r3,r31
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r3) + uint32_t(r31), temp.u32);
  // fsubs f9,f10,f7
  f9 = double(float(f10 - f7));
  // fmuls f4,f0,f6
  f4 = double(float(f0 * f6));
  // fmuls f3,f0,f9
  f3 = double(float(f0 * f9));
  // fadds f0,f7,f10
  f0 = double(float(f7 + f10));
  // fsubs f10,f5,f8
  f10 = double(float(f5 - f8));
  // fmsubs f9,f13,f9,f4
  f9 = double(float(std::fma(f13, f9, -f4)));
  // stfsx f9,r29,r31
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r29) + uint32_t(r31), temp.u32);
  // fmadds f13,f13,f6,f3
  f13 = double(float(std::fma(f13, f6, f3)));
  // stfsx f13,r5,r31
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r5) + uint32_t(r31), temp.u32);
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // fmadds f13,f11,f10,f13
  f13 = double(float(std::fma(f11, f10, f13)));
  // stfsx f13,r30,r31
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + uint32_t(r31), temp.u32);
  // fmsubs f0,f12,f10,f0
  f0 = double(float(std::fma(f12, f10, -f0)));
  // stfsx f0,r6,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + uint32_t(r31), temp.u32);
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // addi r12,r1,-64
  r12 = uint64_t(int64_t(int64_t(r1) + -64));
  // bl 0x825a7948
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A4600: lifted from generated/ by lift.py.
lifted::Result f825A4600(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A4600 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r3,128
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 128, so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x825a4650
  if (!cr6.eq) goto loc_825A4650;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r3,r31,256
  r3 = uint64_t(int64_t(int64_t(r31) + 256));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,384
  r3 = uint64_t(int64_t(int64_t(r31) + 384));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
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
loc_825A4650:
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,192
  r3 = uint64_t(int64_t(int64_t(r31) + 192));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
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

// sub_825A4680: lifted from generated/ by lift.py.
lifted::Result f825A4680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A4680 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f14 = 0.0, f15 = 0.0, f16 = 0.0, f17 = 0.0, f18 = 0.0, f19 = 0.0, f20 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a78e0
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // srawi r27,r3,3
  ca = (int32_t(r3) < 0) & ((uint32_t(r3) & 0x7) != 0);
  r27 = uint64_t(int64_t(int32_t(r3) >> 3));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // rlwinm r30,r27,1,0,30
  r30 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 1) & 0xFFFFFFFE;
  // mr r31,r4
  r31 = r4;
  // rlwinm r28,r30,1,0,30
  r28 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r10,r30,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // extsw r8,r28
  r8 = uint64_t(int64_t(int32_t(r28)));
  // lfs f31,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f27,f31
  f27 = f31;
  // add r9,r10,r30
  r9 = r10 + r30;
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lfs f0,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // std r8,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r8);
  // lis r8,-32251
  r8 = uint64_t(int64_t(-2113601536));
  // lfs f30,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // rlwinm r25,r30,2,0,29
  r25 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // fmr f26,f30
  f26 = f30;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r9,r9,r31
  r9 = r9 + r31;
  // lfs f15,8724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8724);
  f15 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmr f25,f15
  f25 = f15;
  // lfs f11,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f11 = double(temp.f32);
  // fmr f24,f15
  f24 = f15;
  // lfs f8,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f8 = double(temp.f32);
  // fadds f7,f13,f11
  f7 = double(float(f13 + f11));
  // fsubs f13,f13,f11
  f13 = double(float(f13 - f11));
  // lfs f11,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f11 = double(temp.f32);
  // lfs f21,8760(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8760);
  f21 = double(temp.f32);
  // add r11,r25,r31
  r11 = r25 + r31;
  // stfs f21,88(r1)
  temp.f32 = float(f21);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // fmr f23,f21
  f23 = f21;
  // fmr f22,f21
  f22 = f21;
  // lfs f12,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // lfd f10,104(r1)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 104)));
  // fcfid f9,f10
  f9 = double(std::bit_cast<int64_t>(f10));
  // lfd f10,-6072(r8)
  f10 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r8) + -6072)));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f28,8680(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 8680);
  f28 = double(temp.f32);
  // fdiv f10,f10,f9
  f10 = f10 / f9;
  // lfs f9,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f9 = double(temp.f32);
  // frsp f18,f10
  f18 = double(float(f10));
  // lfs f10,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f10 = double(temp.f32);
  // fadds f6,f9,f10
  f6 = double(float(f9 + f10));
  // stfs f18,104(r1)
  temp.f32 = float(f18);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // fsubs f10,f10,f9
  f10 = double(float(f10 - f9));
  // fsubs f9,f0,f8
  f9 = double(float(f0 - f8));
  // fadds f0,f8,f0
  f0 = double(float(f8 + f0));
  // fsubs f8,f12,f11
  f8 = double(float(f12 - f11));
  // fadds f12,f11,f12
  f12 = double(float(f11 + f12));
  // fmuls f29,f18,f28
  f29 = double(float(f18 * f28));
  // fsubs f11,f8,f6
  f11 = double(float(f8 - f6));
  // fadds f8,f6,f8
  f8 = double(float(f6 + f8));
  // fsubs f6,f12,f10
  f6 = double(float(f12 - f10));
  // fadds f10,f10,f12
  f10 = double(float(f10 + f12));
  // fmr f1,f29
  f1 = f29;
  // fmuls f12,f11,f15
  f12 = double(float(f11 * f15));
  // fmuls f11,f8,f15
  f11 = double(float(f8 * f15));
  // fadds f8,f12,f9
  f8 = double(float(f12 + f9));
  // stfs f8,0(r31)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // fadds f8,f11,f7
  f8 = double(float(f11 + f7));
  // stfs f8,4(r31)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // fsubs f12,f9,f12
  f12 = double(float(f9 - f12));
  // stfs f12,0(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fsubs f12,f7,f11
  f12 = double(float(f7 - f11));
  // stfs f12,4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fmuls f12,f6,f15
  f12 = double(float(f6 * f15));
  // fmuls f11,f10,f15
  f11 = double(float(f10 * f15));
  // fadds f10,f12,f13
  f10 = double(float(f12 + f13));
  // stfs f10,4(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // fsubs f10,f0,f11
  f10 = double(float(f0 - f11));
  // stfs f10,0(r10)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fadds f0,f11,f0
  f0 = double(float(f11 + f0));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fsubs f0,f13,f12
  f0 = double(float(f13 - f12));
  // stfs f0,4(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A47D8, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f29
  f1 = f29;
  // frsp f20,f0
  f20 = double(float(f0));
  // stfs f20,96(r1)
  temp.f32 = float(f20);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A47EC, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f19,f1
  lifted::ScalarFloatMode();
  f19 = double(float(f1));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f19,92(r1)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // fmr f0,f20
  f0 = f20;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r24,r27,-4
  r24 = uint64_t(int64_t(int64_t(r27) + -4));
  // lfs f17,8696(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8696);
  f17 = double(temp.f32);
  // stfs f17,84(r1)
  temp.f32 = float(f17);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // fsubs f12,f20,f19
  f12 = double(float(f20 - f19));
  // fmuls f29,f19,f28
  f29 = double(float(f19 * f28));
  // fadds f11,f19,f20
  f11 = double(float(f19 + f20));
  // fmr f13,f19
  f13 = f19;
  // fmuls f10,f12,f15
  f10 = double(float(f12 * f15));
  // fmuls f12,f29,f20
  f12 = double(float(f29 * f20));
  // fmuls f9,f11,f15
  f9 = double(float(f11 * f15));
  // fmuls f12,f12,f28
  f12 = double(float(f12 * f28));
  // fnmsubs f11,f12,f19,f20
  f11 = double(float(-std::fma(f12, f19, -f20)));
  // fnmsubs f12,f12,f20,f19
  f12 = double(float(-std::fma(f12, f20, -f19)));
  // fmuls f28,f12,f28
  f28 = double(float(f12 * f28));
loc_825A4838:
  // fadds f7,f11,f12
  lifted::ScalarFloatMode();
  f7 = double(float(f11 + f12));
  // addi r26,r11,128
  r26 = uint64_t(int64_t(int64_t(r11) + 128));
  // fsubs f8,f11,f12
  f8 = double(float(f11 - f12));
  // cmpw cr6,r26,r24
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r24), so);
  // fmuls f7,f7,f21
  f7 = double(float(f7 * f21));
  // fmuls f8,f8,f21
  f8 = double(float(f8 * f21));
  // ble cr6,0x825a4858
  if (!cr6.gt) goto loc_825A4858;
  // mr r26,r24
  r26 = r24;
loc_825A4858:
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmpw cr6,r11,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r26), so);
  // bge cr6,0x825a4c8c
  if (!cr6.lt) goto loc_825A4C8C;
  // subf r10,r11,r26
  r10 = r26 - r11;
  // add r8,r11,r30
  r8 = r11 + r30;
  // addi r6,r11,2
  r6 = uint64_t(int64_t(int64_t(r11) + 2));
  // addi r3,r10,-1
  r3 = uint64_t(int64_t(int64_t(r10) + -1));
  // addi r5,r8,2
  r5 = uint64_t(int64_t(int64_t(r8) + 2));
  // rlwinm r8,r6,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r3,30,2,31
  r6 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 30) & 0x3FFFFFFF;
  // add r9,r28,r11
  r9 = r28 + r11;
  // addi r3,r6,1
  r3 = uint64_t(int64_t(int64_t(r6) + 1));
  // rlwinm r6,r30,1,0,30
  r6 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 1) & 0xFFFFFFFE;
  // addi r4,r9,2
  r4 = uint64_t(int64_t(int64_t(r9) + 2));
  // neg r7,r11
  r7 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // add r6,r30,r6
  r6 = r30 + r6;
  // rlwinm r9,r5,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r10,r4,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // add r29,r11,r6
  r29 = r11 + r6;
  // add r4,r7,r30
  r4 = r7 + r30;
  // add r5,r28,r7
  r5 = r28 + r7;
  // subf r11,r11,r6
  r11 = r6 - r11;
  // add r7,r7,r25
  r7 = r7 + r25;
  // addi r6,r29,2
  r6 = uint64_t(int64_t(int64_t(r29) + 2));
  // addi r29,r11,-2
  r29 = uint64_t(int64_t(int64_t(r11) + -2));
  // addi r4,r4,-2
  r4 = uint64_t(int64_t(int64_t(r4) + -2));
  // addi r5,r5,-2
  r5 = uint64_t(int64_t(int64_t(r5) + -2));
  // addi r7,r7,-2
  r7 = uint64_t(int64_t(int64_t(r7) + -2));
  // rlwinm r11,r6,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r4,r4,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r5,r5,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r7,r7,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r29,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // add r8,r8,r31
  r8 = r8 + r31;
  // add r9,r9,r31
  r9 = r9 + r31;
  // add r10,r10,r31
  r10 = r10 + r31;
  // add r4,r4,r31
  r4 = r4 + r31;
  // add r5,r5,r31
  r5 = r5 + r31;
  // add r7,r7,r31
  r7 = r7 + r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // add r6,r6,r31
  r6 = r6 + r31;
loc_825A48FC:
  // lfs f1,-8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f1 = double(temp.f32);
  // fmadds f30,f29,f0,f30
  f30 = double(float(std::fma(f29, f0, f30)));
  // lfs f5,-4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -4);
  f5 = double(temp.f32);
  // fnmsubs f27,f28,f12,f27
  f27 = double(float(-std::fma(f28, f12, -f27)));
  // fadds f18,f5,f1
  f18 = double(float(f5 + f1));
  // lfs f2,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f2 = double(temp.f32);
  // lfs f6,-8(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -8);
  f6 = double(temp.f32);
  // fsubs f5,f5,f1
  f5 = double(float(f5 - f1));
  // lfs f20,-8(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -8);
  f20 = double(temp.f32);
  // fsubs f19,f6,f2
  f19 = double(float(f6 - f2));
  // lfs f3,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f3 = double(temp.f32);
  // fadds f6,f6,f2
  f6 = double(float(f6 + f2));
  // lfs f21,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f21 = double(temp.f32);
  // fadds f16,f3,f20
  f16 = double(float(f3 + f20));
  // lfs f4,-8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -8);
  f4 = double(temp.f32);
  // fsubs f3,f3,f20
  f3 = double(float(f3 - f20));
  // fsubs f17,f4,f21
  f17 = double(float(f4 - f21));
  // fadds f4,f21,f4
  f4 = double(float(f21 + f4));
  // fmadds f26,f28,f11,f26
  f26 = double(float(std::fma(f28, f11, f26)));
  // fnmsubs f31,f29,f13,f31
  f31 = double(float(-std::fma(f29, f13, -f31)));
  // fmuls f2,f13,f18
  f2 = double(float(f13 * f18));
  // fmadds f24,f29,f10,f24
  f24 = double(float(std::fma(f29, f10, f24)));
  // fmuls f1,f13,f19
  f1 = double(float(f13 * f19));
  // fmuls f14,f11,f6
  f14 = double(float(f11 * f6));
  // fmuls f21,f9,f16
  f21 = double(float(f9 * f16));
  // fnmsubs f25,f29,f9,f25
  f25 = double(float(-std::fma(f29, f9, -f25)));
  // fmuls f20,f9,f17
  f20 = double(float(f9 * f17));
  // fnmsubs f23,f7,f28,f23
  f23 = double(float(-std::fma(f7, f28, -f23)));
  // fmadds f22,f8,f28,f22
  f22 = double(float(std::fma(f8, f28, f22)));
  // fmsubs f2,f0,f19,f2
  f2 = double(float(std::fma(f0, f19, -f2)));
  // fmuls f19,f12,f6
  f19 = double(float(f12 * f6));
  // fmadds f6,f0,f18,f1
  f6 = double(float(std::fma(f0, f18, f1)));
  // fmsubs f1,f10,f17,f21
  f1 = double(float(std::fma(f10, f17, -f21)));
  // fmadds f21,f10,f16,f20
  f21 = double(float(std::fma(f10, f16, f20)));
  // fadds f20,f1,f2
  f20 = double(float(f1 + f2));
  // stfs f20,-8(r8)
  temp.f32 = float(f20);
  REX_STORE_U32(uint32_t(r8) + -8, temp.u32);
  // fsubs f2,f2,f1
  f2 = double(float(f2 - f1));
  // fadds f1,f21,f6
  f1 = double(float(f21 + f6));
  // stfs f1,-4(r8)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r8) + -4, temp.u32);
  // stfs f2,-8(r9)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r9) + -8, temp.u32);
  // fmuls f2,f8,f4
  f2 = double(float(f8 * f4));
  // fmuls f1,f7,f4
  f1 = double(float(f7 * f4));
  // fsubs f6,f6,f21
  f6 = double(float(f6 - f21));
  // stfs f6,-4(r9)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r9) + -4, temp.u32);
  // fmadds f6,f12,f5,f14
  f6 = double(float(std::fma(f12, f5, f14)));
  // fmsubs f5,f11,f5,f19
  f5 = double(float(std::fma(f11, f5, -f19)));
  // fmadds f4,f7,f3,f2
  f4 = double(float(std::fma(f7, f3, f2)));
  // fmsubs f3,f8,f3,f1
  f3 = double(float(std::fma(f8, f3, -f1)));
  // fadds f2,f4,f6
  f2 = double(float(f4 + f6));
  // stfs f2,-8(r10)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r10) + -8, temp.u32);
  // fsubs f6,f6,f4
  f6 = double(float(f6 - f4));
  // fadds f4,f3,f5
  f4 = double(float(f3 + f5));
  // stfs f4,-4(r10)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // fsubs f5,f5,f3
  f5 = double(float(f5 - f3));
  // stfs f5,-4(r11)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // stfs f6,-8(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // lfs f4,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f4 = double(temp.f32);
  // lfs f5,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f5 = double(temp.f32);
  // lfs f2,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f2 = double(temp.f32);
  // fsubs f19,f5,f4
  f19 = double(float(f5 - f4));
  // lfs f3,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f3 = double(temp.f32);
  // fadds f5,f5,f4
  f5 = double(float(f5 + f4));
  // fadds f4,f3,f2
  f4 = double(float(f3 + f2));
  // lfs f21,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f21 = double(temp.f32);
  // lfs f20,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f20 = double(temp.f32);
  // fsubs f3,f3,f2
  f3 = double(float(f3 - f2));
  // lfs f6,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f6 = double(temp.f32);
  // lfs f1,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f1 = double(temp.f32);
  // fadds f18,f20,f6
  f18 = double(float(f20 + f6));
  // fsubs f2,f1,f21
  f2 = double(float(f1 - f21));
  // fadds f1,f21,f1
  f1 = double(float(f21 + f1));
  // fsubs f6,f20,f6
  f6 = double(float(f20 - f6));
  // fmuls f21,f30,f19
  f21 = double(float(f30 * f19));
  // fmuls f14,f27,f5
  f14 = double(float(f27 * f5));
  // fmuls f20,f30,f4
  f20 = double(float(f30 * f4));
  // fmuls f5,f26,f5
  f5 = double(float(f26 * f5));
  // stfs f5,80(r1)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // fmuls f17,f24,f18
  f17 = double(float(f24 * f18));
  // fmuls f16,f24,f2
  f16 = double(float(f24 * f2));
  // fmadds f4,f31,f4,f21
  f4 = double(float(std::fma(f31, f4, f21)));
  // fmsubs f5,f31,f19,f20
  f5 = double(float(std::fma(f31, f19, -f20)));
  // fmsubs f2,f25,f2,f17
  f2 = double(float(std::fma(f25, f2, -f17)));
  // fmadds f21,f25,f18,f16
  f21 = double(float(std::fma(f25, f18, f16)));
  // fmuls f20,f23,f1
  f20 = double(float(f23 * f1));
  // fmuls f1,f22,f1
  f1 = double(float(f22 * f1));
  // fadds f19,f2,f5
  f19 = double(float(f2 + f5));
  // stfs f19,0(r8)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // fsubs f5,f5,f2
  f5 = double(float(f5 - f2));
  // fadds f2,f21,f4
  f2 = double(float(f21 + f4));
  // stfs f2,4(r8)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // fsubs f4,f4,f21
  f4 = double(float(f4 - f21));
  // stfs f4,4(r9)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // stfs f5,0(r9)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fmadds f4,f22,f6,f20
  f4 = double(float(std::fma(f22, f6, f20)));
  // fmadds f5,f26,f3,f14
  f5 = double(float(std::fma(f26, f3, f14)));
  // lfs f20,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f20 = double(temp.f32);
  // fmsubs f6,f23,f6,f1
  f6 = double(float(std::fma(f23, f6, -f1)));
  // fmsubs f3,f27,f3,f20
  f3 = double(float(std::fma(f27, f3, -f20)));
  // fadds f2,f4,f5
  f2 = double(float(f4 + f5));
  // stfs f2,0(r10)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // fsubs f5,f5,f4
  f5 = double(float(f5 - f4));
  // fadds f4,f6,f3
  f4 = double(float(f6 + f3));
  // stfs f4,4(r10)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // fsubs f6,f3,f6
  f6 = double(float(f3 - f6));
  // stfs f6,4(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f5,0(r11)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f5,12(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 12);
  f5 = double(temp.f32);
  // lfs f6,8(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8);
  f6 = double(temp.f32);
  // fsubs f19,f6,f5
  f19 = double(float(f6 - f5));
  // lfs f3,8(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 8);
  f3 = double(temp.f32);
  // fadds f6,f5,f6
  f6 = double(float(f5 + f6));
  // lfs f4,12(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 12);
  f4 = double(temp.f32);
  // lfs f1,12(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 12);
  f1 = double(temp.f32);
  // fadds f5,f3,f4
  f5 = double(float(f3 + f4));
  // lfs f2,8(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8);
  f2 = double(temp.f32);
  // fsubs f4,f4,f3
  f4 = double(float(f4 - f3));
  // lfs f20,12(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 12);
  f20 = double(temp.f32);
  // fsubs f3,f2,f1
  f3 = double(float(f2 - f1));
  // lfs f21,8(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8);
  f21 = double(temp.f32);
  // fadds f2,f1,f2
  f2 = double(float(f1 + f2));
  // fadds f1,f21,f20
  f1 = double(float(f21 + f20));
  // fsubs f21,f20,f21
  f21 = double(float(f20 - f21));
  // fmuls f20,f10,f19
  f20 = double(float(f10 * f19));
  // fmuls f14,f7,f6
  f14 = double(float(f7 * f6));
  // fmuls f6,f8,f6
  f6 = double(float(f8 * f6));
  // stfs f6,80(r1)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // fmuls f18,f10,f5
  f18 = double(float(f10 * f5));
  // fmuls f17,f0,f3
  f17 = double(float(f0 * f3));
  // fmuls f16,f0,f1
  f16 = double(float(f0 * f1));
  // fmadds f5,f9,f5,f20
  f5 = double(float(std::fma(f9, f5, f20)));
  // fmuls f20,f12,f2
  f20 = double(float(f12 * f2));
  // fmuls f2,f11,f2
  f2 = double(float(f11 * f2));
  // fmsubs f6,f9,f19,f18
  f6 = double(float(std::fma(f9, f19, -f18)));
  // fmadds f1,f13,f1,f17
  f1 = double(float(std::fma(f13, f1, f17)));
  // fmsubs f3,f13,f3,f16
  f3 = double(float(std::fma(f13, f3, -f16)));
  // fadds f19,f3,f6
  f19 = double(float(f3 + f6));
  // stfs f19,8(r4)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r4) + 8, temp.u32);
  // fadds f19,f1,f5
  f19 = double(float(f1 + f5));
  // stfs f19,12(r4)
  temp.f32 = float(f19);
  REX_STORE_U32(uint32_t(r4) + 12, temp.u32);
  // fsubs f6,f6,f3
  f6 = double(float(f6 - f3));
  // stfs f6,8(r5)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r5) + 8, temp.u32);
  // fsubs f6,f5,f1
  f6 = double(float(f5 - f1));
  // stfs f6,12(r5)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r5) + 12, temp.u32);
  // fmadds f6,f8,f4,f14
  f6 = double(float(std::fma(f8, f4, f14)));
  // fmsubs f3,f12,f21,f2
  f3 = double(float(std::fma(f12, f21, -f2)));
  // lfs f18,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f18 = double(temp.f32);
  // fmsubs f5,f7,f4,f18
  f5 = double(float(std::fma(f7, f4, -f18)));
  // fmadds f4,f11,f21,f20
  f4 = double(float(std::fma(f11, f21, f20)));
  // fadds f2,f4,f6
  f2 = double(float(f4 + f6));
  // stfs f2,8(r6)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r6) + 8, temp.u32);
  // fadds f2,f3,f5
  f2 = double(float(f3 + f5));
  // stfs f2,12(r6)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r6) + 12, temp.u32);
  // fsubs f6,f6,f4
  f6 = double(float(f6 - f4));
  // stfs f6,8(r7)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + 8, temp.u32);
  // fsubs f6,f5,f3
  f6 = double(float(f5 - f3));
  // stfs f6,12(r7)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + 12, temp.u32);
  // lfs f6,0(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f6 = double(temp.f32);
  // lfs f5,4(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 4);
  f5 = double(temp.f32);
  // lfs f4,4(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f4 = double(temp.f32);
  // fsubs f2,f6,f5
  f2 = double(float(f6 - f5));
  // lfs f3,0(r6)
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f3 = double(temp.f32);
  // lfs f1,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f1 = double(temp.f32);
  // fadds f20,f3,f4
  f20 = double(float(f3 + f4));
  // lfs f21,0(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 0);
  f21 = double(temp.f32);
  // fmuls f19,f25,f2
  f19 = double(float(f25 * f2));
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // fadds f6,f5,f6
  f6 = double(float(f5 + f6));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // fsubs f5,f4,f3
  f5 = double(float(f4 - f3));
  // lfs f3,4(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 4);
  f3 = double(temp.f32);
  // lfs f4,4(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 4);
  f4 = double(temp.f32);
  // fnmsubs f0,f29,f30,f0
  f0 = double(float(-std::fma(f29, f30, -f0)));
  // fmadds f13,f29,f31,f13
  f13 = double(float(std::fma(f29, f31, f13)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // fnmsubs f11,f28,f26,f11
  f11 = double(float(-std::fma(f28, f26, -f11)));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // fmadds f12,f28,f27,f12
  f12 = double(float(std::fma(f28, f27, f12)));
  // addi r8,r8,16
  r8 = uint64_t(int64_t(int64_t(r8) + 16));
  // fnmsubs f10,f29,f24,f10
  f10 = double(float(-std::fma(f29, f24, -f10)));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // fmadds f9,f29,f25,f9
  f9 = double(float(std::fma(f29, f25, f9)));
  // fmuls f18,f25,f20
  f18 = double(float(f25 * f20));
  // fmadds f20,f24,f20,f19
  f20 = double(float(std::fma(f24, f20, f19)));
  // fadds f19,f21,f3
  f19 = double(float(f21 + f3));
  // fmuls f17,f22,f6
  f17 = double(float(f22 * f6));
  // fsubs f3,f3,f21
  f3 = double(float(f3 - f21));
  // fnmsubs f8,f28,f22,f8
  f8 = double(float(-std::fma(f28, f22, -f8)));
  // fmadds f7,f28,f23,f7
  f7 = double(float(std::fma(f28, f23, f7)));
  // fmsubs f2,f24,f2,f18
  f2 = double(float(std::fma(f24, f2, -f18)));
  // fmuls f18,f23,f6
  f18 = double(float(f23 * f6));
  // fsubs f6,f1,f4
  f6 = double(float(f1 - f4));
  // fadds f4,f4,f1
  f4 = double(float(f4 + f1));
  // fmuls f1,f31,f19
  f1 = double(float(f31 * f19));
  // fmuls f21,f31,f6
  f21 = double(float(f31 * f6));
  // fmuls f14,f27,f4
  f14 = double(float(f27 * f4));
  // fmsubs f6,f30,f6,f1
  f6 = double(float(std::fma(f30, f6, -f1)));
  // fmuls f16,f26,f4
  f16 = double(float(f26 * f4));
  // fmadds f4,f30,f19,f21
  f4 = double(float(std::fma(f30, f19, f21)));
  // fadds f1,f6,f2
  f1 = double(float(f6 + f2));
  // stfs f1,0(r4)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // fsubs f6,f2,f6
  f6 = double(float(f2 - f6));
  // fadds f2,f4,f20
  f2 = double(float(f4 + f20));
  // stfs f2,4(r4)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // fsubs f4,f20,f4
  f4 = double(float(f20 - f4));
  // stfs f6,0(r5)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r5) + 0, temp.u32);
  // stfs f4,4(r5)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r5) + 4, temp.u32);
  // fmadds f6,f23,f5,f17
  f6 = double(float(std::fma(f23, f5, f17)));
  // fmadds f4,f27,f3,f16
  f4 = double(float(std::fma(f27, f3, f16)));
  // addi r5,r5,-16
  r5 = uint64_t(int64_t(int64_t(r5) + -16));
  // fmsubs f5,f22,f5,f18
  f5 = double(float(std::fma(f22, f5, -f18)));
  // addi r4,r4,-16
  r4 = uint64_t(int64_t(int64_t(r4) + -16));
  // fmsubs f3,f26,f3,f14
  f3 = double(float(std::fma(f26, f3, -f14)));
  // fadds f2,f4,f6
  f2 = double(float(f4 + f6));
  // stfs f2,0(r6)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // fsubs f6,f6,f4
  f6 = double(float(f6 - f4));
  // fadds f2,f3,f5
  f2 = double(float(f3 + f5));
  // stfs f2,4(r6)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r6) + 4, temp.u32);
  // stfs f6,0(r7)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + 0, temp.u32);
  // fsubs f6,f5,f3
  f6 = double(float(f5 - f3));
  // stfs f6,4(r7)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r7) + 4, temp.u32);
  // addi r6,r6,-16
  r6 = uint64_t(int64_t(int64_t(r6) + -16));
  // addi r7,r7,-16
  r7 = uint64_t(int64_t(int64_t(r7) + -16));
  // bne cr6,0x825a48fc
  if (!cr6.eq) goto loc_825A48FC;
  // lfs f17,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f17 = double(temp.f32);
  // lfs f21,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f21 = double(temp.f32);
  // lfs f19,92(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f19 = double(temp.f32);
  // lfs f20,96(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f20 = double(temp.f32);
  // lfs f18,104(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f18 = double(temp.f32);
loc_825A4C8C:
  // cmpw cr6,r26,r24
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r24), so);
  // beq cr6,0x825a4d30
  if (cr6.eq) goto loc_825A4D30;
  // extsw r11,r26
  r11 = uint64_t(int64_t(int32_t(r26)));
  // std r11,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r11);
  // lfd f0,112(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 112)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f31,f0,f18
  f31 = double(float(f0 * f18));
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A4CB4, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // fmr f1,f31
  f1 = f31;
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A88A0(base, LIFTED_ARGS(0x825A4CC4, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f30,f1
  lifted::ScalarFloatMode();
  f30 = double(float(f1));
  // mr r11,r26
  r11 = r26;
  // fmuls f13,f19,f31
  f13 = double(float(f19 * f31));
  // fmuls f0,f19,f30
  f0 = double(float(f19 * f30));
  // fmuls f12,f30,f31
  f12 = double(float(f30 * f31));
  // fsubs f11,f31,f30
  f11 = double(float(f31 - f30));
  // fmadds f13,f20,f30,f13
  f13 = double(float(std::fma(f20, f30, f13)));
  // fadds f10,f30,f31
  f10 = double(float(f30 + f31));
  // fmsubs f0,f20,f31,f0
  f0 = double(float(std::fma(f20, f31, -f0)));
  // fmuls f12,f12,f17
  f12 = double(float(f12 * f17));
  // fmuls f25,f11,f15
  f25 = double(float(f11 * f15));
  // fmuls f24,f10,f15
  f24 = double(float(f10 * f15));
  // fmuls f11,f13,f0
  f11 = double(float(f13 * f0));
  // fnmsubs f27,f12,f30,f31
  f27 = double(float(-std::fma(f12, f30, -f31)));
  // fnmsubs f26,f12,f31,f30
  f26 = double(float(-std::fma(f12, f31, -f30)));
  // fsubs f10,f0,f13
  f10 = double(float(f0 - f13));
  // fadds f9,f13,f0
  f9 = double(float(f13 + f0));
  // fmuls f8,f11,f17
  f8 = double(float(f11 * f17));
  // fsubs f7,f27,f26
  f7 = double(float(f27 - f26));
  // fadds f6,f26,f27
  f6 = double(float(f26 + f27));
  // fmuls f10,f10,f15
  f10 = double(float(f10 * f15));
  // fmuls f9,f9,f15
  f9 = double(float(f9 * f15));
  // fnmsubs f11,f8,f13,f0
  f11 = double(float(-std::fma(f8, f13, -f0)));
  // fnmsubs f12,f8,f0,f13
  f12 = double(float(-std::fma(f8, f0, -f13)));
  // fmuls f23,f7,f21
  f23 = double(float(f7 * f21));
  // fmuls f22,f6,f21
  f22 = double(float(f6 * f21));
  // b 0x825a4838
  goto loc_825A4838;
loc_825A4D30:
  // add r7,r30,r27
  r7 = r30 + r27;
  // addi r10,r27,-2
  r10 = uint64_t(int64_t(int64_t(r27) + -2));
  // addi r9,r7,-2
  r9 = uint64_t(int64_t(int64_t(r7) + -2));
  // add r6,r7,r30
  r6 = r7 + r30;
  // rlwinm r29,r9,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r9,r6,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r28,r10,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r31
  r9 = r9 + r31;
  // addi r8,r6,-2
  r8 = uint64_t(int64_t(int64_t(r6) + -2));
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lfsx f4,r29,r31
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r31));
  f4 = double(temp.f32);
  // add r5,r6,r30
  r5 = r6 + r30;
  // rlwinm r4,r8,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lfsx f6,r28,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r31));
  f6 = double(temp.f32);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lfs f1,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f1 = double(temp.f32);
  // rlwinm r8,r5,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // fsubs f29,f6,f1
  f29 = double(float(f6 - f1));
  // rlwinm r10,r7,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // fadds f6,f1,f6
  f6 = double(float(f1 + f6));
  // addi r3,r5,-2
  r3 = uint64_t(int64_t(int64_t(r5) + -2));
  // add r8,r8,r31
  r8 = r8 + r31;
  // lfsx f2,r4,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + uint32_t(r31));
  f2 = double(temp.f32);
  // add r10,r10,r31
  r10 = r10 + r31;
  // lfs f5,-4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -4);
  f5 = double(temp.f32);
  // rlwinm r3,r3,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // fadds f30,f2,f5
  f30 = double(float(f2 + f5));
  // fsubs f5,f5,f2
  f5 = double(float(f5 - f2));
  // addi r30,r27,2
  r30 = uint64_t(int64_t(int64_t(r27) + 2));
  // lfs f2,-4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -4);
  f2 = double(temp.f32);
  // lfs f3,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f3 = double(temp.f32);
  // fsubs f1,f4,f2
  f1 = double(float(f4 - f2));
  // lfsx f31,r3,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r31));
  f31 = double(temp.f32);
  // fadds f4,f2,f4
  f4 = double(float(f2 + f4));
  // fadds f28,f3,f31
  f28 = double(float(f3 + f31));
  // fmuls f2,f13,f29
  f2 = double(float(f13 * f29));
  // fsubs f3,f3,f31
  f3 = double(float(f3 - f31));
  // fmuls f25,f12,f6
  f25 = double(float(f12 * f6));
  // fmuls f27,f13,f30
  f27 = double(float(f13 * f30));
  // fmuls f26,f11,f6
  f26 = double(float(f11 * f6));
  // fmuls f31,f9,f28
  f31 = double(float(f9 * f28));
  // fmadds f2,f0,f30,f2
  f2 = double(float(std::fma(f0, f30, f2)));
  // fmuls f30,f9,f1
  f30 = double(float(f9 * f1));
  // fmsubs f6,f0,f29,f27
  f6 = double(float(std::fma(f0, f29, -f27)));
  // fmuls f29,f8,f4
  f29 = double(float(f8 * f4));
  // fmuls f4,f7,f4
  f4 = double(float(f7 * f4));
  // fmsubs f1,f10,f1,f31
  f1 = double(float(std::fma(f10, f1, -f31)));
  // fmadds f31,f10,f28,f30
  f31 = double(float(std::fma(f10, f28, f30)));
  // fmsubs f4,f8,f3,f4
  f4 = double(float(std::fma(f8, f3, -f4)));
  // fadds f30,f1,f6
  f30 = double(float(f1 + f6));
  // stfsx f30,r28,r31
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r28) + uint32_t(r31), temp.u32);
  // fsubs f6,f6,f1
  f6 = double(float(f6 - f1));
  // fadds f30,f31,f2
  f30 = double(float(f31 + f2));
  // stfs f30,-4(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // stfsx f6,r29,r31
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r29) + uint32_t(r31), temp.u32);
  // fsubs f6,f2,f31
  f6 = double(float(f2 - f31));
  // stfs f6,-4(r10)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r10) + -4, temp.u32);
  // fmsubs f2,f11,f5,f25
  f2 = double(float(std::fma(f11, f5, -f25)));
  // fmadds f6,f12,f5,f26
  f6 = double(float(std::fma(f12, f5, f26)));
  // fmadds f5,f7,f3,f29
  f5 = double(float(std::fma(f7, f3, f29)));
  // fadds f3,f4,f2
  f3 = double(float(f4 + f2));
  // stfs f3,-4(r9)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r9) + -4, temp.u32);
  // fsubs f4,f2,f4
  f4 = double(float(f2 - f4));
  // fadds f1,f5,f6
  f1 = double(float(f5 + f6));
  // stfsx f1,r4,r31
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r4) + uint32_t(r31), temp.u32);
  // lis r4,-32249
  r4 = uint64_t(int64_t(-2113470464));
  // fsubs f1,f6,f5
  f1 = double(float(f6 - f5));
  // stfsx f1,r3,r31
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + uint32_t(r31), temp.u32);
  // stfs f4,-4(r8)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r8) + -4, temp.u32);
  // lfs f4,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f4 = double(temp.f32);
  // lfs f3,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f3 = double(temp.f32);
  // lfs f6,8732(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8732);
  f6 = double(temp.f32);
  // lis r4,-32249
  r4 = uint64_t(int64_t(-2113470464));
  // lfs f2,4(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 4);
  f2 = double(temp.f32);
  // lfs f1,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f1 = double(temp.f32);
  // lfs f31,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f31 = double(temp.f32);
  // lfs f30,4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 4);
  f30 = double(temp.f32);
  // lfs f5,8728(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8728);
  f5 = double(temp.f32);
  // rlwinm r4,r30,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lfs f29,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f29 = double(temp.f32);
  // lfs f28,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f28 = double(temp.f32);
  // fsubs f27,f4,f3
  f27 = double(float(f4 - f3));
  // addi r3,r5,3
  r3 = uint64_t(int64_t(int64_t(r5) + 3));
  // fadds f4,f3,f4
  f4 = double(float(f3 + f4));
  // addi r30,r5,2
  r30 = uint64_t(int64_t(int64_t(r5) + 2));
  // fadds f3,f1,f2
  f3 = double(float(f1 + f2));
  // addi r29,r7,3
  r29 = uint64_t(int64_t(int64_t(r7) + 3));
  // fsubs f2,f2,f1
  f2 = double(float(f2 - f1));
  // fsubs f1,f31,f30
  f1 = double(float(f31 - f30));
  // fadds f31,f30,f31
  f31 = double(float(f30 + f31));
  // fadds f30,f28,f29
  f30 = double(float(f28 + f29));
  // fsubs f29,f28,f29
  f29 = double(float(f28 - f29));
  // fmuls f28,f27,f5
  f28 = double(float(f27 * f5));
  // fmuls f26,f3,f5
  f26 = double(float(f3 * f5));
  // fmuls f23,f2,f6
  f23 = double(float(f2 * f6));
  // fmuls f22,f2,f5
  f22 = double(float(f2 * f5));
  // fmuls f25,f30,f6
  f25 = double(float(f30 * f6));
  // fmuls f24,f30,f5
  f24 = double(float(f30 * f5));
  // fmadds f3,f3,f6,f28
  f3 = double(float(std::fma(f3, f6, f28)));
  // fmuls f28,f29,f5
  f28 = double(float(f29 * f5));
  // fmsubs f2,f27,f6,f26
  f2 = double(float(std::fma(f27, f6, -f26)));
  // fmuls f27,f31,f5
  f27 = double(float(f31 * f5));
  // fmsubs f30,f1,f5,f25
  f30 = double(float(std::fma(f1, f5, -f25)));
  // fmadds f1,f1,f6,f24
  f1 = double(float(std::fma(f1, f6, f24)));
  // fmsubs f5,f4,f5,f23
  f5 = double(float(std::fma(f4, f5, -f23)));
  // fmadds f4,f4,f6,f22
  f4 = double(float(std::fma(f4, f6, f22)));
  // fadds f26,f30,f2
  f26 = double(float(f30 + f2));
  // stfs f26,0(r11)
  temp.f32 = float(f26);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fadds f26,f1,f3
  f26 = double(float(f1 + f3));
  // stfs f26,4(r11)
  temp.f32 = float(f26);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fsubs f3,f3,f1
  f3 = double(float(f3 - f1));
  // stfs f3,4(r10)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r10) + 4, temp.u32);
  // fmsubs f3,f31,f6,f28
  f3 = double(float(std::fma(f31, f6, -f28)));
  // addi r11,r6,3
  r11 = uint64_t(int64_t(int64_t(r6) + 3));
  // fmadds f6,f29,f6,f27
  f6 = double(float(std::fma(f29, f6, f27)));
  // fsubs f2,f2,f30
  f2 = double(float(f2 - f30));
  // stfs f2,0(r10)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r10,r27,3
  r10 = uint64_t(int64_t(int64_t(r27) + 3));
  // rlwinm r5,r11,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // fsubs f2,f5,f3
  f2 = double(float(f5 - f3));
  // stfs f2,0(r9)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // fsubs f2,f4,f6
  f2 = double(float(f4 - f6));
  // stfs f2,4(r9)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r9) + 4, temp.u32);
  // fadds f5,f3,f5
  f5 = double(float(f3 + f5));
  // addi r9,r6,2
  r9 = uint64_t(int64_t(int64_t(r6) + 2));
  // fadds f6,f6,f4
  f6 = double(float(f6 + f4));
  // stfs f5,0(r8)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
  // stfs f6,4(r8)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r8) + 4, temp.u32);
  // addi r8,r7,2
  r8 = uint64_t(int64_t(int64_t(r7) + 2));
  // rlwinm r6,r10,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lfsx f5,r5,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + uint32_t(r31));
  f5 = double(temp.f32);
  // rlwinm r7,r9,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lfsx f6,r4,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + uint32_t(r31));
  f6 = double(temp.f32);
  // rlwinm r9,r29,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // fsubs f29,f6,f5
  f29 = double(float(f6 - f5));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // fadds f6,f5,f6
  f6 = double(float(f5 + f6));
  // rlwinm r10,r3,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lfsx f30,r11,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  f30 = double(temp.f32);
  // lfsx f4,r6,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + uint32_t(r31));
  f4 = double(temp.f32);
  // lfsx f3,r7,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r31));
  f3 = double(temp.f32);
  // lfsx f1,r9,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r31));
  f1 = double(temp.f32);
  // fadds f5,f3,f4
  f5 = double(float(f3 + f4));
  // lfsx f2,r8,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r31));
  f2 = double(temp.f32);
  // fadds f28,f1,f30
  f28 = double(float(f1 + f30));
  // lfsx f31,r10,r31
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  f31 = double(temp.f32);
  // fsubs f4,f4,f3
  f4 = double(float(f4 - f3));
  // fsubs f3,f2,f31
  f3 = double(float(f2 - f31));
  // fadds f2,f31,f2
  f2 = double(float(f31 + f2));
  // fmuls f31,f10,f29
  f31 = double(float(f10 * f29));
  // fsubs f1,f1,f30
  f1 = double(float(f1 - f30));
  // fmuls f26,f7,f6
  f26 = double(float(f7 * f6));
  // fmuls f6,f8,f6
  f6 = double(float(f8 * f6));
  // fmuls f10,f10,f5
  f10 = double(float(f10 * f5));
  // fmuls f30,f0,f28
  f30 = double(float(f0 * f28));
  // fmuls f27,f0,f3
  f27 = double(float(f0 * f3));
  // fmsubs f0,f9,f29,f10
  f0 = double(float(std::fma(f9, f29, -f10)));
  // fmadds f10,f9,f5,f31
  f10 = double(float(std::fma(f9, f5, f31)));
  // fmsubs f9,f13,f3,f30
  f9 = double(float(std::fma(f13, f3, -f30)));
  // fmuls f5,f12,f2
  f5 = double(float(f12 * f2));
  // fmadds f13,f13,f28,f27
  f13 = double(float(std::fma(f13, f28, f27)));
  // fmuls f3,f11,f2
  f3 = double(float(f11 * f2));
  // fadds f2,f9,f0
  f2 = double(float(f9 + f0));
  // stfsx f2,r4,r31
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r4) + uint32_t(r31), temp.u32);
  // fsubs f0,f0,f9
  f0 = double(float(f0 - f9));
  // fadds f9,f13,f10
  f9 = double(float(f13 + f10));
  // stfsx f9,r6,r31
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r6) + uint32_t(r31), temp.u32);
  // stfsx f0,r8,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + uint32_t(r31), temp.u32);
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // fmadds f0,f8,f4,f26
  f0 = double(float(std::fma(f8, f4, f26)));
  // stfsx f13,r9,r31
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r9) + uint32_t(r31), temp.u32);
  // fmadds f11,f11,f1,f5
  f11 = double(float(std::fma(f11, f1, f5)));
  // fmsubs f13,f7,f4,f6
  f13 = double(float(std::fma(f7, f4, -f6)));
  // fmsubs f12,f12,f1,f3
  f12 = double(float(std::fma(f12, f1, -f3)));
  // fadds f10,f11,f0
  f10 = double(float(f11 + f0));
  // stfsx f10,r7,r31
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r7) + uint32_t(r31), temp.u32);
  // fsubs f0,f0,f11
  f0 = double(float(f0 - f11));
  // fadds f10,f12,f13
  f10 = double(float(f12 + f13));
  // stfsx f10,r5,r31
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r5) + uint32_t(r31), temp.u32);
  // stfsx f0,r11,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), temp.u32);
  // fsubs f0,f13,f12
  f0 = double(float(f13 - f12));
  // stfsx f0,r10,r31
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + uint32_t(r31), temp.u32);
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a792c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5020: lifted from generated/ by lift.py.
lifted::Result f825A5020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5020 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-56(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f29));
  // stfd f30,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8768
  r11 = uint64_t(int64_t(int64_t(r11) + 8768));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r29,r31,12
  r29 = uint64_t(int64_t(int64_t(r31) + 12));
  // stw r4,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r4));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f29,-28028(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -28028);
  f29 = double(temp.f32);
  // lfs f31,8764(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8764);
  f31 = double(temp.f32);
  // lfs f30,11404(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 11404);
  f30 = double(temp.f32);
loc_825A5074:
  // extsw r11,r30
  r11 = uint64_t(int64_t(int32_t(r30)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f1,f0,f31
  f1 = double(float(f0 * f31));
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8978(base, LIFTED_ARGS(0x825A5090, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,256
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 256, so);
  // fnmsubs f0,f0,f30,f29
  f0 = double(float(-std::fma(f0, f30, -f29)));
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // blt cr6,0x825a5074
  if (cr6.lt) goto loc_825A5074;
  // addi r11,r31,2060
  r11 = uint64_t(int64_t(int64_t(r31) + 2060));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // mtctr r10
  ctr = r10;
loc_825A50BC:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x825a50bc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825A50BC;
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f29,-56(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // lfd f30,-48(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A50E0: lifted from generated/ by lift.py.
lifted::Result f825A50E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825A50E0 (lifted)");
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
  // mr r3,r4
  r3 = r4;
  // mr r30,r5
  r30 = r5;
  // li r4,2092
  r4 = uint64_t(int64_t(2092));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A5114, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a512c
  if (cr6.eq) goto loc_825A512C;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a5020
  // (left out: dead in C++)
  { const lifted::Result o = f825A5020(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x825a5130
  goto loc_825A5130;
loc_825A512C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825A5130:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
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

// sub_825A5150: lifted from generated/ by lift.py.
lifted::Result f825A5150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825A5150 (lifted)");
  uint64_t r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r31,r5
  r31 = r5;
  // mr r29,r4
  r29 = r4;
  // cmpwi cr6,r3,512
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 512, so);
  // mr r4,r31
  r4 = r31;
  // bne cr6,0x825a5248
  if (!cr6.eq) goto loc_825A5248;
  // li r3,128
  r3 = uint64_t(int64_t(128));
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r3,r31,256
  r3 = uint64_t(int64_t(int64_t(r31) + 256));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,384
  r3 = uint64_t(int64_t(int64_t(r31) + 384));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r30,r31,512
  r30 = uint64_t(int64_t(int64_t(r31) + 512));
  // li r3,128
  r3 = uint64_t(int64_t(128));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,640
  r3 = uint64_t(int64_t(int64_t(r31) + 640));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r3,r31,768
  r3 = uint64_t(int64_t(int64_t(r31) + 768));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,896
  r3 = uint64_t(int64_t(int64_t(r31) + 896));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r30,r31,1024
  r30 = uint64_t(int64_t(int64_t(r31) + 1024));
  // li r3,128
  r3 = uint64_t(int64_t(128));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,1152
  r3 = uint64_t(int64_t(int64_t(r31) + 1152));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r3,r31,1280
  r3 = uint64_t(int64_t(int64_t(r31) + 1280));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,1408
  r3 = uint64_t(int64_t(int64_t(r31) + 1408));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r30,r31,1536
  r30 = uint64_t(int64_t(int64_t(r31) + 1536));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // li r3,128
  r3 = uint64_t(int64_t(128));
  // mr r4,r30
  r4 = r30;
  // beq cr6,0x825a521c
  if (cr6.eq) goto loc_825A521C;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,1920
  r3 = uint64_t(int64_t(int64_t(r31) + 1920));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // b 0x825a5228
  goto loc_825A5228;
loc_825A521C:
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,1920
  r3 = uint64_t(int64_t(int64_t(r31) + 1920));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
loc_825A5228:
  // mr r3,r30
  r3 = r30;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r3,r31,1664
  r3 = uint64_t(int64_t(int64_t(r31) + 1664));
  // bl 0x825a3408
  // (left out: dead in C++)
  f1 = f825A3408(base, r1, r3);
  // addi r3,r31,1792
  r3 = uint64_t(int64_t(int64_t(r31) + 1792));
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825A5248:
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,192
  r3 = uint64_t(int64_t(int64_t(r31) + 192));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r30,r31,256
  r30 = uint64_t(int64_t(int64_t(r31) + 256));
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,320
  r3 = uint64_t(int64_t(int64_t(r31) + 320));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r3,r31,384
  r3 = uint64_t(int64_t(int64_t(r31) + 384));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,448
  r3 = uint64_t(int64_t(int64_t(r31) + 448));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r30,r31,512
  r30 = uint64_t(int64_t(int64_t(r31) + 512));
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,576
  r3 = uint64_t(int64_t(int64_t(r31) + 576));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r3,r31,640
  r3 = uint64_t(int64_t(int64_t(r31) + 640));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,704
  r3 = uint64_t(int64_t(int64_t(r31) + 704));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r30,r31,768
  r30 = uint64_t(int64_t(int64_t(r31) + 768));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // mr r4,r30
  r4 = r30;
  // beq cr6,0x825a52f4
  if (cr6.eq) goto loc_825A52F4;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,960
  r3 = uint64_t(int64_t(int64_t(r31) + 960));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // b 0x825a5300
  goto loc_825A5300;
loc_825A52F4:
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,960
  r3 = uint64_t(int64_t(int64_t(r31) + 960));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
loc_825A5300:
  // mr r3,r30
  r3 = r30;
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r3,r31,832
  r3 = uint64_t(int64_t(int64_t(r31) + 832));
  // bl 0x825a3980
  // (left out: dead in C++)
  f1 = f825A3980(base, r1, r3);
  // addi r3,r31,896
  r3 = uint64_t(int64_t(int64_t(r31) + 896));
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5320: lifted from generated/ by lift.py.
lifted::Result f825A5320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_825A5320 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
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
  // mr r11,r5
  r11 = r5;
  // mr r30,r4
  r30 = r4;
  // clrlwi r10,r11,30
  r10 = uint32_t(r11) & 0x3;
  // mr r29,r6
  r29 = r6;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825a537c
  if (cr6.eq) goto loc_825A537C;
  // clrlwi r31,r11,31
  r31 = uint32_t(r11) & 0x1;
  // subf r11,r3,r30
  r11 = r30 - r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r4,r11,r29
  r4 = r11 + r29;
  // beq cr6,0x825a536c
  if (cr6.eq) goto loc_825A536C;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825A536C:
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825A537C:
  // mr r31,r3
  r31 = r3;
loc_825A5380:
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // rlwinm r31,r31,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // clrlwi r10,r11,30
  r10 = uint32_t(r11) & 0x3;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825a5380
  if (cr6.eq) goto loc_825A5380;
  // clrlwi r28,r11,31
  r28 = uint32_t(r11) & 0x1;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x825a53d4
  if (cr6.eq) goto loc_825A53D4;
  // cmpwi cr6,r31,128
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 128, so);
  // ble cr6,0x825a53fc
  if (!cr6.gt) goto loc_825A53FC;
loc_825A53A8:
  // subf r11,r31,r30
  r11 = r30 - r31;
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // srawi r31,r31,2
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x3) != 0);
  r31 = uint64_t(int64_t(int32_t(r31) >> 2));
  // cmpwi cr6,r31,128
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 128, so);
  // bgt cr6,0x825a53a8
  if (cr6.gt) goto loc_825A53A8;
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825A53D4:
  // cmpwi cr6,r31,128
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 128, so);
  // ble cr6,0x825a53fc
  if (!cr6.gt) goto loc_825A53FC;
loc_825A53DC:
  // subf r11,r31,r30
  r11 = r30 - r31;
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r4,r11,r29
  r4 = r11 + r29;
  // bl 0x825a4680
  // (left out: dead in C++)
  { const lifted::Result o = f825A4680(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // srawi r31,r31,2
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x3) != 0);
  r31 = uint64_t(int64_t(int32_t(r31) >> 2));
  // cmpwi cr6,r31,128
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 128, so);
  // bgt cr6,0x825a53dc
  if (cr6.gt) goto loc_825A53DC;
loc_825A53FC:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5408: lifted from generated/ by lift.py.
lifted::Result f825A5408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5408 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // mr r31,r30
  r31 = r30;
  // cmpwi cr6,r30,512
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 512, so);
  // ble cr6,0x825a5448
  if (!cr6.gt) goto loc_825A5448;
loc_825A5428:
  // srawi r31,r31,2
  ca = (int32_t(r31) < 0) & ((uint32_t(r31) & 0x3) != 0);
  r31 = uint64_t(int64_t(int32_t(r31) >> 2));
  // subf r11,r31,r30
  r11 = r30 - r31;
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r4,r11,r27
  r4 = r11 + r27;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r31,512
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 512, so);
  // bgt cr6,0x825a5428
  if (cr6.gt) goto loc_825A5428;
loc_825A5448:
  // subf r30,r31,r30
  r30 = r30 - r31;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r31
  r3 = r31;
  // add r5,r11,r27
  r5 = r11 + r27;
  // bl 0x825a5150
  // (left out: dead in C++)
  { const lifted::Result o = f825A5150(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // ble cr6,0x825a54b4
  if (!cr6.gt) goto loc_825A54B4;
  // subf r11,r31,r30
  r11 = r30 - r31;
  // rlwinm r26,r31,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r29,r11,r27
  r29 = r11 + r27;
loc_825A547C:
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // mr r6,r27
  r6 = r27;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // mr r5,r28
  r5 = r28;
  // bl 0x825a5320
  // (left out: dead in C++)
  { const lifted::Result o = f825A5320(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a5150
  // (left out: dead in C++)
  { const lifted::Result o = f825A5150(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // subf r30,r31,r30
  r30 = r30 - r31;
  // subf r29,r26,r29
  r29 = r29 - r26;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bgt cr6,0x825a547c
  if (cr6.gt) goto loc_825A547C;
loc_825A54B4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A54C0: lifted from generated/ by lift.py.
lifted::Result f825A54C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825A54C0 (lifted)");
  uint64_t r5 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r30,8
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 8, so);
  // ble cr6,0x825a55ac
  if (!cr6.gt) goto loc_825A55AC;
  // cmpwi cr6,r30,32
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 32, so);
  // ble cr6,0x825a5550
  if (!cr6.gt) goto loc_825A5550;
  // bl 0x825a3e18
  // (left out: dead in C++)
  { const lifted::Result o = f825A3E18(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,512
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 512, so);
  // mr r3,r30
  r3 = r30;
  // ble cr6,0x825a5514
  if (!cr6.gt) goto loc_825A5514;
  // mr r4,r31
  r4 = r31;
  // bl 0x825a5408
  // (left out: dead in C++)
  { const lifted::Result o = f825A5408(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a23a0
  // (left out: dead in C++)
  r3 = f825A23A0(base, r1, r3, r4);
  // b 0x825a55f4
  goto loc_825A55F4;
loc_825A5514:
  // cmpwi cr6,r30,128
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 128, so);
  // ble cr6,0x825a5538
  if (!cr6.gt) goto loc_825A5538;
  // mr r5,r31
  r5 = r31;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x825a5150
  // (left out: dead in C++)
  { const lifted::Result o = f825A5150(base, r1, r13, r3, r4, r5); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a23a0
  // (left out: dead in C++)
  r3 = f825A23A0(base, r1, r3, r4);
  // b 0x825a55f4
  goto loc_825A55F4;
loc_825A5538:
  // mr r4,r31
  r4 = r31;
  // bl 0x825a4600
  // (left out: dead in C++)
  { const lifted::Result o = f825A4600(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x825a23a0
  // (left out: dead in C++)
  r3 = f825A23A0(base, r1, r3, r4);
  // b 0x825a55f4
  goto loc_825A55F4;
loc_825A5550:
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x825a5568
  if (!cr6.eq) goto loc_825A5568;
  // bl 0x825a2cd8
  // (left out: dead in C++)
  f1 = f825A2CD8(base, r1, r3);
  // mr r3,r31
  r3 = r31;
  // bl 0x825a3108
  // (left out: dead in C++)
  f1 = f825A3108(base, r1, r3);
  // b 0x825a55f4
  goto loc_825A55F4;
loc_825A5568:
  // bl 0x825a31f0
  // (left out: dead in C++)
  f1 = f825A31F0(base, r1, r3);
  // lfs f13,12(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f13 = double(temp.f32);
  // lfs f12,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f12 = double(temp.f32);
  // lfs f11,28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 28);
  f11 = double(temp.f32);
  // lfs f9,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f9 = double(temp.f32);
  // lfs f8,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f8 = double(temp.f32);
  // lfs f7,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f7 = double(temp.f32);
  // lfs f0,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f0 = double(temp.f32);
  // lfs f10,32(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 32);
  f10 = double(temp.f32);
  // stfs f9,12(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // stfs f8,24(r31)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r31) + 24, temp.u32);
  // stfs f7,28(r31)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f0,32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // stfs f13,36(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // stfs f12,48(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stfs f11,52(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // b 0x825a55f0
  goto loc_825A55F0;
loc_825A55AC:
  // bne cr6,0x825a55bc
  if (!cr6.eq) goto loc_825A55BC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a3380
  // (left out: dead in C++)
  f825A3380(base, r3);
  // b 0x825a55f4
  goto loc_825A55F4;
loc_825A55BC:
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // bne cr6,0x825a55f4
  if (!cr6.eq) goto loc_825A55F4;
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // lfs f13,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fsubs f10,f0,f13
  f10 = double(float(f0 - f13));
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f11,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f11 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // fadds f13,f12,f11
  f13 = double(float(f12 + f11));
  // fsubs f0,f12,f11
  f0 = double(float(f12 - f11));
  // stfs f13,4(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
loc_825A55F0:
  // stfs f10,8(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
loc_825A55F4:
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

// sub_825A5790: lifted from generated/ by lift.py.
lifted::Result f825A5790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825A5790 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r29,r3
  r29 = r3;
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // mr r31,r4
  r31 = r4;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // ori r3,r3,1
  r3 = r3 | 1;
  // stw r28,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r28));
  // mr r30,r6
  r30 = r6;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r31,24
  r11 = uint32_t(r31) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825a57e4
  if (cr6.eq) goto loc_825A57E4;
  // lis r28,-32768
  r28 = uint64_t(int64_t(-2147483648));
  // ori r28,r28,16389
  r28 = r28 | 16389;
  // b 0x825a57f4
  goto loc_825A57F4;
loc_825A57E4:
  // addi r4,r29,292
  r4 = uint64_t(int64_t(int64_t(r29) + 292));
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A57F4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825A57F4:
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // std r11,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r11);
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5818: lifted from generated/ by lift.py.
uint64_t f825A5818(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5818 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_825A5828: lifted from generated/ by lift.py.
uint64_t f825A5828(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5828 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // li r11,332
  r11 = uint64_t(int64_t(332));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_825A5838: lifted from generated/ by lift.py.
lifted::Result f825A5838(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5838 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // addi r12,r1,-16
  r12 = uint64_t(int64_t(int64_t(r1) + -16));
  // bl 0x825a7900
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8245f718
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245F718(base, LIFTED_ARGS(0x825A5860, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825A5864:
  // lwz r10,88(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 88);
  // lfs f13,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 32);
  f13 = double(temp.f32);
  // lfs f12,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f12 = double(temp.f32);
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f9,60(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 60);
  f9 = double(temp.f32);
  // lfs f8,44(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 44);
  f8 = double(temp.f32);
  // fmuls f24,f12,f13
  f24 = double(float(f12 * f13));
  // lfs f1,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f1 = double(temp.f32);
  // lfs f29,64(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 64);
  f29 = double(temp.f32);
  // lfs f11,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f11 = double(temp.f32);
  // lfs f0,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // stfs f13,36(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // fmuls f13,f8,f9
  f13 = double(float(f8 * f9));
  // stfs f9,64(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
  // lfs f10,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f10 = double(temp.f32);
  // lfs f31,28(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 28);
  f31 = double(temp.f32);
  // lfs f30,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f30 = double(temp.f32);
  // fneg f9,f24
  f9 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f24) ^ 0x8000000000000000));
  // stfs f0,24(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 24, temp.u32);
  // lfs f7,40(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 40);
  f7 = double(temp.f32);
  // lfs f6,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f6 = double(temp.f32);
  // lfs f28,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f28 = double(temp.f32);
  // lfs f27,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f27 = double(temp.f32);
  // fneg f24,f13
  f24 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // lfs f4,72(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 72);
  f4 = double(temp.f32);
  // fmuls f13,f11,f0
  f13 = double(float(f11 * f0));
  // lfs f3,68(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f3 = double(temp.f32);
  // lfs f26,92(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 92);
  f26 = double(temp.f32);
  // lfs f5,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f5 = double(temp.f32);
  // stfs f5,92(r31)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r31) + 92, temp.u32);
  // fmuls f5,f4,f5
  f5 = double(float(f4 * f5));
  // lfs f2,80(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f2 = double(temp.f32);
  // lfs f25,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f25 = double(temp.f32);
  // stfs f6,56(r31)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r31) + 56, temp.u32);
  // stfs f10,28(r31)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f2,84(r31)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r31) + 84, temp.u32);
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fneg f5,f5
  f5 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f5) ^ 0x8000000000000000));
  // fadds f13,f9,f13
  f13 = double(float(f9 + f13));
  // lfs f9,108(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 108);
  f9 = double(temp.f32);
  // fmadds f13,f10,f12,f13
  f13 = double(float(std::fma(f10, f12, f13)));
  // lfs f10,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f10 = double(temp.f32);
  // fmadds f13,f1,f11,f13
  f13 = double(float(std::fma(f1, f11, f13)));
  // lfs f11,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f11 = double(temp.f32);
  // fadds f13,f13,f31
  f13 = double(float(f13 + f31));
  // stfs f13,32(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // lfs f31,120(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 120);
  f31 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fmadds f0,f12,f30,f0
  f0 = double(float(std::fma(f12, f30, f0)));
  // stfs f0,52(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 52, temp.u32);
  // lfs f30,112(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 112);
  f30 = double(temp.f32);
  // fmuls f13,f7,f0
  f13 = double(float(f7 * f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f24,f13
  f13 = double(float(f24 + f13));
  // fmadds f13,f6,f8,f13
  f13 = double(float(std::fma(f6, f8, f13)));
  // lfs f8,144(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 144);
  f8 = double(temp.f32);
  // fmadds f13,f29,f7,f13
  f13 = double(float(std::fma(f29, f7, f13)));
  // lfs f29,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f29 = double(temp.f32);
  // lfs f7,128(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 128);
  f7 = double(temp.f32);
  // fadds f13,f13,f28
  f13 = double(float(f13 + f28));
  // stfs f13,60(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
  // lfs f28,148(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 148);
  f28 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fmadds f0,f12,f27,f0
  f0 = double(float(std::fma(f12, f27, f0)));
  // stfs f0,80(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 80, temp.u32);
  // lfs f12,116(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 116);
  f12 = double(temp.f32);
  // lfs f27,140(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 140);
  f27 = double(temp.f32);
  // fmuls f13,f3,f0
  f13 = double(float(f3 * f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f13,f5
  f13 = double(float(f13 + f5));
  // lfs f5,76(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 76);
  f5 = double(temp.f32);
  // fmadds f13,f26,f3,f13
  f13 = double(float(std::fma(f26, f3, f13)));
  // lfs f26,132(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 132);
  f26 = double(temp.f32);
  // lfs f3,152(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 152);
  f3 = double(temp.f32);
  // fmadds f13,f2,f4,f13
  f13 = double(float(std::fma(f2, f4, f13)));
  // lfs f4,156(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 156);
  f4 = double(temp.f32);
  // fadds f13,f13,f25
  f13 = double(float(f13 + f25));
  // fsubs f6,f0,f13
  f6 = double(float(f0 - f13));
  // fmadds f0,f6,f5,f0
  f0 = double(float(std::fma(f6, f5, f0)));
  // lfs f6,124(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 124);
  f6 = double(temp.f32);
  // lfs f5,136(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 136);
  f5 = double(temp.f32);
  // stfs f13,88(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 88, temp.u32);
  // fmuls f13,f11,f12
  f13 = double(float(f11 * f12));
  // stfs f12,120(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 120, temp.u32);
  // fmuls f12,f7,f8
  f12 = double(float(f7 * f8));
  // stfs f8,148(r31)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r31) + 148, temp.u32);
  // fmuls f8,f10,f0
  f8 = double(float(f10 * f0));
  // stfs f0,108(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 108, temp.u32);
  // lfs f2,172(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 172);
  f2 = double(temp.f32);
  // fmuls f23,f4,f2
  f23 = double(float(f4 * f2));
  // lfs f1,164(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 164);
  f1 = double(temp.f32);
  // lfs f25,176(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 176);
  f25 = double(temp.f32);
  // lfs f24,168(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 168);
  f24 = double(temp.f32);
  // stfs f5,140(r31)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r31) + 140, temp.u32);
  // stfs f1,168(r31)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r31) + 168, temp.u32);
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // stfs f9,112(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 112, temp.u32);
  // fneg f12,f12
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) ^ 0x8000000000000000));
  // stfs f2,176(r31)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r31) + 176, temp.u32);
  // fneg f8,f8
  f8 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f8) ^ 0x8000000000000000));
  // lfs f22,252(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 252);
  f22 = double(temp.f32);
  // fneg f23,f23
  f23 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f23) ^ 0x8000000000000000));
  // fadds f13,f8,f13
  f13 = double(float(f8 + f13));
  // lfs f8,228(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 228);
  f8 = double(temp.f32);
  // fmadds f13,f10,f31,f13
  f13 = double(float(std::fma(f10, f31, f13)));
  // lfs f10,180(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 180);
  f10 = double(temp.f32);
  // lfs f31,248(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 248);
  f31 = double(temp.f32);
  // fmadds f13,f9,f11,f13
  f13 = double(float(std::fma(f9, f11, f13)));
  // lfs f9,192(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 192);
  f9 = double(temp.f32);
  // fadds f13,f13,f30
  f13 = double(float(f13 + f30));
  // stfs f13,116(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 116, temp.u32);
  // fsubs f11,f0,f13
  f11 = double(float(f0 - f13));
  // fmadds f0,f11,f29,f0
  f0 = double(float(std::fma(f11, f29, f0)));
  // stfs f0,136(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 136, temp.u32);
  // lfs f11,184(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 184);
  f11 = double(temp.f32);
  // lfs f29,204(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 204);
  f29 = double(temp.f32);
  // fmuls f13,f6,f0
  f13 = double(float(f6 * f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f12,f13
  f13 = double(float(f12 + f13));
  // fmadds f13,f5,f7,f13
  f13 = double(float(std::fma(f5, f7, f13)));
  // lfs f5,160(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 160);
  f5 = double(temp.f32);
  // lfs f7,212(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 212);
  f7 = double(temp.f32);
  // fmadds f13,f6,f28,f13
  f13 = double(float(std::fma(f6, f28, f13)));
  // lfs f28,196(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 196);
  f28 = double(temp.f32);
  // stfs f9,196(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 196, temp.u32);
  // fadds f13,f13,f27
  f13 = double(float(f13 + f27));
  // stfs f13,144(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 144, temp.u32);
  // lfs f27,188(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 188);
  f27 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fmadds f0,f12,f26,f0
  f0 = double(float(std::fma(f12, f26, f0)));
  // lfs f12,200(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 200);
  f12 = double(temp.f32);
  // lfs f26,232(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 232);
  f26 = double(temp.f32);
  // stfs f12,204(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 204, temp.u32);
  // stfs f8,232(r31)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r31) + 232, temp.u32);
  // fmuls f13,f3,f0
  f13 = double(float(f3 * f0));
  // fmr f30,f0
  f30 = f0;
  // stfs f30,164(r31)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r31) + 164, temp.u32);
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f23,f13
  f13 = double(float(f23 + f13));
  // lfs f23,260(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 260);
  f23 = double(temp.f32);
  // fmadds f13,f1,f4,f13
  f13 = double(float(std::fma(f1, f4, f13)));
  // lfs f4,240(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 240);
  f4 = double(temp.f32);
  // lfs f1,256(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 256);
  f1 = double(temp.f32);
  // fmuls f2,f4,f1
  f2 = double(float(f4 * f1));
  // fmadds f13,f25,f3,f13
  f13 = double(float(std::fma(f25, f3, f13)));
  // lfs f25,224(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 224);
  f25 = double(temp.f32);
  // lfs f3,236(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 236);
  f3 = double(temp.f32);
  // fadds f13,f13,f24
  f13 = double(float(f13 + f24));
  // stfs f13,172(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 172, temp.u32);
  // lfs f24,216(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 216);
  f24 = double(temp.f32);
  // fsubs f6,f0,f13
  f6 = double(float(f0 - f13));
  // fmuls f13,f11,f12
  f13 = double(float(f11 * f12));
  // fmuls f12,f7,f8
  f12 = double(float(f7 * f8));
  // fmadds f0,f6,f5,f0
  f0 = double(float(std::fma(f6, f5, f0)));
  // lfs f5,220(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 220);
  f5 = double(temp.f32);
  // lfs f6,208(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 208);
  f6 = double(temp.f32);
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // stfs f0,192(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 192, temp.u32);
  // fneg f12,f12
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) ^ 0x8000000000000000));
  // stfs f5,224(r31)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r31) + 224, temp.u32);
  // fmuls f8,f10,f0
  f8 = double(float(f10 * f0));
  // fneg f8,f8
  f8 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f8) ^ 0x8000000000000000));
  // fadds f13,f8,f13
  f13 = double(float(f8 + f13));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fneg f2,f2
  f2 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f2) ^ 0x8000000000000000));
  // stfs f31,252(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 252, temp.u32);
  // stfs f1,260(r31)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r31) + 260, temp.u32);
  // cmpwi cr6,r11,1024
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1024, so);
  // fmadds f13,f10,f29,f13
  f13 = double(float(std::fma(f10, f29, f13)));
  // lfs f10,264(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 264);
  f10 = double(temp.f32);
  // fmadds f13,f9,f11,f13
  f13 = double(float(std::fma(f9, f11, f13)));
  // lfs f9,276(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 276);
  f9 = double(temp.f32);
  // fadds f13,f13,f28
  f13 = double(float(f13 + f28));
  // stfs f13,200(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 200, temp.u32);
  // fsubs f11,f0,f13
  f11 = double(float(f0 - f13));
  // fmadds f0,f11,f27,f0
  f0 = double(float(std::fma(f11, f27, f0)));
  // stfs f0,220(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 220, temp.u32);
  // lfs f11,268(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 268);
  f11 = double(temp.f32);
  // fmuls f13,f6,f0
  f13 = double(float(f6 * f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // fmadds f13,f5,f7,f13
  f13 = double(float(std::fma(f5, f7, f13)));
  // lfs f7,244(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 244);
  f7 = double(temp.f32);
  // lfs f5,280(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 280);
  f5 = double(temp.f32);
  // stfs f9,280(r31)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r31) + 280, temp.u32);
  // fmadds f13,f6,f26,f13
  f13 = double(float(std::fma(f6, f26, f13)));
  // lfs f6,288(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 288);
  f6 = double(temp.f32);
  // fadds f13,f13,f25
  f13 = double(float(f13 + f25));
  // stfs f13,228(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 228, temp.u32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fmadds f0,f12,f24,f0
  f0 = double(float(std::fma(f12, f24, f0)));
  // stfs f0,248(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 248, temp.u32);
  // lfs f12,284(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 284);
  f12 = double(temp.f32);
  // stfs f12,288(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 288, temp.u32);
  // fmuls f13,f3,f0
  f13 = double(float(f3 * f0));
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fadds f13,f13,f2
  f13 = double(float(f13 + f2));
  // fmadds f13,f3,f23,f13
  f13 = double(float(std::fma(f3, f23, f13)));
  // fmadds f13,f31,f4,f13
  f13 = double(float(std::fma(f31, f4, f13)));
  // lfs f4,272(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 272);
  f4 = double(temp.f32);
  // fadds f13,f13,f22
  f13 = double(float(f13 + f22));
  // stfs f13,256(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 256, temp.u32);
  // fsubs f8,f0,f13
  f8 = double(float(f0 - f13));
  // fmuls f13,f11,f12
  f13 = double(float(f11 * f12));
  // fmadds f0,f8,f7,f0
  f0 = double(float(std::fma(f8, f7, f0)));
  // stfs f0,276(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 276, temp.u32);
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fmuls f12,f10,f0
  f12 = double(float(f10 * f0));
  // fneg f12,f12
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) ^ 0x8000000000000000));
  // fadds f13,f12,f13
  f13 = double(float(f12 + f13));
  // fmadds f13,f9,f11,f13
  f13 = double(float(std::fma(f9, f11, f13)));
  // fmadds f13,f10,f6,f13
  f13 = double(float(std::fma(f10, f6, f13)));
  // fadds f13,f13,f5
  f13 = double(float(f13 + f5));
  // stfs f13,284(r31)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r31) + 284, temp.u32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // fmadds f0,f12,f4,f0
  f0 = double(float(std::fma(f12, f4, f0)));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // blt cr6,0x825a5864
  if (cr6.lt) goto loc_825A5864;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // addi r12,r1,-16
  r12 = uint64_t(int64_t(int64_t(r1) + -16));
  // bl 0x825a794c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825A5BF0: lifted from generated/ by lift.py.
lifted::Result f825A5BF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A5BF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32255
  r8 = uint64_t(int64_t(-2113863680));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r27,r11,4048
  r27 = uint64_t(int64_t(int64_t(r11) + 4048));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lfd f27,8792(r7)
  lifted::ScalarFloatMode();
  f27 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r7) + 8792)));
  // lfd f28,-2264(r8)
  f28 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r8) + -2264)));
  // addi r31,r3,12
  r31 = uint64_t(int64_t(int64_t(r3) + 12));
  // lfs f29,7140(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 7140);
  f29 = double(temp.f32);
  // addi r29,r3,292
  r29 = uint64_t(int64_t(int64_t(r3) + 292));
  // lfs f30,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f30 = double(temp.f32);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r28,r11,4008
  r28 = uint64_t(int64_t(int64_t(r11) + 4008));
loc_825A5C40:
  // lfs f31,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f31 = double(temp.f32);
  // fmr f2,f27
  f2 = f27;
  // fsubs f0,f31,f30
  f0 = double(float(f31 - f30));
  // fmr f1,f28
  f1 = f28;
  // fmuls f0,f0,f29
  f0 = double(float(f0 * f29));
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8ED0(base, LIFTED_ARGS(0x825A5C5C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,0(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // lfsx f0,r30,r28
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r28));
  f0 = double(temp.f32);
  // ble cr6,0x825a5c7c
  if (!cr6.gt) goto loc_825A5C7C;
  // fsubs f13,f0,f30
  f13 = double(float(f0 - f30));
  // fadds f0,f0,f30
  f0 = double(float(f0 + f30));
  // b 0x825a5c84
  goto loc_825A5C84;
loc_825A5C7C:
  // fsubs f13,f0,f31
  lifted::ScalarFloatMode();
  f13 = double(float(f0 - f31));
  // fadds f0,f0,f31
  f0 = double(float(f0 + f31));
loc_825A5C84:
  // fdivs f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 / f0));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // lfsx f13,r30,r27
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r27));
  f13 = double(temp.f32);
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,40
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 40, so);
  // fsubs f0,f30,f0
  f0 = double(float(f30 - f0));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,4(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // addi r31,r31,28
  r31 = uint64_t(int64_t(int64_t(r31) + 28));
  // blt cr6,0x825a5c40
  if (cr6.lt) goto loc_825A5C40;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5CC0: lifted from generated/ by lift.py.
lifted::Result f825A5CC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825A5CC0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,8804
  r11 = uint64_t(int64_t(int64_t(r11) + 8804));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r29,r31,292
  r29 = uint64_t(int64_t(int64_t(r31) + 292));
  // li r30,10
  r30 = uint64_t(int64_t(10));
  // stw r4,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r4));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfd f31,-2264(r11)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // lfd f30,8792(r10)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r10) + 8792)));
loc_825A5D08:
  // fmr f2,f30
  lifted::ScalarFloatMode();
  f2 = f30;
  // fmr f1,f31
  f1 = f31;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8ED0(base, LIFTED_ARGS(0x825A5D14, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825a5d08
  if (!cr6.eq) goto loc_825A5D08;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a5bf0
  // (left out: dead in C++)
  { const lifted::Result o = f825A5BF0(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f30,-48(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5D48: lifted from generated/ by lift.py.
lifted::Result f825A5D48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A5D48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // mr r30,r4
  r30 = r4;
  // ori r3,r3,1
  r3 = r3 | 1;
  // mr r29,r6
  r29 = r6;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825a5d88
  if (cr6.eq) goto loc_825A5D88;
  // lis r28,-32768
  r28 = uint64_t(int64_t(-2147483648));
  // ori r28,r28,16389
  r28 = r28 | 16389;
  // b 0x825a5da0
  goto loc_825A5DA0;
loc_825A5D88:
  // addi r3,r31,292
  r3 = uint64_t(int64_t(int64_t(r31) + 292));
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r5,40
  r5 = uint64_t(int64_t(40));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x825A5D98, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a5bf0
  // (left out: dead in C++)
  { const lifted::Result o = f825A5BF0(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
loc_825A5DA0:
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A5DB8: lifted from generated/ by lift.py.
lifted::Result f825A5DB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825A5DB8 (lifted)");
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
  // mr r3,r4
  r3 = r4;
  // mr r30,r5
  r30 = r5;
  // li r4,332
  r4 = uint64_t(int64_t(332));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A5DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a5e04
  if (cr6.eq) goto loc_825A5E04;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825a5cc0
  // (left out: dead in C++)
  { const lifted::Result o = f825A5CC0(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x825a5e08
  goto loc_825A5E08;
loc_825A5E04:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825A5E08:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
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

// sub_825A5E28: lifted from generated/ by lift.py.
lifted::Result f825A5E28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A5E28 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r29,r3
  r29 = r3;
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // mr r30,r4
  r30 = r4;
  // ori r3,r3,1
  r3 = r3 | 1;
  // mr r31,r6
  r31 = r6;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825a5e68
  if (cr6.eq) goto loc_825A5E68;
  // lis r27,-32768
  r27 = uint64_t(int64_t(-2147483648));
  // ori r27,r27,16389
  r27 = r27 | 16389;
  // b 0x825a6058
  goto loc_825A6058;
loc_825A5E68:
  // lwz r28,0(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,24(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 24);
  // stw r11,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r11));
  // lwz r3,16(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 16);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825a6050
  if (cr6.eq) goto loc_825A6050;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r31,r28,40
  r31 = uint64_t(int64_t(int64_t(r28) + 40));
  // lfs f9,-32680(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -32680);
  f9 = double(temp.f32);
  // lfs f10,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f10 = double(temp.f32);
loc_825A5E9C:
  // lwz r9,28(r29)
  r9 = REX_LOAD_U32(uint32_t(r29) + 28);
  // fmr f12,f10
  lifted::ScalarFloatMode();
  f12 = f10;
  // lwz r10,32(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 32);
  // fmr f0,f10
  f0 = f10;
  // lwz r5,20(r29)
  r5 = REX_LOAD_U32(uint32_t(r29) + 20);
  // add r8,r4,r9
  r8 = r4 + r9;
  // lwz r11,36(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 36);
  // add r9,r10,r4
  r9 = r10 + r4;
  // fmr f11,f10
  f11 = f10;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // add r10,r11,r4
  r10 = r11 + r4;
  // cmpwi cr6,r5,4
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 4, so);
  // blt cr6,0x825a5fc0
  if (cr6.lt) goto loc_825A5FC0;
  // addi r7,r5,-4
  r7 = uint64_t(int64_t(int64_t(r5) + -4));
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r7,r7,30,2,31
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 30) & 0x3FFFFFFF;
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // rlwinm r6,r7,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
loc_825A5EE4:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // add r8,r11,r8
  r8 = r11 + r8;
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // fadds f12,f12,f8
  f12 = double(float(f12 + f8));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825a5f00
  if (cr6.gt) goto loc_825A5F00;
  // fmr f0,f13
  f0 = f13;
loc_825A5F00:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // add r9,r11,r9
  r9 = r11 + r9;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x825a5f14
  if (cr6.gt) goto loc_825A5F14;
  // fmr f11,f13
  f11 = f13;
loc_825A5F14:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // add r8,r11,r8
  r8 = r11 + r8;
  // fadds f12,f12,f8
  f12 = double(float(f12 + f8));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825a5f34
  if (cr6.gt) goto loc_825A5F34;
  // fmr f0,f13
  f0 = f13;
loc_825A5F34:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // add r9,r11,r9
  r9 = r11 + r9;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x825a5f48
  if (cr6.gt) goto loc_825A5F48;
  // fmr f11,f13
  f11 = f13;
loc_825A5F48:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // add r8,r11,r8
  r8 = r11 + r8;
  // fadds f12,f12,f8
  f12 = double(float(f12 + f8));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825a5f68
  if (cr6.gt) goto loc_825A5F68;
  // fmr f0,f13
  f0 = f13;
loc_825A5F68:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // add r9,r11,r9
  r9 = r11 + r9;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x825a5f7c
  if (cr6.gt) goto loc_825A5F7C;
  // fmr f11,f13
  f11 = f13;
loc_825A5F7C:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // add r10,r11,r10
  r10 = r11 + r10;
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // add r8,r11,r8
  r8 = r11 + r8;
  // fadds f12,f12,f8
  f12 = double(float(f12 + f8));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825a5f9c
  if (cr6.gt) goto loc_825A5F9C;
  // fmr f0,f13
  f0 = f13;
loc_825A5F9C:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // add r9,r11,r9
  r9 = r11 + r9;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x825a5fb0
  if (cr6.gt) goto loc_825A5FB0;
  // fmr f11,f13
  f11 = f13;
loc_825A5FB0:
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // add r10,r11,r10
  r10 = r11 + r10;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // bne cr6,0x825a5ee4
  if (!cr6.eq) goto loc_825A5EE4;
loc_825A5FC0:
  // cmplw cr6,r6,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r5), so);
  // bge cr6,0x825a6010
  if (!cr6.lt) goto loc_825A6010;
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // subf r7,r6,r5
  r7 = r5 - r6;
loc_825A5FD0:
  // lfs f8,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f8 = double(temp.f32);
  // add r8,r11,r8
  r8 = r11 + r8;
  // lfs f13,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // fadds f12,f12,f8
  f12 = double(float(f12 + f8));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x825a5fec
  if (cr6.gt) goto loc_825A5FEC;
  // fmr f0,f13
  f0 = f13;
loc_825A5FEC:
  // lfs f13,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // add r9,r11,r9
  r9 = r11 + r9;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x825a6000
  if (cr6.gt) goto loc_825A6000;
  // fmr f11,f13
  f11 = f13;
loc_825A6000:
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // add r10,r11,r10
  r10 = r11 + r10;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // bne cr6,0x825a5fd0
  if (!cr6.eq) goto loc_825A5FD0;
loc_825A6010:
  // clrldi r11,r5,32
  r11 = r5 & 0xFFFFFFFF;
  // stfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // stfs f11,32(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 32, temp.u32);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fdivs f0,f12,f0
  f0 = double(float(f12 / f0));
  // fmuls f0,f0,f9
  f0 = double(float(f0 * f9));
  // stfs f0,-32(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + -32, temp.u32);
  // lwz r3,16(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 16);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // blt cr6,0x825a5e9c
  if (cr6.lt) goto loc_825A5E9C;
loc_825A6050:
  // lwz r11,44(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 44);
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_825A6058:
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A6070: lifted from generated/ by lift.py.
lifted::Result f825A6070(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A6070 (lifted)");
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
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // mr r31,r4
  r31 = r4;
  // ori r3,r3,1
  r3 = r3 | 1;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r31,24
  r11 = uint32_t(r31) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825a60b8
  if (cr6.eq) goto loc_825A60B8;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
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
loc_825A60B8:
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,3
  r3 = r3 | 3;
  // bl 0x8245ed60
  // (left out: dead in C++)
  { const lifted::Result o = f8245ED60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_825A6110: lifted from generated/ by lift.py.
uint64_t f825A6110(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A6110 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addic. r3,r10,-1
  ca = uint32_t(r10) > 0;
  r3 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // bnelr
  if (!cr0.eq) return r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // blr
  return r3;
}

// sub_825A6B18: lifted from generated/ by lift.py.
lifted::Result f825A6B18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825A6B18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // addi r31,r1,-176
  r31 = uint64_t(int64_t(int64_t(r1) + -176));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,80
  r3 = uint64_t(int64_t(int64_t(r31) + 80));
  // addi r4,r11,9032
  r4 = uint64_t(int64_t(int64_t(r11) + 9032));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825931D0(base, LIFTED_ARGS(0x825A6B3C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,80
  r4 = uint64_t(int64_t(int64_t(r31) + 80));
  // addi r3,r31,112
  r3 = uint64_t(int64_t(int64_t(r31) + 112));
  // bl 0x82091968
  // (left out: dead in C++)
  { const lifted::Result o = f82091968(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,112
  r3 = uint64_t(int64_t(int64_t(r31) + 112));
  // addi r4,r11,29576
  r4 = uint64_t(int64_t(int64_t(r11) + 29576));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r11));
  // bl 0x825ad198
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825AD198(base, LIFTED_ARGS(0x825A6B64, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A6CC0: lifted from generated/ by lift.py.
lifted::Result f825A6CC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825A6CC0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // clrlwi r11,r4,30
  r11 = uint32_t(r4) & 0x3;
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // mr r31,r3
  r31 = r3;
  // addi r10,r10,4112
  r10 = uint64_t(int64_t(int64_t(r10) + 4112));
  // mulli r9,r11,28
  r9 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(28))));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // add r3,r9,r10
  r3 = r9 + r10;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = f825A6D28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x825A6CF0); r3 = o.r3; f1 = o.f1; }
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

// sub_825A6D08: lifted from generated/ by lift.py.
lifted::Result f825A6D08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825A6D08 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f1 = 0.0;
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,4112
  r11 = uint64_t(int64_t(int64_t(r11) + 4112));
  // mulli r10,r10,28
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(28))));
  // add r3,r10,r11
  r3 = r10 + r11;
  // b 0x8258e578
  { const lifted::Result o = f8258E578(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A6D20: lifted from generated/ by lift.py.
lifted::Result f825A6D20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825A6D20 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db954
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A6D28: lifted from generated/ by lift.py.
lifted::Result f825A6D28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825A6D28 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db8e4
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825A6D30: lifted from generated/ by lift.py.
lifted::Result f825A6D30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825A6D30 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r30,-32153
  r30 = uint64_t(int64_t(-2107179008));
  // lwz r11,-19452(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -19452);
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // bge cr6,0x825a6d94
  if (!cr6.lt) goto loc_825A6D94;
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // addi r31,r10,4368
  r31 = uint64_t(int64_t(int64_t(r10) + 4368));
loc_825A6D5C:
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwzx r10,r10,r31
  r10 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  // stw r11,-19452(r30)
  REX_STORE_U32(uint32_t(r30) + -19452, uint32_t(r11));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825a6d8c
  if (cr6.eq) goto loc_825A6D8C;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r11,-4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825A6D88, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-19452(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -19452);
loc_825A6D8C:
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // blt cr6,0x825a6d5c
  if (cr6.lt) goto loc_825A6D5C;
loc_825A6D94:
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

// sub_825AE2C0: lifted from generated/ by lift.py.
lifted::Result f825AE2C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825AE2C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // li r28,48
  r28 = uint64_t(int64_t(48));
  // mr r29,r7
  r29 = r7;
  // mr r31,r4
  r31 = r4;
  // li r30,1023
  r30 = uint64_t(int64_t(1023));
  // mr r7,r28
  r7 = r28;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // bge cr6,0x825ae2f0
  if (!cr6.lt) goto loc_825AE2F0;
  // mr r6,r26
  r6 = r26;
loc_825AE2F0:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825ae328
  if (!cr6.eq) goto loc_825AE328;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825AE2FC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,22
  r10 = uint64_t(int64_t(22));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825AFCC8(base, LIFTED_ARGS(0x825AE320, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,22
  r3 = uint64_t(int64_t(22));
  // b 0x825ae700
  goto loc_825AE700;
loc_825AE328:
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x825ae35c
  if (!cr6.eq) goto loc_825AE35C;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825AE334, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,22
  r31 = uint64_t(int64_t(22));
loc_825AE338:
  // stw r31,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r31));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825afcc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825AFCC8(base, LIFTED_ARGS(0x825AE354, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // b 0x825ae700
  goto loc_825AE700;
loc_825AE35C:
  // addi r11,r6,11
  r11 = uint64_t(int64_t(int64_t(r6) + 11));
  // stb r26,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r26));
  // cmplw cr6,r5,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r11), so);
  // bgt cr6,0x825ae378
  if (cr6.gt) goto loc_825AE378;
  // bl 0x825a81e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A81E0, LIFTED_ARGS(0x825AE370, r31)); r3 = o.r3; f1 = o.f1; }
  // li r31,34
  r31 = uint64_t(int64_t(34));
  // b 0x825ae338
  goto loc_825AE338;
loc_825AE378:
  // ld r11,0(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 0);
  // rlwinm r10,r11,0,20,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFE;
  // cmpldi cr6,r10,4094
  cr6 = lifted::Compare<uint64_t>(r10, 4094, so);
  // bne cr6,0x825ae420
  if (!cr6.eq) goto loc_825AE420;
  // cmpwi cr6,r5,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r5), -1, so);
  // bne cr6,0x825ae398
  if (!cr6.eq) goto loc_825AE398;
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // b 0x825ae39c
  goto loc_825AE39C;
loc_825AE398:
  // addi r5,r5,-2
  r5 = uint64_t(int64_t(int64_t(r5) + -2));
loc_825AE39C:
  // addi r30,r31,2
  r30 = uint64_t(int64_t(int64_t(r31) + 2));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x825ae1a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825AE1A8(base, LIFTED_ARGS(0x825AE3B0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x825ae3c0
  if (cr0.eq) goto loc_825AE3C0;
  // stb r26,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r26));
  // b 0x825ae700
  goto loc_825AE700;
loc_825AE3C0:
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // cmplwi cr6,r11,45
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 45, so);
  // bne cr6,0x825ae3d4
  if (!cr6.eq) goto loc_825AE3D4;
  // stb r11,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r11));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_825AE3D4:
  // subfic r11,r29,0
  ca = uint32_t(r29) <= 0;
  r11 = static_cast<uint64_t>(0) - r29;
  // stb r28,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r28));
  // li r4,101
  r4 = uint64_t(int64_t(101));
  // subfe r10,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r10 = ~r11 + r11 + ca;
  ca = temp.u8;
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // rlwinm r10,r10,0,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFE0;
  // addi r3,r11,1
  r3 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,120
  r10 = uint64_t(int64_t(int64_t(r10) + 120));
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // bl 0x825a8630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A8630(base, LIFTED_ARGS(0x825AE3FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825ae6fc
  if (cr0.eq) goto loc_825AE6FC;
  // subfic r11,r29,0
  ca = uint32_t(r29) <= 0;
  r11 = static_cast<uint64_t>(0) - r29;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r11,r11,0,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFE0;
  // addi r11,r11,112
  r11 = uint64_t(int64_t(int64_t(r11) + 112));
  // stb r11,0(r3)
  REX_STORE_U8(uint32_t(r3) + 0, uint8_t(r11));
  // stb r26,3(r3)
  REX_STORE_U8(uint32_t(r3) + 3, uint8_t(r26));
  // b 0x825ae6fc
  goto loc_825AE6FC;
loc_825AE420:
  // clrldi r11,r11,63
  r11 = r11 & 0x1;
  // li r27,45
  r27 = uint64_t(int64_t(45));
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x825ae438
  if (cr6.eq) goto loc_825AE438;
  // stb r27,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r27));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_825AE438:
  // subfic r11,r29,0
  ca = uint32_t(r29) <= 0;
  r11 = static_cast<uint64_t>(0) - r29;
  // stb r28,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r28));
  // subfe r10,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r10 = ~r11 + r11 + ca;
  ca = temp.u8;
  // subfic r9,r29,0
  ca = uint32_t(r29) <= 0;
  r9 = static_cast<uint64_t>(0) - r29;
  // rlwinm r10,r10,0,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFE0;
  // subfe r9,r9,r9
  temp.u8 = (~uint32_t(r9) + uint32_t(r9) < ~uint32_t(r9)) | (~uint32_t(r9) + uint32_t(r9) + ca < ca);
  r9 = ~r9 + r9 + ca;
  ca = temp.u8;
  // addi r8,r10,120
  r8 = uint64_t(int64_t(int64_t(r10) + 120));
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // rlwinm r10,r9,0,0,26
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFE0;
  // addi r10,r10,97
  r10 = uint64_t(int64_t(int64_t(r10) + 97));
  // addi r5,r10,-58
  r5 = uint64_t(int64_t(int64_t(r10) + -58));
  // stb r8,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r8));
  // ld r10,0(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r10,r10,0,20,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFE;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // bne cr6,0x825ae4a4
  if (!cr6.eq) goto loc_825AE4A4;
  // stb r28,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r28));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // ld r10,0(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 0);
  // rldicr r10,r10,0,51
  r10 = __builtin_rotateleft64(r10, 0) & 0xFFFFFFFFFFFFF000;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // bne cr6,0x825ae49c
  if (!cr6.eq) goto loc_825AE49C;
  // mr r30,r26
  r30 = r26;
  // b 0x825ae4b0
  goto loc_825AE4B0;
loc_825AE49C:
  // li r30,1022
  r30 = uint64_t(int64_t(1022));
  // b 0x825ae4b0
  goto loc_825AE4B0;
loc_825AE4A4:
  // li r10,49
  r10 = uint64_t(int64_t(49));
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_825AE4B0:
  // mr r4,r11
  r4 = r11;
  // addi r8,r11,1
  r8 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // bne cr6,0x825ae4c8
  if (!cr6.eq) goto loc_825AE4C8;
  // stb r26,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r26));
  // b 0x825ae4dc
  goto loc_825AE4DC;
loc_825AE4C8:
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // lwz r11,-16348(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -16348);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
loc_825AE4DC:
  // ld r11,0(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 0);
  // rldicr r11,r11,0,51
  r11 = __builtin_rotateleft64(r11, 0) & 0xFFFFFFFFFFFFF000;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // ble cr6,0x825ae5d4
  if (!cr6.gt) goto loc_825AE5D4;
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // rldicr r10,r10,48,15
  r10 = __builtin_rotateleft64(r10, 48) & 0xFFFF000000000000;
loc_825AE4F4:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // ble cr6,0x825ae54c
  if (!cr6.gt) goto loc_825AE54C;
  // ld r11,0(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 0);
  // extsh r9,r7
  r9 = uint64_t(int64_t(int16_t(r7)));
  // rldicl r11,r11,52,12
  r11 = __builtin_rotateleft64(r11, 52) & 0xFFFFFFFFFFFFF;
  // and r11,r11,r10
  r11 = r11 & r10;
  // srd r11,r11,r9
  r11 = uint8_t(r9) & 0x40 ? 0 : (r11 >> (uint8_t(r9) & 0x7F));
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // clrlwi r9,r11,16
  r9 = uint32_t(r11) & 0xFFFF;
  // cmplwi cr6,r9,57
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 57, so);
  // ble cr6,0x825ae530
  if (!cr6.gt) goto loc_825AE530;
  // clrlwi r11,r5,16
  r11 = uint32_t(r5) & 0xFFFF;
  // add r11,r11,r9
  r11 = r11 + r9;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
loc_825AE530:
  // addi r9,r7,-4
  r9 = uint64_t(int64_t(int64_t(r7) + -4));
  // stb r11,0(r8)
  REX_STORE_U8(uint32_t(r8) + 0, uint8_t(r11));
  // rldicl r10,r10,60,4
  r10 = __builtin_rotateleft64(r10, 60) & 0xFFFFFFFFFFFFFFF;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // extsh. r7,r9
  r7 = uint64_t(int64_t(int16_t(r9)));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bge 0x825ae4f4
  if (!cr0.lt) goto loc_825AE4F4;
loc_825AE54C:
  // extsh. r11,r7
  r11 = uint64_t(int64_t(int16_t(r7)));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x825ae5d4
  if (cr0.lt) goto loc_825AE5D4;
  // ld r11,0(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 0);
  // extsh r9,r7
  r9 = uint64_t(int64_t(int16_t(r7)));
  // rldicl r11,r11,52,12
  r11 = __builtin_rotateleft64(r11, 52) & 0xFFFFFFFFFFFFF;
  // and r11,r11,r10
  r11 = r11 & r10;
  // srd r11,r11,r9
  r11 = uint8_t(r9) & 0x40 ? 0 : (r11 >> (uint8_t(r9) & 0x7F));
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // ble cr6,0x825ae5d4
  if (!cr6.gt) goto loc_825AE5D4;
  // addi r11,r8,-1
  r11 = uint64_t(int64_t(int64_t(r8) + -1));
loc_825AE578:
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r10,102
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 102, so);
  // beq cr6,0x825ae590
  if (cr6.eq) goto loc_825AE590;
  // cmpwi cr6,r10,70
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 70, so);
  // bne cr6,0x825ae59c
  if (!cr6.eq) goto loc_825AE59C;
loc_825AE590:
  // stb r28,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r28));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x825ae578
  goto loc_825AE578;
loc_825AE59C:
  // cmplw cr6,r11,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r4), so);
  // beq cr6,0x825ae5c8
  if (cr6.eq) goto loc_825AE5C8;
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r10,57
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 57, so);
  // bne cr6,0x825ae5bc
  if (!cr6.eq) goto loc_825AE5BC;
  // addi r10,r5,58
  r10 = uint64_t(int64_t(int64_t(r5) + 58));
  // b 0x825ae5c0
  goto loc_825AE5C0;
loc_825AE5BC:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_825AE5C0:
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // b 0x825ae5d4
  goto loc_825AE5D4;
loc_825AE5C8:
  // lbz r10,-1(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + -1);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stb r10,-1(r11)
  REX_STORE_U8(uint32_t(r11) + -1, uint8_t(r10));
loc_825AE5D4:
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // ble cr6,0x825ae600
  if (!cr6.gt) goto loc_825AE600;
  // mr r11,r8
  r11 = r8;
  // mr r10,r28
  r10 = r28;
  // cmplwi r6,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq 0x825ae5fc
  if (cr0.eq) goto loc_825AE5FC;
  // mtctr r6
  ctr = r6;
loc_825AE5F0:
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // bdnz 0x825ae5f0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825AE5F0;
loc_825AE5FC:
  // add r8,r8,r6
  r8 = r8 + r6;
loc_825AE600:
  // lbz r11,0(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825ae610
  if (!cr6.eq) goto loc_825AE610;
  // mr r8,r4
  r8 = r4;
loc_825AE610:
  // subfic r11,r29,0
  ca = uint32_t(r29) <= 0;
  r11 = static_cast<uint64_t>(0) - r29;
  // addi r10,r8,1
  r10 = uint64_t(int64_t(int64_t(r8) + 1));
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // rlwinm r11,r11,0,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFE0;
  // addi r11,r11,112
  r11 = uint64_t(int64_t(int64_t(r11) + 112));
  // stb r11,0(r8)
  REX_STORE_U8(uint32_t(r8) + 0, uint8_t(r11));
  // ld r11,0(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 0);
  // rldicl r11,r11,63,53
  r11 = __builtin_rotateleft64(r11, 63) & 0x7FF;
  // subf r11,r30,r11
  r11 = r11 - r30;
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // blt cr6,0x825ae648
  if (cr6.lt) goto loc_825AE648;
  // li r9,43
  r9 = uint64_t(int64_t(43));
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // b 0x825ae650
  goto loc_825AE650;
loc_825AE648:
  // neg r11,r11
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // stb r27,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r27));
loc_825AE650:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpdi cr6,r11,1000
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 1000, so);
  // mr r8,r10
  r8 = r10;
  // stb r28,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r28));
  // blt cr6,0x825ae690
  if (cr6.lt) goto loc_825AE690;
  // li r9,1000
  r9 = uint64_t(int64_t(1000));
  // divd r7,r11,r9
  r7 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // divd r6,r11,r9
  r6 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // mr r9,r7
  r9 = r7;
  // mulli r7,r6,1000
  r7 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(1000))));
  // addi r9,r9,48
  r9 = uint64_t(int64_t(int64_t(r9) + 48));
  // subf r11,r7,r11
  r11 = r11 - r7;
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // bne cr6,0x825ae698
  if (!cr6.eq) goto loc_825AE698;
loc_825AE690:
  // cmpdi cr6,r11,100
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 100, so);
  // blt cr6,0x825ae6bc
  if (cr6.lt) goto loc_825AE6BC;
loc_825AE698:
  // li r9,100
  r9 = uint64_t(int64_t(100));
  // divd r7,r11,r9
  r7 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // divd r6,r11,r9
  r6 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // mr r9,r7
  r9 = r7;
  // mulli r7,r6,100
  r7 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(100))));
  // addi r9,r9,48
  r9 = uint64_t(int64_t(int64_t(r9) + 48));
  // subf r11,r7,r11
  r11 = r11 - r7;
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_825AE6BC:
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // bne cr6,0x825ae6cc
  if (!cr6.eq) goto loc_825AE6CC;
  // cmpdi cr6,r11,10
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 10, so);
  // blt cr6,0x825ae6f0
  if (cr6.lt) goto loc_825AE6F0;
loc_825AE6CC:
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // divd r8,r11,r9
  r8 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // divd r7,r11,r9
  r7 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r11) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r11) / int64_t(r9) : 0));
  // mr r9,r8
  r9 = r8;
  // mulli r8,r7,10
  r8 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(10))));
  // addi r9,r9,48
  r9 = uint64_t(int64_t(int64_t(r9) + 48));
  // subf r11,r8,r11
  r11 = r11 - r8;
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
loc_825AE6F0:
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // stb r11,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r11));
  // stb r26,1(r10)
  REX_STORE_U8(uint32_t(r10) + 1, uint8_t(r26));
loc_825AE6FC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825AE700:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825AEB70: lifted from generated/ by lift.py.
lifted::Result f825AEB70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825AEB70 (lifted)");
  uint64_t r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r6
  r11 = r6;
  // mr r6,r7
  r6 = r7;
  // mr r7,r8
  r7 = r8;
  // cmpwi cr6,r11,101
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 101, so);
  // beq cr6,0x825aebbc
  if (cr6.eq) goto loc_825AEBBC;
  // cmpwi cr6,r11,69
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 69, so);
  // beq cr6,0x825aebbc
  if (cr6.eq) goto loc_825AEBBC;
  // cmpwi cr6,r11,102
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 102, so);
  // bne cr6,0x825aeb9c
  if (!cr6.eq) goto loc_825AEB9C;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // b 0x825ae8e0
  { const lifted::Result o = lifted::Call(base, 0x825AE8E0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825AEB9C:
  // cmpwi cr6,r11,97
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 97, so);
  // beq cr6,0x825aebb4
  if (cr6.eq) goto loc_825AEBB4;
  // cmpwi cr6,r11,65
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 65, so);
  // beq cr6,0x825aebb4
  if (cr6.eq) goto loc_825AEBB4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x825ae9d0
  { const lifted::Result o = lifted::entry::e825AE9D0(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825AEBB4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x825ae2c0
  { const lifted::Result o = f825AE2C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825AEBBC:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x825ae1a8
  { const lifted::Result o = lifted::entry::e825AE1A8(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825B9D30: lifted from generated/ by lift.py.
lifted::Result f825B9D30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825B9D30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
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
  // li r7,14
  r7 = uint64_t(int64_t(14));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r3
  r31 = r3;
  // bl 0x825db6e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtQueryInformationFile, LIFTED_ARGS(0x825B9D58, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825b9db8
  if (cr0.lt) goto loc_825B9DB8;
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // std r11,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r11);
  // bl 0x825db854
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtSetInformationFile, LIFTED_ARGS(0x825B9D80, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825b9db8
  if (cr0.lt) goto loc_825B9DB8;
  // ld r11,88(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 88);
  // li r7,19
  r7 = uint64_t(int64_t(19));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // addi r5,r1,104
  r5 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // std r11,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r11);
  // bl 0x825db854
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtSetInformationFile, LIFTED_ARGS(0x825B9DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825b9db8
  if (cr0.lt) goto loc_825B9DB8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825b9dc0
  goto loc_825B9DC0;
loc_825B9DB8:
  // bl 0x82296e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296E88(base, LIFTED_ARGS(0x825B9DBC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825B9DC0:
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

// sub_825BA1E0: lifted from generated/ by lift.py.
lifted::Result f825BA1E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825BA1E0 (lifted)");
  uint64_t r4 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825dbfd4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtSetEvent, LIFTED_ARGS(0x825BA1F4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825ba204
  if (cr0.lt) goto loc_825BA204;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x825ba20c
  goto loc_825BA20C;
loc_825BA204:
  // bl 0x82296e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296E88(base, LIFTED_ARGS(0x825BA208, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825BA20C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825BF200: lifted from generated/ by lift.py.
lifted::Result f825BF200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825BF200 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // mr r26,r4
  r26 = r4;
  // mr r29,r6
  r29 = r6;
  // mr r25,r7
  r25 = r7;
  // mr r24,r9
  r24 = r9;
  // mr r23,r10
  r23 = r10;
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x825bf244
  if (!cr6.eq) goto loc_825BF244;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825bf390
  if (cr6.eq) goto loc_825BF390;
  // addi r5,r31,12492
  r5 = uint64_t(int64_t(int64_t(r31) + 12492));
loc_825BF244:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825bf390
  if (cr6.eq) goto loc_825BF390;
  // lwz r11,12488(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12488);
  // mr r27,r22
  r27 = r22;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x825bf390
  if (!cr6.gt) goto loc_825BF390;
  // mr r28,r22
  r28 = r22;
  // mr r30,r5
  r30 = r5;
loc_825BF264:
  // lwz r9,0(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r8,4(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r7,8(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r6,12(r30)
  r6 = REX_LOAD_U32(uint32_t(r30) + 12);
  // lwz r10,12464(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12464);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // stw r6,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r6));
  // lwz r11,36(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 36);
  // rlwinm r11,r11,14,18,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 14) & 0x3FFF;
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwz r11,120(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 120);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x825bf2b0
  if (!cr6.eq) goto loc_825BF2B0;
  // addi r11,r11,7
  r11 = uint64_t(int64_t(int64_t(r11) + 7));
  // rlwinm r11,r11,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF8;
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
loc_825BF2B0:
  // lwz r11,36(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 36);
  // rlwinm r11,r11,29,17,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x7FFF;
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwz r11,124(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 124);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825bf2d4
  if (!cr6.eq) goto loc_825BF2D4;
  // addi r11,r11,7
  r11 = uint64_t(int64_t(int64_t(r11) + 7));
  // rlwinm r11,r11,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF8;
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
loc_825BF2D4:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // slw r4,r11,r28
  r4 = uint8_t(r28) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r28) & 0x3F));
  // bl 0x825beb80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BEB80(base, LIFTED_ARGS(0x825BF2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12488(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12488);
  // mr r4,r26
  r4 = r26;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // bge cr6,0x825bf34c
  if (!cr6.lt) goto loc_825BF34C;
  // lwz r11,12184(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12184);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bf308
  if (cr6.eq) goto loc_825BF308;
  // ori r4,r26,256
  r4 = r26 | 256;
loc_825BF308:
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bf318
  if (cr6.eq) goto loc_825BF318;
  // ori r4,r4,512
  r4 = r4 | 512;
loc_825BF318:
  // lwz r11,12948(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12948);
  // addi r10,r31,12928
  r10 = uint64_t(int64_t(int64_t(r31) + 12928));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lfs f1,12944(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12944);
  f1 = double(temp.f32);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r22,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r22));
  // mr r7,r30
  r7 = r30;
  // mr r6,r29
  r6 = r29;
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // bl 0x822afa58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AFA58(base, LIFTED_ARGS(0x825BF348, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825bf378
  goto loc_825BF378;
loc_825BF34C:
  // mr r10,r25
  r10 = r25;
  // stw r23,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r23));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r24,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r24));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // mr r7,r30
  r7 = r30;
  // mr r6,r29
  r6 = r29;
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r3,r31
  r3 = r31;
  // bl 0x822afa58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AFA58(base, LIFTED_ARGS(0x825BF378, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BF378:
  // lwz r11,12488(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12488);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r30,r30,16
  r30 = uint64_t(int64_t(int64_t(r30) + 16));
  // addi r28,r28,2
  r28 = uint64_t(int64_t(int64_t(r28) + 2));
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x825bf264
  if (cr6.lt) goto loc_825BF264;
loc_825BF390:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825beb80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BEB80(base, LIFTED_ARGS(0x825BF39C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x825BF3A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12924(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12924);
  // lwz r10,11208(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 11208);
  // lis r9,256
  r9 = uint64_t(int64_t(16777216));
  // rlwinm. r11,r11,0,2,7
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x3F000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // slw r30,r9,r10
  r30 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r9) << (uint8_t(r10) & 0x3F));
  // beq 0x825bf3c0
  if (cr0.eq) goto loc_825BF3C0;
  // mr r30,r11
  r30 = r11;
loc_825BF3C0:
  // lwz r11,12488(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12488);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x825bf414
  if (cr6.eq) goto loc_825BF414;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x825beb80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BEB80(base, LIFTED_ARGS(0x825BF3D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,13016(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 13016);
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // rlwinm r9,r11,12,20,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0xFFF;
  // addi r5,r10,27256
  r5 = uint64_t(int64_t(int64_t(r10) + 27256));
  // clrlwi r10,r11,3
  r10 = uint32_t(r11) & 0x1FFFFFFF;
  // addi r11,r9,512
  r11 = uint64_t(int64_t(int64_t(r9) + 512));
  // mr r4,r30
  r4 = r30;
  // rlwinm r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  // mr r3,r31
  r3 = r31;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addis r6,r11,-16384
  r6 = uint64_t(int64_t(int64_t(r11) + -1073741824));
  // bl 0x8229f228
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F228(base, LIFTED_ARGS(0x825BF408, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825beb80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BEB80(base, LIFTED_ARGS(0x825BF414, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BF414:
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x825BF41C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,13016
  r3 = uint64_t(int64_t(int64_t(r31) + 13016));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,12(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x825bf43c
  if (!cr6.gt) goto loc_825BF43C;
  // bl 0x8229eb10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EB10(base, LIFTED_ARGS(0x825BF438, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_825BF43C:
  // lis r10,-30976
  r10 = uint64_t(int64_t(-2030043136));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lbz r11,10808(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // lbz r10,10811(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 10811);
  // stw r9,13024(r31)
  REX_STORE_U32(uint32_t(r31) + 13024, uint32_t(r9));
  // stw r22,12464(r31)
  REX_STORE_U32(uint32_t(r31) + 12464, uint32_t(r22));
  // stw r22,12456(r31)
  REX_STORE_U32(uint32_t(r31) + 12456, uint32_t(r22));
  // andi. r11,r11,223
  r11 = r11 & 223;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // andi. r10,r10,191
  r10 = r10 & 191;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stb r11,10808(r31)
  REX_STORE_U8(uint32_t(r31) + 10808, uint8_t(r11));
  // stb r10,10811(r31)
  REX_STORE_U8(uint32_t(r31) + 10811, uint8_t(r10));
  // rlwinm. r9,r11,0,27,27
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x825bf47c
  if (cr0.eq) goto loc_825BF47C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bf510
  goto loc_825BF510;
loc_825BF47C:
  // lbz r11,10808(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825bf508
  if (cr0.eq) goto loc_825BF508;
  // lwz r11,12184(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12184);
  // lwz r9,12468(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12468);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825bf4a0
  if (cr6.eq) goto loc_825BF4A0;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf508
  if (!cr6.eq) goto loc_825BF508;
loc_825BF4A0:
  // lwz r11,12188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12188);
  // lwz r9,12472(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12472);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825bf4b8
  if (cr6.eq) goto loc_825BF4B8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf508
  if (!cr6.eq) goto loc_825BF508;
loc_825BF4B8:
  // lwz r11,12192(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12192);
  // lwz r9,12476(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12476);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825bf4d0
  if (cr6.eq) goto loc_825BF4D0;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf508
  if (!cr6.eq) goto loc_825BF508;
loc_825BF4D0:
  // lwz r11,12196(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12196);
  // lwz r9,12480(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12480);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825bf4e8
  if (cr6.eq) goto loc_825BF4E8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf508
  if (!cr6.eq) goto loc_825BF508;
loc_825BF4E8:
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
  // lwz r9,12484(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12484);
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // beq cr6,0x825bf500
  if (cr6.eq) goto loc_825BF500;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf508
  if (!cr6.eq) goto loc_825BF508;
loc_825BF500:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bf50c
  goto loc_825BF50C;
loc_825BF508:
  // mr r11,r22
  r11 = r22;
loc_825BF50C:
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
loc_825BF510:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825bf54c
  if (cr0.eq) goto loc_825BF54C;
  // lbz r11,10808(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // rlwinm. r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825bf54c
  if (!cr0.eq) goto loc_825BF54C;
  // rlwinm. r11,r10,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x825bf544
  if (!cr0.eq) goto loc_825BF544;
  // lbz r11,11865(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 11865);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x825bf54c
  if (!cr0.eq) goto loc_825BF54C;
  // lbz r11,11931(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 11931);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x825bf54c
  if (!cr0.eq) goto loc_825BF54C;
loc_825BF544:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bf550
  goto loc_825BF550;
loc_825BF54C:
  // mr r11,r22
  r11 = r22;
loc_825BF550:
  // lbz r10,10808(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // mr r5,r30
  r5 = r30;
  // lwz r4,12488(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 12488);
  // rlwimi r10,r11,1,30,30
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0x2) | (r10 & 0xFFFFFFFFFFFFFFFD);
  // stw r22,12452(r31)
  REX_STORE_U32(uint32_t(r31) + 12452, uint32_t(r22));
  // mr r3,r31
  r3 = r31;
  // stw r22,10800(r31)
  REX_STORE_U32(uint32_t(r31) + 10800, uint32_t(r22));
  // stw r22,10804(r31)
  REX_STORE_U32(uint32_t(r31) + 10804, uint32_t(r22));
  // stw r9,12448(r31)
  REX_STORE_U32(uint32_t(r31) + 12448, uint32_t(r9));
  // stb r10,10808(r31)
  REX_STORE_U8(uint32_t(r31) + 10808, uint8_t(r10));
  // bl 0x822b6bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822B6BF8(base, LIFTED_ARGS(0x825BF580, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r30,r3
  r30 = r3;
  // addi r4,r11,13824
  r4 = uint64_t(int64_t(int64_t(r11) + 13824));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a3518
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A3518(base, LIFTED_ARGS(0x825BF594, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825bf5b4
  if (cr6.eq) goto loc_825BF5B4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a3b90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A3B90(base, LIFTED_ARGS(0x825BF5A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x825bf5b8
  goto loc_825BF5B8;
loc_825BF5B4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825BF5B8:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // lfd f31,-96(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825BF5C8: lifted from generated/ by lift.py.
lifted::Result f825BF5C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825BF5C8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r31,-32105
  r31 = uint64_t(int64_t(-2104033280));
  // mr r30,r4
  r30 = r4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,5240(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 5240);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf61c
  if (!cr6.eq) goto loc_825BF61C;
  // li r3,196
  r3 = uint64_t(int64_t(196));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x825BF5FC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825bf638
  if (cr6.eq) goto loc_825BF638;
  // bl 0x825c09a8
  // (left out: dead in C++)
  { const lifted::Result o = f825C09A8(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r11,5240(r31)
  REX_STORE_U32(uint32_t(r31) + 5240, uint32_t(r11));
  // beq cr6,0x825bf640
  if (cr6.eq) goto loc_825BF640;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825BF61C:
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_825BF620:
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
loc_825BF638:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,5240(r31)
  REX_STORE_U32(uint32_t(r31) + 5240, uint32_t(r11));
loc_825BF640:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x825bf620
  goto loc_825BF620;
}

// sub_825BF650: lifted from generated/ by lift.py.
uint64_t f825BF650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825BF650 (lifted)");
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825bf67c
  if (!cr6.eq) goto loc_825BF67C;
  // mr r3,r31
  r3 = r31;
  // b 0x825bf6a0
  goto loc_825BF6A0;
loc_825BF67C:
  // addi r3,r3,172
  r3 = uint64_t(int64_t(int64_t(r3) + 172));
  // bl 0x825d8f18
  // (left out: dead in C++)
  r3 = f825D8F18(base, r3);
  // subf r11,r31,r30
  r11 = r30 - r31;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // divwu r10,r3,r11
  r10 = uint32_t(uint32_t(r11) ? uint32_t(r3) / uint32_t(r11) : 0);
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // subf r11,r11,r3
  r11 = r3 - r11;
  // add r3,r11,r31
  r3 = r11 + r31;
loc_825BF6A0:
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

// sub_825BF6B8: lifted from generated/ by lift.py.
uint64_t f825BF6B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825BF6B8 (lifted)");
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // cmpw cr6,r31,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r30), so);
  // bne cr6,0x825bf6e4
  if (!cr6.eq) goto loc_825BF6E4;
  // mr r3,r31
  r3 = r31;
  // b 0x825bf708
  goto loc_825BF708;
loc_825BF6E4:
  // addi r3,r3,172
  r3 = uint64_t(int64_t(int64_t(r3) + 172));
  // bl 0x825d8f18
  // (left out: dead in C++)
  r3 = f825D8F18(base, r3);
  // subf r11,r31,r30
  r11 = r30 - r31;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // divwu r10,r3,r11
  r10 = uint32_t(uint32_t(r11) ? uint32_t(r3) / uint32_t(r11) : 0);
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
  // mullw r11,r10,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // subf r11,r11,r3
  r11 = r3 - r11;
  // add r3,r11,r31
  r3 = r11 + r31;
loc_825BF708:
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

// sub_825BF720: lifted from generated/ by lift.py.
lifted::Result f825BF720(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_825BF720 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f30,-32(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f30));
  // stfd f31,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // fmr f31,f1
  f31 = f1;
  // mr r31,r3
  r31 = r3;
  // fmr f30,f2
  f30 = f2;
  // fcmpu cr6,f31,f30
  cr6 = lifted::CompareFloat(f31, f30);
  // bne cr6,0x825bf750
  if (!cr6.eq) goto loc_825BF750;
  // b 0x825bf78c
  goto loc_825BF78C;
loc_825BF750:
  // addi r3,r31,172
  r3 = uint64_t(int64_t(int64_t(r31) + 172));
  // bl 0x825d8f18
  // (left out: dead in C++)
  r3 = f825D8F18(base, r3);
  // lwz r11,176(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 176);
  // fsubs f0,f30,f31
  lifted::ScalarFloatMode();
  f0 = double(float(f30 - f31));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // clrldi r11,r3,32
  r11 = r3 & 0xFFFFFFFF;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // fmadds f1,f13,f0,f31
  f1 = double(float(std::fma(f13, f0, f31)));
loc_825BF78C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-32(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825BF7A8: lifted from generated/ by lift.py.
lifted::Result f825BF7A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825BF7A8 (lifted)");
  uint64_t r0 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r3
  r30 = r3;
  // lbz r10,0(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 0);
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // clrlwi r11,r10,30
  r11 = uint32_t(r10) & 0x3;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bf818
  if (cr6.lt) goto loc_825BF818;
  // beq cr6,0x825bf804
  if (cr6.eq) goto loc_825BF804;
  // lwz r11,5(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 5);
  // lwz r10,1(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 1);
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // lfs f2,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f2 = double(temp.f32);
  // lfs f1,92(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f1 = double(temp.f32);
  // bl 0x825bf720
  // (left out: dead in C++)
  { const lifted::Result o = f825BF720(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
  // stfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // b 0x825bf838
  goto loc_825BF838;
loc_825BF804:
  // lwz r11,1(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // lfs f1,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f1 = double(temp.f32);
  // stfs f1,80(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // b 0x825bf838
  goto loc_825BF838;
loc_825BF818:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lhz r4,1(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 1);
  // mr r3,r30
  r3 = r30;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BF834, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_825BF838:
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // rlwinm r11,r11,30,30,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bf88c
  if (cr6.lt) goto loc_825BF88C;
  // beq cr6,0x825bf878
  if (cr6.eq) goto loc_825BF878;
  // lwz r11,13(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 13);
  // mr r3,r30
  r3 = r30;
  // lwz r10,9(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 9);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lfs f2,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f2 = double(temp.f32);
  // lfs f1,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f1 = double(temp.f32);
  // bl 0x825bf720
  // (left out: dead in C++)
  { const lifted::Result o = f825BF720(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // b 0x825bf8ac
  goto loc_825BF8AC;
loc_825BF878:
  // lwz r11,9(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // lfs f0,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // b 0x825bf8b0
  goto loc_825BF8B0;
loc_825BF88C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lhz r4,9(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 9);
  // mr r3,r30
  r3 = r30;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BF8A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f0 = double(temp.f32);
loc_825BF8AC:
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_825BF8B0:
  // lbz r10,33(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 33);
  // rlwinm r11,r10,30,29,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x7;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bgt cr6,0x825bf950
  if (cr6.gt) goto loc_825BF950;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,-1832
  r12 = uint64_t(int64_t(int64_t(r12) + -1832));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825BF8EC;
  case 1:
  goto loc_825BF910;
  case 2:
  goto loc_825BF920;
  case 3:
  goto loc_825BF930;
  case 4:
  goto loc_825BF940;
  default:
  std::abort();
  }
loc_825BF8EC:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825bf958
  if (cr6.lt) goto loc_825BF958;
loc_825BF8F4:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_825BF8F8:
  // clrlwi r11,r10,30
  r11 = uint32_t(r10) & 0x3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825bf960
  if (!cr6.eq) goto loc_825BF960;
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825BF910:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x825bf8f4
  if (cr6.gt) goto loc_825BF8F4;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825bf8f8
  goto loc_825BF8F8;
loc_825BF920:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x825bf8f4
  if (!cr6.gt) goto loc_825BF8F4;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825bf8f8
  goto loc_825BF8F8;
loc_825BF930:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825bf8f4
  if (cr6.lt) goto loc_825BF8F4;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825bf8f8
  goto loc_825BF8F8;
loc_825BF940:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // bne cr6,0x825bf8f4
  if (!cr6.eq) goto loc_825BF8F4;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825bf8f8
  goto loc_825BF8F8;
loc_825BF950:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // beq cr6,0x825bf8f4
  if (cr6.eq) goto loc_825BF8F4;
loc_825BF958:
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // b 0x825bf8f8
  goto loc_825BF8F8;
loc_825BF960:
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // rlwinm r11,r11,28,30,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bf9b0
  if (cr6.lt) goto loc_825BF9B0;
  // beq cr6,0x825bf99c
  if (cr6.eq) goto loc_825BF99C;
  // lwz r11,21(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 21);
  // mr r3,r30
  r3 = r30;
  // lwz r10,17(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 17);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lfs f2,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f2 = double(temp.f32);
  // lfs f1,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f1 = double(temp.f32);
  // bl 0x825bf720
  // (left out: dead in C++)
  { const lifted::Result o = f825BF720(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
  // stfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // b 0x825bf9d0
  goto loc_825BF9D0;
loc_825BF99C:
  // lwz r11,17(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 17);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // lfs f1,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f1 = double(temp.f32);
  // stfs f1,80(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // b 0x825bf9d0
  goto loc_825BF9D0;
loc_825BF9B0:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lhz r4,17(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 17);
  // mr r3,r30
  r3 = r30;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BF9CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_825BF9D0:
  // lbz r11,0(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 0);
  // rlwinm r11,r11,26,6,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 26) & 0x3FFFFFF;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825bfa1c
  if (cr6.lt) goto loc_825BFA1C;
  // beq cr6,0x825bfa0c
  if (cr6.eq) goto loc_825BFA0C;
  // lwz r11,29(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 29);
  // mr r3,r30
  r3 = r30;
  // lwz r10,25(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 25);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lfs f2,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f2 = double(temp.f32);
  // lfs f1,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f1 = double(temp.f32);
  // bl 0x825bf720
  // (left out: dead in C++)
  { const lifted::Result o = f825BF720(base, r1, r13, r3, f1, f2); r3 = o.r3; f1 = o.f1; }
  // fmr f0,f1
  lifted::ScalarFloatMode();
  f0 = f1;
  // b 0x825bfa3c
  goto loc_825BFA3C;
loc_825BFA0C:
  // lwz r11,25(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 25);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // lfs f0,92(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 92);
  f0 = double(temp.f32);
  // b 0x825bfa40
  goto loc_825BFA40;
loc_825BFA1C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lhz r4,25(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 25);
  // mr r3,r30
  r3 = r30;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BFA38, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,84(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f0 = double(temp.f32);
loc_825BFA3C:
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_825BFA40:
  // lbz r10,33(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 33);
  // rlwinm r11,r10,27,5,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x7FFFFFF;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bgt cr6,0x825bfae8
  if (cr6.gt) goto loc_825BFAE8;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,-1432
  r12 = uint64_t(int64_t(int64_t(r12) + -1432));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825BFA7C;
  case 1:
  goto loc_825BFAA8;
  case 2:
  goto loc_825BFAB8;
  case 3:
  goto loc_825BFAC8;
  case 4:
  goto loc_825BFAD8;
  default:
  std::abort();
  }
loc_825BFA7C:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825bfaf0
  if (cr6.lt) goto loc_825BFAF0;
loc_825BFA84:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825BFA88:
  // clrlwi r10,r10,30
  r10 = uint32_t(r10) & 0x3;
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x825bfb18
  if (cr6.eq) goto loc_825BFB18;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x825bfaf8
  if (cr6.eq) goto loc_825BFAF8;
  // xor r3,r11,r29
  r3 = r11 ^ r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825BFAA8:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x825bfa84
  if (cr6.gt) goto loc_825BFA84;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bfa88
  goto loc_825BFA88;
loc_825BFAB8:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x825bfa84
  if (!cr6.gt) goto loc_825BFA84;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bfa88
  goto loc_825BFA88;
loc_825BFAC8:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825bfa84
  if (cr6.lt) goto loc_825BFA84;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bfa88
  goto loc_825BFA88;
loc_825BFAD8:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // bne cr6,0x825bfa84
  if (!cr6.eq) goto loc_825BFA84;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bfa88
  goto loc_825BFA88;
loc_825BFAE8:
  // fcmpu cr6,f1,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f0);
  // beq cr6,0x825bfa84
  if (cr6.eq) goto loc_825BFA84;
loc_825BFAF0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x825bfa88
  goto loc_825BFA88;
loc_825BFAF8:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x825bfb0c
  if (!cr6.eq) goto loc_825BFB0C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // beq cr6,0x825bfb30
  if (cr6.eq) goto loc_825BFB30;
loc_825BFB0C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825BFB18:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x825bfb2c
  if (cr6.eq) goto loc_825BFB2C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bne cr6,0x825bfb30
  if (!cr6.eq) goto loc_825BFB30;
loc_825BFB2C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825BFB30:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825BFDF8: lifted from generated/ by lift.py.
lifted::Result f825BFDF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825BFDF8 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825BFF38: lifted from generated/ by lift.py.
lifted::Result f825BFF38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825BFF38 (lifted)");
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
  // mr r28,r4
  r28 = r4;
  // mr r30,r3
  r30 = r3;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // mr r31,r27
  r31 = r27;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // stw r11,40(r30)
  REX_STORE_U32(uint32_t(r30) + 40, uint32_t(r11));
  // lwz r11,20(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 20);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bff6c
  if (cr6.eq) goto loc_825BFF6C;
  // stw r11,188(r30)
  REX_STORE_U32(uint32_t(r30) + 188, uint32_t(r11));
loc_825BFF6C:
  // lwz r11,24(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825bff7c
  if (cr6.eq) goto loc_825BFF7C;
  // stw r11,192(r30)
  REX_STORE_U32(uint32_t(r30) + 192, uint32_t(r11));
loc_825BFF7C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,68(r30)
  REX_STORE_U32(uint32_t(r30) + 68, uint32_t(r11));
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c0040
  if (cr6.eq) goto loc_825C0040;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // li r3,148
  r3 = uint64_t(int64_t(148));
  // ori r4,r4,32800
  r4 = r4 | 32800;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822976F8(base, LIFTED_ARGS(0x825BFFA0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c0010
  if (cr6.eq) goto loc_825C0010;
  // mr r4,r30
  r4 = r30;
  // bl 0x825c21b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C21B0(base, LIFTED_ARGS(0x825BFFB0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x825c0018
  if (!cr6.eq) goto loc_825C0018;
loc_825BFFBC:
  // lis r29,-32761
  r29 = uint64_t(int64_t(-2147024896));
  // ori r29,r29,14
  r29 = r29 | 14;
loc_825BFFC4:
  // lwz r3,12(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 12);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825bffe0
  if (cr6.eq) goto loc_825BFFE0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825BFFE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825BFFE0:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c0000
  if (cr6.eq) goto loc_825C0000;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0000, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C0000:
  // stw r27,68(r30)
  REX_STORE_U32(uint32_t(r30) + 68, uint32_t(r27));
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C0010:
  // mr r31,r27
  r31 = r27;
  // b 0x825bffbc
  goto loc_825BFFBC;
loc_825C0018:
  // lwz r11,12(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 12);
  // mr r3,r31
  r3 = r31;
  // lwz r7,16(r28)
  r7 = REX_LOAD_U32(uint32_t(r28) + 16);
  // clrlwi r6,r11,31
  r6 = uint32_t(r11) & 0x1;
  // lwz r5,8(r28)
  r5 = REX_LOAD_U32(uint32_t(r28) + 8);
  // lwz r4,4(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 4);
  // bl 0x825c26e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C26E0(base, LIFTED_ARGS(0x825C0034, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825bffc4
  if (cr6.lt) goto loc_825BFFC4;
loc_825C0040:
  // mr r5,r27
  r5 = r27;
  // stw r27,180(r30)
  REX_STORE_U32(uint32_t(r30) + 180, uint32_t(r27));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c0074
  if (cr6.eq) goto loc_825C0074;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0064, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,21(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 21);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r5,r11,1
  r5 = r11 ^ 1;
loc_825C0074:
  // addi r6,r30,12
  r6 = uint64_t(int64_t(int64_t(r30) + 12));
  // mr r4,r30
  r4 = r30;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825c3460
  // (left out: dead in C++)
  { const lifted::Result o = f825C3460(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825bffc4
  if (cr6.lt) goto loc_825BFFC4;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c00a4
  if (cr6.eq) goto loc_825C00A4;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c27c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C27C0(base, LIFTED_ARGS(0x825C00A0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825C00A4:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825bffc4
  if (cr6.lt) goto loc_825BFFC4;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // stw r31,124(r30)
  REX_STORE_U32(uint32_t(r30) + 124, uint32_t(r31));
  // beq cr6,0x825c00c8
  if (cr6.eq) goto loc_825C00C8;
  // mr r3,r31
  r3 = r31;
  // lwz r4,4(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 4);
  // bl 0x825c2910
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C2910(base, LIFTED_ARGS(0x825C00C4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825C00C8:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825bffc4
  if (cr6.lt) goto loc_825BFFC4;
  // lwz r11,28(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 28);
  // stw r11,16(r30)
  REX_STORE_U32(uint32_t(r30) + 16, uint32_t(r11));
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825BFE28(base, LIFTED_ARGS(0x825C00DC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r29
  r3 = r29;
  // stw r11,172(r30)
  REX_STORE_U32(uint32_t(r30) + 172, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C00F0: lifted from generated/ by lift.py.
lifted::Result f825C00F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C00F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r31,124(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 124);
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c015c
  if (cr6.eq) goto loc_825C015C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c38a8
  // (left out: dead in C++)
  { const lifted::Result o = f825C38A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825c0158
  if (cr6.eq) goto loc_825C0158;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C0144, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 9);
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c015c
  if (cr6.eq) goto loc_825C015C;
loc_825C0158:
  // mr r3,r30
  r3 = r30;
loc_825C015C:
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

// sub_825C0178: lifted from generated/ by lift.py.
lifted::Result f825C0178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C0178 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // mr r29,r4
  r29 = r4;
  // addi r28,r31,144
  r28 = uint64_t(int64_t(int64_t(r31) + 144));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8e4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlEnterCriticalSection, LIFTED_ARGS(0x825C01A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,124(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 124);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825c01bc
  if (!cr6.eq) goto loc_825C01BC;
  // lis r27,-30009
  r27 = uint64_t(int64_t(-1966669824));
  // ori r27,r27,9
  r27 = r27 | 9;
  // b 0x825c0230
  goto loc_825C0230;
loc_825C01BC:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C01D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 9);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c0228
  if (cr6.eq) goto loc_825C0228;
  // mr r4,r29
  r4 = r29;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // mr r3,r30
  r3 = r30;
  // bl 0x825c4070
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825C4070(base, LIFTED_ARGS(0x825C01F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,28
  r30 = uint64_t(int64_t(int64_t(r31) + 28));
  // lwz r31,32(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 32);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825c0230
  if (cr6.eq) goto loc_825C0230;
loc_825C0204:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r3,r31,-12
  r3 = uint64_t(int64_t(int64_t(r31) + -12));
  // bne cr6,0x825c0214
  if (!cr6.eq) goto loc_825C0214;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825C0214:
  // bl 0x825c5610
  // (left out: dead in C++)
  { const lifted::Result o = f825C5610(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825c0204
  if (!cr6.eq) goto loc_825C0204;
  // b 0x825c0230
  goto loc_825C0230;
loc_825C0228:
  // lis r27,-30009
  r27 = uint64_t(int64_t(-1966669824));
  // ori r27,r27,11
  r27 = r27 | 11;
loc_825C0230:
  // mr r3,r28
  r3 = r28;
  // bl 0x825db8d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(0x825C0238, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f31,-56(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e82594440(uint8_t* base, const lifted::Args& a) {
  return f82594440(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825944B0(uint8_t* base, const lifted::Args& a) {
  return f825944B0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825946B0(uint8_t* base, const lifted::Args& a) {
  return f825946B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82594FD0(uint8_t* base, const lifted::Args& a) {
  return f82594FD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825968D0(uint8_t* base, const lifted::Args& a) {
  return f825968D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82596E88(uint8_t* base, const lifted::Args& a) {
  return {f82596E88(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82599BE0(uint8_t* base, const lifted::Args& a) {
  return f82599BE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259A2B0(uint8_t* base, const lifted::Args& a) {
  return {f8259A2B0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8259B600(uint8_t* base, const lifted::Args& a) {
  return f8259B600(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259BB98(uint8_t* base, const lifted::Args& a) {
  return f8259BB98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e8259C550(uint8_t* base, const lifted::Args& a) {
  return {f8259C550(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8259C6C0(uint8_t* base, const lifted::Args& a) {
  return {f8259C6C0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8259C6F0(uint8_t* base, const lifted::Args& a) {
  f8259C6F0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8259C700(uint8_t* base, const lifted::Args& a) {
  return f8259C700(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8259C770(uint8_t* base, const lifted::Args& a) {
  return f8259C770(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8259C7F8(uint8_t* base, const lifted::Args& a) {
  f8259C7F8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8259C8C8(uint8_t* base, const lifted::Args& a) {
  return f8259C8C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8259C910(uint8_t* base, const lifted::Args& a) {
  return f8259C910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8259CB48(uint8_t* base, const lifted::Args& a) {
  return f8259CB48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259CBE8(uint8_t* base, const lifted::Args& a) {
  return f8259CBE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259CC38(uint8_t* base, const lifted::Args& a) {
  return f8259CC38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8259CC58(uint8_t* base, const lifted::Args& a) {
  return f8259CC58(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8259CCA0(uint8_t* base, const lifted::Args& a) {
  return f8259CCA0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259CCF8(uint8_t* base, const lifted::Args& a) {
  return f8259CCF8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259CE78(uint8_t* base, const lifted::Args& a) {
  return f8259CE78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[6], a.r[7]);
}

lifted::Result e8259CF10(uint8_t* base, const lifted::Args& a) {
  return f8259CF10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[6], a.r[7]);
}

lifted::Result e8259CFA8(uint8_t* base, const lifted::Args& a) {
  return f8259CFA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[6], a.r[7]);
}

lifted::Result e8259D038(uint8_t* base, const lifted::Args& a) {
  return f8259D038(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[6], a.r[7]);
}

lifted::Result e8259D0C8(uint8_t* base, const lifted::Args& a) {
  return f8259D0C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8259D100(uint8_t* base, const lifted::Args& a) {
  return f8259D100(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8259D120(uint8_t* base, const lifted::Args& a) {
  return f8259D120(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8259D140(uint8_t* base, const lifted::Args& a) {
  return f8259D140(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8259D160(uint8_t* base, const lifted::Args& a) {
  return f8259D160(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e8259D1C0(uint8_t* base, const lifted::Args& a) {
  return f8259D1C0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e8259D1F0(uint8_t* base, const lifted::Args& a) {
  return f8259D1F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259D370(uint8_t* base, const lifted::Args& a) {
  return f8259D370(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8259D3F0(uint8_t* base, const lifted::Args& a) {
  return f8259D3F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8259D5B8(uint8_t* base, const lifted::Args& a) {
  f8259D5B8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825A1318(uint8_t* base, const lifted::Args& a) {
  return f825A1318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A13C0(uint8_t* base, const lifted::Args& a) {
  return f825A13C0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e825A13D8(uint8_t* base, const lifted::Args& a) {
  return f825A13D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A1438(uint8_t* base, const lifted::Args& a) {
  return f825A1438(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A1610(uint8_t* base, const lifted::Args& a) {
  return f825A1610(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A16B0(uint8_t* base, const lifted::Args& a) {
  return f825A16B0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A17A0(uint8_t* base, const lifted::Args& a) {
  return f825A17A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A1810(uint8_t* base, const lifted::Args& a) {
  return f825A1810(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A2238(uint8_t* base, const lifted::Args& a) {
  return {f825A2238(base, a.r[1]), a.f[0]};
}

lifted::Result e825A2250(uint8_t* base, const lifted::Args& a) {
  return f825A2250(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825A22E8(uint8_t* base, const lifted::Args& a) {
  return f825A22E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A2360(uint8_t* base, const lifted::Args& a) {
  return {f825A2360(base, a.r[1]), a.f[0]};
}

lifted::Result e825A2370(uint8_t* base, const lifted::Args& a) {
  return {f825A2370(base, a.r[0]), a.f[0]};
}

lifted::Result e825A2388(uint8_t* base, const lifted::Args& a) {
  return {f825A2388(base, a.r[0]), a.f[0]};
}

lifted::Result e825A23A0(uint8_t* base, const lifted::Args& a) {
  return {f825A23A0(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825A2CD8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A2CD8(base, a.r1, a.r[0])};
}

lifted::Result e825A3108(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A3108(base, a.r1, a.r[0])};
}

lifted::Result e825A31F0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A31F0(base, a.r1, a.r[0])};
}

lifted::Result e825A3380(uint8_t* base, const lifted::Args& a) {
  f825A3380(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825A3408(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A3408(base, a.r1, a.r[0])};
}

lifted::Result e825A3980(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A3980(base, a.r1, a.r[0])};
}

lifted::Result e825A3B80(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825A3B80(base, a.r1, a.r13, a.r[0], a.r[1])};
}

lifted::Result e825A3E18(uint8_t* base, const lifted::Args& a) {
  return f825A3E18(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A4600(uint8_t* base, const lifted::Args& a) {
  return f825A4600(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A4680(uint8_t* base, const lifted::Args& a) {
  return f825A4680(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A5020(uint8_t* base, const lifted::Args& a) {
  return f825A5020(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A50E0(uint8_t* base, const lifted::Args& a) {
  return f825A50E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825A5150(uint8_t* base, const lifted::Args& a) {
  return f825A5150(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e825A5320(uint8_t* base, const lifted::Args& a) {
  return f825A5320(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

lifted::Result e825A5408(uint8_t* base, const lifted::Args& a) {
  return f825A5408(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A54C0(uint8_t* base, const lifted::Args& a) {
  return f825A54C0(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e825A5790(uint8_t* base, const lifted::Args& a) {
  return f825A5790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825A5818(uint8_t* base, const lifted::Args& a) {
  return {f825A5818(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825A5828(uint8_t* base, const lifted::Args& a) {
  return {f825A5828(base, a.r[1]), a.f[0]};
}

lifted::Result e825A5838(uint8_t* base, const lifted::Args& a) {
  return f825A5838(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A5BF0(uint8_t* base, const lifted::Args& a) {
  return f825A5BF0(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e825A5CC0(uint8_t* base, const lifted::Args& a) {
  return f825A5CC0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e825A5D48(uint8_t* base, const lifted::Args& a) {
  return f825A5D48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A5DB8(uint8_t* base, const lifted::Args& a) {
  return f825A5DB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825A5E28(uint8_t* base, const lifted::Args& a) {
  return f825A5E28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A6070(uint8_t* base, const lifted::Args& a) {
  return f825A6070(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A6110(uint8_t* base, const lifted::Args& a) {
  return {f825A6110(base, a.r[0]), a.f[0]};
}

lifted::Result e825A6B18(uint8_t* base, const lifted::Args& a) {
  return f825A6B18(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825A6CC0(uint8_t* base, const lifted::Args& a) {
  return f825A6CC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e825A6D08(uint8_t* base, const lifted::Args& a) {
  return f825A6D08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.lr);
}

lifted::Result e825A6D20(uint8_t* base, const lifted::Args& a) {
  return f825A6D20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825A6D28(uint8_t* base, const lifted::Args& a) {
  return f825A6D28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e825A6D30(uint8_t* base, const lifted::Args& a) {
  return f825A6D30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825AE2C0(uint8_t* base, const lifted::Args& a) {
  return f825AE2C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825AEB70(uint8_t* base, const lifted::Args& a) {
  return f825AEB70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825B9D30(uint8_t* base, const lifted::Args& a) {
  return f825B9D30(base, a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825BA1E0(uint8_t* base, const lifted::Args& a) {
  return f825BA1E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825BF200(uint8_t* base, const lifted::Args& a) {
  return f825BF200(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825BF5C8(uint8_t* base, const lifted::Args& a) {
  return f825BF5C8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825BF650(uint8_t* base, const lifted::Args& a) {
  return {f825BF650(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825BF6B8(uint8_t* base, const lifted::Args& a) {
  return {f825BF6B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825BF720(uint8_t* base, const lifted::Args& a) {
  return f825BF720(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1]);
}

lifted::Result e825BF7A8(uint8_t* base, const lifted::Args& a) {
  return f825BF7A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825BFDF8(uint8_t* base, const lifted::Args& a) {
  return f825BFDF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825BFF38(uint8_t* base, const lifted::Args& a) {
  return f825BFF38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C00F0(uint8_t* base, const lifted::Args& a) {
  return f825C00F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C0178(uint8_t* base, const lifted::Args& a) {
  return f825C0178(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x82594440, e82594440},
    {0x825944B0, e825944B0},
    {0x825946B0, e825946B0},
    {0x82594FD0, e82594FD0},
    {0x825968D0, e825968D0},
    {0x82596E88, e82596E88},
    {0x82599BE0, e82599BE0},
    {0x8259A2B0, e8259A2B0},
    {0x8259B600, e8259B600},
    {0x8259BB98, e8259BB98},
    {0x8259C550, e8259C550},
    {0x8259C6C0, e8259C6C0},
    {0x8259C6F0, e8259C6F0},
    {0x8259C700, e8259C700},
    {0x8259C770, e8259C770},
    {0x8259C7F8, e8259C7F8},
    {0x8259C8C8, e8259C8C8},
    {0x8259C910, e8259C910},
    {0x8259CB48, e8259CB48},
    {0x8259CBE8, e8259CBE8},
    {0x8259CC38, e8259CC38},
    {0x8259CC58, e8259CC58},
    {0x8259CCA0, e8259CCA0},
    {0x8259CCF8, e8259CCF8},
    {0x8259CE78, e8259CE78},
    {0x8259CF10, e8259CF10},
    {0x8259CFA8, e8259CFA8},
    {0x8259D038, e8259D038},
    {0x8259D0C8, e8259D0C8},
    {0x8259D100, e8259D100},
    {0x8259D120, e8259D120},
    {0x8259D140, e8259D140},
    {0x8259D160, e8259D160},
    {0x8259D1C0, e8259D1C0},
    {0x8259D1F0, e8259D1F0},
    {0x8259D370, e8259D370},
    {0x8259D3F0, e8259D3F0},
    {0x8259D5B8, e8259D5B8},
    {0x825A1318, e825A1318},
    {0x825A13C0, e825A13C0},
    {0x825A13D8, e825A13D8},
    {0x825A1438, e825A1438},
    {0x825A1610, e825A1610},
    {0x825A16B0, e825A16B0},
    {0x825A17A0, e825A17A0},
    {0x825A1810, e825A1810},
    {0x825A2238, e825A2238},
    {0x825A2250, e825A2250},
    {0x825A22E8, e825A22E8},
    {0x825A2360, e825A2360},
    {0x825A2370, e825A2370},
    {0x825A2388, e825A2388},
    {0x825A23A0, e825A23A0},
    {0x825A2CD8, e825A2CD8},
    {0x825A3108, e825A3108},
    {0x825A31F0, e825A31F0},
    {0x825A3380, e825A3380},
    {0x825A3408, e825A3408},
    {0x825A3980, e825A3980},
    {0x825A3B80, e825A3B80},
    {0x825A3E18, e825A3E18},
    {0x825A4600, e825A4600},
    {0x825A4680, e825A4680},
    {0x825A5020, e825A5020},
    {0x825A50E0, e825A50E0},
    {0x825A5150, e825A5150},
    {0x825A5320, e825A5320},
    {0x825A5408, e825A5408},
    {0x825A54C0, e825A54C0},
    {0x825A5790, e825A5790},
    {0x825A5818, e825A5818},
    {0x825A5828, e825A5828},
    {0x825A5838, e825A5838},
    {0x825A5BF0, e825A5BF0},
    {0x825A5CC0, e825A5CC0},
    {0x825A5D48, e825A5D48},
    {0x825A5DB8, e825A5DB8},
    {0x825A5E28, e825A5E28},
    {0x825A6070, e825A6070},
    {0x825A6110, e825A6110},
    {0x825A6B18, e825A6B18},
    {0x825A6CC0, e825A6CC0},
    {0x825A6D08, e825A6D08},
    {0x825A6D20, e825A6D20},
    {0x825A6D28, e825A6D28},
    {0x825A6D30, e825A6D30},
    {0x825AE2C0, e825AE2C0},
    {0x825AEB70, e825AEB70},
    {0x825B9D30, e825B9D30},
    {0x825BA1E0, e825BA1E0},
    {0x825BF200, e825BF200},
    {0x825BF5C8, e825BF5C8},
    {0x825BF650, e825BF650},
    {0x825BF6B8, e825BF6B8},
    {0x825BF720, e825BF720},
    {0x825BF7A8, e825BF7A8},
    {0x825BFDF8, e825BFDF8},
    {0x825BFF38, e825BFF38},
    {0x825C00F0, e825C00F0},
    {0x825C0178, e825C0178},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_021)), true);

}  // namespace

}  // namespace lifted
