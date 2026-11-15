// Lift sweep batch 008 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_008, true, "ITC",
              "Run lift sweep batch 008's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_821633C8: lifted from generated/ by lift.py.
uint64_t f821633C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821633C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,9612(r3)
  REX_STORE_U32(uint32_t(r3) + 9612, uint32_t(r11));
  // stw r11,9616(r3)
  REX_STORE_U32(uint32_t(r3) + 9616, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,7152(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7152);
  // b 0x8209d0c8
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_821633F0: lifted from generated/ by lift.py.
uint64_t f821633F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821633F0 (lifted)");
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,7148(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D038, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9616(r31)
  REX_STORE_U32(uint32_t(r31) + 9616, uint32_t(r11));
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

// sub_821634D8: lifted from generated/ by lift.py.
lifted::Result f821634D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821634D8 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // mr r26,r4
  r26 = r4;
  // ori r31,r11,38432
  r31 = r11 | 38432;
  // mr r29,r5
  r29 = r5;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101900, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // addi r11,r29,577
  r11 = uint64_t(int64_t(int64_t(r29) + 577));
  // rlwinm r29,r11,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,-32012(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -32012);
  // lwzx r10,r29,r11
  r10 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // lwz r10,24(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 24);
  // mulli r9,r10,3936
  r9 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(3936))));
  // lwz r10,23340(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r27,r9,48
  r27 = uint64_t(int64_t(int64_t(r9) + 48));
  // lwz r3,3912(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 3912);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82163570
  if (cr6.eq) goto loc_82163570;
  // bl 0x821435c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821435C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82163558
  if (cr0.eq) goto loc_82163558;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r4,3912(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 3912);
  // li r3,48
  r3 = uint64_t(int64_t(48));
  // bl 0x82112db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82112DB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821635c4
  goto loc_821635C4;
loc_82163558:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,3912(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 3912);
  // bl 0x821116e0
  // (left out: dead in C++)
  r3 = f821116E0(base, r1, r13, r3, r4);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821635c8
  goto loc_821635C8;
loc_82163570:
  // addi r9,r26,577
  r9 = uint64_t(int64_t(int64_t(r26) + 577));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // add r3,r10,r31
  r3 = r10 + r31;
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r4,24(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 24);
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFAB8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-32012(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -32012);
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwzx r10,r29,r11
  r10 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r3,r11,r31
  r3 = r11 + r31;
  // lwz r4,24(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 24);
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFAB8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x820ffa50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFA50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821635C4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821635C8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821643E0: lifted from generated/ by lift.py.
lifted::Result f821643E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821643E0 (lifted)");
  uint64_t r4 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // mr r29,r3
  r29 = r3;
  // ori r31,r11,39652
  r31 = r11 | 39652;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // rlwinm. r11,r11,25,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 25) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82164424
  if (!cr0.eq) goto loc_82164424;
  // bl 0x82104d40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82104D40, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,32
  r10 = r10 | 32;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_82164424:
  // lwz r11,9624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9624);
  // lwz r11,6100(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 6100);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x8216443c
  if (cr6.eq) goto loc_8216443C;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82164444
  if (!cr6.eq) goto loc_82164444;
loc_8216443C:
  // mr r3,r29
  r3 = r29;
  // bl 0x821635d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821635D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82164444:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82164700: lifted from generated/ by lift.py.
uint64_t f82164700(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82164700 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,9612(r3)
  REX_STORE_U32(uint32_t(r3) + 9612, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,2048
  r10 = r10 | 2048;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,7152(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7152);
  // b 0x8209d0c8
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82164740: lifted from generated/ by lift.py.
void f82164740(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82164740 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // ori r11,r11,39652
  r11 = r11 | 39652;
  // lwz r9,23340(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // mr r8,r11
  r8 = r11;
  // mr r7,r11
  r7 = r11;
  // add r9,r9,r11
  r9 = r9 + r11;
  // mr r6,r11
  r6 = r11;
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // rlwinm r11,r11,0,26,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFBF;
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // lwz r11,23340(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // add r11,r11,r8
  r11 = r11 + r8;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r9,0,27,25
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // add r11,r11,r7
  r11 = r11 + r7;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r9,0,20,18
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFEFFF;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // add r11,r11,r6
  r11 = r11 + r6;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,19,17
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFDFFF;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return;
}

// sub_82164820: lifted from generated/ by lift.py.
lifted::Result f82164820(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_82164820 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f13 = 0.0;
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
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lis r9,5
  r9 = uint64_t(int64_t(327680));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // ori r30,r9,38432
  r30 = r9 | 38432;
  // stw r10,9612(r11)
  REX_STORE_U32(uint32_t(r11) + 9612, uint32_t(r10));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,9616(r11)
  REX_STORE_U32(uint32_t(r11) + 9616, uint32_t(r10));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // add r4,r11,r30
  r4 = r11 + r30;
  // bl 0x820ffd88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFD88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // add r3,r11,r30
  r3 = r11 + r30;
  // bl 0x820ffc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x821648a0
  if (!cr0.eq) goto loc_821648A0;
  // lis r10,10
  r10 = uint64_t(int64_t(655360));
  // ori r10,r10,15088
  r10 = r10 | 15088;
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x821648b4
  if (!cr6.eq) goto loc_821648B4;
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821648b4
  if (cr6.eq) goto loc_821648B4;
loc_821648A0:
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,8192
  r10 = r10 | 8192;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821648B4:
  // li r5,671
  r5 = uint64_t(int64_t(671));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822478d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822478D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_82164B50: lifted from generated/ by lift.py.
lifted::Result f82164B50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82164B50 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-3184(r1)
  ea = -3184 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // mr r25,r3
  r25 = r3;
  // ori r29,r11,38432
  r29 = r11 | 38432;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 23340);
  // addis r30,r11,6
  r30 = uint64_t(int64_t(int64_t(r11) + 393216));
  // add r31,r11,r29
  r31 = r11 + r29;
  // addi r30,r30,-25880
  r30 = uint64_t(int64_t(int64_t(r30) + -25880));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x820ffa30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFA30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // ori r27,r11,15088
  r27 = r11 | 15088;
  // lwz r11,23340(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 23340);
  // beq 0x82164c50
  if (cr0.eq) goto loc_82164C50;
  // lwzx r10,r11,r27
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r27));
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x82164c3c
  if (!cr6.eq) goto loc_82164C3C;
  // addis r29,r30,5
  r29 = uint64_t(int64_t(int64_t(r30) + 327680));
  // lwz r11,800(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 800);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r29,r29,-24608
  r29 = uint64_t(int64_t(int64_t(r29) + -24608));
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // rlwinm r27,r11,31,1,31
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7FFFFFFF;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82164c28
  if (!cr0.gt) goto loc_82164C28;
loc_82164BE0:
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3920(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 3920);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82164c28
  if (cr6.eq) goto loc_82164C28;
  // lwz r11,3924(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 3924);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82164c0c
  if (!cr6.eq) goto loc_82164C0C;
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // b 0x82164c18
  goto loc_82164C18;
loc_82164C0C:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x82164c18
  if (!cr6.eq) goto loc_82164C18;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
loc_82164C18:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82164be0
  if (cr6.lt) goto loc_82164BE0;
loc_82164C28:
  // cmpw cr6,r28,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r27), so);
  // bge cr6,0x82164c60
  if (!cr6.lt) goto loc_82164C60;
  // cmpw cr6,r26,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r27), so);
  // bge cr6,0x82164c60
  if (!cr6.lt) goto loc_82164C60;
  // b 0x82164c48
  goto loc_82164C48;
loc_82164C3C:
  // lwz r10,3924(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 3924);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x82164c50
  if (!cr6.eq) goto loc_82164C50;
loc_82164C48:
  // li r31,41
  r31 = uint64_t(int64_t(41));
  // b 0x82164da0
  goto loc_82164DA0;
loc_82164C50:
  // add r3,r11,r29
  r3 = r11 + r29;
  // bl 0x820ffc68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82164c68
  if (cr0.eq) goto loc_82164C68;
loc_82164C60:
  // li r31,48
  r31 = uint64_t(int64_t(48));
  // b 0x82164da0
  goto loc_82164DA0;
loc_82164C68:
  // lwz r11,23340(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 23340);
  // lwzx r11,r11,r27
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r27));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82164d9c
  if (!cr6.eq) goto loc_82164D9C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,-32012(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -32012);
  // bl 0x82170608
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170608, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821434E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82164cb4
  if (cr0.eq) goto loc_82164CB4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x821442d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821442D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // b 0x82164cb8
  goto loc_82164CB8;
loc_82164CB4:
  // li r31,-1
  r31 = uint64_t(int64_t(-1));
loc_82164CB8:
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x82164cd4
  if (cr6.eq) goto loc_82164CD4;
  // bl 0x820ff958
  // (left out: dead in C++)
  r3 = f820FF958(base);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // bl 0x82112db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82112DB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82164c48
  goto loc_82164C48;
loc_82164CD4:
  // lwz r11,3924(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 3924);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82164d9c
  if (!cr6.eq) goto loc_82164D9C;
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82164d9c
  if (!cr0.gt) goto loc_82164D9C;
  // addi r30,r26,4
  r30 = uint64_t(int64_t(int64_t(r26) + 4));
loc_82164CF4:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r10,23340(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 23340);
  // mulli r11,r11,3936
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3936))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r31,r11,48
  r31 = uint64_t(int64_t(int64_t(r11) + 48));
  // lwz r11,3912(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3912);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82164d88
  if (cr6.eq) goto loc_82164D88;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,3912(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3912);
  // addi r10,r11,8746
  r10 = uint64_t(int64_t(int64_t(r11) + 8746));
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r3
  r10 = r10 + r3;
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r8,4(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r9,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r9));
  // lhz r9,90(r1)
  r9 = REX_LOAD_U16(uint32_t(r1) + 90);
  // stw r8,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r8));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // bne cr6,0x82164d88
  if (!cr6.eq) goto loc_82164D88;
  // mr r3,r11
  r3 = r11;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117178, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,147(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 147);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x82164c60
  if (cr6.eq) goto loc_82164C60;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x82164c60
  if (cr6.eq) goto loc_82164C60;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x82164c60
  if (cr6.eq) goto loc_82164C60;
loc_82164D88:
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,972
  r30 = uint64_t(int64_t(int64_t(r30) + 972));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x82164cf4
  if (cr6.lt) goto loc_82164CF4;
loc_82164D9C:
  // li r31,7
  r31 = uint64_t(int64_t(7));
loc_82164DA0:
  // lwz r3,9624(r25)
  r3 = REX_LOAD_U32(uint32_t(r25) + 9624);
  // bl 0x82163440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82163440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,9624(r25)
  r3 = REX_LOAD_U32(uint32_t(r25) + 9624);
  // bl 0x821648e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821648E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,3184
  r1 = uint64_t(int64_t(int64_t(r1) + 3184));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821654F8: lifted from generated/ by lift.py.
lifted::Result f821654F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_821654F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-288(r1)
  ea = -288 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lis r10,5
  r10 = uint64_t(int64_t(327680));
  // lis r9,5
  r9 = uint64_t(int64_t(327680));
  // ori r10,r10,38368
  r10 = r10 | 38368;
  // ori r17,r9,38432
  r17 = r9 | 38432;
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r9,12000
  r9 = uint64_t(int64_t(12000));
  // addi r28,r11,48
  r28 = uint64_t(int64_t(int64_t(r11) + 48));
  // mr r31,r3
  r31 = r3;
  // add r27,r11,r17
  r27 = r11 + r17;
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // mr r3,r29
  r3 = r29;
  // stw r29,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r29));
  // lwzx r10,r28,r10
  r10 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r10));
  // stw r28,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r28));
  // stw r9,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r9));
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble 0x82165580
  if (!cr0.gt) goto loc_82165580;
  // addi r9,r28,3888
  r9 = uint64_t(int64_t(int64_t(r28) + 3888));
loc_82165554:
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // clrlwi. r8,r8,31
  r8 = uint32_t(r8) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x82165574
  if (!cr0.eq) goto loc_82165574;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r9,r9,3936
  r9 = uint64_t(int64_t(int64_t(r9) + 3936));
  // cmpw cr6,r3,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r10), so);
  // blt cr6,0x82165554
  if (cr6.lt) goto loc_82165554;
  // b 0x82165580
  goto loc_82165580;
loc_82165574:
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
loc_82165580:
  // lis r10,10
  r10 = uint64_t(int64_t(655360));
  // lis r9,5
  r9 = uint64_t(int64_t(327680));
  // ori r21,r10,15088
  r21 = r10 | 15088;
  // ori r14,r9,39656
  r14 = r9 | 39656;
  // lwzx r10,r11,r21
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r21));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8216561c
  if (!cr6.eq) goto loc_8216561C;
  // lis r10,5
  r10 = uint64_t(int64_t(327680));
  // ori r10,r10,39652
  r10 = r10 | 39652;
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // rlwinm. r10,r10,25,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8216561c
  if (!cr0.eq) goto loc_8216561C;
  // lwz r10,804(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 804);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // blt cr6,0x8216561c
  if (cr6.lt) goto loc_8216561C;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // add r3,r11,r14
  r3 = r11 + r14;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r9,0(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r11,r29
  r11 = r29;
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // ble 0x8216561c
  if (!cr0.gt) goto loc_8216561C;
  // addi r10,r3,4
  r10 = uint64_t(int64_t(int64_t(r3) + 4));
loc_821655E0:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // mulli r8,r8,3936
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(3936))));
  // add r8,r8,r28
  r8 = r8 + r28;
  // lwz r8,3888(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 3888);
  // clrlwi. r8,r8,31
  r8 = uint32_t(r8) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8216560c
  if (!cr0.eq) goto loc_8216560C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,972
  r10 = uint64_t(int64_t(int64_t(r10) + 972));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // blt cr6,0x821655e0
  if (cr6.lt) goto loc_821655E0;
  // b 0x8216561c
  goto loc_8216561C;
loc_8216560C:
  // mulli r11,r11,972
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(972))));
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r11,788(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 788);
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
loc_8216561C:
  // lwz r3,9624(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // mr r15,r29
  r15 = r29;
  // bl 0x821648e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821648E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // bl 0x820ffc08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // bl 0x820ffbe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFBE8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // mr r5,r3
  r5 = r3;
  // lwz r3,9624(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // mr r6,r30
  r6 = r30;
  // lwzx r4,r11,r21
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r21));
  // bl 0x821649b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821649B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // add r4,r11,r17
  r4 = r11 + r17;
  // bl 0x820ffd48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFD48, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r6,0(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r5,4(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r4,8(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,12(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 12);
  // li r16,-1
  r16 = uint64_t(int64_t(-1));
  // lwz r9,9624(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // li r8,10
  r8 = uint64_t(int64_t(10));
  // mr r7,r16
  r7 = r16;
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // addi r9,r9,6076
  r9 = uint64_t(int64_t(int64_t(r9) + 6076));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
  // stw r3,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r3));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r6,4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwz r5,8(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + 8);
  // lwz r10,12(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // stw r6,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r6));
  // stw r5,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r5));
  // stw r10,12(r9)
  REX_STORE_U32(uint32_t(r9) + 12, uint32_t(r10));
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // mtctr r8
  ctr = r8;
loc_821656D4:
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x821656d4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821656D4;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // mr r25,r29
  r25 = r29;
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x82165a30
  if (!cr0.gt) goto loc_82165A30;
  // mr r18,r29
  r18 = r29;
  // mr r22,r29
  r22 = r29;
  // li r19,-2
  r19 = uint64_t(int64_t(-2));
  // li r23,100
  r23 = uint64_t(int64_t(100));
  // b 0x82165710
  goto loc_82165710;
loc_8216570C:
  // lwz r28,92(r1)
  r28 = REX_LOAD_U32(uint32_t(r1) + 92);
loc_82165710:
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // mr r4,r25
  r4 = r25;
  // mr r3,r27
  r3 = r27;
  // bl 0x820ffc98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r20,r3
  r20 = r3;
  // mulli r11,r20,3936
  r11 = uint64_t(int64_t(static_cast<int64_t>(r20 * static_cast<uint64_t>(3936))));
  // add r24,r11,r28
  r24 = r11 + r28;
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // ori r11,r11,16
  r11 = r11 | 16;
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
  // lwz r10,3888(r24)
  r10 = REX_LOAD_U32(uint32_t(r24) + 3888);
  // rlwinm. r10,r10,21,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 21) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82165754
  if (cr0.eq) goto loc_82165754;
  // ori r11,r11,4096
  r11 = r11 | 4096;
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_82165754:
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // rlwinm. r10,r11,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821657b0
  if (cr0.eq) goto loc_821657B0;
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // rlwinm r9,r11,0,27,25
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // addi r30,r28,8
  r30 = uint64_t(int64_t(int64_t(r28) + 8));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // mr r11,r10
  r11 = r10;
  // stw r9,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r9));
  // extsh r4,r19
  r4 = uint64_t(int64_t(int16_t(r19)));
  // mr r3,r30
  r3 = r30;
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // sth r10,0(r30)
  REX_STORE_U16(uint32_t(r30) + 0, uint16_t(r10));
  // lhz r11,2(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 2);
  // sth r11,2(r30)
  REX_STORE_U16(uint32_t(r30) + 2, uint16_t(r11));
  // bl 0x82170f18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lhz r10,0(r30)
  r10 = REX_LOAD_U16(uint32_t(r30) + 0);
  // addi r11,r24,3904
  r11 = uint64_t(int64_t(int64_t(r24) + 3904));
  // addi r19,r19,-2
  r19 = uint64_t(int64_t(int64_t(r19) + -2));
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // lhz r10,2(r30)
  r10 = REX_LOAD_U16(uint32_t(r30) + 2);
  // sth r10,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r10));
loc_821657B0:
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // addi r3,r11,2184
  r3 = uint64_t(int64_t(int64_t(r11) + 2184));
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mulli r10,r10,536
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(536))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r28,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r28));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // lwz r4,24(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 24);
  // addi r11,r11,48
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  // mulli r10,r4,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r4 * static_cast<uint64_t>(3936))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r5,3912(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 3912);
  // bl 0x820ffdd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFDD8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r10,9624(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // subf r11,r25,r11
  r11 = r11 - r25;
  // add r10,r10,r29
  r10 = r10 + r29;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r4,r25
  r4 = r25;
  // mr r3,r27
  r3 = r27;
  // stw r11,2196(r10)
  REX_STORE_U32(uint32_t(r10) + 2196, uint32_t(r11));
  // bl 0x820ffcc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFCC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // mr r10,r23
  r10 = r23;
  // mr r9,r23
  r9 = r23;
  // add r11,r11,r22
  r11 = r11 + r22;
  // stw r3,568(r11)
  REX_STORE_U32(uint32_t(r11) + 568, uint32_t(r3));
  // lwz r11,3924(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 3924);
  // cmpwi cr6,r11,15
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 15, so);
  // blt cr6,0x82165840
  if (cr6.lt) goto loc_82165840;
  // li r10,200
  r10 = uint64_t(int64_t(200));
  // b 0x8216587c
  goto loc_8216587C;
loc_82165840:
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // blt cr6,0x82165850
  if (cr6.lt) goto loc_82165850;
  // li r10,180
  r10 = uint64_t(int64_t(180));
  // b 0x8216587c
  goto loc_8216587C;
loc_82165850:
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // blt cr6,0x82165860
  if (cr6.lt) goto loc_82165860;
  // li r10,160
  r10 = uint64_t(int64_t(160));
  // b 0x8216587c
  goto loc_8216587C;
loc_82165860:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // blt cr6,0x82165870
  if (cr6.lt) goto loc_82165870;
  // li r10,140
  r10 = uint64_t(int64_t(140));
  // b 0x8216587c
  goto loc_8216587C;
loc_82165870:
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // blt cr6,0x8216587c
  if (cr6.lt) goto loc_8216587C;
  // li r10,120
  r10 = uint64_t(int64_t(120));
loc_8216587C:
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // ori r8,r11,15092
  r8 = r11 | 15092;
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // lwzx r11,r11,r8
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // bne cr6,0x82165898
  if (!cr6.eq) goto loc_82165898;
  // li r9,150
  r9 = uint64_t(int64_t(150));
loc_82165898:
  // mulli r10,r10,12000
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12000))));
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // divw r10,r10,r23
  r10 = uint32_t((int32_t(r23) && !(int32_t(r10) == INT32_MIN && int32_t(r23) == -1)) ? int32_t(r10) / int32_t(r23) : 0);
  // add r11,r11,r29
  r11 = r11 + r29;
  // mullw r10,r10,r9
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // divw r10,r10,r23
  r10 = uint32_t((int32_t(r23) && !(int32_t(r10) == INT32_MIN && int32_t(r23) == -1)) ? int32_t(r10) / int32_t(r23) : 0);
  // stw r10,2972(r11)
  REX_STORE_U32(uint32_t(r11) + 2972, uint32_t(r10));
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // add r11,r11,r29
  r11 = r11 + r29;
  // lwz r10,2972(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 2972);
  // stw r10,2976(r11)
  REX_STORE_U32(uint32_t(r11) + 2976, uint32_t(r10));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // lwzx r10,r11,r21
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r21));
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x821658f4
  if (!cr6.eq) goto loc_821658F4;
  // lwz r9,9624(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // lwz r8,6100(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 6100);
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // beq cr6,0x821658f4
  if (cr6.eq) goto loc_821658F4;
  // add r11,r9,r29
  r11 = r9 + r29;
  // lwz r11,2972(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 2972);
  // stw r11,6112(r9)
  REX_STORE_U32(uint32_t(r9) + 6112, uint32_t(r11));
  // b 0x82165998
  goto loc_82165998;
