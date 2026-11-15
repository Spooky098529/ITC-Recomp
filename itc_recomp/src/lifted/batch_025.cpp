// Lift sweep batch 025 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_025, true, "ITC",
              "Run lift sweep batch 025's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_825D1D70: lifted from generated/ by lift.py.
lifted::Result f825D1D70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1D70 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,44(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 44);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D1D90: lifted from generated/ by lift.py.
lifted::Result f825D1D90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1D90 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,44(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 44);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D1DB0: lifted from generated/ by lift.py.
lifted::Result f825D1DB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1DB0 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,44(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 44);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D1DD0: lifted from generated/ by lift.py.
lifted::Result f825D1DD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1DD0 (lifted)");
  uint64_t r10 = 0, r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,44(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 44);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r3,r11
  r3 = r11;
  // lwz r11,48(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 48);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D2300: lifted from generated/ by lift.py.
lifted::Result f825D2300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D2300 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82379d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379D50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A8958, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825cee60
  // (left out: dead in C++)
  r3 = f825CEE60(base, r3);
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // lis r10,4194
  r10 = uint64_t(int64_t(274857984));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // ori r10,r10,19923
  r10 = r10 | 19923;
  // mullw r11,r11,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r31))));
  // mulhwu r10,r3,r10
  r10 = (uint64_t(uint32_t(r3)) * uint64_t(uint32_t(r10))) >> 32;
  // rlwinm r10,r10,26,6,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 26) & 0x3FFFFFF;
  // add r11,r11,r29
  r11 = r11 + r29;
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r3,r11,r10
  r3 = uint32_t(uint32_t(r10) ? uint32_t(r11) / uint32_t(r10) : 0);
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D2380: lifted from generated/ by lift.py.
lifted::Result f825D2380(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D2380 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825cee00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE00, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825cee60
  // (left out: dead in C++)
  r3 = f825CEE60(base, r3);
  // lis r11,4194
  r11 = uint64_t(int64_t(274857984));
  // ori r11,r11,19923
  r11 = r11 | 19923;
  // mulhwu r11,r3,r11
  r11 = (uint64_t(uint32_t(r3)) * uint64_t(uint32_t(r11))) >> 32;
  // rlwinm r11,r11,26,6,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 26) & 0x3FFFFFF;
  // divwu r3,r31,r11
  r3 = uint32_t(uint32_t(r11) ? uint32_t(r31) / uint32_t(r11) : 0);
  // twllei r11,0
  if (int32_t(r11) == 0 || uint32_t(r11) < 0u) lifted::Trap(0);
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

// sub_825D24B0: lifted from generated/ by lift.py.
lifted::Result f825D24B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D24B0 (lifted)");
  uint64_t r10 = 0, r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // mr r11,r3
  r11 = r3;
  // mr r3,r4
  r3 = r4;
  // addic. r10,r11,-12
  ca = uint32_t(r11) > 11;
  r10 = uint64_t(int64_t(int64_t(r11) + -12));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x825d24c4
  if (!cr0.eq) goto loc_825D24C4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825D24C4:
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r4,r11
  r4 = r11;
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D2A48: lifted from generated/ by lift.py.
lifted::Result f825D2A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D2A48 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,27192
  r11 = uint64_t(int64_t(int64_t(r11) + 27192));
  // addi r10,r10,27248
  r10 = uint64_t(int64_t(int64_t(r10) + 27248));
  // lbz r9,25(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 25);
  // mr r30,r4
  r30 = r4;
  // rlwinm r9,r9,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // stw r10,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r10));
  // bne cr6,0x825d2a90
  if (!cr6.eq) goto loc_825D2A90;
  // bl 0x825d28d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D28D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D2A90:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // clrlwi r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d2abc
  if (cr6.eq) goto loc_825D2ABC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32870
  r4 = r4 | 32870;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D2ABC:
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

// sub_825D2AE8: lifted from generated/ by lift.py.
lifted::Result f825D2AE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2AE8 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r4
  r28 = r4;
  // mr r27,r5
  r27 = r5;
  // mr r26,r6
  r26 = r6;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r30,r28,8
  r30 = uint64_t(int64_t(int64_t(r28) + 8));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825D2B0C:
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // lhz r11,2(r28)
  r11 = REX_LOAD_U16(uint32_t(r28) + 2);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x825d2b70
  if (!cr6.lt) goto loc_825D2B70;
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r11,r31,r11
  r11 = r31 + r11;
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // add r11,r11,r30
  r11 = r11 + r30;
  // mtctr r9
  ctr = r9;
loc_825D2B34:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // bdnz 0x825d2b34
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D2B34;
  // mr r3,r27
  r3 = r27;
  // lbz r4,82(r1)
  r4 = REX_LOAD_U8(uint32_t(r1) + 82);
  // bl 0x825c7218
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7218, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r3,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r26), so);
  // bne cr6,0x825d2b60
  if (!cr6.eq) goto loc_825D2B60;
  // li r29,1
  r29 = uint64_t(int64_t(1));
loc_825D2B60:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // beq cr6,0x825d2b0c
  if (cr6.eq) goto loc_825D2B0C;
loc_825D2B70:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D2B80: lifted from generated/ by lift.py.
lifted::Result f825D2B80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D2B80 (lifted)");
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r25,r6
  r25 = r6;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // mr r29,r5
  r29 = r5;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r26
  r30 = r26;
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r24,r3
  r24 = r3;
  // lbz r11,0(r24)
  r11 = REX_LOAD_U8(uint32_t(r24) + 0);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x825d2cf8
  if (!cr6.eq) goto loc_825D2CF8;
  // lbz r11,9(r24)
  r11 = REX_LOAD_U8(uint32_t(r24) + 9);
  // mr r28,r26
  r28 = r26;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d2cec
  if (cr6.eq) goto loc_825D2CEC;
loc_825D2BE0:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x825d2cec
  if (!cr6.eq) goto loc_825D2CEC;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r5,r28
  r5 = r28;
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r9,0(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r10,1(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 1);
  // lwz r9,24(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 24);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r26,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r26));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d2cd0
  if (cr6.eq) goto loc_825D2CD0;
loc_825D2C40:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x825d2c94
  if (cr6.eq) goto loc_825D2C94;
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // beq cr6,0x825d2c94
  if (cr6.eq) goto loc_825D2C94;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // beq cr6,0x825d2c94
  if (cr6.eq) goto loc_825D2C94;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // beq cr6,0x825d2c78
  if (cr6.eq) goto loc_825D2C78;
  // cmplwi cr6,r10,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 6, so);
  // beq cr6,0x825d2c78
  if (cr6.eq) goto loc_825D2C78;
  // cmplwi cr6,r10,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 5, so);
  // bne cr6,0x825d2cac
  if (!cr6.eq) goto loc_825D2CAC;
loc_825D2C78:
  // mr r6,r29
  r6 = r29;
  // mr r5,r31
  r5 = r31;
  // addi r4,r11,9
  r4 = uint64_t(int64_t(int64_t(r11) + 9));
  // mr r3,r27
  r3 = r27;
  // bl 0x825d2ae8
  // (left out: dead in C++)
  { const lifted::Result o = f825D2AE8(base, r1, r13, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // b 0x825d2cac
  goto loc_825D2CAC;
loc_825D2C94:
  // mr r3,r31
  r3 = r31;
  // lbz r4,10(r11)
  r4 = REX_LOAD_U8(uint32_t(r11) + 10);
  // bl 0x825c7218
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7218, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // subf r11,r3,r29
  r11 = r29 - r3;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
loc_825D2CAC:
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d2c40
  if (!cr6.eq) goto loc_825D2C40;
loc_825D2CD0:
  // clrlwi r11,r28,24
  r11 = uint32_t(r28) & 0xFF;
  // lbz r10,9(r24)
  r10 = REX_LOAD_U8(uint32_t(r24) + 9);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r28,r11,24
  r28 = uint32_t(r11) & 0xFF;
  // mr r11,r28
  r11 = r28;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825d2be0
  if (cr6.lt) goto loc_825D2BE0;
loc_825D2CEC:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_825D2CF8:
  // mr r3,r31
  r3 = r31;
  // lbz r4,11(r24)
  r4 = REX_LOAD_U8(uint32_t(r24) + 11);
  // bl 0x825c7218
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7218, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // subf r11,r3,r29
  r11 = r29 - r3;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D2D18: lifted from generated/ by lift.py.
lifted::Result f825D2D18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2D18 (lifted)");
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
  // addi r11,r11,27240
  r11 = uint64_t(int64_t(int64_t(r11) + 27240));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d2d54
  if (cr6.eq) goto loc_825D2D54;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32791
  r4 = r4 | 32791;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D2D54:
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

// sub_825D2D68: lifted from generated/ by lift.py.
lifted::Result f825D2D68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2D68 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
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
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r23,r6
  r23 = r6;
  // fmr f31,f1
  f31 = f1;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // mr r24,r5
  r24 = r5;
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // mr r25,r3
  r25 = r3;
  // stw r11,0(r23)
  REX_STORE_U32(uint32_t(r23) + 0, uint32_t(r11));
  // sth r10,0(r24)
  REX_STORE_U16(uint32_t(r24) + 0, uint16_t(r10));
  // lwz r11,8(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 8);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r28,r11,16
  r28 = uint32_t(r11) & 0xFFFF;
  // rlwinm r3,r28,1,0,30
  r3 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 1) & 0xFFFFFFFE;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825d2e88
  if (cr6.eq) goto loc_825D2E88;
  // lwz r11,8(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 8);
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r26,r11,8
  r26 = uint64_t(int64_t(int64_t(r11) + 8));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825d2e3c
  if (cr6.eq) goto loc_825D2E3C;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825D2DD8:
  // rlwinm r11,r30,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 4) & 0xFFFFFFF0;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // add r4,r11,r26
  r4 = r11 + r26;
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,100(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 100);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // bgt cr6,0x825d2e28
  if (cr6.gt) goto loc_825D2E28;
  // lwz r11,104(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 104);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lfs f0,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x825d2e28
  if (cr6.lt) goto loc_825D2E28;
  // clrlwi r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r29,r11,16
  r29 = uint32_t(r11) & 0xFFFF;
  // sthx r31,r10,r27
  REX_STORE_U16(uint32_t(r10) + uint32_t(r27), uint16_t(r31));
loc_825D2E28:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // mr r30,r31
  r30 = r31;
  // cmplw cr6,r30,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r28), so);
  // blt cr6,0x825d2dd8
  if (cr6.lt) goto loc_825D2DD8;
loc_825D2E3C:
  // clrlwi r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d2e70
  if (cr6.eq) goto loc_825D2E70;
  // addi r5,r11,-1
  r5 = uint64_t(int64_t(int64_t(r11) + -1));
  // lwz r3,4(r25)
  r3 = REX_LOAD_U32(uint32_t(r25) + 4);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825bf6b8
  // (left out: dead in C++)
  r3 = f825BF6B8(base, r1, r13, r3, r4, r5);
  // rlwinm r11,r3,1,15,30
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x1FFFE;
  // lhzx r11,r11,r27
  r11 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r27));
  // rotlwi r10,r11,4
  r10 = __builtin_rotateleft32(uint32_t(r11), 4);
  // lwzx r10,r10,r26
  r10 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r26));
  // stw r10,0(r23)
  REX_STORE_U32(uint32_t(r23) + 0, uint32_t(r10));
  // sth r11,0(r24)
  REX_STORE_U16(uint32_t(r24) + 0, uint16_t(r11));
loc_825D2E70:
  // mr r3,r27
  r3 = r27;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r22
  r3 = r22;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f31,-96(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
loc_825D2E88:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfd f31,-96(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -96)));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D2EA0: lifted from generated/ by lift.py.
uint64_t f825D2EA0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2EA0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // rlwinm r10,r4,4,12,27
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 4) & 0xFFFF0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r10,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r10));
  // lfs f0,-16(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bltlr cr6
  if (cr6.lt) return r3;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // lfs f0,-16(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgtlr cr6
  if (cr6.gt) return r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_825D2EF0: lifted from generated/ by lift.py.
lifted::Result f825D2EF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2EF0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r3
  r28 = r3;
  // mr r27,r4
  r27 = r4;
  // mr r26,r5
  r26 = r5;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,8(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 8);
  // addi r30,r10,8
  r30 = uint64_t(int64_t(int64_t(r10) + 8));
loc_825D2F18:
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // lwz r11,8(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 8);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x825d2f84
  if (!cr6.lt) goto loc_825D2F84;
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r11,r31,r11
  r11 = r31 + r11;
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // add r11,r11,r30
  r11 = r11 + r30;
  // mtctr r9
  ctr = r9;
