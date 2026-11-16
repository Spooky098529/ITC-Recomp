// Lift sweep batch 014 (docs/loops/lift-sweep.md): written by
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

ITC_FLAG_BOOL(native_lifted_014, true, "ITC",
              "Run lift sweep batch 014's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_822929E8: lifted from generated/ by lift.py.
lifted::Result f822929E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822929E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,160(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 160);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82292a18
  if (cr6.lt) goto loc_82292A18;
  // bne cr6,0x82292a28
  if (!cr6.eq) goto loc_82292A28;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82292a28
  goto loc_82292A28;
loc_82292A18:
  // li r3,4111
  r3 = uint64_t(int64_t(4111));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x822917b8
  // (left out: dead in C++)
  { const lifted::Result o = f822917B8(base, r1, r13, r3, r8); r3 = o.r3; f1 = o.f1; }
loc_82292A28:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  REX_STORE_U32(uint32_t(r31) + 160, uint32_t(r11));
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

// sub_82292A48: lifted from generated/ by lift.py.
lifted::Result f82292A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82292A48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r26,r3
  r26 = r3;
  // bl 0x82292648
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82292648(base, LIFTED_ARGS(0x82292A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r27,-32148
  r27 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,25616
  r29 = uint64_t(int64_t(int64_t(r11) + 25616));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r28,r11,25312
  r28 = uint64_t(int64_t(int64_t(r11) + 25312));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r4,27336(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 27336);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r7,r29
  r7 = r29;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // mr r5,r28
  r5 = r28;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822950c0
  // (left out: dead in C++)
  { const lifted::Result o = f822950C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,122
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 122, so);
  // beq cr6,0x82292ab0
  if (cr6.eq) goto loc_82292AB0;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82292b20
  goto loc_82292B20;
loc_82292AB0:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82292AB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r30,27348(r11)
  REX_STORE_U32(uint32_t(r11) + 27348, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x82292ADC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r31
  r10 = r31;
  // lwz r4,27336(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 27336);
  // mr r9,r30
  r9 = r30;
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r7,r29
  r7 = r29;
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // mr r5,r28
  r5 = r28;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822950c0
  // (left out: dead in C++)
  { const lifted::Result o = f822950C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82292b10
  if (cr6.eq) goto loc_82292B10;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82292B10:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r26,r11
  REX_STORE_U32(uint32_t(r26) + uint32_t(r11), uint32_t(r10));
loc_82292B20:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82292B28: lifted from generated/ by lift.py.
lifted::Result f82292B28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82292B28 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq cr6,0x82292c14
  if (cr6.eq) goto loc_82292C14;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r30,224(r31)
  REX_STORE_U32(uint32_t(r31) + 224, uint32_t(r30));
  // stw r30,232(r31)
  REX_STORE_U32(uint32_t(r31) + 232, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x82292B60, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r9,19
  r9 = uint64_t(int64_t(1245184));
  // lwz r8,220(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 220);
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // lwz r5,216(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 216);
  // ori r9,r9,52544
  r9 = r9 | 52544;
  // lis r7,54
  r7 = uint64_t(int64_t(3538944));
  // ori r10,r11,52540
  r10 = r11 | 52540;
  // ori r4,r7,61056
  r4 = r7 | 61056;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // stwx r30,r31,r9
  REX_STORE_U32(uint32_t(r31) + uint32_t(r9), uint32_t(r30));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lis r7,8192
  r7 = uint64_t(int64_t(536870912));
  // addi r11,r31,48
  r11 = uint64_t(int64_t(int64_t(r31) + 48));
  // lwzx r10,r31,r10
  r10 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r10));
  // ori r3,r7,6
  r3 = r7 | 6;
  // lis r30,8192
  r30 = uint64_t(int64_t(536870912));
  // lwz r9,29428(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 29428);
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // stw r8,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r8));
  // lis r8,4096
  r8 = uint64_t(int64_t(268435456));
  // stw r5,136(r31)
  REX_STORE_U32(uint32_t(r31) + 136, uint32_t(r5));
  // lis r5,4096
  r5 = uint64_t(int64_t(268435456));
  // ori r29,r8,11
  r29 = r8 | 11;
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r9,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r9));
  // lis r9,19
  r9 = uint64_t(int64_t(1245184));
  // ori r30,r30,9
  r30 = r30 | 9;
  // stw r6,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r6));
  // ori r9,r9,52536
  r9 = r9 | 52536;
  // stb r7,56(r31)
  REX_STORE_U8(uint32_t(r31) + 56, uint8_t(r7));
  // ori r5,r5,12
  r5 = r5 | 12;
  // stb r7,80(r31)
  REX_STORE_U8(uint32_t(r31) + 80, uint8_t(r7));
  // stw r29,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r29));
  // stb r8,104(r31)
  REX_STORE_U8(uint32_t(r31) + 104, uint8_t(r8));
  // stw r30,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r30));
  // stwx r6,r31,r9
  REX_STORE_U32(uint32_t(r31) + uint32_t(r9), uint32_t(r6));
  // subf r9,r10,r4
  r9 = r4 - r10;
  // stw r5,120(r31)
  REX_STORE_U32(uint32_t(r31) + 120, uint32_t(r5));
  // clrldi r11,r9,32
  r11 = r9 & 0xFFFFFFFF;
  // stb r8,128(r31)
  REX_STORE_U8(uint32_t(r31) + 128, uint8_t(r8));
  // std r10,64(r31)
  REX_STORE_U64(uint32_t(r31) + 64, r10);
  // std r11,88(r31)
  REX_STORE_U64(uint32_t(r31) + 88, r11);
  // b 0x82292cb4
  goto loc_82292CB4;
loc_82292C14:
  // addis r29,r31,20
  r29 = uint64_t(int64_t(int64_t(r31) + 1310720));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r29,-12992
  r29 = uint64_t(int64_t(int64_t(r29) + -12992));
  // lwz r11,27336(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 27336);
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x82292c9c
  if (!cr6.lt) goto loc_82292C9C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // addi r30,r11,24704
  r30 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x82292C48, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r9,r10,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // addi r11,r11,25312
  r11 = uint64_t(int64_t(int64_t(r11) + 25312));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r6,r31,32
  r6 = uint64_t(int64_t(int64_t(r31) + 32));
  // mr r7,r30
  r7 = r30;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // ldx r4,r9,r11
  r4 = REX_LOAD_U64(uint32_t(r9) + uint32_t(r11));
  // lwz r3,27324(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 27324);
  // bl 0x825766e0
  // (left out: dead in C++)
  { const lifted::Result o = f825766E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82292c8c
  if (cr6.eq) goto loc_82292C8C;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,232(r31)
  REX_STORE_U32(uint32_t(r31) + 232, uint32_t(r11));
loc_82292C8C:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // b 0x82292cb4
  goto loc_82292CB4;
loc_82292C9C:
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x822919a0
  // (left out: dead in C++)
  { const lifted::Result o = f822919A0(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,224(r31)
  REX_STORE_U32(uint32_t(r31) + 224, uint32_t(r11));
loc_82292CB4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82292DB8: lifted from generated/ by lift.py.
lifted::Result f82292DB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82292DB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // lwz r3,27348(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 27348);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82292de4
  if (cr6.eq) goto loc_82292DE4;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x82292DDC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27348(r29)
  REX_STORE_U32(uint32_t(r29) + 27348, uint32_t(r11));
loc_82292DE4:
  // mr r3,r28
  r3 = r28;
  // bl 0x82292648
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82292648(base, LIFTED_ARGS(0x82292DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r9,r1,84
  r9 = uint64_t(int64_t(int64_t(r1) + 84));
  // ld r4,168(r28)
  r4 = REX_LOAD_U64(uint32_t(r28) + 168);
  // addi r7,r11,25616
  r7 = uint64_t(int64_t(int64_t(r11) + 25616));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82295208
  // (left out: dead in C++)
  { const lifted::Result o = f82295208(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82292e2c
  if (cr0.eq) goto loc_82292E2C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,180(r28)
  REX_STORE_U32(uint32_t(r28) + 180, uint32_t(r11));
  // b 0x82292e8c
  goto loc_82292E8C;
loc_82292E2C:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82292E34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r30,27348(r29)
  REX_STORE_U32(uint32_t(r29) + 27348, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x82292E54, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r7,r31
  r7 = r31;
  // lwz r5,80(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r4,r30
  r4 = r30;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // bl 0x82295d30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D30, LIFTED_ARGS(0x82292E6C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x82292e7c
  if (cr6.eq) goto loc_82292E7C;
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82292E7C:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r28,r11
  REX_STORE_U32(uint32_t(r28) + uint32_t(r11), uint32_t(r10));
loc_82292E8C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82292E98: lifted from generated/ by lift.py.
lifted::Result f82292E98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82292E98 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x82292f9c
  if (cr6.eq) goto loc_82292F9C;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x82292EC4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82292f38
  if (cr0.lt) goto loc_82292F38;
  // addis r10,r31,20
  r10 = uint64_t(int64_t(int64_t(r31) + 1310720));
  // addi r10,r10,-13000
  r10 = uint64_t(int64_t(int64_t(r10) + -13000));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82292f2c
  if (cr6.lt) goto loc_82292F2C;
  // beq cr6,0x82292f20
  if (cr6.eq) goto loc_82292F20;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82292f14
  if (cr6.lt) goto loc_82292F14;
  // beq cr6,0x82292f08
  if (cr6.eq) goto loc_82292F08;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x82292f9c
  if (!cr6.lt) goto loc_82292F9C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x82292b28
  // (left out: dead in C++)
  { const lifted::Result o = f82292B28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82292f9c
  goto loc_82292F9C;
loc_82292F08:
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // b 0x82292f58
  goto loc_82292F58;
loc_82292F14:
  // mr r3,r31
  r3 = r31;
  // bl 0x82292a48
  // (left out: dead in C++)
  { const lifted::Result o = f82292A48(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82292f9c
  goto loc_82292F9C;
loc_82292F20:
  // mr r3,r31
  r3 = r31;
  // bl 0x82291920
  // (left out: dead in C++)
  { const lifted::Result o = f82291920(base, r1, r13, r3, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x82292f9c
  goto loc_82292F9C;
loc_82292F2C:
  // mr r3,r31
  r3 = r31;
  // bl 0x82291858
  // (left out: dead in C++)
  { const lifted::Result o = f82291858(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // b 0x82292f9c
  goto loc_82292F9C;
loc_82292F38:
  // lis r11,-32747
  r11 = uint64_t(int64_t(-2146107392));
  // ori r11,r11,36866
  r11 = r11 | 36866;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // bne cr6,0x82292f6c
  if (!cr6.eq) goto loc_82292F6C;
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r31,r11
  REX_STORE_U32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_82292F58:
  // lis r9,19
  r9 = uint64_t(int64_t(1245184));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ori r9,r9,52540
  r9 = r9 | 52540;
  // stwx r8,r31,r9
  REX_STORE_U32(uint32_t(r31) + uint32_t(r9), uint32_t(r8));
  // b 0x82292f9c
  goto loc_82292F9C;
loc_82292F6C:
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,27324(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 27324);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82292f94
  if (cr6.eq) goto loc_82292F94;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82576080
  // (left out: dead in C++)
  { const lifted::Result o = f82576080(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,27324(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 27324);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82292F8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,27324(r31)
  REX_STORE_U32(uint32_t(r31) + 27324, uint32_t(r11));
loc_82292F94:
  // li r3,4109
  r3 = uint64_t(int64_t(4109));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82292F9C:
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

// sub_82292FB0: lifted from generated/ by lift.py.
lifted::Result f82292FB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82292FB0 (lifted)");
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
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // mr r31,r3
  r31 = r3;
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82292fe0
  if (cr6.eq) goto loc_82292FE0;
  // bl 0x82292e98
  // (left out: dead in C++)
  { const lifted::Result o = f82292E98(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293010
  goto loc_82293010;
loc_82292FE0:
  // lwz r11,160(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 160);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82293008
  if (!cr6.eq) goto loc_82293008;
  // lis r10,19
  r10 = uint64_t(int64_t(1245184));
  // ld r11,200(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 200);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // ori r10,r10,52540
  r10 = r10 | 52540;
  // mr r3,r31
  r3 = r31;
  // stwx r11,r31,r10
  REX_STORE_U32(uint32_t(r31) + uint32_t(r10), uint32_t(r11));
  // bl 0x82292b28
  // (left out: dead in C++)
  { const lifted::Result o = f82292B28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_82293008:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  REX_STORE_U32(uint32_t(r31) + 160, uint32_t(r11));
loc_82293010:
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

// sub_82293028: lifted from generated/ by lift.py.
lifted::Result f82293028(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82293028 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r27,r3
  r27 = r3;
  // lwz r11,236(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 236);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82293054
  if (!cr0.eq) goto loc_82293054;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,236(r27)
  REX_STORE_U32(uint32_t(r27) + 236, uint32_t(r11));
  // b 0x82293188
  goto loc_82293188;
loc_82293054:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82293064
  if (!cr6.eq) goto loc_82293064;
loc_8229305C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82293188
  goto loc_82293188;
loc_82293064:
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x82293074
  if (!cr6.eq) goto loc_82293074;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82293178
  goto loc_82293178;
loc_82293074:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addis r29,r27,20
  r29 = uint64_t(int64_t(int64_t(r27) + 1310720));
  // addi r29,r29,-13004
  r29 = uint64_t(int64_t(int64_t(r29) + -13004));
  // lwz r11,29428(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 29428);
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82293098
  if (cr0.eq) goto loc_82293098;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x82293098, r31)); r3 = o.r3; f1 = o.f1; }
loc_82293098:
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r31,r11,-32440
  r31 = uint64_t(int64_t(int64_t(r11) + -32440));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
  // mr r7,r31
  r7 = r31;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // lwz r28,-21596(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + -21596);
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r28
  r3 = r28;
  // bl 0x82576f30
  // (left out: dead in C++)
  { const lifted::Result o = f82576F30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x8229305c
  if (!cr0.eq) goto loc_8229305C;
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // ori r10,r11,65535
  r10 = r11 | 65535;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // rlwinm r3,r11,1,0,30
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // ble cr6,0x822930f4
  if (!cr6.gt) goto loc_822930F4;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_822930F4:
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x822930F8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r8,r3
  r8 = r3;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r7,r31
  r7 = r31;
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r8,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r8));
  // mr r3,r28
  r3 = r28;
  // bl 0x82576f30
  // (left out: dead in C++)
  { const lifted::Result o = f82576F30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x82293134, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // addi r30,r11,28240
  r30 = uint64_t(int64_t(int64_t(r11) + 28240));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r3,r30
  r3 = r30;
  // lwz r4,23376(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 23376);
  // bl 0x821a66a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821A66A8(base, LIFTED_ARGS(0x8229314C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,19
  r5 = uint64_t(int64_t(1245184));
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r7,r31
  r7 = r31;
  // mr r6,r30
  r6 = r30;
  // ori r5,r5,51440
  r5 = r5 | 51440;
  // mr r3,r28
  r3 = r28;
  // bl 0x82576be0
  // (left out: dead in C++)
  { const lifted::Result o = f82576BE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bne cr6,0x82293178
  if (!cr6.eq) goto loc_82293178;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82293178:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r27,r11
  REX_STORE_U32(uint32_t(r27) + uint32_t(r11), uint32_t(r10));
loc_82293188:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82293190: lifted from generated/ by lift.py.
lifted::Result f82293190(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82293190 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r11,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 997, so);
  // beq cr6,0x8229327c
  if (cr6.eq) goto loc_8229327C;
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x822931B8);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82293244
  if (cr0.lt) goto loc_82293244;
  // addis r30,r29,20
  r30 = uint64_t(int64_t(int64_t(r29) + 1310720));
  // addi r30,r30,-13000
  r30 = uint64_t(int64_t(int64_t(r30) + -13000));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x82293238
  if (cr6.eq) goto loc_82293238;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x8229322c
  if (cr6.eq) goto loc_8229322C;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x8229320c
  if (cr6.eq) goto loc_8229320C;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // beq cr6,0x82293200
  if (cr6.eq) goto loc_82293200;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bne cr6,0x8229327c
  if (!cr6.eq) goto loc_8229327C;
  // mr r3,r29
  r3 = r29;
  // bl 0x82291a40
  // (left out: dead in C++)
  { const lifted::Result o = f82291A40(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8229327c
  goto loc_8229327C;
loc_82293200:
  // mr r3,r29
  r3 = r29;
  // bl 0x82293028
  // (left out: dead in C++)
  { const lifted::Result o = f82293028(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8229327c
  goto loc_8229327C;
loc_8229320C:
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,27324(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 27324);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82293218, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // stw r11,27324(r31)
  REX_STORE_U32(uint32_t(r31) + 27324, uint32_t(r11));
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // b 0x8229327c
  goto loc_8229327C;
loc_8229322C:
  // mr r3,r29
  r3 = r29;
  // bl 0x82291ad0
  // (left out: dead in C++)
  { const lifted::Result o = f82291AD0(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8229327c
  goto loc_8229327C;
loc_82293238:
  // mr r3,r29
  r3 = r29;
  // bl 0x82292db8
  // (left out: dead in C++)
  { const lifted::Result o = f82292DB8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8229327c
  goto loc_8229327C;
loc_82293244:
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,27324(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 27324);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8229326c
  if (cr6.eq) goto loc_8229326C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82576080
  // (left out: dead in C++)
  { const lifted::Result o = f82576080(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,27324(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 27324);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82293264, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,27324(r31)
  REX_STORE_U32(uint32_t(r31) + 27324, uint32_t(r11));
loc_8229326C:
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // stwx r10,r29,r11
  REX_STORE_U32(uint32_t(r29) + uint32_t(r11), uint32_t(r10));
loc_8229327C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82293288: lifted from generated/ by lift.py.
lifted::Result f82293288(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82293288 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // mr r31,r3
  r31 = r3;
  // ori r11,r11,52536
  r11 = r11 | 52536;
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // beq cr6,0x822932b8
  if (cr6.eq) goto loc_822932B8;
  // bl 0x82293190
  // (left out: dead in C++)
  { const lifted::Result o = f82293190(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293344
  goto loc_82293344;
loc_822932B8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,236(r31)
  REX_STORE_U32(uint32_t(r31) + 236, uint32_t(r11));
  // lwz r11,160(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 160);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82293334
  if (cr6.lt) goto loc_82293334;
  // beq cr6,0x8229332c
  if (cr6.eq) goto loc_8229332C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x822932e4
  if (cr6.lt) goto loc_822932E4;
  // bne cr6,0x8229333c
  if (!cr6.eq) goto loc_8229333C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82293338
  goto loc_82293338;
loc_822932E4:
  // li r3,4112
  r3 = uint64_t(int64_t(4112));
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,19
  r11 = uint64_t(int64_t(1245184));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // ori r9,r11,52528
  r9 = r11 | 52528;
  // lwz r11,188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 188);
  // mr r3,r31
  r3 = r31;
  // stw r11,29428(r10)
  REX_STORE_U32(uint32_t(r10) + 29428, uint32_t(r11));
  // lwzx r11,r31,r9
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r9));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8229331c
  if (cr6.eq) goto loc_8229331C;
  // ld r5,168(r31)
  r5 = REX_LOAD_U64(uint32_t(r31) + 168);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x82293324
  goto loc_82293324;
loc_8229331C:
  // lwz r4,192(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 192);
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_82293324:
  // bl 0x82292cc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82292CC0(base, LIFTED_ARGS(0x82293328, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8229333c
  goto loc_8229333C;
loc_8229332C:
  // li r3,4113
  r3 = uint64_t(int64_t(4113));
  // b 0x82293338
  goto loc_82293338;
loc_82293334:
  // li r3,4110
  r3 = uint64_t(int64_t(4110));
loc_82293338:
  // bl 0x8214b330
  // (left out: dead in C++)
  { const lifted::Result o = f8214B330(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8229333C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,160(r31)
  REX_STORE_U32(uint32_t(r31) + 160, uint32_t(r11));
loc_82293344:
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

// sub_82293358: lifted from generated/ by lift.py.
lifted::Result f82293358(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82293358 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r11,31064(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 31064);
  // lwz r10,31044(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 31044);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x8229338c
  if (cr6.lt) goto loc_8229338C;
  // lis r10,-32158
  r10 = uint64_t(int64_t(-2107506688));
  // lwz r10,-29828(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -29828);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822933ac
  if (!cr6.gt) goto loc_822933AC;
loc_8229338C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r6,r11,9308
  r6 = uint64_t(int64_t(int64_t(r11) + 9308));
  // li r4,250
  r4 = uint64_t(int64_t(250));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x822933A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_822933A4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8229344c
  goto loc_8229344C;
loc_822933AC:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-21596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -21596);
  // slw r11,r10,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r11) & 0x3F));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,31060(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 31060);
  // and. r10,r11,r10
  r10 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822933a4
  if (cr0.eq) goto loc_822933A4;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,31068(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 31068);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822933a4
  if (cr0.eq) goto loc_822933A4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27304(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 27304);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82293444
  if (cr0.eq) goto loc_82293444;
  // cmpwi cr6,r11,4109
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4109, so);
  // beq cr6,0x8229343c
  if (cr6.eq) goto loc_8229343C;
  // cmpwi cr6,r11,4110
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4110, so);
  // beq cr6,0x82293434
  if (cr6.eq) goto loc_82293434;
  // cmpwi cr6,r11,4111
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4111, so);
  // beq cr6,0x8229342c
  if (cr6.eq) goto loc_8229342C;
  // cmpwi cr6,r11,4112
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4112, so);
  // beq cr6,0x8229341c
  if (cr6.eq) goto loc_8229341C;
  // cmpwi cr6,r11,4113
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4113, so);
  // bne cr6,0x82293448
  if (!cr6.eq) goto loc_82293448;
  // bl 0x82291760
  // (left out: dead in C++)
  { const lifted::Result o = f82291760(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293448
  goto loc_82293448;
loc_8229341C:
  // bl 0x82291b50
  // (left out: dead in C++)
  { const lifted::Result o = f82291B50(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82293448
  if (!cr0.lt) goto loc_82293448;
  // b 0x822933a4
  goto loc_822933A4;
loc_8229342C:
  // bl 0x82292fb0
  // (left out: dead in C++)
  { const lifted::Result o = f82292FB0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293448
  goto loc_82293448;
loc_82293434:
  // bl 0x822929e8
  // (left out: dead in C++)
  { const lifted::Result o = f822929E8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293448
  goto loc_82293448;
loc_8229343C:
  // bl 0x82293288
  // (left out: dead in C++)
  { const lifted::Result o = f82293288(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x82293448
  goto loc_82293448;
loc_82293444:
  // bl 0x82292938
  // (left out: dead in C++)
  { const lifted::Result o = f82292938(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_82293448:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8229344C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82293CD8: lifted from generated/ by lift.py.
double f82293CD8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82293CD8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f7,f0,f0
  f7 = double(float(f0 * f0));
  // lfs f8,12(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 12);
  f8 = double(temp.f32);
  // lfs f13,4(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f13 = double(temp.f32);
  // lfs f10,4(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 4);
  f10 = double(temp.f32);
  // fmuls f4,f10,f13
  f4 = double(float(f10 * f13));
  // lfs f12,8(r4)
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 8);
  f12 = double(temp.f32);
  // lfs f11,8(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8);
  f11 = double(temp.f32);
  // fmuls f3,f10,f12
  f3 = double(float(f10 * f12));
  // lfs f9,0(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 0);
  f9 = double(temp.f32);
  // fmuls f2,f11,f0
  f2 = double(float(f11 * f0));
  // fmuls f1,f9,f13
  f1 = double(float(f9 * f13));
  // lfs f5,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f5 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmsubs f7,f8,f8,f7
  f7 = double(float(std::fma(f8, f8, -f7)));
  // lfs f6,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f6 = double(temp.f32);
  // fmadds f4,f11,f12,f4
  f4 = double(float(std::fma(f11, f12, f4)));
  // fmuls f8,f8,f6
  f8 = double(float(f8 * f6));
  // fmsubs f2,f9,f12,f2
  f2 = double(float(std::fma(f9, f12, -f2)));
  // fmsubs f3,f11,f13,f3
  f3 = double(float(std::fma(f11, f13, -f3)));
  // fmsubs f1,f10,f0,f1
  f1 = double(float(std::fma(f10, f0, -f1)));
  // fnmsubs f7,f13,f13,f7
  f7 = double(float(-std::fma(f13, f13, -f7)));
  // fmadds f4,f9,f0,f4
  f4 = double(float(std::fma(f9, f0, f4)));
  // fnmsubs f7,f12,f12,f7
  f7 = double(float(-std::fma(f12, f12, -f7)));
  // fmuls f6,f4,f6
  f6 = double(float(f4 * f6));
  // fmuls f9,f9,f7
  f9 = double(float(f9 * f7));
  // fmuls f10,f10,f7
  f10 = double(float(f10 * f7));
  // fmuls f11,f11,f7
  f11 = double(float(f11 * f7));
  // fmadds f9,f3,f8,f9
  f9 = double(float(std::fma(f3, f8, f9)));
  // fmadds f10,f2,f8,f10
  f10 = double(float(std::fma(f2, f8, f10)));
  // fmadds f11,f1,f8,f11
  f11 = double(float(std::fma(f1, f8, f11)));
  // fmadds f0,f0,f6,f9
  f0 = double(float(std::fma(f0, f6, f9)));
  // stfs f0,0(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fmadds f0,f13,f6,f10
  f0 = double(float(std::fma(f13, f6, f10)));
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fmadds f0,f12,f6,f11
  f0 = double(float(std::fma(f12, f6, f11)));
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // stfs f5,12(r3)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // blr
  return f1;
}

// sub_82293F68: lifted from generated/ by lift.py.
lifted::Result f82293F68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82293F68 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // mr r4,r5
  r4 = r5;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,32(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 32);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82294430(base, LIFTED_ARGS(0x82293F94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82293cd8
  // (left out: dead in C++)
  f1 = f82293CD8(base, r3, r4, r5);
  // mr r4,r3
  r4 = r3;
  // addi r5,r31,16
  r5 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r3,r30
  r3 = r30;
  // bl 0x822942a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822942A0(base, LIFTED_ARGS(0x82293FB4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
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

// sub_82293FD0: lifted from generated/ by lift.py.
lifted::Result f82293FD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82293FD0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r4,r5
  r4 = r5;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,32(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 32);
  f1 = double(temp.f32);
  // bl 0x82294430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82294430(base, LIFTED_ARGS(0x82293FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x82293cd8
  // (left out: dead in C++)
  f1 = f82293CD8(base, r3, r4, r5);
  // mr r3,r31
  r3 = r31;
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

// sub_822945F8: lifted from generated/ by lift.py.
double f822945F8(uint8_t* base, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_822945F8 (lifted)");
  double f0 = 0.0;
  // fsubs f0,f2,f3
  lifted::ScalarFloatMode();
  f0 = double(float(f2 - f3));
  // fnmsubs f1,f0,f1,f2
  f1 = double(float(-std::fma(f0, f1, -f2)));
  // blr
  return f1;
}

// sub_822947A0: lifted from generated/ by lift.py.
lifted::Result f822947A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822947A0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mullw r10,r11,r4
  r10 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822947e0
  if (cr6.eq) goto loc_822947E0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x822947e0
  if (cr6.eq) goto loc_822947E0;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x822947e0
  if (cr6.lt) goto loc_822947E0;
  // cmplw cr6,r10,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r4), so);
  // blt cr6,0x822947e0
  if (cr6.lt) goto loc_822947E0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bgelr cr6
  if (!cr6.lt) return {r3, f1};
  // mr r3,r10
  r3 = r10;
  // b 0x825a9a90
  { const lifted::Result o = lifted::entry::e825A9A90(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_822947E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return {r3, f1};
}

// sub_822947E8: lifted from generated/ by lift.py.
lifted::Result f822947E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822947E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825a9b58
  { const lifted::Result o = lifted::entry::e825A9B58(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82294C88: lifted from generated/ by lift.py.
lifted::Result f82294C88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82294C88 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // b 0x82294ccc
  goto loc_82294CCC;
loc_82294CA8:
  // mr r3,r31
  r3 = r31;
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x82294ccc
  if (cr6.eq) goto loc_82294CCC;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82294CCC, r31)); r3 = o.r3; f1 = o.f1; }
loc_82294CCC:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x82294ca8
  if (!cr6.eq) goto loc_82294CA8;
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

// sub_82294CF0: lifted from generated/ by lift.py.
lifted::Result f82294CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82294CF0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // bl 0x82294c88
  // (left out: dead in C++)
  { const lifted::Result o = f82294C88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
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

// sub_82294D38: lifted from generated/ by lift.py.
lifted::Result f82294D38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82294D38 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,10440
  r11 = uint64_t(int64_t(int64_t(r11) + 10440));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x82294cf0
  { const lifted::Result o = f82294CF0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82294D48: lifted from generated/ by lift.py.
lifted::Result f82294D48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82294D48 (lifted)");
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,10440
  r11 = uint64_t(int64_t(int64_t(r11) + 10440));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82294cf0
  // (left out: dead in C++)
  { const lifted::Result o = f82294CF0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82294d84
  if (cr0.eq) goto loc_82294D84;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82294D84, r31)); r3 = o.r3; f1 = o.f1; }
loc_82294D84:
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

// sub_82294F78: lifted from generated/ by lift.py.
lifted::Result f82294F78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_82294F78 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // mr r11,r3
  r11 = r3;
  // mr r10,r4
  r10 = r4;
  // mr r9,r5
  r9 = r5;
  // mr r8,r6
  r8 = r6;
  // mr r31,r7
  r31 = r7;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // li r7,32
  r7 = uint64_t(int64_t(32));
  // stw r9,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r9));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r8,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r8));
  // mr r5,r31
  r5 = r31;
  // ori r4,r4,7
  r4 = r4 | 7;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82294FD0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82294fe0
  if (!cr0.lt) goto loc_82294FE0;
  // li r3,1627
  r3 = uint64_t(int64_t(1627));
  // b 0x82295000
  goto loc_82295000;
loc_82294FE0:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x82294ffc
  if (!cr6.eq) goto loc_82294FFC;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82294FEC, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r3,r11,1627
  r3 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // b 0x82295000
  goto loc_82295000;
loc_82294FFC:
  // li r3,997
  r3 = uint64_t(int64_t(997));
loc_82295000:
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

// sub_82295018: lifted from generated/ by lift.py.
lifted::Result f82295018(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82295018 (lifted)");
  uint64_t r7 = 0;
  double f1 = 0.0;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // b 0x82294f78
  { const lifted::Result o = f82294F78(base, r1, r13, r3, r4, r5, r6, r7); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822950C0: lifted from generated/ by lift.py.
lifted::Result f822950C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822950C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r28,r10
  r28 = r10;
  // mulli r10,r31,52
  r10 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(52))));
  // mr r11,r6
  r11 = r6;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // mr r27,r3
  r27 = r3;
  // mullw r10,r10,r11
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // mr r26,r5
  r26 = r5;
  // mr r30,r7
  r30 = r7;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // beq cr6,0x82295120
  if (cr6.eq) goto loc_82295120;
  // addi r6,r30,4
  r6 = uint64_t(int64_t(int64_t(r30) + 4));
  // mr r7,r11
  r7 = r11;
loc_82295104:
  // lwz r5,0(r6)
  r5 = REX_LOAD_U32(uint32_t(r6) + 0);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r6,r6,136
  r6 = uint64_t(int64_t(int64_t(r6) + 136));
  // mullw r5,r5,r31
  r5 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r31))));
  // mulli r5,r5,28
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(28))));
  // add r10,r5,r10
  r10 = r5 + r10;
  // bne 0x82295104
  if (!cr0.eq) goto loc_82295104;
loc_82295120:
  // lwz r29,0(r8)
  r29 = REX_LOAD_U32(uint32_t(r8) + 0);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // ble cr6,0x82295138
  if (!cr6.gt) goto loc_82295138;
  // li r3,122
  r3 = uint64_t(int64_t(122));
  // stw r10,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r10));
  // b 0x822951a0
  goto loc_822951A0;
loc_82295138:
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // stw r27,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r27));
  // li r7,28
  r7 = uint64_t(int64_t(28));
  // stw r31,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r31));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r26,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r26));
  // mr r5,r28
  r5 = r28;
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // ori r4,r4,33
  r4 = r4 | 33;
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r29,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r29));
  // stw r9,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r9));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82295170, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82295180
  if (!cr0.lt) goto loc_82295180;
  // li r3,1627
  r3 = uint64_t(int64_t(1627));
  // b 0x822951a0
  goto loc_822951A0;
loc_82295180:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8229519c
  if (!cr6.eq) goto loc_8229519C;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x8229518C, r31)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r3,r11,1627
  r3 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // b 0x822951a0
  goto loc_822951A0;
loc_8229519C:
  // li r3,997
  r3 = uint64_t(int64_t(997));
loc_822951A0:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822951A8: lifted from generated/ by lift.py.
lifted::Result f822951A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822951A8 (lifted)");
  uint64_t r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // mr r26,r8
  r26 = r8;
  // mr r25,r9
  r25 = r9;
  // bl 0x82296ee0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296EE0(base, LIFTED_ARGS(0x822951D4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82295200
  if (!cr0.eq) goto loc_82295200;
  // mr r10,r25
  r10 = r25;
  // mr r9,r26
  r9 = r26;
  // mr r8,r27
  r8 = r27;
  // mr r7,r28
  r7 = r28;
  // mr r6,r29
  r6 = r29;
  // clrldi r5,r30,32
  r5 = r30 & 0xFFFFFFFF;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x825db514
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamUserCreateStatsEnumerator, LIFTED_ARGS(0x82295200, r31)); r3 = o.r3; f1 = o.f1; }
loc_82295200:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82295208: lifted from generated/ by lift.py.
lifted::Result f82295208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82295208 (lifted)");
  uint64_t r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r28,r6
  r28 = r6;
  // mr r27,r7
  r27 = r7;
  // mr r26,r8
  r26 = r8;
  // mr r25,r9
  r25 = r9;
  // bl 0x82296ee0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296EE0(base, LIFTED_ARGS(0x82295234, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82295260
  if (!cr0.eq) goto loc_82295260;
  // mr r10,r25
  r10 = r25;
  // mr r9,r26
  r9 = r26;
  // mr r8,r27
  r8 = r27;
  // mr r7,r28
  r7 = r28;
  // mr r6,r29
  r6 = r29;
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825db514
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamUserCreateStatsEnumerator, LIFTED_ARGS(0x82295260, r31)); r3 = o.r3; f1 = o.f1; }
loc_82295260:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82295310: lifted from generated/ by lift.py.
lifted::Result f82295310(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82295310 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // bl 0x825db5a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamShowDirtyDiscErrorUI, LIFTED_ARGS(0x82295320, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822967e0
  // (left out: dead in C++)
  { const lifted::Result o = f822967E0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8229532C); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82295500: lifted from generated/ by lift.py.
lifted::Result f82295500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82295500 (lifted)");
  uint64_t r4 = 0;
  double f1 = 0.0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x82296f48
  { const lifted::Result o = f82296F48(base, r1, r13, r3, r4, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82295700: lifted from generated/ by lift.py.
uint64_t f82295700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82295700 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,996
  r3 = uint64_t(int64_t(996));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82295714
  if (!cr6.eq) goto loc_82295714;
  // b 0x82295f18
  { const lifted::Result o = lifted::entry::e82295F18(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return r3;
loc_82295714:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 997, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // lwz r3,24(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 24);
  // blr
  return r3;
}

// sub_82295C00: lifted from generated/ by lift.py.
lifted::Result f82295C00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82295C00 (lifted)");
  uint64_t r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db5f4
  { const lifted::Result o = lifted::Import(base, __imp__XamContentGetCreator, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82295E78: lifted from generated/ by lift.py.
lifted::Result f82295E78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82295E78 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r30,r5
  r30 = r5;
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq cr6,0x82295eb4
  if (cr6.eq) goto loc_82295EB4;
  // mr r5,r6
  r5 = r6;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x822970a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822970A8(base, LIFTED_ARGS(0x82295EAC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // b 0x82295eb8
  goto loc_82295EB8;
loc_82295EB4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_82295EB8:
  // cntlzw r11,r31
  r11 = uint32_t(r31) == 0 ? 32 : __builtin_clz(uint32_t(r31));
  // clrlwi r6,r30,24
  r6 = uint32_t(r30) & 0xFF;
  // rlwinm r5,r11,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825db774
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtCreateEvent, LIFTED_ARGS(0x82295ECC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82295ef4
  if (cr0.lt) goto loc_82295EF4;
  // lis r11,16384
  r11 = uint64_t(int64_t(1073741824));
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // li r3,183
  r3 = uint64_t(int64_t(183));
  // beq cr6,0x82295ee8
  if (cr6.eq) goto loc_82295EE8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82295EE8:
  // bl 0x82296e70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296E70(base, LIFTED_ARGS(0x82295EEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // b 0x82295efc
  goto loc_82295EFC;
loc_82295EF4:
  // bl 0x82296e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296E88(base, LIFTED_ARGS(0x82295EF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82295EFC:
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

// sub_822967E0: lifted from generated/ by lift.py.
lifted::Result f822967E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822967E0 (lifted)");
  uint64_t r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82296868
  if (cr6.eq) goto loc_82296868;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r11,10476
  r10 = uint64_t(int64_t(int64_t(r11) + 10476));
  // mr r11,r3
  r11 = r3;
loc_822967F4:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x82296818
  if (cr0.eq) goto loc_82296818;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x822967f4
  if (cr6.eq) goto loc_822967F4;
loc_82296818:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x82296828
  if (!cr0.eq) goto loc_82296828;
  // ori r4,r4,2
  r4 = r4 | 2;
  // b 0x82296864
  goto loc_82296864;
loc_82296828:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r11,10456
  r10 = uint64_t(int64_t(int64_t(r11) + 10456));
  // mr r11,r3
  r11 = r3;
loc_82296834:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r8,0(r10)
  r8 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // subf r8,r8,r9
  r8 = r9 - r8;
  // beq 0x82296858
  if (cr0.eq) goto loc_82296858;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x82296834
  if (cr6.eq) goto loc_82296834;
loc_82296858:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x82296868
  if (!cr0.eq) goto loc_82296868;
  // ori r4,r4,256
  r4 = r4 | 256;
loc_82296864:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82296868:
  // b 0x825db634
  { const lifted::Result o = lifted::Import(base, __imp__XamLoaderLaunchTitle, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82296F48: lifted from generated/ by lift.py.
lifted::Result f82296F48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82296F48 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r4
  r29 = r4;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x82296f68
  if (!cr6.eq) goto loc_82296F68;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x82296f78
  goto loc_82296F78;
loc_82296F68:
  // clrldi r10,r3,32
  r10 = r3 & 0xFFFFFFFF;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mulli r10,r10,-10000
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(-10000))));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
loc_82296F78:
  // mr r30,r11
  r30 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82296f94
  if (!cr6.eq) goto loc_82296F94;
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lis r11,-32768
  r11 = uint64_t(int64_t(-2147483648));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
loc_82296F94:
  // clrlwi r31,r29,24
  r31 = uint32_t(r29) & 0xFF;
loc_82296F98:
  // mr r5,r30
  r5 = r30;
  // mr r4,r29
  r4 = r29;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x825db874
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeDelayExecutionThread, LIFTED_ARGS(0x82296FA8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82296fb8
  if (cr6.eq) goto loc_82296FB8;
  // cmpwi cr6,r3,257
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 257, so);
  // beq cr6,0x82296f98
  if (cr6.eq) goto loc_82296F98;
loc_82296FB8:
  // cmpwi cr6,r3,192
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 192, so);
  // li r3,192
  r3 = uint64_t(int64_t(192));
  // beq cr6,0x82296fc8
  if (cr6.eq) goto loc_82296FC8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82296FC8:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822970A0: lifted from generated/ by lift.py.
lifted::Result f822970A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822970A0 (lifted)");
  uint64_t r5 = 0;
  double f1 = 0.0;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // b 0x82297d58
  { const lifted::Result o = f82297D58(base, r1, r13, r3, r4, r5, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82297540: lifted from generated/ by lift.py.
uint64_t f82297540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82297540 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r28,r3
  r28 = r3;
  // mr r31,r5
  r31 = r5;
  // mr r29,r6
  r29 = r6;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // bne cr6,0x8229756c
  if (!cr6.eq) goto loc_8229756C;
  // bl 0x825a9958
  // (left out: dead in C++)
  r3 = f825A9958(base, r3);
  // addi r4,r3,1
  r4 = uint64_t(int64_t(int64_t(r3) + 1));
loc_8229756C:
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // ble cr6,0x8229764c
  if (!cr6.gt) goto loc_8229764C;
  // li r9,1
  r9 = uint64_t(int64_t(1));
loc_82297578:
  // lhz r10,0(r28)
  r10 = REX_LOAD_U16(uint32_t(r28) + 0);
  // mr r11,r10
  r11 = r10;
  // cmplwi cr6,r11,127
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 127, so);
  // bgt cr6,0x822975a4
  if (cr6.gt) goto loc_822975A4;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpw cr6,r30,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r29), so);
  // bgt cr6,0x82297640
  if (cr6.gt) goto loc_82297640;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82297640
  if (cr6.eq) goto loc_82297640;
  // stb r10,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r10));
  // b 0x8229763c
  goto loc_8229763C;
loc_822975A4:
  // cmplwi cr6,r11,2047
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2047, so);
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // bgt cr6,0x822975d0
  if (cr6.gt) goto loc_822975D0;
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // bgt cr6,0x8229761c
  if (cr6.gt) goto loc_8229761C;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8229761c
  if (cr6.eq) goto loc_8229761C;
  // rlwinm r10,r10,26,22,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0x3FF;
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // rlwimi r10,r8,6,0,26
  r10 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 6) & 0xFFFFFFE0) | (r10 & 0xFFFFFFFF0000001F);
  // b 0x82297614
  goto loc_82297614;
loc_822975D0:
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // bgt cr6,0x822975f4
  if (cr6.gt) goto loc_822975F4;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x822975f4
  if (cr6.eq) goto loc_822975F4;
  // rlwinm r10,r10,20,28,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 20) & 0xF;
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // rlwimi r10,r8,5,0,27
  r10 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 5) & 0xFFFFFFF0) | (r10 & 0xFFFFFFFF0000000F);
  // stb r10,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r10));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_822975F4:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // bgt cr6,0x8229761c
  if (cr6.gt) goto loc_8229761C;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8229761c
  if (cr6.eq) goto loc_8229761C;
  // lhz r10,0(r28)
  r10 = REX_LOAD_U16(uint32_t(r28) + 0);
  // rlwinm r10,r10,26,6,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0x3FFFFFF;
  // rlwimi r10,r9,7,0,25
  r10 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFFC0) | (r10 & 0xFFFFFFFF0000003F);
loc_82297614:
  // stb r10,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r10));
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_8229761C:
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpw cr6,r30,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r29), so);
  // bgt cr6,0x82297640
  if (cr6.gt) goto loc_82297640;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82297640
  if (cr6.eq) goto loc_82297640;
  // lhz r11,0(r28)
  r11 = REX_LOAD_U16(uint32_t(r28) + 0);
  // rlwimi r11,r9,7,0,25
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 7) & 0xFFFFFFC0) | (r11 & 0xFFFFFFFF0000003F);
  // stb r11,0(r31)
  REX_STORE_U8(uint32_t(r31) + 0, uint8_t(r11));
loc_8229763C:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
loc_82297640:
  // addic. r4,r4,-1
  ca = uint32_t(r4) > 0;
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // addi r28,r28,2
  r28 = uint64_t(int64_t(int64_t(r28) + 2));
  // bne 0x82297578
  if (!cr0.eq) goto loc_82297578;
loc_8229764C:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x8229766c
  if (cr6.eq) goto loc_8229766C;
  // cmpw cr6,r29,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r30), so);
  // bge cr6,0x8229766c
  if (!cr6.lt) goto loc_8229766C;
  // li r3,122
  r3 = uint64_t(int64_t(122));
  // bl 0x82296fd0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296FD0(base, LIFTED_ARGS(0x82297664, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82297670
  goto loc_82297670;
loc_8229766C:
  // mr r3,r30
  r3 = r30;
loc_82297670:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_822979E0: lifted from generated/ by lift.py.
lifted::Result f822979E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822979E0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lwz r10,9116(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 9116);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82297a10
  if (cr6.eq) goto loc_82297A10;
  // rotlwi r11,r10,0
  r11 = __builtin_rotateleft32(uint32_t(r10), 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82297A10, r31)); r3 = o.r3; f1 = o.f1; }
loc_82297A10:
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lis r10,-32160
  r10 = uint64_t(int64_t(-2107637760));
  // addi r11,r11,644
  r11 = uint64_t(int64_t(int64_t(r11) + 644));
  // addi r30,r10,656
  r30 = uint64_t(int64_t(int64_t(r10) + 656));
  // mr r31,r11
  r31 = r11;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x82297a60
  if (!cr6.lt) goto loc_82297A60;
loc_82297A30:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x82297aa8
  if (!cr6.eq) goto loc_82297AA8;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82297a4c
  if (cr0.eq) goto loc_82297A4C;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82297A4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82297A4C:
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // blt cr6,0x82297a30
  if (cr6.lt) goto loc_82297A30;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x82297aa8
  if (!cr6.eq) goto loc_82297AA8;
loc_82297A60:
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lis r10,-32160
  r10 = uint64_t(int64_t(-2107637760));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r30,r10,640
  r30 = uint64_t(int64_t(int64_t(r10) + 640));
  // mr r31,r11
  r31 = r11;
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bge cr6,0x82297aa4
  if (!cr6.lt) goto loc_82297AA4;
loc_82297A7C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82297a98
  if (cr0.eq) goto loc_82297A98;
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82297a98
  if (cr6.eq) goto loc_82297A98;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82297A98, r31)); r3 = o.r3; f1 = o.f1; }
loc_82297A98:
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // blt cr6,0x82297a7c
  if (cr6.lt) goto loc_82297A7C;
loc_82297AA4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82297AA8:
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

// sub_82297D38: lifted from generated/ by lift.py.
uint64_t f82297D38(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82297D38 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // bne cr6,0x82297d48
  if (!cr6.eq) goto loc_82297D48;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82297D48:
  // clrldi r11,r4,32
  r11 = r4 & 0xFFFFFFFF;
  // mulli r11,r11,-10000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(-10000))));
  // std r11,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r11);
  // blr
  return r3;
}

// sub_82297D58: lifted from generated/ by lift.py.
lifted::Result f82297D58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82297D58 (lifted)");
  uint64_t r6 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // bl 0x82297d38
  // (left out: dead in C++)
  r3 = f82297D38(base, r3, r4);
  // mr r30,r3
  r30 = r3;
  // clrlwi r29,r28,24
  r29 = uint32_t(r28) & 0xFF;
loc_82297D7C:
  // mr r6,r30
  r6 = r30;
  // mr r5,r29
  r5 = r29;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x825db734
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtWaitForSingleObjectEx, LIFTED_ARGS(0x82297D90, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82297dac
  if (cr0.lt) goto loc_82297DAC;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x82297db4
  if (cr6.eq) goto loc_82297DB4;
  // cmpwi cr6,r3,257
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 257, so);
  // beq cr6,0x82297d7c
  if (cr6.eq) goto loc_82297D7C;
  // b 0x82297db4
  goto loc_82297DB4;
loc_82297DAC:
  // bl 0x82296e88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82296E88(base, LIFTED_ARGS(0x82297DB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
loc_82297DB4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82297FD8: lifted from generated/ by lift.py.
lifted::Result f82297FD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82297FD8 (lifted)");
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // addi r11,r10,76
  r11 = uint64_t(int64_t(int64_t(r10) + 76));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x82298174
  if (!cr6.eq) goto loc_82298174;
  // lwz r11,72(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 72);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // beq 0x82298080
  if (cr0.eq) goto loc_82298080;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // beq cr6,0x82298080
  if (cr6.eq) goto loc_82298080;
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r3,r1,92
  r3 = uint64_t(int64_t(int64_t(r1) + 92));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x82298058, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x822980b0
  if (cr0.lt) goto loc_822980B0;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r9,84(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // add r10,r10,r9
  r10 = r10 + r9;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,92(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 92);
  // b 0x82298140
  goto loc_82298140;
loc_82298080:
  // lis r11,16
  r11 = uint64_t(int64_t(1048576));
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,8192
  r5 = r5 | 8192;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x822980A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x822980b8
  if (!cr0.lt) goto loc_822980B8;
loc_822980B0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82298180
  goto loc_82298180;
loc_822980B8:
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x822980DC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82298100
  if (!cr0.lt) goto loc_82298100;
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825db6c4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtFreeVirtualMemory, LIFTED_ARGS(0x822980FC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822980b0
  goto loc_822980B0;
loc_82298100:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // stw r11,72(r10)
  REX_STORE_U32(uint32_t(r10) + 72, uint32_t(r11));
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r11,r10,16
  r11 = uint64_t(int64_t(int64_t(r10) + 16));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
loc_82298140:
  // lwz r9,8(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 8);
  // lwz r8,24(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 24);
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r10,r8,76
  r10 = uint64_t(int64_t(int64_t(r8) + 76));
  // b 0x82298160
  goto loc_82298160;
loc_82298154:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // mr r10,r11
  r10 = r11;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
loc_82298160:
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x82298154
  if (cr6.lt) goto loc_82298154;
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // addi r11,r11,76
  r11 = uint64_t(int64_t(int64_t(r11) + 76));
loc_82298174:
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_82298180:
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

// sub_82298198: lifted from generated/ by lift.py.
lifted::Result f82298198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_82298198 (lifted)");
  uint64_t r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r4
  r29 = r4;
  // addi r28,r31,56
  r28 = uint64_t(int64_t(int64_t(r31) + 56));
  // mr r30,r5
  r30 = r5;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8229823c
  if (cr0.eq) goto loc_8229823C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_822981C4:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r10,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r29), so);
  // bgt cr6,0x82298284
  if (cr6.gt) goto loc_82298284;
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // add r7,r9,r10
  r7 = r9 + r10;
  // cmplw cr6,r7,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r29), so);
  // bne cr6,0x8229822c
  if (!cr6.eq) goto loc_8229822C;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r29,r10
  r29 = r10;
  // add r30,r9,r30
  r30 = r9 + r30;
  // stw r7,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r7));
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // lwz r10,76(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 76);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // stw r11,76(r10)
  REX_STORE_U32(uint32_t(r10) + 76, uint32_t(r11));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // lwz r10,28(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 28);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // ble cr6,0x82298230
  if (!cr6.gt) goto loc_82298230;
  // stw r30,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r30));
  // b 0x82298230
  goto loc_82298230;
loc_8229822C:
  // mr r28,r11
  r28 = r11;
loc_82298230:
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x822981c4
  if (!cr0.eq) goto loc_822981C4;
loc_8229823C:
  // mr r3,r31
  r3 = r31;
  // bl 0x82297fd8
  // (left out: dead in C++)
  { const lifted::Result o = f82297FD8(base, r1, r13, r3, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8229827c
  if (cr0.eq) goto loc_8229827C;
  // stw r29,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r29));
  // stw r30,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r30));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // stw r3,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r3));
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // lwz r10,28(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 28);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r30,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r10), so);
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // blt cr6,0x8229827c
  if (cr6.lt) goto loc_8229827C;
  // stw r30,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r30));
loc_8229827C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_82298284:
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r10,r29,r30
  r10 = r29 + r30;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8229823c
  if (!cr6.eq) goto loc_8229823C;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // add r10,r30,r10
  r10 = r30 + r10;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // ble cr6,0x8229827c
  if (!cr6.gt) goto loc_8229827C;
  // stw r10,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r10));
  // b 0x8229827c
  goto loc_8229827C;
}

// sub_82298950: lifted from generated/ by lift.py.
void f82298950(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82298950 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lbz r30,4(r4)
  r30 = REX_LOAD_U8(uint32_t(r4) + 4);
  // lwz r11,48(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 48);
  // mr r10,r30
  r10 = r30;
  // lhz r7,2(r4)
  r7 = REX_LOAD_U16(uint32_t(r4) + 2);
  // add r11,r11,r5
  r11 = r11 + r5;
  // lbz r29,5(r4)
  r29 = REX_LOAD_U8(uint32_t(r4) + 5);
  // addi r10,r10,24
  r10 = uint64_t(int64_t(int64_t(r10) + 24));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r31,r10,r3
  r31 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r3));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // b 0x82298a6c
  goto loc_82298A6C;
loc_82298984:
  // cmplwi cr6,r5,61440
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 61440, so);
  // ble cr6,0x822989a8
  if (!cr6.gt) goto loc_822989A8;
  // li r6,-4096
  r6 = uint64_t(int64_t(-4096));
  // cmplwi cr6,r5,61441
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 61441, so);
  // bne cr6,0x8229899c
  if (!cr6.eq) goto loc_8229899C;
  // li r6,-4112
  r6 = uint64_t(int64_t(-4112));
loc_8229899C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stb r11,5(r4)
  REX_STORE_U8(uint32_t(r4) + 5, uint8_t(r11));
  // b 0x822989b0
  goto loc_822989B0;
loc_822989A8:
  // clrlwi r6,r5,16
  r6 = uint32_t(r5) & 0xFFFF;
  // stb r29,5(r4)
  REX_STORE_U8(uint32_t(r4) + 5, uint8_t(r29));
loc_822989B0:
  // lbz r11,5(r4)
  r11 = REX_LOAD_U8(uint32_t(r4) + 5);
  // clrlwi r9,r6,16
  r9 = uint32_t(r6) & 0xFFFF;
  // sth r7,2(r4)
  REX_STORE_U16(uint32_t(r4) + 2, uint16_t(r7));
  // rlwinm r11,r11,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF8;
  // stb r30,4(r4)
  REX_STORE_U8(uint32_t(r4) + 4, uint8_t(r30));
  // sth r6,0(r4)
  REX_STORE_U16(uint32_t(r4) + 0, uint16_t(r6));
  // cmplwi cr6,r9,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 128, so);
  // stb r11,5(r4)
  REX_STORE_U8(uint32_t(r4) + 5, uint8_t(r11));
  // bge cr6,0x82298a14
  if (!cr6.lt) goto loc_82298A14;
  // addi r11,r9,48
  r11 = uint64_t(int64_t(int64_t(r9) + 48));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r10,r11,r3
  r10 = r11 + r3;
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82298a38
  if (!cr6.eq) goto loc_82298A38;
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // clrlwi r8,r9,27
  r8 = uint32_t(r9) & 0x1F;
  // rlwinm r11,r9,27,5,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x7FFFFFF;
  // addi r11,r11,88
  r11 = uint64_t(int64_t(int64_t(r11) + 88));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // slw r8,r7,r8
  r8 = uint8_t(r8) & 0x20 ? 0 : (uint32_t(r7) << (uint8_t(r8) & 0x3F));
  // lwzx r7,r11,r3
  r7 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  // or r8,r8,r7
  r8 = r8 | r7;
  // stwx r8,r11,r3
  REX_STORE_U32(uint32_t(r11) + uint32_t(r3), uint32_t(r8));
  // b 0x82298a38
  goto loc_82298A38;
loc_82298A14:
  // addi r11,r3,384
  r11 = uint64_t(int64_t(int64_t(r3) + 384));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // b 0x82298a30
  goto loc_82298A30;
loc_82298A20:
  // lhz r8,-8(r10)
  r8 = REX_LOAD_U16(uint32_t(r10) + -8);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // ble cr6,0x82298a38
  if (!cr6.gt) goto loc_82298A38;
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
loc_82298A30:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82298a20
  if (!cr6.eq) goto loc_82298A20;
loc_82298A38:
  // rlwinm r8,r9,4,0,27
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 4) & 0xFFFFFFF0;
  // subf r5,r9,r5
  r5 = r5 - r9;
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // addi r11,r4,8
  r11 = uint64_t(int64_t(int64_t(r4) + 8));
  // mr r7,r6
  r7 = r6;
  // stw r9,12(r4)
  REX_STORE_U32(uint32_t(r4) + 12, uint32_t(r9));
  // add r4,r8,r4
  r4 = r8 + r4;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // bge cr6,0x82298a80
  if (!cr6.lt) goto loc_82298A80;
loc_82298A6C:
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x82298984
  if (!cr6.eq) goto loc_82298984;
  // rlwinm. r11,r29,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82298a80
  if (!cr0.eq) goto loc_82298A80;
  // sth r7,2(r4)
  REX_STORE_U16(uint32_t(r4) + 2, uint16_t(r7));
loc_82298A80:
  // b 0x825a6edc
  // (left out: dead in C++)
  return;
}

// sub_822990A0: lifted from generated/ by lift.py.
lifted::Result f822990A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822990A0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r7
  r29 = r7;
  // stw r8,236(r1)
  REX_STORE_U32(uint32_t(r1) + 236, uint32_t(r8));
  // mr r27,r9
  r27 = r9;
  // mr r31,r4
  r31 = r4;
  // subf r11,r29,r27
  r11 = r27 - r29;
  // mr r28,r3
  r28 = r3;
  // addi r10,r31,87
  r10 = uint64_t(int64_t(int64_t(r31) + 87));
  // srawi r11,r11,16
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 16));
  // mr r25,r5
  r25 = r5;
  // mr r23,r6
  r23 = r6;
  // rlwinm r30,r10,0,0,27
  r30 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFF0;
  // addze r26,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r26 = uint64_t(int64_t(temp.s64));
  // cmplw cr6,r28,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r29), so);
  // bne cr6,0x822990ec
  if (!cr6.eq) goto loc_822990EC;
  // lhz r22,0(r28)
  r22 = REX_LOAD_U16(uint32_t(r28) + 0);
  // b 0x822990f0
  goto loc_822990F0;
loc_822990EC:
  // li r22,0
  r22 = uint64_t(int64_t(0));
loc_822990F0:
  // subf r10,r31,r30
  r10 = r30 - r31;
  // addi r11,r30,16
  r11 = uint64_t(int64_t(int64_t(r30) + 16));
  // srawi r10,r10,4
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xF) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // clrlwi r24,r10,16
  r24 = uint32_t(r10) & 0xFFFF;
  // blt cr6,0x82299158
  if (cr6.lt) goto loc_82299158;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // blt cr6,0x82299118
  if (cr6.lt) goto loc_82299118;
loc_82299110:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82299200
  goto loc_82299200;
loc_82299118:
  // subf r11,r8,r30
  r11 = r30 - r8;
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,236
  r3 = uint64_t(int64_t(int64_t(r1) + 236));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x82299140, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82299110
  if (cr0.lt) goto loc_82299110;
  // lwz r10,236(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 236);
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // add r8,r11,r10
  r8 = r11 + r10;
  // stw r8,236(r1)
  REX_STORE_U32(uint32_t(r1) + 236, uint32_t(r8));
loc_82299158:
  // subf r11,r8,r27
  r11 = r27 - r8;
  // sth r22,2(r31)
  REX_STORE_U16(uint32_t(r31) + 2, uint16_t(r22));
  // rlwinm r10,r26,16,0,15
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 16) & 0xFFFF0000;
  // sth r24,0(r31)
  REX_STORE_U16(uint32_t(r31) + 0, uint16_t(r24));
  // srawi r11,r11,16
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 16));
  // stb r25,4(r31)
  REX_STORE_U8(uint32_t(r31) + 4, uint8_t(r25));
  // lis r7,-18
  r7 = uint64_t(int64_t(-1179648));
  // stw r23,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r23));
  // addze. r11,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r11 = uint64_t(int64_t(temp.s64));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r28,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r28));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r29,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r29));
  // ori r7,r7,65518
  r7 = r7 | 65518;
  // stw r30,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r30));
  // add r10,r10,r29
  r10 = r10 + r29;
  // stw r26,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r26));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // stb r9,5(r31)
  REX_STORE_U8(uint32_t(r31) + 5, uint8_t(r9));
  // stw r7,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r7));
  // stw r10,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r10));
  // beq 0x822991c0
  if (cr0.eq) goto loc_822991C0;
  // rlwinm r5,r11,16,0,15
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF0000;
  // mr r4,r8
  r4 = r8;
  // mr r3,r31
  r3 = r31;
  // bl 0x82298198
  // (left out: dead in C++)
  { const lifted::Result o = f82298198(base, r1, r13, r3, r4, r5, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r8,236(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 236);
loc_822991C0:
  // clrlwi r11,r25,24
  r11 = uint32_t(r25) & 0xFF;
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // subf r9,r30,r8
  r9 = r8 - r30;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r30
  r4 = r30;
  // srawi r5,r9,4
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0xF) != 0);
  r5 = uint64_t(int64_t(int32_t(r9) >> 4));
  // mr r3,r28
  r3 = r28;
  // stwx r31,r11,r28
  REX_STORE_U32(uint32_t(r11) + uint32_t(r28), uint32_t(r31));
  // lhz r11,0(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 0);
  // stb r10,5(r30)
  REX_STORE_U8(uint32_t(r30) + 5, uint8_t(r10));
  // stw r30,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r30));
  // stb r25,4(r30)
  REX_STORE_U8(uint32_t(r30) + 4, uint8_t(r25));
  // sth r11,2(r30)
  REX_STORE_U16(uint32_t(r30) + 2, uint16_t(r11));
  // bl 0x82298950
  // (left out: dead in C++)
  f82298950(base, r3, r4, r5);
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_82299200:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229BFD0: lifted from generated/ by lift.py.
lifted::Result f8229BFD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229BFD0 (lifted)");
  lifted::Cr cr0;
  uint8_t so = 0;
  // lwz r3,12(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 12);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x8229bfe0
  if (!cr0.eq) goto loc_8229BFE0;
  // blr
  return {r3, f1};
loc_8229BFE0:
  // b 0x8229f068
  { const lifted::Result o = f8229F068(base, r1, r13, r3, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8229D620: lifted from generated/ by lift.py.
lifted::Result f8229D620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229D620 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addic. r3,r11,-1
  ca = uint32_t(r11) > 0;
  r3 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // bne 0x8229d670
  if (!cr0.eq) goto loc_8229D670;
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8229d654
  if (cr0.eq) goto loc_8229D654;
  // bl 0x8229eaf0
  // (left out: dead in C++)
  { const lifted::Result o = f8229EAF0(base, r1, r13, r3, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, 0x8229D654); r3 = o.r3; f1 = o.f1; }
loc_8229D654:
  // lis r4,-20096
  r4 = uint64_t(int64_t(-1317011456));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x8229D660, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,9344
  r4 = uint64_t(int64_t(612368384));
  // mr r3,r31
  r3 = r31;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x8229D66C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8229D670:
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

// sub_8229D688: lifted from generated/ by lift.py.
lifted::Result f8229D688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229D688 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r30,r6
  r30 = r6;
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8229d6b0
  if (cr0.eq) goto loc_8229D6B0;
  // bl 0x8229eaf0
  // (left out: dead in C++)
  { const lifted::Result o = f8229EAF0(base, r1, r13, r3, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, 0x8229D6B0); r3 = o.r3; f1 = o.f1; }
loc_8229D6B0:
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // mr r3,r29
  r3 = r29;
  // mr r4,r31
  r4 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x8229D6C4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r31,480
  r4 = uint64_t(int64_t(int64_t(r31) + 480));
  // mr r3,r31
  r3 = r31;
  // bl 0x822ac470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AC470(base, LIFTED_ARGS(0x8229D6D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8229d6e4
  if (cr6.eq) goto loc_8229D6E4;
  // lis r11,10922
  r11 = uint64_t(int64_t(715784192));
  // ori r11,r11,43690
  r11 = r11 | 43690;
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8229D6E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229D7E0: lifted from generated/ by lift.py.
uint64_t f8229D7E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8229D7E0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,52(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 52);
  // addi r3,r10,1
  r3 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r3,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r3));
  // blr
  return r3;
}

// sub_8229D7F8: lifted from generated/ by lift.py.
lifted::Result f8229D7F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8229D7F8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // lis r4,9344
  r4 = uint64_t(int64_t(612368384));
  // add r3,r3,r31
  r3 = r3 + r31;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822976F8(base, LIFTED_ARGS(0x8229D818, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8229d828
  if (!cr0.eq) goto loc_8229D828;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8229d83c
  goto loc_8229D83C;
loc_8229D828:
  // add r10,r11,r31
  r10 = r11 + r31;
  // addi r9,r31,-1
  r9 = uint64_t(int64_t(int64_t(r31) + -1));
  // addi r10,r10,3
  r10 = uint64_t(int64_t(int64_t(r10) + 3));
  // andc r3,r10,r9
  r3 = r10 & ~r9;
  // stw r11,-4(r3)
  REX_STORE_U32(uint32_t(r3) + -4, uint32_t(r11));
loc_8229D83C:
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

// sub_8229D850: lifted from generated/ by lift.py.
lifted::Result f8229D850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8229D850 (lifted)");
  uint64_t r5 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x8229d7f8
  // (left out: dead in C++)
  { const lifted::Result o = f8229D7F8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x8229d884
  if (cr0.eq) goto loc_8229D884;
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71D0(base, LIFTED_ARGS(0x8229D884, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229D884:
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

// sub_8229D8A0: lifted from generated/ by lift.py.
lifted::Result f8229D8A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8229D8A0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // lwz r29,0(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
loc_8229D8BC:
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229d8bc
  if (!cr6.eq) goto loc_8229D8BC;
  // lbz r11,10809(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 10809);
  // rlwinm. r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8229d968
  if (!cr0.eq) goto loc_8229D968;
  // lwz r10,256(r13)
  r10 = REX_LOAD_U32(uint32_t(r13) + 256);
  // lwz r11,10768(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 10768);
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r30,88(r10)
  r30 = REX_LOAD_U32(uint32_t(r10) + 88);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // beq cr6,0x8229d928
  if (cr6.eq) goto loc_8229D928;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
loc_8229D928:
  // bl 0x825b9a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9A60(base, LIFTED_ARGS(0x8229D92C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,10760(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 10760);
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // bne cr6,0x8229d948
  if (!cr6.eq) goto loc_8229D948;
  // lwz r11,10864(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 10864);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8229d948
  if (cr6.eq) goto loc_8229D948;
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
loc_8229D948:
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // subf r11,r11,r30
  r11 = r30 - r11;
  // cmplwi cr6,r11,5000
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5000, so);
  // bge cr6,0x8229d960
  if (!cr6.lt) goto loc_8229D960;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x8229d96c
  goto loc_8229D96C;
loc_8229D960:
  // mr r3,r29
  r3 = r29;
  // bl 0x822acaa8
  // (left out: dead in C++)
  { const lifted::Result o = f822ACAA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8229D968:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8229D96C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229D978: lifted from generated/ by lift.py.
lifted::Result f8229D978(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229D978 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8229d9b0
  if (!cr6.eq) goto loc_8229D9B0;
  // bl 0x822ad500
  // (left out: dead in C++)
  { const lifted::Result o = f822AD500(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lis r4,9344
  r4 = uint64_t(int64_t(612368384));
  // lwz r3,-4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -4);
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x8229D9A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8229d9b8
  goto loc_8229D9B8;
loc_8229D9B0:
  // addi r3,r11,-1
  r3 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r3,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r3));
loc_8229D9B8:
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

// sub_8229DA20: lifted from generated/ by lift.py.
void f8229DA20(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8229DA20 (lifted)");
  // stw r4,15144(r3)
  REX_STORE_U32(uint32_t(r3) + 15144, uint32_t(r4));
  // blr
  return;
}

// sub_8229DA28: lifted from generated/ by lift.py.
lifted::Result f8229DA28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229DA28 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
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
  // addi r29,r31,12184
  r29 = uint64_t(int64_t(int64_t(r31) + 12184));
loc_8229DA40:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r10,13432(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 13432);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8229da60
  if (cr6.eq) goto loc_8229DA60;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x822a3d30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3D30, LIFTED_ARGS(0x8229DA60, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229DA60:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x8229da40
  if (cr6.lt) goto loc_8229DA40;
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
  // lwz r10,13424(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 13424);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8229da8c
  if (cr6.eq) goto loc_8229DA8C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a4060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A4060(base, LIFTED_ARGS(0x8229DA8C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229DA8C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8958(base, LIFTED_ARGS(0x8229DA98, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a8618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8618(base, LIFTED_ARGS(0x8229DAA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a8960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A8960(base, LIFTED_ARGS(0x8229DAB0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a3908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A3908(base, LIFTED_ARGS(0x8229DABC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // rldicr r29,r11,63,63
  r29 = __builtin_rotateleft64(r11, 63) & 0xFFFFFFFFFFFFFFFF;
loc_8229DAC8:
  // subfic r11,r30,95
  ca = uint32_t(r30) <= 95;
  r11 = static_cast<uint64_t>(95) - r30;
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // mulli r11,r11,21846
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(21846))));
  // rlwinm r11,r11,16,16,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // mr r4,r30
  r4 = r30;
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // mr r3,r31
  r3 = r31;
  // srd r8,r29,r11
  r8 = uint8_t(r11) & 0x40 ? 0 : (r29 >> (uint8_t(r11) & 0x7F));
  // bl 0x822a3758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A3758(base, LIFTED_ARGS(0x8229DAF8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 16, so);
  // blt cr6,0x8229dac8
  if (cr6.lt) goto loc_8229DAC8;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_8229DB08:
  // addi r11,r30,20
  r11 = uint64_t(int64_t(int64_t(r30) + 20));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // srd r6,r29,r11
  r6 = uint8_t(r11) & 0x40 ? 0 : (r29 >> (uint8_t(r11) & 0x7F));
  // bl 0x822a6008
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A6008(base, LIFTED_ARGS(0x8229DB24, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 26, so);
  // blt cr6,0x8229db08
  if (cr6.lt) goto loc_8229DB08;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229DB38: lifted from generated/ by lift.py.
lifted::Result f8229DB38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229DB38 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r27,r8
  r27 = r8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r29,r4
  r29 = r4;
  // li r4,128
  r4 = uint64_t(int64_t(128));
  // li r3,20480
  r3 = uint64_t(int64_t(20480));
  // mr r30,r6
  r30 = r6;
  // mr r28,r7
  r28 = r7;
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
  // bl 0x8229d850
  // (left out: dead in C++)
  { const lifted::Result o = f8229D850(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x8229db7c
  if (!cr0.eq) goto loc_8229DB7C;
loc_8229DB70:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x8229dbe4
  goto loc_8229DBE4;
loc_8229DB7C:
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // bne cr6,0x8229dbb4
  if (!cr6.eq) goto loc_8229DBB4;
  // lbz r11,10808(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,128
  r11 = r11 | 128;
  // stb r11,10808(r31)
  REX_STORE_U8(uint32_t(r31) + 10808, uint8_t(r11));
  // bl 0x8229e098
  // (left out: dead in C++)
  f8229E098(base, r3);
  // mr r3,r31
  r3 = r31;
  // bl 0x822ad4a0
  // (left out: dead in C++)
  { const lifted::Result o = f822AD4A0(base, r1, r13, r3, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_8229DBA0:
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8229dbdc
  if (!cr0.eq) goto loc_8229DBDC;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229d978
  // (left out: dead in C++)
  { const lifted::Result o = f8229D978(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // b 0x8229db70
  goto loc_8229DB70;
loc_8229DBB4:
  // rlwinm. r11,r30,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x100;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8229dbc8
  if (!cr0.eq) goto loc_8229DBC8;
  // rlwinm. r11,r30,0,2,7
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x3F000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8229dbc8
  if (!cr0.eq) goto loc_8229DBC8;
  // oris r30,r30,1024
  r30 = r30 | 67108864;
loc_8229DBC8:
  // mr r4,r28
  r4 = r28;
  // stw r30,20440(r31)
  REX_STORE_U32(uint32_t(r31) + 20440, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // bl 0x822ad298
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AD298(base, LIFTED_ARGS(0x8229DBD8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8229dba0
  goto loc_8229DBA0;
loc_8229DBDC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r31,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r31));
loc_8229DBE4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229DCC0: lifted from generated/ by lift.py.
lifted::Result f8229DCC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8229DCC0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // add r10,r31,r5
  r10 = r31 + r5;
  // lwz r11,13500(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 13500);
  // and r30,r10,r11
  r30 = r10 & r11;
  // lwz r11,10768(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 10768);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // bge cr6,0x8229dd04
  if (!cr6.lt) goto loc_8229DD04;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x8229dd60
  if (!cr6.lt) goto loc_8229DD60;
loc_8229DCF8:
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // ble cr6,0x8229dd0c
  if (!cr6.gt) goto loc_8229DD0C;
  // b 0x8229dd60
  goto loc_8229DD60;
loc_8229DD04:
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x8229dcf8
  if (!cr6.lt) goto loc_8229DCF8;
loc_8229DD0C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r29
  r4 = r29;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8229d6f0
  // (left out: dead in C++)
  f8229D6F0(base, r13, r3, r4, r5);
loc_8229DD1C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8229d8a0
  // (left out: dead in C++)
  { const lifted::Result o = f8229D8A0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8229dd58
  if (cr0.eq) goto loc_8229DD58;
  // lwz r11,10768(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 10768);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // bge cr6,0x8229dd48
  if (!cr6.lt) goto loc_8229DD48;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x8229dd58
  if (!cr6.lt) goto loc_8229DD58;
  // b 0x8229dd50
  goto loc_8229DD50;
loc_8229DD48:
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8229dd1c
  if (cr6.lt) goto loc_8229DD1C;
loc_8229DD50:
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // ble cr6,0x8229dd1c
  if (!cr6.gt) goto loc_8229DD1C;
loc_8229DD58:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8229d720
  // (left out: dead in C++)
  { const lifted::Result o = f8229D720(base, r1, r13, r3, r5, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_8229DD60:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229DD70: lifted from generated/ by lift.py.
lifted::Result f8229DD70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8229DD70 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r29,r5
  r29 = r5;
  // lwz r31,10820(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 10820);
  // lwz r27,13500(r30)
  r27 = REX_LOAD_U32(uint32_t(r30) + 13500);
  // mr r4,r31
  r4 = r31;
  // lwz r26,13496(r30)
  r26 = REX_LOAD_U32(uint32_t(r30) + 13496);
  // bl 0x8229dcc0
  // (left out: dead in C++)
  { const lifted::Result o = f8229DCC0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8229ddc8
  if (cr6.eq) goto loc_8229DDC8;
  // mr r11,r28
  r11 = r28;
loc_8229DDA8:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r31,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r31,1
  r8 = uint64_t(int64_t(int64_t(r31) + 1));
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // and r31,r8,r27
  r31 = r8 & r27;
  // stwx r10,r9,r26
  REX_STORE_U32(uint32_t(r9) + uint32_t(r26), uint32_t(r10));
  // bne 0x8229dda8
  if (!cr0.eq) goto loc_8229DDA8;
loc_8229DDC8:
  // stw r31,10820(r30)
  REX_STORE_U32(uint32_t(r30) + 10820, uint32_t(r31));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229E098: lifted from generated/ by lift.py.
void f8229E098(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8229E098 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  // lwz r11,15176(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 15176);
  // lbz r9,10809(r3)
  r9 = REX_LOAD_U8(uint32_t(r3) + 10809);
  // addi r10,r11,4800
  r10 = uint64_t(int64_t(int64_t(r11) + 4800));
  // ori r9,r9,64
  r9 = r9 | 64;
  // stw r11,40(r3)
  REX_STORE_U32(uint32_t(r3) + 40, uint32_t(r11));
  // addi r11,r10,-160
  r11 = uint64_t(int64_t(int64_t(r10) + -160));
  // stw r10,44(r3)
  REX_STORE_U32(uint32_t(r3) + 44, uint32_t(r10));
  // stb r9,10809(r3)
  REX_STORE_U8(uint32_t(r3) + 10809, uint8_t(r9));
  // stw r11,48(r3)
  REX_STORE_U32(uint32_t(r3) + 48, uint32_t(r11));
  // blr
  return;
}

// sub_8229EAF0: lifted from generated/ by lift.py.
lifted::Result f8229EAF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8229EAF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r4,r3
  r4 = r3;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // lwz r11,1968(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1968);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // b 0x8229e908
  { const lifted::Result o = lifted::Call(base, 0x8229E908, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8229F068: lifted from generated/ by lift.py.
lifted::Result f8229F068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229F068 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8229f0c8
  if (cr6.eq) goto loc_8229F0C8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,1968(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1968);
  // lwz r31,0(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,10780(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10780);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x8229f0a8
  if (!cr6.eq) goto loc_8229F0A8;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x8229F0A8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229F0A8:
  // lwz r10,10768(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 10768);
  // lwz r11,10780(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10780);
  // subf r9,r30,r11
  r9 = r11 - r30;
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // subfc r11,r11,r9
  ca = uint32_t(r9) >= uint32_t(r11);
  r11 = r9 - r11;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // clrlwi r3,r11,31
  r3 = uint32_t(r11) & 0x1;
loc_8229F0C8:
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

// sub_8229FC40: lifted from generated/ by lift.py.
lifted::Result f8229FC40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8229FC40 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
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
  // stwu r1,-304(r1)
  ea = -304 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f30,f1
  f30 = f1;
  // mr r26,r4
  r26 = r4;
  // mr r28,r6
  r28 = r6;
  // mr r27,r8
  r27 = r8;
  // li r18,0
  r18 = uint64_t(int64_t(0));
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x8229fd64
  if (!cr6.eq) goto loc_8229FD64;
  // lwz r8,12184(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 12184);
  // mr. r11,r8
  r11 = r8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8229fc84
  if (!cr0.eq) goto loc_8229FC84;
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
loc_8229FC84:
  // lbz r10,10808(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 10808);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // stw r18,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r18));
  // rlwinm r9,r11,14,18,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 14) & 0x3FFF;
  // stw r18,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r18));
  // rlwinm r11,r11,29,17,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x7FFF;
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r9,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r9));
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // rlwinm. r7,r10,0,27,27
  r7 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq 0x8229fcbc
  if (cr0.eq) goto loc_8229FCBC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x8229fd48
  goto loc_8229FD48;
loc_8229FCBC:
  // rlwinm. r11,r10,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8229fd40
  if (cr0.eq) goto loc_8229FD40;
  // lwz r11,12468(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12468);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // beq cr6,0x8229fcd8
  if (cr6.eq) goto loc_8229FCD8;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // bne cr6,0x8229fd40
  if (!cr6.eq) goto loc_8229FD40;
loc_8229FCD8:
  // lwz r11,12188(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12188);
  // lwz r10,12472(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12472);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x8229fcf0
  if (cr6.eq) goto loc_8229FCF0;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229fd40
  if (!cr6.eq) goto loc_8229FD40;
loc_8229FCF0:
  // lwz r11,12192(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12192);
  // lwz r10,12476(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12476);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x8229fd08
  if (cr6.eq) goto loc_8229FD08;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229fd40
  if (!cr6.eq) goto loc_8229FD40;
loc_8229FD08:
  // lwz r11,12196(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12196);
  // lwz r10,12480(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12480);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x8229fd20
  if (cr6.eq) goto loc_8229FD20;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229fd40
  if (!cr6.eq) goto loc_8229FD40;
loc_8229FD20:
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
  // lwz r10,12484(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12484);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x8229fd38
  if (cr6.eq) goto loc_8229FD38;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229fd40
  if (!cr6.eq) goto loc_8229FD40;
loc_8229FD38:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // b 0x8229fd44
  goto loc_8229FD44;
loc_8229FD40:
  // mr r11,r18
  r11 = r18;
loc_8229FD44:
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
loc_8229FD48:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8229fd60
  if (cr0.eq) goto loc_8229FD60;
  // lwz r11,12916(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12916);
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // lwz r11,12920(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12920);
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
loc_8229FD60:
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
loc_8229FD64:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // bne cr6,0x8229fd88
  if (!cr6.eq) goto loc_8229FD88;
  // stfs f31,128(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // addi r28,r1,128
  r28 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f31,132(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // stfs f31,136(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // stfs f31,140(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
loc_8229FD88:
  // lwz r11,12200(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12200);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8229fd9c
  if (!cr6.eq) goto loc_8229FD9C;
  // rlwinm. r26,r26,0,28,25
  r26 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 0) & 0xFFFFFFFFFFFFFFCF;
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // beq 0x822a04e0
  if (cr0.eq) goto loc_822A04E0;
loc_8229FD9C:
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // lwz r9,12392(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 12392);
  // mr r19,r11
  r19 = r11;
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bgt cr6,0x8229fdb4
  if (cr6.gt) goto loc_8229FDB4;
  // mr r19,r9
  r19 = r9;
loc_8229FDB4:
  // lwz r11,4(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 4);
  // lwz r8,12396(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 12396);
  // mr r20,r11
  r20 = r11;
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // bgt cr6,0x8229fdcc
  if (cr6.gt) goto loc_8229FDCC;
  // mr r20,r8
  r20 = r8;
loc_8229FDCC:
  // lwz r10,12400(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12400);
  // lwz r11,8(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 8);
  // add r10,r10,r9
  r10 = r10 + r9;
  // mr r21,r11
  r21 = r11;
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // blt cr6,0x8229fde8
  if (cr6.lt) goto loc_8229FDE8;
  // mr r21,r10
  r21 = r10;
loc_8229FDE8:
  // lwz r10,12404(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12404);
  // lwz r11,12(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 12);
  // add r10,r10,r8
  r10 = r10 + r8;
  // mr r22,r11
  r22 = r11;
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // blt cr6,0x8229fe04
  if (cr6.lt) goto loc_8229FE04;
  // mr r22,r10
  r22 = r10;
loc_8229FE04:
  // lwz r11,11600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 11600);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8229fe50
  if (cr6.eq) goto loc_8229FE50;
  // lwz r11,12416(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12416);
  // cmpw cr6,r19,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r19), int32_t(r11), so);
  // bgt cr6,0x8229fe20
  if (cr6.gt) goto loc_8229FE20;
  // mr r19,r11
  r19 = r11;
loc_8229FE20:
  // lwz r11,12420(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12420);
  // cmpw cr6,r20,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r20), int32_t(r11), so);
  // bgt cr6,0x8229fe30
  if (cr6.gt) goto loc_8229FE30;
  // mr r20,r11
  r20 = r11;
loc_8229FE30:
  // lwz r11,12424(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12424);
  // cmpw cr6,r21,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r11), so);
  // blt cr6,0x8229fe40
  if (cr6.lt) goto loc_8229FE40;
  // mr r21,r11
  r21 = r11;
loc_8229FE40:
  // lwz r11,12428(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12428);
  // cmpw cr6,r22,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r22), int32_t(r11), so);
  // blt cr6,0x8229fe50
  if (cr6.lt) goto loc_8229FE50;
  // mr r22,r11
  r22 = r11;
loc_8229FE50:
  // cmpw cr6,r21,r19
  cr6 = lifted::Compare<int32_t>(int32_t(r21), int32_t(r19), so);
  // ble cr6,0x822a04e0
  if (!cr6.gt) goto loc_822A04E0;
  // cmpw cr6,r22,r20
  cr6 = lifted::Compare<int32_t>(int32_t(r22), int32_t(r20), so);
  // ble cr6,0x822a04e0
  if (!cr6.gt) goto loc_822A04E0;
  // ld r4,16(r31)
  r4 = REX_LOAD_U64(uint32_t(r31) + 16);
  // cmpldi cr6,r4,0
  cr6 = lifted::Compare<uint64_t>(r4, 0, so);
  // beq cr6,0x8229ff08
  if (cr6.eq) goto loc_8229FF08;
  // mr r11,r4
  r11 = r4;
  // rlwinm r11,r11,0,23,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x100;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x8229fe98
  if (cr6.eq) goto loc_8229FE98;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r5,10432(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 10432);
  // mr r3,r31
  r3 = r31;
  // bl 0x822adc08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADC08(base, LIFTED_ARGS(0x8229FE8C, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // or r11,r3,r11
  r11 = r3 | r11;
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
loc_8229FE98:
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // clrldi r10,r11,52
  r10 = r11 & 0xFFF;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x8229fec8
  if (cr6.eq) goto loc_8229FEC8;
  // addi r6,r31,10420
  r6 = uint64_t(int64_t(int64_t(r31) + 10420));
  // li r5,8704
  r5 = uint64_t(int64_t(8704));
  // rldicr r4,r11,52,11
  r4 = __builtin_rotateleft64(r11, 52) & 0xFFF0000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x8229FEBC, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // rldicr r11,r11,0,51
  r11 = __builtin_rotateleft64(r11, 0) & 0xFFFFFFFFFFFFF000;
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
loc_8229FEC8:
  // li r12,31
  r12 = uint64_t(int64_t(31));
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // rldicr r12,r12,44,19
  r12 = __builtin_rotateleft64(r12, 44) & 0xFFFFF00000000000;
  // and r10,r11,r12
  r10 = r11 & r12;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x8229ff08
  if (cr6.eq) goto loc_8229FF08;
  // addi r6,r31,10400
  r6 = uint64_t(int64_t(int64_t(r31) + 10400));
  // li r5,8576
  r5 = uint64_t(int64_t(8576));
  // rldicr r4,r11,15,4
  r4 = __builtin_rotateleft64(r11, 15) & 0xF800000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x8229FEF4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r12,-32
  r12 = uint64_t(int64_t(-32));
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // rldicr r12,r12,44,63
  r12 = __builtin_rotateleft64(r12, 44) & 0xFFFFFFFFFFFFFFFF;
  // and r11,r11,r12
  r11 = r11 & r12;
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
loc_8229FF08:
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x8229ffd4
  if (cr6.eq) goto loc_8229FFD4;
  // rlwinm r10,r11,0,9,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x7FFFFC;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x8229ff48
  if (cr6.eq) goto loc_8229FF48;
  // addi r6,r31,10316
  r6 = uint64_t(int64_t(int64_t(r31) + 10316));
  // li r5,8448
  r5 = uint64_t(int64_t(8448));
  // rldicr r4,r11,41,20
  r4 = __builtin_rotateleft64(r11, 41) & 0xFFFFF80000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x8229FF34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r12,-128
  r12 = uint64_t(int64_t(-8388608));
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ori r12,r12,3
  r12 = r12 | 3;
  // and r11,r11,r12
  r11 = r11 & r12;
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
loc_8229FF48:
  // lis r12,-128
  r12 = uint64_t(int64_t(-8388608));
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // clrldi r12,r12,25
  r12 = r12 & 0x7FFFFFFFFF;
  // and r10,r11,r12
  r10 = r11 & r12;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x8229ff8c
  if (cr6.eq) goto loc_8229FF8C;
  // addi r6,r31,10240
  r6 = uint64_t(int64_t(int64_t(r31) + 10240));
  // li r5,8192
  r5 = uint64_t(int64_t(8192));
  // rldicr r4,r11,25,15
  r4 = __builtin_rotateleft64(r11, 25) & 0xFFFF000000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x8229FF74, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r12,-1
  r12 = uint64_t(int64_t(-65536));
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ori r12,r12,0
  r12 = r12 | 0;
  // rldicr r12,r12,23,63
  r12 = __builtin_rotateleft64(r12, 23) & 0xFFFFFFFFFFFFFFFF;
  // and r11,r11,r12
  r11 = r11 & r12;
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
loc_8229FF8C:
  // lis r12,31
  r12 = uint64_t(int64_t(2031616));
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ori r12,r12,65535
  r12 = r12 | 65535;
  // rldicr r12,r12,39,24
  r12 = __builtin_rotateleft64(r12, 39) & 0xFFFFFF8000000000;
  // and r10,r11,r12
  r10 = r11 & r12;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x8229ffd4
  if (cr6.eq) goto loc_8229FFD4;
  // addi r6,r31,10468
  r6 = uint64_t(int64_t(int64_t(r31) + 10468));
  // li r5,8832
  r5 = uint64_t(int64_t(8832));
  // rldicr r4,r11,4,20
  r4 = __builtin_rotateleft64(r11, 4) & 0xFFFFF80000000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x8229FFBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r12,-32
  r12 = uint64_t(int64_t(-2097152));
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ori r12,r12,0
  r12 = r12 | 0;
  // rldicr r12,r12,39,63
  r12 = __builtin_rotateleft64(r12, 39) & 0xFFFFFFFFFFFFFFFF;
  // and r11,r11,r12
  r11 = r11 & r12;
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
loc_8229FFD4:
  // ld r11,32(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 32);
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822a000c
  if (cr6.eq) goto loc_822A000C;
  // clrldi r10,r11,26
  r10 = r11 & 0x3FFFFFFFFF;
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // beq cr6,0x822a000c
  if (cr6.eq) goto loc_822A000C;
  // addi r6,r31,10552
  r6 = uint64_t(int64_t(int64_t(r31) + 10552));
  // li r5,8960
  r5 = uint64_t(int64_t(8960));
  // rldicr r4,r11,26,37
  r4 = __builtin_rotateleft64(r11, 26) & 0xFFFFFFFFFC000000;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ade30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADE30(base, LIFTED_ARGS(0x822A0000, r31)); r3 = o.r3; f1 = o.f1; }
  // ld r11,32(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 32);
  // rldicr r11,r11,0,25
  r11 = __builtin_rotateleft64(r11, 0) & 0xFFFFFFC000000000;
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
loc_822A000C:
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r10,48(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822a0028
  if (!cr6.gt) goto loc_822A0028;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822A0024, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822A0028:
  // lis r10,-16356
  r10 = uint64_t(int64_t(-1071906816));
  // mr r8,r18
  r8 = r18;
  // ori r9,r10,11008
  r9 = r10 | 11008;
  // li r7,27
  r7 = uint64_t(int64_t(27));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // li r5,108
  r5 = uint64_t(int64_t(108));
  // addi r30,r10,13656
  r30 = uint64_t(int64_t(int64_t(r10) + 13656));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r4,r30,40
  r4 = uint64_t(int64_t(int64_t(r30) + 40));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r11
  r29 = r11;
  // addi r3,r29,4
  r3 = uint64_t(int64_t(int64_t(r29) + 4));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822A0060, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r29,108
  r3 = uint64_t(int64_t(int64_t(r29) + 108));
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // stw r3,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r3));
  // ble cr6,0x822a007c
  if (!cr6.gt) goto loc_822A007C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822A007C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822A007C:
  // lis r11,-16374
  r11 = uint64_t(int64_t(-1073086464));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r11,r11,11008
  r11 = r11 | 11008;
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // mr r4,r30
  r4 = r30;
  // li r5,36
  r5 = uint64_t(int64_t(36));
  // stwu r11,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r11));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r9));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r3,r30,4
  r3 = uint64_t(int64_t(int64_t(r30) + 4));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822A00AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // addi r11,r30,36
  r11 = uint64_t(int64_t(int64_t(r30) + 36));
  // ori r10,r10,8576
  r10 = r10 | 8576;
  // lis r9,4097
  r9 = uint64_t(int64_t(268500992));
  // mr r8,r18
  r8 = r18;
  // ori r9,r9,1
  r9 = r9 | 1;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,48(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // ble cr6,0x822a00ec
  if (!cr6.gt) goto loc_822A00EC;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822A00E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822A00EC:
  // mr r4,r11
  r4 = r11;
  // mr r3,r31
  r3 = r31;
  // bl 0x822af978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AF978(base, LIFTED_ARGS(0x822A00F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,10432(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 10432);
  // mr r11,r18
  r11 = r18;
  // rlwinm r5,r10,0,0,27
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFF0;
  // rlwinm. r9,r26,0,27,27
  r9 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x822a0114
  if (cr0.eq) goto loc_822A0114;
  // li r11,118
  r11 = uint64_t(int64_t(118));
  // ori r5,r5,1
  r5 = r5 | 1;
loc_822A0114:
  // rlwinm. r10,r26,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822a0144
  if (cr0.eq) goto loc_822A0144;
  // li r10,8461
  r10 = uint64_t(int64_t(8461));
  // li r9,255
  r9 = uint64_t(int64_t(255));
  // ori r11,r11,34561
  r11 = r11 | 34561;
  // rlwimi r27,r9,16,0,23
  r27 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0xFFFFFF00) | (r27 & 0xFFFFFFFF000000FF);
  // ori r5,r5,4
  r5 = r5 | 4;
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r27,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r27));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // ld r10,24(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ori r10,r10,512
  r10 = r10 | 512;
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
loc_822A0144:
  // li r10,8704
  r10 = uint64_t(int64_t(8704));
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // rldicr r12,r12,51,63
  r12 = __builtin_rotateleft64(r12, 51) & 0xFFFFFFFFFFFFFFFF;
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r11,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r11));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // ld r10,16(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 16);
  // mr r11,r3
  r11 = r3;
  // ori r10,r10,8
  r10 = r10 | 8;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // or r10,r10,r12
  r10 = r10 | r12;
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // rldicr r12,r12,52,63
  r12 = __builtin_rotateleft64(r12, 52) & 0xFFFFFFFFFFFFFFFF;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // or r10,r10,r12
  r10 = r10 | r12;
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // rldicr r12,r12,43,63
  r12 = __builtin_rotateleft64(r12, 43) & 0xFFFFFFFFFFFFFFFF;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // or r10,r10,r12
  r10 = r10 | r12;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // ori r10,r10,2048
  r10 = r10 | 2048;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // ori r10,r10,256
  r10 = r10 | 256;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // lwz r10,10560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 10560);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822a01b4
  if (!cr0.eq) goto loc_822A01B4;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f31,7140(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 7140);
  f31 = double(temp.f32);
loc_822A01B4:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822adc08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822ADC08(base, LIFTED_ARGS(0x822A01C8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r26,28
  r11 = uint32_t(r26) & 0xF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822a01e0
  if (!cr0.eq) goto loc_822A01E0;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r23,1
  r23 = uint64_t(int64_t(1));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
  // b 0x822a0228
  goto loc_822A0228;
loc_822A01E0:
  // mr r23,r18
  r23 = r18;
  // mr r11,r18
  r11 = r18;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r9,r31,12184
  r9 = uint64_t(int64_t(int64_t(r31) + 12184));
loc_822A01F0:
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // slw r8,r8,r11
  r8 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r11) & 0x3F));
  // and. r8,r8,r26
  r8 = r8 & r26;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x822a0218
  if (cr0.eq) goto loc_822A0218;
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x822a0218
  if (cr6.eq) goto loc_822A0218;
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addi r23,r23,1
  r23 = uint64_t(int64_t(int64_t(r23) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
loc_822A0218:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x822a01f0
  if (cr6.lt) goto loc_822A01F0;
loc_822A0228:
  // mr r24,r18
  r24 = r18;
  // cmplwi cr6,r23,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 0, so);
  // beq cr6,0x822a04c0
  if (cr6.eq) goto loc_822A04C0;
  // lis r11,-16384
  r11 = uint64_t(int64_t(-1073741824));
  // addi r25,r1,144
  r25 = uint64_t(int64_t(int64_t(r1) + 144));
  // ori r26,r11,13825
  r26 = r11 | 13825;
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r27,r11,136
  r27 = r11 | 136;
loc_822A0248:
  // lwz r30,40(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r29,0(r25)
  r29 = REX_LOAD_U32(uint32_t(r25) + 0);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // ble cr6,0x822a0268
  if (!cr6.gt) goto loc_822A0268;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822A0264, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_822A0268:
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8229e308
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229E308(base, LIFTED_ARGS(0x822A0278, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822a04f0
  if (cr0.eq) goto loc_822A04F0;
  // extsw r9,r19
  r9 = uint64_t(int64_t(int32_t(r19)));
  // stfs f30,8(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // extsw r11,r20
  r11 = uint64_t(int64_t(int32_t(r20)));
  // extsw r10,r21
  r10 = uint64_t(int64_t(int32_t(r21)));
  // extsw r8,r22
  r8 = uint64_t(int64_t(int32_t(r22)));
  // lis r5,2
  r5 = uint64_t(int64_t(131072));
  // std r9,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r9);
  // mr r7,r18
  r7 = r18;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // clrlwi r10,r3,3
  r10 = uint32_t(r3) & 0x1FFFFFFF;
  // std r8,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r8);
  // lfd f11,112(r1)
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 112)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // ori r9,r11,18432
  r9 = r11 | 18432;
  // rlwinm r11,r3,12,20,31
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 12) & 0xFFF;
  // mr r8,r18
  r8 = r18;
  // addi r11,r11,512
  r11 = uint64_t(int64_t(int64_t(r11) + 512));
  // mr r6,r18
  r6 = r18;
  // rlwinm r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  // ori r5,r5,20480
  r5 = r5 | 20480;
  // add r11,r11,r10
  r11 = r11 + r10;
  // mr r4,r18
  r4 = r18;
  // ori r10,r11,3
  r10 = r11 | 3;
  // frsp f11,f11
  f11 = double(float(f11));
  // cmpwi cr6,r29,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), -1, so);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f0,f0
  f0 = double(float(f0));
  // frsp f13,f13
  f13 = double(float(f13));
  // fsubs f12,f12,f31
  f12 = double(float(f12 - f31));
  // stfs f12,0(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 0, temp.u32);
  // fsubs f0,f0,f31
  f0 = double(float(f0 - f31));
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // fsubs f12,f11,f31
  f12 = double(float(f11 - f31));
  // lfs f11,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f11 = double(temp.f32);
  // stfs f11,12(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // fsubs f13,f13,f31
  f13 = double(float(f13 - f31));
  // lfs f11,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f11 = double(temp.f32);
  // stfs f11,16(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // lfs f11,8(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 8);
  f11 = double(temp.f32);
  // stfs f11,20(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 20, temp.u32);
  // lfs f11,12(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 12);
  f11 = double(temp.f32);
  // stfs f11,24(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 24, temp.u32);
  // stfs f13,28(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // stfs f0,32(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 32, temp.u32);
  // stfs f30,36(r3)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // lfs f0,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // stfs f0,40(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 40, temp.u32);
  // lfs f0,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f0 = double(temp.f32);
  // stfs f0,44(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // lfs f0,8(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 8);
  f0 = double(temp.f32);
  // stfs f0,48(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // lfs f0,12(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 12);
  f0 = double(temp.f32);
  // stfs f0,52(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // stfs f13,56(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // stfs f12,60(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // stfs f30,64(r3)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r3) + 64, temp.u32);
  // lfs f0,0(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 0);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // lfs f0,4(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 4);
  f0 = double(temp.f32);
  // stfs f0,72(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 72, temp.u32);
  // lfs f0,8(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 8);
  f0 = double(temp.f32);
  // stfs f0,76(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 76, temp.u32);
  // lfs f0,12(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 12);
  f0 = double(temp.f32);
  // stfs f0,80(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // stwu r9,4(r30)
  ea = 4 + uint32_t(r30);
  REX_STORE_U32(ea, uint32_t(r9));
  r30 = (r30 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r9,r18
  r9 = r18;
  // mr r11,r30
  r11 = r30;
  // mr r3,r18
  r3 = r18;
  // mr r30,r18
  r30 = r18;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // ori r10,r10,86
  r10 = r10 | 86;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r10,8712
  r10 = uint64_t(int64_t(8712));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r5));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r4,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r4));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r3,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r3));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r30,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r30));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bne cr6,0x822a040c
  if (!cr6.eq) goto loc_822A040C;
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // mr r8,r26
  r8 = r26;
  // mr r10,r27
  r10 = r27;
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // b 0x822a04ac
  goto loc_822A04AC;
loc_822A040C:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,8452
  r8 = uint64_t(int64_t(8452));
  // lis r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,8706
  r6 = uint64_t(int64_t(8706));
  // ori r7,r7,65535
  r7 = r7 | 65535;
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r18
  r10 = r18;
  // li r5,8705
  r5 = uint64_t(int64_t(8705));
  // lis r4,1
  r4 = uint64_t(int64_t(65536));
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // ori r4,r4,1
  r4 = r4 | 1;
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // rldicr r12,r12,37,63
  r12 = __builtin_rotateleft64(r12, 37) & 0xFFFFFFFFFFFFFFFF;
  // addi r3,r29,3046
  r3 = uint64_t(int64_t(int64_t(r29) + 3046));
  // li r30,8193
  r30 = uint64_t(int64_t(8193));
  // rlwinm r3,r3,2,0,29
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // mr r29,r26
  r29 = r26;
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r17,r27
  r17 = r27;
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r5));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r4,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r4));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // ld r10,24(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 24);
  // oris r10,r10,4
  r10 = r10 | 262144;
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
  // ld r10,16(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 16);
  // ori r10,r10,512
  r10 = r10 | 512;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // ori r10,r10,1024
  r10 = r10 | 1024;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // ld r10,24(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 24);
  // or r10,r10,r12
  r10 = r10 | r12;
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
  // stwu r30,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r30));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwzx r10,r3,r31
  r10 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r31));
  // lwz r10,28(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 28);
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r29,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r29));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r17,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r17));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
loc_822A04AC:
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // addi r25,r25,4
  r25 = uint64_t(int64_t(int64_t(r25) + 4));
  // cmplw cr6,r24,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), uint32_t(r23), so);
  // blt cr6,0x822a0248
  if (cr6.lt) goto loc_822A0248;
loc_822A04C0:
  // lbz r11,10809(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10809);
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // ori r11,r11,4
  r11 = r11 | 4;
  // rldicr r12,r12,49,63
  r12 = __builtin_rotateleft64(r12, 49) & 0xFFFFFFFFFFFFFFFF;
  // stb r11,10809(r31)
  REX_STORE_U8(uint32_t(r31) + 10809, uint8_t(r11));
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // or r11,r11,r12
  r11 = r11 | r12;
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
loc_822A04E0:
  // addi r1,r1,304
  r1 = uint64_t(int64_t(int64_t(r1) + 304));
  // lfd f30,-144(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -144)));
  // lfd f31,-136(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -136)));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
loc_822A04F0:
  // stw r30,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r30));
  // b 0x822a04e0
  goto loc_822A04E0;
}

// sub_822A0718: lifted from generated/ by lift.py.
lifted::Result f822A0718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822A0718 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32768
  r11 = uint64_t(int64_t(-2147483648));
  // lwz r10,13240(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 13240);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x822a088c
  if (cr6.eq) goto loc_822A088C;
  // lwz r11,13476(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 13476);
  // rlwinm. r11,r11,0,0,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFF000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822a088c
  if (cr0.eq) goto loc_822A088C;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x822b1ff8
  // (left out: dead in C++)
  { const lifted::Result o = f822B1FF8(base, r1, r13, r3, r4, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r10,48(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822a0768
  if (!cr6.gt) goto loc_822A0768;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822A0764, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822A0768:
  // li r10,1401
  r10 = uint64_t(int64_t(1401));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r8,-16380
  r8 = uint64_t(int64_t(-1073479680));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // ori r30,r8,15360
  r30 = r8 | 15360;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r10,19
  r10 = uint64_t(int64_t(19));
  // mr r8,r30
  r8 = r30;
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // li r5,256
  r5 = uint64_t(int64_t(256));
  // mr r6,r29
  r6 = r29;
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,10772(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 10772);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // clrlwi r9,r10,3
  r9 = uint32_t(r10) & 0x1FFFFFFF;
  // addi r10,r8,512
  r10 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // add r10,r10,r9
  r10 = r10 + r9;
  // ori r10,r10,2
  r10 = r10 | 2;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r5));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,13240(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 13240);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x822a0800
  if (cr6.lt) goto loc_822A0800;
  // beq cr6,0x822a0800
  if (cr6.eq) goto loc_822A0800;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x822a07f8
  if (cr6.lt) goto loc_822A07F8;
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // bne cr6,0x822a0808
  if (!cr6.eq) goto loc_822A0808;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x822a080c
  goto loc_822A080C;
loc_822A07F8:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x822a080c
  goto loc_822A080C;
loc_822A0800:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x822a080c
  goto loc_822A080C;
loc_822A0808:
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
loc_822A080C:
  // lwz r9,11596(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 11596);
  // rlwinm r8,r10,16,0,15
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF0000;
  // lis r10,-32214
  r10 = uint64_t(int64_t(-2111176704));
  // rlwinm r9,r9,9,25,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 9) & 0x7F;
  // addi r6,r10,1672
  r6 = uint64_t(int64_t(int64_t(r10) + 1672));
  // or r7,r9,r8
  r7 = r9 | r8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r11
  r4 = r11;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229e9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229E9D0(base, LIFTED_ARGS(0x822A0834, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,19
  r10 = uint64_t(int64_t(19));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,256
  r6 = uint64_t(int64_t(256));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stwu r30,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r30));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r3,r31
  r3 = r31;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,10772(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 10772);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // clrlwi r9,r10,3
  r9 = uint32_t(r10) & 0x1FFFFFFF;
  // addi r10,r8,512
  r10 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // add r10,r10,r9
  r10 = r10 + r9;
  // ori r10,r10,2
  r10 = r10 | 2;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r29,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r29));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // bl 0x822b1ff8
  // (left out: dead in C++)
  { const lifted::Result o = f822B1FF8(base, r1, r13, r3, r4, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_822A088C:
  // lbz r11,10809(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10809);
  // ori r11,r11,32
  r11 = r11 | 32;
  // stb r11,10809(r31)
  REX_STORE_U8(uint32_t(r31) + 10809, uint8_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822A31D8: lifted from generated/ by lift.py.
uint64_t f822A31D8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822A31D8 (lifted)");
  uint64_t r11 = 0;
  // mulli r11,r4,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(24))));
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r11,1024(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1024);
  // rlwinm r3,r11,22,29,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x7;
  // blr
  return r3;
}

// sub_822A3AA8: lifted from generated/ by lift.py.
lifted::Result f822A3AA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822A3AA8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-1632(r1)
  ea = -1632 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r4,r5
  r4 = r5;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822b0ab0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822B0AB0(base, LIFTED_ARGS(0x822A3ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,13608
  r3 = uint64_t(int64_t(int64_t(r31) + 13608));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r10,r3
  r10 = r3;
  // addi r9,r11,1536
  r9 = uint64_t(int64_t(int64_t(r11) + 1536));
loc_822A3ADC:
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = REX_LOAD_U8(uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x822a3afc
  if (!cr0.eq) goto loc_822A3AFC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x822a3adc
  if (!cr6.eq) goto loc_822A3ADC;
loc_822A3AFC:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x822a3b1c
  if (cr0.eq) goto loc_822A3B1C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,1536
  r5 = uint64_t(int64_t(1536));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822A3B10, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x822b0d18
  // (left out: dead in C++)
  { const lifted::Result o = f822B0D18(base, r1, r13, r3, r4, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_822A3B1C:
  // addi r1,r1,1632
  r1 = uint64_t(int64_t(int64_t(r1) + 1632));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822A5C50: lifted from generated/ by lift.py.
lifted::Result f822A5C50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_822A5C50 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r5
  r28 = r5;
  // addi r6,r1,116
  r6 = uint64_t(int64_t(int64_t(r1) + 116));
  // addi r5,r1,120
  r5 = uint64_t(int64_t(int64_t(r1) + 120));
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r10,r1,124
  r10 = uint64_t(int64_t(int64_t(r1) + 124));
  // addi r9,r1,128
  r9 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r6,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r6));
  // addi r8,r1,132
  r8 = uint64_t(int64_t(int64_t(r1) + 132));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // addi r7,r1,136
  r7 = uint64_t(int64_t(int64_t(r1) + 136));
  // addi r6,r1,140
  r6 = uint64_t(int64_t(int64_t(r1) + 140));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // mr r5,r28
  r5 = r28;
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // bl 0x822a4ff0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A4FF0(base, LIFTED_ARGS(0x822A5C9C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,25728
  r4 = uint64_t(int64_t(1686110208));
  // li r3,48
  r3 = uint64_t(int64_t(48));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822976F8(base, LIFTED_ARGS(0x822A5CA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x822a5cb8
  if (!cr0.eq) goto loc_822A5CB8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x822a5d00
  goto loc_822A5D00;
loc_822A5CB8:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r9,17409
  r9 = uint64_t(int64_t(17409));
  // rlwinm r10,r28,28,0,3
  r10 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 28) & 0xF0000000;
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // rlwimi r11,r9,20,11,9
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 20) & 0xFFFFFFFFFFDFFFFF) | (r11 & 0x200000);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r8,-1
  r8 = uint64_t(int64_t(-65536));
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // stw r9,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r9));
  // clrlwi r11,r11,10
  r11 = uint32_t(r11) & 0x3FFFFF;
  // stw r8,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r8));
  // or r11,r11,r10
  r11 = r11 | r10;
  // rlwimi r11,r29,22,4,9
  r11 = (__builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 22) & 0xFC00000) | (r11 & 0xFFFFFFFFF03FFFFF);
  // stw r11,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r11));
  // bl 0x822a6738
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A6738(base, LIFTED_ARGS(0x822A5CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_822A5D00:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822A8418: lifted from generated/ by lift.py.
void f822A8418(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822A8418 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
loc_822A8418:
  // lwz r9,0(r5)
  r9 = REX_LOAD_U32(uint32_t(r5) + 0);
  // clrlwi r11,r4,27
  r11 = uint32_t(r4) & 0x1F;
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // clrlwi r9,r9,31
  r9 = uint32_t(r9) & 0x1;
  // rlwinm r10,r4,27,5,31
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 27) & 0x7FFFFFF;
  // addic. r6,r6,-1
  ca = uint32_t(r6) > 0;
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // addi r10,r10,2496
  r10 = uint64_t(int64_t(int64_t(r10) + 2496));
  // addi r5,r5,4
  r5 = uint64_t(int64_t(int64_t(r5) + 4));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
  // slw r9,r9,r11
  r9 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r9) << (uint8_t(r11) & 0x3F));
  // slw r11,r8,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r11) & 0x3F));
  // lwzx r8,r10,r3
  r8 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r3));
  // andc r11,r8,r11
  r11 = r8 & ~r11;
  // or r11,r11,r9
  r11 = r11 | r9;
  // stwx r11,r10,r3
  REX_STORE_U32(uint32_t(r10) + uint32_t(r3), uint32_t(r11));
  // bne 0x822a8418
  if (!cr0.eq) goto loc_822A8418;
  // ld r11,24(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 24);
  // ori r11,r11,2
  r11 = r11 | 2;
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // blr
  return;
}

// sub_822A84C8: lifted from generated/ by lift.py.
void f822A84C8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822A84C8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // addi r11,r4,2504
  r11 = uint64_t(int64_t(int64_t(r4) + 2504));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r3
  r11 = r11 + r3;
loc_822A84D4:
  // lbz r10,11(r5)
  r10 = REX_LOAD_U8(uint32_t(r5) + 11);
  // addic. r6,r6,-1
  ca = uint32_t(r6) > 0;
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // lbz r9,7(r5)
  r9 = REX_LOAD_U8(uint32_t(r5) + 7);
  // rotlwi r10,r10,8
  r10 = __builtin_rotateleft32(uint32_t(r10), 8);
  // lbz r8,3(r5)
  r8 = REX_LOAD_U8(uint32_t(r5) + 3);
  // addi r5,r5,16
  r5 = uint64_t(int64_t(int64_t(r5) + 16));
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x822a84d4
  if (!cr0.eq) goto loc_822A84D4;
  // ld r11,24(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 24);
  // ori r11,r11,2
  r11 = r11 | 2;
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // blr
  return;
}

// sub_822A8518: lifted from generated/ by lift.py.
void f822A8518(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822A8518 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // addi r11,r4,2520
  r11 = uint64_t(int64_t(int64_t(r4) + 2520));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r3
  r11 = r11 + r3;
loc_822A8524:
  // lbz r10,11(r5)
  r10 = REX_LOAD_U8(uint32_t(r5) + 11);
  // addic. r6,r6,-1
  ca = uint32_t(r6) > 0;
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // lbz r9,7(r5)
  r9 = REX_LOAD_U8(uint32_t(r5) + 7);
  // rotlwi r10,r10,8
  r10 = __builtin_rotateleft32(uint32_t(r10), 8);
  // lbz r8,3(r5)
  r8 = REX_LOAD_U8(uint32_t(r5) + 3);
  // addi r5,r5,16
  r5 = uint64_t(int64_t(int64_t(r5) + 16));
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x822a8524
  if (!cr0.eq) goto loc_822A8524;
  // ld r11,24(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 24);
  // ori r11,r11,2
  r11 = r11 | 2;
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // blr
  return;
}

// sub_822AC2A0: lifted from generated/ by lift.py.
lifted::Result f822AC2A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822AC2A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // mr r31,r3
  r31 = r3;
  // clrldi r30,r11,32
  r30 = r11 & 0xFFFFFFFF;
  // addi r11,r31,1024
  r11 = uint64_t(int64_t(int64_t(r31) + 1024));
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // std r30,11560(r31)
  REX_STORE_U64(uint32_t(r31) + 11560, r30);
loc_822AC2C4:
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r9,r9,0,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFC;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,24
  r11 = uint64_t(int64_t(int64_t(r11) + 24));
  // bne 0x822ac2c4
  if (!cr0.eq) goto loc_822AC2C4;
  // addi r11,r31,1648
  r11 = uint64_t(int64_t(int64_t(r31) + 1648));
  // li r10,18
  r10 = uint64_t(int64_t(18));
loc_822AC2E4:
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwimi r9,r8,0,30,31
  r9 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x3) | (r9 & 0xFFFFFFFFFFFFFFFC);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne 0x822ac2e4
  if (!cr0.eq) goto loc_822AC2E4;
  // lis r9,8192
  r9 = uint64_t(int64_t(536870912));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // ori r8,r9,8192
  r8 = r9 | 8192;
  // lis r9,15
  r9 = uint64_t(int64_t(983040));
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // ori r7,r9,61440
  r7 = r9 | 61440;
  // stw r11,10476(r31)
  REX_STORE_U32(uint32_t(r31) + 10476, uint32_t(r11));
  // lis r9,15
  r9 = uint64_t(int64_t(983040));
  // lis r11,255
  r11 = uint64_t(int64_t(16711680));
  // stw r8,10300(r31)
  REX_STORE_U32(uint32_t(r31) + 10300, uint32_t(r8));
  // ori r6,r9,61696
  r6 = r9 | 61696;
  // ori r8,r11,65535
  r8 = r11 | 65535;
  // stw r10,10500(r31)
  REX_STORE_U32(uint32_t(r31) + 10500, uint32_t(r10));
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // stw r7,10580(r31)
  REX_STORE_U32(uint32_t(r31) + 10580, uint32_t(r7));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // stw r10,10640(r31)
  REX_STORE_U32(uint32_t(r31) + 10640, uint32_t(r10));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r6,10584(r31)
  REX_STORE_U32(uint32_t(r31) + 10584, uint32_t(r6));
  // stw r8,10316(r31)
  REX_STORE_U32(uint32_t(r31) + 10316, uint32_t(r8));
  // stw r9,10452(r31)
  REX_STORE_U32(uint32_t(r31) + 10452, uint32_t(r9));
  // stw r9,10560(r31)
  REX_STORE_U32(uint32_t(r31) + 10560, uint32_t(r9));
  // lwz r9,10436(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 10436);
  // lwz r8,10440(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 10440);
  // oris r9,r9,8
  r9 = r9 | 524288;
  // stw r5,10644(r31)
  REX_STORE_U32(uint32_t(r31) + 10644, uint32_t(r5));
  // oris r8,r8,1
  r8 = r8 | 65536;
  // stw r7,10696(r31)
  REX_STORE_U32(uint32_t(r31) + 10696, uint32_t(r7));
  // stw r9,10436(r31)
  REX_STORE_U32(uint32_t(r31) + 10436, uint32_t(r9));
  // stw r8,10440(r31)
  REX_STORE_U32(uint32_t(r31) + 10440, uint32_t(r8));
  // std r11,0(r31)
  REX_STORE_U64(uint32_t(r31) + 0, r11);
  // std r11,8(r31)
  REX_STORE_U64(uint32_t(r31) + 8, r11);
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r9,48(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 48);
  // stw r10,10788(r31)
  REX_STORE_U32(uint32_t(r31) + 10788, uint32_t(r10));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // ble cr6,0x822ac3b0
  if (!cr6.gt) goto loc_822AC3B0;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822AC3AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822AC3B0:
  // li r9,3329
  r9 = uint64_t(int64_t(3329));
  // lis r8,1024
  r8 = uint64_t(int64_t(67108864));
  // lis r10,-16382
  r10 = uint64_t(int64_t(-1073610752));
  // mr r6,r30
  r6 = r30;
  // ori r10,r10,8448
  r10 = r10 | 8448;
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r9,129
  r9 = uint64_t(int64_t(129));
  // mr r7,r10
  r7 = r10;
  // mr r4,r10
  r4 = r10;
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // li r5,130
  r5 = uint64_t(int64_t(130));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,3205
  r3 = uint64_t(int64_t(3205));
  // li r29,15
  r29 = uint64_t(int64_t(15));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,14308(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 14308);
  // oris r10,r10,32769
  r10 = r10 | 2147549184;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stwu r4,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r4));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r5));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r30,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r30));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,14312(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 14312);
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stwu r3,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r3));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r29,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r29));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r9,14304(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 14304);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x822ac440
  if (cr6.eq) goto loc_822AC440;
  // li r9,3205
  r9 = uint64_t(int64_t(3205));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,14304(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 14304);
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
loc_822AC440:
  // li r10,1404
  r10 = uint64_t(int64_t(1404));
  // lis r9,2989
  r9 = uint64_t(int64_t(195887104));
  // li r8,1403
  r8 = uint64_t(int64_t(1403));
  // ori r9,r9,61453
  r9 = r9 | 61453;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AC530: lifted from generated/ by lift.py.