loc_821658F4:
  // lwz r9,0(r28)
  r9 = REX_LOAD_U32(uint32_t(r28) + 0);
  // rlwinm. r9,r9,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x82165998
  if (cr0.eq) goto loc_82165998;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8216591c
  if (cr6.eq) goto loc_8216591C;
  // lis r10,5
  r10 = uint64_t(int64_t(327680));
  // ori r10,r10,39652
  r10 = r10 | 39652;
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // rlwinm. r10,r10,25,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 25) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8216593c
  if (cr0.eq) goto loc_8216593C;
loc_8216591C:
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // add r3,r11,r14
  r3 = r11 + r14;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // lwz r10,3928(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 3928);
  // add r11,r11,r29
  r11 = r11 + r29;
  // stw r10,2972(r11)
  REX_STORE_U32(uint32_t(r11) + 2972, uint32_t(r10));
  // b 0x82165988
  goto loc_82165988;
loc_8216593C:
  // lwz r10,84(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // blt cr6,0x82165988
  if (cr6.lt) goto loc_82165988;
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // addi r9,r10,3
  r9 = uint64_t(int64_t(int64_t(r10) + 3));
  // lwz r8,88(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 88);
  // add r10,r11,r29
  r10 = r11 + r29;
  // lwz r11,2972(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 2972);
  // mullw r9,r9,r11
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r11))));
  // mulli r9,r9,5
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(5))));
  // divw r9,r9,r23
  r9 = uint32_t((int32_t(r23) && !(int32_t(r9) == INT32_MIN && int32_t(r23) == -1)) ? int32_t(r9) / int32_t(r23) : 0);
  // add r9,r9,r8
  r9 = r9 + r8;
  // stw r9,2972(r10)
  REX_STORE_U32(uint32_t(r10) + 2972, uint32_t(r9));
  // lwz r10,9624(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // add r10,r10,r29
  r10 = r10 + r29;
  // lwz r9,2972(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 2972);
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bge cr6,0x82165988
  if (!cr6.lt) goto loc_82165988;
  // stw r11,2972(r10)
  REX_STORE_U32(uint32_t(r10) + 2972, uint32_t(r11));
loc_82165988:
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // add r10,r11,r29
  r10 = r11 + r29;
  // lwz r10,2972(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 2972);
  // stw r10,6112(r11)
  REX_STORE_U32(uint32_t(r11) + 6112, uint32_t(r10));
loc_82165998:
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // rlwinm. r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x821659e8
  if (!cr0.eq) goto loc_821659E8;
  // lwz r3,3912(r24)
  r3 = REX_LOAD_U32(uint32_t(r24) + 3912);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x821659e8
  if (cr6.eq) goto loc_821659E8;
  // rlwinm. r11,r11,0,18,18
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821659c0
  if (cr0.eq) goto loc_821659C0;
  // li r15,1
  r15 = uint64_t(int64_t(1));
  // b 0x821659e8
  goto loc_821659E8;
loc_821659C0:
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117178, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,35(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 35);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821659e8
  if (cr6.eq) goto loc_821659E8;
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // stwx r20,r11,r18
  REX_STORE_U32(uint32_t(r11) + uint32_t(r18), uint32_t(r20));
  // addi r18,r18,4
  r18 = uint64_t(int64_t(int64_t(r18) + 4));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // ori r11,r11,1024
  r11 = r11 | 1024;
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821659E8:
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r27
  r3 = r27;
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // addi r22,r22,536
  r22 = uint64_t(int64_t(int64_t(r22) + 536));
  // addi r29,r29,972
  r29 = uint64_t(int64_t(int64_t(r29) + 972));
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r25,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r25), int32_t(r3), so);
  // blt cr6,0x8216570c
  if (cr6.lt) goto loc_8216570C;
  // cmpwi cr6,r15,0
  cr6 = lifted::Compare<int32_t>(int32_t(r15), 0, so);
  // beq cr6,0x82165a2c
  if (cr6.eq) goto loc_82165A2C;
  // lwz r11,9624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9624);
  // mr r9,r16
  r9 = r16;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // mtctr r10
  ctr = r10;
loc_82165A20:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x82165a20
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82165A20;
loc_82165A2C:
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82165A30:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -32012);
  // bl 0x82171978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // add r3,r11,r17
  r3 = r11 + r17;
  // bl 0x820ffc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82165a6c
  if (cr0.eq) goto loc_82165A6C;
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,4096
  r10 = r10 | 4096;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_82165A6C:
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // li r5,671
  r5 = uint64_t(int64_t(671));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,15,13
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFDFFFF;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r29,9620(r31)
  REX_STORE_U32(uint32_t(r31) + 9620, uint32_t(r29));
  // stw r29,9628(r31)
  REX_STORE_U32(uint32_t(r31) + 9628, uint32_t(r29));
  // stw r29,9632(r31)
  REX_STORE_U32(uint32_t(r31) + 9632, uint32_t(r29));
  // stw r29,9612(r31)
  REX_STORE_U32(uint32_t(r31) + 9612, uint32_t(r29));
  // stw r29,9616(r31)
  REX_STORE_U32(uint32_t(r31) + 9616, uint32_t(r29));
  // bl 0x822478d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822478D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,288
  r1 = uint64_t(int64_t(int64_t(r1) + 288));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821665F0: lifted from generated/ by lift.py.
void f821665F0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821665F0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r11,20692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20692);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,44
  r9 = uint64_t(int64_t(44));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r4,3
  r4 = uint64_t(int64_t(3));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // stw r4,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r4));
  // blr
  return;
}

// sub_82166658: lifted from generated/ by lift.py.
lifted::Result f82166658(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82166658 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-128
  r12 = uint64_t(int64_t(int64_t(r1) + -128));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-464(r1)
  ea = -464 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r18,r3
  r18 = r3;
  // mr r22,r4
  r22 = r4;
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,9624(r18)
  r10 = REX_LOAD_U32(uint32_t(r18) + 9624);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // mr r19,r3
  r19 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,7240(r10)
  REX_STORE_U32(uint32_t(r10) + 7240, uint32_t(r11));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // addis r31,r11,10
  r31 = uint64_t(int64_t(int64_t(r11) + 655360));
  // addi r31,r31,15056
  r31 = uint64_t(int64_t(int64_t(r31) + 15056));
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0468, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x820c0128
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0128, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8692(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8692);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821666cc
  if (!cr6.lt) goto loc_821666CC;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
loc_821666CC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // addi r6,r11,-26536
  r6 = uint64_t(int64_t(int64_t(r11) + -26536));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r17,0
  r17 = uint64_t(int64_t(0));
  // cmpwi cr6,r19,0
  cr6 = lifted::Compare<int32_t>(int32_t(r19), 0, so);
  // ble cr6,0x8216672c
  if (!cr6.gt) goto loc_8216672C;
  // li r31,2308
  r31 = uint64_t(int64_t(2308));
  // lis r20,-32146
  r20 = uint64_t(int64_t(-2106720256));
loc_821666F8:
  // lwz r11,-32012(r20)
  r11 = REX_LOAD_U32(uint32_t(r20) + -32012);
  // lwzx r11,r31,r11
  r11 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // lwz r23,20(r11)
  r23 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mr r3,r23
  r3 = r23;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0468, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r11,6148(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 6148);
  // clrlwi. r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821667b8
  if (!cr0.eq) goto loc_821667B8;
  // addi r17,r17,1
  r17 = uint64_t(int64_t(int64_t(r17) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // cmpw cr6,r17,r19
  cr6 = lifted::Compare<int32_t>(int32_t(r17), int32_t(r19), so);
  // blt cr6,0x821666f8
  if (cr6.lt) goto loc_821666F8;
loc_8216672C:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x821434a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821434A0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x821667a8
  if (cr0.lt) goto loc_821667A8;
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // bl 0x82101900
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82101900, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // li r3,48
  r3 = uint64_t(int64_t(48));
  // bl 0x82112db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82112DB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r22,204
  r11 = uint64_t(int64_t(int64_t(r22) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
loc_821667A0:
  // li r9,17
  r9 = uint64_t(int64_t(17));
loc_821667A4:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
loc_821667A8:
  // addi r1,r1,464
  r1 = uint64_t(int64_t(int64_t(r1) + 464));
  // addi r12,r1,-128
  r12 = uint64_t(int64_t(int64_t(r1) + -128));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
loc_821667B8:
  // mr r3,r30
  r3 = r30;
  // bl 0x820c0168
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0168, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r21,r10,23312
  r21 = uint64_t(int64_t(int64_t(r10) + 23312));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mr r3,r21
  r3 = r21;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x820f8d38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8D38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x82166884
  if (cr6.eq) goto loc_82166884;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r6,r31,14
  r6 = uint64_t(int64_t(int64_t(r31) + 14));
  // lis r10,5
  r10 = uint64_t(int64_t(327680));
  // addi r11,r22,204
  r11 = uint64_t(int64_t(int64_t(r22) + 204));
  // ori r10,r10,39652
  r10 = r10 | 39652;
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // stw r6,6112(r3)
  REX_STORE_U32(uint32_t(r3) + 6112, uint32_t(r6));
  // mr r4,r10
  r4 = r10;
  // mr r31,r10
  r31 = r10;
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // lwz r6,604(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // add r11,r11,r4
  r11 = r11 + r4;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,29,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFFFF7;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,16
  r10 = r10 | 16;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821667a8
  goto loc_821667A8;
loc_82166884:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // rlwinm r8,r23,2,22,29
  r8 = __builtin_rotateleft64(uint32_t(r23) | (r23 << 32), 2) & 0x3FC;
  // addi r11,r11,23344
  r11 = uint64_t(int64_t(int64_t(r11) + 23344));
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // mtctr r9
  ctr = r9;
loc_821668A0:
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x821668a0
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821668A0;
  // mr r3,r30
  r3 = r30;
  // bl 0x820c0168
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0168, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // mr r3,r30
  r3 = r30;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lfs f0,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f0 = double(temp.f32);
  // stfs f0,220(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 220, temp.u32);
  // stfs f0,236(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 236, temp.u32);
  // stfs f0,252(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 252, temp.u32);
  // stfs f0,268(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 268, temp.u32);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x820c0128
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0128, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // fmr f27,f1
  lifted::ScalarFloatMode();
  f27 = f1;
  // bl 0x820b6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82293df0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82293DF0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f30,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // stfs f30,100(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // bl 0x822941b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822941B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,336(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 336);
  // bl 0x820b67d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B67D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // ld r10,0(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 0);
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // lhz r24,84(r1)
  r24 = REX_LOAD_U16(uint32_t(r1) + 84);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r25,2308
  r25 = uint64_t(int64_t(2308));
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // ld r10,8(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 8);
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f30,148(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 148, temp.u32);
  // lfd f29,-26544(r10)
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r10) + -26544)));
  // lfs f28,10704(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10704);
  f28 = double(temp.f32);
loc_82166954:
  // lwz r11,-32012(r20)
  r11 = REX_LOAD_U32(uint32_t(r20) + -32012);
  // lwzx r11,r25,r11
  r11 = REX_LOAD_U32(uint32_t(r25) + uint32_t(r11));
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // cmpw cr6,r23,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r23), int32_t(r3), so);
  // beq cr6,0x82166a34
  if (cr6.eq) goto loc_82166A34;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0468, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // lwz r11,6148(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 6148);
  // rlwinm. r11,r11,30,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82166a34
  if (!cr0.eq) goto loc_82166A34;
  // bl 0x820c0168
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0168, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // bl 0x820c2150
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C2150, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,336(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 336);
  // bl 0x820b67d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B67D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lha r29,196(r1)
  r29 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r1) + 196))));
  // extsh r10,r24
  r10 = uint64_t(int64_t(int16_t(r24)));
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // cmpw cr6,r29,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r10), so);
  // ld r10,0(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 0);
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // ld r10,8(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 8);
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
  // stfs f30,132(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // beq cr6,0x821669ec
  if (cr6.eq) goto loc_821669EC;
  // li r28,0
  r28 = uint64_t(int64_t(0));
loc_821669C0:
  // clrlwi r5,r28,16
  r5 = uint32_t(r28) & 0xFFFF;
  // mr r4,r24
  r4 = r24;
  // mr r3,r21
  r3 = r21;
  // bl 0x820f89f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F89F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // extsh r11,r3
  r11 = uint64_t(int64_t(int16_t(r3)));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // beq cr6,0x821669ec
  if (cr6.eq) goto loc_821669EC;
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r28,12
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 12, so);
  // blt cr6,0x821669c0
  if (cr6.lt) goto loc_821669C0;
  // b 0x82166a34
  goto loc_82166A34;
loc_821669EC:
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822941b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822941B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fcmpu cr6,f1,f28
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f1, f28);
  // bge cr6,0x82166a34
  if (!cr6.lt) goto loc_82166A34;
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82294170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294170, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // fmr f1,f29
  f1 = f29;
  // bl 0x825a8978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // fcmpu cr6,f31,f0
  cr6 = lifted::CompareFloat(f31, f0);
  // bgt cr6,0x82166a48
  if (cr6.gt) goto loc_82166A48;
loc_82166A34:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r25,r25,4
  r25 = uint64_t(int64_t(int64_t(r25) + 4));
  // cmpw cr6,r27,r19
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r19), so);
  // blt cr6,0x82166954
  if (cr6.lt) goto loc_82166954;
  // b 0x8216672c
  goto loc_8216672C;
loc_82166A48:
  // mr r3,r31
  r3 = r31;
  // bl 0x820c0128
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C0128, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,160(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 160);
  // addi r11,r27,577
  r11 = uint64_t(int64_t(int64_t(r27) + 577));
  // lwz r9,9624(r18)
  r9 = REX_LOAD_U32(uint32_t(r18) + 9624);
  // fsubs f0,f27,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f27 - f1));
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,-32012(r20)
  r11 = REX_LOAD_U32(uint32_t(r20) + -32012);
  // lwzx r11,r8,r11
  r11 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r11));
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // rlwinm. r10,r10,0,23,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x100;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // mulli r10,r11,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3936))));
  // lwz r11,23340(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 23340);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r11,3960(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 3960);
  // stw r11,7240(r9)
  REX_STORE_U32(uint32_t(r9) + 7240, uint32_t(r11));
  // beq 0x8216672c
  if (cr0.eq) goto loc_8216672C;
  // lwz r11,6168(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 6168);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8216672c
  if (!cr6.eq) goto loc_8216672C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,3396(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3396);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x8216672c
  if (!cr6.lt) goto loc_8216672C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f13,-29112(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -29112);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8216672c
  if (!cr6.gt) goto loc_8216672C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,9572(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9572);
  f0 = double(temp.f32);
  // fcmpu cr6,f27,f0
  cr6 = lifted::CompareFloat(f27, f0);
  // blt cr6,0x8216672c
  if (cr6.lt) goto loc_8216672C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32164(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -32164);
  f0 = double(temp.f32);
  // fcmpu cr6,f27,f0
  cr6 = lifted::CompareFloat(f27, f0);
  // bgt cr6,0x8216672c
  if (cr6.gt) goto loc_8216672C;
  // mr r5,r27
  r5 = r27;
  // mr r4,r17
  r4 = r17;
  // mr r3,r18
  r3 = r18;
  // bl 0x821634d8
  // (left out: dead in C++)
  { const lifted::Result o = f821634D8(base, r1, r13, r4, r5, r6, r7, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r11,r22,204
  r11 = uint64_t(int64_t(int64_t(r22) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
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
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // bne 0x821667a0
  if (!cr0.eq) goto loc_821667A0;
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // b 0x821667a4
  goto loc_821667A4;
}

// sub_82166B40: lifted from generated/ by lift.py.
lifted::Result f82166B40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82166B40 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
  // mr r29,r4
  r29 = r4;
  // bl 0x82163218
  // (left out: dead in C++)
  { const lifted::Result o = f82163218(base, r1, r13, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82166bac
  if (!cr0.eq) goto loc_82166BAC;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r11,20692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20692);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // bne cr6,0x82166e48
  if (!cr6.eq) goto loc_82166E48;
loc_82166B70:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26520
  r6 = uint64_t(int64_t(int64_t(r11) + -26520));
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // stw r11,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x82166e48
  if (cr6.lt) goto loc_82166E48;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82166e48
  goto loc_82166E48;
loc_82166BAC:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // ori r27,r11,15088
  r27 = r11 | 15088;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // lwzx r11,r11,r27
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r27));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82166c44
  if (!cr6.eq) goto loc_82166C44;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,6100(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 6100);
  // cmpwi cr6,r11,999
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 999, so);
  // bgt cr6,0x82166be4
  if (cr6.gt) goto loc_82166BE4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82166bf4
  if (cr6.eq) goto loc_82166BF4;
loc_82166BE4:
  // lwz r11,9624(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9624);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x82166c44
  if (cr6.eq) goto loc_82166C44;
loc_82166BF4:
  // addi r11,r29,204
  r11 = uint64_t(int64_t(int64_t(r29) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82166e48
  goto loc_82166E48;
loc_82166C44:
  // lwz r11,9612(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82166cbc
  if (cr6.lt) goto loc_82166CBC;
  // bne cr6,0x82166e48
  if (!cr6.eq) goto loc_82166E48;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82166e48
  if (!cr0.eq) goto loc_82166E48;
  // addi r11,r29,204
  r11 = uint64_t(int64_t(int64_t(r29) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,27
  r9 = uint64_t(int64_t(27));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82166e40
  goto loc_82166E40;
loc_82166CBC:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lwz r11,20692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20692);
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // bne cr6,0x82166e38
  if (!cr6.eq) goto loc_82166E38;
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82166b70
  if (!cr0.eq) goto loc_82166B70;
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x82166b70
  if (!cr6.gt) goto loc_82166B70;
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
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // ori r10,r11,39652
  r10 = r11 | 39652;
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // rlwinm. r11,r11,24,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82166dc0
  if (cr0.eq) goto loc_82166DC0;
  // addi r11,r29,204
  r11 = uint64_t(int64_t(int64_t(r29) + 204));
  // mr r26,r10
  r26 = r10;
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r3,604(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // add r3,r3,r10
  r3 = r3 + r10;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r3,-1
  r3 = uint64_t(int64_t(int64_t(r3) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r25,r3,r31
  r25 = uint32_t(uint32_t(r31) ? uint32_t(r3) / uint32_t(r31) : 0);
  // mulli r25,r25,50
  r25 = uint64_t(int64_t(static_cast<int64_t>(r25 * static_cast<uint64_t>(50))));
  // subf r10,r25,r3
  r10 = r3 - r25;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,23340(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // add r10,r10,r26
  r10 = r10 + r26;
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,0,24,22
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r31
  r8 = uint32_t(uint32_t(r31) ? uint32_t(r9) / uint32_t(r31) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
loc_82166DC0:
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addis r4,r11,6
  r4 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r4,-27104
  r4 = uint64_t(int64_t(int64_t(r4) + -27104));
  // bl 0x820ffd88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFD88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // lwzx r11,r11,r27
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r27));
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x82166e24
  if (!cr6.eq) goto loc_82166E24;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82166e24
  if (cr6.eq) goto loc_82166E24;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // addi r11,r29,-36
  r11 = uint64_t(int64_t(int64_t(r29) + -36));
  // bne cr6,0x82166e0c
  if (!cr6.eq) goto loc_82166E0C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82166E0C:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,10512
  r3 = uint64_t(int64_t(int64_t(r11) + 10512));
  // lwz r10,11496(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 11496);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r4,1860(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 1860);
  // bl 0x821591f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821591F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82166E24:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,9612(r28)
  REX_STORE_U32(uint32_t(r28) + 9612, uint32_t(r11));
  // stw r10,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r10));
  // b 0x82166e48
  goto loc_82166E48;
loc_82166E38:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,9612(r28)
  REX_STORE_U32(uint32_t(r28) + 9612, uint32_t(r10));
loc_82166E40:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r11));
loc_82166E48:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82166F98: lifted from generated/ by lift.py.
lifted::Result f82166F98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82166F98 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9612(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821671a4
  if (cr6.lt) goto loc_821671A4;
  // beq cr6,0x82167144
  if (cr6.eq) goto loc_82167144;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821672e8
  if (!cr6.lt) goto loc_821672E8;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821672e8
  if (!cr0.eq) goto loc_821672E8;
  // addi r31,r31,204
  r31 = uint64_t(int64_t(int64_t(r31) + 204));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // ori r28,r11,39652
  r28 = r11 | 39652;
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r7,604(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 604);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lis r27,-32148
  r27 = uint64_t(int64_t(-2106851328));
  // add r7,r7,r11
  r7 = r7 + r11;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // divwu r6,r7,r30
  r6 = uint32_t(uint32_t(r30) ? uint32_t(r7) / uint32_t(r30) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x82165e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82165E20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -32012);
  // bl 0x82171e50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171E50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addis r4,r11,6
  r4 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r4,r4,-27104
  r4 = uint64_t(int64_t(int64_t(r4) + -27104));
  // bl 0x820ffd88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFD88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x82167098
  if (cr6.eq) goto loc_82167098;
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,19,17
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFDFFF;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_82167098:
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,18,16
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFBFFF;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8213a140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213A140, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // lwz r4,604(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 604);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // add r4,r4,r11
  r4 = r4 + r11;
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // divwu r3,r4,r30
  r3 = uint32_t(uint32_t(r30) ? uint32_t(r4) / uint32_t(r30) : 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mulli r3,r3,50
  r3 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(50))));
  // subf r11,r3,r4
  r11 = r4 - r3;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // lwz r10,604(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r10,r11
  r10 = r10 + r11;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r9,r10,r30
  r9 = uint32_t(uint32_t(r30) ? uint32_t(r10) / uint32_t(r30) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r11,r9,r10
  r11 = r10 - r9;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r6));
  // stw r5,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r5));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r11));
  // b 0x821672e8
  goto loc_821672E8;
loc_82167144:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26500
  r6 = uint64_t(int64_t(int64_t(r11) + -26500));
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821672e8
  if (cr0.eq) goto loc_821672E8;
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // rlwinm r8,r8,0,0,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFE;
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // stw r10,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r10));
  // stw r9,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r9));
  // b 0x821672e8
  goto loc_821672E8;
loc_821671A4:
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x821672cc
  if (!cr0.eq) goto loc_821672CC;
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x821672cc
  if (!cr6.gt) goto loc_821672CC;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r6,31
  r6 = uint64_t(int64_t(31));
  // lwz r27,604(r11)
  r27 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r27,r27,r10
  r27 = r27 + r10;
  // li r3,32
  r3 = uint64_t(int64_t(32));
  // addi r27,r27,-1
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // divwu r26,r27,r30
  r26 = uint32_t(uint32_t(r30) ? uint32_t(r27) / uint32_t(r30) : 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mulli r26,r26,50
  r26 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(50))));
  // subf r10,r26,r27
  r10 = r27 - r26;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r6,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r6));
  // stw r5,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r5));
  // stw r4,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r4));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,23340(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // addis r10,r10,6
  r10 = uint64_t(int64_t(int64_t(r10) + 393216));
  // addi r10,r10,-25884
  r10 = uint64_t(int64_t(int64_t(r10) + -25884));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,0,24,22
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // bl 0x8213a000
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213A000, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r11,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r11));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // b 0x821672e8
  goto loc_821672E8;