loc_825D2F48:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // bdnz 0x825d2f48
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D2F48;
  // mr r3,r27
  r3 = r27;
  // lbz r4,82(r1)
  r4 = REX_LOAD_U8(uint32_t(r1) + 82);
  // bl 0x825c7218
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7218, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r3,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r26), so);
  // bne cr6,0x825d2f74
  if (!cr6.eq) goto loc_825D2F74;
  // li r29,1
  r29 = uint64_t(int64_t(1));
loc_825D2F74:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // beq cr6,0x825d2f18
  if (cr6.eq) goto loc_825D2F18;
loc_825D2F84:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D2F90: lifted from generated/ by lift.py.
lifted::Result f825D2F90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D2F90 (lifted)");
  uint64_t r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r27,r5
  r27 = r5;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // addi r29,r10,8
  r29 = uint64_t(int64_t(int64_t(r10) + 8));
loc_825D2FB8:
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x825d3024
  if (!cr6.lt) goto loc_825D3024;
  // rlwinm r11,r31,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 1) & 0xFFFFFFFE;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r11,r31,r11
  r11 = r31 + r11;
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r29
  r11 = r11 + r29;
  // mtctr r9
  ctr = r9;
loc_825D2FEC:
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r9,0(r10)
  REX_STORE_U8(uint32_t(r10) + 0, uint8_t(r9));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // bdnz 0x825d2fec
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D2FEC;
  // mr r5,r27
  r5 = r27;
  // lwz r6,80(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x825d2b80
  // (left out: dead in C++)
  { const lifted::Result o = f825D2B80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // beq cr6,0x825d2fb8
  if (cr6.eq) goto loc_825D2FB8;
loc_825D3024:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D3030: lifted from generated/ by lift.py.
lifted::Result f825D3030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D3030 (lifted)");
  uint64_t r6 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // addi r27,r10,8
  r27 = uint64_t(int64_t(int64_t(r10) + 8));
loc_825D3058:
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bge cr6,0x825d30a8
  if (!cr6.lt) goto loc_825D30A8;
  // rlwinm r11,r31,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 4) & 0xFFFFFFF0;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r4,r11,r27
  r4 = r11 + r27;
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r28
  r5 = r28;
  // mr r4,r29
  r4 = r29;
  // lwz r6,80(r1)
  r6 = REX_LOAD_U32(uint32_t(r1) + 80);
  // mr r3,r30
  r3 = r30;
  // bl 0x825d2b80
  // (left out: dead in C++)
  { const lifted::Result o = f825D2B80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // beq cr6,0x825d3058
  if (cr6.eq) goto loc_825D3058;
loc_825D30A8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D30B0: lifted from generated/ by lift.py.
uint64_t f825D30B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D30B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r5,12(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x825bf650
  // (left out: dead in C++)
  r3 = f825BF650(base, r1, r13, r3, r4, r5);
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r7,r3
  r7 = r3;
  // ori r8,r11,65535
  r8 = r11 | 65535;
  // lwz r10,0(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 0);
  // add r11,r10,r8
  r11 = r10 + r8;
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d3160
  if (cr6.eq) goto loc_825D3160;
  // rlwinm r6,r10,13,29,31
  r6 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 13) & 0x7;
loc_825D3108:
  // cmplwi cr6,r6,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 1, so);
  // blt cr6,0x825d3128
  if (cr6.lt) goto loc_825D3128;
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r10,r11,r10
  r10 = r11 + r10;
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lbz r10,13(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 13);
  // b 0x825d3138
  goto loc_825D3138;
loc_825D3128:
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r11,r10
  r10 = r11 + r10;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lbz r10,12(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 12);
loc_825D3138:
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r30,r10,1
  r30 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r7,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r30), so);
  // ble cr6,0x825d3160
  if (!cr6.gt) goto loc_825D3160;
  // add r11,r11,r8
  r11 = r11 + r8;
  // clrlwi r3,r11,16
  r3 = uint32_t(r11) & 0xFFFF;
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d3108
  if (!cr6.eq) goto loc_825D3108;
loc_825D3160:
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

// sub_825D3178: lifted from generated/ by lift.py.
uint64_t f825D3178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D3178 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r26,r3
  r26 = r3;
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // ble cr6,0x825d3330
  if (!cr6.gt) goto loc_825D3330;
  // rotlwi r10,r11,0
  r10 = __builtin_rotateleft32(uint32_t(r11), 0);
  // addi r27,r10,8
  r27 = uint64_t(int64_t(int64_t(r10) + 8));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r10,r9,13,29,31
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 13) & 0x7;
  // addis r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 65536));
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // clrlwi r25,r9,16
  r25 = uint32_t(r9) & 0xFFFF;
  // bgt cr6,0x825d3200
  if (cr6.gt) goto loc_825D3200;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,12760
  r12 = uint64_t(int64_t(int64_t(r12) + 12760));
  // rlwinm r0,r10,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_825D31E8;
  case 1:
  goto loc_825D31F0;
  case 2:
  goto loc_825D31F0;
  case 3:
  goto loc_825D31F8;
  default:
  std::abort();
  }
loc_825D31E8:
  // li r31,5
  r31 = uint64_t(int64_t(5));
  // b 0x825d3204
  goto loc_825D3204;
loc_825D31F0:
  // li r31,6
  r31 = uint64_t(int64_t(6));
  // b 0x825d3204
  goto loc_825D3204;
loc_825D31F8:
  // li r31,16
  r31 = uint64_t(int64_t(16));
  // b 0x825d3204
  goto loc_825D3204;
loc_825D3200:
  // li r31,3
  r31 = uint64_t(int64_t(3));
loc_825D3204:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825d32c4
  if (cr6.eq) goto loc_825D32C4;
  // li r28,0
  r28 = uint64_t(int64_t(0));
loc_825D321C:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 4);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // addi r5,r11,-1
  r5 = uint64_t(int64_t(int64_t(r11) + -1));
  // bl 0x825bf650
  // (left out: dead in C++)
  r3 = f825BF650(base, r1, r13, r3, r4, r5);
  // clrlwi r10,r3,16
  r10 = uint32_t(r3) & 0xFFFF;
  // clrlwi r11,r25,16
  r11 = uint32_t(r25) & 0xFFFF;
  // mr r29,r10
  r29 = r10;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // bne cr6,0x825d3250
  if (!cr6.eq) goto loc_825D3250;
  // mr r25,r30
  r25 = r30;
  // b 0x825d325c
  goto loc_825D325C;
loc_825D3250:
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bne cr6,0x825d325c
  if (!cr6.eq) goto loc_825D325C;
  // mr r25,r10
  r25 = r10;
loc_825D325C:
  // cmplw cr6,r28,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r29), so);
  // beq cr6,0x825d32a4
  if (cr6.eq) goto loc_825D32A4;
  // mullw r11,r28,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r28)) * int64_t(int32_t(r31))));
  // add r30,r11,r27
  r30 = r11 + r27;
  // mr r5,r31
  r5 = r31;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r30
  r4 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mullw r11,r29,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r29)) * int64_t(int32_t(r31))));
  // add r29,r11,r27
  r29 = r11 + r27;
  // mr r5,r31
  r5 = r31;
  // mr r3,r30
  r3 = r30;
  // mr r4,r29
  r4 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r29
  r3 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D32A4:
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // addi r10,r28,1
  r10 = uint64_t(int64_t(int64_t(r28) + 1));
  // clrlwi r30,r10,16
  r30 = uint32_t(r10) & 0xFFFF;
  // mr r28,r30
  r28 = r30;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplw cr6,r28,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r10), so);
  // blt cr6,0x825d321c
  if (cr6.lt) goto loc_825D321C;
loc_825D32C4:
  // clrlwi r11,r25,16
  r11 = uint32_t(r25) & 0xFFFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d3330
  if (!cr6.eq) goto loc_825D3330;
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 4);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // addi r5,r11,-2
  r5 = uint64_t(int64_t(int64_t(r11) + -2));
  // bl 0x825bf650
  // (left out: dead in C++)
  r3 = f825BF650(base, r1, r13, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,16
  r11 = uint32_t(r30) & 0xFFFF;
  // mr r5,r31
  r5 = r31;
  // mullw r11,r11,r31
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r31))));
  // add r30,r11,r27
  r30 = r11 + r27;
  // mr r3,r27
  r3 = r27;
  // mr r4,r30
  r4 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D3330:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_825D3338: lifted from generated/ by lift.py.
lifted::Result f825D3338(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D3338 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r11,r11,13,29,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 13) & 0x7;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d3350
  if (!cr6.eq) goto loc_825D3350;
  // b 0x825d2ef0
  { const lifted::Result o = f825D2EF0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825D3350:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x825d335c
  if (!cr6.eq) goto loc_825D335C;
  // b 0x825d2f90
  { const lifted::Result o = f825D2F90(base, r1, r13, r3, r4, r5, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825D335C:
  // b 0x825d3030
  { const lifted::Result o = f825D3030(base, r1, r13, r3, r4, r5, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D3360: lifted from generated/ by lift.py.
void f825D3360(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D3360 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r4,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r4));
  // stw r5,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r5));
  // lis r10,24
  r10 = uint64_t(int64_t(1572864));
  // addi r11,r11,27240
  r11 = uint64_t(int64_t(int64_t(r11) + 27240));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r11));
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // rlwinm r9,r11,0,10,12
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x380000;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // beqlr cr6
  if (cr6.eq) return;
  // rlwinm r10,r11,16,29,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0x7;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x825d33a4
  if (cr6.eq) goto loc_825D33A4;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bnelr cr6
  if (!cr6.eq) return;
loc_825D33A4:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // addi r8,r5,8
  r8 = uint64_t(int64_t(int64_t(r5) + 8));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825D33B8:
  // lwz r10,0(r5)
  r10 = REX_LOAD_U32(uint32_t(r5) + 0);
  // rlwinm r10,r10,13,29,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 13) & 0x7;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x825d33e0
  if (cr6.lt) goto loc_825D33E0;
  // rlwinm r10,r11,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r10,r11,r10
  r10 = r11 + r10;
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r8
  r10 = r10 + r8;
  // lbz r10,5(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 5);
  // b 0x825d33f0
  goto loc_825D33F0;
loc_825D33E0:
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r11,r10
  r10 = r11 + r10;
  // add r10,r10,r8
  r10 = r10 + r8;
  // lbz r10,4(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 4);
loc_825D33F0:
  // lwz r9,12(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 12);
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // add r10,r10,r9
  r10 = r10 + r9;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r10));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825d33b8
  if (cr6.lt) goto loc_825D33B8;
  // blr
  return;
}

// sub_825D3428: lifted from generated/ by lift.py.
uint64_t f825D3428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D3428 (lifted)");
  uint64_t r0 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r30,r3
  r30 = r3;
  // li r31,-1
  r31 = uint64_t(int64_t(-1));
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // rlwinm r9,r11,16,29,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0x7;
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // bgt cr6,0x825d3580
  if (cr6.gt) goto loc_825D3580;
  // lis r12,-32163
  r12 = uint64_t(int64_t(-2107834368));
  // addi r12,r12,13424
  r12 = uint64_t(int64_t(int64_t(r12) + 13424));
  // rlwinm r0,r9,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r9)) {
  case 0:
  goto loc_825D3484;
  case 1:
  goto loc_825D34B8;
  case 2:
  goto loc_825D3508;
  case 3:
  goto loc_825D3518;
  case 4:
  goto loc_825D353C;
  default:
  std::abort();
  }