lifted::Result f822AC530(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AC530 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-368(r1)
  ea = -368 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r10,r1,392
  r10 = uint64_t(int64_t(int64_t(r1) + 392));
  // mr r5,r3
  r5 = r3;
  // li r4,260
  r4 = uint64_t(int64_t(260));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r6,80(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825dba54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp___vsnprintf, LIFTED_ARGS(0x822AC578, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825db834
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__DbgPrint, LIFTED_ARGS(0x822AC580, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,368
  r1 = uint64_t(int64_t(int64_t(r1) + 368));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822AC590: lifted from generated/ by lift.py.
uint64_t f822AC590(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822AC590 (lifted)");
  uint64_t r11 = 0;
  // addis r11,r3,8178
  r11 = uint64_t(int64_t(int64_t(r3) + 535953408));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // blr
  return r3;
}

// sub_822AC5B8: lifted from generated/ by lift.py.
lifted::Result f822AC5B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AC5B8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-400(r1)
  ea = -400 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r10,r1,432
  r10 = uint64_t(int64_t(int64_t(r1) + 432));
  // mr r31,r3
  r31 = r3;
  // mr r5,r4
  r5 = r4;
  // li r4,260
  r4 = uint64_t(int64_t(260));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r6,96(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 96);
  // bl 0x825dba54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp___vsnprintf, LIFTED_ARGS(0x822AC604, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822ac620
  if (cr0.eq) goto loc_822AC620;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC61C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822ac624
  goto loc_822AC624;
loc_822AC620:
  // bl 0x822ac530
  // (left out: dead in C++)
  { const lifted::Result o = f822AC530(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822AC624:
  // addi r1,r1,400
  r1 = uint64_t(int64_t(int64_t(r1) + 400));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822AC638: lifted from generated/ by lift.py.
lifted::Result f822AC638(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AC638 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-400(r1)
  ea = -400 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r3,1488
  r3 = uint64_t(int64_t(1488));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC658, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r29,r3
  r29 = r3;
  // addi r5,r11,15072
  r5 = uint64_t(int64_t(int64_t(r11) + 15072));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,15052
  r4 = uint64_t(int64_t(int64_t(r11) + 15052));
  // mr r6,r29
  r6 = r29;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r26,11
  r26 = uint64_t(int64_t(11));
  // addi r27,r11,-28968
  r27 = uint64_t(int64_t(int64_t(r11) + -28968));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r30,r27,4
  r30 = uint64_t(int64_t(int64_t(r27) + 4));
  // addi r25,r11,15048
  r25 = uint64_t(int64_t(int64_t(r11) + 15048));
loc_822AC694:
  // lwz r11,-4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -4);
  // and. r11,r11,r29
  r11 = r11 & r29;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822ac6c4
  if (cr0.eq) goto loc_822AC6C4;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x822ac6b4
  if (cr6.eq) goto loc_822AC6B4;
  // mr r3,r31
  r3 = r31;
  // mr r4,r25
  r4 = r25;
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822AC6B4:
  // mr r3,r31
  r3 = r31;
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_822AC6C4:
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // bne 0x822ac694
  if (!cr0.eq) goto loc_822AC694;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x822ac6e8
  if (!cr6.eq) goto loc_822AC6E8;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,15028
  r4 = uint64_t(int64_t(int64_t(r11) + 15028));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822AC6E8:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,15024
  r4 = uint64_t(int64_t(int64_t(r11) + 15024));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r30,r27,88
  r30 = uint64_t(int64_t(int64_t(r27) + 88));
  // li r29,30
  r29 = uint64_t(int64_t(30));
  // addi r28,r11,15004
  r28 = uint64_t(int64_t(int64_t(r11) + 15004));
loc_822AC708:
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC718, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r5,4(r30)
  r5 = REX_LOAD_U32(uint32_t(r30) + 4);
  // mr r4,r28
  r4 = r28;
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // bne 0x822ac708
  if (!cr0.eq) goto loc_822AC708;
  // addi r11,r27,328
  r11 = uint64_t(int64_t(int64_t(r27) + 328));
  // li r23,5
  r23 = uint64_t(int64_t(5));
  // addi r29,r11,16
  r29 = uint64_t(int64_t(int64_t(r11) + 16));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r26,r11,3312
  r26 = uint64_t(int64_t(int64_t(r11) + 3312));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r27,r11,14996
  r27 = uint64_t(int64_t(int64_t(r11) + 14996));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r25,r11,14984
  r25 = uint64_t(int64_t(int64_t(r11) + 14984));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r24,r11,14980
  r24 = uint64_t(int64_t(int64_t(r11) + 14980));
loc_822AC764:
  // mr r3,r31
  r3 = r31;
  // mr r4,r24
  r4 = r24;
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x822ac840
  if (!cr6.gt) goto loc_822AC840;
loc_822AC780:
  // mr r6,r30
  r6 = r30;
  // lwz r5,4(r29)
  r5 = REX_LOAD_U32(uint32_t(r29) + 4);
  // mr r4,r25
  r4 = r25;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825dba64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__sprintf, LIFTED_ARGS(0x822AC794, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r30
  r28 = r30;
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
loc_822AC79C:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bge cr6,0x822ac7ac
  if (!cr6.lt) goto loc_822AC7AC;
  // mr r11,r30
  r11 = r30;
loc_822AC7AC:
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bge cr6,0x822ac824
  if (!cr6.lt) goto loc_822AC824;
  // lwz r11,-4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -4);
  // lwz r10,-8(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -8);
  // lwz r3,-16(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -16);
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // slw r11,r28,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r28) << (uint8_t(r11) & 0x3F));
  // or r4,r11,r10
  r4 = r11 | r10;
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC7D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-12(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -12);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC7E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r5,r3
  r5 = r3;
  // mr r10,r11
  r10 = r11;
loc_822AC7F0:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x822ac7f0
  if (!cr6.eq) goto loc_822AC7F0;
  // subf r11,r10,r11
  r11 = r11 - r10;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r4,r27
  r4 = r27;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825dba64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__sprintf, LIFTED_ARGS(0x822AC81C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // b 0x822ac79c
  goto loc_822AC79C;
loc_822AC824:
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x822ac780
  if (cr6.lt) goto loc_822AC780;
loc_822AC840:
  // addic. r23,r23,-1
  ca = uint32_t(r23) > 0;
  r23 = uint64_t(int64_t(int64_t(r23) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // addi r29,r29,24
  r29 = uint64_t(int64_t(int64_t(r29) + 24));
  // bne 0x822ac764
  if (!cr0.eq) goto loc_822AC764;
  // addi r1,r1,400
  r1 = uint64_t(int64_t(int64_t(r1) + 400));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AC858: lifted from generated/ by lift.py.
lifted::Result f822AC858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AC858 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r4
  r31 = r4;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r5,r31
  r5 = r31;
  // addi r4,r11,15368
  r4 = uint64_t(int64_t(int64_t(r11) + 15368));
  // mr r28,r3
  r28 = r3;
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,152(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 152);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822ac8a0
  if (!cr6.eq) goto loc_822AC8A0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lwz r6,164(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 164);
  // mr r3,r28
  r3 = r28;
  // lwz r5,172(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 172);
  // addi r4,r11,15240
  r4 = uint64_t(int64_t(int64_t(r11) + 15240));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822AC8A0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r28
  r3 = r28;
  // addi r4,r11,15104
  r4 = uint64_t(int64_t(int64_t(r11) + 15104));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r30,116(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 116);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x822ac90c
  if (cr0.eq) goto loc_822AC90C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r27,r11,15084
  r27 = uint64_t(int64_t(int64_t(r11) + 15084));
loc_822AC8C4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x822ac900
  if (!cr6.gt) goto loc_822AC900;
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
loc_822AC8D8:
  // mr r3,r28
  r3 = r28;
  // lwz r6,4(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r4,r27
  r4 = r27;
  // lwz r5,0(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r31,r31,8
  r31 = uint64_t(int64_t(int64_t(r31) + 8));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x822ac8d8
  if (cr6.lt) goto loc_822AC8D8;
loc_822AC900:
  // lwz r30,0(r30)
  r30 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne 0x822ac8c4
  if (!cr0.eq) goto loc_822AC8C4;
loc_822AC90C:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AC918: lifted from generated/ by lift.py.
lifted::Result f822AC918(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822AC918 (lifted)");
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
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r27,r3
  r27 = r3;
  // mr r29,r5
  r29 = r5;
  // mr r25,r6
  r25 = r6;
  // mr r28,r7
  r28 = r7;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x822ac948
  if (!cr6.eq) goto loc_822AC948;
  // lis r11,-32213
  r11 = uint64_t(int64_t(-2111111168));
  // addi r31,r11,-14960
  r31 = uint64_t(int64_t(int64_t(r11) + -14960));
loc_822AC948:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x822ac958
  if (!cr6.eq) goto loc_822AC958;
  // lis r11,-32213
  r11 = uint64_t(int64_t(-2111111168));
  // addi r29,r11,-14944
  r29 = uint64_t(int64_t(int64_t(r11) + -14944));
loc_822AC958:
  // li r3,1488
  r3 = uint64_t(int64_t(1488));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC964, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r3,3878
  r3 = uint64_t(int64_t(3878));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC974, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1403
  r3 = uint64_t(int64_t(1403));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC980, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r27,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r27));
  // mr r26,r3
  r26 = r3;
  // stw r31,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r31));
  // addi r7,r11,16360
  r7 = uint64_t(int64_t(int64_t(r11) + 16360));
  // stw r29,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r29));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r9,r28
  r9 = r28;
  // addi r4,r11,16288
  r4 = uint64_t(int64_t(int64_t(r11) + 16288));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,3215
  r5 = uint64_t(int64_t(3215));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lwz r11,2188(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 2188);
  // lhz r8,4(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 4);
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32763
  r11 = uint64_t(int64_t(-2147155968));
  // ori r11,r11,272
  r11 = r11 | 272;
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x822aca24
  if (!cr6.eq) goto loc_822ACA24;
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x822aca24
  if (cr6.eq) goto loc_822ACA24;
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822aca24
  if (cr6.eq) goto loc_822ACA24;
  // li r3,1404
  r3 = uint64_t(int64_t(1404));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AC9F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r3,1405
  r3 = uint64_t(int64_t(1405));
  // mtctr r31
  ctr = r31;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822ACA04, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r6,r3
  r6 = r3;
  // lwz r7,0(r25)
  r7 = REX_LOAD_U32(uint32_t(r25) + 0);
  // addi r4,r11,16080
  r4 = uint64_t(int64_t(int64_t(r11) + 16080));
  // mr r5,r30
  r5 = r30;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x822acaa0
  goto loc_822ACAA0;
loc_822ACA24:
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822ac638
  // (left out: dead in C++)
  { const lifted::Result o = f822AC638(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r12,32763
  r12 = uint64_t(int64_t(2147155968));
  // lis r10,1025
  r10 = uint64_t(int64_t(67174400));
  // ori r12,r12,61664
  r12 = r12 | 61664;
  // and r11,r30,r12
  r11 = r30 & r12;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x822aca68
  if (!cr6.eq) goto loc_822ACA68;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r11,15944
  r4 = uint64_t(int64_t(int64_t(r11) + 15944));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r11,15792
  r4 = uint64_t(int64_t(int64_t(r11) + 15792));
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r28,1
  r28 = uint64_t(int64_t(1));
loc_822ACA68:
  // cmplwi cr6,r26,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq cr6,0x822aca7c
  if (cr6.eq) goto loc_822ACA7C;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // bl 0x822ac858
  // (left out: dead in C++)
  { const lifted::Result o = f822AC858(base, r1, r13, r3, r4, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822ACA7C:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // beq cr6,0x822aca94
  if (cr6.eq) goto loc_822ACA94;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,15656
  r4 = uint64_t(int64_t(int64_t(r11) + 15656));
  // b 0x822aca9c
  goto loc_822ACA9C;
loc_822ACA94:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,15512
  r4 = uint64_t(int64_t(int64_t(r11) + 15512));
loc_822ACA9C:
  // bl 0x822ac5b8
  // (left out: dead in C++)
  { const lifted::Result o = f822AC5B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_822ACAA0:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822ACAA8: lifted from generated/ by lift.py.
lifted::Result f822ACAA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822ACAA8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,16688
  r3 = uint64_t(int64_t(int64_t(r11) + 16688));
  // bl 0x822ac530
  // (left out: dead in C++)
  { const lifted::Result o = f822AC530(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r7,20024(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 20024);
  // lwz r6,10772(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 10772);
  // lwz r3,13080(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 13080);
  // bl 0x822ac918
  // (left out: dead in C++)
  { const lifted::Result o = f822AC918(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,13080(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 13080);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822acb18
  if (cr6.eq) goto loc_822ACB18;
  // lbz r8,10809(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 10809);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,10780(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10780);
  // ori r8,r8,3
  r8 = r8 | 3;
  // lwz r9,10768(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 10768);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // stb r8,10809(r31)
  REX_STORE_U8(uint32_t(r31) + 10809, uint8_t(r8));
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // stw r10,10868(r31)
  REX_STORE_U32(uint32_t(r31) + 10868, uint32_t(r10));
  // bl 0x822b0f40
  // (left out: dead in C++)
  r3 = f822B0F40(base);
  // b 0x822acb34
  goto loc_822ACB34;
loc_822ACB18:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r11,16464
  r3 = uint64_t(int64_t(int64_t(r11) + 16464));
  // bl 0x822ac530
  // (left out: dead in C++)
  { const lifted::Result o = f822AC530(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r11,16368
  r3 = uint64_t(int64_t(int64_t(r11) + 16368));
  // bl 0x822ac530
  // (left out: dead in C++)
  { const lifted::Result o = f822AC530(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // twi 31,r0,22
  lifted::Trap(22);
loc_822ACB34:
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

// sub_822ACB68: lifted from generated/ by lift.py.
lifted::Result f822ACB68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_822ACB68 (lifted)");
  uint64_t r4 = 0, r12 = 0, r31 = 0;
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
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825dba84
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ExRegisterTitleTerminateNotification, LIFTED_ARGS(0x822ACB7C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dba74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__VdShutdownEngines, LIFTED_ARGS(0x822ACB80, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822ACFB8: lifted from generated/ by lift.py.
lifted::Result f822ACFB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822ACFB8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x822b4f10
  // (left out: dead in C++)
  { const lifted::Result o = f822B4F10(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822ad010
  if (cr6.eq) goto loc_822AD010;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x822a4060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A4060(base, LIFTED_ARGS(0x822ACFE8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r29
  r30 = r29;
loc_822ACFEC:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x822a3d30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3D30, LIFTED_ARGS(0x822ACFFC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x822acfec
  if (cr6.lt) goto loc_822ACFEC;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229f1b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F1B0(base, LIFTED_ARGS(0x822AD010, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD010:
  // lwz r3,13428(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 13428);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad024
  if (cr0.eq) goto loc_822AD024;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x822AD020, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,13428(r31)
  REX_STORE_U32(uint32_t(r31) + 13428, uint32_t(r29));
loc_822AD024:
  // lwz r3,13432(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 13432);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad038
  if (cr0.eq) goto loc_822AD038;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x822AD034, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,13432(r31)
  REX_STORE_U32(uint32_t(r31) + 13432, uint32_t(r29));
loc_822AD038:
  // lwz r3,13424(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 13424);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad04c
  if (cr0.eq) goto loc_822AD04C;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822A67B0(base, LIFTED_ARGS(0x822AD048, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,13424(r31)
  REX_STORE_U32(uint32_t(r31) + 13424, uint32_t(r29));
loc_822AD04C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AD058: lifted from generated/ by lift.py.
lifted::Result f822AD058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822AD058 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x822ad09c
  if (cr6.eq) goto loc_822AD09C;
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // bl 0x822ad058
  // (left out: dead in C++)
  { const lifted::Result o = f822AD058(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lis r4,-20096
  r4 = uint64_t(int64_t(-1317011456));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x822AD090, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,9344
  r4 = uint64_t(int64_t(612368384));
  // mr r3,r31
  r3 = r31;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x822AD09C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD09C:
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

// sub_822AD228: lifted from generated/ by lift.py.
lifted::Result f822AD228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822AD228 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // ld r11,-28976(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + -28976);
  // std r11,10752(r31)
  REX_STORE_U64(uint32_t(r31) + 10752, r11);
  // bl 0x825b9a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9A60(base, LIFTED_ARGS(0x822AD254, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r4,-19072
  r4 = uint64_t(int64_t(-1249902592));
  // li r3,4800
  r3 = uint64_t(int64_t(4800));
  // stw r11,10760(r31)
  REX_STORE_U32(uint32_t(r31) + 10760, uint32_t(r11));
  // stw r10,10764(r31)
  REX_STORE_U32(uint32_t(r31) + 10764, uint32_t(r10));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822976F8(base, LIFTED_ARGS(0x822AD270, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // stw r3,15176(r31)
  REX_STORE_U32(uint32_t(r31) + 15176, uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
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

// sub_822AD4A0: lifted from generated/ by lift.py.
lifted::Result f822AD4A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822AD4A0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x822ad228
  // (left out: dead in C++)
  { const lifted::Result o = f822AD228(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822ad4ec
  if (cr0.eq) goto loc_822AD4EC;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229e098
  // (left out: dead in C++)
  f8229E098(base, r3);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r11,20008(r31)
  REX_STORE_U32(uint32_t(r31) + 20008, uint32_t(r11));
  // stw r11,20012(r31)
  REX_STORE_U32(uint32_t(r31) + 20012, uint32_t(r11));
  // stw r11,13536(r31)
  REX_STORE_U32(uint32_t(r31) + 13536, uint32_t(r11));
  // bl 0x822ac2a0
  // (left out: dead in C++)
  { const lifted::Result o = f822AC2A0(base, r1, r13, r3, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x822acb90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822ACB90, LIFTED_ARGS(0x822AD4E8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_822AD4EC:
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

// sub_822AD500: lifted from generated/ by lift.py.
lifted::Result f822AD500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822AD500 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822ad520
  if (cr6.eq) goto loc_822AD520;
  // bl 0x8229da28
  // (left out: dead in C++)
  { const lifted::Result o = f8229DA28(base, r1, r13, r3, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_822AD520:
  // lbz r11,10808(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 10808);
  // lis r28,-20096
  r28 = uint64_t(int64_t(-1317011456));
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822ad664
  if (!cr0.eq) goto loc_822AD664;
  // lwz r11,40(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 40);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822ad544
  if (cr6.eq) goto loc_822AD544;
  // mr r3,r30
  r3 = r30;
  // bl 0x8229f1b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F1B0(base, LIFTED_ARGS(0x822AD544, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD544:
  // addi r31,r30,20084
  r31 = uint64_t(int64_t(int64_t(r30) + 20084));
  // li r29,4
  r29 = uint64_t(int64_t(4));
loc_822AD54C:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad55c
  if (cr0.eq) goto loc_822AD55C;
  // bl 0x8229d620
  // (left out: dead in C++)
  { const lifted::Result o = f8229D620(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_822AD55C:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x822ad54c
  if (!cr0.eq) goto loc_822AD54C;
  // lwz r3,20100(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20100);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad57c
  if (cr0.eq) goto loc_822AD57C;
  // mr r4,r28
  r4 = r28;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x822AD57C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD57C:
  // mr r3,r30
  r3 = r30;
  // bl 0x822b58c0
  // (left out: dead in C++)
  { const lifted::Result o = f822B58C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32256
  r31 = uint64_t(int64_t(-2113929216));
  // lwz r11,2032(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 2032);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x822ad5b0
  if (cr0.eq) goto loc_822AD5B0;
  // lwz r11,24(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 24);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,28
  r3 = uint64_t(int64_t(28));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AD5AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,2032(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 2032);
loc_822AD5B0:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822ad5d0
  if (cr0.eq) goto loc_822AD5D0;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,48
  r3 = uint64_t(int64_t(48));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822AD5D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD5D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dba14
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__VdSetSystemCommandBufferGpuIdentifierAddress, LIFTED_ARGS(0x822AD5D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x822acfb8
  // (left out: dead in C++)
  { const lifted::Result o = f822ACFB8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x822b62e8
  // (left out: dead in C++)
  { const lifted::Result o = f822B62E8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,2032(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 2032);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822ad604
  if (cr0.eq) goto loc_822AD604;
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // b 0x822ad608
  goto loc_822AD608;
loc_822AD604:
  // mr r11,r31
  r11 = r31;
loc_822AD608:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822ad62c
  if (cr6.eq) goto loc_822AD62C;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 8, so);
  // ble cr6,0x822ad624
  if (!cr6.gt) goto loc_822AD624;
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
loc_822AD624:
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
loc_822AD62C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbac4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__VdSetGraphicsInterruptCallback, LIFTED_ARGS(0x822AD638, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8229f280
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F280(base, LIFTED_ARGS(0x822AD644, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r11,-28520
  r3 = uint64_t(int64_t(int64_t(r11) + -28520));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,1968(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1968);
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // bl 0x825dba84
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ExRegisterTitleTerminateNotification, LIFTED_ARGS(0x822AD660, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dba74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__VdShutdownEngines, LIFTED_ARGS(0x822AD664, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD664:
  // lwz r3,15176(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 15176);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad678
  if (cr0.eq) goto loc_822AD678;
  // mr r4,r28
  r4 = r28;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82297790(base, LIFTED_ARGS(0x822AD678, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD678:
  // lwz r3,20020(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20020);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822ad688
  if (cr0.eq) goto loc_822AD688;
  // bl 0x822ad058
  // (left out: dead in C++)
  { const lifted::Result o = f822AD058(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_822AD688:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AD790: lifted from generated/ by lift.py.
lifted::Result f822AD790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822AD790 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r3
  r30 = r3;
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,502
  r4 = uint64_t(int64_t(502));
  // lwz r29,13164(r30)
  r29 = REX_LOAD_U32(uint32_t(r30) + 13164);
  // bl 0x8229e308
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229E308(base, LIFTED_ARGS(0x822AD7B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10809(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 10809);
  // mr r31,r3
  r31 = r3;
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822ad7c8
  if (cr0.eq) goto loc_822AD7C8;
  // lwz r31,15176(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 15176);
  // b 0x822ad834
  goto loc_822AD834;
loc_822AD7C8:
  // rlwinm r11,r31,12,20,31
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 12) & 0xFFF;
  // stw r31,13164(r30)
  REX_STORE_U32(uint32_t(r30) + 13164, uint32_t(r31));
  // clrlwi r10,r31,3
  r10 = uint32_t(r31) & 0x1FFFFFFF;
  // addi r11,r11,512
  r11 = uint64_t(int64_t(int64_t(r11) + 512));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // rlwinm r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addis r3,r11,-16384
  r3 = uint64_t(int64_t(int64_t(r11) + -1073741824));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
  // clrlwi r11,r11,1
  r11 = uint32_t(r11) & 0x7FFFFFFF;
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bne cr6,0x822ad80c
  if (!cr6.eq) goto loc_822AD80C;
  // lwz r11,13160(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 13160);
  // stw r3,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r3));
  // b 0x822ad82c
  goto loc_822AD82C;
loc_822AD80C:
  // stw r3,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r3));
  // lwz r11,13168(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 13168);
  // lwz r10,4(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 4);
  // subf r11,r29,r11
  r11 = r11 - r29;
  // addi r11,r11,-8
  r11 = uint64_t(int64_t(int64_t(r11) + -8));
  // srawi r11,r11,3
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x7) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 3));
  // rlwimi r11,r10,0,0,0
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000) | (r11 & 0xFFFFFFFF7FFFFFFF);
  // stw r11,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r11));
loc_822AD82C:
  // addi r4,r3,2008
  r4 = uint64_t(int64_t(int64_t(r3) + 2008));
  // bl 0x822ac470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AC470(base, LIFTED_ARGS(0x822AD834, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AD834:
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // addi r11,r31,2008
  r11 = uint64_t(int64_t(int64_t(r31) + 2008));
  // stw r3,13168(r30)
  REX_STORE_U32(uint32_t(r30) + 13168, uint32_t(r3));
  // stw r11,13172(r30)
  REX_STORE_U32(uint32_t(r30) + 13172, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822ADA98: lifted from generated/ by lift.py.
uint64_t f822ADA98(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822ADA98 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r9,10432(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 10432);
  // lwz r11,10420(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 10420);
  // rlwinm r10,r11,28,29,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x7;
  // rlwinm. r8,r9,0,27,27
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x822adabc
  if (!cr0.eq) goto loc_822ADABC;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x822adad8
  if (cr6.eq) goto loc_822ADAD8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // b 0x822adac8
  goto loc_822ADAC8;
loc_822ADABC:
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // beq cr6,0x822adad8
  if (cr6.eq) goto loc_822ADAD8;
  // cmplwi cr6,r10,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 6, so);
loc_822ADAC8:
  // beq cr6,0x822adad8
  if (cr6.eq) goto loc_822ADAD8;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // bne cr6,0x822adadc
  if (!cr6.eq) goto loc_822ADADC;
loc_822ADAD8:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_822ADADC:
  // rlwinm r7,r11,31,1,31
  r7 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7FFFFFFF;
  // cntlzw r8,r4
  r8 = uint32_t(r4) == 0 ? 32 : __builtin_clz(uint32_t(r4));
  // rlwinm r8,r8,27,31,31
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 27) & 0x1;
  // and r8,r8,r7
  r8 = r8 & r7;
  // and r9,r8,r9
  r9 = r8 & r9;
  // and r10,r9,r10
  r10 = r9 & r10;
  // clrlwi r3,r10,31
  r3 = uint32_t(r10) & 0x1;
  // clrlwi. r7,r11,31
  r7 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beqlr
  if (cr0.eq) return r3;
  // rlwinm r10,r11,0,18,20
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x3800;
  // rlwinm r9,r11,0,12,14
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xE0000;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // rlwinm. r8,r11,0,24,24
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // and r10,r10,r9
  r10 = r10 & r9;
  // and r3,r10,r3
  r3 = r10 & r3;
  // beqlr
  if (cr0.eq) return r3;
  // rlwinm r10,r11,0,0,2
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xE0000000;
  // rlwinm r11,r11,0,6,8
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x3800000;
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // and r11,r10,r11
  r11 = r10 & r11;
  // and r3,r11,r3
  r3 = r11 & r3;
  // blr
  return r3;
}

// sub_822ADB50: lifted from generated/ by lift.py.
lifted::Result f822ADB50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822ADB50 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r31,r7
  r31 = r7;
  // mr r30,r8
  r30 = r8;
loc_822ADB70:
  // lwz r11,44(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 44);
  // twllei r30,0
  if (int32_t(r30) == 0 || uint32_t(r30) < 0u) lifted::Trap(0);
  // subf r11,r4,r11
  r11 = r11 - r4;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // divw. r11,r10,r30
  r11 = uint32_t((int32_t(r30) && !(int32_t(r10) == INT32_MIN && int32_t(r30) == -1)) ? int32_t(r10) / int32_t(r30) : 0);
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // rotlwi r10,r10,1
  r10 = __builtin_rotateleft32(uint32_t(r10), 1);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // andc r10,r30,r10
  r10 = r30 & ~r10;
  // twlgei r10,-1
  if (int32_t(r10) == -1 || uint32_t(r10) > 4294967295u) lifted::Trap(0);
  // ble 0x822adbe0
  if (!cr0.gt) goto loc_822ADBE0;
  // mullw r11,r11,r30
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r30))));
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // blt cr6,0x822adbac
  if (cr6.lt) goto loc_822ADBAC;
  // mr r11,r31
  r11 = r31;
loc_822ADBAC:
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // subf r31,r11,r31
  r31 = r31 - r11;
  // rlwinm r10,r10,16,0,15
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF0000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // or r10,r10,r28
  r10 = r10 | r28;
  // stwu r10,4(r4)
  ea = 4 + uint32_t(r4);
  REX_STORE_U32(ea, uint32_t(r10));
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // beq cr6,0x822adbdc
  if (cr6.eq) goto loc_822ADBDC;
  // mr r10,r11
  r10 = r11;
loc_822ADBCC:
  // lwzu r9,4(r27)
  ea = 4 + uint32_t(r27);
  r9 = REX_LOAD_U32(ea);
  r27 = (r27 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stwu r9,4(r4)
  ea = 4 + uint32_t(r4);
  REX_STORE_U32(ea, uint32_t(r9));
  r4 = (r4 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bne 0x822adbcc
  if (!cr0.eq) goto loc_822ADBCC;
loc_822ADBDC:
  // add r28,r11,r28
  r28 = r11 + r28;
loc_822ADBE0:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x822adbfc
  if (cr6.eq) goto loc_822ADBFC;
  // mr r3,r29
  r3 = r29;
  // stw r4,40(r29)
  REX_STORE_U32(uint32_t(r29) + 40, uint32_t(r4));
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822ADBF4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,40(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 40);
  // b 0x822adb70
  goto loc_822ADB70;
loc_822ADBFC:
  // mr r3,r4
  r3 = r4;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AE918: lifted from generated/ by lift.py.
uint64_t f822AE918(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_822AE918 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r8,28(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 28);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // rlwinm r7,r10,27,27,31
  r7 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1F;
  // lhz r10,0(r5)
  r10 = REX_LOAD_U16(uint32_t(r5) + 0);
  // lwz r9,24(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 24);
  // mr r30,r4
  r30 = r4;
  // clrlwi r10,r10,20
  r10 = uint32_t(r10) & 0xFFF;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // add r10,r10,r7
  r10 = r10 + r7;
  // std r6,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r6);
  // stw r6,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r6));
  // add r10,r10,r8
  r10 = r10 + r8;
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwimi r11,r4,25,3,7
  r11 = (__builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 25) & 0x1F000000) | (r11 & 0xFFFFFFFFE0FFFFFF);
  // addi r10,r10,10
  r10 = uint64_t(int64_t(int64_t(r10) + 10));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r31,r10,r3
  r31 = r10 + r3;
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // clrlwi r10,r10,6
  r10 = uint32_t(r10) & 0x3FFFFFF;
  // oris r10,r10,51200
  r10 = r10 | 3355443200;
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
loc_822AE990:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // clrlwi r11,r11,20
  r11 = uint32_t(r11) & 0xFFF;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r3,r11,r30
  r3 = r11 + r30;
  // cmplw cr6,r3,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r10), so);
  // beq cr6,0x822ae9b8
  if (cr6.eq) goto loc_822AE9B8;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822AE9B8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AE9B8:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // rlwinm. r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822ae9cc
  if (!cr0.eq) goto loc_822AE9CC;
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // b 0x822ae990
  goto loc_822AE990;
loc_822AE9CC:
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
  return r3;
}

// sub_822AE9E8: lifted from generated/ by lift.py.
uint64_t f822AE9E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_822AE9E8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // mr r29,r4
  r29 = r4;
  // lwz r9,28(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 28);
  // mr r28,r6
  r28 = r6;
  // rlwinm r8,r11,27,27,31
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1F;
  // lhz r11,0(r5)
  r11 = REX_LOAD_U16(uint32_t(r5) + 0);
  // lwz r10,24(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 24);
  // clrlwi r11,r11,20
  r11 = uint32_t(r11) & 0xFFF;
  // add r11,r11,r8
  r11 = r11 + r8;
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r30,r11,r3
  r30 = r11 + r3;
loc_822AEA2C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // clrlwi r11,r11,20
  r11 = uint32_t(r11) & 0xFFF;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r31,r11,r29
  r31 = r11 + r29;
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // beq cr6,0x822aea58
  if (cr6.eq) goto loc_822AEA58;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822AEA58, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AEA58:
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // rlwimi r10,r11,24,28,31
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xF) | (r10 & 0xFFFFFFFFFFFFFFF0);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // beq cr6,0x822aea84
  if (cr6.eq) goto loc_822AEA84;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822AEA84, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AEA84:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // rlwinm. r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822aea98
  if (!cr0.eq) goto loc_822AEA98;
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // b 0x822aea2c
  goto loc_822AEA2C;
loc_822AEA98:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_822AEAA0: lifted from generated/ by lift.py.
uint64_t f822AEAA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_822AEAA0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r23,r5
  r23 = r5;
  // mr r22,r6
  r22 = r6;
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // clrlwi r11,r11,29
  r11 = uint32_t(r11) & 0x7;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // beq cr6,0x822aebe8
  if (cr6.eq) goto loc_822AEBE8;
  // lwz r10,16(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 16);
  // rlwinm. r11,r10,0,13,13
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822aebe8
  if (!cr0.eq) goto loc_822AEBE8;
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x822aeae8
  if (cr6.eq) goto loc_822AEAE8;
  // lwz r11,16(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 16);
  // rlwinm. r11,r11,0,14,14
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822aebe8
  if (!cr0.eq) goto loc_822AEBE8;
loc_822AEAE8:
  // rlwinm r26,r10,27,27,31
  r26 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1F;
  // lwz r11,28(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28);
  // lwz r10,24(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 24);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,10
  r11 = uint64_t(int64_t(int64_t(r11) + 10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r31,r11,r30
  r31 = r11 + r30;
  // beq cr6,0x822aeb2c
  if (cr6.eq) goto loc_822AEB2C;
  // lwz r11,16(r7)
  r11 = REX_LOAD_U32(uint32_t(r7) + 16);
  // addi r28,r7,32
  r28 = uint64_t(int64_t(int64_t(r7) + 32));
  // clrlwi. r24,r11,27
  r24 = uint32_t(r11) & 0x1F;
  cr0 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // rlwinm r11,r11,27,27,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1F;
  // beq 0x822aeb38
  if (cr0.eq) goto loc_822AEB38;
  // addi r24,r24,-1
  r24 = uint64_t(int64_t(int64_t(r24) + -1));
  // b 0x822aeb38
  goto loc_822AEB38;
loc_822AEB2C:
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r24,0
  r24 = uint64_t(int64_t(0));
loc_822AEB38:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822aebb4
  if (cr6.eq) goto loc_822AEBB4;
  // mr r25,r11
  r25 = r11;
loc_822AEB44:
  // lbz r27,3(r28)
  r27 = REX_LOAD_U8(uint32_t(r28) + 3);
  // b 0x822aeb70
  goto loc_822AEB70;
loc_822AEB4C:
  // lbz r11,3(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 3);
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // bge cr6,0x822aeb78
  if (!cr6.lt) goto loc_822AEB78;
  // mr r5,r31
  r5 = r31;
  // mr r4,r23
  r4 = r23;
  // mr r3,r30
  r3 = r30;
  // bl 0x822ae918
  // (left out: dead in C++)
  r3 = f822AE918(base, r1, r13, r3, r4, r5);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
loc_822AEB70:
  // cmplw cr6,r29,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r26), so);
  // blt cr6,0x822aeb4c
  if (cr6.lt) goto loc_822AEB4C;
loc_822AEB78:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,0(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 0);
  // xor r11,r11,r10
  r11 = r11 ^ r10;
  // rlwinm. r11,r11,0,20,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xF00;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822aeba0
  if (cr0.eq) goto loc_822AEBA0;
  // mr r6,r28
  r6 = r28;
  // mr r5,r31
  r5 = r31;
  // mr r4,r23
  r4 = r23;
  // mr r3,r30
  r3 = r30;
  // bl 0x822ae9e8
  // (left out: dead in C++)
  r3 = f822AE9E8(base, r1, r13, r3, r4, r5, r6);
loc_822AEBA0:
  // addic. r25,r25,-1
  ca = uint32_t(r25) > 0;
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // bne 0x822aeb44
  if (!cr0.eq) goto loc_822AEB44;
loc_822AEBB4:
  // cmplw cr6,r29,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r26), so);
  // bge cr6,0x822aebdc
  if (!cr6.lt) goto loc_822AEBDC;
  // subf r29,r29,r26
  r29 = r26 - r29;
loc_822AEBC0:
  // mr r5,r31
  r5 = r31;
  // mr r4,r23
  r4 = r23;
  // mr r3,r30
  r3 = r30;
  // bl 0x822ae918
  // (left out: dead in C++)
  r3 = f822AE918(base, r1, r13, r3, r4, r5);
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x822aebc0
  if (!cr0.eq) goto loc_822AEBC0;
loc_822AEBDC:
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // rlwimi r11,r24,20,8,11
  r11 = (__builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 20) & 0xF00000) | (r11 & 0xFFFFFFFFFF0FFFFF);
  // stw r11,0(r22)
  REX_STORE_U32(uint32_t(r22) + 0, uint32_t(r11));
loc_822AEBE8:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return r3;
}

// sub_822AEBF0: lifted from generated/ by lift.py.
lifted::Result f822AEBF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822AEBF0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r5
  r26 = r5;
  // mr r24,r4
  r24 = r4;
  // mr r31,r3
  r31 = r3;
  // mr r22,r6
  r22 = r6;
  // mr r23,r7
  r23 = r7;
  // lwz r28,600(r26)
  r28 = REX_LOAD_U32(uint32_t(r26) + 600);
  // mr r21,r8
  r21 = r8;
  // mr r20,r9
  r20 = r9;
  // rlwinm r29,r28,30,2,31
  r29 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 30) & 0x3FFFFFFF;
  // addi r4,r29,3
  r4 = uint64_t(int64_t(int64_t(r29) + 3));
  // bl 0x8229f148
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F148(base, LIFTED_ARGS(0x822AEC28, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822aed0c
  if (cr0.eq) goto loc_822AED0C;
  // lis r10,-16384
  r10 = uint64_t(int64_t(-1073741824));
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // ori r10,r10,11008
  r10 = r10 | 11008;
  // clrlwi r9,r29,18
  r9 = uint32_t(r29) & 0x3FFF;
  // rlwimi r10,r11,16,2,15
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0x3FFF0000) | (r10 & 0xFFFFFFFFC000FFFF);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r11,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r11));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r9));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,48(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 48);
  // mr r25,r3
  r25 = r3;
  // rlwinm r9,r11,12,20,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0xFFF;
  // clrlwi r10,r11,3
  r10 = uint32_t(r11) & 0x1FFFFFFF;
  // addi r11,r9,512
  r11 = uint64_t(int64_t(int64_t(r9) + 512));
  // addi r27,r25,4
  r27 = uint64_t(int64_t(int64_t(r25) + 4));
  // rlwinm r11,r11,0,19,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x1000;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addis r30,r11,-16384
  r30 = uint64_t(int64_t(int64_t(r11) + -1073741824));
  // mr r5,r28
  r5 = r28;
  // mr r4,r30
  r4 = r30;
  // mr r3,r27
  r3 = r27;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822AEC8C, r31)); r3 = o.r3; f1 = o.f1; }
  // add r4,r30,r28
  r4 = r30 + r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x822ac470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AC470(base, LIFTED_ARGS(0x822AEC98, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r24,0
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 0, so);
  // beq cr6,0x822aecb8
  if (cr6.eq) goto loc_822AECB8;
  // mr r7,r20
  r7 = r20;
  // mr r6,r22
  r6 = r22;
  // mr r5,r27
  r5 = r27;
  // mr r4,r21
  r4 = r21;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822aeaa0
  // (left out: dead in C++)
  r3 = f822AEAA0(base, r1, r13, r3, r4, r5, r6, r7);
loc_822AECB8:
  // cntlzw r11,r24
  r11 = uint32_t(r24) == 0 ? 32 : __builtin_clz(uint32_t(r24));
  // lbz r10,10810(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 10810);
  // addi r30,r31,12272
  r30 = uint64_t(int64_t(int64_t(r31) + 12272));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // clrlwi r10,r10,25
  r10 = uint32_t(r10) & 0x7F;
  // rlwinm r11,r11,7,24,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0x80;
  // mr r5,r23
  r5 = r23;
  // xori r11,r11,128
  r11 = r11 ^ 128;
  // mr r4,r27
  r4 = r27;
  // or r11,r10,r11
  r11 = r10 | r11;
  // mr r3,r26
  r3 = r26;
  // mr r6,r30
  r6 = r30;
  // stb r11,10810(r31)
  REX_STORE_U8(uint32_t(r31) + 10810, uint8_t(r11));
  // bl 0x822ae340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822AE340(base, LIFTED_ARGS(0x822AECF0, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // ld r10,0(r30)
  r10 = REX_LOAD_U64(uint32_t(r30) + 0);
  // ld r9,12280(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 12280);
  // add r11,r11,r25
  r11 = r11 + r25;
  // std r10,11568(r31)
  REX_STORE_U64(uint32_t(r31) + 11568, r10);
  // std r9,11576(r31)
  REX_STORE_U64(uint32_t(r31) + 11576, r9);
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
loc_822AED0C:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822AF2D8: lifted from generated/ by lift.py.
uint64_t f822AF2D8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822AF2D8 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r3,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 30, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 31, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 32, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,36
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 36, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,37
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 37, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,38
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 38, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,57
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 57, so);
  // beq cr6,0x822af31c
  if (cr6.eq) goto loc_822AF31C;
  // cmplwi cr6,r3,63
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 63, so);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bnelr cr6
  if (!cr6.eq) return r3;
loc_822AF31C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_822AF3D0: lifted from generated/ by lift.py.
uint64_t f822AF3D0(uint8_t* base, uint64_t r1, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_822AF3D0 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // stfs f1,20(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 20, temp.u32);
  // lwz r10,20(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 20);
  // rlwinm. r11,r10,0,0,0
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822af3e8
  if (cr0.eq) goto loc_822AF3E8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x822af42c
  goto loc_822AF42C;
loc_822AF3E8:
  // rlwinm r11,r10,9,23,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 9) & 0x1FF;
  // cmplwi cr6,r11,113
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 113, so);
  // blt cr6,0x822af408
  if (cr6.lt) goto loc_822AF408;
  // rlwinm r11,r10,0,5,8
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x7800000;
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // or r11,r11,r10
  r11 = r11 | r10;
  // rlwinm r11,r11,29,3,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 29) & 0x1FFFFFFF;
  // b 0x822af42c
  goto loc_822AF42C;
loc_822AF408:
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // clrlwi r9,r10,9
  r9 = uint32_t(r10) & 0x7FFFFF;
  // subfic r11,r11,113
  ca = uint32_t(r11) <= 113;
  r11 = static_cast<uint64_t>(113) - r11;
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // slw r10,r8,r10
  r10 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r8) << (uint8_t(r10) & 0x3F));
  // add r10,r10,r9
  r10 = r10 + r9;
  // oris r10,r10,128
  r10 = r10 | 8388608;
  // srw r11,r10,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r10) >> (uint8_t(r11) & 0x3F));
loc_822AF42C:
  // mr r3,r4
  r3 = r4;
  // rlwimi r3,r11,8,0,23
  r3 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00) | (r3 & 0xFFFFFFFF000000FF);
  // blr
  return r3;
}

// sub_822AF8A0: lifted from generated/ by lift.py.
lifted::Result f822AF8A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, lifted::Vec v1) {
  ITC_NATIVE_RUNNING("sub_822AF8A0 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x822af8c0
  if (!cr6.eq) goto loc_822AF8C0;
  // addi r11,r29,10244
  r11 = uint64_t(int64_t(int64_t(r29) + 10244));
  // b 0x822af8cc
  goto loc_822AF8CC;
loc_822AF8C0:
  // addi r11,r5,2562
  r11 = uint64_t(int64_t(int64_t(r5) + 2562));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r29
  r11 = r11 + r29;
loc_822AF8CC:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // rlwinm r3,r11,16,28,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xF;
  // bne cr6,0x822af8f0
  if (!cr6.eq) goto loc_822AF8F0;
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // rlwinm r4,r11,12,26,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0x3F;
  // bl 0x822af550
  // (left out: dead in C++)
  { const lifted::Result o = f822AF550(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, v1); r3 = o.r3; f1 = o.f1; }
  // b 0x822af954
  goto loc_822AF954;
loc_822AF8F0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lwz r8,0(r4)
  r8 = REX_LOAD_U32(uint32_t(r4) + 0);
  // addi r9,r11,17096
  r9 = uint64_t(int64_t(int64_t(r11) + 17096));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r10,r11,13416
  r10 = uint64_t(int64_t(int64_t(r11) + 13416));
  // clrlwi r11,r8,26
  r11 = uint32_t(r8) & 0x3F;
  // lbzx r9,r3,r9
  r9 = REX_LOAD_U8(uint32_t(r3) + uint32_t(r9));
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi cr6,r11,54
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 54, so);
  // rotlwi r9,r9,1
  r9 = __builtin_rotateleft32(uint32_t(r9), 1);
  // lbzx r31,r9,r8
  r31 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r8));
  // bne cr6,0x822af924
  if (!cr6.eq) goto loc_822AF924;
  // li r11,7
  r11 = uint64_t(int64_t(7));
loc_822AF924:
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lwz r4,4(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 4);
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lhzx r11,r11,r10
  r11 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r10));
  // rlwinm r3,r11,24,28,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xF;
  // clrlwi r30,r11,24
  r30 = uint32_t(r11) & 0xFF;
  // bl 0x822af550
  // (left out: dead in C++)
  { const lifted::Result o = f822AF550(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1, v1); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // ble cr6,0x822af954
  if (!cr6.gt) goto loc_822AF954;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_822AF954:
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,10672(r29)
  REX_STORE_U32(uint32_t(r29) + 10672, uint32_t(r11));
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // stw r11,10676(r29)
  REX_STORE_U32(uint32_t(r29) + 10676, uint32_t(r11));
  // ld r11,32(r29)
  r11 = REX_LOAD_U64(uint32_t(r29) + 32);
  // ori r11,r11,192
  r11 = r11 | 192;
  // std r11,32(r29)
  REX_STORE_U64(uint32_t(r29) + 32, r11);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B0D18: lifted from generated/ by lift.py.
lifted::Result f822B0D18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B0D18 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // li r4,1413
  r4 = uint64_t(int64_t(1413));
  // mr r30,r3
  r30 = r3;
  // bl 0x8229f148
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229F148(base, LIFTED_ARGS(0x822B0D34, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r8,r11,6433
  r8 = r11 | 6433;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,6439
  r6 = uint64_t(int64_t(6439));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stwu r8,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r8));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r11,r31,514
  r11 = uint64_t(int64_t(int64_t(r31) + 514));
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r7));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r3)
  ea = 4 + uint32_t(r3);
  REX_STORE_U32(ea, uint32_t(r6));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r3
  r10 = r3;
  // stwu r5,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r5));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
loc_822B0D6C:
  // lis r8,2
  r8 = uint64_t(int64_t(131072));
  // lis r7,-16379
  r7 = uint64_t(int64_t(-1073414144));
  // ori r8,r8,39204
  r8 = r8 | 39204;
  // ori r7,r7,17664
  r7 = r7 | 17664;
  // li r6,7
  r6 = uint64_t(int64_t(7));
  // li r5,6436
  r5 = uint64_t(int64_t(6436));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stwu r8,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r8));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // lhz r8,-512(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + -512);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // lhz r31,-514(r11)
  r31 = REX_LOAD_U16(uint32_t(r11) + -514);
  // rotlwi r8,r8,16
  r8 = __builtin_rotateleft32(uint32_t(r8), 16);
  // cmplwi cr6,r9,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 128, so);
  // or r8,r8,r31
  r8 = r8 | r31;
  // li r31,6434
  r31 = uint64_t(int64_t(6434));
  // stwu r8,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r8));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lhz r8,0(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 0);
  // lhz r29,-2(r11)
  r29 = REX_LOAD_U16(uint32_t(r11) + -2);
  // rotlwi r8,r8,16
  r8 = __builtin_rotateleft32(uint32_t(r8), 16);
  // or r8,r8,r29
  r8 = r8 | r29;
  // stwu r8,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r8));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lhz r8,512(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 512);
  // lhz r29,510(r11)
  r29 = REX_LOAD_U16(uint32_t(r11) + 510);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rotlwi r8,r8,16
  r8 = __builtin_rotateleft32(uint32_t(r8), 16);
  // or r8,r8,r29
  r8 = r8 | r29;
  // stwu r8,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r8));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r7));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r6,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r6));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r5));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r4,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r4));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r3,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r3));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r31,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r31));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r10)
  ea = 4 + uint32_t(r10);
  REX_STORE_U32(ea, uint32_t(r9));
  r10 = (r10 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // blt cr6,0x822b0d6c
  if (cr6.lt) goto loc_822B0D6C;
  // stw r10,40(r30)
  REX_STORE_U32(uint32_t(r30) + 40, uint32_t(r10));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B0E08: lifted from generated/ by lift.py.
lifted::Result f822B0E08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822B0E08 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r3,1024
  r11 = uint64_t(int64_t(int64_t(r3) + 1024));
loc_822B0E10:
  // li r9,255
  r9 = uint64_t(int64_t(255));
  // lis r8,3
  r8 = uint64_t(int64_t(196608));
  // divwu r9,r10,r9
  r9 = uint32_t(uint32_t(r9) ? uint32_t(r10) / uint32_t(r9) : 0);
  // ori r8,r8,65280
  r8 = r8 | 65280;
  // rlwinm r9,r9,6,10,25
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 6) & 0x3FFFC0;
  // addi r10,r10,1023
  r10 = uint64_t(int64_t(int64_t(r10) + 1023));
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // sth r9,-1024(r11)
  REX_STORE_U16(uint32_t(r11) + -1024, uint16_t(r9));
  // sth r9,-512(r11)
  REX_STORE_U16(uint32_t(r11) + -512, uint16_t(r9));
  // sth r9,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r9));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // blt cr6,0x822b0e10
  if (cr6.lt) goto loc_822B0E10;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r3
  r4 = r3;
  // b 0x822b0910
  { const lifted::Result o = lifted::entry::e822B0910(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822B0E60: lifted from generated/ by lift.py.
lifted::Result f822B0E60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_822B0E60 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r3,512
  r11 = uint64_t(int64_t(int64_t(r3) + 512));
  // li r7,127
  r7 = uint64_t(int64_t(127));
loc_822B0E6C:
  // divwu r10,r9,r7
  r10 = uint32_t(uint32_t(r7) ? uint32_t(r9) / uint32_t(r7) : 0);
  // addis r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 65536));
  // lis r8,127
  r8 = uint64_t(int64_t(8323072));
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // ori r6,r8,65408
  r6 = r8 | 65408;
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // divwu r8,r9,r7
  r8 = uint32_t(uint32_t(r7) ? uint32_t(r9) / uint32_t(r7) : 0);
  // cmplw cr6,r9,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r6), so);
  // subf r8,r10,r8
  r8 = r8 - r10;
  // clrlwi r8,r8,16
  r8 = uint32_t(r8) & 0xFFFF;
  // sth r10,-512(r11)
  REX_STORE_U16(uint32_t(r11) + -512, uint16_t(r10));
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // sth r10,512(r11)
  REX_STORE_U16(uint32_t(r11) + 512, uint16_t(r10));
  // sth r8,-510(r11)
  REX_STORE_U16(uint32_t(r11) + -510, uint16_t(r8));
  // sth r8,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r8));
  // sth r8,514(r11)
  REX_STORE_U16(uint32_t(r11) + 514, uint16_t(r8));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // blt cr6,0x822b0e6c
  if (cr6.lt) goto loc_822B0E6C;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r3
  r4 = r3;
  // b 0x822b0ab0
  { const lifted::Result o = lifted::entry::e822B0AB0(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_822B0ED0: lifted from generated/ by lift.py.
lifted::Result f822B0ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_822B0ED0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // mr r31,r4
  r31 = r4;
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // cmpldi cr6,r31,0
  cr6 = lifted::Compare<uint64_t>(r31, 0, so);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bne cr6,0x822b0f04
  if (!cr6.eq) goto loc_822B0F04;
  // lfs f1,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
  // b 0x822b0f24
  goto loc_822B0F24;
loc_822B0F04:
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B0F08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // lfd f0,17376(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 17376)));
  // fmul f31,f1,f0
  f31 = f1 * f0;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B0F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // fdiv f0,f31,f1
  lifted::ScalarFloatMode();
  f0 = f31 / f1;
  // frsp f1,f0
  f1 = double(float(f0));
loc_822B0F24:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822B0F40: lifted from generated/ by lift.py.
uint64_t f822B0F40(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_822B0F40 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r10,15632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 15632);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x822b0f60
  if (!cr6.eq) goto loc_822B0F60;
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,15632(r11)
  REX_STORE_U32(uint32_t(r11) + 15632, uint32_t(r10));
  // blr
  return r3;
loc_822B0F60:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
}

// sub_822B0F70: lifted from generated/ by lift.py.
lifted::Result f822B0F70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B0F70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // lwz r11,20116(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20116);
  // lwz r10,20024(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 20024);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x822b0fa0
  if (!cr6.eq) goto loc_822B0FA0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x822b0fc0
  goto loc_822B0FC0;
loc_822B0FA0:
  // lwz r11,20104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20104);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x822b0fd8
  if (cr6.eq) goto loc_822B0FD8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822b0fbc
  if (!cr6.eq) goto loc_822B0FBC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,20104(r30)
  REX_STORE_U32(uint32_t(r30) + 20104, uint32_t(r11));
loc_822B0FBC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_822B0FC0:
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
loc_822B0FD8:
  // lwz r10,20112(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 20112);
  // lwz r11,20108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20108);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x822b0fbc
  if (cr6.eq) goto loc_822B0FBC;
  // addi r11,r11,5021
  r11 = uint64_t(int64_t(int64_t(r11) + 5021));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r30
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r30));
  // bl 0x8229bfd0
  // (left out: dead in C++)
  { const lifted::Result o = f8229BFD0(base, r1, r13, r3, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x822b0fbc
  if (!cr0.eq) goto loc_822B0FBC;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // li r5,480
  r5 = uint64_t(int64_t(480));
  // addi r31,r11,14412
  r31 = uint64_t(int64_t(int64_t(r11) + 14412));
  // addi r4,r31,484
  r4 = uint64_t(int64_t(int64_t(r31) + 484));
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x822B1018, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20108);
  // addi r4,r31,484
  r4 = uint64_t(int64_t(int64_t(r31) + 484));
  // addi r11,r11,5021
  r11 = uint64_t(int64_t(int64_t(r11) + 5021));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r3,r10,r30
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r30));
  // stw r11,-4(r31)
  REX_STORE_U32(uint32_t(r31) + -4, uint32_t(r11));
  // bl 0x8229d688
  // (left out: dead in C++)
  { const lifted::Result o = f8229D688(base, r1, r13, r3, r4, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 20108);
  // lwz r10,20100(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 20100);
  // rlwinm r9,r11,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // bne cr6,0x822b1070
  if (!cr6.eq) goto loc_822B1070;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x822b1074
  goto loc_822B1074;
loc_822B1070:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_822B1074:
  // lwz r10,20024(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 20024);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,20108(r30)
  REX_STORE_U32(uint32_t(r30) + 20108, uint32_t(r11));
  // stw r10,20116(r30)
  REX_STORE_U32(uint32_t(r30) + 20116, uint32_t(r10));
  // b 0x822b0fc0
  goto loc_822B0FC0;
}

// sub_822B1088: lifted from generated/ by lift.py.
lifted::Result f822B1088(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B1088 (lifted)");
  uint64_t r0 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // stfd f30,-40(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f30));
  // stfd f31,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // cmplwi cr6,r4,17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 17, so);
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bgt cr6,0x822b16f4
  if (cr6.gt) goto loc_822B16F4;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,17384
  r12 = uint64_t(int64_t(int64_t(r12) + 17384));
  // rlwinm r0,r4,1,0,30
  r0 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 1) & 0xFFFFFFFE;
  // lhzx r0,r12,r0
  r0 = REX_LOAD_U16(uint32_t(r12) + uint32_t(r0));
  // lis r12,-32213
  r12 = uint64_t(int64_t(-2111111168));
  // addi r12,r12,4320
  r12 = uint64_t(int64_t(int64_t(r12) + 4320));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r4)) {
  case 0:
  goto loc_frag_822B10E0;
  return {r3, f1};
  case 1:
  goto loc_822B1100;
  case 2:
  goto loc_822B112C;
  case 3:
  goto loc_822B1144;
  case 4:
  goto loc_822B11A8;
  case 5:
  goto loc_822B11C4;
  case 6:
  goto loc_822B1204;
  case 7:
  goto loc_822B12A8;
  case 8:
  goto loc_822B1348;
  case 9:
  goto loc_822B13C8;
  case 10:
  goto loc_822B1470;
  case 11:
  goto loc_822B14CC;
  case 12:
  goto loc_822B1558;
  case 13:
  goto loc_822B15B8;
  case 14:
  goto loc_822B160C;
  case 15:
  goto loc_822B1660;
  case 16:
  goto loc_822B16C4;
  case 17:
  goto loc_822B1298;
  default:
  std::abort();
  }
loc_822B1100:
  // lwz r11,20028(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20028);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // lfs f0,20040(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20040);
  f0 = double(temp.f32);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f1,f0,f13
  f1 = double(float(f0 / f13));
  // b 0x822b16f8
  goto loc_822B16F8;
loc_822B112C:
  // lwz r11,20024(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20024);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
loc_822B1138:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f1,f0
  f1 = double(float(f0));
  // b 0x822b16f8
  goto loc_822B16F8;
loc_822B1144:
  // lwz r11,20064(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20064);
  // lwz r10,20028(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20028);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
loc_822B115C:
  // fcfid f13,f13
  lifted::ScalarFloatMode();
  f13 = double(std::bit_cast<int64_t>(f13));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f0,f0
  f0 = double(float(f0));
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
loc_822B1170:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10764(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // lfs f12,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bge cr6,0x822b1194
  if (!cr6.lt) goto loc_822B1194;
  // fmr f0,f12
  f0 = f12;
  // b 0x822b11a0
  goto loc_822B11A0;
loc_822B1194:
  // fcmpu cr6,f13,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x822b11a0
  if (cr6.gt) goto loc_822B11A0;
  // fmr f0,f13
  f0 = f13;
loc_822B11A0:
  // fmr f1,f0
  lifted::ScalarFloatMode();
  f1 = f0;
  // b 0x822b16f8
  goto loc_822B16F8;
loc_822B11A8:
  // lwz r11,20068(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20068);
  // lwz r10,20028(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20028);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // b 0x822b115c
  goto loc_822B115C;
loc_822B11C4:
  // lwz r11,20028(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20028);
  // lwz r10,20068(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 20068);
  // lwz r9,20064(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 20064);
  // subf r10,r10,r11
  r10 = r11 - r10;
  // subf r10,r9,r10
  r10 = r10 - r9;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // clrldi r10,r10,32
  r10 = r10 & 0xFFFFFFFF;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f0,f0
  f0 = double(float(f0));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f13,f13,f0
  f13 = double(float(f13 / f0));
  // b 0x822b1170
  goto loc_822B1170;
loc_822B1204:
  // lwz r11,20072(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20072);
  // lwz r10,10768(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 10768);
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r7,r11,-1
  r7 = uint64_t(int64_t(int64_t(r11) + -1));
  // clrlwi r9,r9,29
  r9 = uint32_t(r9) & 0x7;
  // clrlwi r8,r11,29
  r8 = uint32_t(r11) & 0x7;
  // addi r6,r9,16
  r6 = uint64_t(int64_t(int64_t(r9) + 16));
  // clrlwi r9,r7,29
  r9 = uint32_t(r7) & 0x7;
  // addi r8,r8,16
  r8 = uint64_t(int64_t(int64_t(r8) + 16));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // rlwinm r7,r6,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r6,r9,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwbrx r31,r7,r10
  r31 = __builtin_bswap32(REX_LOAD_U32(uint32_t(r7) + uint32_t(r10)));
  // lwbrx r8,r8,r10
  r8 = __builtin_bswap32(REX_LOAD_U32(uint32_t(r8) + uint32_t(r10)));
  // lwbrx r30,r6,r10
  r30 = __builtin_bswap32(REX_LOAD_U32(uint32_t(r6) + uint32_t(r10)));
  // cmpldi cr6,r31,0
  cr6 = lifted::Compare<uint64_t>(r31, 0, so);
  // mr r10,r8
  r10 = r8;
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmpldi cr6,r10,0
  cr6 = lifted::Compare<uint64_t>(r10, 0, so);
  // stw r11,20072(r3)
  REX_STORE_U32(uint32_t(r3) + 20072, uint32_t(r11));
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // cmpldi cr6,r30,0
  cr6 = lifted::Compare<uint64_t>(r30, 0, so);
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // cmpld cr6,r10,r30
  cr6 = lifted::Compare<uint64_t>(r10, r30, so);
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // bgt cr6,0x822b127c
  if (cr6.gt) goto loc_822B127C;
  // add r10,r10,r11
  r10 = r10 + r11;
loc_822B127C:
  // cmpld cr6,r31,r10
  cr6 = lifted::Compare<uint64_t>(r31, r10, so);
  // bgt cr6,0x822b1288
  if (cr6.gt) goto loc_822B1288;
  // add r31,r31,r11
  r31 = r31 + r11;
loc_822B1288:
  // subf r3,r10,r31
  r3 = r31 - r10;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B1290, r31)); r3 = o.r3; f1 = o.f1; }
  // subf r3,r30,r31
  r3 = r31 - r30;
  // b 0x822b13b4
  goto loc_822B13B4;
loc_822B1298:
  // lwz r11,20120(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20120);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // b 0x822b1138
  goto loc_822B1138;
loc_822B12A8:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r31,r11,14412
  r31 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r9,496(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 496);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r10,492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 492);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // ld r10,88(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 88);
  // subf r3,r10,r11
  r3 = r11 - r10;
  // cmpldi cr6,r3,0
  cr6 = lifted::Compare<uint64_t>(r3, 0, so);
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B1304, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r9,488(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 488);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r10,484(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 484);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // subf r3,r11,r10
  r3 = r10 - r11;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B1330, r31)); r3 = o.r3; f1 = o.f1; }
  // fdiv f0,f31,f1
  lifted::ScalarFloatMode();
  f0 = f31 / f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fsubs f13,f0,f13
  f13 = double(float(f0 - f13));
  // b 0x822b1170
  goto loc_822B1170;
loc_822B1348:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,124(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 124);
  // lwz r9,128(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 128);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,608(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 608);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r31,r10,r9
  r31 = r9 - r10;
  // cmpldi cr6,r31,0
  cr6 = lifted::Compare<uint64_t>(r31, 0, so);
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // lwz r10,132(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 132);
  // lwz r9,136(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 136);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,612(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 612);
  // lwz r11,616(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 616);
loc_822B13A0:
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r11,r9
  r11 = r11 | r9;
  // subf r3,r10,r11
  r3 = r11 - r10;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B13B0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_822B13B4:
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B13BC, r31)); r3 = o.r3; f1 = o.f1; }
  // fdiv f0,f31,f1
  lifted::ScalarFloatMode();
  f0 = f31 / f1;
loc_822B13C0:
  // frsp f13,f0
  lifted::ScalarFloatMode();
  f13 = double(float(f0));
  // b 0x822b1170
  goto loc_822B1170;
loc_822B13C8:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r31,r11,14412
  r31 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r11,124(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 124);
  // lwz r10,128(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 128);
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r9,608(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 608);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r10,604(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 604);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // subf r3,r11,r10
  r3 = r10 - r11;
  // cmpldi cr6,r3,0
  cr6 = lifted::Compare<uint64_t>(r3, 0, so);
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B140C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,132(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 132);
  // lwz r10,136(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 136);
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r9,616(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 616);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r10,612(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 612);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // subf r3,r11,r10
  r3 = r10 - r11;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B1438, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,140(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 140);
  // lwz r10,144(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 144);
  // fsub f30,f31,f1
  lifted::ScalarFloatMode();
  f30 = f31 - f1;
  // rldicr r11,r11,32,63
  r11 = __builtin_rotateleft64(r11, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r9,624(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 624);
  // or r11,r10,r11
  r11 = r10 | r11;
  // lwz r10,620(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 620);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // subf r3,r11,r10
  r3 = r10 - r11;
  // bl 0x825a9db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A9DB0(base, LIFTED_ARGS(0x822B1464, r31)); r3 = o.r3; f1 = o.f1; }
  // fsub f0,f30,f1
  lifted::ScalarFloatMode();
  f0 = f30 - f1;
  // fdiv f0,f0,f31
  f0 = f0 / f31;
  // b 0x822b13c0
  goto loc_822B13C0;
loc_822B1470:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,124(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 124);
  // lwz r9,128(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 128);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,608(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 608);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r31,r10,r9
  r31 = r9 - r10;
  // cmpldi cr6,r31,0
  cr6 = lifted::Compare<uint64_t>(r31, 0, so);
  // beq cr6,0x822b16f4
  if (cr6.eq) goto loc_822B16F4;
  // lwz r10,140(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 140);
  // lwz r9,144(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 144);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,620(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 620);
  // lwz r11,624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 624);
  // b 0x822b13a0
  goto loc_822B13A0;
loc_822B14CC:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,444(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 444);
  // lwz r9,448(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 448);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,928(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 928);
  // lwz r7,488(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 488);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,924(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 924);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r8,r10,r9
  r8 = r9 - r10;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,484(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 484);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r7,r9
  r9 = r7 | r9;
  // subf r4,r10,r9
  r4 = r9 - r10;
  // lwz r10,436(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 436);
  // lwz r9,440(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 440);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,916(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 916);
  // lwz r11,920(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 920);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // subf r10,r10,r8
  r10 = r8 - r10;
  // or r11,r11,r9
  r11 = r11 | r9;
  // add r3,r10,r11
  r3 = r10 + r11;
loc_822B1550:
  // bl 0x822b0ed0
  // (left out: dead in C++)
  { const lifted::Result o = f822B0ED0(base, r1, r13, r3, r4); r3 = o.r3; f1 = o.f1; }
  // b 0x822b16f8
  goto loc_822B16F8;
loc_822B1558:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,488(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 488);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,484(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 484);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r4,r10,r9
  r4 = r9 - r10;
  // lwz r10,452(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 452);
  // lwz r9,456(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 456);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,932(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 932);
  // lwz r11,936(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 936);
loc_822B15A8:
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r11,r9
  r11 = r11 | r9;
  // subf r3,r10,r11
  r3 = r11 - r10;
  // b 0x822b1550
  goto loc_822B1550;
loc_822B15B8:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,488(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 488);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,484(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 484);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r4,r10,r9
  r4 = r9 - r10;
  // lwz r10,236(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 236);
  // lwz r9,240(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 240);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,716(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 716);
  // lwz r11,720(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 720);
  // b 0x822b15a8
  goto loc_822B15A8;
loc_822B160C:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,488(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 488);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,484(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 484);
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // subf r4,r10,r9
  r4 = r9 - r10;
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // lwz r9,96(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 96);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,572(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 572);
  // lwz r11,576(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 576);
  // b 0x822b15a8
  goto loc_822B15A8;
loc_822B1660:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,460(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 460);
  // lwz r9,464(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 464);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,944(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 944);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,940(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 940);
loc_822B168C:
  // rldicr r9,r9,32,63
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r9,r8,r9
  r9 = r8 | r9;
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // rldicr r3,r9,1,62
  r3 = __builtin_rotateleft64(r9, 1) & 0xFFFFFFFFFFFFFFFE;
  // or r10,r8,r10
  r10 = r8 | r10;
  // lwz r8,484(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 484);
  // lwz r11,488(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 488);
  // rldicr r8,r8,32,63
  r8 = __builtin_rotateleft64(r8, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r11,r8
  r11 = r11 | r8;
  // subf r4,r10,r11
  r4 = r11 - r10;
  // b 0x822b1550
  goto loc_822B1550;
loc_822B16C4:
  // bl 0x822b0f70
  // (left out: dead in C++)
  { const lifted::Result o = f822B0F70(base, r1, r13, r3, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822b16f4
  if (cr0.eq) goto loc_822B16F4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r11,r11,14412
  r11 = uint64_t(int64_t(int64_t(r11) + 14412));
  // lwz r10,468(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 468);
  // lwz r9,472(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 472);
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // lwz r8,952(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 952);
  // or r10,r9,r10
  r10 = r9 | r10;
  // lwz r9,948(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 948);
  // b 0x822b168c
  goto loc_822B168C;
loc_822B16F4:
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
loc_822B16F8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-40(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_frag_822B10E0:
  // lwz r11,20028(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20028);
  // lfs f0,20044(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 20044);
  f0 = double(temp.f32);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // b 0x822b16f8
  goto loc_822B16F8_822B10E0;
loc_822B16F8_822B10E0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-40(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_822B1AC8: lifted from generated/ by lift.py.
lifted::Result f822B1AC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822B1AC8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r4
  r31 = r4;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x822b1af0
  if (!cr6.eq) goto loc_822B1AF0;
  // lis r3,-32038
  r3 = uint64_t(int64_t(-2099642368));
  // ori r3,r3,23
  r3 = r3 | 23;
  // b 0x822b1c9c
  goto loc_822B1C9C;
loc_822B1AF0:
  // lbz r11,4(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 4);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,103
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 103, so);
  // bgt cr6,0x822b1c2c
  if (cr6.gt) goto loc_822B1C2C;
  // beq cr6,0x822b1c24
  if (cr6.eq) goto loc_822B1C24;
  // cmpwi cr6,r11,97
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 97, so);
  // beq cr6,0x822b1bc8
  if (cr6.eq) goto loc_822B1BC8;
  // cmpwi cr6,r11,99
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 99, so);
  // beq cr6,0x822b1b94
  if (cr6.eq) goto loc_822B1B94;
  // cmpwi cr6,r11,100
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 100, so);
  // beq cr6,0x822b1b6c
  if (cr6.eq) goto loc_822B1B6C;
  // cmpwi cr6,r11,102
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 102, so);
  // bne cr6,0x822b1c98
  if (!cr6.eq) goto loc_822B1C98;
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_822B1B28:
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,-28504
  r11 = uint64_t(int64_t(int64_t(r11) + -28504));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r10,15632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 15632);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x822b1b5c
  if (!cr6.eq) goto loc_822B1B5C;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r4,r10,17744
  r4 = uint64_t(int64_t(int64_t(r10) + 17744));
  // li r10,13
  r10 = uint64_t(int64_t(13));
loc_822B1B54:
  // stw r10,15632(r11)
  REX_STORE_U32(uint32_t(r11) + 15632, uint32_t(r10));
  // b 0x822b1b64
  goto loc_822B1B64;
loc_822B1B5C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17736
  r4 = uint64_t(int64_t(int64_t(r11) + 17736));
loc_822B1B64:
  // bl 0x825dba64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__sprintf, LIFTED_ARGS(0x822B1B68, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b1c98
  goto loc_822B1C98;
loc_822B1B6C:
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // mr r3,r31
  r3 = r31;
  // lwz r11,15632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 15632);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822b1b5c
  if (!cr6.eq) goto loc_822B1B5C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17744
  r4 = uint64_t(int64_t(int64_t(r11) + 17744));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,15632(r10)
  REX_STORE_U32(uint32_t(r10) + 15632, uint32_t(r11));
  // b 0x822b1b64
  goto loc_822B1B64;
loc_822B1B94:
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r11,r11,-28504
  r11 = uint64_t(int64_t(int64_t(r11) + -28504));
  // mr r3,r31
  r3 = r31;
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r10,15632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 15632);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x822b1b5c
  if (!cr6.eq) goto loc_822B1B5C;
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // addi r4,r10,17744
  r4 = uint64_t(int64_t(int64_t(r10) + 17744));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // b 0x822b1b54
  goto loc_822B1B54;
loc_822B1BC8:
  // lis r9,-32107
  r9 = uint64_t(int64_t(-2104164352));
  // lwz r11,15632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 15632);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822b1c14
  if (!cr6.eq) goto loc_822B1C14;
  // lbz r11,5(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 5);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,48
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 48, so);
  // blt cr6,0x822b1bf4
  if (cr6.lt) goto loc_822B1BF4;
  // cmpwi cr6,r11,57
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 57, so);
  // addi r11,r11,-47
  r11 = uint64_t(int64_t(int64_t(r11) + -47));
  // ble cr6,0x822b1bf8
  if (!cr6.gt) goto loc_822B1BF8;
loc_822B1BF4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_822B1BF8:
  // lis r10,-32156
  r10 = uint64_t(int64_t(-2107375616));
  // stw r11,-28504(r10)
  REX_STORE_U32(uint32_t(r10) + -28504, uint32_t(r11));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17744
  r4 = uint64_t(int64_t(int64_t(r11) + 17744));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,15632(r9)
  REX_STORE_U32(uint32_t(r9) + 15632, uint32_t(r11));
  // b 0x822b1c1c
  goto loc_822B1C1C;
loc_822B1C14:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r4,r11,17736
  r4 = uint64_t(int64_t(int64_t(r11) + 17736));
loc_822B1C1C:
  // mr r3,r31
  r3 = r31;
  // b 0x822b1b64
  goto loc_822B1B64;
loc_822B1C24:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // b 0x822b1b28
  goto loc_822B1B28;
loc_822B1C2C:
  // cmpwi cr6,r11,109
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 109, so);
  // beq cr6,0x822b1c90
  if (cr6.eq) goto loc_822B1C90;
  // cmpwi cr6,r11,112
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 112, so);
  // beq cr6,0x822b1c48
  if (cr6.eq) goto loc_822B1C48;
  // cmpwi cr6,r11,116
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 116, so);
  // bne cr6,0x822b1c98
  if (!cr6.eq) goto loc_822B1C98;
  // b 0x822b1c14
  goto loc_822B1C14;
loc_822B1C48:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,2032(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 2032);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b1c78
  if (cr0.eq) goto loc_822B1C78;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,58
  r3 = uint64_t(int64_t(58));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B1C70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // b 0x822b1c7c
  goto loc_822B1C7C;
loc_822B1C78:
  // li r5,0
  r5 = uint64_t(int64_t(0));
loc_822B1C7C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,17724
  r4 = uint64_t(int64_t(int64_t(r11) + 17724));
  // bl 0x825dba64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__sprintf, LIFTED_ARGS(0x822B1C8C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x822b1c98
  goto loc_822B1C98;
loc_822B1C90:
  // mr r4,r31
  r4 = r31;
  // bl 0x822b43e8
  // (left out: dead in C++)
  { const lifted::Result o = f822B43E8(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822B1C98:
  // lis r3,730
  r3 = uint64_t(int64_t(47841280));
loc_822B1C9C:
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

// sub_822B1CB0: lifted from generated/ by lift.py.
lifted::Result f822B1CB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B1CB0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r11,1968(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1968);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x822b1e64
  if (cr6.eq) goto loc_822B1E64;
  // rotlwi r31,r10,0
  r31 = __builtin_rotateleft32(uint32_t(r10), 0);
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b1e64
  if (cr6.eq) goto loc_822B1E64;
  // ld r11,10752(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 10752);
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822b1e64
  if (cr6.eq) goto loc_822B1E64;
  // cmplwi cr6,r3,224
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 224, so);
  // bgt cr6,0x822b1d90
  if (cr6.gt) goto loc_822B1D90;
  // beq cr6,0x822b1e2c
  if (cr6.eq) goto loc_822B1E2C;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x822b1d80
  if (cr6.eq) goto loc_822B1D80;
  // cmplwi cr6,r3,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 16, so);
  // beq cr6,0x822b1d54
  if (cr6.eq) goto loc_822B1D54;
  // cmplwi cr6,r3,17
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 17, so);
  // beq cr6,0x822b1d20
  if (cr6.eq) goto loc_822B1D20;
  // cmplwi cr6,r3,34
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 34, so);
  // bne cr6,0x822b1e64
  if (!cr6.eq) goto loc_822B1E64;
  // stw r4,20124(r31)
  REX_STORE_U32(uint32_t(r31) + 20124, uint32_t(r4));
  // b 0x822b1e64
  goto loc_822B1E64;
loc_822B1D20:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // cmplwi cr6,r4,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 6, so);
  // lwz r10,15636(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 15636);
  // slw r8,r9,r4
  r8 = uint8_t(r4) & 0x20 ? 0 : (uint32_t(r9) << (uint8_t(r4) & 0x3F));
  // or r10,r8,r10
  r10 = r8 | r10;
  // stw r10,15636(r11)
  REX_STORE_U32(uint32_t(r11) + 15636, uint32_t(r10));
  // bne cr6,0x822b1e64
  if (!cr6.eq) goto loc_822B1E64;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r9,20080(r31)
  REX_STORE_U32(uint32_t(r31) + 20080, uint32_t(r9));
  // stw r11,20072(r31)
  REX_STORE_U32(uint32_t(r31) + 20072, uint32_t(r11));
  // stw r11,20076(r31)
  REX_STORE_U32(uint32_t(r31) + 20076, uint32_t(r11));
  // b 0x822b1e64
  goto loc_822B1E64;
loc_822B1D54:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // cmplwi cr6,r4,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 6, so);
  // slw r9,r11,r4
  r9 = uint8_t(r4) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r4) & 0x3F));
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // lwz r10,15636(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 15636);
  // andc r10,r10,r9
  r10 = r10 & ~r9;
  // stw r10,15636(r11)
  REX_STORE_U32(uint32_t(r11) + 15636, uint32_t(r10));
  // bne cr6,0x822b1e64
  if (!cr6.eq) goto loc_822B1E64;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_822B1D78:
  // stw r11,20080(r31)
  REX_STORE_U32(uint32_t(r31) + 20080, uint32_t(r11));
  // b 0x822b1e64
  goto loc_822B1E64;
loc_822B1D80:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32107
  r10 = uint64_t(int64_t(-2104164352));
  // stw r11,15636(r10)
  REX_STORE_U32(uint32_t(r10) + 15636, uint32_t(r11));
  // b 0x822b1d78
  goto loc_822B1D78;
loc_822B1D90:
  // cmplwi cr6,r3,225
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 225, so);
  // beq cr6,0x822b1e2c
  if (cr6.eq) goto loc_822B1E2C;
  // cmplwi cr6,r3,226
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 226, so);
  // beq cr6,0x822b1e2c
  if (cr6.eq) goto loc_822B1E2C;
  // cmplwi cr6,r3,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 255, so);
  // bne cr6,0x822b1e64
  if (!cr6.eq) goto loc_822B1E64;
  // lis r28,-32107
  r28 = uint64_t(int64_t(-2104164352));
  // lwz r11,15636(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 15636);
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822b1de8
  if (cr0.eq) goto loc_822B1DE8;
  // lwz r11,20028(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20028);
  // lfs f0,20044(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20044);
  f0 = double(temp.f32);
  // lis r3,2
  r3 = uint64_t(int64_t(131072));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // lfs f0,7132(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7132);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // bl 0x825b9ba8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9BA8(base, LIFTED_ARGS(0x822B1DE8, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B1DE8:
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // li r29,16
  r29 = uint64_t(int64_t(16));
  // addi r11,r11,-28496
  r11 = uint64_t(int64_t(int64_t(r11) + -28496));
  // addi r30,r11,8
  r30 = uint64_t(int64_t(int64_t(r11) + 8));
loc_822B1DF8:
  // lwz r11,-8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -8);
  // lwz r10,15636(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 15636);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822b1e1c
  if (cr0.eq) goto loc_822B1E1C;
  // mr r3,r31
  r3 = r31;
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x822b1088
  // (left out: dead in C++)
  { const lifted::Result o = f822B1088(base, r1, r13, r3, r4, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -4);
  // bl 0x825b9ba8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9BA8(base, LIFTED_ARGS(0x822B1E1C, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B1E1C:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,12
  r30 = uint64_t(int64_t(int64_t(r30) + 12));
  // bne 0x822b1df8
  if (!cr0.eq) goto loc_822B1DF8;
  // b 0x822b1e64
  goto loc_822B1E64;
loc_822B1E2C:
  // lwz r11,19980(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 19980);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x822b1e64
  if (cr6.eq) goto loc_822B1E64;
  // bl 0x825b9a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9A60(base, LIFTED_ARGS(0x822B1E3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,10760(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10760);
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // bne cr6,0x822b1e64
  if (!cr6.eq) goto loc_822B1E64;
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r3,40(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // ble cr6,0x822b1e60
  if (!cr6.gt) goto loc_822B1E60;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822B1E60, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B1E60:
  // stw r3,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r3));
loc_822B1E64:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822B1E70: lifted from generated/ by lift.py.
lifted::Result f822B1E70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_822B1E70 (lifted)");
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
  // lis r31,-32256
  r31 = uint64_t(int64_t(-2113929216));
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // lwz r11,1972(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1972);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x822b1eb8
  if (cr6.eq) goto loc_822B1EB8;
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x822b1ee4
  if (cr0.eq) goto loc_822B1EE4;
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x822b1ee4
  if (cr0.eq) goto loc_822B1EE4;
  // b 0x822b1ecc
  goto loc_822B1ECC;
loc_822B1EB8:
  // lwz r10,2032(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 2032);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x822b1ee4
  if (cr0.eq) goto loc_822B1EE4;
  // lwz r10,24(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 24);
loc_822B1ECC:
  // lis r11,-32213
  r11 = uint64_t(int64_t(-2111111168));
  // li r3,28
  r3 = uint64_t(int64_t(28));
  // addi r4,r11,6856
  r4 = uint64_t(int64_t(int64_t(r11) + 6856));
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B1EE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,1972(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 1972);
loc_822B1EE4:
  // lis r10,-32213
  r10 = uint64_t(int64_t(-2111111168));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // addi r10,r10,7344
  r10 = uint64_t(int64_t(int64_t(r10) + 7344));
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // lis r9,-32107
  r9 = uint64_t(int64_t(-2104164352));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // stw r10,15636(r9)
  REX_STORE_U32(uint32_t(r9) + 15636, uint32_t(r10));
  // beq cr6,0x822b1f2c
  if (cr6.eq) goto loc_822B1F2C;
  // rotlwi r11,r8,0
  r11 = __builtin_rotateleft32(uint32_t(r8), 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b1f50
  if (cr0.eq) goto loc_822B1F50;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b1f50
  if (cr0.eq) goto loc_822B1F50;
  // b 0x822b1f40
  goto loc_822B1F40;
loc_822B1F2C:
  // lwz r11,2032(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 2032);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x822b1f50
  if (cr0.eq) goto loc_822B1F50;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
loc_822B1F40:
  // li r3,47
  r3 = uint64_t(int64_t(47));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822B1F50, r31)); r3 = o.r3; f1 = o.f1; }
loc_822B1F50:
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

// sub_822B1FF8: lifted from generated/ by lift.py.
lifted::Result f822B1FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822B1FF8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r11,20104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20104);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x822b20bc
  if (!cr6.eq) goto loc_822B20BC;
  // lwz r11,20112(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20112);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x822b2030
  if (!cr6.eq) goto loc_822B2030;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x822b2034
  goto loc_822B2034;
loc_822B2030:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_822B2034:
  // lwz r10,20108(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20108);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x822b20bc
  if (cr6.eq) goto loc_822B20BC;
  // rlwinm r30,r11,1,0,30
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x822b2050
  if (!cr6.eq) goto loc_822B2050;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
loc_822B2050:
  // lwz r10,48(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822b206c
  if (!cr6.gt) goto loc_822B206C;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8229EF48(base, LIFTED_ARGS(0x822B2068, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822B206C:
  // lis r10,-16382
  r10 = uint64_t(int64_t(-1073610752));
  // lis r9,-32768
  r9 = uint64_t(int64_t(-2147483648));
  // ori r10,r10,22528
  r10 = r10 | 22528;
  // ori r8,r9,3
  r8 = r9 | 3;
  // rlwinm r9,r30,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lis r7,-8531
  r7 = uint64_t(int64_t(-559087616));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // ori r7,r7,48879
  r7 = r7 | 48879;
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,20100(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20100);
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // clrlwi r9,r10,3
  r9 = uint32_t(r10) & 0x1FFFFFFF;
  // addi r10,r8,512
  r10 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // add r10,r10,r9
  r10 = r10 + r9;
  // ori r10,r10,2
  r10 = r10 | 2;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  REX_STORE_U32(ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r11,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r11));
loc_822B20BC:
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

namespace {

lifted::Result e822929E8(uint8_t* base, const lifted::Args& a) {
  return f822929E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82292A48(uint8_t* base, const lifted::Args& a) {
  return f82292A48(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e82292B28(uint8_t* base, const lifted::Args& a) {
  return f82292B28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82292DB8(uint8_t* base, const lifted::Args& a) {
  return f82292DB8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82292E98(uint8_t* base, const lifted::Args& a) {
  return f82292E98(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82292FB0(uint8_t* base, const lifted::Args& a) {
  return f82292FB0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82293028(uint8_t* base, const lifted::Args& a) {
  return f82293028(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82293190(uint8_t* base, const lifted::Args& a) {
  return f82293190(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82293288(uint8_t* base, const lifted::Args& a) {
  return f82293288(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82293358(uint8_t* base, const lifted::Args& a) {
  return f82293358(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82293CD8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82293CD8(base, a.r[0], a.r[1], a.r[2])};
}

lifted::Result e82293F68(uint8_t* base, const lifted::Args& a) {
  return f82293F68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e82293FD0(uint8_t* base, const lifted::Args& a) {
  return f82293FD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]);
}

lifted::Result e822945F8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f822945F8(base, a.f[0], a.f[1], a.f[2])};
}

lifted::Result e822947A0(uint8_t* base, const lifted::Args& a) {
  return f822947A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e822947E8(uint8_t* base, const lifted::Args& a) {
  return f822947E8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e82294C88(uint8_t* base, const lifted::Args& a) {
  return f82294C88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82294CF0(uint8_t* base, const lifted::Args& a) {
  return f82294CF0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82294D38(uint8_t* base, const lifted::Args& a) {
  return f82294D38(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82294D48(uint8_t* base, const lifted::Args& a) {
  return f82294D48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82294F78(uint8_t* base, const lifted::Args& a) {
  return f82294F78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e82295018(uint8_t* base, const lifted::Args& a) {
  return f82295018(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]);
}

lifted::Result e822950C0(uint8_t* base, const lifted::Args& a) {
  return f822950C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822951A8(uint8_t* base, const lifted::Args& a) {
  return f822951A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82295208(uint8_t* base, const lifted::Args& a) {
  return f82295208(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82295310(uint8_t* base, const lifted::Args& a) {
  return f82295310(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82295500(uint8_t* base, const lifted::Args& a) {
  return f82295500(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82295700(uint8_t* base, const lifted::Args& a) {
  return {f82295700(base, a.r1, a.r13, a.r[0], a.lr), a.f[0]};
}

lifted::Result e82295C00(uint8_t* base, const lifted::Args& a) {
  return f82295C00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82295E78(uint8_t* base, const lifted::Args& a) {
  return f82295E78(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822967E0(uint8_t* base, const lifted::Args& a) {
  return f822967E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e82296F48(uint8_t* base, const lifted::Args& a) {
  return f82296F48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822970A0(uint8_t* base, const lifted::Args& a) {
  return f822970A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82297540(uint8_t* base, const lifted::Args& a) {
  return {f82297540(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822979E0(uint8_t* base, const lifted::Args& a) {
  return f822979E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82297D38(uint8_t* base, const lifted::Args& a) {
  return {f82297D38(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82297D58(uint8_t* base, const lifted::Args& a) {
  return f82297D58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82297FD8(uint8_t* base, const lifted::Args& a) {
  return f82297FD8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e82298198(uint8_t* base, const lifted::Args& a) {
  return f82298198(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[5], a.f[0]);
}

lifted::Result e82298950(uint8_t* base, const lifted::Args& a) {
  f82298950(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822990A0(uint8_t* base, const lifted::Args& a) {
  return f822990A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8229BFD0(uint8_t* base, const lifted::Args& a) {
  return f8229BFD0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229D620(uint8_t* base, const lifted::Args& a) {
  return f8229D620(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229D688(uint8_t* base, const lifted::Args& a) {
  return f8229D688(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229D7E0(uint8_t* base, const lifted::Args& a) {
  return {f8229D7E0(base, a.r[0]), a.f[0]};
}

lifted::Result e8229D7F8(uint8_t* base, const lifted::Args& a) {
  return f8229D7F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8229D850(uint8_t* base, const lifted::Args& a) {
  return f8229D850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8229D8A0(uint8_t* base, const lifted::Args& a) {
  return f8229D8A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8229D978(uint8_t* base, const lifted::Args& a) {
  return f8229D978(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229DA20(uint8_t* base, const lifted::Args& a) {
  f8229DA20(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8229DA28(uint8_t* base, const lifted::Args& a) {
  return f8229DA28(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229DB38(uint8_t* base, const lifted::Args& a) {
  return f8229DB38(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229DCC0(uint8_t* base, const lifted::Args& a) {
  return f8229DCC0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8229DD70(uint8_t* base, const lifted::Args& a) {
  return f8229DD70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8229E098(uint8_t* base, const lifted::Args& a) {
  f8229E098(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8229EAF0(uint8_t* base, const lifted::Args& a) {
  return f8229EAF0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12], a.lr);
}

lifted::Result e8229F068(uint8_t* base, const lifted::Args& a) {
  return f8229F068(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8229FC40(uint8_t* base, const lifted::Args& a) {
  return f8229FC40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822A0718(uint8_t* base, const lifted::Args& a) {
  return f822A0718(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822A31D8(uint8_t* base, const lifted::Args& a) {
  return {f822A31D8(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e822A3AA8(uint8_t* base, const lifted::Args& a) {
  return f822A3AA8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822A5C50(uint8_t* base, const lifted::Args& a) {
  return f822A5C50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e822A8418(uint8_t* base, const lifted::Args& a) {
  f822A8418(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822A84C8(uint8_t* base, const lifted::Args& a) {
  f822A84C8(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822A8518(uint8_t* base, const lifted::Args& a) {
  f822A8518(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e822AC2A0(uint8_t* base, const lifted::Args& a) {
  return f822AC2A0(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AC530(uint8_t* base, const lifted::Args& a) {
  return f822AC530(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822AC590(uint8_t* base, const lifted::Args& a) {
  return {f822AC590(base, a.r[0]), a.f[0]};
}

lifted::Result e822AC5B8(uint8_t* base, const lifted::Args& a) {
  return f822AC5B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822AC638(uint8_t* base, const lifted::Args& a) {
  return f822AC638(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822AC858(uint8_t* base, const lifted::Args& a) {
  return f822AC858(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822AC918(uint8_t* base, const lifted::Args& a) {
  return f822AC918(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822ACAA8(uint8_t* base, const lifted::Args& a) {
  return f822ACAA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822ACB68(uint8_t* base, const lifted::Args& a) {
  return f822ACB68(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e822ACFB8(uint8_t* base, const lifted::Args& a) {
  return f822ACFB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AD058(uint8_t* base, const lifted::Args& a) {
  return f822AD058(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822AD228(uint8_t* base, const lifted::Args& a) {
  return f822AD228(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822AD4A0(uint8_t* base, const lifted::Args& a) {
  return f822AD4A0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AD500(uint8_t* base, const lifted::Args& a) {
  return f822AD500(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AD790(uint8_t* base, const lifted::Args& a) {
  return f822AD790(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e822ADA98(uint8_t* base, const lifted::Args& a) {
  return {f822ADA98(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e822ADB50(uint8_t* base, const lifted::Args& a) {
  return f822ADB50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AE918(uint8_t* base, const lifted::Args& a) {
  return {f822AE918(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e822AE9E8(uint8_t* base, const lifted::Args& a) {
  return {f822AE9E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e822AEAA0(uint8_t* base, const lifted::Args& a) {
  return {f822AEAA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e822AEBF0(uint8_t* base, const lifted::Args& a) {
  return f822AEBF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822AF2D8(uint8_t* base, const lifted::Args& a) {
  return {f822AF2D8(base, a.r[0]), a.f[0]};
}

lifted::Result e822AF3D0(uint8_t* base, const lifted::Args& a) {
  return {f822AF3D0(base, a.r1, a.r[1], a.f[0]), a.f[0]};
}

lifted::Result e822AF8A0(uint8_t* base, const lifted::Args& a) {
  return f822AF8A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.v[0]);
}

lifted::Result e822B0D18(uint8_t* base, const lifted::Args& a) {
  return f822B0D18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B0E08(uint8_t* base, const lifted::Args& a) {
  return f822B0E08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0], a.lr);
}

lifted::Result e822B0E60(uint8_t* base, const lifted::Args& a) {
  return f822B0E60(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0], a.lr);
}

lifted::Result e822B0ED0(uint8_t* base, const lifted::Args& a) {
  return f822B0ED0(base, a.r1, a.r13, a.r[0], a.r[1]);
}

lifted::Result e822B0F40(uint8_t* base, const lifted::Args& a) {
  return {f822B0F40(base), a.f[0]};
}

lifted::Result e822B0F70(uint8_t* base, const lifted::Args& a) {
  return f822B0F70(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B1088(uint8_t* base, const lifted::Args& a) {
  return f822B1088(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B1AC8(uint8_t* base, const lifted::Args& a) {
  return f822B1AC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822B1CB0(uint8_t* base, const lifted::Args& a) {
  return f822B1CB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e822B1E70(uint8_t* base, const lifted::Args& a) {
  return f822B1E70(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e822B1FF8(uint8_t* base, const lifted::Args& a) {
  return f822B1FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

const Registration kEntries[] = {
    {0x822929E8, e822929E8},
    {0x82292A48, e82292A48},
    {0x82292B28, e82292B28},
    {0x82292DB8, e82292DB8},
    {0x82292E98, e82292E98},
    {0x82292FB0, e82292FB0},
    {0x82293028, e82293028},
    {0x82293190, e82293190},
    {0x82293288, e82293288},
    {0x82293358, e82293358},
    {0x82293CD8, e82293CD8},
    {0x82293F68, e82293F68},
    {0x82293FD0, e82293FD0},
    {0x822945F8, e822945F8},
    {0x822947A0, e822947A0},
    {0x822947E8, e822947E8},
    {0x82294C88, e82294C88},
    {0x82294CF0, e82294CF0},
    {0x82294D38, e82294D38},
    {0x82294D48, e82294D48},
    {0x82294F78, e82294F78},
    {0x82295018, e82295018},
    {0x822950C0, e822950C0},
    {0x822951A8, e822951A8},
    {0x82295208, e82295208},
    {0x82295310, e82295310},
    {0x82295500, e82295500},
    {0x82295700, e82295700},
    {0x82295C00, e82295C00},
    {0x82295E78, e82295E78},
    {0x822967E0, e822967E0},
    {0x82296F48, e82296F48},
    {0x822970A0, e822970A0},
    {0x82297540, e82297540},
    {0x822979E0, e822979E0},
    {0x82297D38, e82297D38},
    {0x82297D58, e82297D58},
    {0x82297FD8, e82297FD8},
    {0x82298198, e82298198},
    {0x82298950, e82298950},
    {0x822990A0, e822990A0},
    {0x8229BFD0, e8229BFD0},
    {0x8229D620, e8229D620},
    {0x8229D688, e8229D688},
    {0x8229D7E0, e8229D7E0},
    {0x8229D7F8, e8229D7F8},
    {0x8229D850, e8229D850},
    {0x8229D8A0, e8229D8A0},
    {0x8229D978, e8229D978},
    {0x8229DA20, e8229DA20},
    {0x8229DA28, e8229DA28},
    {0x8229DB38, e8229DB38},
    {0x8229DCC0, e8229DCC0},
    {0x8229DD70, e8229DD70},
    {0x8229E098, e8229E098},
    {0x8229EAF0, e8229EAF0},
    {0x8229F068, e8229F068},
    {0x8229FC40, e8229FC40},
    {0x822A0718, e822A0718},
    {0x822A31D8, e822A31D8},
    {0x822A3AA8, e822A3AA8},
    {0x822A5C50, e822A5C50},
    {0x822A8418, e822A8418},
    {0x822A84C8, e822A84C8},
    {0x822A8518, e822A8518},
    {0x822AC2A0, e822AC2A0},
    {0x822AC530, e822AC530},
    {0x822AC590, e822AC590},
    {0x822AC5B8, e822AC5B8},
    {0x822AC638, e822AC638},
    {0x822AC858, e822AC858},
    {0x822AC918, e822AC918},
    {0x822ACAA8, e822ACAA8},
    {0x822ACB68, e822ACB68},
    {0x822ACFB8, e822ACFB8},
    {0x822AD058, e822AD058},
    {0x822AD228, e822AD228},
    {0x822AD4A0, e822AD4A0},
    {0x822AD500, e822AD500},
    {0x822AD790, e822AD790},
    {0x822ADA98, e822ADA98},
    {0x822ADB50, e822ADB50},
    {0x822AE918, e822AE918},
    {0x822AE9E8, e822AE9E8},
    {0x822AEAA0, e822AEAA0},
    {0x822AEBF0, e822AEBF0},
    {0x822AF2D8, e822AF2D8},
    {0x822AF3D0, e822AF3D0},
    {0x822AF8A0, e822AF8A0},
    {0x822B0D18, e822B0D18},
    {0x822B0E08, e822B0E08},
    {0x822B0E60, e822B0E60},
    {0x822B0ED0, e822B0ED0},
    {0x822B0F40, e822B0F40},
    {0x822B0F70, e822B0F70},
    {0x822B1088, e822B1088},
    {0x822B1AC8, e822B1AC8},
    {0x822B1CB0, e822B1CB0},
    {0x822B1E70, e822B1E70},
    {0x822B1FF8, e822B1FF8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_014)), true);

}  // namespace

}  // namespace lifted