loc_821672CC:
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // blt cr6,0x821672e8
  if (cr6.lt) goto loc_821672E8;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821672E8:
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821672F0: lifted from generated/ by lift.py.
lifted::Result f821672F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821672F0 (lifted)");
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9612(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82167384
  if (cr6.lt) goto loc_82167384;
  // bne cr6,0x82167494
  if (!cr6.eq) goto loc_82167494;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82167494
  if (!cr0.eq) goto loc_82167494;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,27
  r9 = uint64_t(int64_t(27));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r11));
  // b 0x82167494
  goto loc_82167494;
loc_82167384:
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82167478
  if (!cr0.eq) goto loc_82167478;
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x82167478
  if (!cr6.gt) goto loc_82167478;
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
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // lwz r31,604(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r31,r31,r10
  r31 = r31 + r10;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // divwu r27,r31,r30
  r27 = uint32_t(uint32_t(r30) ? uint32_t(r31) / uint32_t(r30) : 0);
  // mulli r27,r27,50
  r27 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(50))));
  // subf r10,r27,r31
  r10 = r31 - r27;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,23340(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // addis r10,r10,6
  r10 = uint64_t(int64_t(int64_t(r10) + 393216));
  // addi r10,r10,-25884
  r10 = uint64_t(int64_t(int64_t(r10) + -25884));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,0,24,22
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
  // stw r28,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r28));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82167494
  goto loc_82167494;
loc_82167478:
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // blt cr6,0x82167494
  if (cr6.lt) goto loc_82167494;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82167494:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82167A40: lifted from generated/ by lift.py.
lifted::Result f82167A40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82167A40 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // lwz r11,9612(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82167ba4
  if (cr6.lt) goto loc_82167BA4;
  // beq cr6,0x82167b18
  if (cr6.eq) goto loc_82167B18;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82167bc8
  if (!cr6.lt) goto loc_82167BC8;
  // bl 0x82104898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82104898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r7,37
  r7 = uint64_t(int64_t(37));
  // ori r11,r11,39652
  r11 = r11 | 39652;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r10,23340(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 23340);
  // mr r4,r11
  r4 = r11;
  // mr r3,r11
  r3 = r11;
  // add r8,r10,r11
  r8 = r10 + r11;
  // addi r10,r31,204
  r10 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r31,50
  r31 = uint64_t(int64_t(50));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,0(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + 0);
  // rlwinm r11,r11,0,25,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFFFFFFF7F;
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // lwz r11,23340(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 23340);
  // add r11,r11,r4
  r11 = r11 + r4;
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r8,r8,0,27,25
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // lwz r11,23340(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 23340);
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r9,0,26,24
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFFBF;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 600);
  // lwz r9,604(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r9,r9,r11
  r9 = r9 + r11;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r11,600(r10)
  REX_STORE_U32(uint32_t(r10) + 600, uint32_t(r11));
  // divwu r8,r9,r31
  r8 = uint32_t(uint32_t(r31) ? uint32_t(r9) / uint32_t(r31) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r11,r8,r9
  r11 = r9 - r8;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r6));
  // stw r5,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r5));
  // b 0x82167bc8
  goto loc_82167BC8;
loc_82167B18:
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82167bc8
  if (!cr0.eq) goto loc_82167BC8;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r4,2
  r4 = uint64_t(int64_t(2));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r4,9612(r30)
  REX_STORE_U32(uint32_t(r30) + 9612, uint32_t(r4));
  // b 0x82167bc8
  goto loc_82167BC8;
loc_82167BA4:
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
  // lwz r3,7152(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9612(r30)
  REX_STORE_U32(uint32_t(r30) + 9612, uint32_t(r11));
loc_82167BC8:
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

// sub_82167BE0: lifted from generated/ by lift.py.
lifted::Result f82167BE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82167BE0 (lifted)");
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
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9612(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82167c6c
  if (cr6.lt) goto loc_82167C6C;
  // bne cr6,0x82167d7c
  if (!cr6.eq) goto loc_82167D7C;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82167d7c
  if (!cr0.eq) goto loc_82167D7C;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,38
  r9 = uint64_t(int64_t(38));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x82167d7c
  goto loc_82167D7C;
loc_82167C6C:
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82167d60
  if (!cr0.eq) goto loc_82167D60;
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x82167d60
  if (!cr6.gt) goto loc_82167D60;
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
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // lwz r31,604(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r31,r31,r10
  r31 = r31 + r10;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // divwu r27,r31,r30
  r27 = uint32_t(uint32_t(r30) ? uint32_t(r31) / uint32_t(r30) : 0);
  // mulli r27,r27,50
  r27 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(50))));
  // subf r10,r27,r31
  r10 = r31 - r27;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lwz r10,23340(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // addis r10,r10,6
  r10 = uint64_t(int64_t(int64_t(r10) + 393216));
  // addi r10,r10,-25884
  r10 = uint64_t(int64_t(int64_t(r10) + -25884));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,0,24,22
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r6,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r6));
  // stw r5,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r5));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
  // stw r28,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r28));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82167d7c
  goto loc_82167D7C;
loc_82167D60:
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // blt cr6,0x82167d7c
  if (cr6.lt) goto loc_82167D7C;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82167D7C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82167D88: lifted from generated/ by lift.py.
lifted::Result f82167D88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82167D88 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // mr r22,r3
  r22 = r3;
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8216800c
  if (!cr0.eq) goto loc_8216800C;
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // addi r30,r4,204
  r30 = uint64_t(int64_t(int64_t(r4) + 204));
  // ori r11,r11,39652
  r11 = r11 | 39652;
  // li r20,50
  r20 = uint64_t(int64_t(50));
  // mr r6,r11
  r6 = r11;
  // mr r5,r11
  r5 = r11;
  // mr r4,r11
  r4 = r11;
  // lwz r7,604(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 604);
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // ori r3,r11,39236
  r3 = r11 | 39236;
  // lwz r11,600(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 600);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // add r7,r7,r11
  r7 = r7 + r11;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r30)
  REX_STORE_U32(uint32_t(r30) + 600, uint32_t(r11));
  // divwu r31,r7,r20
  r31 = uint32_t(uint32_t(r20) ? uint32_t(r7) / uint32_t(r20) : 0);
  // mulli r31,r31,50
  r31 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(50))));
  // subf r11,r31,r7
  r11 = r7 - r31;
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // add r11,r11,r6
  r11 = r11 + r6;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // add r11,r11,r5
  r11 = r11 + r5;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,8192
  r10 = r10 | 8192;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // add r11,r11,r4
  r11 = r11 + r4;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,16384
  r10 = r10 | 16384;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // lwz r10,9624(r22)
  r10 = REX_LOAD_U32(uint32_t(r22) + 9624);
  // addi r27,r11,48
  r27 = uint64_t(int64_t(int64_t(r11) + 48));
  // lwzx r25,r11,r3
  r25 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  // lwz r3,0(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 0);
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82117178, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // mr r24,r3
  r24 = r3;
  // ori r11,r11,38368
  r11 = r11 | 38368;
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwzx r11,r27,r11
  r11 = REX_LOAD_U32(uint32_t(r27) + uint32_t(r11));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82167ebc
  if (!cr0.gt) goto loc_82167EBC;
  // addi r10,r27,3888
  r10 = uint64_t(int64_t(int64_t(r27) + 3888));
loc_82167E94:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // clrlwi. r9,r9,31
  r9 = uint32_t(r9) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82167eb4
  if (!cr0.eq) goto loc_82167EB4;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r10,r10,3936
  r10 = uint64_t(int64_t(int64_t(r10) + 3936));
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // blt cr6,0x82167e94
  if (cr6.lt) goto loc_82167E94;
  // b 0x82167ebc
  goto loc_82167EBC;
loc_82167EB4:
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r23,r3
  r23 = r3;
loc_82167EBC:
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // mr r5,r25
  r5 = r25;
  // ori r26,r11,38432
  r26 = r11 | 38432;
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // add r3,r11,r26
  r3 = r11 + r26;
  // bl 0x820ffa88
  // (left out: dead in C++)
  f820FFA88(base, r3, r4, r5);
  // li r21,-20
  r21 = uint64_t(int64_t(-20));
  // addi r19,r24,92
  r19 = uint64_t(int64_t(int64_t(r24) + 92));
loc_82167EE0:
  // lha r4,0(r19)
  r4 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r19) + 0))));
  // cmpwi cr6,r4,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r4), -1, so);
  // beq cr6,0x82167fa0
  if (cr6.eq) goto loc_82167FA0;
  // mr r3,r27
  r3 = r27;
  // bl 0x820ff9f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FF9F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // mr r29,r3
  r29 = r3;
  // mr r6,r25
  r6 = r25;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // add r3,r11,r26
  r3 = r11 + r26;
  // mr r4,r29
  r4 = r29;
  // bl 0x820ffab8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFAB8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r11,r23,8
  r11 = uint64_t(int64_t(int64_t(r23) + 8));
  // addi r28,r29,8
  r28 = uint64_t(int64_t(int64_t(r29) + 8));
  // extsh r4,r21
  r4 = uint64_t(int64_t(int16_t(r21)));
  // mr r3,r28
  r3 = r28;
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // rlwinm r10,r10,0,27,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFFFDF;
  // ori r10,r10,9232
  r10 = r10 | 9232;
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // sth r10,0(r28)
  REX_STORE_U16(uint32_t(r28) + 0, uint16_t(r10));
  // lhz r11,2(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 2);
  // sth r11,2(r28)
  REX_STORE_U16(uint32_t(r28) + 2, uint16_t(r11));
  // lbz r11,36(r24)
  r11 = REX_LOAD_U8(uint32_t(r24) + 36);
  // extsb r5,r11
  r5 = uint64_t(int64_t(int8_t(r11)));
  // bl 0x82170f18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 24);
  // mulli r10,r11,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3936))));
  // lwz r11,23340(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 23340);
  // add r11,r10,r11
  r11 = r10 + r11;
  // addic. r11,r11,48
  ca = uint32_t(r11) > 4294967247;
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82167f90
  if (cr0.eq) goto loc_82167F90;
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r11,r11,3904
  r11 = uint64_t(int64_t(int64_t(r11) + 3904));
  // ori r10,r10,4096
  r10 = r10 | 4096;
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
  // lhz r10,0(r28)
  r10 = REX_LOAD_U16(uint32_t(r28) + 0);
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // lhz r10,2(r28)
  r10 = REX_LOAD_U16(uint32_t(r28) + 2);
  // sth r10,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r10));
loc_82167F90:
  // addi r21,r21,-2
  r21 = uint64_t(int64_t(int64_t(r21) + -2));
  // addi r19,r19,2
  r19 = uint64_t(int64_t(int64_t(r19) + 2));
  // cmpwi cr6,r21,-26
  cr6 = lifted::Compare<int32_t>(int32_t(r21), -26, so);
  // bgt cr6,0x82167ee0
  if (cr6.gt) goto loc_82167EE0;
loc_82167FA0:
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -32012);
  // bl 0x82171978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r22
  r3 = r22;
  // bl 0x821662e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821662E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-32012(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -32012);
  // bl 0x82171e50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171E50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r10,22
  r10 = uint64_t(int64_t(22));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r11,9612(r22)
  REX_STORE_U32(uint32_t(r22) + 9612, uint32_t(r11));
  // lwz r11,600(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 600);
  // lwz r7,604(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r7,r7,r11
  r7 = r7 + r11;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r30)
  REX_STORE_U32(uint32_t(r30) + 600, uint32_t(r11));
  // divwu r6,r7,r20
  r6 = uint32_t(uint32_t(r20) ? uint32_t(r7) / uint32_t(r20) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
loc_8216800C:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82168018: lifted from generated/ by lift.py.
lifted::Result f82168018(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168018 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,9612(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821681f8
  if (cr6.lt) goto loc_821681F8;
  // beq cr6,0x8216819c
  if (cr6.eq) goto loc_8216819C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821683a0
  if (!cr6.lt) goto loc_821683A0;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821683a0
  if (!cr0.eq) goto loc_821683A0;
  // addi r31,r31,204
  r31 = uint64_t(int64_t(int64_t(r31) + 204));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // ori r29,r11,39652
  r29 = r11 | 39652;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r10,11
  r10 = uint64_t(int64_t(11));
  // lwz r25,604(r31)
  r25 = REX_LOAD_U32(uint32_t(r31) + 604);
  // mr r9,r26
  r9 = r26;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r8,r26
  r8 = r26;
  // add r25,r25,r11
  r25 = r25 + r11;
  // mr r27,r26
  r27 = r26;
  // addi r25,r25,-1
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // li r7,31
  r7 = uint64_t(int64_t(31));
  // divwu r24,r25,r30
  r24 = uint32_t(uint32_t(r30) ? uint32_t(r25) / uint32_t(r30) : 0);
  // mr r6,r26
  r6 = r26;
  // mulli r24,r24,50
  r24 = uint64_t(int64_t(static_cast<int64_t>(r24 * static_cast<uint64_t>(50))));
  // subf r11,r24,r25
  r11 = r25 - r24;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // lwz r10,604(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r10,r11
  r10 = r10 + r11;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r9,r10,r30
  r9 = uint32_t(uint32_t(r30) ? uint32_t(r10) / uint32_t(r30) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r11,r9,r10
  r11 = r10 - r9;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r27,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r27));
  // lis r27,-32148
  r27 = uint64_t(int64_t(-2106851328));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r6));
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // add r11,r11,r29
  r11 = r11 + r29;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x82165e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82165E20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -32012);
  // bl 0x82171e50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171E50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // li r10,25
  r10 = uint64_t(int64_t(25));
  // mr r9,r26
  r9 = r26;
  // add r11,r11,r29
  r11 = r11 + r29;
  // mr r8,r26
  r8 = r26;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r7,r7,0,19,17
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFDFFF;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lwz r11,23340(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 23340);
  // add r11,r11,r29
  r11 = r11 + r29;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r7,r7,0,18,16
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFBFFF;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // lwz r7,604(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // add r11,r7,r11
  r11 = r7 + r11;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // divwu r7,r11,r30
  r7 = uint32_t(uint32_t(r30) ? uint32_t(r11) / uint32_t(r30) : 0);
  // mulli r7,r7,50
  r7 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(50))));
  // subf r11,r7,r11
  r11 = r11 - r7;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // stw r26,9612(r28)
  REX_STORE_U32(uint32_t(r28) + 9612, uint32_t(r26));
  // b 0x821683a0
  goto loc_821683A0;
loc_8216819C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821681d0
  if (cr0.eq) goto loc_821681D0;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // rlwinm r9,r9,0,0,30
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFE;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,9612(r28)
  REX_STORE_U32(uint32_t(r28) + 9612, uint32_t(r10));
  // b 0x821683a0
  goto loc_821683A0;
loc_821681D0:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26400
  r6 = uint64_t(int64_t(int64_t(r11) + -26400));
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r11));
  // b 0x821683a0
  goto loc_821683A0;
loc_821681F8:
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82168368
  if (!cr0.eq) goto loc_82168368;
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x82168368
  if (!cr6.gt) goto loc_82168368;
  // bl 0x82163218
  // (left out: dead in C++)
  { const lifted::Result o = f82163218(base, r1, r13, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82168368
  if (cr0.eq) goto loc_82168368;
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // mr r8,r26
  r8 = r26;
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // mr r7,r26
  r7 = r26;
  // lwz r25,604(r11)
  r25 = REX_LOAD_U32(uint32_t(r11) + 604);
  // li r6,32
  r6 = uint64_t(int64_t(32));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // mr r5,r26
  r5 = r26;
  // add r25,r25,r10
  r25 = r25 + r10;
  // mr r4,r26
  r4 = r26;
  // addi r25,r25,-1
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // li r3,10
  r3 = uint64_t(int64_t(10));
  // divwu r24,r25,r30
  r24 = uint32_t(uint32_t(r30) ? uint32_t(r25) / uint32_t(r30) : 0);
  // mr r31,r26
  r31 = r26;
  // mulli r24,r24,50
  r24 = uint64_t(int64_t(static_cast<int64_t>(r24 * static_cast<uint64_t>(50))));
  // subf r10,r24,r25
  r10 = r25 - r24;
  // mr r29,r26
  r29 = r26;
  // li r27,23
  r27 = uint64_t(int64_t(23));
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r7,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r7));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lwz r10,23340(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // addis r10,r10,6
  r10 = uint64_t(int64_t(int64_t(r10) + 393216));
  // addi r10,r10,-25884
  r10 = uint64_t(int64_t(int64_t(r10) + -25884));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,0,24,22
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r6,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r6));
  // stw r5,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r5));
  // stw r4,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r4));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r8,r9,r30
  r8 = uint32_t(uint32_t(r30) ? uint32_t(r9) / uint32_t(r30) : 0);
  // mulli r8,r8,50
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(50))));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // mr r9,r26
  r9 = r26;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r3,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r3));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r31,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r31));
  // stw r29,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r29));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r8,604(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r6,r8,r30
  r6 = uint32_t(uint32_t(r30) ? uint32_t(r8) / uint32_t(r30) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r10,r6,r8
  r10 = r8 - r6;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // stw r26,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r26));
  // stw r7,9612(r28)
  REX_STORE_U32(uint32_t(r28) + 9612, uint32_t(r7));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821683a0
  goto loc_821683A0;
loc_82168368:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26520
  r6 = uint64_t(int64_t(int64_t(r11) + -26520));
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // stw r11,9616(r28)
  REX_STORE_U32(uint32_t(r28) + 9616, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x821683a0
  if (cr6.lt) goto loc_821683A0;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821683A0:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821683A8: lifted from generated/ by lift.py.
lifted::Result f821683A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821683A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,9612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8216843c
  if (cr6.lt) goto loc_8216843C;
  // bne cr6,0x821684e4
  if (!cr6.eq) goto loc_821684E4;
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821684e4
  if (!cr0.eq) goto loc_821684E4;
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x821684e4
  goto loc_821684E4;
loc_8216843C:
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x821684ac
  if (!cr0.eq) goto loc_821684AC;
  // lwz r11,9616(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x821684ac
  if (!cr6.gt) goto loc_821684AC;
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821684e4
  goto loc_821684E4;
loc_821684AC:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26520
  r6 = uint64_t(int64_t(int64_t(r11) + -26520));
  // lwz r11,9616(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9616);
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // stw r11,9616(r31)
  REX_STORE_U32(uint32_t(r31) + 9616, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x821684e4
  if (cr6.lt) goto loc_821684E4;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821684E4:
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

// sub_82168620: lifted from generated/ by lift.py.
lifted::Result f82168620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_82168620 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f13 = 0.0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // addi r6,r11,-26372
  r6 = uint64_t(int64_t(int64_t(r11) + -26372));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x821686e4
  if (cr6.lt) goto loc_821686E4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // bl 0x820ffc68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r9,48
  r9 = uint64_t(int64_t(48));
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
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // bne 0x821686c8
  if (!cr0.eq) goto loc_821686C8;
  // li r9,7
  r9 = uint64_t(int64_t(7));
loc_821686C8:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r3,9624(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 9624);
  // bl 0x82163440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82163440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,9624(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 9624);
  // bl 0x821648e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821648E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821686e8
  goto loc_821686E8;
loc_821686E4:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
loc_821686E8:
  // stw r11,9616(r30)
  REX_STORE_U32(uint32_t(r30) + 9616, uint32_t(r11));
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

// sub_82168708: lifted from generated/ by lift.py.
lifted::Result f82168708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168708 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // mr r31,r4
  r31 = r4;
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82168850
  if (!cr0.eq) goto loc_82168850;
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // lis r10,10
  r10 = uint64_t(int64_t(655360));
  // ori r30,r11,39652
  r30 = r11 | 39652;
  // ori r10,r10,15088
  r10 = r10 | 15088;
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r9,0,19,17
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFDFFF;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r9,r9,0,18,16
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFFBFFF;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r9,r9,128
  r9 = r9 | 128;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r9,r9,64
  r9 = r9 | 64;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x821687a8
  if (cr6.eq) goto loc_821687A8;
  // bl 0x82104e68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82104E68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821687A8:
  // addi r31,r31,204
  r31 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r28,50
  r28 = uint64_t(int64_t(50));
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // lwz r7,604(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r7,r7,r11
  r7 = r7 + r11;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r6,r7,r28
  r6 = uint32_t(uint32_t(r28) ? uint32_t(r7) / uint32_t(r28) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821046f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821046F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r10,43
  r10 = uint64_t(int64_t(43));
  // lwz r7,604(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 604);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // add r7,r7,r11
  r7 = r7 + r11;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r6,r7,r28
  r6 = uint32_t(uint32_t(r28) ? uint32_t(r7) / uint32_t(r28) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
loc_82168850:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82168858: lifted from generated/ by lift.py.
lifted::Result f82168858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168858 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // ori r11,r11,39652
  r11 = r11 | 39652;
  // mr r31,r4
  r31 = r4;
  // mr r9,r11
  r9 = r11;
  // mr r8,r11
  r8 = r11;
  // lwz r11,23340(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r9,r9,128
  r9 = r9 | 128;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,23340(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 23340);
  // add r11,r11,r8
  r11 = r11 + r8;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,64
  r10 = r10 | 64;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821046f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821046F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,43
  r9 = uint64_t(int64_t(43));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
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

// sub_82168910: lifted from generated/ by lift.py.
lifted::Result f82168910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168910 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // mr r29,r3
  r29 = r3;
  // mr r28,r4
  r28 = r4;
  // bl 0x8209d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82168a68
  if (!cr0.eq) goto loc_82168A68;
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // ble cr6,0x82168a68
  if (!cr6.gt) goto loc_82168A68;
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
  // addi r31,r28,204
  r31 = uint64_t(int64_t(int64_t(r28) + 204));
  // mr r4,r28
  r4 = r28;
  // li r30,50
  r30 = uint64_t(int64_t(50));
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r28,604(r31)
  r28 = REX_LOAD_U32(uint32_t(r31) + 604);
  // li r7,32
  r7 = uint64_t(int64_t(32));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // add r28,r28,r11
  r28 = r28 + r11;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r28,r28,-1
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // divwu r26,r28,r30
  r26 = uint32_t(uint32_t(r30) ? uint32_t(r28) / uint32_t(r30) : 0);
  // mr r3,r29
  r3 = r29;
  // mulli r26,r26,50
  r26 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(50))));
  // subf r11,r26,r28
  r11 = r28 - r26;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,24,22
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFFFEFF;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // lwz r10,604(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r10,r11
  r10 = r10 + r11;
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r9,r10,r30
  r9 = uint32_t(uint32_t(r30) ? uint32_t(r10) / uint32_t(r30) : 0);
  // mulli r9,r9,50
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(50))));
  // subf r11,r9,r10
  r11 = r10 - r9;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r6));
  // stw r5,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r5));
  // stw r27,9612(r29)
  REX_STORE_U32(uint32_t(r29) + 9612, uint32_t(r27));
  // bl 0x821654f8
  // (left out: dead in C++)
  { const lifted::Result o = f821654F8(base, r1, r13, r3, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
  // lwz r11,600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 600);
  // li r10,26
  r10 = uint64_t(int64_t(26));
  // lwz r7,604(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 604);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // add r7,r7,r11
  r7 = r7 + r11;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r31)
  REX_STORE_U32(uint32_t(r31) + 600, uint32_t(r11));
  // divwu r6,r7,r30
  r6 = uint32_t(uint32_t(r30) ? uint32_t(r7) / uint32_t(r30) : 0);
  // mulli r11,r6,50
  r11 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r11,r7
  r11 = r7 - r11;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // bl 0x82247a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247A60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82168aa0
  goto loc_82168AA0;
loc_82168A68:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-26520
  r6 = uint64_t(int64_t(int64_t(r11) + -26520));
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // li r4,420
  r4 = uint64_t(int64_t(420));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // li r3,400
  r3 = uint64_t(int64_t(400));
  // stw r11,9616(r29)
  REX_STORE_U32(uint32_t(r29) + 9616, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9616);
  // cmplwi cr6,r11,30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 30, so);
  // blt cr6,0x82168aa0
  if (cr6.lt) goto loc_82168AA0;
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82247898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82247898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82168AA0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82168AA8: lifted from generated/ by lift.py.
lifted::Result f82168AA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_82168AA8 (lifted)");
  uint64_t r3 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f13 = 0.0;
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
  // li r5,671
  r5 = uint64_t(int64_t(671));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x822478d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822478D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82168af0
  if (!cr0.eq) goto loc_82168AF0;
  // li r9,46
  r9 = uint64_t(int64_t(46));
  // b 0x82168b20
  goto loc_82168B20;
loc_82168AF0:
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bne cr6,0x82168b00
  if (!cr6.eq) goto loc_82168B00;
  // li r9,42
  r9 = uint64_t(int64_t(42));
  // b 0x82168b20
  goto loc_82168B20;
loc_82168B00:
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // bl 0x820ffbe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFBE8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,4
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 4, so);
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // bne cr6,0x82168b20
  if (!cr6.eq) goto loc_82168B20;
  // li r9,5
  r9 = uint64_t(int64_t(5));