loc_825D3484:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lhz r8,4(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 4);
  // clrlwi r11,r9,16
  r11 = uint32_t(r9) & 0xFFFF;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bge cr6,0x825d34b0
  if (!cr6.lt) goto loc_825D34B0;
  // lhz r11,4(r10)
  r11 = REX_LOAD_U16(uint32_t(r10) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // b 0x825d3580
  goto loc_825D3580;
loc_825D34B0:
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // b 0x825d3580
  goto loc_825D3580;
loc_825D34B8:
  // lhz r10,4(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 4);
  // cmplwi cr6,r10,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 65535, so);
  // beq cr6,0x825d34ec
  if (cr6.eq) goto loc_825D34EC;
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lhz r8,4(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 4);
  // clrlwi r11,r9,16
  r11 = uint32_t(r9) & 0xFFFF;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bge cr6,0x825d34b0
  if (!cr6.lt) goto loc_825D34B0;
  // addi r11,r10,1
  r11 = uint64_t(int64_t(int64_t(r10) + 1));
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // b 0x825d3580
  goto loc_825D3580;
loc_825D34EC:
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r5,r11,-1
  r5 = uint64_t(int64_t(int64_t(r11) + -1));
  // bl 0x825bf650
  // (left out: dead in C++)
  r3 = f825BF650(base, r1, r13, r3, r4, r5);
  // clrlwi r31,r3,16
  r31 = uint32_t(r3) & 0xFFFF;
  // b 0x825d3580
  goto loc_825D3580;
loc_825D3508:
  // mr r3,r30
  r3 = r30;
  // bl 0x825d30b0
  // (left out: dead in C++)
  r3 = f825D30B0(base, r1, r13, r3);
  // mr r31,r3
  r31 = r3;
  // b 0x825d3580
  goto loc_825D3580;
loc_825D3518:
  // mr r3,r30
  r3 = r30;
  // bl 0x825d30b0
  // (left out: dead in C++)
  r3 = f825D30B0(base, r1, r13, r3);
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // mr r31,r3
  r31 = r3;
  // clrlwi r10,r31,16
  r10 = uint32_t(r31) & 0xFFFF;
  // lhz r11,4(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // beq cr6,0x825d3518
  if (cr6.eq) goto loc_825D3518;
  // b 0x825d3580
  goto loc_825D3580;
loc_825D353C:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lhz r8,4(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 4);
  // clrlwi r11,r9,16
  r11 = uint32_t(r9) & 0xFFFF;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpw cr6,r8,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r11), so);
  // bge cr6,0x825d3568
  if (!cr6.lt) goto loc_825D3568;
  // lhz r11,4(r10)
  r11 = REX_LOAD_U16(uint32_t(r10) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r31,r11,16
  r31 = uint32_t(r11) & 0xFFFF;
  // b 0x825d356c
  goto loc_825D356C;
loc_825D3568:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825D356C:
  // clrlwi r11,r31,16
  r11 = uint32_t(r31) & 0xFFFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d3580
  if (!cr6.eq) goto loc_825D3580;
  // mr r3,r30
  r3 = r30;
  // bl 0x825d3178
  // (left out: dead in C++)
  r3 = f825D3178(base, r1, r13, r3);
loc_825D3580:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // mr r3,r31
  r3 = r31;
  // sth r31,4(r11)
  REX_STORE_U16(uint32_t(r11) + 4, uint16_t(r31));
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

// sub_825D3618: lifted from generated/ by lift.py.
uint64_t f825D3618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D3618 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825d3428
  // (left out: dead in C++)
  r3 = f825D3428(base, r1, r13, r3);
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x825d3668
  if (cr6.eq) goto loc_825D3668;
  // rlwinm r9,r11,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r11,r9
  r11 = r11 + r9;
  // rlwinm r11,r11,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // b 0x825d3670
  goto loc_825D3670;
loc_825D3668:
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,20
  r3 = r3 | 20;
loc_825D3670:
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

// sub_825D3688: lifted from generated/ by lift.py.
void f825D3688(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D3688 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r4,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r4));
  // sth r5,8(r3)
  REX_STORE_U16(uint32_t(r3) + 8, uint16_t(r5));
  // addi r11,r11,27244
  r11 = uint64_t(int64_t(int64_t(r11) + 27244));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_825D36A0: lifted from generated/ by lift.py.
lifted::Result f825D36A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D36A0 (lifted)");
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
  // addi r11,r11,27244
  r11 = uint64_t(int64_t(int64_t(r11) + 27244));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d36dc
  if (cr6.eq) goto loc_825D36DC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32813
  r4 = r4 | 32813;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D36DC:
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

// sub_825D3F28: lifted from generated/ by lift.py.
lifted::Result f825D3F28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D3F28 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // bl 0x825d23f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D23F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lbz r8,176(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 176);
  // addi r10,r11,27300
  r10 = uint64_t(int64_t(int64_t(r11) + 27300));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // clrlwi r8,r8,28
  r8 = uint32_t(r8) & 0xF;
  // addi r9,r11,27248
  r9 = uint64_t(int64_t(int64_t(r11) + 27248));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stb r8,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r8));
  // stw r9,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r9));
  // addi r9,r31,116
  r9 = uint64_t(int64_t(int64_t(r31) + 116));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // stw r11,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r11));
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
  // stw r11,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r11));
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
  // stw r11,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r11));
  // stw r11,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r11));
  // stw r11,160(r31)
  REX_STORE_U32(uint32_t(r31) + 160, uint32_t(r11));
  // stw r11,164(r31)
  REX_STORE_U32(uint32_t(r31) + 164, uint32_t(r11));
  // stw r11,168(r31)
  REX_STORE_U32(uint32_t(r31) + 168, uint32_t(r11));
  // stw r11,172(r31)
  REX_STORE_U32(uint32_t(r31) + 172, uint32_t(r11));
  // mtctr r10
  ctr = r10;
loc_825D3FA8:
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x825d3fa8
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D3FA8;
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