loc_82168B20:
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
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

// sub_82168C80: lifted from generated/ by lift.py.
lifted::Result f82168C80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168C80 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // mr r20,r3
  r20 = r3;
  // mr r31,r4
  r31 = r4;
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82168f48
  if (!cr0.eq) goto loc_82168F48;
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lis r9,5
  r9 = uint64_t(int64_t(327680));
  // li r21,0
  r21 = uint64_t(int64_t(0));
  // ori r9,r9,38368
  r9 = r9 | 38368;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // addi r10,r11,48
  r10 = uint64_t(int64_t(int64_t(r11) + 48));
  // addis r28,r11,6
  r28 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r28,r28,-27104
  r28 = uint64_t(int64_t(int64_t(r28) + -27104));
  // lwzx r11,r10,r9
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x82168d08
  if (!cr0.gt) goto loc_82168D08;
  // addi r10,r10,3888
  r10 = uint64_t(int64_t(int64_t(r10) + 3888));
loc_82168CE0:
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // clrlwi. r9,r9,31
  r9 = uint32_t(r9) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82168d00
  if (!cr0.eq) goto loc_82168D00;
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
  // addi r10,r10,3936
  r10 = uint64_t(int64_t(int64_t(r10) + 3936));
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // blt cr6,0x82168ce0
  if (cr6.lt) goto loc_82168CE0;
  // b 0x82168d08
  goto loc_82168D08;
loc_82168D00:
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r21,r3
  r21 = r3;
loc_82168D08:
  // addi r30,r31,204
  r30 = uint64_t(int64_t(int64_t(r31) + 204));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r22,50
  r22 = uint64_t(int64_t(50));
  // ori r31,r11,39652
  r31 = r11 | 39652;
  // li r10,31
  r10 = uint64_t(int64_t(31));
  // lwz r11,600(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 600);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r7,604(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 604);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r3,r28
  r3 = r28;
  // add r7,r7,r11
  r7 = r7 + r11;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r30)
  REX_STORE_U32(uint32_t(r30) + 600, uint32_t(r11));
  // divwu r6,r7,r22
  r6 = uint32_t(uint32_t(r22) ? uint32_t(r7) / uint32_t(r22) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,256
  r10 = r10 | 256;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,8192
  r10 = r10 | 8192;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,16384
  r10 = r10 | 16384;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r27,804(r28)
  r27 = REX_LOAD_U32(uint32_t(r28) + 804);
  // mr r4,r27
  r4 = r27;
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x82168dfc
  if (!cr0.gt) goto loc_82168DFC;
loc_82168DB4:
  // mr r5,r27
  r5 = r27;
  // mr r4,r26
  r4 = r26;
  // mr r3,r28
  r3 = r28;
  // bl 0x820ffc98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rlwinm. r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82168de4
  if (!cr0.eq) goto loc_82168DE4;
  // rlwinm. r10,r11,0,28,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82168de4
  if (!cr0.eq) goto loc_82168DE4;
  // ori r11,r11,32
  r11 = r11 | 32;
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
loc_82168DE4:
  // mr r4,r27
  r4 = r27;
  // mr r3,r28
  r3 = r28;
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r26,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r3), so);
  // blt cr6,0x82168db4
  if (cr6.lt) goto loc_82168DB4;
loc_82168DFC:
  // mr r3,r28
  r3 = r28;
  // bl 0x82101940
  // (left out: dead in C++)
  r3 = f82101940(base, r1, r13, r3);
  // lwz r24,804(r28)
  r24 = REX_LOAD_U32(uint32_t(r28) + 804);
  // mr r3,r28
  r3 = r28;
  // mr r4,r24
  r4 = r24;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x82168eb8
  if (!cr0.gt) goto loc_82168EB8;
  // li r23,-10
  r23 = uint64_t(int64_t(-10));
loc_82168E24:
  // mr r5,r24
  r5 = r24;
  // mr r4,r25
  r4 = r25;
  // mr r3,r28
  r3 = r28;
  // bl 0x820ffc98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821705a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821705A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r26,r27,8
  r26 = uint64_t(int64_t(int64_t(r27) + 8));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // extsh r4,r23
  r4 = uint64_t(int64_t(int16_t(r23)));
  // lwz r10,0(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 0);
  // mr r3,r26
  r3 = r26;
  // rlwimi r10,r11,4,26,27
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0x30) | (r10 & 0xFFFFFFFFFFFFFFCF);
  // stw r10,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r10));
  // lhz r11,8(r21)
  r11 = REX_LOAD_U16(uint32_t(r21) + 8);
  // sth r11,0(r26)
  REX_STORE_U16(uint32_t(r26) + 0, uint16_t(r11));
  // lhz r11,10(r21)
  r11 = REX_LOAD_U16(uint32_t(r21) + 10);
  // sth r11,2(r26)
  REX_STORE_U16(uint32_t(r26) + 2, uint16_t(r11));
  // bl 0x82170f90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82170F90, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,24(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 24);
  // mulli r10,r11,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3936))));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r10,r11
  r11 = r10 + r11;
  // addic. r11,r11,48
  ca = uint32_t(r11) > 4294967247;
  r11 = uint64_t(int64_t(int64_t(r11) + 48));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82168e9c
  if (cr0.eq) goto loc_82168E9C;
  // lhz r10,0(r26)
  r10 = REX_LOAD_U16(uint32_t(r26) + 0);
  // addi r11,r11,3904
  r11 = uint64_t(int64_t(int64_t(r11) + 3904));
  // sth r10,0(r11)
  REX_STORE_U16(uint32_t(r11) + 0, uint16_t(r10));
  // lhz r10,2(r26)
  r10 = REX_LOAD_U16(uint32_t(r26) + 2);
  // sth r10,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r10));
loc_82168E9C:
  // mr r4,r24
  r4 = r24;
  // mr r3,r28
  r3 = r28;
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // addi r23,r23,-2
  r23 = uint64_t(int64_t(int64_t(r23) + -2));
  // bl 0x820ffc70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFC70, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r25,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r25), int32_t(r3), so);
  // blt cr6,0x82168e24
  if (cr6.lt) goto loc_82168E24;
loc_82168EB8:
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -32012);
  // bl 0x82171978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r20
  r3 = r20;
  // bl 0x82165e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82165E20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-32012(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -32012);
  // bl 0x82171e50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82171E50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // li r10,17
  r10 = uint64_t(int64_t(17));
  // add r11,r11,r31
  r11 = r11 + r31;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r7,r7,0,19,17
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFDFFF;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lwz r11,23340(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 23340);
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r7,r7,0,18,16
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFFFFFFFBFFF;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lwz r11,600(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 600);
  // lwz r7,604(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r7,r7,r11
  r7 = r7 + r11;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r11,600(r30)
  REX_STORE_U32(uint32_t(r30) + 600, uint32_t(r11));
  // divwu r6,r7,r22
  r6 = uint32_t(uint32_t(r22) ? uint32_t(r7) / uint32_t(r22) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r11,r6,r7
  r11 = r7 - r6;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
loc_82168F48:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82168FB8: lifted from generated/ by lift.py.
lifted::Result f82168FB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82168FB8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x82164b50
  // (left out: dead in C++)
  { const lifted::Result o = f82164B50(base, r1, r13, r3, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,204
  r11 = uint64_t(int64_t(int64_t(r31) + 204));
  // li r6,50
  r6 = uint64_t(int64_t(50));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r7,604(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r7,r7,r10
  r7 = r7 + r10;
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r6,r7,r6
  r6 = uint32_t(uint32_t(r6) ? uint32_t(r7) / uint32_t(r6) : 0);
  // mulli r6,r6,50
  r6 = uint64_t(int64_t(static_cast<int64_t>(r6 * static_cast<uint64_t>(50))));
  // subf r10,r6,r7
  r10 = r7 - r6;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r3,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r3));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
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

// sub_82169030: lifted from generated/ by lift.py.
lifted::Result f82169030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82169030 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // mr r27,r3
  r27 = r3;
  // mr r26,r4
  r26 = r4;
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or. r11,r11,r10
  r11 = r11 | r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169284
  if (!cr0.eq) goto loc_82169284;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r10,-21596
  r10 = uint64_t(int64_t(int64_t(r10) + -21596));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // add r31,r9,r11
  r31 = r9 + r11;
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // rlwinm r8,r9,3,0,28
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r9,8(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 8);
  // lwz r10,12(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 12);
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r8,r11
  r30 = r8 + r11;
  // add r29,r9,r11
  r29 = r9 + r11;
  // add r28,r10,r11
  r28 = r10 + r11;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821690c8
  if (cr6.eq) goto loc_821690C8;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,152
  r4 = uint64_t(int64_t(152));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_821690C8:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821690f4
  if (cr6.eq) goto loc_821690F4;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_821690F4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82169120
  if (cr6.eq) goto loc_82169120;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,152
  r4 = uint64_t(int64_t(152));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_82169120:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8216914c
  if (cr6.eq) goto loc_8216914C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_8216914C:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82169178
  if (cr6.eq) goto loc_82169178;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,152
  r4 = uint64_t(int64_t(152));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_82169178:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821691a4
  if (cr6.eq) goto loc_821691A4;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_821691A4:
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821691d0
  if (cr6.eq) goto loc_821691D0;
  // lwz r3,0(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 0);
  // li r4,152
  r4 = uint64_t(int64_t(152));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_821691D0:
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821691fc
  if (cr6.eq) goto loc_821691FC;
  // lwz r3,0(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82169214
  if (!cr0.eq) goto loc_82169214;
loc_821691FC:
  // lwz r11,9616(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 9616);
  // cmplwi cr6,r11,1800
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1800, so);
  // bge cr6,0x82169214
  if (!cr6.lt) goto loc_82169214;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,9616(r27)
  REX_STORE_U32(uint32_t(r27) + 9616, uint32_t(r11));
  // b 0x82169284
  goto loc_82169284;
loc_82169214:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,9616(r27)
  REX_STORE_U32(uint32_t(r27) + 9616, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r3,7152(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r26,204
  r11 = uint64_t(int64_t(int64_t(r26) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,48
  r9 = uint64_t(int64_t(48));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
loc_82169284:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82169290: lifted from generated/ by lift.py.
lifted::Result f82169290(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82169290 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // ori r10,r11,15092
  r10 = r11 | 15092;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 23340);
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821692c4
  if (cr6.eq) goto loc_821692C4;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // addi r6,r11,-26356
  r6 = uint64_t(int64_t(int64_t(r11) + -26356));
  // li r4,100
  r4 = uint64_t(int64_t(100));
  // li r3,500
  r3 = uint64_t(int64_t(500));
  // b 0x82106540
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_821692C4:
  // b 0x82166658
  { const lifted::Result o = f82166658(base, r1, r13, r3, r4, r7, r8, r9, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8216C3E8: lifted from generated/ by lift.py.
lifted::Result f8216C3E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_8216C3E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x822126c8
  // (left out: dead in C++)
  r3 = f822126C8(base, r1, r13, r3);
  // lwz r11,9612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9612);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x8216c438
  if (!cr0.gt) goto loc_8216C438;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // ble cr6,0x8216c42c
  if (!cr6.gt) goto loc_8216C42C;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8216c44c
  if (cr6.eq) goto loc_8216C44C;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x8216c438
  if (!cr6.eq) goto loc_8216C438;
loc_8216C42C:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212888
  // (left out: dead in C++)
  { const lifted::Result o = f82212888(base, r1, r13, r3, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
loc_8216C438:
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
loc_8216C44C:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82215d08
  // (left out: dead in C++)
  { const lifted::Result o = f82215D08(base, r1, r13, r3, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
  // b 0x8216c438
  goto loc_8216C438;
}

// sub_8216C460: lifted from generated/ by lift.py.
lifted::Result f8216C460(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216C460 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // addi r3,r11,1004
  r3 = uint64_t(int64_t(int64_t(r11) + 1004));
  // b 0x82216c00
  { const lifted::Result o = lifted::Call(base, 0x82216C00, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8216C470: lifted from generated/ by lift.py.
uint64_t f8216C470(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8216C470 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // b 0x822126c8
  r3 = f822126C8(base, r1, r13, r3);
  return r3;
}

// sub_8216C480: lifted from generated/ by lift.py.
lifted::Result f8216C480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216C480 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x822128a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822128A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212ae8
  // (left out: dead in C++)
  { const lifted::Result o = f82212AE8(base, r1, r13, r3, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
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

// sub_8216C4C0: lifted from generated/ by lift.py.
lifted::Result f8216C4C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216C4C0 (lifted)");
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
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r31,r11,16
  r31 = uint64_t(int64_t(int64_t(r11) + 16));
  // mr r3,r31
  r3 = r31;
  // bl 0x822153a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822153A0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82212b68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212B68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_8216C500: lifted from generated/ by lift.py.
lifted::Result f8216C500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_8216C500 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // b 0x82212868
  { const lifted::Result o = f82212868(base, r1, r13, r3, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8216C648: lifted from generated/ by lift.py.
void f8216C648(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8216C648 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9616(r3)
  REX_STORE_U32(uint32_t(r3) + 9616, uint32_t(r11));
  // blr
  return;
}

// sub_8216CEB8: lifted from generated/ by lift.py.
lifted::Result f8216CEB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216CEB8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9612(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9612);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blelr
  if (!cr0.gt) return {r3, f1};
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // ble cr6,0x8216ceec
  if (!cr6.gt) goto loc_8216CEEC;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x8216cee0
  if (cr6.eq) goto loc_8216CEE0;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x8216ceec
  if (cr6.eq) goto loc_8216CEEC;
  // blr
  return {r3, f1};
loc_8216CEE0:
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // addi r3,r11,1004
  r3 = uint64_t(int64_t(int64_t(r11) + 1004));
  // b 0x82217778
  { const lifted::Result o = lifted::Call(base, 0x82217778, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_8216CEEC:
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beqlr
  if (cr0.eq) return {r3, f1};
  // addi r3,r11,1004
  r3 = uint64_t(int64_t(int64_t(r11) + 1004));
  // b 0x82217778
  { const lifted::Result o = lifted::Call(base, 0x82217778, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8216D0C8: lifted from generated/ by lift.py.
lifted::Result f8216D0C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216D0C8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,9612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9612);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8216d178
  if (cr6.lt) goto loc_8216D178;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x8216d0fc
  if (cr6.lt) goto loc_8216D0FC;
  // beq cr6,0x8216d148
  if (cr6.eq) goto loc_8216D148;
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // bne cr6,0x8216d184
  if (!cr6.eq) goto loc_8216D184;
loc_8216D0FC:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8216d184
  if (!cr0.eq) goto loc_8216D184;
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x822128a8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822128A8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x822141e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822141E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212a38
  // (left out: dead in C++)
  { const lifted::Result o = f82212A38(base, r1, r13, r3, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212ac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82212AC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,9608(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // bl 0x82211d08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82211D08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x8216d184
  goto loc_8216D184;
loc_8216D148:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82215d58
  // (left out: dead in C++)
  { const lifted::Result o = f82215D58(base, r1, r13, r4, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82215da8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82215DA8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // lwz r3,1016(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 1016);
  // bl 0x82211590
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82211590, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x8216d184
  goto loc_8216D184;
loc_8216D178:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,12
  r3 = uint64_t(int64_t(int64_t(r11) + 12));
  // bl 0x8220b9e8
  // (left out: dead in C++)
  { const lifted::Result o = f8220B9E8(base, r1, r13, r8, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
loc_8216D184:
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

// sub_8216D270: lifted from generated/ by lift.py.
lifted::Result f8216D270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8216D270 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // mr r28,r4
  r28 = r4;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-25880
  r3 = uint64_t(int64_t(int64_t(r3) + -25880));
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820FFE58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9608(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9608);
  // mr r31,r3
  r31 = r3;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // clrlwi. r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8216d2f0
  if (!cr0.eq) goto loc_8216D2F0;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // lwz r10,3892(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 3892);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r10,23340(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 23340);
  // lwz r10,60(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 60);
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-32024(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -32024);
  // lwz r10,2196(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 2196);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lwz r3,1028(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 1028);
  // bl 0x8221f990
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8221F990, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8216D2F0:
  // lwz r11,3920(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 3920);
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r5,r11,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // lwz r11,9608(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9608);
  // lwz r3,1020(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 1020);
  // bne cr6,0x8216d318
  if (!cr6.eq) goto loc_8216D318;
  // li r4,5
  r4 = uint64_t(int64_t(5));
loc_8216D318:
  // bl 0x8221ba68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8221BA68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8216E668: lifted from generated/ by lift.py.
lifted::Result f8216E668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8216E668 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,9612(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9612);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x8216e6b4
  if (!cr0.gt) goto loc_8216E6B4;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // ble cr6,0x8216e6a8
  if (!cr6.gt) goto loc_8216E6A8;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8216e6d8
  if (cr6.eq) goto loc_8216E6D8;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x8216e6b4
  if (!cr6.eq) goto loc_8216E6B4;
loc_8216E6A8:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,16
  r3 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x82212888
  // (left out: dead in C++)
  { const lifted::Result o = f82212888(base, r1, r13, r3, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
loc_8216E6B4:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8216e5b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8216E5B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
loc_8216E6D8:
  // lwz r11,9608(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9608);
  // addi r3,r11,656
  r3 = uint64_t(int64_t(int64_t(r11) + 656));
  // bl 0x82215d08
  // (left out: dead in C++)
  { const lifted::Result o = f82215D08(base, r1, r13, r3, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12); r3 = o.r3; f1 = o.f1; }
  // b 0x8216e6b4
  goto loc_8216E6B4;
}

// sub_82170550: lifted from generated/ by lift.py.
lifted::Result f82170550(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82170550 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23336(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23336);
  // b 0x820ff368
  { const lifted::Result o = f820FF368(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82171308: lifted from generated/ by lift.py.
lifted::Result f82171308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82171308 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // bl 0x820c3b78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C3B78, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x8213b2b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213B2B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32012(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -32012);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r11,-32012(r31)
  REX_STORE_U32(uint32_t(r31) + -32012, uint32_t(r11));
  // stw r10,9604(r30)
  REX_STORE_U32(uint32_t(r30) + 9604, uint32_t(r10));
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

// sub_82179F60: lifted from generated/ by lift.py.
lifted::Result f82179F60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82179F60 (lifted)");
  uint64_t r3 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // mr r31,r4
  r31 = r4;
  // addi r11,r11,24208
  r11 = uint64_t(int64_t(int64_t(r11) + 24208));
  // mulli r10,r5,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(48))));
  // lfs f0,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f0 = double(temp.f32);
  // lwz r30,12(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lfsx f31,r10,r11
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fdivs f12,f31,f0
  f12 = double(float(f31 / f0));
  // lfs f0,20(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 20);
  f0 = double(temp.f32);
  // lfs f13,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x82179fc0
  if (cr6.lt) goto loc_82179FC0;
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x82179fc4
  if (!cr6.gt) goto loc_82179FC4;
  // fmr f0,f31
  f0 = f31;
  // b 0x82179fc4
  goto loc_82179FC4;
loc_82179FC0:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_82179FC4:
  // stfs f0,20(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // stfs f13,24(r30)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + 24, temp.u32);
  // lfs f0,20(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bgt cr6,0x82179fe0
  if (cr6.gt) goto loc_82179FE0;
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82179FE0:
  // lfs f0,20(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x82179ff8
  if (cr6.lt) goto loc_82179FF8;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // stw r29,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r29));
  // stfs f31,20(r30)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
loc_82179FF8:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8217A008: lifted from generated/ by lift.py.
lifted::Result f8217A008(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8217A008 (lifted)");
  uint64_t r3 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
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
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // lfs f13,4(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f13 = double(temp.f32);
  // mulli r9,r5,48
  r9 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(48))));
  // addi r10,r11,24208
  r10 = uint64_t(int64_t(int64_t(r11) + 24208));
  // lwz r11,12(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 12);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r10,r10,20
  r10 = uint64_t(int64_t(int64_t(r10) + 20));
  // lfs f12,20(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f12 = double(temp.f32);
  // lfsx f0,r9,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r10));
  f0 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fdivs f13,f0,f13
  f13 = double(float(f0 / f13));
  // fsubs f13,f12,f13
  f13 = double(float(f12 - f13));
  // lfs f12,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // blt cr6,0x8217a064
  if (cr6.lt) goto loc_8217A064;
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x8217a068
  if (cr6.gt) goto loc_8217A068;
  // fmr f0,f13
  f0 = f13;
  // b 0x8217a068
  goto loc_8217A068;
loc_8217A064:
  // fmr f0,f12
  lifted::ScalarFloatMode();
  f0 = f12;
loc_8217A068:
  // stfs f0,20(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // stfs f12,24(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // lfs f0,20(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bgt cr6,0x8217a090
  if (cr6.gt) goto loc_8217A090;
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // li r3,13
  r3 = uint64_t(int64_t(13));
  // stw r31,20(r4)
  REX_STORE_U32(uint32_t(r4) + 20, uint32_t(r31));
  // stfs f12,20(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8217A090:
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

// sub_8217A0A8: lifted from generated/ by lift.py.
lifted::Result f8217A0A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8217A0A8 (lifted)");
  uint64_t r3 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
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
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
  // mulli r11,r5,48
  r11 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(48))));
  // addi r10,r10,24208
  r10 = uint64_t(int64_t(int64_t(r10) + 24208));
  // mr r30,r4
  r30 = r4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r9,r10,36
  r9 = uint64_t(int64_t(int64_t(r10) + 36));
  // addi r10,r10,40
  r10 = uint64_t(int64_t(int64_t(r10) + 40));
  // lfs f13,4(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f13 = double(temp.f32);
  // lwz r31,12(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 12);
  // lfsx f31,r11,r9
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  f31 = double(temp.f32);
  // lfsx f29,r11,r10
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  f29 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10600(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10600);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f30,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
  // fdivs f13,f31,f0
  f13 = double(float(f31 / f0));
  // lfs f0,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f0 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // blt cr6,0x8217a124
  if (cr6.lt) goto loc_8217A124;
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x8217a128
  if (!cr6.gt) goto loc_8217A128;
  // fmr f0,f31
  f0 = f31;
  // b 0x8217a128
  goto loc_8217A128;
loc_8217A124:
  // fmr f0,f30
  lifted::ScalarFloatMode();
  f0 = f30;
loc_8217A128:
  // stfs f0,20(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 20, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x8217a13c
  if (cr6.gt) goto loc_8217A13C;
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8217A13C:
  // lfs f0,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f0 = double(temp.f32);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8217a154
  if (cr6.lt) goto loc_8217A154;
  // stfs f31,20(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 20, temp.u32);
  // stw r10,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r10));
loc_8217A154:
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8217a168
  if (!cr6.eq) goto loc_8217A168;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8217a1bc
  goto loc_8217A1BC;
loc_8217A168:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,4(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 4);
  f13 = double(temp.f32);
  // lfs f0,10588(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10588);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f13,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f13 = double(temp.f32);
  // fdivs f0,f29,f0
  f0 = double(float(f29 / f0));
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // blt cr6,0x8217a19c
  if (cr6.lt) goto loc_8217A19C;
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // ble cr6,0x8217a1a0
  if (!cr6.gt) goto loc_8217A1A0;
  // fmr f0,f29
  f0 = f29;
  // b 0x8217a1a0
  goto loc_8217A1A0;
loc_8217A19C:
  // fmr f0,f30
  lifted::ScalarFloatMode();
  f0 = f30;
loc_8217A1A0:
  // stfs f0,24(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 24, temp.u32);
  // fcmpu cr6,f0,f29
  cr6 = lifted::CompareFloat(f0, f29);
  // blt cr6,0x8217a1b8
  if (cr6.lt) goto loc_8217A1B8;
  // stfs f29,24(r31)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r31) + 24, temp.u32);
  // mr r29,r10
  r29 = r10;
  // stw r10,20(r30)
  REX_STORE_U32(uint32_t(r30) + 20, uint32_t(r10));
loc_8217A1B8:
  // mr r3,r29
  r3 = r29;
loc_8217A1BC:
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

// sub_8217A1D0: lifted from generated/ by lift.py.
lifted::Result f8217A1D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8217A1D0 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lfs f0,4(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 4);
  f0 = double(temp.f32);
  // lis r9,-32157
  r9 = uint64_t(int64_t(-2107441152));
  // lwz r11,12(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 12);
  // mulli r10,r5,48
  r10 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(48))));
  // lfs f13,2612(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 2612);
  f13 = double(temp.f32);
  // fmr f12,f13
  f12 = f13;
  // addi r9,r9,24208
  r9 = uint64_t(int64_t(int64_t(r9) + 24208));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r8,r9,40
  r8 = uint64_t(int64_t(int64_t(r9) + 40));
  // addi r7,r9,36
  r7 = uint64_t(int64_t(int64_t(r9) + 36));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfsx f10,r10,r8
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r8));
  f10 = double(temp.f32);
  // lfs f11,10588(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 10588);
  f11 = double(temp.f32);
  // fmuls f9,f0,f11
  f9 = double(float(f0 * f11));
  // lfsx f11,r10,r7
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r7));
  f11 = double(temp.f32);
  // lwz r10,64(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 64);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // fdivs f9,f10,f9
  f9 = double(float(f10 / f9));
  // bne cr6,0x8217a240
  if (!cr6.eq) goto loc_8217A240;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f12,10600(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10600);
  f12 = double(temp.f32);
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fdivs f12,f11,f0
  f12 = double(float(f11 / f0));
loc_8217A240:
  // lfs f0,20(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x8217a260
  if (cr6.lt) goto loc_8217A260;
  // fcmpu cr6,f0,f11
  cr6 = lifted::CompareFloat(f0, f11);
  // bgt cr6,0x8217a264
  if (cr6.gt) goto loc_8217A264;
  // fmr f11,f0
  f11 = f0;
  // b 0x8217a264
  goto loc_8217A264;
loc_8217A260:
  // fmr f11,f13
  lifted::ScalarFloatMode();
  f11 = f13;
loc_8217A264:
  // lfs f0,24(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 24);
  f0 = double(temp.f32);
  // fsubs f0,f0,f9
  f0 = double(float(f0 - f9));
  // stfs f11,20(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x8217a288
  if (cr6.lt) goto loc_8217A288;
  // fcmpu cr6,f0,f10
  cr6 = lifted::CompareFloat(f0, f10);
  // ble cr6,0x8217a28c
  if (!cr6.gt) goto loc_8217A28C;
  // fmr f0,f10
  f0 = f10;
  // b 0x8217a28c
  goto loc_8217A28C;
loc_8217A288:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_8217A28C:
  // stfs f0,24(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x8217a2a0
  if (cr6.gt) goto loc_8217A2A0;
  // stfs f13,24(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
loc_8217A2A0:
  // fcmpu cr6,f11,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f11, f13);
  // bgt cr6,0x8217a2bc
  if (cr6.gt) goto loc_8217A2BC;
  // li r31,1
  r31 = uint64_t(int64_t(1));
  // stfs f13,20(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // li r3,13
  r3 = uint64_t(int64_t(13));
  // stw r31,20(r4)
  REX_STORE_U32(uint32_t(r4) + 20, uint32_t(r31));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8217A2BC:
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

// sub_8217B6B8: lifted from generated/ by lift.py.
lifted::Result f8217B6B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8217B6B8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r30,r3
  r30 = r3;
  // lwz r5,16(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,11844(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 11844);
  // bl 0x820c5c20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C5C20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,3736(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 3736);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8217b710
  if (!cr0.eq) goto loc_8217B710;
  // mr r4,r31
  r4 = r31;
  // lwz r5,48(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mr r3,r30
  r3 = r30;
  // bl 0x82179f60
  // (left out: dead in C++)
  { const lifted::Result o = f82179F60(base, r1, r13, r4, r5, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8217b728
  goto loc_8217B728;
loc_8217B710:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8217b728
  if (!cr6.eq) goto loc_8217B728;
  // mr r4,r31
  r4 = r31;
  // lwz r5,48(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mr r3,r30
  r3 = r30;
  // bl 0x8217a0a8
  // (left out: dead in C++)
  { const lifted::Result o = f8217A0A8(base, r1, r13, r4, r5, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_8217B728:
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

// sub_8217B740: lifted from generated/ by lift.py.
lifted::Result f8217B740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8217B740 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r4
  r31 = r4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r30,r3
  r30 = r3;
  // lwz r5,16(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,11844(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 11844);
  // bl 0x820c5c20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C5C20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,3736(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 3736);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x8217b798
  if (!cr0.eq) goto loc_8217B798;
  // mr r4,r31
  r4 = r31;
  // lwz r5,48(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mr r3,r30
  r3 = r30;
  // bl 0x8217a008
  // (left out: dead in C++)
  { const lifted::Result o = f8217A008(base, r1, r13, r4, r5, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8217b7b0
  goto loc_8217B7B0;
loc_8217B798:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x8217b7b0
  if (!cr6.eq) goto loc_8217B7B0;
  // mr r4,r31
  r4 = r31;
  // lwz r5,48(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mr r3,r30
  r3 = r30;
  // bl 0x8217a1d0
  // (left out: dead in C++)
  { const lifted::Result o = f8217A1D0(base, r1, r13, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
loc_8217B7B0:
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

// sub_8217C4C0: lifted from generated/ by lift.py.
lifted::Result f8217C4C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8217C4C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r31,r3
  r31 = r3;
  // addi r30,r11,-31792
  r30 = uint64_t(int64_t(int64_t(r11) + -31792));
  // mr r28,r4
  r28 = r4;
  // mr r27,r5
  r27 = r5;
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258C9D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // mr r4,r31
  r4 = r31;
  // mr r3,r28
  r3 = r28;
  // bl 0x8217aea8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217AEA8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // stw r28,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r28));
  // bl 0x8258cb08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CB08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lfs f0,80(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f0 = double(temp.f32);
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // lfs f0,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f0 = double(temp.f32);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lfs f0,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f0 = double(temp.f32);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // lwz r10,140(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 140);
  // lbz r9,131(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 131);
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // lbz r8,135(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 135);
  // lbz r7,139(r31)
  r7 = REX_LOAD_U8(uint32_t(r31) + 139);
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r7
  r10 = r10 | r7;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lfs f0,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f0 = double(temp.f32);
  // stfs f0,24(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // lfs f0,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f0 = double(temp.f32);
  // stfs f0,28(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // lfs f0,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f0 = double(temp.f32);
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // lwz r10,156(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 156);
  // lbz r9,147(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 147);
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // lbz r8,151(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 151);
  // lbz r7,155(r31)
  r7 = REX_LOAD_U8(uint32_t(r31) + 155);
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r7
  r10 = r10 | r7;
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // lfs f0,112(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 112);
  f0 = double(temp.f32);
  // stfs f0,48(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // lfs f0,116(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 116);
  f0 = double(temp.f32);
  // stfs f0,52(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // lfs f0,120(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 120);
  f0 = double(temp.f32);
  // stfs f0,56(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 56, temp.u32);
  // lwz r10,172(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 172);
  // lbz r9,163(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 163);
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // lbz r8,171(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 171);
  // or r10,r10,r9
  r10 = r10 | r9;
  // lbz r9,167(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 167);
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // stw r10,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r10));
  // lfs f0,80(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f0 = double(temp.f32);
  // stfs f0,72(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 72, temp.u32);
  // lfs f0,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f0 = double(temp.f32);
  // stfs f0,76(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // lfs f0,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f0 = double(temp.f32);
  // stfs f0,80(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 80, temp.u32);
  // lwz r10,140(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 140);
  // lbz r9,131(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 131);
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // lbz r8,135(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 135);
  // lbz r7,139(r31)
  r7 = REX_LOAD_U8(uint32_t(r31) + 139);
  // or r10,r10,r9
  r10 = r10 | r9;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r8
  r10 = r10 | r8;
  // rlwinm r10,r10,8,0,23
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFFFFFF00;
  // or r10,r10,r7
  r10 = r10 | r7;
  // stw r10,84(r11)
  REX_STORE_U32(uint32_t(r11) + 84, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CA50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // bl 0x8258ca88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CA88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // sth r28,0(r29)
  REX_STORE_U16(uint32_t(r29) + 0, uint16_t(r28));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // sth r28,2(r29)
  REX_STORE_U16(uint32_t(r29) + 2, uint16_t(r28));
  // stw r28,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r28));
  // mr r5,r27
  r5 = r27;
  // stw r28,12(r29)
  REX_STORE_U32(uint32_t(r29) + 12, uint32_t(r28));
  // mr r4,r29
  r4 = r29;
  // lfs f1,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f1 = double(temp.f32);
  // stw r11,8(r29)
  REX_STORE_U32(uint32_t(r29) + 8, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82180C18: lifted from generated/ by lift.py.
void f82180C18(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82180C18 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31756(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31756);
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // blr
  return;
}

// sub_82180C30: lifted from generated/ by lift.py.
void f82180C30(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82180C30 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,-31756(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31756);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // blr
  return;
}

// sub_82180CD0: lifted from generated/ by lift.py.
void f82180CD0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82180CD0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,-31756(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31756);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // blr
  return;
}

// sub_82181BE8: lifted from generated/ by lift.py.
void f82181BE8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82181BE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r9,50
  r9 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // divwu r5,r6,r9
  r5 = uint32_t(uint32_t(r9) ? uint32_t(r6) / uint32_t(r9) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r10,r5,r6
  r10 = r6 - r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // blr
  return;
}

// sub_82181FC0: lifted from generated/ by lift.py.
lifted::Result f82181FC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82181FC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,4
  r11 = uint64_t(int64_t(4));
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
  // bne cr6,0x821820a8
  if (!cr6.eq) goto loc_821820A8;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31764(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,-31764(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31764);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,37
  r4 = uint64_t(int64_t(int64_t(r11) + 37));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,37
  r29 = uint64_t(int64_t(int64_t(r29) + 37));
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
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31764(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31764);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8218207c
  if (!cr6.eq) goto loc_8218207C;
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // b 0x82182088
  goto loc_82182088;
loc_8218207C:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821820a0
  if (!cr6.eq) goto loc_821820A0;
  // li r4,47
  r4 = uint64_t(int64_t(47));
loc_82182088:
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821820A0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821820A8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
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

// sub_82182990: lifted from generated/ by lift.py.
lifted::Result f82182990(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82182990 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r9,0
  r9 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ori r8,r9,40960
  r8 = r9 | 40960;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31756(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31756);
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // lwz r10,52(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 52);
  // lwz r9,48(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,1136(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1136);
  // add r10,r10,r9
  r10 = r10 + r9;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // stwx r7,r31,r8
  REX_STORE_U32(uint32_t(r31) + uint32_t(r8), uint32_t(r7));
  // lwz r10,8432(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8432);
  // lbz r11,769(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 769);
  // addi r29,r10,1646
  r29 = uint64_t(int64_t(int64_t(r10) + 1646));
  // addi r30,r11,1934
  r30 = uint64_t(int64_t(int64_t(r11) + 1934));
  // addi r28,r10,1718
  r28 = uint64_t(int64_t(int64_t(r10) + 1718));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r26,r3
  r26 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // mr r9,r28
  r9 = r28;
  // mr r8,r29
  r8 = r29;
  // mr r7,r30
  r7 = r30;
  // li r6,21
  r6 = uint64_t(int64_t(21));
  // li r5,97
  r5 = uint64_t(int64_t(97));
  // mr r4,r27
  r4 = r27;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82182A80: lifted from generated/ by lift.py.
lifted::Result f82182A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82182A80 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r11,r11,40960
  r11 = r11 | 40960;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stwx r10,r31,r11
  REX_STORE_U32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82183078: lifted from generated/ by lift.py.
uint64_t f82183078(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82183078 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31768(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31768);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x82183090
  if (cr6.eq) goto loc_82183090;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82183090:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82182b30
  { const lifted::Result o = lifted::Call(base, 0x82182B30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82183BD8: lifted from generated/ by lift.py.
lifted::Result f82183BD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82183BD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r25,r3
  r25 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,4
  r11 = uint64_t(int64_t(4));
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
  // bne cr6,0x82183dc4
  if (!cr6.eq) goto loc_82183DC4;
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31764(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,-31764(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31764);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // blt cr6,0x82183c6c
  if (cr6.lt) goto loc_82183C6C;
  // bne cr6,0x82183c78
  if (!cr6.eq) goto loc_82183C78;
  // stw r26,108(r29)
  REX_STORE_U32(uint32_t(r29) + 108, uint32_t(r26));
  // stw r26,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r26));
  // b 0x82183c78
  goto loc_82183C78;
loc_82183C6C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,108(r29)
  REX_STORE_U32(uint32_t(r29) + 108, uint32_t(r11));
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
loc_82183C78:
  // li r4,78
  r4 = uint64_t(int64_t(78));
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
  // li r4,78
  r4 = uint64_t(int64_t(78));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r27,37
  r4 = uint64_t(int64_t(int64_t(r27) + 37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // blt cr6,0x82183cf0
  if (cr6.lt) goto loc_82183CF0;
  // bne cr6,0x82183d28
  if (!cr6.eq) goto loc_82183D28;
  // lwz r11,-31756(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31756);
  // lwz r11,1132(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1132);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82183d28
  if (!cr6.gt) goto loc_82183D28;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,38
  r4 = uint64_t(int64_t(38));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,38
  r4 = uint64_t(int64_t(38));
  // b 0x82183d20
  goto loc_82183D20;
loc_82183CF0:
  // lwz r11,-31756(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31756);
  // lwz r11,616(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 616);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82183d28
  if (!cr6.gt) goto loc_82183D28;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,37
  r4 = uint64_t(int64_t(37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,37
  r4 = uint64_t(int64_t(37));
loc_82183D20:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82183D28:
  // li r4,47
  r4 = uint64_t(int64_t(47));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31764(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31764);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82183d58
  if (!cr6.eq) goto loc_82183D58;
  // li r4,48
  r4 = uint64_t(int64_t(48));
  // b 0x82183d64
  goto loc_82183D64;
loc_82183D58:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82183d7c
  if (!cr6.eq) goto loc_82183D7C;
  // li r4,47
  r4 = uint64_t(int64_t(47));
loc_82183D64:
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82183D7C:
  // lwz r3,-31764(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82183da4
  if (!cr0.eq) goto loc_82183DA4;
  // lwz r11,-31756(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31756);
  // lwz r11,616(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 616);
loc_82183D94:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82183db8
  if (!cr6.eq) goto loc_82183DB8;
  // li r3,583
  r3 = uint64_t(int64_t(583));
  // b 0x82183dbc
  goto loc_82183DBC;
loc_82183DA4:
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x82183db8
  if (!cr6.eq) goto loc_82183DB8;
  // lwz r11,-31756(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31756);
  // lwz r11,1132(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1132);
  // b 0x82183d94
  goto loc_82183D94;
loc_82183DB8:
  // addi r3,r3,550
  r3 = uint64_t(int64_t(int64_t(r3) + 550));
loc_82183DBC:
  // bl 0x82199b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199B38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r26,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r26));
loc_82183DC4:
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
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82183F68: lifted from generated/ by lift.py.
lifted::Result f82183F68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82183F68 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r27,0(r3)
  r27 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82184000
  if (!cr6.eq) goto loc_82184000;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31760(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31760);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31760(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31760);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r30,r3
  r30 = r3;
  // lwz r29,4(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31756(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31756);
  // stw r30,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r30));
  // lwz r11,-31756(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31756);
  // stw r29,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r29));
  // bl 0x82181330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82181330, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // add r30,r29,r30
  r30 = r29 + r30;
  // mr r3,r30
  r3 = r30;
  // bl 0x82182620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82182620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31756(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31756);
  // lwz r10,1136(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 1136);
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,8432(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8432);
  // addi r3,r11,1862
  r3 = uint64_t(int64_t(int64_t(r11) + 1862));
  // bl 0x82199b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199B38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_82184000:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
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

// sub_82184268: lifted from generated/ by lift.py.
lifted::Result f82184268(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82184268 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,4
  r11 = uint64_t(int64_t(4));
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
  // bne cr6,0x8218430c
  if (!cr6.eq) goto loc_8218430C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31764(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,37
  r4 = uint64_t(int64_t(int64_t(r11) + 37));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,86
  r4 = uint64_t(int64_t(86));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x82182c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82182C28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,160
  r4 = uint64_t(int64_t(160));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8218430C:
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

// sub_82185178: lifted from generated/ by lift.py.
lifted::Result f82185178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82185178 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821851f0
  if (!cr6.eq) goto loc_821851F0;
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
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82184650
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82184650, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82181330
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82181330, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x82182c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82182C28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821851F0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
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

// sub_82185220: lifted from generated/ by lift.py.
lifted::Result f82185220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82185220 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821852b4
  if (!cr6.eq) goto loc_821852B4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31764(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,37
  r4 = uint64_t(int64_t(int64_t(r11) + 37));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,86
  r4 = uint64_t(int64_t(86));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,87
  r4 = uint64_t(int64_t(87));
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
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82184488
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82184488, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r29));
loc_821852B4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82185304
  if (!cr0.eq) goto loc_82185304;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821852e0
  if (cr6.eq) goto loc_821852E0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821852E0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_82185304:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82185400: lifted from generated/ by lift.py.
lifted::Result f82185400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82185400 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r11,4
  r11 = uint64_t(int64_t(4));
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
  // bne cr6,0x821854b4
  if (!cr6.eq) goto loc_821854B4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31764(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31764);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r10,r3
  r10 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r10,134
  r4 = uint64_t(int64_t(int64_t(r10) + 134));
  // lwz r11,-31756(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31756);
  // lwz r9,52(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 52);
  // lwz r8,48(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,1136(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 1136);
  // add r9,r9,r8
  r9 = r9 + r8;
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwz r30,8432(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 8432);
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
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82184650
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82184650, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r30,1862
  r3 = uint64_t(int64_t(int64_t(r30) + 1862));
  // bl 0x82199b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199B38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x82182c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82182C28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821854B4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82185508
  if (!cr0.eq) goto loc_82185508;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821854e0
  if (cr6.eq) goto loc_821854E0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821854E0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
loc_82185508:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82185518: lifted from generated/ by lift.py.
lifted::Result f82185518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82185518 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8218558c
  if (!cr6.eq) goto loc_8218558C;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,157
  r4 = uint64_t(int64_t(157));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821847a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821847A0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x82182c28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82182C28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8218558C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821855e0
  if (!cr0.eq) goto loc_821855E0;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821855b8
  if (cr6.eq) goto loc_821855B8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821855B8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // li r7,3
  r7 = uint64_t(int64_t(3));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31768(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31768);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
loc_821855E0:
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

// sub_8218D6D8: lifted from generated/ by lift.py.
void f8218D6D8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6D8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // lwz r11,-31720(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31720);
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // blr
  return;
}

// sub_8218D6F0: lifted from generated/ by lift.py.
void f8218D6F0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6F0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31720(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31720);
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // blr
  return;
}

// sub_8218DE18: lifted from generated/ by lift.py.
uint64_t f8218DE18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8218DE18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31720(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31720);
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8218de78
  if (cr6.eq) goto loc_8218DE78;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // bl 0x8218dd78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8218DD78, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8218de78
  if (cr0.eq) goto loc_8218DE78;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31728(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31728);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x8218de64
  if (cr6.eq) goto loc_8218DE64;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_8218DE64:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
loc_8218DE78:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8218EB78: lifted from generated/ by lift.py.
lifted::Result f8218EB78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8218EB78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
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
  // bne cr6,0x8218ec2c
  if (!cr6.eq) goto loc_8218EC2C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8218EC2C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
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

// sub_8218F150: lifted from generated/ by lift.py.
lifted::Result f8218F150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8218F150 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
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
  // bne cr6,0x8218f1d0
  if (!cr6.eq) goto loc_8218F1D0;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8218F1D0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
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

// sub_8218F220: lifted from generated/ by lift.py.
lifted::Result f8218F220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8218F220 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8218f288
  if (!cr6.eq) goto loc_8218F288;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_8218F288:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
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

// sub_8218F2B8: lifted from generated/ by lift.py.
lifted::Result f8218F2B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8218F2B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,0
  r11 = uint64_t(int64_t(0));
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
  // bne cr6,0x8218f3d0
  if (!cr6.eq) goto loc_8218F3D0;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31724(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31724);
  // lwz r7,0(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // mr r9,r7
  r9 = r7;
  // addi r5,r9,2
  r5 = uint64_t(int64_t(int64_t(r9) + 2));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // lwz r11,-31724(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31724);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ddd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDD8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r10,r3
  r10 = r3;
  // li r6,543
  r6 = uint64_t(int64_t(543));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // lfs f0,-30444(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -30444);
  f0 = double(temp.f32);
  // stfs f0,16(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 16, temp.u32);
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
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8218F3D0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_8218F620: lifted from generated/ by lift.py.
uint64_t f8218F620(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8218F620 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // lwz r11,-31724(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31724);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lfs f0,7820(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7820);
  f0 = double(temp.f32);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // stfs f0,16(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // lwz r11,-31720(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31720);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8218FAA0: lifted from generated/ by lift.py.
uint64_t f8218FAA0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8218FAA0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r10,-32157
  r10 = uint64_t(int64_t(-2107441152));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,25836(r10)
  REX_STORE_U32(uint32_t(r10) + 25836, uint32_t(r11));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31728(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31728);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x8218fad0
  if (cr6.eq) goto loc_8218FAD0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_8218FAD0:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82112838
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82112838, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,7
  r7 = uint64_t(int64_t(7));
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8218f530
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8218F530, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82190168: lifted from generated/ by lift.py.
uint64_t f82190168(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82190168 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8219019c
  if (cr6.lt) goto loc_8219019C;
  // bnelr cr6
  if (!cr6.eq) return r3;
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8218f530
  { const lifted::Result o = lifted::Call(base, 0x8218F530, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
loc_8219019C:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x8218f530
  { const lifted::Result o = lifted::Call(base, 0x8218F530, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_82190690: lifted from generated/ by lift.py.
lifted::Result f82190690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82190690 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
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
  // bne cr6,0x82190714
  if (!cr6.eq) goto loc_82190714;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D038, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821903d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821903D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_82190714:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
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

// sub_82190760: lifted from generated/ by lift.py.
lifted::Result f82190760(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82190760 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821907cc
  if (!cr6.eq) goto loc_821907CC;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821903d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821903D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821907CC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
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

// sub_82192958: lifted from generated/ by lift.py.
lifted::Result f82192958(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82192958 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // lwz r30,-31700(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // lwz r11,68(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 68);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82192ae4
  if (cr6.lt) goto loc_82192AE4;
  // beq cr6,0x82192a54
  if (cr6.eq) goto loc_82192A54;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82192b9c
  if (!cr6.lt) goto loc_82192B9C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,36(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 36);
  // mr r10,r3
  r10 = r3;
  // lwz r9,48(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 48);
  // lwz r8,44(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + 44);
  // mr r3,r11
  r3 = r11;
  // add r4,r9,r8
  r4 = r9 + r8;
  // lwz r30,0(r10)
  r30 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lha r29,6328(r11)
  r29 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 6328))));
  // bl 0x82116d88
  // (left out: dead in C++)
  r3 = f82116D88(base, r1, r13, r3, r4);
  // lwz r10,-31700(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // lwz r11,36(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 36);
  // lwz r4,100(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 100);
  // lis r10,152
  r10 = uint64_t(int64_t(9961472));
  // ori r9,r10,38527
  r9 = r10 | 38527;
  // lwz r10,6116(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 6116);
  // add r10,r10,r4
  r10 = r10 + r4;
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // ble cr6,0x821929f4
  if (!cr6.gt) goto loc_821929F4;
  // mr r10,r9
  r10 = r9;
loc_821929F4:
  // addi r3,r11,48
  r3 = uint64_t(int64_t(int64_t(r11) + 48));
  // stw r10,6116(r11)
  REX_STORE_U32(uint32_t(r11) + 6116, uint32_t(r10));
  // bl 0x82110b18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82110B18, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r10,6116(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 6116);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // lwz r6,24(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 24);
  // bl 0x8211c740
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C740, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // addi r8,r29,-1
  r8 = uint64_t(int64_t(int64_t(r29) + -1));
  // lwz r10,48(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r9,44(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 44);
  // add r10,r10,r9
  r10 = r10 + r9;
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // bne cr6,0x82192b9c
  if (!cr6.eq) goto loc_82192B9C;
  // rotlwi r10,r9,0
  r10 = __builtin_rotateleft32(uint32_t(r9), 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,44(r11)
  REX_STORE_U32(uint32_t(r11) + 44, uint32_t(r10));
  // b 0x82192b9c
  goto loc_82192B9C;
loc_82192A54:
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lwz r10,44(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 44);
  // lwz r9,36(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + 36);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // ori r8,r11,39200
  r8 = r11 | 39200;
  // lwz r11,48(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 48);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stwx r11,r9,r8
  REX_STORE_U32(uint32_t(r9) + uint32_t(r8), uint32_t(r11));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x821159b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821159B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // addi r10,r3,8
  r10 = uint64_t(int64_t(int64_t(r3) + 8));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // mtctr r11
  ctr = r11;
loc_82192A9C:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82192a9c
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82192A9C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r6,r11,4
  r6 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x8211d930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D930, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x82192b9c
  goto loc_82192B9C;
loc_82192AE4:
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,32
  r9 = uint64_t(int64_t(32));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
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
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // lwz r11,-31700(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31700);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r9,48(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 48);
  // addi r10,r10,6120
  r10 = uint64_t(int64_t(int64_t(r10) + 6120));
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // add. r11,r9,r11
  r11 = r9 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lha r9,204(r10)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 204))));
  // b 0x82192b58
  goto loc_82192B58;
loc_82192B50:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_82192B58:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r10
  r9 = r9 + r10;
  // bne 0x82192b50
  if (!cr0.eq) goto loc_82192B50;
  // lha r11,2(r9)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // mulli r11,r11,1968
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1968))));
  // lwz r9,-31688(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + -31688);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r8,r9,4
  r8 = uint64_t(int64_t(int64_t(r9) + 4));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // addi r11,r11,1780
  r11 = uint64_t(int64_t(int64_t(r11) + 1780));
  // mtctr r9
  ctr = r9;
loc_82192B88:
  // lwz r10,0(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + 0);
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x82192b88
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82192B88;
loc_82192B9C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82195F10: lifted from generated/ by lift.py.
lifted::Result f82195F10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82195F10 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82195fd8
  if (!cr6.eq) goto loc_82195FD8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,142
  r4 = uint64_t(int64_t(142));
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
  // li r4,138
  r4 = uint64_t(int64_t(138));
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
  // li r4,139
  r4 = uint64_t(int64_t(139));
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
  // li r4,140
  r4 = uint64_t(int64_t(140));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r11,-31700(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31700);
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // bl 0x82191b20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82191B20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821958d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821958D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r29));
loc_82195FD8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82196024
  if (!cr0.eq) goto loc_82196024;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31712(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31712);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82196004
  if (cr6.eq) goto loc_82196004;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82196004:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r28,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r28));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31712(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31712);
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
loc_82196024:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82196030: lifted from generated/ by lift.py.
lifted::Result f82196030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82196030 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82196114
  if (!cr6.eq) goto loc_82196114;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31708(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31708);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 2));
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
  // li r4,145
  r4 = uint64_t(int64_t(145));
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
  // li r4,138
  r4 = uint64_t(int64_t(138));
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
  // li r4,139
  r4 = uint64_t(int64_t(139));
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
  // li r4,140
  r4 = uint64_t(int64_t(140));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r11,-31700(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31700);
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // bl 0x82191b20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82191B20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821958d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821958D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r29));
loc_82196114:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x82196160
  if (!cr0.eq) goto loc_82196160;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31712(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31712);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x82196140
  if (cr6.eq) goto loc_82196140;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82196140:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r28,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r28));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31712(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31712);
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
loc_82196160:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821970B8: lifted from generated/ by lift.py.
uint64_t f821970B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821970B8 (lifted)");
  double f1 = 0.0;
  // b 0x8218de18
  r3 = f8218DE18(base, r1, r13, r3);
  return r3;
}

// sub_82198830: lifted from generated/ by lift.py.
lifted::Result f82198830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82198830 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // mr r29,r3
  r29 = r3;
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220c1e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220C1E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220bd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BD50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r31,-1
  r31 = uint64_t(int64_t(-1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // beq 0x821988f8
  if (cr0.eq) goto loc_821988F8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31688);
  // stw r3,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r3));
  // lwz r11,-31696(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x82198894
  if (cr6.eq) goto loc_82198894;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82198894:
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // stw r31,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r31));
  // addi r10,r29,204
  r10 = uint64_t(int64_t(int64_t(r29) + 204));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r9,29
  r9 = uint64_t(int64_t(29));
  // lwz r11,600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 600);
  // lwz r6,604(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 604);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r6,r6,r11
  r6 = r6 + r11;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r11,600(r10)
  REX_STORE_U32(uint32_t(r10) + 600, uint32_t(r11));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r11,r5,r6
  r11 = r6 - r5;
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x8219893c
  goto loc_8219893C;
loc_821988F8:
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220bde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8219893c
  if (cr0.eq) goto loc_8219893C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31696(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x82198924
  if (cr6.eq) goto loc_82198924;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82198924:
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // stw r31,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r31));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_8219893C:
  // lwz r11,-31692(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82198980
  if (cr6.eq) goto loc_82198980;
  // lwz r11,-31696(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x82198960
  if (cr6.eq) goto loc_82198960;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_82198960:
  // li r10,17
  r10 = uint64_t(int64_t(17));
  // stw r31,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r31));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82198980:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82198B58: lifted from generated/ by lift.py.
lifted::Result f82198B58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82198B58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
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
  // bne cr6,0x82198c04
  if (!cr6.eq) goto loc_82198C04;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mulli r11,r11,3
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(3))));
  // lwz r10,-31688(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31688);
  // lwz r10,52(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 52);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // add r11,r10,r11
  r11 = r10 + r11;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r30,r11,2
  r30 = uint64_t(int64_t(int64_t(r11) + 2));
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
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
loc_82198C04:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
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

// sub_82198E70: lifted from generated/ by lift.py.
lifted::Result f82198E70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82198E70 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
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
  // bne cr6,0x82198eec
  if (!cr6.eq) goto loc_82198EEC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,62
  r31 = uint64_t(int64_t(int64_t(r11) + 62));
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_82198EEC:
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
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82198F08: lifted from generated/ by lift.py.
lifted::Result f82198F08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82198F08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
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
  // bne cr6,0x82198f9c
  if (!cr6.eq) goto loc_82198F9C;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r3,-31692(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31692);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,62
  r4 = uint64_t(int64_t(int64_t(r11) + 62));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r28,62
  r30 = uint64_t(int64_t(int64_t(r28) + 62));
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
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_82198F9C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
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

// sub_82199220: lifted from generated/ by lift.py.
lifted::Result f82199220(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82199220 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mulli r27,r31,3
  r27 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(3))));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r26,r27
  r26 = r27;
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // mr r31,r24
  r31 = r24;
loc_82199260:
  // addi r28,r31,33
  r28 = uint64_t(int64_t(int64_t(r31) + 33));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r29
  r3 = r29;
  // addi r5,r28,-21
  r5 = uint64_t(int64_t(int64_t(r28) + -21));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r5,r28
  r5 = r28;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmpwi cr6,r26,14
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 14, so);
  // bge cr6,0x821992b4
  if (!cr6.lt) goto loc_821992B4;
  // add r10,r27,r31
  r10 = r27 + r31;
  // stw r25,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r25));
  // mr r4,r30
  r4 = r30;
  // stw r25,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r25));
  // addi r5,r10,5411
  r5 = uint64_t(int64_t(int64_t(r10) + 5411));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821992bc
  goto loc_821992BC;
loc_821992B4:
  // stw r24,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r24));
  // stw r24,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r24));
loc_821992BC:
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // cmpwi cr6,r31,3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 3, so);
  // blt cr6,0x82199260
  if (cr6.lt) goto loc_82199260;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821995D8: lifted from generated/ by lift.py.
lifted::Result f821995D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821995D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r28,103
  r28 = uint64_t(int64_t(103));
  // mulli r31,r31,10
  r31 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(10))));
  // li r27,10
  r27 = uint64_t(int64_t(10));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // li r25,0
  r25 = uint64_t(int64_t(0));
loc_82199618:
  // addi r5,r28,-70
  r5 = uint64_t(int64_t(int64_t(r28) + -70));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r5,r28
  r5 = r28;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmpwi cr6,r31,38
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 38, so);
  // bge cr6,0x82199664
  if (!cr6.lt) goto loc_82199664;
  // addi r5,r31,5425
  r5 = uint64_t(int64_t(int64_t(r31) + 5425));
  // stw r26,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r26));
  // mr r4,r30
  r4 = r30;
  // stw r26,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r26));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x8219966c
  goto loc_8219966C;
loc_82199664:
  // stw r25,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r25));
  // stw r25,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r25));
loc_8219966C:
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // bne 0x82199618
  if (!cr0.eq) goto loc_82199618;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821997E8: lifted from generated/ by lift.py.
lifted::Result f821997E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821997E8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // mr r31,r28
  r31 = r28;
  // lwz r11,104(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 104);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x82199860
  if (!cr6.gt) goto loc_82199860;
loc_82199838:
  // lwz r11,100(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 100);
  // addi r6,r31,270
  r6 = uint64_t(int64_t(int64_t(r31) + 270));
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // add r5,r31,r11
  r5 = r31 + r11;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 104);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x82199838
  if (cr6.lt) goto loc_82199838;
loc_82199860:
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r28));
  // stw r28,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r28));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82199E48: lifted from generated/ by lift.py.
lifted::Result f82199E48(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_82199E48 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // li r11,158
  r11 = uint64_t(int64_t(158));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BAC0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // lwz r11,-31688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31688);
  // lwz r4,20(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x8220bc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // bl 0x82199aa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199AA8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821997e8
  // (left out: dead in C++)
  { const lifted::Result o = f821997E8(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,572
  r3 = uint64_t(int64_t(572));
  // bl 0x82199b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199B38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_8219A450: lifted from generated/ by lift.py.
lifted::Result f8219A450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219A450 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r23,r3
  r23 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r23)
  r11 = REX_LOAD_U32(uint32_t(r23) + 0);
  // lwz r24,0(r3)
  r24 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219a5d0
  if (!cr6.eq) goto loc_8219A5D0;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mulli r7,r3,3
  r7 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(3))));
  // lwz r6,-31688(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + -31688);
  // lwz r11,-31692(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r8,28(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mulli r9,r10,3
  r9 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(3))));
  // lwz r11,52(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 52);
  // add r28,r7,r11
  r28 = r7 + r11;
  // subf r26,r10,r31
  r26 = r31 - r10;
  // add. r11,r8,r11
  r11 = r8 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // add r27,r9,r28
  r27 = r9 + r28;
  // blt 0x8219a4e8
  if (cr0.lt) goto loc_8219A4E8;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // ble cr6,0x8219a4ec
  if (!cr6.gt) goto loc_8219A4EC;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // b 0x8219a4ec
  goto loc_8219A4EC;
loc_8219A4E8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8219A4EC:
  // mulli r10,r30,3
  r10 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(3))));
  // add r29,r10,r11
  r29 = r10 + r11;
  // mulli r9,r31,3
  r9 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(3))));
  // add r30,r9,r29
  r30 = r9 + r29;
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // cmpwi cr6,r30,14
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 14, so);
  // bge cr6,0x8219a510
  if (!cr6.lt) goto loc_8219A510;
  // stw r11,52(r6)
  REX_STORE_U32(uint32_t(r6) + 52, uint32_t(r11));
  // b 0x8219a51c
  goto loc_8219A51C;
loc_8219A510:
  // li r30,13
  r30 = uint64_t(int64_t(13));
  // stw r25,52(r6)
  REX_STORE_U32(uint32_t(r6) + 52, uint32_t(r25));
  // addi r29,r10,1
  r29 = uint64_t(int64_t(int64_t(r10) + 1));
loc_8219A51C:
  // mr r3,r30
  r3 = r30;
  // bl 0x82199898
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199898, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r27,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r30), so);
  // beq cr6,0x8219a56c
  if (cr6.eq) goto loc_8219A56C;
  // addi r4,r28,2
  r4 = uint64_t(int64_t(int64_t(r28) + 2));
  // mr r3,r24
  r3 = r24;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r29,2
  r30 = uint64_t(int64_t(int64_t(r29) + 2));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r24
  r3 = r24;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r24
  r3 = r24;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219A56C:
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // beq cr6,0x8219a5cc
  if (cr6.eq) goto loc_8219A5CC;
  // bl 0x821990d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821990D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // mr r3,r24
  r3 = r24;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r24
  r3 = r24;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bge cr6,0x8219a5a8
  if (!cr6.lt) goto loc_8219A5A8;
  // addi r3,r31,3
  r3 = uint64_t(int64_t(int64_t(r31) + 3));
  // bl 0x82199220
  // (left out: dead in C++)
  { const lifted::Result o = f82199220(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // b 0x8219a5b4
  goto loc_8219A5B4;
loc_8219A5A8:
  // addi r3,r31,-1
  r3 = uint64_t(int64_t(int64_t(r31) + -1));
  // bl 0x82199220
  // (left out: dead in C++)
  { const lifted::Result o = f82199220(base, r1, r13, r3, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
loc_8219A5B4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r24
  r3 = r24;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219A5CC:
  // stw r25,0(r23)
  REX_STORE_U32(uint32_t(r23) + 0, uint32_t(r25));
loc_8219A5D0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r24
  r3 = r24;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219A6B8: lifted from generated/ by lift.py.
lifted::Result f8219A6B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219A6B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r27,0(r3)
  r27 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219a808
  if (!cr6.eq) goto loc_8219A808;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r9,-31692(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // mr r4,r3
  r4 = r3;
  // rlwinm r10,r4,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 2) & 0xFFFFFFFC;
  // lwz r8,-31688(r30)
  r8 = REX_LOAD_U32(uint32_t(r30) + -31688);
  // lwz r11,28(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 28);
  // lwz r31,52(r8)
  r31 = REX_LOAD_U32(uint32_t(r8) + 52);
  // add. r11,r11,r31
  r11 = r11 + r31;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // add r28,r10,r31
  r28 = r10 + r31;
  // blt 0x8219a740
  if (cr0.lt) goto loc_8219A740;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // ble cr6,0x8219a744
  if (!cr6.gt) goto loc_8219A744;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x8219a744
  goto loc_8219A744;
loc_8219A740:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8219A744:
  // rlwinm r10,r29,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // add r29,r10,r11
  r29 = r10 + r11;
  // cmpwi cr6,r29,10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 10, so);
  // bge cr6,0x8219a75c
  if (!cr6.lt) goto loc_8219A75C;
  // stw r11,52(r8)
  REX_STORE_U32(uint32_t(r8) + 52, uint32_t(r11));
  // b 0x8219a770
  goto loc_8219A770;
loc_8219A75C:
  // mr r3,r9
  r3 = r9;
  // bl 0x8220bc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31688(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31688);
  // mr r29,r28
  r29 = r28;
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
loc_8219A770:
  // lwz r9,-31688(r30)
  r9 = REX_LOAD_U32(uint32_t(r30) + -31688);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r8,56(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 56);
loc_8219A780:
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bne cr6,0x8219a790
  if (!cr6.eq) goto loc_8219A790;
  // stw r29,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r29));
  // b 0x8219a7a0
  goto loc_8219A7A0;
loc_8219A790:
  // add r7,r9,r11
  r7 = r9 + r11;
  // lbz r7,6(r7)
  r7 = REX_LOAD_U8(uint32_t(r7) + 6);
  // extsb r7,r7
  r7 = uint64_t(int64_t(int8_t(r7)));
  // stw r7,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r7));
loc_8219A7A0:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // blt cr6,0x8219a780
  if (cr6.lt) goto loc_8219A780;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x821998f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821998F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r29,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r28), so);
  // beq cr6,0x8219a800
  if (cr6.eq) goto loc_8219A800;
  // addi r4,r28,15
  r4 = uint64_t(int64_t(int64_t(r28) + 15));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r29,15
  r31 = uint64_t(int64_t(int64_t(r29) + 15));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219A800:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_8219A808:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219A8E0: lifted from generated/ by lift.py.
lifted::Result f8219A8E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219A8E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219a9a8
  if (!cr6.eq) goto loc_8219A9A8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,-31688(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31688);
  // lwz r10,52(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 52);
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,15
  r4 = uint64_t(int64_t(int64_t(r11) + 15));
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
  // li r4,70
  r4 = uint64_t(int64_t(70));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31688(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31688);
  // mr r10,r29
  r10 = r29;
  // addi r9,r1,88
  r9 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 6));
loc_8219A978:
  // lbzx r8,r11,r10
  r8 = REX_LOAD_U8(uint32_t(r11) + uint32_t(r10));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // extsb r8,r8
  r8 = uint64_t(int64_t(int8_t(r8)));
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // blt cr6,0x8219a978
  if (cr6.lt) goto loc_8219A978;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x821998f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821998F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82199c10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199C10, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_8219A9A8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8219a9f4
  if (!cr0.eq) goto loc_8219A9F4;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8219a9d4
  if (cr0.eq) goto loc_8219A9D4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_8219A9D4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // stw r29,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r29));
loc_8219A9F4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219AAB0: lifted from generated/ by lift.py.
lifted::Result f8219AAB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219AAB0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r24,r3
  r24 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 0);
  // lwz r25,0(r3)
  r25 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219ac6c
  if (!cr6.eq) goto loc_8219AC6C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,-31692(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31692(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31692);
  // lis r5,-32146
  r5 = uint64_t(int64_t(-2106720256));
  // mulli r8,r3,10
  r8 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(10))));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r6,-31688(r5)
  r6 = REX_LOAD_U32(uint32_t(r5) + -31688);
  // lwz r7,28(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r9,r10,r3
  r9 = r10 + r3;
  // subf r26,r10,r31
  r26 = r31 - r10;
  // lwz r11,52(r6)
  r11 = REX_LOAD_U32(uint32_t(r6) + 52);
  // mulli r9,r9,10
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(10))));
  // add. r10,r7,r11
  r10 = r7 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // add r28,r8,r11
  r28 = r8 + r11;
  // add r27,r9,r11
  r27 = r9 + r11;
  // blt 0x8219ab4c
  if (cr0.lt) goto loc_8219AB4C;
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // ble cr6,0x8219ab50
  if (!cr6.gt) goto loc_8219AB50;
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x8219ab50
  goto loc_8219AB50;