// sub_825D4020: lifted from generated/ by lift.py.
lifted::Result f825D4020(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D4020 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // addi r11,r11,27300
  r11 = uint64_t(int64_t(int64_t(r11) + 27300));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r10,r10,27248
  r10 = uint64_t(int64_t(int64_t(r10) + 27248));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // ori r11,r11,128
  r11 = r11 | 128;
  // stw r10,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r10));
  // stb r11,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r11));
  // bl 0x825d28d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D28D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d40a8
  if (cr6.eq) goto loc_825D40A8;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = f825D5088(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // beq cr6,0x825d4088
  if (cr6.eq) goto loc_825D4088;
  // bl 0x825d50e8
  // (left out: dead in C++)
  r3 = f825D50E8(base, r1, r13, r3);
  // b 0x825d40a4
  goto loc_825D40A4;
loc_825D4088:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d40a4
  if (cr6.eq) goto loc_825D40A4;
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
loc_825D40A4:
  // stw r30,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r30));
loc_825D40A8:
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d40ec
  if (cr6.eq) goto loc_825D40EC;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = f825D5088(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // beq cr6,0x825d40cc
  if (cr6.eq) goto loc_825D40CC;
  // bl 0x825d50e8
  // (left out: dead in C++)
  r3 = f825D50E8(base, r1, r13, r3);
  // b 0x825d40e8
  goto loc_825D40E8;
loc_825D40CC:
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d40e8
  if (cr6.eq) goto loc_825D40E8;
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
loc_825D40E8:
  // stw r30,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r30));
loc_825D40EC:
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4104
  if (cr6.eq) goto loc_825D4104;
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // bl 0x825cfae0
  // (left out: dead in C++)
  { const lifted::Result o = f825CFAE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_825D4104:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d29e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D29E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_825D4128: lifted from generated/ by lift.py.
lifted::Result f825D4128(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D4128 (lifted)");
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r10,156(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 156);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825d41dc
  if (cr6.lt) goto loc_825D41DC;
  // lwz r11,76(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 76);
  // lwz r10,72(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825d4184
  if (!cr6.eq) goto loc_825D4184;
  // lbz r10,24(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x825d4184
  if (!cr6.eq) goto loc_825D4184;
  // lbz r10,25(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r10,r10,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x20;
  // cmplwi cr6,r10,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 32, so);
  // beq cr6,0x825d4184
  if (cr6.eq) goto loc_825D4184;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // b 0x825d41b0
  goto loc_825D41B0;
loc_825D4184:
  // lbz r10,25(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r10,r10,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x20;
  // cmplwi cr6,r10,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 32, so);
  // beq cr6,0x825d41a0
  if (cr6.eq) goto loc_825D41A0;
  // lbz r10,24(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x825d41dc
  if (!cr6.eq) goto loc_825D41DC;
loc_825D41A0:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d41dc
  if (cr6.eq) goto loc_825D41DC;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,100(r11)
  REX_STORE_U32(uint32_t(r11) + 100, uint32_t(r10));
loc_825D41B0:
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // ori r11,r11,64
  r11 = r11 | 64;
  // stb r11,25(r31)
  REX_STORE_U8(uint32_t(r31) + 25, uint8_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // ori r11,r11,128
  r11 = r11 | 128;
  // stb r11,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r11));
loc_825D41DC:
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

// sub_825D4300: lifted from generated/ by lift.py.
lifted::Result f825D4300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D4300 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r5
  r29 = r5;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x825d4364
  if (cr6.eq) goto loc_825D4364;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x825d4340
  if (!cr6.eq) goto loc_825D4340;
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x825d4364
  if (!cr6.eq) goto loc_825D4364;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x825d4364
  if (!cr6.eq) goto loc_825D4364;
loc_825D4340:
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // beq cr6,0x825d4370
  if (cr6.eq) goto loc_825D4370;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x825d4358
  if (!cr6.eq) goto loc_825D4358;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq cr6,0x825d4370
  if (cr6.eq) goto loc_825D4370;
loc_825D4358:
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d24d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D24D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D4364:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D4370:
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // blt cr6,0x825d43b0
  if (cr6.lt) goto loc_825D43B0;
  // bne cr6,0x825d4364
  if (!cr6.eq) goto loc_825D4364;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d24d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D24D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x825d4364
  if (cr6.eq) goto loc_825D4364;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stb r30,24(r31)
  REX_STORE_U8(uint32_t(r31) + 24, uint8_t(r30));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D43B0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // ori r11,r11,128
  r11 = r11 | 128;
  // stb r11,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r11));
  // bl 0x825d24d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D24D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D43F0: lifted from generated/ by lift.py.
lifted::Result f825D43F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D43F0 (lifted)");
  uint64_t r10 = 0, r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,48(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 48);
  // cntlzw r10,r4
  r10 = uint32_t(r4) == 0 ? 32 : __builtin_clz(uint32_t(r4));
  // rlwinm r10,r10,27,31,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // xori r4,r10,1
  r4 = r10 ^ 1;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D4420: lifted from generated/ by lift.py.
lifted::Result f825D4420(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D4420 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // addi r30,r31,116
  r30 = uint64_t(int64_t(int64_t(r31) + 116));
  // addi r29,r31,100
  r29 = uint64_t(int64_t(int64_t(r31) + 100));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // stw r11,28(r30)
  REX_STORE_U32(uint32_t(r30) + 28, uint32_t(r11));
  // stw r29,36(r30)
  REX_STORE_U32(uint32_t(r30) + 36, uint32_t(r29));
  // bl 0x825cee10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE10, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d4500
  if (cr6.eq) goto loc_825D4500;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825cf288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF288, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // bgt cr6,0x825d4500
  if (cr6.gt) goto loc_825D4500;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // stw r11,20(r30)
  REX_STORE_U32(uint32_t(r30) + 20, uint32_t(r11));
  // bl 0x82379d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379D50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 20);
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,24(r30)
  REX_STORE_U32(uint32_t(r30) + 24, uint32_t(r11));
  // bl 0x825cf288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF288, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,16(r30)
  REX_STORE_U32(uint32_t(r30) + 16, uint32_t(r11));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,-1
  r11 = uint64_t(int64_t(int64_t(r3) + -1));
  // lbz r10,176(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 176);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // rlwimi r10,r11,5,26,26
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0x20) | (r10 & 0xFFFFFFFFFFFFFFDF);
  // stb r10,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r10));
loc_825D4500:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
  // addi r10,r31,92
  r10 = uint64_t(int64_t(int64_t(r31) + 92));
  // stw r30,112(r31)
  REX_STORE_U32(uint32_t(r31) + 112, uint32_t(r30));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // stw r10,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r10));
  // ori r11,r11,64
  r11 = r11 | 64;
  // stb r11,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r11));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D4530: lifted from generated/ by lift.py.
lifted::Result f825D4530(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D4530 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d4568
  if (cr6.eq) goto loc_825D4568;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = f825D5088(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d4568
  if (cr6.eq) goto loc_825D4568;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_825D4568:
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d4584
  if (cr6.eq) goto loc_825D4584;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = f825D5088(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d4584
  if (cr6.eq) goto loc_825D4584;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_825D4584:
  // lwz r3,72(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d45b0
  if (cr6.eq) goto loc_825D45B0;
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // rlwinm r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  // cmplwi cr6,r11,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64, so);
  // beq cr6,0x825d45b0
  if (cr6.eq) goto loc_825D45B0;
  // bl 0x825d5088
  // (left out: dead in C++)
  { const lifted::Result o = f825D5088(base, r1, r13, r3, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bne cr6,0x825d45b4
  if (!cr6.eq) goto loc_825D45B4;
loc_825D45B0:
  // mr r3,r30
  r3 = r30;
loc_825D45B4:
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

// sub_825D45D0: lifted from generated/ by lift.py.
lifted::Result f825D45D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D45D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r4
  r29 = r4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,48(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d4634
  if (cr6.eq) goto loc_825D4634;
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // bne cr6,0x825d4620
  if (!cr6.eq) goto loc_825D4620;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4620
  if (cr6.eq) goto loc_825D4620;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,100(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 100);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D4620:
  // lwz r3,48(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D4634:
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // beq cr6,0x825d4684
  if (cr6.eq) goto loc_825D4684;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r29
  r4 = r29;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4684
  if (cr6.lt) goto loc_825D4684;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x825d4678
  if (cr6.eq) goto loc_825D4678;
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // bl 0x825d1c78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1C78, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D4678:
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // ori r11,r11,2
  r11 = r11 | 2;
  // stb r11,25(r31)
  REX_STORE_U8(uint32_t(r31) + 25, uint8_t(r11));
loc_825D4684:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D4690: lifted from generated/ by lift.py.
lifted::Result f825D4690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D4690 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825d4020
  // (left out: dead in C++)
  { const lifted::Result o = f825D4020(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d46d0
  if (cr6.eq) goto loc_825D46D0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32795
  r4 = r4 | 32795;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D46D0:
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

// sub_825D46E8: lifted from generated/ by lift.py.
lifted::Result f825D46E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D46E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d2778
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D2778, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // mr r28,r27
  r28 = r27;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825d4734
  if (!cr6.eq) goto loc_825D4734;
loc_825D4714:
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825D4734:
  // lbz r11,176(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 176);
  // rlwinm r10,r11,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // bne cr6,0x825d476c
  if (!cr6.eq) goto loc_825D476C;
  // lwz r10,40(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x825d476c
  if (!cr6.eq) goto loc_825D476C;
  // andi. r11,r11,239
  r11 = r11 & 239;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stb r11,176(r31)
  REX_STORE_U8(uint32_t(r31) + 176, uint8_t(r11));
  // bl 0x825d45d0
  // (left out: dead in C++)
  { const lifted::Result o = f825D45D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d4714
  if (cr6.lt) goto loc_825D4714;
loc_825D476C:
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x825d47b0
  if (!cr6.eq) goto loc_825D47B0;
  // lwz r4,44(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 44);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825d4b70
  if (cr6.eq) goto loc_825D4B70;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d1df8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1DF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825D47B0:
  // lbz r10,176(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 176);
  // rlwinm r11,r10,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFF80;
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // beq cr6,0x825d4b70
  if (cr6.eq) goto loc_825D4B70;
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4828
  if (cr6.eq) goto loc_825D4828;
  // rlwinm r10,r10,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40;
  // cmplwi cr6,r10,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 64, so);
  // beq cr6,0x825d4828
  if (cr6.eq) goto loc_825D4828;
  // lwz r10,72(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 72);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x825d4820
  if (!cr6.eq) goto loc_825D4820;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // addi r5,r11,52
  r5 = uint64_t(int64_t(int64_t(r11) + 52));
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d4b70
  if (cr6.eq) goto loc_825D4B70;
  // lwz r4,80(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r3,72(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 72);
  // bl 0x825d5258
  // (left out: dead in C++)
  r3 = f825D5258(base, r3, r4);
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // stw r27,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r27));
loc_825D4820:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d4420
  // (left out: dead in C++)
  { const lifted::Result o = f825D4420(base, r1, r13, r3, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_825D4828:
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // li r29,2
  r29 = uint64_t(int64_t(2));
  // lwz r10,72(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 72);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x825d4954
  if (!cr6.eq) goto loc_825D4954;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // addi r5,r11,52
  r5 = uint64_t(int64_t(int64_t(r11) + 52));
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d4954
  if (cr6.eq) goto loc_825D4954;
  // lwz r4,80(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r3,88(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 88);
  // bl 0x825d5258
  // (left out: dead in C++)
  r3 = f825D5258(base, r3, r4);
  // stw r3,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r3));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d4900
  if (cr6.eq) goto loc_825D4900;
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r10,88(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 88);
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r9,96(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 96);
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
  // stw r9,40(r10)
  REX_STORE_U32(uint32_t(r10) + 40, uint32_t(r9));
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r27,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r27));
  // stw r29,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r29));
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // lwz r10,68(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 68);
  // addi r4,r11,12
  r4 = uint64_t(int64_t(int64_t(r11) + 12));
  // cntlzw r10,r10
  r10 = uint32_t(r10) == 0 ? 32 : __builtin_clz(uint32_t(r10));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // rlwinm r5,r10,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825d48e0
  if (cr6.lt) goto loc_825D48E0;
  // stw r11,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r11));
  // b 0x825d48e4
  goto loc_825D48E4;
loc_825D48E0:
  // stw r27,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r27));
loc_825D48E4:
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x825d4908
  if (!cr6.eq) goto loc_825D4908;
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d24d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D24D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x825d4908
  goto loc_825D4908;
loc_825D4900:
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r27,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r27));
loc_825D4908:
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // lwz r11,88(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 88);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825d492c
  if (!cr6.eq) goto loc_825D492C;
  // lwz r11,68(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4930
  if (cr6.eq) goto loc_825D4930;
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
  // b 0x825d4930
  goto loc_825D4930;
loc_825D492C:
  // stw r10,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r10));
loc_825D4930:
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825d4950
  if (!cr6.eq) goto loc_825D4950;
  // lwz r11,68(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4954
  if (cr6.eq) goto loc_825D4954;
  // stw r11,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r11));
  // b 0x825d4954
  goto loc_825D4954;
loc_825D4950:
  // stw r10,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r10));
loc_825D4954:
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4a18
  if (cr6.eq) goto loc_825D4A18;
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825d4a18
  if (cr6.eq) goto loc_825D4A18;
  // lwz r11,164(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 164);
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x825d4a18
  if (cr6.lt) goto loc_825D4A18;
  // lwz r11,92(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 92);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d4b70
  if (!cr6.eq) goto loc_825D4B70;
  // addi r30,r31,116
  r30 = uint64_t(int64_t(int64_t(r31) + 116));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d49cc
  if (cr6.eq) goto loc_825D49CC;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r29,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r29));
  // mr r4,r30
  r4 = r30;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bge cr6,0x825d49d4
  if (!cr6.lt) goto loc_825D49D4;
  // stw r27,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r27));
loc_825D49CC:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825d4a00
  if (cr6.lt) goto loc_825D4A00;
loc_825D49D4:
  // lwz r10,72(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 72);
  // lwz r11,76(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 76);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825d49ec
  if (!cr6.eq) goto loc_825D49EC;
  // mr r11,r30
  r11 = r30;
  // b 0x825d49f0
  goto loc_825D49F0;
loc_825D49EC:
  // addi r11,r11,12
  r11 = uint64_t(int64_t(int64_t(r11) + 12));
loc_825D49F0:
  // mr r3,r31
  r3 = r31;
  // stw r11,172(r31)
  REX_STORE_U32(uint32_t(r31) + 172, uint32_t(r11));
  // stw r10,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r10));
  // bl 0x825d4128
  // (left out: dead in C++)
  { const lifted::Result o = f825D4128(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_825D4A00:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r10,160(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 160);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825D4A18:
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r10,156(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 156);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825d4a38
  if (cr6.lt) goto loc_825D4A38;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d4128
  // (left out: dead in C++)
  { const lifted::Result o = f825D4128(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825D4A38:
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // lwz r10,72(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 72);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x825d4b70
  if (!cr6.eq) goto loc_825D4B70;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r10));
  // lwz r4,80(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // bl 0x825cf528
  // (left out: dead in C++)
  { const lifted::Result o = f825CF528(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r3,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r3));
  // stw r27,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r27));
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // subf r11,r3,r11
  r11 = r11 - r3;
  // stw r11,88(r10)
  REX_STORE_U32(uint32_t(r10) + 88, uint32_t(r11));
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825d4a9c
  if (cr6.eq) goto loc_825D4A9C;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4a9c
  if (cr6.eq) goto loc_825D4A9C;
  // lwz r11,164(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 164);
  // b 0x825d4aa0
  goto loc_825D4AA0;
loc_825D4A9C:
  // lwz r11,156(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 156);
loc_825D4AA0:
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r9,76(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 76);
  // lwz r10,88(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 88);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x825d4acc
  if (!cr6.gt) goto loc_825D4ACC;
  // mr r11,r10
  r11 = r10;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
loc_825D4ACC:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825d4b3c
  if (cr6.lt) goto loc_825D4B3C;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825d4b10
  if (!cr6.eq) goto loc_825D4B10;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplwi cr6,r11,2048
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2048, so);
  // ble cr6,0x825d4b38
  if (!cr6.gt) goto loc_825D4B38;
  // clrlwi r10,r11,21
  r10 = uint32_t(r11) & 0x7FF;
  // b 0x825d4b34
  goto loc_825D4B34;
loc_825D4B10:
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825ceee8
  // (left out: dead in C++)
  r3 = f825CEEE8(base, r3);
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // ble cr6,0x825d4b38
  if (!cr6.gt) goto loc_825D4B38;
  // divwu r10,r11,r3
  r10 = uint32_t(uint32_t(r3) ? uint32_t(r11) / uint32_t(r3) : 0);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // mullw r10,r10,r3
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r3))));
  // subf r10,r10,r11
  r10 = r11 - r10;
loc_825D4B34:
  // subf r11,r10,r11
  r11 = r11 - r10;
loc_825D4B38:
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
loc_825D4B3C:
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r11,80(r10)
  REX_STORE_U32(uint32_t(r10) + 80, uint32_t(r11));
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r5,84(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 84);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge cr6,0x825d4b70
  if (!cr6.lt) goto loc_825D4B70;
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r27,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r27));
loc_825D4B70:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D4B78: lifted from generated/ by lift.py.
lifted::Result f825D4B78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D4B78 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // stb r4,255(r1)
  REX_STORE_U8(uint32_t(r1) + 255, uint8_t(r4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r31,r3
  r31 = r3;
  // mr r30,r5
  r30 = r5;
  // mr r29,r6
  r29 = r6;
  // bl 0x825d24d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D24D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r30
  r11 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // lbz r9,25(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 25);
  // lbz r10,255(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 255);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // rlwimi r9,r11,5,26,26
  r9 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0x20) | (r9 & 0xFFFFFFFFFFFFFFDF);
  // stb r10,24(r31)
  REX_STORE_U8(uint32_t(r31) + 24, uint8_t(r10));
  // stb r9,25(r31)
  REX_STORE_U8(uint32_t(r31) + 25, uint8_t(r9));
  // bl 0x825ced88
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED88, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r5,36(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // bl 0x825ced98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CED98, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // addi r7,r1,255
  r7 = uint64_t(int64_t(int64_t(r1) + 255));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r5,r29
  r5 = r29;
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r3,r31
  r3 = r31;
  // stw r26,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r26));
  // bl 0x825d2170
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D2170, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // stw r26,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r26));
  // mr r27,r26
  r27 = r26;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4cb8
  if (cr6.lt) goto loc_825D4CB8;
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r4,80(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825cf080
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF080, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82099ba0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099BA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r11,156(r31)
  REX_STORE_U32(uint32_t(r31) + 156, uint32_t(r11));
  // bl 0x825cf210
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF210, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r4,r31,168
  r4 = uint64_t(int64_t(int64_t(r31) + 168));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r11,160(r31)
  REX_STORE_U32(uint32_t(r31) + 160, uint32_t(r11));
  // bl 0x825cf220
  // (left out: dead in C++)
  { const lifted::Result o = f825CF220(base, r1, r13, r3, r4, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,255(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 255);
  // stw r3,164(r31)
  REX_STORE_U32(uint32_t(r31) + 164, uint32_t(r3));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4c74
  if (cr6.eq) goto loc_825D4C74;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825cee10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE10, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825d4c74
  if (!cr6.eq) goto loc_825D4C74;
  // lwz r11,156(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 156);
  // stw r11,164(r31)
  REX_STORE_U32(uint32_t(r31) + 164, uint32_t(r11));
loc_825D4C74:
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // mr r11,r26
  r11 = r26;
  // beq cr6,0x825d4c8c
  if (cr6.eq) goto loc_825D4C8C;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
loc_825D4C8C:
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // clrlwi r7,r11,24
  r7 = uint32_t(r11) & 0xFF;
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // mr r8,r27
  r8 = r27;
  // addi r6,r31,68
  r6 = uint64_t(int64_t(int64_t(r31) + 68));
  // addi r5,r31,64
  r5 = uint64_t(int64_t(int64_t(r31) + 64));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_825D4CB8:
  // mr r28,r26
  r28 = r26;
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4d58
  if (cr6.lt) goto loc_825D4D58;
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // mr r28,r25
  r28 = r25;
  // stw r31,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r31));
  // lwz r11,68(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4ce8
  if (cr6.eq) goto loc_825D4CE8;
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // stw r31,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r31));
loc_825D4CE8:
  // clrlwi r29,r28,24
  r29 = uint32_t(r28) & 0xFF;
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // bne cr6,0x825d4d0c
  if (!cr6.eq) goto loc_825D4D0C;
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825d4d0c
  if (cr6.eq) goto loc_825D4D0C;
  // stb r26,24(r31)
  REX_STORE_U8(uint32_t(r31) + 24, uint8_t(r26));
  // b 0x825d4d58
  goto loc_825D4D58;
loc_825D4D0C:
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825d4d58
  if (cr6.eq) goto loc_825D4D58;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4d58
  if (cr6.eq) goto loc_825D4D58;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // addi r5,r31,72
  r5 = uint64_t(int64_t(int64_t(r31) + 72));
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,76(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4d58
  if (cr6.lt) goto loc_825D4D58;
  // addi r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 1));
  // clrlwi r28,r11,24
  r28 = uint32_t(r11) & 0xFF;
loc_825D4D58:
  // mr r29,r26
  r29 = r26;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4fe8
  if (cr6.lt) goto loc_825D4FE8;
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4d90
  if (cr6.eq) goto loc_825D4D90;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lhz r4,1(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 1);
  // bl 0x825c0708
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0708, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
loc_825D4D90:
  // addi r5,r1,100
  r5 = uint64_t(int64_t(int64_t(r1) + 100));
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r26,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r26));
  // bl 0x825cc320
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC320, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // stw r26,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r26));
  // mr r9,r26
  r9 = r26;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lfs f0,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // beq cr6,0x825d4e00
  if (cr6.eq) goto loc_825D4E00;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r10,28(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lbz r10,0(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 0);
  // rlwinm r10,r10,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x10;
  // cmplwi cr6,r10,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 16, so);
  // bne cr6,0x825d4e00
  if (!cr6.eq) goto loc_825D4E00;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // mr r9,r25
  r9 = r25;
  // lwz r10,124(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 124);
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lfs f0,116(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 116);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
loc_825D4E00:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r10,r1,92
  r10 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r8,r1,84
  r8 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r7,r1,88
  r7 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r6,r29
  r6 = r29;
  // mr r5,r28
  r5 = r28;
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4fe8
  if (cr6.lt) goto loc_825D4FE8;
  // mr r3,r31
  r3 = r31;
  // lwz r4,92(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 92);
  // bl 0x825d1df8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1DF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4e98
  if (cr6.eq) goto loc_825D4E98;
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0268
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0268, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d02b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D02B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D4E98:
  // lwz r11,156(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 156);
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // bge cr6,0x825d4fe0
  if (!cr6.lt) goto loc_825D4FE0;
  // lwz r11,96(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 96);
  // mr r4,r27
  r4 = r27;
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r11));
  // bl 0x825cf528
  // (left out: dead in C++)
  { const lifted::Result o = f825CF528(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // subf r10,r3,r27
  r10 = r27 - r3;
  // stw r3,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r3));
  // stw r26,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r26));
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // stw r10,88(r11)
  REX_STORE_U32(uint32_t(r11) + 88, uint32_t(r10));
  // lbz r11,25(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 25);
  // rlwinm r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // beq cr6,0x825d4f08
  if (cr6.eq) goto loc_825D4F08;
  // lbz r11,24(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d4f08
  if (cr6.eq) goto loc_825D4F08;
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // lwz r8,164(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 164);
  // lwz r11,76(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 76);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x825d4f2c
  if (cr6.lt) goto loc_825D4F2C;
  // mr r11,r8
  r11 = r8;
  // b 0x825d4f2c
  goto loc_825D4F2C;
loc_825D4F08:
  // lwz r11,156(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 156);
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // subf r9,r27,r11
  r9 = r11 - r27;
  // mr r8,r11
  r8 = r11;
  // stw r11,164(r31)
  REX_STORE_U32(uint32_t(r31) + 164, uint32_t(r11));
  // lwz r11,76(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 76);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x825d4f2c
  if (cr6.lt) goto loc_825D4F2C;
  // mr r11,r9
  r11 = r9;
loc_825D4F2C:
  // lwz r10,88(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 88);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // subf r30,r10,r11
  r30 = r11 - r10;
  // blt cr6,0x825d4f40
  if (cr6.lt) goto loc_825D4F40;
  // subf r30,r27,r30
  r30 = r30 - r27;
loc_825D4F40:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825d4f64
  if (!cr6.eq) goto loc_825D4F64;
  // mr r11,r30
  r11 = r30;
  // cmplwi cr6,r30,2048
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 2048, so);
  // ble cr6,0x825d4f8c
  if (!cr6.gt) goto loc_825D4F8C;
  // clrlwi r11,r30,21
  r11 = uint32_t(r30) & 0x7FF;
  // b 0x825d4f88
  goto loc_825D4F88;
loc_825D4F64:
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x825ceee8
  // (left out: dead in C++)
  r3 = f825CEEE8(base, r3);
  // mr r11,r30
  r11 = r30;
  // cmplw cr6,r30,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r3), so);
  // ble cr6,0x825d4f8c
  if (!cr6.gt) goto loc_825D4F8C;
  // divwu r11,r30,r3
  r11 = uint32_t(uint32_t(r3) ? uint32_t(r30) / uint32_t(r3) : 0);
  // twllei r3,0
  if (int32_t(r3) == 0 || uint32_t(r3) < 0u) lifted::Trap(0);
  // mullw r11,r11,r3
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r3))));
  // subf r11,r11,r30
  r11 = r30 - r11;
loc_825D4F88:
  // subf r11,r11,r30
  r11 = r30 - r11;
loc_825D4F8C:
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // stw r11,80(r10)
  REX_STORE_U32(uint32_t(r10) + 80, uint32_t(r11));
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // stw r25,72(r11)
  REX_STORE_U32(uint32_t(r11) + 72, uint32_t(r25));
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r5,64(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 64);
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt cr6,0x825d4fd0
  if (cr6.lt) goto loc_825D4FD0;
  // lwz r11,80(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 80);
  // add r11,r11,r27
  r11 = r11 + r27;
  // stw r11,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r11));
loc_825D4FD0:
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // stw r25,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r25));
  // stw r11,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r11));
  // stw r11,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r11));
loc_825D4FE0:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x825d504c
  if (!cr6.lt) goto loc_825D504C;
loc_825D4FE8:
  // lwz r3,64(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d500c
  if (cr6.eq) goto loc_825D500C;
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
  // stw r26,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r26));
loc_825D500C:
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d5030
  if (cr6.eq) goto loc_825D5030;
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
  // stw r26,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r26));
loc_825D5030:
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d504c
  if (cr6.eq) goto loc_825D504C;
  // lhz r4,52(r31)
  r4 = REX_LOAD_U16(uint32_t(r31) + 52);
  // lwz r3,36(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 36);
  // bl 0x825cfae0
  // (left out: dead in C++)
  { const lifted::Result o = f825CFAE0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // stw r26,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r26));
loc_825D504C:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D5058: lifted from generated/ by lift.py.
lifted::Result f825D5058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5058 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,40(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 40);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // bne cr6,0x825d5074
  if (!cr6.eq) goto loc_825D5074;
  // mr r3,r11
  r3 = r11;
  // b 0x825d45d0
  { const lifted::Result o = f825D45D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825D5074:
  // lbz r10,176(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 176);
  // ori r10,r10,16
  r10 = r10 | 16;
  // stb r10,176(r11)
  REX_STORE_U8(uint32_t(r11) + 176, uint8_t(r10));
  // blr
  return {r3, f1};
}

// sub_825D5088: lifted from generated/ by lift.py.
lifted::Result f825D5088(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5088 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
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
  // mr r11,r3
  r11 = r3;
  // lwz r10,72(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 72);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x825d50d0
  if (!cr6.eq) goto loc_825D50D0;
  // lwz r3,108(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 108);
  // addi r5,r11,52
  r5 = uint64_t(int64_t(int64_t(r11) + 52));
  // lhz r4,112(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 112);
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq cr6,0x825d50d4
  if (cr6.eq) goto loc_825D50D4;
loc_825D50D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825D50D4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D50E8: lifted from generated/ by lift.py.
uint64_t f825D50E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D50E8 (lifted)");
  uint64_t r4 = 0;
  double f1 = 0.0;
  // mr r4,r3
  r4 = r3;
  // lwz r3,108(r4)
  r3 = REX_LOAD_U32(uint32_t(r4) + 108);
  // b 0x825cff70
  f825CFF70(base, r3, r4);
  return r3;
}

// sub_825D50F8: lifted from generated/ by lift.py.
lifted::Result f825D50F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D50F8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r27,r4
  r27 = r4;
  // lis r4,-21628
  r4 = uint64_t(int64_t(-1417412608));
  // mr r29,r3
  r29 = r3;
  // ori r4,r4,3
  r4 = r4 | 3;
  // mr r3,r27
  r3 = r27;
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,84(r29)
  REX_STORE_U32(uint32_t(r29) + 84, uint32_t(r3));
  // beq cr6,0x825d51d4
  if (cr6.eq) goto loc_825D51D4;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // stw r27,76(r29)
  REX_STORE_U32(uint32_t(r29) + 76, uint32_t(r27));
  // addi r31,r29,12
  r31 = uint64_t(int64_t(int64_t(r29) + 12));
  // mr r9,r26
  r9 = r26;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // mr r11,r31
  r11 = r31;
  // mtctr r10
  ctr = r10;
loc_825D514C:
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x825d514c
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D514C;
  // addi r11,r29,92
  r11 = uint64_t(int64_t(int64_t(r29) + 92));
  // stw r26,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r26));
  // clrlwi r28,r28,24
  r28 = uint32_t(r28) & 0xFF;
  // stw r27,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r27));
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // stw r11,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r11));
  // beq cr6,0x825d5248
  if (cr6.eq) goto loc_825D5248;
  // mr r3,r30
  r3 = r30;
  // stw r28,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r28));
  // bl 0x82379d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379D50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // mr r3,r30
  r3 = r30;
  // beq cr6,0x825d51e4
  if (cr6.eq) goto loc_825D51E4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825cf210
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF210, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // bl 0x820a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A8958, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // bl 0x825cf288
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CF288, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
  // bl 0x82379d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82379D50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // add r11,r11,r3
  r11 = r11 + r3;
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // b 0x825d5208
  goto loc_825D5208;
loc_825D51D4:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825D51E4:
  // stw r28,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r28));
  // stw r26,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r26));
  // stw r26,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r26));
  // bl 0x82099ba0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82099BA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
  // bl 0x825cee00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE00, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r3,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r3));
loc_825D5208:
  // mr r3,r30
  r3 = r30;
  // bl 0x825cee28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CEE28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825d5248
  if (!cr6.eq) goto loc_825D5248;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // ble cr6,0x825d522c
  if (!cr6.gt) goto loc_825D522C;
  // clrlwi r10,r11,25
  r10 = uint32_t(r11) & 0x7F;
  // subf r11,r10,r11
  r11 = r11 - r10;
loc_825D522C:
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi cr6,r11,128
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 128, so);
  // ble cr6,0x825d5244
  if (!cr6.gt) goto loc_825D5244;
  // clrlwi r10,r11,25
  r10 = uint32_t(r11) & 0x7F;
  // subf r11,r10,r11
  r11 = r11 - r10;
loc_825D5244:
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
loc_825D5248:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D5258: lifted from generated/ by lift.py.
uint64_t f825D5258(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D5258 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,88(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 88);
  // cmplw cr6,r4,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r11), so);
  // ble cr6,0x825d529c
  if (!cr6.gt) goto loc_825D529C;
  // lwz r10,88(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 88);
  // addi r9,r3,12
  r9 = uint64_t(int64_t(int64_t(r3) + 12));
  // lwz r11,84(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 84);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r11));
  // lwz r10,88(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 88);
  // lwz r11,80(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 80);
  // subf r10,r10,r4
  r10 = r4 - r10;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x825d5290
  if (!cr6.lt) goto loc_825D5290;
  // mr r11,r10
  r11 = r10;
loc_825D5290:
  // stw r11,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r11));
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // blr
  return r3;
loc_825D529C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825D52A8: lifted from generated/ by lift.py.
uint64_t f825D52A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D52A8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r11,r4
  r11 = r4;
  // mr r10,r5
  r10 = r5;
  // addi r9,r9,27348
  r9 = uint64_t(int64_t(int64_t(r9) + 27348));
  // stw r31,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r31));
  // addi r29,r31,72
  r29 = uint64_t(int64_t(int64_t(r31) + 72));
  // stw r31,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r31));
  // li r5,20
  r5 = uint64_t(int64_t(20));
  // stw r30,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r30));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r30,80(r31)
  REX_STORE_U32(uint32_t(r31) + 80, uint32_t(r30));
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // stw r30,84(r31)
  REX_STORE_U32(uint32_t(r31) + 84, uint32_t(r30));
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
  // stw r30,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r30));
  // stw r11,108(r31)
  REX_STORE_U32(uint32_t(r31) + 108, uint32_t(r11));
  // sth r10,112(r31)
  REX_STORE_U16(uint32_t(r31) + 112, uint16_t(r10));
  // stw r30,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r10,r31,12
  r10 = uint64_t(int64_t(int64_t(r31) + 12));
  // mr r8,r30
  r8 = r30;
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // mr r11,r10
  r11 = r10;
  // mtctr r9
  ctr = r9;
loc_825D531C:
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x825d531c
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825D531C;
  // stw r30,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // stw r30,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r30));
  // stw r30,100(r31)
  REX_STORE_U32(uint32_t(r31) + 100, uint32_t(r30));
  // stw r30,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r30));
  // stw r29,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r29));
  // stw r10,104(r31)
  REX_STORE_U32(uint32_t(r31) + 104, uint32_t(r10));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_825D5350: lifted from generated/ by lift.py.
lifted::Result f825D5350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5350 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,27348
  r11 = uint64_t(int64_t(int64_t(r11) + 27348));
  // lwz r3,84(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 84);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d5390
  if (cr6.eq) goto loc_825D5390;
  // lis r4,-21628
  r4 = uint64_t(int64_t(-1417412608));
  // ori r4,r4,3
  r4 = r4 | 3;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_825D5390:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // clrlwi r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d53bc
  if (cr6.eq) goto loc_825D53BC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32804
  r4 = r4 | 32804;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D53BC:
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

// sub_825D5620: lifted from generated/ by lift.py.
lifted::Result f825D5620(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5620 (lifted)");
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
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // mr r31,r4
  r31 = r4;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d565c
  if (cr6.eq) goto loc_825D565C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplw cr6,r3,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r31), so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq cr6,0x825d5660
  if (cr6.eq) goto loc_825D5660;
loc_825D565C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825D5660:
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

// sub_825D56B8: lifted from generated/ by lift.py.
lifted::Result f825D56B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D56B8 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D56D8: lifted from generated/ by lift.py.
lifted::Result f825D56D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D56D8 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,120(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 120);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D56F8: lifted from generated/ by lift.py.
lifted::Result f825D56F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D56F8 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,124(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 124);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D5738: lifted from generated/ by lift.py.
lifted::Result f825D5738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5738 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D5758: lifted from generated/ by lift.py.
lifted::Result f825D5758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5758 (lifted)");
  uint64_t r10 = 0, r11 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,36(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 36);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mr r3,r11
  r3 = r11;
  // lwz r11,84(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 84);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D5E08: lifted from generated/ by lift.py.
void f825D5E08(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D5E08 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,52(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 52);
  // addi r10,r3,48
  r10 = uint64_t(int64_t(int64_t(r3) + 48));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beqlr cr6
  if (cr6.eq) return;
loc_825D5E18:
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r9,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r4), so);
  // beq cr6,0x825d5e34
  if (cr6.eq) goto loc_825D5E34;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825d5e18
  if (!cr6.eq) goto loc_825D5E18;
  // blr
  return;
loc_825D5E34:
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // stw r11,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r11));
  // stw r11,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r11));
  // blr
  return;
}

// sub_825D71B0: lifted from generated/ by lift.py.
uint64_t f825D71B0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D71B0 (lifted)");
  uint64_t r3 = 0;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,6
  r3 = r3 | 6;
  // blr
  return r3;
}

// sub_825D72C0: lifted from generated/ by lift.py.
lifted::Result f825D72C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D72C0 (lifted)");
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27480
  r11 = uint64_t(int64_t(int64_t(r11) + 27480));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d730c
  if (cr6.eq) goto loc_825D730C;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32867
  r4 = r4 | 32867;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D730C:
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

// sub_825D7418: lifted from generated/ by lift.py.
lifted::Result f825D7418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7418 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7468
  if (cr6.eq) goto loc_825D7468;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x825d1db0
  // (left out: dead in C++)
  { const lifted::Result o = f825D1DB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D7468:
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

// sub_825D7480: lifted from generated/ by lift.py.
lifted::Result f825D7480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7480 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // mr r28,r6
  r28 = r6;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d74e4
  if (cr6.eq) goto loc_825D74E4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r6,r28
  r6 = r28;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // bl 0x825d1dd0
  // (left out: dead in C++)
  { const lifted::Result o = f825D1DD0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825D74E4:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D74F0: lifted from generated/ by lift.py.
lifted::Result f825D74F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D74F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7540
  if (cr6.eq) goto loc_825D7540;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825d1d30
  // (left out: dead in C++)
  { const lifted::Result o = f825D1D30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D7540:
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

// sub_825D8048: lifted from generated/ by lift.py.
lifted::Result f825D8048(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D8048 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // mr r31,r3
  r31 = r3;
  // fmr f31,f1
  f31 = f1;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d8098
  if (cr6.eq) goto loc_825D8098;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x825d1ce8
  // (left out: dead in C++)
  { const lifted::Result o = f825D1CE8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D8098:
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

// sub_825D80B0: lifted from generated/ by lift.py.
lifted::Result f825D80B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D80B0 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d8100
  if (cr6.eq) goto loc_825D8100;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x825d1d50
  // (left out: dead in C++)
  { const lifted::Result o = f825D1D50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D8100:
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

// sub_825D8118: lifted from generated/ by lift.py.
lifted::Result f825D8118(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D8118 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d8168
  if (cr6.eq) goto loc_825D8168;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x825d1d70
  // (left out: dead in C++)
  { const lifted::Result o = f825D1D70(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D8168:
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

// sub_825D8180: lifted from generated/ by lift.py.
lifted::Result f825D8180(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D8180 (lifted)");
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
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d81d0
  if (cr6.eq) goto loc_825D81D0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // bl 0x825d1d90
  // (left out: dead in C++)
  { const lifted::Result o = f825D1D90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D81D0:
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

// sub_825D8570: lifted from generated/ by lift.py.
lifted::Result f825D8570(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D8570 (lifted)");
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
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d85b0
  if (cr6.eq) goto loc_825D85B0;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x825cadf0
  // (left out: dead in C++)
  { const lifted::Result o = f825CADF0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_825D85B0:
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

// sub_825D85C8: lifted from generated/ by lift.py.
lifted::Result f825D85C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D85C8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // lwz r29,20(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 20);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // add r11,r29,r3
  r11 = r29 + r3;
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // bge cr6,0x825d8650
  if (!cr6.lt) goto loc_825D8650;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r29,20(r31)
  r29 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // subf r11,r30,r29
  r11 = r29 - r30;
  // add r3,r11,r3
  r3 = r11 + r3;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D8650:
  // subf r3,r30,r11
  r3 = r11 - r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D8F18: lifted from generated/ by lift.py.
uint64_t f825D8F18(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D8F18 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,32767
  r10 = uint64_t(int64_t(2147418112));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // ori r9,r10,65517
  r9 = r10 | 65517;
  // lis r10,32767
  r10 = uint64_t(int64_t(2147418112));
  // mullw r11,r11,r9
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r9))));
  // ori r10,r10,65475
  r10 = r10 | 65475;
  // add r8,r11,r10
  r8 = r11 + r10;
  // lis r11,32767
  r11 = uint64_t(int64_t(2147418112));
  // mr r6,r10
  r6 = r10;
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // divwu r7,r8,r11
  r7 = uint32_t(uint32_t(r11) ? uint32_t(r8) / uint32_t(r11) : 0);
  // mullw r10,r7,r11
  r10 = uint64_t(int64_t(int64_t(int32_t(r7)) * int64_t(int32_t(r11))));
  // subf r10,r10,r8
  r10 = r8 - r10;
  // mr r8,r11
  r8 = r11;
  // mr r7,r11
  r7 = r11;
  // mullw r9,r10,r9
  r9 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // stw r10,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r10));
  // add r9,r9,r6
  r9 = r9 + r6;
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // divwu r8,r9,r8
  r8 = uint32_t(uint32_t(r8) ? uint32_t(r9) / uint32_t(r8) : 0);
  // addi r11,r11,-12800
  r11 = uint64_t(int64_t(int64_t(r11) + -12800));
  // mullw r8,r8,r7
  r8 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r7))));
  // subf r9,r8,r9
  r9 = r9 - r8;
  // stw r9,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r9));
  // rlwinm r9,r9,2,23,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x1FC;
  // lwzx r3,r9,r11
  r3 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // stwx r10,r9,r11
  REX_STORE_U32(uint32_t(r9) + uint32_t(r11), uint32_t(r10));
  // blr
  return r3;
}

// sub_825D9140: lifted from generated/ by lift.py.
lifted::Result f825D9140(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9140 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21336
  r3 = uint64_t(int64_t(int64_t(r11) + -21336));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9150: lifted from generated/ by lift.py.
lifted::Result f825D9150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9150 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21240
  r3 = uint64_t(int64_t(int64_t(r11) + -21240));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9200: lifted from generated/ by lift.py.
lifted::Result f825D9200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D9200 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
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
  // addi r3,r11,-21792
  r3 = uint64_t(int64_t(int64_t(r11) + -21792));
  // bl 0x825925f0
  // (left out: dead in C++)
  { const lifted::Result o = f825925F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21304
  r3 = uint64_t(int64_t(int64_t(r11) + -21304));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D9238: lifted from generated/ by lift.py.
lifted::Result f825D9238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D9238 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
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
  // addi r3,r11,-21872
  r3 = uint64_t(int64_t(int64_t(r11) + -21872));
  // bl 0x825925f0
  // (left out: dead in C++)
  { const lifted::Result o = f825925F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21288
  r3 = uint64_t(int64_t(int64_t(r11) + -21288));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D9270: lifted from generated/ by lift.py.
lifted::Result f825D9270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D9270 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
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
  // addi r3,r11,-21832
  r3 = uint64_t(int64_t(int64_t(r11) + -21832));
  // bl 0x825925f0
  // (left out: dead in C++)
  { const lifted::Result o = f825925F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21272
  r3 = uint64_t(int64_t(int64_t(r11) + -21272));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D92A8: lifted from generated/ by lift.py.
lifted::Result f825D92A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D92A8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
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
  // addi r3,r11,-21912
  r3 = uint64_t(int64_t(int64_t(r11) + -21912));
  // bl 0x825925f0
  // (left out: dead in C++)
  { const lifted::Result o = f825925F0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21256
  r3 = uint64_t(int64_t(int64_t(r11) + -21256));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D92E0: lifted from generated/ by lift.py.
lifted::Result f825D92E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D92E0 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r11,-21632
  r10 = uint64_t(int64_t(int64_t(r11) + -21632));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
loc_825D92F0:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bge 0x825d92f0
  if (!cr0.lt) goto loc_825D92F0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21104
  r3 = uint64_t(int64_t(int64_t(r11) + -21104));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9310: lifted from generated/ by lift.py.
lifted::Result f825D9310(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_825D9310 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f31 = 0.0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // li r30,20
  r30 = uint64_t(int64_t(20));
  // addi r31,r11,428
  r31 = uint64_t(int64_t(int64_t(r11) + 428));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f31 = double(temp.f32);
  // addi r28,r11,7872
  r28 = uint64_t(int64_t(int64_t(r11) + 7872));
loc_825D9340:
  // addi r29,r31,-428
  r29 = uint64_t(int64_t(int64_t(r31) + -428));
  // mr r3,r29
  r3 = r29;
  // bl 0x82593908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82593908, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r28));
  // stfs f31,-16(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + -16, temp.u32);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stfs f31,0(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 0, temp.u32);
  // stfs f31,16(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 16, temp.u32);
  // stfs f31,48(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 48, temp.u32);
  // stfs f31,64(r31)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r31) + 64, temp.u32);
  // addi r31,r31,592
  r31 = uint64_t(int64_t(int64_t(r31) + 592));
  // bge 0x825d9340
  if (!cr0.lt) goto loc_825D9340;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-21096
  r3 = uint64_t(int64_t(int64_t(r11) + -21096));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-48(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D9388: lifted from generated/ by lift.py.
lifted::Result f825D9388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9388 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20976
  r3 = uint64_t(int64_t(int64_t(r11) + -20976));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9398: lifted from generated/ by lift.py.
lifted::Result f825D9398(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9398 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20968
  r3 = uint64_t(int64_t(int64_t(r11) + -20968));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D93A8: lifted from generated/ by lift.py.
lifted::Result f825D93A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D93A8 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,20752
  r11 = uint64_t(int64_t(int64_t(r11) + 20752));
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // addi r11,r11,28
  r11 = uint64_t(int64_t(int64_t(r11) + 28));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_825D93C0:
  // stfs f0,-16(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + -16, temp.u32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // bge 0x825d93c0
  if (!cr0.lt) goto loc_825D93C0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20960
  r3 = uint64_t(int64_t(int64_t(r11) + -20960));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D93E8: lifted from generated/ by lift.py.
lifted::Result f825D93E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D93E8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20952
  r3 = uint64_t(int64_t(int64_t(r11) + -20952));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D93F8: lifted from generated/ by lift.py.
lifted::Result f825D93F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D93F8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20856
  r3 = uint64_t(int64_t(int64_t(r11) + -20856));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9408: lifted from generated/ by lift.py.
lifted::Result f825D9408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9408 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20848
  r3 = uint64_t(int64_t(int64_t(r11) + -20848));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9418: lifted from generated/ by lift.py.
lifted::Result f825D9418(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9418 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20840
  r3 = uint64_t(int64_t(int64_t(r11) + -20840));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9428: lifted from generated/ by lift.py.
lifted::Result f825D9428(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9428 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20832
  r3 = uint64_t(int64_t(int64_t(r11) + -20832));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9438: lifted from generated/ by lift.py.
lifted::Result f825D9438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9438 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20824
  r3 = uint64_t(int64_t(int64_t(r11) + -20824));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9448: lifted from generated/ by lift.py.
lifted::Result f825D9448(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9448 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20816
  r3 = uint64_t(int64_t(int64_t(r11) + -20816));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9458: lifted from generated/ by lift.py.
lifted::Result f825D9458(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9458 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20808
  r3 = uint64_t(int64_t(int64_t(r11) + -20808));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9468: lifted from generated/ by lift.py.
lifted::Result f825D9468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9468 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20800
  r3 = uint64_t(int64_t(int64_t(r11) + -20800));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9478: lifted from generated/ by lift.py.
lifted::Result f825D9478(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9478 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20792
  r3 = uint64_t(int64_t(int64_t(r11) + -20792));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9488: lifted from generated/ by lift.py.
lifted::Result f825D9488(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9488 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20784
  r3 = uint64_t(int64_t(int64_t(r11) + -20784));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9498: lifted from generated/ by lift.py.
lifted::Result f825D9498(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9498 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20776
  r3 = uint64_t(int64_t(int64_t(r11) + -20776));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D94A8: lifted from generated/ by lift.py.
lifted::Result f825D94A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D94A8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20768
  r3 = uint64_t(int64_t(int64_t(r11) + -20768));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D94B8: lifted from generated/ by lift.py.
lifted::Result f825D94B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D94B8 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20760
  r3 = uint64_t(int64_t(int64_t(r11) + -20760));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D94F0: lifted from generated/ by lift.py.
void f825D94F0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D94F0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lfs f0,13036(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 13036);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,27076(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 27076);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23456(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23456, temp.u32);
  // blr
  return;
}

// sub_825D9668: lifted from generated/ by lift.py.
lifted::Result f825D9668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9668 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20720
  r3 = uint64_t(int64_t(int64_t(r11) + -20720));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9678: lifted from generated/ by lift.py.
lifted::Result f825D9678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9678 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,23756
  r31 = uint64_t(int64_t(int64_t(r11) + 23756));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211b0e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B0E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32162
  r10 = uint64_t(int64_t(-2107768832));
  // addi r3,r10,-20712
  r3 = uint64_t(int64_t(int64_t(r10) + -20712));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // bl 0x825a7068
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_825D96C0: lifted from generated/ by lift.py.
void f825D96C0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D96C0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,27824(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 27824);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31684(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31684);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23776(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23776, temp.u32);
  // blr
  return;
}

// sub_825D96E8: lifted from generated/ by lift.py.
void f825D96E8(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825D96E8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32159
  r11 = uint64_t(int64_t(-2107572224));
  // lfs f0,28068(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28068);
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // lfs f0,-31636(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -31636);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,23828(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 23828, temp.u32);
  // blr
  return;
}

// sub_825D9710: lifted from generated/ by lift.py.
lifted::Result f825D9710(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9710 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20696
  r3 = uint64_t(int64_t(int64_t(r11) + -20696));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9720: lifted from generated/ by lift.py.
lifted::Result f825D9720(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9720 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20688
  r3 = uint64_t(int64_t(int64_t(r11) + -20688));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9730: lifted from generated/ by lift.py.
lifted::Result f825D9730(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9730 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20680
  r3 = uint64_t(int64_t(int64_t(r11) + -20680));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9740: lifted from generated/ by lift.py.
lifted::Result f825D9740(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9740 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20672
  r3 = uint64_t(int64_t(int64_t(r11) + -20672));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9750: lifted from generated/ by lift.py.
lifted::Result f825D9750(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9750 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20664
  r3 = uint64_t(int64_t(int64_t(r11) + -20664));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9760: lifted from generated/ by lift.py.
lifted::Result f825D9760(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9760 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20656
  r3 = uint64_t(int64_t(int64_t(r11) + -20656));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9770: lifted from generated/ by lift.py.
lifted::Result f825D9770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9770 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20648
  r3 = uint64_t(int64_t(int64_t(r11) + -20648));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9780: lifted from generated/ by lift.py.
lifted::Result f825D9780(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9780 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20640
  r3 = uint64_t(int64_t(int64_t(r11) + -20640));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D9790: lifted from generated/ by lift.py.
lifted::Result f825D9790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D9790 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32162
  r11 = uint64_t(int64_t(-2107768832));
  // addi r3,r11,-20632
  r3 = uint64_t(int64_t(int64_t(r11) + -20632));
  // b 0x825a7068
  { const lifted::Result o = lifted::Call(base, 0x825A7068, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

namespace {

lifted::Result e825D1D70(uint8_t* base, const lifted::Args& a) {
  return f825D1D70(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D1D90(uint8_t* base, const lifted::Args& a) {
  return f825D1D90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D1DB0(uint8_t* base, const lifted::Args& a) {
  return f825D1DB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D1DD0(uint8_t* base, const lifted::Args& a) {
  return f825D1DD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D2300(uint8_t* base, const lifted::Args& a) {
  return f825D2300(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D2380(uint8_t* base, const lifted::Args& a) {
  return f825D2380(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D24B0(uint8_t* base, const lifted::Args& a) {
  return f825D24B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D2A48(uint8_t* base, const lifted::Args& a) {
  return f825D2A48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D2AE8(uint8_t* base, const lifted::Args& a) {
  return f825D2AE8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D2B80(uint8_t* base, const lifted::Args& a) {
  return f825D2B80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D2D18(uint8_t* base, const lifted::Args& a) {
  return f825D2D18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D2D68(uint8_t* base, const lifted::Args& a) {
  return f825D2D68(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D2EA0(uint8_t* base, const lifted::Args& a) {
  return {f825D2EA0(base, a.r1, a.r[0], a.r[1], a.f[0]), a.f[0]};
}

lifted::Result e825D2EF0(uint8_t* base, const lifted::Args& a) {
  return f825D2EF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D2F90(uint8_t* base, const lifted::Args& a) {
  return f825D2F90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D3030(uint8_t* base, const lifted::Args& a) {
  return f825D3030(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D30B0(uint8_t* base, const lifted::Args& a) {
  return {f825D30B0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D3178(uint8_t* base, const lifted::Args& a) {
  return {f825D3178(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D3338(uint8_t* base, const lifted::Args& a) {
  return f825D3338(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D3360(uint8_t* base, const lifted::Args& a) {
  f825D3360(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D3428(uint8_t* base, const lifted::Args& a) {
  return {f825D3428(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D3618(uint8_t* base, const lifted::Args& a) {
  return {f825D3618(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825D3688(uint8_t* base, const lifted::Args& a) {
  f825D3688(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D36A0(uint8_t* base, const lifted::Args& a) {
  return f825D36A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D3F28(uint8_t* base, const lifted::Args& a) {
  return f825D3F28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
}

lifted::Result e825D4020(uint8_t* base, const lifted::Args& a) {
  return f825D4020(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D4128(uint8_t* base, const lifted::Args& a) {
  return f825D4128(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D4300(uint8_t* base, const lifted::Args& a) {
  return f825D4300(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D43F0(uint8_t* base, const lifted::Args& a) {
  return f825D43F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D4420(uint8_t* base, const lifted::Args& a) {
  return f825D4420(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D4530(uint8_t* base, const lifted::Args& a) {
  return f825D4530(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D45D0(uint8_t* base, const lifted::Args& a) {
  return f825D45D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D4690(uint8_t* base, const lifted::Args& a) {
  return f825D4690(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D46E8(uint8_t* base, const lifted::Args& a) {
  return f825D46E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D4B78(uint8_t* base, const lifted::Args& a) {
  return f825D4B78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D5058(uint8_t* base, const lifted::Args& a) {
  return f825D5058(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D5088(uint8_t* base, const lifted::Args& a) {
  return f825D5088(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D50E8(uint8_t* base, const lifted::Args& a) {
  return {f825D50E8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D50F8(uint8_t* base, const lifted::Args& a) {
  return f825D50F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D5258(uint8_t* base, const lifted::Args& a) {
  return {f825D5258(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825D52A8(uint8_t* base, const lifted::Args& a) {
  return {f825D52A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825D5350(uint8_t* base, const lifted::Args& a) {
  return f825D5350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D5620(uint8_t* base, const lifted::Args& a) {
  return f825D5620(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D56B8(uint8_t* base, const lifted::Args& a) {
  return f825D56B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D56D8(uint8_t* base, const lifted::Args& a) {
  return f825D56D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D56F8(uint8_t* base, const lifted::Args& a) {
  return f825D56F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D5738(uint8_t* base, const lifted::Args& a) {
  return f825D5738(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D5758(uint8_t* base, const lifted::Args& a) {
  return f825D5758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D5E08(uint8_t* base, const lifted::Args& a) {
  f825D5E08(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D71B0(uint8_t* base, const lifted::Args& a) {
  return {f825D71B0(base), a.f[0]};
}

lifted::Result e825D72C0(uint8_t* base, const lifted::Args& a) {
  return f825D72C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D7418(uint8_t* base, const lifted::Args& a) {
  return f825D7418(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7480(uint8_t* base, const lifted::Args& a) {
  return f825D7480(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D74F0(uint8_t* base, const lifted::Args& a) {
  return f825D74F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D8048(uint8_t* base, const lifted::Args& a) {
  return f825D8048(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D80B0(uint8_t* base, const lifted::Args& a) {
  return f825D80B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D8118(uint8_t* base, const lifted::Args& a) {
  return f825D8118(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D8180(uint8_t* base, const lifted::Args& a) {
  return f825D8180(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D8570(uint8_t* base, const lifted::Args& a) {
  return f825D8570(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D85C8(uint8_t* base, const lifted::Args& a) {
  return f825D85C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D8F18(uint8_t* base, const lifted::Args& a) {
  return {f825D8F18(base, a.r[0]), a.f[0]};
}

lifted::Result e825D9140(uint8_t* base, const lifted::Args& a) {
  return f825D9140(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9150(uint8_t* base, const lifted::Args& a) {
  return f825D9150(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9200(uint8_t* base, const lifted::Args& a) {
  return f825D9200(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D9238(uint8_t* base, const lifted::Args& a) {
  return f825D9238(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D9270(uint8_t* base, const lifted::Args& a) {
  return f825D9270(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D92A8(uint8_t* base, const lifted::Args& a) {
  return f825D92A8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D92E0(uint8_t* base, const lifted::Args& a) {
  return f825D92E0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D9310(uint8_t* base, const lifted::Args& a) {
  return f825D9310(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825D9388(uint8_t* base, const lifted::Args& a) {
  return f825D9388(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9398(uint8_t* base, const lifted::Args& a) {
  return f825D9398(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D93A8(uint8_t* base, const lifted::Args& a) {
  return f825D93A8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D93E8(uint8_t* base, const lifted::Args& a) {
  return f825D93E8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D93F8(uint8_t* base, const lifted::Args& a) {
  return f825D93F8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9408(uint8_t* base, const lifted::Args& a) {
  return f825D9408(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9418(uint8_t* base, const lifted::Args& a) {
  return f825D9418(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9428(uint8_t* base, const lifted::Args& a) {
  return f825D9428(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9438(uint8_t* base, const lifted::Args& a) {
  return f825D9438(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9448(uint8_t* base, const lifted::Args& a) {
  return f825D9448(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9458(uint8_t* base, const lifted::Args& a) {
  return f825D9458(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9468(uint8_t* base, const lifted::Args& a) {
  return f825D9468(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9478(uint8_t* base, const lifted::Args& a) {
  return f825D9478(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9488(uint8_t* base, const lifted::Args& a) {
  return f825D9488(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9498(uint8_t* base, const lifted::Args& a) {
  return f825D9498(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D94A8(uint8_t* base, const lifted::Args& a) {
  return f825D94A8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D94B8(uint8_t* base, const lifted::Args& a) {
  return f825D94B8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D94F0(uint8_t* base, const lifted::Args& a) {
  f825D94F0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9668(uint8_t* base, const lifted::Args& a) {
  return f825D9668(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9678(uint8_t* base, const lifted::Args& a) {
  return f825D9678(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D96C0(uint8_t* base, const lifted::Args& a) {
  f825D96C0(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D96E8(uint8_t* base, const lifted::Args& a) {
  f825D96E8(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D9710(uint8_t* base, const lifted::Args& a) {
  return f825D9710(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9720(uint8_t* base, const lifted::Args& a) {
  return f825D9720(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9730(uint8_t* base, const lifted::Args& a) {
  return f825D9730(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9740(uint8_t* base, const lifted::Args& a) {
  return f825D9740(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9750(uint8_t* base, const lifted::Args& a) {
  return f825D9750(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9760(uint8_t* base, const lifted::Args& a) {
  return f825D9760(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9770(uint8_t* base, const lifted::Args& a) {
  return f825D9770(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9780(uint8_t* base, const lifted::Args& a) {
  return f825D9780(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D9790(uint8_t* base, const lifted::Args& a) {
  return f825D9790(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

const Registration kEntries[] = {
    {0x825D1D70, e825D1D70},
    {0x825D1D90, e825D1D90},
    {0x825D1DB0, e825D1DB0},
    {0x825D1DD0, e825D1DD0},
    {0x825D2300, e825D2300},
    {0x825D2380, e825D2380},
    {0x825D24B0, e825D24B0},
    {0x825D2A48, e825D2A48},
    {0x825D2AE8, e825D2AE8},
    {0x825D2B80, e825D2B80},
    {0x825D2D18, e825D2D18},
    {0x825D2D68, e825D2D68},
    {0x825D2EA0, e825D2EA0},
    {0x825D2EF0, e825D2EF0},
    {0x825D2F90, e825D2F90},
    {0x825D3030, e825D3030},
    {0x825D30B0, e825D30B0},
    {0x825D3178, e825D3178},
    {0x825D3338, e825D3338},
    {0x825D3360, e825D3360},
    {0x825D3428, e825D3428},
    {0x825D3618, e825D3618},
    {0x825D3688, e825D3688},
    {0x825D36A0, e825D36A0},
    {0x825D3F28, e825D3F28},
    {0x825D4020, e825D4020},
    {0x825D4128, e825D4128},
    {0x825D4300, e825D4300},
    {0x825D43F0, e825D43F0},
    {0x825D4420, e825D4420},
    {0x825D4530, e825D4530},
    {0x825D45D0, e825D45D0},
    {0x825D4690, e825D4690},
    {0x825D46E8, e825D46E8},
    {0x825D4B78, e825D4B78},
    {0x825D5058, e825D5058},
    {0x825D5088, e825D5088},
    {0x825D50E8, e825D50E8},
    {0x825D50F8, e825D50F8},
    {0x825D5258, e825D5258},
    {0x825D52A8, e825D52A8},
    {0x825D5350, e825D5350},
    {0x825D5620, e825D5620},
    {0x825D56B8, e825D56B8},
    {0x825D56D8, e825D56D8},
    {0x825D56F8, e825D56F8},
    {0x825D5738, e825D5738},
    {0x825D5758, e825D5758},
    {0x825D5E08, e825D5E08},
    {0x825D71B0, e825D71B0},
    {0x825D72C0, e825D72C0},
    {0x825D7418, e825D7418},
    {0x825D7480, e825D7480},
    {0x825D74F0, e825D74F0},
    {0x825D8048, e825D8048},
    {0x825D80B0, e825D80B0},
    {0x825D8118, e825D8118},
    {0x825D8180, e825D8180},
    {0x825D8570, e825D8570},
    {0x825D85C8, e825D85C8},
    {0x825D8F18, e825D8F18},
    {0x825D9140, e825D9140},
    {0x825D9150, e825D9150},
    {0x825D9200, e825D9200},
    {0x825D9238, e825D9238},
    {0x825D9270, e825D9270},
    {0x825D92A8, e825D92A8},
    {0x825D92E0, e825D92E0},
    {0x825D9310, e825D9310},
    {0x825D9388, e825D9388},
    {0x825D9398, e825D9398},
    {0x825D93A8, e825D93A8},
    {0x825D93E8, e825D93E8},
    {0x825D93F8, e825D93F8},
    {0x825D9408, e825D9408},
    {0x825D9418, e825D9418},
    {0x825D9428, e825D9428},
    {0x825D9438, e825D9438},
    {0x825D9448, e825D9448},
    {0x825D9458, e825D9458},
    {0x825D9468, e825D9468},
    {0x825D9478, e825D9478},
    {0x825D9488, e825D9488},
    {0x825D9498, e825D9498},
    {0x825D94A8, e825D94A8},
    {0x825D94B8, e825D94B8},
    {0x825D94F0, e825D94F0},
    {0x825D9668, e825D9668},
    {0x825D9678, e825D9678},
    {0x825D96C0, e825D96C0},
    {0x825D96E8, e825D96E8},
    {0x825D9710, e825D9710},
    {0x825D9720, e825D9720},
    {0x825D9730, e825D9730},
    {0x825D9740, e825D9740},
    {0x825D9750, e825D9750},
    {0x825D9760, e825D9760},
    {0x825D9770, e825D9770},
    {0x825D9780, e825D9780},
    {0x825D9790, e825D9790},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_025)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_825D1D70) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D1D70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D1D70);
}

extern "C" REX_FUNC(sub_825D1D90) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D1D90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D1D90);
}

extern "C" REX_FUNC(sub_825D1DB0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D1DB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D1DB0);
}

extern "C" REX_FUNC(sub_825D1DD0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D1DD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D1DD0);
}

extern "C" REX_FUNC(sub_825D2300) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2300(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2300);
}

extern "C" REX_FUNC(sub_825D2380) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2380(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2380);
}

extern "C" REX_FUNC(sub_825D24B0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D24B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D24B0);
}

extern "C" REX_FUNC(sub_825D2A48) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2A48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2A48);
}

extern "C" REX_FUNC(sub_825D2AE8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2AE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2AE8);
}

extern "C" REX_FUNC(sub_825D2B80) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2B80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2B80);
}

extern "C" REX_FUNC(sub_825D2D18) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2D18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2D18);
}

extern "C" REX_FUNC(sub_825D2D68) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2D68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2D68);
}

extern "C" REX_FUNC(sub_825D2EA0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2EA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2EA0);
}

extern "C" REX_FUNC(sub_825D2EF0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2EF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2EF0);
}

extern "C" REX_FUNC(sub_825D2F90) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D2F90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D2F90);
}

extern "C" REX_FUNC(sub_825D3030) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3030(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3030);
}

extern "C" REX_FUNC(sub_825D30B0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D30B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D30B0);
}

extern "C" REX_FUNC(sub_825D3178) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3178);
}

extern "C" REX_FUNC(sub_825D3338) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3338(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3338);
}

extern "C" REX_FUNC(sub_825D3360) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3360(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3360);
}

extern "C" REX_FUNC(sub_825D3428) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3428(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3428);
}

extern "C" REX_FUNC(sub_825D3618) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3618(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3618);
}

extern "C" REX_FUNC(sub_825D3688) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3688(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3688);
}

extern "C" REX_FUNC(sub_825D36A0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D36A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D36A0);
}

extern "C" REX_FUNC(sub_825D3F28) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D3F28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D3F28);
}

extern "C" REX_FUNC(sub_825D4020) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4020(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4020);
}

extern "C" REX_FUNC(sub_825D4128) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4128(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4128);
}

extern "C" REX_FUNC(sub_825D4300) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4300(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4300);
}

extern "C" REX_FUNC(sub_825D43F0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D43F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D43F0);
}

extern "C" REX_FUNC(sub_825D4420) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4420(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4420);
}

extern "C" REX_FUNC(sub_825D4530) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4530(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4530);
}

extern "C" REX_FUNC(sub_825D45D0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D45D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D45D0);
}

extern "C" REX_FUNC(sub_825D4690) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4690(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4690);
}

extern "C" REX_FUNC(sub_825D46E8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D46E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D46E8);
}

extern "C" REX_FUNC(sub_825D4B78) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D4B78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D4B78);
}

extern "C" REX_FUNC(sub_825D5058) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5058(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5058);
}

extern "C" REX_FUNC(sub_825D5088) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5088(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5088);
}

extern "C" REX_FUNC(sub_825D50E8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D50E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D50E8);
}

extern "C" REX_FUNC(sub_825D50F8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D50F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D50F8);
}

extern "C" REX_FUNC(sub_825D5258) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5258(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5258);
}

extern "C" REX_FUNC(sub_825D52A8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D52A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D52A8);
}

extern "C" REX_FUNC(sub_825D5350) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5350(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5350);
}

extern "C" REX_FUNC(sub_825D5620) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5620(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5620);
}

extern "C" REX_FUNC(sub_825D56B8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D56B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D56B8);
}

extern "C" REX_FUNC(sub_825D56D8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D56D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D56D8);
}

extern "C" REX_FUNC(sub_825D56F8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D56F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D56F8);
}

extern "C" REX_FUNC(sub_825D5738) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5738(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5738);
}

extern "C" REX_FUNC(sub_825D5758) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5758(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5758);
}

extern "C" REX_FUNC(sub_825D5E08) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D5E08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D5E08);
}

extern "C" REX_FUNC(sub_825D71B0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D71B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D71B0);
}

extern "C" REX_FUNC(sub_825D72C0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D72C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D72C0);
}

extern "C" REX_FUNC(sub_825D7418) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D7418(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D7418);
}

extern "C" REX_FUNC(sub_825D7480) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D7480(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D7480);
}

extern "C" REX_FUNC(sub_825D74F0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D74F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D74F0);
}

extern "C" REX_FUNC(sub_825D8048) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D8048(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D8048);
}

extern "C" REX_FUNC(sub_825D80B0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D80B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D80B0);
}

extern "C" REX_FUNC(sub_825D8118) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D8118(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D8118);
}

extern "C" REX_FUNC(sub_825D8180) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D8180(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D8180);
}

extern "C" REX_FUNC(sub_825D8570) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D8570(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D8570);
}

extern "C" REX_FUNC(sub_825D85C8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D85C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D85C8);
}

extern "C" REX_FUNC(sub_825D8F18) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D8F18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D8F18);
}

extern "C" REX_FUNC(sub_825D9140) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9140(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9140);
}

extern "C" REX_FUNC(sub_825D9150) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9150(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9150);
}

extern "C" REX_FUNC(sub_825D9200) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9200(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9200);
}

extern "C" REX_FUNC(sub_825D9238) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9238);
}

extern "C" REX_FUNC(sub_825D9270) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9270);
}

extern "C" REX_FUNC(sub_825D92A8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D92A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D92A8);
}

extern "C" REX_FUNC(sub_825D92E0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D92E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D92E0);
}

extern "C" REX_FUNC(sub_825D9310) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9310(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9310);
}

extern "C" REX_FUNC(sub_825D9388) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9388(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9388);
}

extern "C" REX_FUNC(sub_825D9398) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9398(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9398);
}

extern "C" REX_FUNC(sub_825D93A8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D93A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D93A8);
}

extern "C" REX_FUNC(sub_825D93E8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D93E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D93E8);
}

extern "C" REX_FUNC(sub_825D93F8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D93F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D93F8);
}

extern "C" REX_FUNC(sub_825D9408) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9408(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9408);
}

extern "C" REX_FUNC(sub_825D9418) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9418(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9418);
}

extern "C" REX_FUNC(sub_825D9428) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9428(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9428);
}

extern "C" REX_FUNC(sub_825D9438) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9438);
}

extern "C" REX_FUNC(sub_825D9448) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9448(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9448);
}

extern "C" REX_FUNC(sub_825D9458) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9458(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9458);
}

extern "C" REX_FUNC(sub_825D9468) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9468(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9468);
}

extern "C" REX_FUNC(sub_825D9478) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9478(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9478);
}

extern "C" REX_FUNC(sub_825D9488) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9488(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9488);
}

extern "C" REX_FUNC(sub_825D9498) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9498(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9498);
}

extern "C" REX_FUNC(sub_825D94A8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D94A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D94A8);
}

extern "C" REX_FUNC(sub_825D94B8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D94B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D94B8);
}

extern "C" REX_FUNC(sub_825D94F0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D94F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D94F0);
}

extern "C" REX_FUNC(sub_825D9668) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9668(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9668);
}

extern "C" REX_FUNC(sub_825D9678) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9678(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9678);
}

extern "C" REX_FUNC(sub_825D96C0) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D96C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D96C0);
}

extern "C" REX_FUNC(sub_825D96E8) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D96E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D96E8);
}

extern "C" REX_FUNC(sub_825D9710) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9710(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9710);
}

extern "C" REX_FUNC(sub_825D9720) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9720(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9720);
}

extern "C" REX_FUNC(sub_825D9730) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9730(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9730);
}

extern "C" REX_FUNC(sub_825D9740) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9740(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9740);
}

extern "C" REX_FUNC(sub_825D9750) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9750(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9750);
}

extern "C" REX_FUNC(sub_825D9760) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9760(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9760);
}

extern "C" REX_FUNC(sub_825D9770) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9770(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9770);
}

extern "C" REX_FUNC(sub_825D9780) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9780(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9780);
}

extern "C" REX_FUNC(sub_825D9790) {
  if (!ITC_FLAG(native_lifted_025)) {
    __imp__sub_825D9790(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825D9790);
}