loc_8219AB4C:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8219AB50:
  // add r9,r31,r30
  r9 = r31 + r30;
  // mulli r11,r30,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(10))));
  // mulli r9,r9,10
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(10))));
  // add r30,r9,r10
  r30 = r9 + r10;
  // add r29,r11,r10
  r29 = r11 + r10;
  // cmpwi cr6,r30,38
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 38, so);
  // blt cr6,0x8219ab78
  if (cr6.lt) goto loc_8219AB78;
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // li r30,37
  r30 = uint64_t(int64_t(37));
  // addi r29,r11,7
  r29 = uint64_t(int64_t(int64_t(r11) + 7));
loc_8219AB78:
  // stw r10,52(r6)
  REX_STORE_U32(uint32_t(r6) + 52, uint32_t(r10));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r9,-31688(r5)
  r9 = REX_LOAD_U32(uint32_t(r5) + -31688);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r8,56(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 56);
loc_8219AB8C:
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bne cr6,0x8219ab9c
  if (!cr6.eq) goto loc_8219AB9C;
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // b 0x8219abac
  goto loc_8219ABAC;
loc_8219AB9C:
  // add r7,r9,r11
  r7 = r9 + r11;
  // lbz r7,9(r7)
  r7 = REX_LOAD_U8(uint32_t(r7) + 9);
  // extsb r7,r7
  r7 = uint64_t(int64_t(int8_t(r7)));
  // stw r7,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r7));
loc_8219ABAC:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // blt cr6,0x8219ab8c
  if (cr6.lt) goto loc_8219AB8C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82199990
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199990, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r30,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r27), so);
  // beq cr6,0x8219ac04
  if (cr6.eq) goto loc_8219AC04;
  // addi r4,r28,27
  r4 = uint64_t(int64_t(int64_t(r28) + 27));
  // mr r3,r25
  r3 = r25;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r29,27
  r30 = uint64_t(int64_t(int64_t(r29) + 27));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r25
  r3 = r25;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r25
  r3 = r25;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219AC04:
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // beq cr6,0x8219ac64
  if (cr6.eq) goto loc_8219AC64;
  // bl 0x82199488
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199488, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,58
  r4 = uint64_t(int64_t(58));
  // mr r3,r25
  r3 = r25;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,59
  r4 = uint64_t(int64_t(59));
  // mr r3,r25
  r3 = r25;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bge cr6,0x8219ac40
  if (!cr6.lt) goto loc_8219AC40;
  // addi r3,r31,3
  r3 = uint64_t(int64_t(int64_t(r31) + 3));
  // bl 0x821995d8
  // (left out: dead in C++)
  { const lifted::Result o = f821995D8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,59
  r4 = uint64_t(int64_t(59));
  // b 0x8219ac4c
  goto loc_8219AC4C;
loc_8219AC40:
  // addi r3,r31,-1
  r3 = uint64_t(int64_t(int64_t(r31) + -1));
  // bl 0x821995d8
  // (left out: dead in C++)
  { const lifted::Result o = f821995D8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r4,58
  r4 = uint64_t(int64_t(58));
loc_8219AC4C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r25
  r3 = r25;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219AC64:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r24)
  REX_STORE_U32(uint32_t(r24) + 0, uint32_t(r11));
loc_8219AC6C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r25
  r3 = r25;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219AD68: lifted from generated/ by lift.py.
lifted::Result f8219AD68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219AD68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219ae30
  if (!cr6.eq) goto loc_8219AE30;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r11,-31688(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31688);
  // addi r7,r11,9
  r7 = uint64_t(int64_t(int64_t(r11) + 9));
  // lwz r9,52(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mulli r11,r3,10
  r11 = uint64_t(int64_t(static_cast<int64_t>(r3 * static_cast<uint64_t>(10))));
  // add r30,r9,r11
  r30 = r9 + r11;
loc_8219ADD0:
  // lbzx r11,r7,r10
  r11 = REX_LOAD_U8(uint32_t(r7) + uint32_t(r10));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // blt cr6,0x8219add0
  if (cr6.lt) goto loc_8219ADD0;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82199990
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199990, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r30,27
  r4 = uint64_t(int64_t(int64_t(r30) + 27));
  // mr r3,r29
  r3 = r29;
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
  // li r4,72
  r4 = uint64_t(int64_t(72));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31688(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31688);
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r10));
  // bl 0x82199cc8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199CC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r27,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r27));
loc_8219AE30:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8219ae80
  if (!cr0.eq) goto loc_8219AE80;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8219ae5c
  if (cr6.eq) goto loc_8219AE5C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_8219AE5C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
loc_8219AE80:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219AE90: lifted from generated/ by lift.py.
lifted::Result f8219AE90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219AE90 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,26
  r11 = uint64_t(int64_t(26));
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
  // bne cr6,0x8219af28
  if (!cr6.eq) goto loc_8219AF28;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,-31688(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31688);
  // lwz r10,52(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 52);
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,27
  r4 = uint64_t(int64_t(int64_t(r11) + 27));
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
  // li r4,73
  r4 = uint64_t(int64_t(73));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x82199e48
  // (left out: dead in C++)
  { const lifted::Result o = f82199E48(base, r1, r13); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8219AF28:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_8219AF50: lifted from generated/ by lift.py.
lifted::Result f8219AF50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219AF50 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,26
  r11 = uint64_t(int64_t(26));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219afe4
  if (!cr6.eq) goto loc_8219AFE4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31692(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31692);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,62
  r4 = uint64_t(int64_t(int64_t(r11) + 62));
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
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31688(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31688);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // stw r10,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r10));
  // bl 0x82199d88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199D88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,571
  r3 = uint64_t(int64_t(571));
  // bl 0x82199b38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199B38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8219AFE4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8219b044
  if (!cr0.eq) goto loc_8219B044;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x8219b010
  if (cr6.eq) goto loc_8219B010;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_8219B010:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31696(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31696);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // lwz r11,-31688(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31688);
  // lwz r3,60(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 60);
  // bl 0x82199688
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82199688, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219B044:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219EE30: lifted from generated/ by lift.py.
lifted::Result f8219EE30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12) {
  ITC_NATIVE_RUNNING("sub_8219EE30 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,86
  r4 = uint64_t(int64_t(86));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
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
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,87
  r4 = uint64_t(int64_t(87));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_8219F3A0: lifted from generated/ by lift.py.
lifted::Result f8219F3A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219F3A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219f504
  if (!cr6.eq) goto loc_8219F504;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r3,-31680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31680(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // mr r10,r3
  r10 = r3;
  // mr r3,r29
  r3 = r29;
  // addi r4,r10,61
  r4 = uint64_t(int64_t(int64_t(r10) + 61));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r28,r11,r31
  r28 = r31 - r11;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r27,61
  r30 = uint64_t(int64_t(int64_t(r27) + 61));
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
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x8219f4bc
  if (cr6.eq) goto loc_8219F4BC;
  // bl 0x8219d3e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D3E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,67
  r4 = uint64_t(int64_t(67));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // li r3,6
  r3 = uint64_t(int64_t(6));
  // bge cr6,0x8219f480
  if (!cr6.lt) goto loc_8219F480;
  // addi r4,r31,6
  r4 = uint64_t(int64_t(int64_t(r31) + 6));
  // bl 0x8219d248
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D248, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,68
  r4 = uint64_t(int64_t(68));
  // b 0x8219f48c
  goto loc_8219F48C;
loc_8219F480:
  // addi r4,r31,-1
  r4 = uint64_t(int64_t(int64_t(r31) + -1));
  // bl 0x8219d248
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D248, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,67
  r4 = uint64_t(int64_t(67));
loc_8219F48C:
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // mr r3,r31
  r3 = r31;
  // ori r10,r11,8176
  r10 = r11 | 8176;
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // lhax r4,r11,r10
  r4 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + uint32_t(r10)))));
  // bl 0x8219d4b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D4B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219F4BC:
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // add. r10,r31,r27
  r10 = r31 + r27;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // addis r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r11,r11,6568
  r11 = uint64_t(int64_t(int64_t(r11) + 6568));
  // lha r9,1604(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 1604))));
  // b 0x8219f4dc
  goto loc_8219F4DC;
loc_8219F4D4:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_8219F4DC:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x8219f4d4
  if (!cr0.eq) goto loc_8219F4D4;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r10,r10,403
  r10 = uint64_t(int64_t(int64_t(r10) + 403));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8219d440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_8219F504:
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
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8219F520: lifted from generated/ by lift.py.
lifted::Result f8219F520(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219F520 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // li r11,6
  r11 = uint64_t(int64_t(6));
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
  // bne cr6,0x8219f678
  if (!cr6.eq) goto loc_8219F678;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r29
  r3 = r29;
  // lwz r11,-31680(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r28,r11,r30
  r28 = r30 - r11;
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
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // beq cr6,0x8219f610
  if (cr6.eq) goto loc_8219F610;
  // lwz r11,-31680(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r4,4(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 4);
  // bl 0x8219d550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bge cr6,0x8219f5ec
  if (!cr6.lt) goto loc_8219F5EC;
  // addi r4,r30,1
  r4 = uint64_t(int64_t(int64_t(r30) + 1));
  // bl 0x8219d550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,95
  r4 = uint64_t(int64_t(95));
  // b 0x8219f5f8
  goto loc_8219F5F8;
loc_8219F5EC:
  // addi r4,r30,-1
  r4 = uint64_t(int64_t(int64_t(r30) + -1));
  // bl 0x8219d550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,94
  r4 = uint64_t(int64_t(94));
loc_8219F5F8:
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219F610:
  // lwz r3,-31680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // add. r10,r3,r30
  r10 = r3 + r30;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,-31672(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31672);
  // addis r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r11,r11,6568
  r11 = uint64_t(int64_t(int64_t(r11) + 6568));
  // lha r9,1604(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 1604))));
  // b 0x8219f63c
  goto loc_8219F63C;
loc_8219F634:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_8219F63C:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x8219f634
  if (!cr0.eq) goto loc_8219F634;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r10,r10,403
  r10 = uint64_t(int64_t(int64_t(r10) + 403));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r31,r10,r11
  r31 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8219d440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // rlwinm r4,r11,0,30,30
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // bl 0x8219d6f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D6F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_8219F678:
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

// sub_8219F690: lifted from generated/ by lift.py.
lifted::Result f8219F690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219F690 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,6
  r11 = uint64_t(int64_t(6));
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
  // bne cr6,0x8219f754
  if (!cr6.eq) goto loc_8219F754;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31680(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r6,r3
  r6 = r3;
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c370
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C370, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // lis r8,3
  r8 = uint64_t(int64_t(196608));
  // ori r9,r11,9804
  r9 = r11 | 9804;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // ori r8,r8,9800
  r8 = r8 | 9800;
  // lwz r11,-31672(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31672);
  // addis r10,r11,3
  r10 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r10,r10,6568
  r10 = uint64_t(int64_t(int64_t(r10) + 6568));
  // lwzx r9,r11,r9
  r9 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // lwzx r11,r11,r8
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // add. r11,r9,r11
  r11 = r9 + r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lha r9,1604(r10)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 1604))));
  // b 0x8219f72c
  goto loc_8219F72C;
loc_8219F724:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
loc_8219F72C:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r10
  r9 = r9 + r10;
  // bne 0x8219f724
  if (!cr0.eq) goto loc_8219F724;
  // lha r11,2(r9)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r11,r11,403
  r11 = uint64_t(int64_t(int64_t(r11) + 403));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8219d820
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D820, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8219F754:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_8219FB08: lifted from generated/ by lift.py.
lifted::Result f8219FB08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8219FB08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8219fc48
  if (!cr6.eq) goto loc_8219FC48;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // lwz r3,-31680(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // bl 0x8220bc28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31680(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31680);
  // mr r10,r3
  r10 = r3;
  // mr r3,r30
  r3 = r30;
  // addi r4,r10,29
  r4 = uint64_t(int64_t(int64_t(r10) + 29));
  // lwz r31,4(r11)
  r31 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r27,r11,r31
  r27 = r31 - r11;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r28,r29,29
  r28 = uint64_t(int64_t(int64_t(r29) + 29));
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
  // mr r4,r28
  r4 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // add r11,r31,r29
  r11 = r31 + r29;
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-14799
  r11 = uint64_t(int64_t(int64_t(r11) + -14799));
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,-31672(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31672);
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8219cf08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219CF08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // beq cr6,0x8219fc40
  if (cr6.eq) goto loc_8219FC40;
  // bl 0x8219fa28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219FA28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,40
  r4 = uint64_t(int64_t(40));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // li r3,11
  r3 = uint64_t(int64_t(11));
  // bge cr6,0x8219fc04
  if (!cr6.lt) goto loc_8219FC04;
  // addi r4,r31,11
  r4 = uint64_t(int64_t(int64_t(r31) + 11));
  // bl 0x8219ce08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219CE08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // b 0x8219fc10
  goto loc_8219FC10;
loc_8219FC04:
  // addi r4,r31,-1
  r4 = uint64_t(int64_t(int64_t(r31) + -1));
  // bl 0x8219ce08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219CE08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,40
  r4 = uint64_t(int64_t(40));
loc_8219FC10:
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
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // mr r3,r31
  r3 = r31;
  // ori r10,r11,6564
  r10 = r11 | 6564;
  // lwz r11,-31672(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31672);
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x8219d1b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D1B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_8219FC40:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_8219FC48:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A0B38: lifted from generated/ by lift.py.
lifted::Result f821A0B38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_821A0B38 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r27,0(r3)
  r27 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a0d1c
  if (!cr6.eq) goto loc_821A0D1C;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31676(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31676(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // mr r10,r3
  r10 = r3;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r27
  r3 = r27;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r11,r11,r10
  r11 = r11 + r10;
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-14799
  r11 = uint64_t(int64_t(int64_t(r11) + -14799));
  // rlwinm r28,r11,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // lwzx r11,r28,r11
  r11 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
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
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x821a0128
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A0128, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r9,r11,9792
  r9 = r11 | 9792;
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r8,r11,9796
  r8 = r11 | 9796;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r10,r11,r9
  REX_STORE_U32(uint32_t(r11) + uint32_t(r9), uint32_t(r10));
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r10,r11,r8
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), uint32_t(r10));
  // bl 0x8219c078
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219C078, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31680(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // lwz r4,4(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 4);
  // bl 0x8219d550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // lwzx r3,r28,r11
  r3 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // bl 0x8219cf08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219CF08, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31676(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r10,r11,9784
  r10 = r11 | 9784;
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r9,r11,9788
  r9 = r11 | 9788;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r3,r11,r10
  REX_STORE_U32(uint32_t(r11) + uint32_t(r10), uint32_t(r3));
  // lwz r11,-31676(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r10,r11,r9
  REX_STORE_U32(uint32_t(r11) + uint32_t(r9), uint32_t(r10));
  // lwz r3,-31680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r30,r11,6568
  r30 = r11 | 6568;
  // lwz r11,-31680(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // add. r10,r10,r3
  r10 = r10 + r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lha r9,1604(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 1604))));
  // b 0x821a0c90
  goto loc_821A0C90;
loc_821A0C88:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821A0C90:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821a0c88
  if (!cr0.eq) goto loc_821A0C88;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r10,r10,403
  r10 = uint64_t(int64_t(int64_t(r10) + 403));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8219d440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8220BC20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31680(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // add. r10,r10,r3
  r10 = r10 + r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lha r9,1604(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 1604))));
  // b 0x821a0cdc
  goto loc_821A0CDC;
loc_821A0CD4:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821A0CDC:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821a0cd4
  if (!cr0.eq) goto loc_821A0CD4;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r10,r10,403
  r10 = uint64_t(int64_t(int64_t(r10) + 403));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r31,r10,r11
  r31 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // mr r3,r31
  r3 = r31;
  // bl 0x8219d440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D440, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // rlwinm r4,r11,0,30,30
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // bl 0x8219d6f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D6F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x8219d7b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D7B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_821A0D1C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r27
  r3 = r27;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

namespace {

lifted::Result e821633C8(uint8_t* base, const lifted::Args& a) {
  return {f821633C8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821633F0(uint8_t* base, const lifted::Args& a) {
  return {f821633F0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821634D8(uint8_t* base, const lifted::Args& a) {
  return f821634D8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821643E0(uint8_t* base, const lifted::Args& a) {
  return f821643E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82164700(uint8_t* base, const lifted::Args& a) {
  return {f82164700(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82164740(uint8_t* base, const lifted::Args& a) {
  f82164740(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82164820(uint8_t* base, const lifted::Args& a) {
  return f82164820(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e82164B50(uint8_t* base, const lifted::Args& a) {
  return f82164B50(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821654F8(uint8_t* base, const lifted::Args& a) {
  return f821654F8(base, a.r1, a.r13, a.r[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e821665F0(uint8_t* base, const lifted::Args& a) {
  f821665F0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82166658(uint8_t* base, const lifted::Args& a) {
  return f82166658(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82166B40(uint8_t* base, const lifted::Args& a) {
  return f82166B40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82166F98(uint8_t* base, const lifted::Args& a) {
  return f82166F98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821672F0(uint8_t* base, const lifted::Args& a) {
  return f821672F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82167A40(uint8_t* base, const lifted::Args& a) {
  return f82167A40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82167BE0(uint8_t* base, const lifted::Args& a) {
  return f82167BE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82167D88(uint8_t* base, const lifted::Args& a) {
  return f82167D88(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168018(uint8_t* base, const lifted::Args& a) {
  return f82168018(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821683A8(uint8_t* base, const lifted::Args& a) {
  return f821683A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168620(uint8_t* base, const lifted::Args& a) {
  return f82168620(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e82168708(uint8_t* base, const lifted::Args& a) {
  return f82168708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168858(uint8_t* base, const lifted::Args& a) {
  return f82168858(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168910(uint8_t* base, const lifted::Args& a) {
  return f82168910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168AA8(uint8_t* base, const lifted::Args& a) {
  return f82168AA8(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e82168C80(uint8_t* base, const lifted::Args& a) {
  return f82168C80(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82168FB8(uint8_t* base, const lifted::Args& a) {
  return f82168FB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82169030(uint8_t* base, const lifted::Args& a) {
  return f82169030(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82169290(uint8_t* base, const lifted::Args& a) {
  return f82169290(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216C3E8(uint8_t* base, const lifted::Args& a) {
  return f8216C3E8(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e8216C460(uint8_t* base, const lifted::Args& a) {
  return f8216C460(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216C470(uint8_t* base, const lifted::Args& a) {
  return {f8216C470(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8216C480(uint8_t* base, const lifted::Args& a) {
  return f8216C480(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216C4C0(uint8_t* base, const lifted::Args& a) {
  return f8216C4C0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216C500(uint8_t* base, const lifted::Args& a) {
  return f8216C500(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e8216C648(uint8_t* base, const lifted::Args& a) {
  f8216C648(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8216CEB8(uint8_t* base, const lifted::Args& a) {
  return f8216CEB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216D0C8(uint8_t* base, const lifted::Args& a) {
  return f8216D0C8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8216D270(uint8_t* base, const lifted::Args& a) {
  return f8216D270(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8216E668(uint8_t* base, const lifted::Args& a) {
  return f8216E668(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82170550(uint8_t* base, const lifted::Args& a) {
  return f82170550(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82171308(uint8_t* base, const lifted::Args& a) {
  return f82171308(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82179F60(uint8_t* base, const lifted::Args& a) {
  return f82179F60(base, a.r1, a.r13, a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8217A008(uint8_t* base, const lifted::Args& a) {
  return f8217A008(base, a.r1, a.r13, a.r[1], a.r[2], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8217A0A8(uint8_t* base, const lifted::Args& a) {
  return f8217A0A8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8217A1D0(uint8_t* base, const lifted::Args& a) {
  return f8217A1D0(base, a.r1, a.r13, a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8217B6B8(uint8_t* base, const lifted::Args& a) {
  return f8217B6B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8217B740(uint8_t* base, const lifted::Args& a) {
  return f8217B740(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8217C4C0(uint8_t* base, const lifted::Args& a) {
  return f8217C4C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82180C18(uint8_t* base, const lifted::Args& a) {
  f82180C18(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82180C30(uint8_t* base, const lifted::Args& a) {
  f82180C30(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82180CD0(uint8_t* base, const lifted::Args& a) {
  f82180CD0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e82181BE8(uint8_t* base, const lifted::Args& a) {
  f82181BE8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82181FC0(uint8_t* base, const lifted::Args& a) {
  return f82181FC0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82182990(uint8_t* base, const lifted::Args& a) {
  return f82182990(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e82182A80(uint8_t* base, const lifted::Args& a) {
  return f82182A80(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82183078(uint8_t* base, const lifted::Args& a) {
  return {f82183078(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82183BD8(uint8_t* base, const lifted::Args& a) {
  return f82183BD8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82183F68(uint8_t* base, const lifted::Args& a) {
  return f82183F68(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82184268(uint8_t* base, const lifted::Args& a) {
  return f82184268(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82185178(uint8_t* base, const lifted::Args& a) {
  return f82185178(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82185220(uint8_t* base, const lifted::Args& a) {
  return f82185220(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82185400(uint8_t* base, const lifted::Args& a) {
  return f82185400(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82185518(uint8_t* base, const lifted::Args& a) {
  return f82185518(base, a.r1, a.r13, a.r[0], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8218D6D8(uint8_t* base, const lifted::Args& a) {
  f8218D6D8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8218D6F0(uint8_t* base, const lifted::Args& a) {
  f8218D6F0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e8218DE18(uint8_t* base, const lifted::Args& a) {
  return {f8218DE18(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8218EB78(uint8_t* base, const lifted::Args& a) {
  return f8218EB78(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8218F150(uint8_t* base, const lifted::Args& a) {
  return f8218F150(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8218F220(uint8_t* base, const lifted::Args& a) {
  return f8218F220(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8218F2B8(uint8_t* base, const lifted::Args& a) {
  return f8218F2B8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8218F620(uint8_t* base, const lifted::Args& a) {
  return {f8218F620(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8218FAA0(uint8_t* base, const lifted::Args& a) {
  return {f8218FAA0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e82190168(uint8_t* base, const lifted::Args& a) {
  return {f82190168(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82190690(uint8_t* base, const lifted::Args& a) {
  return f82190690(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82190760(uint8_t* base, const lifted::Args& a) {
  return f82190760(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82192958(uint8_t* base, const lifted::Args& a) {
  return f82192958(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e82195F10(uint8_t* base, const lifted::Args& a) {
  return f82195F10(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82196030(uint8_t* base, const lifted::Args& a) {
  return f82196030(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821970B8(uint8_t* base, const lifted::Args& a) {
  return {f821970B8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82198830(uint8_t* base, const lifted::Args& a) {
  return f82198830(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82198B58(uint8_t* base, const lifted::Args& a) {
  return f82198B58(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82198E70(uint8_t* base, const lifted::Args& a) {
  return f82198E70(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82198F08(uint8_t* base, const lifted::Args& a) {
  return f82198F08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82199220(uint8_t* base, const lifted::Args& a) {
  return f82199220(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821995D8(uint8_t* base, const lifted::Args& a) {
  return f821995D8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821997E8(uint8_t* base, const lifted::Args& a) {
  return f821997E8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82199E48(uint8_t* base, const lifted::Args& a) {
  return f82199E48(base, a.r1, a.r13);
}

lifted::Result e8219A450(uint8_t* base, const lifted::Args& a) {
  return f8219A450(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219A6B8(uint8_t* base, const lifted::Args& a) {
  return f8219A6B8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219A8E0(uint8_t* base, const lifted::Args& a) {
  return f8219A8E0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219AAB0(uint8_t* base, const lifted::Args& a) {
  return f8219AAB0(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219AD68(uint8_t* base, const lifted::Args& a) {
  return f8219AD68(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219AE90(uint8_t* base, const lifted::Args& a) {
  return f8219AE90(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219AF50(uint8_t* base, const lifted::Args& a) {
  return f8219AF50(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219EE30(uint8_t* base, const lifted::Args& a) {
  return f8219EE30(base, a.r1, a.r13, a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11]);
}

lifted::Result e8219F3A0(uint8_t* base, const lifted::Args& a) {
  return f8219F3A0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219F520(uint8_t* base, const lifted::Args& a) {
  return f8219F520(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219F690(uint8_t* base, const lifted::Args& a) {
  return f8219F690(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8219FB08(uint8_t* base, const lifted::Args& a) {
  return f8219FB08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821A0B38(uint8_t* base, const lifted::Args& a) {
  return f821A0B38(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

const Registration kEntries[] = {
    {0x821633C8, e821633C8},
    {0x821633F0, e821633F0},
    {0x821634D8, e821634D8},
    {0x821643E0, e821643E0},
    {0x82164700, e82164700},
    {0x82164740, e82164740},
    {0x82164820, e82164820},
    {0x82164B50, e82164B50},
    {0x821654F8, e821654F8},
    {0x821665F0, e821665F0},
    {0x82166658, e82166658},
    {0x82166B40, e82166B40},
    {0x82166F98, e82166F98},
    {0x821672F0, e821672F0},
    {0x82167A40, e82167A40},
    {0x82167BE0, e82167BE0},
    {0x82167D88, e82167D88},
    {0x82168018, e82168018},
    {0x821683A8, e821683A8},
    {0x82168620, e82168620},
    {0x82168708, e82168708},
    {0x82168858, e82168858},
    {0x82168910, e82168910},
    {0x82168AA8, e82168AA8},
    {0x82168C80, e82168C80},
    {0x82168FB8, e82168FB8},
    {0x82169030, e82169030},
    {0x82169290, e82169290},
    {0x8216C3E8, e8216C3E8},
    {0x8216C460, e8216C460},
    {0x8216C470, e8216C470},
    {0x8216C480, e8216C480},
    {0x8216C4C0, e8216C4C0},
    {0x8216C500, e8216C500},
    {0x8216C648, e8216C648},
    {0x8216CEB8, e8216CEB8},
    {0x8216D0C8, e8216D0C8},
    {0x8216D270, e8216D270},
    {0x8216E668, e8216E668},
    {0x82170550, e82170550},
    {0x82171308, e82171308},
    {0x82179F60, e82179F60},
    {0x8217A008, e8217A008},
    {0x8217A0A8, e8217A0A8},
    {0x8217A1D0, e8217A1D0},
    {0x8217B6B8, e8217B6B8},
    {0x8217B740, e8217B740},
    {0x8217C4C0, e8217C4C0},
    {0x82180C18, e82180C18},
    {0x82180C30, e82180C30},
    {0x82180CD0, e82180CD0},
    {0x82181BE8, e82181BE8},
    {0x82181FC0, e82181FC0},
    {0x82182990, e82182990},
    {0x82182A80, e82182A80},
    {0x82183078, e82183078},
    {0x82183BD8, e82183BD8},
    {0x82183F68, e82183F68},
    {0x82184268, e82184268},
    {0x82185178, e82185178},
    {0x82185220, e82185220},
    {0x82185400, e82185400},
    {0x82185518, e82185518},
    {0x8218D6D8, e8218D6D8},
    {0x8218D6F0, e8218D6F0},
    {0x8218DE18, e8218DE18},
    {0x8218EB78, e8218EB78},
    {0x8218F150, e8218F150},
    {0x8218F220, e8218F220},
    {0x8218F2B8, e8218F2B8},
    {0x8218F620, e8218F620},
    {0x8218FAA0, e8218FAA0},
    {0x82190168, e82190168},
    {0x82190690, e82190690},
    {0x82190760, e82190760},
    {0x82192958, e82192958},
    {0x82195F10, e82195F10},
    {0x82196030, e82196030},
    {0x821970B8, e821970B8},
    {0x82198830, e82198830},
    {0x82198B58, e82198B58},
    {0x82198E70, e82198E70},
    {0x82198F08, e82198F08},
    {0x82199220, e82199220},
    {0x821995D8, e821995D8},
    {0x821997E8, e821997E8},
    {0x82199E48, e82199E48},
    {0x8219A450, e8219A450},
    {0x8219A6B8, e8219A6B8},
    {0x8219A8E0, e8219A8E0},
    {0x8219AAB0, e8219AAB0},
    {0x8219AD68, e8219AD68},
    {0x8219AE90, e8219AE90},
    {0x8219AF50, e8219AF50},
    {0x8219EE30, e8219EE30},
    {0x8219F3A0, e8219F3A0},
    {0x8219F520, e8219F520},
    {0x8219F690, e8219F690},
    {0x8219FB08, e8219FB08},
    {0x821A0B38, e821A0B38},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_008)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_821633C8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821633C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821633C8);
}

extern "C" REX_FUNC(sub_821633F0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821633F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821633F0);
}

extern "C" REX_FUNC(sub_821634D8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821634D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821634D8);
}

extern "C" REX_FUNC(sub_821643E0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821643E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821643E0);
}

extern "C" REX_FUNC(sub_82164700) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82164700(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82164700);
}

extern "C" REX_FUNC(sub_82164740) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82164740(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82164740);
}

extern "C" REX_FUNC(sub_82164820) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82164820(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82164820);
}

extern "C" REX_FUNC(sub_82164B50) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82164B50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82164B50);
}

extern "C" REX_FUNC(sub_821654F8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821654F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821654F8);
}

extern "C" REX_FUNC(sub_821665F0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821665F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821665F0);
}

extern "C" REX_FUNC(sub_82166658) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82166658(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82166658);
}

extern "C" REX_FUNC(sub_82166B40) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82166B40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82166B40);
}

extern "C" REX_FUNC(sub_82166F98) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82166F98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82166F98);
}

extern "C" REX_FUNC(sub_821672F0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821672F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821672F0);
}

extern "C" REX_FUNC(sub_82167A40) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82167A40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82167A40);
}

extern "C" REX_FUNC(sub_82167BE0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82167BE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82167BE0);
}

extern "C" REX_FUNC(sub_82167D88) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82167D88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82167D88);
}

extern "C" REX_FUNC(sub_82168018) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168018(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168018);
}

extern "C" REX_FUNC(sub_821683A8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821683A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821683A8);
}

extern "C" REX_FUNC(sub_82168620) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168620(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168620);
}

extern "C" REX_FUNC(sub_82168708) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168708(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168708);
}

extern "C" REX_FUNC(sub_82168858) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168858(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168858);
}

extern "C" REX_FUNC(sub_82168910) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168910(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168910);
}

extern "C" REX_FUNC(sub_82168AA8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168AA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168AA8);
}

extern "C" REX_FUNC(sub_82168C80) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168C80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168C80);
}

extern "C" REX_FUNC(sub_82168FB8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82168FB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82168FB8);
}

extern "C" REX_FUNC(sub_82169030) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82169030(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82169030);
}

extern "C" REX_FUNC(sub_82169290) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82169290(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82169290);
}

extern "C" REX_FUNC(sub_8216C3E8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C3E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C3E8);
}

extern "C" REX_FUNC(sub_8216C460) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C460);
}

extern "C" REX_FUNC(sub_8216C470) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C470(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C470);
}

extern "C" REX_FUNC(sub_8216C480) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C480(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C480);
}

extern "C" REX_FUNC(sub_8216C4C0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C4C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C4C0);
}

extern "C" REX_FUNC(sub_8216C500) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C500(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C500);
}

extern "C" REX_FUNC(sub_8216C648) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216C648(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216C648);
}

extern "C" REX_FUNC(sub_8216CEB8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216CEB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216CEB8);
}

extern "C" REX_FUNC(sub_8216D0C8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216D0C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216D0C8);
}

extern "C" REX_FUNC(sub_8216D270) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216D270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216D270);
}

extern "C" REX_FUNC(sub_8216E668) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8216E668(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8216E668);
}

extern "C" REX_FUNC(sub_82170550) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82170550(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82170550);
}

extern "C" REX_FUNC(sub_82171308) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82171308(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82171308);
}

extern "C" REX_FUNC(sub_82179F60) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82179F60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82179F60);
}

extern "C" REX_FUNC(sub_8217A008) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217A008(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217A008);
}

extern "C" REX_FUNC(sub_8217A0A8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217A0A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217A0A8);
}

extern "C" REX_FUNC(sub_8217A1D0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217A1D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217A1D0);
}

extern "C" REX_FUNC(sub_8217B6B8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217B6B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217B6B8);
}

extern "C" REX_FUNC(sub_8217B740) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217B740(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217B740);
}

extern "C" REX_FUNC(sub_8217C4C0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8217C4C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8217C4C0);
}

extern "C" REX_FUNC(sub_82180C18) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82180C18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82180C18);
}

extern "C" REX_FUNC(sub_82180C30) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82180C30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82180C30);
}

extern "C" REX_FUNC(sub_82180CD0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82180CD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82180CD0);
}

extern "C" REX_FUNC(sub_82181BE8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82181BE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82181BE8);
}

extern "C" REX_FUNC(sub_82181FC0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82181FC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82181FC0);
}

extern "C" REX_FUNC(sub_82182990) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82182990(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82182990);
}

extern "C" REX_FUNC(sub_82182A80) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82182A80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82182A80);
}

extern "C" REX_FUNC(sub_82183078) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82183078(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82183078);
}

extern "C" REX_FUNC(sub_82183BD8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82183BD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82183BD8);
}

extern "C" REX_FUNC(sub_82183F68) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82183F68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82183F68);
}

extern "C" REX_FUNC(sub_82184268) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82184268(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82184268);
}

extern "C" REX_FUNC(sub_82185178) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82185178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82185178);
}

extern "C" REX_FUNC(sub_82185220) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82185220(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82185220);
}

extern "C" REX_FUNC(sub_82185400) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82185400(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82185400);
}

extern "C" REX_FUNC(sub_82185518) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82185518(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82185518);
}

extern "C" REX_FUNC(sub_8218D6D8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218D6D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218D6D8);
}

extern "C" REX_FUNC(sub_8218D6F0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218D6F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218D6F0);
}

extern "C" REX_FUNC(sub_8218DE18) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218DE18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218DE18);
}

extern "C" REX_FUNC(sub_8218EB78) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218EB78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218EB78);
}

extern "C" REX_FUNC(sub_8218F150) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218F150(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218F150);
}

extern "C" REX_FUNC(sub_8218F220) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218F220(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218F220);
}

extern "C" REX_FUNC(sub_8218F2B8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218F2B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218F2B8);
}

extern "C" REX_FUNC(sub_8218F620) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218F620(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218F620);
}

extern "C" REX_FUNC(sub_8218FAA0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8218FAA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8218FAA0);
}

extern "C" REX_FUNC(sub_82190168) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82190168(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82190168);
}

extern "C" REX_FUNC(sub_82190690) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82190690(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82190690);
}

extern "C" REX_FUNC(sub_82190760) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82190760(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82190760);
}

extern "C" REX_FUNC(sub_82192958) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82192958(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82192958);
}

extern "C" REX_FUNC(sub_82195F10) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82195F10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82195F10);
}

extern "C" REX_FUNC(sub_82196030) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82196030(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82196030);
}

extern "C" REX_FUNC(sub_821970B8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821970B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821970B8);
}

extern "C" REX_FUNC(sub_82198830) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82198830(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82198830);
}

extern "C" REX_FUNC(sub_82198B58) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82198B58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82198B58);
}

extern "C" REX_FUNC(sub_82198E70) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82198E70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82198E70);
}

extern "C" REX_FUNC(sub_82198F08) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82198F08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82198F08);
}

extern "C" REX_FUNC(sub_82199220) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82199220(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82199220);
}

extern "C" REX_FUNC(sub_821995D8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821995D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821995D8);
}

extern "C" REX_FUNC(sub_821997E8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821997E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821997E8);
}

extern "C" REX_FUNC(sub_82199E48) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_82199E48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82199E48);
}

extern "C" REX_FUNC(sub_8219A450) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219A450(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219A450);
}

extern "C" REX_FUNC(sub_8219A6B8) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219A6B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219A6B8);
}

extern "C" REX_FUNC(sub_8219A8E0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219A8E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219A8E0);
}

extern "C" REX_FUNC(sub_8219AAB0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219AAB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219AAB0);
}

extern "C" REX_FUNC(sub_8219AD68) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219AD68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219AD68);
}

extern "C" REX_FUNC(sub_8219AE90) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219AE90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219AE90);
}

extern "C" REX_FUNC(sub_8219AF50) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219AF50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219AF50);
}

extern "C" REX_FUNC(sub_8219EE30) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219EE30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219EE30);
}

extern "C" REX_FUNC(sub_8219F3A0) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219F3A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219F3A0);
}

extern "C" REX_FUNC(sub_8219F520) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219F520(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219F520);
}

extern "C" REX_FUNC(sub_8219F690) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219F690(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219F690);
}

extern "C" REX_FUNC(sub_8219FB08) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_8219FB08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8219FB08);
}

extern "C" REX_FUNC(sub_821A0B38) {
  if (!ITC_FLAG(native_lifted_008)) {
    __imp__sub_821A0B38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821A0B38);
}
